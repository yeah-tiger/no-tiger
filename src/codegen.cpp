#include "codegen.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include "type.hpp"
namespace ntc {

void SymbolTable::push_table() {
  table_stack_.push_back(std::map<std::string, SymbolRecord>());
}

void SymbolTable::pop_table() { table_stack_.pop_back(); }

bool SymbolTable::find_symbol(const std::string& name) {
  for (auto table_iter = table_stack_.rbegin();
       table_iter != table_stack_.rend(); ++table_iter) {
    auto search = table_iter->find(name);
    if (search != table_iter->end()) {
      return true;
    }
  }
  return false;
}

bool SymbolTable::find_symbol_local(const std::string& name) {
  if (table_stack_.empty()) {
    return false;
  } else {
    auto& last = table_stack_.back();
    auto search = last.find(name);
    if (search != last.end()) {
      return true;
    } else {
      return false;
    }
  }
}

void SymbolTable::add_symbol(const std::string& name, llvm::Value* val,
                             llvm::Type* type, bool is_const) {
  assert(find_symbol_local(name) == false);
  auto& cur_table = table_stack_.back();
  SymbolRecord to_be_add(val, type, is_const);
  cur_table[name] = to_be_add;
}

SymbolRecord* SymbolTable::get_symbol(const std::string& name) {
  for (auto table_iter = table_stack_.rbegin();
       table_iter != table_stack_.rend(); ++table_iter) {
    auto search = table_iter->find(name);
    if (search != table_iter->end()) {
      return &(search->second);
    }
  }
  return nullptr;
}

CodeGenerator::CodeGenerator(const std::string& module_id)
    : module_id_(module_id),
      module_(std::make_unique<llvm::Module>(module_id, llvm_context)),
      builder_(llvm::IRBuilder<>(llvm_context)) {}

llvm::Value* CodeGenerator::visit(AST& ast) { return ast.accept(*this); }

llvm::Value* CodeGenerator::visit(BlockItem& block_item) {
  return block_item.accept(*this);
}

llvm::Value* CodeGenerator::visit(ExternalDeclaration& external_declaration) {
  return external_declaration.accept(*this);
}

llvm::Value* CodeGenerator::visit(TranslationUnit& translation_unit) {
  auto& decls = translation_unit.get_declarations();
  for (auto& decl : decls) {
    visit(*decl);
  }
  return nullptr;
}

llvm::Value* CodeGenerator::visit(FunctionDefinition& function_definition) {
  symbol_table_.push_table();
  auto& declaration_specifier = function_definition.get_declaration_specifier();
  auto& identifier = function_definition.get_identifier();
  auto& parameter_list = function_definition.get_parameter_list();
  auto& compound_statment = function_definition.get_compound_statement();

  llvm::Type* return_type = get_llvm_type(*declaration_specifier);
  std::vector<llvm::Type*> parameter_types;
  std::vector<bool> parameter_consts;
  std::vector<std::string> parameter_names;
  for (auto& parameter : parameter_list) {
    auto& parameter_specifier = parameter->get_declaration_specifier();
    parameter_types.push_back(get_llvm_type(*parameter_specifier));
    parameter_consts.push_back(get_const(*parameter_specifier));
    parameter_names.push_back(parameter->get_identifier()->get_name());
  }
  auto* function_type =
      llvm::FunctionType::get(return_type, parameter_types, false);
  auto* function =
      llvm::Function::Create(function_type, llvm::Function::ExternalLinkage,
                             identifier->get_name(), module_.get());
  auto* block =
      llvm::BasicBlock::Create(module_->getContext(), "entry", function);
  builder_.SetInsertPoint(block);
  size_t index = 0;
  for (auto& arg : function->args()) {
    auto* local = builder_.CreateAlloca(arg.getType());
    symbol_table_.add_symbol(parameter_names[index], local,
                             parameter_types[index], parameter_consts[index]);
    builder_.CreateStore(&arg, local);
    ++index;
  }
  if (!return_type->isVoidTy()) {
    auto* ret = builder_.CreateAlloca(return_type);
    symbol_table_.add_symbol(identifier->get_name(), ret, return_type, false);
  }
  cur_function_name_ = identifier->get_name();
  cur_function_return_type_ = return_type;
  is_func_def = true;
  visit(*compound_statment);
  if (!return_type->isVoidTy()) {
    auto* val = symbol_table_.get_symbol(identifier->get_name())->val;
    auto* load = builder_.CreateLoad(val);
    builder_.CreateRet(load);
  } else {
    builder_.CreateRetVoid();
  }
  llvm::verifyFunction(*function);
  symbol_table_.pop_table();

  cur_function_name_ = "";
  cur_function_return_type_ = nullptr;
  return nullptr;
}

llvm::Value* CodeGenerator::visit(DeclarationSpecifier&) {
  assert(false);
  return nullptr;
}

llvm::Value* CodeGenerator::visit(Identifier& identifier) {
  return builder_.CreateLoad(get_identifier_ptr(&identifier));
}

llvm::Value* CodeGenerator::visit(ParameterDeclaration&) {
  assert(false);
  return nullptr;
}

llvm::Value* CodeGenerator::visit(TypeSpecifier&) {
  assert(false);
  return nullptr;
}

llvm::Value* CodeGenerator::visit(Declaration& declaration) {
  auto& declaration_speicifer = declaration.get_declaration_specifier();
  auto& identifier = declaration.get_identifier();
  auto& initializer = declaration.get_initializer();

  auto* type = get_llvm_type(*declaration_speicifer);
  bool is_const = get_const(*declaration_speicifer);
  auto* local = builder_.CreateAlloca(type);

  if (symbol_table_.find_symbol_local(identifier->get_name())) {
    codegen_error("varaible \'" + identifier->get_name() + "\' redeclared");
  }
  symbol_table_.add_symbol(identifier->get_name(), local, type, is_const);

  if (initializer != nullptr) {
    auto& expression = initializer->get_expression();
    assert(expression != nullptr);
    auto* value = expression->accept(*this);
    auto* lhs_type = local->getType()->getPointerElementType();
    auto* rhs_type = value->getType();
    if (lhs_type->isDoubleTy() && rhs_type->isIntegerTy(32)) {
      value = builder_.CreateSIToFP(value, builder_.getDoubleTy());
      rhs_type = value->getType();
    }
    assignment_type_check(lhs_type, rhs_type, &value);
    builder_.CreateStore(value, local);
  }
  return nullptr;
}

llvm::Value* CodeGenerator::visit(Initializer&) {
  assert(false);
  return nullptr;
}

llvm::Value* CodeGenerator::visit(Statement& statement) {
  return statement.accept(*this);
}

llvm::Value* CodeGenerator::visit(CompoundStatement& compound_statement) {
  bool is_func_def_ori = is_func_def;
  if (is_func_def_ori) {
    is_func_def = false;
  }
  if (!is_func_def_ori) {
    symbol_table_.push_table();
  }
  auto& block_item_list = compound_statement.get_block_item_list();
  for (auto& block_item : block_item_list) {
    block_item->accept(*this);
  }
  if (!is_func_def_ori) {
    symbol_table_.pop_table();
  }
  return nullptr;
}

llvm::Value* CodeGenerator::visit(ExpressionStatement& expression_statement) {
  auto& expr = expression_statement.get_expression();
  if (expr != nullptr) {
    expr->accept(*this);
  }
  return nullptr;
}

llvm::Value* CodeGenerator::visit(ReturnStatement& return_statement) {
  if (cur_function_return_type_ == nullptr) {
    codegen_error("invalid return statement");
  }
  auto& expr = return_statement.get_expression();
  auto* record = symbol_table_.get_symbol(cur_function_name_);
  if (expr == nullptr && record != nullptr) {
    codegen_error("empty return");
  } else if (expr != nullptr && record == nullptr) {
    codegen_error("return value in a void function");
  }
  if (expr != nullptr) {
    auto* value = expr->accept(*this);
    auto* local = record->val;
    auto* lhs_type = local->getType()->getPointerElementType();
    auto* rhs_type = value->getType();
    if (lhs_type->isDoubleTy() && rhs_type->isIntegerTy(32)) {
      value = builder_.CreateSIToFP(value, builder_.getDoubleTy());
      rhs_type = value->getType();
    }
    assignment_type_check(lhs_type, rhs_type, &value);
    builder_.CreateStore(value, local);
  }
  return nullptr;
}
// TODO:
llvm::Value* CodeGenerator::visit(BreakStatement&) { return nullptr; }
// TODO:
llvm::Value* CodeGenerator::visit(ContinueStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(IfStatement& statement) {
  auto& if_cond = statement.get_if_expression();
  auto& then_statement = statement.get_then_statment();
  auto& else_statement = statement.get_else_statement();
  auto* cond_val = if_cond->accept(*this);
  if (!cond_val->getType()->isIntegerTy(1)) {
    codegen_error(
        "type error: if statement needs boolean condition expression");
  }
  auto* function = builder_.GetInsertBlock()->getParent();
  if (function == nullptr) {
    codegen_error("invalid if statement");
  }
  auto* then_block =
      llvm::BasicBlock::Create(module_->getContext(), "then", function);
  auto* else_block = llvm::BasicBlock::Create(module_->getContext(), "else");
  auto* continue_block =
      llvm::BasicBlock::Create(module_->getContext(), "continue");
  builder_.CreateCondBr(cond_val, then_block, else_block);
  builder_.SetInsertPoint(then_block);
  then_statement->accept(*this);
  builder_.CreateBr(continue_block);

  function->getBasicBlockList().push_back(else_block);
  builder_.SetInsertPoint(else_block);
  if (else_statement != nullptr) {
    else_statement->accept(*this);
  }
  builder_.CreateBr(continue_block);

  function->getBasicBlockList().push_back(continue_block);
  builder_.SetInsertPoint(continue_block);
  return nullptr;
}

llvm::Value* CodeGenerator::visit(WhileStatement& statement) {
  auto& cond = statement.get_while_expression();
  auto& loop_statement = statement.get_loop_statement();

  auto* function = builder_.GetInsertBlock()->getParent();
  auto* while_block =
      llvm::BasicBlock::Create(module_->getContext(), "while", function);
  auto* loop_block = llvm::BasicBlock::Create(module_->getContext(), "loop", function);
  auto* continue_block =
      llvm::BasicBlock::Create(module_->getContext(), "continue");
  builder_.CreateBr(while_block);
  builder_.SetInsertPoint(while_block);
  auto* cond_val = cond->accept(*this);
  if (!cond_val->getType()->isIntegerTy(1)) {
    codegen_error(
        "type error: while statement needs boolean condition expression");
  }
  builder_.CreateCondBr(cond_val, loop_block, continue_block);
  
  builder_.SetInsertPoint(loop_block);
  loop_statement->accept(*this);
  builder_.CreateBr(while_block);

  function->getBasicBlockList().push_back(continue_block);
  builder_.SetInsertPoint(continue_block);
  return nullptr;
}
// TODO:
llvm::Value* CodeGenerator::visit(ForStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(Expression& expression) {
  return llvm::ConstantInt::getSigned(builder_.getInt32Ty(), 10);
}

llvm::Value* CodeGenerator::visit(IntegerExpression& expr) {
  return llvm::ConstantInt::getSigned(builder_.getInt32Ty(), expr.get_val());
}

llvm::Value* CodeGenerator::visit(FloatExpression& expr) {
  return llvm::ConstantFP::get(builder_.getDoubleTy(), expr.get_val());
}

llvm::Value* CodeGenerator::visit(BooleanExpression& expr) {
  return expr.get_val() ? builder_.getInt1(1) : builder_.getInt1(0);
}

llvm::Value* CodeGenerator::visit(CharacterExpression& expr) {
  return builder_.getInt8(static_cast<uint8_t>(expr.get_val()));
}

llvm::Value* CodeGenerator::visit(StringLiteralExpression& expr) {
  return builder_.CreateGlobalStringPtr(expr.get_val());
}

llvm::Value* CodeGenerator::visit(BinaryOperationExpression& expr) {
  auto& lhs = expr.get_lhs();
  auto op = expr.get_op_type();
  auto& rhs = expr.get_rhs();
  auto* rhs_val = rhs->accept(*this);
  // special handle assign
  if (op == type::BinaryOp::ASSIGN) {
    Identifier* identifier = dynamic_cast<Identifier*>(lhs.get());
    if (identifier == nullptr) {
      codegen_error("cannot assign value to rvalue");
    } else {
      auto* lhs_val = get_identifier_ptr(identifier);
      auto* lhs_type = lhs_val->getType()->getPointerElementType();
      auto* rhs_type = rhs_val->getType();
      if (lhs_type->isDoubleTy() && rhs_type->isIntegerTy(32)) {
        rhs_val = builder_.CreateSIToFP(rhs_val, builder_.getDoubleTy());
        rhs_type = rhs_val->getType();
      }
      assignment_type_check(lhs_type, rhs_type, &rhs_val);
      builder_.CreateStore(rhs_val, lhs_val);
      return rhs_val;
    }
  }

  // other op
  auto* lhs_val = lhs->accept(*this);
  auto* lhs_type = lhs_val->getType();
  auto* rhs_type = rhs_val->getType();

  // bool
  if (lhs_type->isIntegerTy(1) && rhs_type->isIntegerTy(1)) {
    llvm::CmpInst::Predicate cmp;
    switch (op) {
      case type::BinaryOp::EQUAL:
        cmp = llvm::CmpInst::ICMP_EQ;
        break;
      case type::BinaryOp::NOT_EQUAL:
        cmp = llvm::CmpInst::ICMP_NE;
        break;
      default:
        cmp = llvm::CmpInst::FCMP_FALSE;
    }
    if (cmp != llvm::CmpInst::FCMP_FALSE) {
      return builder_.CreateICmp(cmp, lhs_val, rhs_val);
    }
    llvm::Instruction::BinaryOps binop;
    switch (op) {
      case type::BinaryOp::LOGIC_AND:
        binop = llvm::Instruction::And;
        break;
      case type::BinaryOp::LOGIC_OR:
        binop = llvm::Instruction::Or;
        break;
      default:
        codegen_error("type error: boolean " + to_string(op) + " boolean");
    }
    return builder_.CreateBinOp(binop, lhs_val, rhs_val);
  }
  // char
  if (lhs_type->isIntegerTy(8) && rhs_type->isIntegerTy(8)) {
    llvm::CmpInst::Predicate cmp;
    switch (op) {
      case type::BinaryOp::LESS:
        cmp = llvm::CmpInst::ICMP_SLT;
        break;
      case type::BinaryOp::GREATER:
        cmp = llvm::CmpInst::ICMP_SGT;
        break;
      case type::BinaryOp::LESS_EQUAL:
        cmp = llvm::CmpInst::ICMP_SLE;
        break;
      case type::BinaryOp::GREATER_EQUAL:
        cmp = llvm::CmpInst::ICMP_SGE;
        break;
      case type::BinaryOp::EQUAL:
        cmp = llvm::CmpInst::ICMP_EQ;
        break;
      case type::BinaryOp::NOT_EQUAL:
        cmp = llvm::CmpInst::ICMP_NE;
        break;
      default:
        codegen_error("type error: char " + to_string(op) + " char");
    }
    return builder_.CreateICmp(cmp, lhs_val, rhs_val);
  }

  // string
  if (lhs_type->isPointerTy() || rhs_type->isPointerTy()) {
    codegen_error("no supported op other than assignment for string");
    return nullptr;
  }

  // fp
  if ((lhs_type->isFloatTy() || lhs_type->isDoubleTy()) ||
      (rhs_type->isFloatTy() || rhs_type->isDoubleTy())) {
    llvm::Value* lhs_val_tmp = lhs_val;
    llvm::Value* rhs_val_tmp = rhs_val;
    if (lhs_type->isFloatTy()) {
      lhs_val_tmp = builder_.CreateFPCast(lhs_val, builder_.getDoubleTy());
    } else if (lhs_type->isIntegerTy(16) || lhs_type->isIntegerTy(32) ||
               lhs_type->isIntegerTy(64)) {
      lhs_val_tmp = builder_.CreateSIToFP(lhs_val, builder_.getDoubleTy());
    } else if (lhs_type->isDoubleTy()) {
      ;
    } else {
      codegen_error("floating point arithmetic: type incompatible");
    }
    if (rhs_type->isFloatTy()) {
      rhs_val_tmp = builder_.CreateFPCast(rhs_val, builder_.getDoubleTy());
    } else if (rhs_type->isIntegerTy(16) || rhs_type->isIntegerTy(32) ||
               lhs_type->isIntegerTy(64)) {
      rhs_val_tmp = builder_.CreateSIToFP(rhs_val, builder_.getDoubleTy());
    } else if (rhs_type->isDoubleTy()) {
      ;
    } else {
      codegen_error("floating point arithmetic: type incompatible");
    }
    llvm::CmpInst::Predicate cmp;
    switch (op) {
      case type::BinaryOp::LESS:
        cmp = llvm::CmpInst::FCMP_OLT;
        break;
      case type::BinaryOp::GREATER:
        cmp = llvm::CmpInst::FCMP_OGT;
        break;
      case type::BinaryOp::LESS_EQUAL:
        cmp = llvm::CmpInst::FCMP_OLE;
        break;
      case type::BinaryOp::GREATER_EQUAL:
        cmp = llvm::CmpInst::FCMP_OGE;
        break;
      case type::BinaryOp::EQUAL:
        cmp = llvm::CmpInst::FCMP_OEQ;
        break;
      case type::BinaryOp::NOT_EQUAL:
        cmp = llvm::CmpInst::FCMP_ONE;
        break;
      default:
        cmp = llvm::CmpInst::FCMP_FALSE;
    }
    if (cmp != llvm::CmpInst::FCMP_FALSE) {
      return builder_.CreateFCmp(cmp, lhs_val_tmp, rhs_val_tmp);
    }
    llvm::Instruction::BinaryOps binop;
    switch (op) {
      case type::BinaryOp::ADD:
        binop = llvm::Instruction::FAdd;
        break;
      case type::BinaryOp::SUB:
        binop = llvm::Instruction::FSub;
        break;
      case type::BinaryOp::MUL:
        binop = llvm::Instruction::FMul;
        break;
      case type::BinaryOp::DIV:
        binop = llvm::Instruction::FDiv;
        break;
      default:
        codegen_error("floating point arithmetic: unsupported op: " +
                      to_string(op));
    }
    return builder_.CreateBinOp(binop, lhs_val_tmp, rhs_val_tmp);
  }

  // integer
  if ((lhs_type->isIntegerTy(16) || lhs_type->isIntegerTy(32) ||
       lhs_type->isIntegerTy(64)) &&
      (rhs_type->isIntegerTy(16) || rhs_type->isIntegerTy(32) ||
       rhs_type->isIntegerTy(64))) {
    llvm::Value* lhs_val_tmp = lhs_val;
    llvm::Value* rhs_val_tmp = rhs_val;
    if (lhs_type->isIntegerTy(64) || rhs_type->isIntegerTy(64)) {
      lhs_val_tmp =
          builder_.CreateIntCast(lhs_val, builder_.getInt64Ty(), true);
      rhs_val_tmp =
          builder_.CreateIntCast(rhs_val, builder_.getInt64Ty(), true);
    } else if (lhs_type->isIntegerTy(32) || rhs_type->isIntegerTy(32)) {
      lhs_val_tmp =
          builder_.CreateIntCast(lhs_val, builder_.getInt32Ty(), true);
      rhs_val_tmp =
          builder_.CreateIntCast(rhs_val, builder_.getInt32Ty(), true);
    }
    llvm::CmpInst::Predicate cmp;
    switch (op) {
      case type::BinaryOp::LESS:
        cmp = llvm::CmpInst::ICMP_SLT;
        break;
      case type::BinaryOp::GREATER:
        cmp = llvm::CmpInst::ICMP_SGT;
        break;
      case type::BinaryOp::LESS_EQUAL:
        cmp = llvm::CmpInst::ICMP_SLE;
        break;
      case type::BinaryOp::GREATER_EQUAL:
        cmp = llvm::CmpInst::ICMP_SGE;
        break;
      case type::BinaryOp::EQUAL:
        cmp = llvm::CmpInst::ICMP_EQ;
        break;
      case type::BinaryOp::NOT_EQUAL:
        cmp = llvm::CmpInst::ICMP_NE;
        break;
      default:
        cmp = llvm::CmpInst::FCMP_FALSE;
    }
    if (cmp != llvm::CmpInst::FCMP_FALSE) {
      return builder_.CreateICmp(cmp, lhs_val_tmp, rhs_val_tmp);
    }
    llvm::Instruction::BinaryOps binop;
    switch (op) {
      case type::BinaryOp::ADD:
        binop = llvm::Instruction::Add;
        break;
      case type::BinaryOp::SUB:
        binop = llvm::Instruction::Sub;
        break;
      case type::BinaryOp::MUL:
        binop = llvm::Instruction::Mul;
        break;
      case type::BinaryOp::DIV:
        binop = llvm::Instruction::SDiv;
        break;
      case type::BinaryOp::MOD:
        binop = llvm::Instruction::SRem;
        break;
      default:
        codegen_error("integer point arithmetic: unsupported op: " +
                      to_string(op));
    }
    return builder_.CreateBinOp(binop, lhs_val_tmp, rhs_val_tmp);
  }

  codegen_error("binary operation: type incompatible");
  return nullptr;
}

llvm::Value* CodeGenerator::visit(UnaryOperationExpression& expr) {
  auto& operand = expr.get_operand();
  auto op = expr.get_op_type();
  auto* val = operand->accept(*this);
  auto* type = val->getType();
  if (type->isPointerTy()) {
    codegen_error("unary operation: no supported op for string");
  } else if (type->isIntegerTy(8)) {
    codegen_error("unary operation: no supported op for char");
  } else if (type->isIntegerTy(1)) {
    if (op == type::UnaryOp::LOGIC_NOT) {
      return builder_.CreateNot(val);
    } else {
      codegen_error("unary operation: unsupported op: " + to_string(op) +
                    " for char");
    }
  } else if (type->isFloatTy() || type->isDoubleTy()) {
    switch (op) {
      case type::UnaryOp::POSITIVIZE:
        return val;
      case type::UnaryOp::NEGATE:
        return builder_.CreateNeg(val);
      default:
        codegen_error("unary operation: unsupported op: " + to_string(op) +
                      " for floating point");
    }
  } else if (type->isIntegerTy(16) || type->isIntegerTy(32) ||
             type->isIntegerTy(64)) {
    switch (op) {
      case type::UnaryOp::POSITIVIZE:
        return val;
      case type::UnaryOp::NEGATE:
        return builder_.CreateNeg(val);
      default:
        codegen_error("unary operation: unsupported op: " + to_string(op) +
                      " for integer");
    }
  }
  codegen_error("unary operation: type incompatible");
  return nullptr;
}

llvm::Value* CodeGenerator::visit(ConditionalExpression&) {
  assert(false);
  return nullptr;
}

llvm::Value* CodeGenerator::visit(FunctionCall& function_call) {
  auto& target = function_call.get_target();
  auto& argument_list = function_call.get_argument_list();
  Identifier* identifier = dynamic_cast<Identifier*>(target.get());
  std::vector<llvm::Value*> args;
  for (auto& arg : argument_list) {
    auto* val = arg->accept(*this);
    args.push_back(val);
  }
  if (identifier == nullptr) {
    codegen_error("cannot call on rvalue");
  }
  if (identifier->get_name() == "print") {
    if (args.size() < 1) {
      codegen_error("print: too few arguments");
    }
    if (args.size() > 1) {
      codegen_error("print: too many arguments");
    }
    return print_call(args[0], false);
  };
  if (identifier->get_name() == "println") {
    if (args.size() < 1) {
      codegen_error("println: too few arguments");
    }
    if (args.size() > 1) {
      codegen_error("println: too many arguments");
    }
    return print_call(args[0], true);
  };
  if (identifier->get_name() == "input") {
    if (args.size() < 1) {
      codegen_error("input: too few arguments");
    }
    if (args.size() > 1) {
      codegen_error("input: too many arguments");
    }
    return input_call(*(argument_list[0]));
  };

  auto* function = module_->getFunction(identifier->get_name());
  if (function == nullptr) {
    codegen_error("invalid function: " + identifier->get_name());
  }

  if (function->arg_size() != argument_list.size()) {
    codegen_error("invalid argument number: " + identifier->get_name());
  }
  return builder_.CreateCall(function, args);
}

void CodeGenerator::output(const std::string& filename) {
  std::error_code ec;
  llvm::raw_fd_ostream fd(filename, ec, llvm::sys::fs::F_None);
  module_->print(fd, nullptr);
}

llvm::Type* CodeGenerator::get_llvm_type(
    DeclarationSpecifier& declaration_specifier) {
  auto& type_specifier = declaration_specifier.get_type_specifier();
  switch (type_specifier->get_specifier()) {
    case type::Specifier::BOOL:
      return builder_.getInt1Ty();
    case type::Specifier::CHAR:
      return builder_.getInt8Ty();
    case type::Specifier::DOUBLE:
      return builder_.getDoubleTy();
    case type::Specifier::FLOAT:
      return builder_.getFloatTy();
    case type::Specifier::INT:
      return builder_.getInt32Ty();
    case type::Specifier::LONG:
      return builder_.getInt64Ty();
    case type::Specifier::SHORT:
      return builder_.getInt16Ty();
    case type::Specifier::VOID:
      return builder_.getVoidTy();
    case type::Specifier::STRING:
      return builder_.getInt8PtrTy();
    case type::Specifier::UNDEFINED:
    default:
      return nullptr;
  }
}

llvm::Value* CodeGenerator::get_identifier_ptr(Identifier* identifier) {
  if (symbol_table_.find_symbol(identifier->get_name())) {
    return symbol_table_.get_symbol(identifier->get_name())->val;
  } else {
    codegen_error("variable \'" + identifier->get_name() +
                  "\' used before declared");
  }
  return nullptr;
}

bool CodeGenerator::get_const(DeclarationSpecifier& declaration_specifier) {
  return declaration_specifier.get_is_const();
}

void CodeGenerator::codegen_error(const std::string& msg) {
  throw std::logic_error("Codegen: " + msg);
}

void CodeGenerator::assignment_type_check(llvm::Type* lhs_type,
                                          llvm::Type* rhs_type,
                                          llvm::Value** rhs) {
  // bool
  if (lhs_type->isIntegerTy(1) && rhs_type->isIntegerTy(1)) {
    return;
  } else if (lhs_type->isIntegerTy(8) && rhs_type->isIntegerTy(8)) {
    return;
  } else if (lhs_type->isPointerTy() && rhs_type->isPointerTy()) {
    return;
  } else if (lhs_type->isDoubleTy()) {
    if (rhs_type->isDoubleTy() || rhs_type->isFloatTy()) {
      *rhs = builder_.CreateFPCast(*rhs, builder_.getDoubleTy());
      return;
    }
  } else if (lhs_type->isFloatTy()) {
    if (rhs_type->isDoubleTy() || rhs_type->isFloatTy()) {
      *rhs = builder_.CreateFPCast(*rhs, builder_.getFloatTy());
      return;
    }
  } else if (lhs_type->isIntegerTy(16)) {
    if (rhs_type->isIntegerTy(16) || rhs_type->isIntegerTy(32) ||
        rhs_type->isIntegerTy(64)) {
      *rhs = builder_.CreateIntCast(*rhs, builder_.getInt16Ty(), true);
      return;
    }
  } else if (lhs_type->isIntegerTy(32)) {
    if (rhs_type->isIntegerTy(16) || rhs_type->isIntegerTy(32) ||
        rhs_type->isIntegerTy(64)) {
      *rhs = builder_.CreateIntCast(*rhs, builder_.getInt32Ty(), true);
      return;
    }
  } else if (lhs_type->isIntegerTy(64)) {
    if (rhs_type->isIntegerTy(16) || rhs_type->isIntegerTy(32) ||
        rhs_type->isIntegerTy(64)) {
      *rhs = builder_.CreateIntCast(*rhs, builder_.getInt64Ty(), true);
      return;
    }
  }
  codegen_error("type incompatible");
}

llvm::Value* CodeGenerator::print_call(llvm::Value* arg, bool new_line) {
  auto* char_ptr = builder_.getInt8Ty()->getPointerTo();
  auto* printf_type =
      llvm::FunctionType::get(builder_.getInt32Ty(), char_ptr, true);
  auto* printf_func = module_->getOrInsertFunction("printf", printf_type);
  std::string format_string;
  std::vector<llvm::Value*> parameters;
  parameters.resize(2);
  parameters[1] = arg;
  auto* type = arg->getType();
  if (type->isIntegerTy(8)) {
    format_string = "%c";
  } else if (type->isIntegerTy(1) || type->isIntegerTy(16) ||
             type->isIntegerTy(32) || type->isIntegerTy(64)) {
    format_string = "%d";
  } else if (type->isFloatTy() || type->isDoubleTy()) {
    format_string = "%lf";
  } else if (type->isPointerTy()) {
    format_string = "%s";
  } else {
    codegen_error("print: incompatible type");
  }
  if (new_line) {
    format_string += "\n";
  }
  StringLiteralExpression expr(format_string);
  parameters[0] = expr.accept(*this);
  return builder_.CreateCall(printf_func, parameters);
}

llvm::Value* CodeGenerator::input_call(Expression& expr) {
  Identifier* identifier = dynamic_cast<Identifier*>(&expr);
  if (identifier == nullptr) {
    codegen_error("cannot call input on rvalue");
  }
  auto* identifier_ptr = get_identifier_ptr(identifier);
  auto* char_ptr = builder_.getInt8Ty()->getPointerTo();
  auto* scanf_type =
      llvm::FunctionType::get(builder_.getInt32Ty(), char_ptr, true);
  auto* scanf_func = module_->getOrInsertFunction("scanf", scanf_type);
  std::string format_string;
  std::vector<llvm::Value*> parameters;
  parameters.resize(2);
  parameters[1] = identifier_ptr;
  auto* type = identifier_ptr->getType()->getPointerElementType();
  if (type->isIntegerTy(8)) {
    format_string = "%c";
  } else if (type->isIntegerTy(1) || type->isIntegerTy(16) ||
             type->isIntegerTy(32) || type->isIntegerTy(64)) {
    format_string = "%d";
  } else if (type->isFloatTy() || type->isDoubleTy()) {
    format_string = "%lf";
  } else {
    codegen_error("input: incompatible type");
  }
  StringLiteralExpression tmp_expr(format_string);
  parameters[0] = tmp_expr.accept(*this);
  return builder_.CreateCall(scanf_func, parameters);
}

}  // namespace ntc
