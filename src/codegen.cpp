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
    auto *ret = builder_.CreateAlloca(return_type);
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
  if (symbol_table_.find_symbol(identifier.get_name())) {
    return symbol_table_.get_symbol(identifier.get_name())->val;
  } else {
    codegen_error("variable \'" + identifier.get_name() +
                  "\' used before declared");
  }
  return nullptr;
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
    type_check(lhs_type, rhs_type, &value);
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

llvm::Value* CodeGenerator::visit(ExpressionStatement& expression_statement) { return nullptr; }

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
    type_check(lhs_type, rhs_type, &value);
    builder_.CreateStore(value, local);
  }
  return nullptr;
}
// TODO:
llvm::Value* CodeGenerator::visit(BreakStatement&) { return nullptr; }
// TODO:
llvm::Value* CodeGenerator::visit(ContinueStatement&) { return nullptr; }
// TODO:
llvm::Value* CodeGenerator::visit(IfStatement&) { return nullptr; }
// TODO:
llvm::Value* CodeGenerator::visit(WhileStatement&) { return nullptr; }
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
// TODO:
llvm::Value* CodeGenerator::visit(BinaryOperationExpression& expr) {
  // auto& lhs = expr.get_lhs();
  // auto op = expr.get_op_type();
  // auto& rhs = expr.get_rhs();
  // auto* lhs_val = lhs->accept(*this);
  // auto* rhs_val = rhs->accept(*this);
  
  
  return llvm::ConstantInt::getSigned(builder_.getInt32Ty(), 10);
}
// TODO:
llvm::Value* CodeGenerator::visit(UnaryOperationExpression&) {
  return llvm::ConstantInt::getSigned(builder_.getInt32Ty(), 10);
}

llvm::Value* CodeGenerator::visit(ConditionalExpression&) {
  assert(false);
  return nullptr;
}
// TODO:
llvm::Value* CodeGenerator::visit(FunctionCall&) {
  return llvm::ConstantInt::getSigned(builder_.getInt32Ty(), 10);
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

bool CodeGenerator::get_const(DeclarationSpecifier& declaration_specifier) {
  return declaration_specifier.get_is_const();
}

void CodeGenerator::codegen_error(const std::string& msg) {
  throw std::logic_error("Codegen: " + msg);
}

void CodeGenerator::type_check(llvm::Type* lhs_type, llvm::Type* rhs_type,
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
  codegen_error("type imcompatible");
}

}  // namespace ntc
