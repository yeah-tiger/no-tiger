#include "codegen.hpp"
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
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

void SymbolTable::add_symbol(const std::string& name, llvm::Value* val,
                             bool is_const) {
  assert(find_symbol(name) == false);
  auto& cur_table = table_stack_.back();
  SymbolRecord to_be_add(val, is_const);
  cur_table[name] = to_be_add;
}

SymbolRecord* SymbolTable::get_symbol(const std::string& name) {
  assert(find_symbol(name) == true);
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
  auto& declaration_specifier = function_definition.get_declaration_specifier();
  auto& identifier = function_definition.get_identifier();
  auto& parameter_list = function_definition.get_parameter_list();
  auto& compound_statment = function_definition.get_compound_statement();

  // step 1: get type

  return nullptr;
}

llvm::Value* CodeGenerator::visit(DeclarationSpecifier&) { return nullptr; }

llvm::Value* CodeGenerator::visit(Identifier&) { return nullptr; }

llvm::Value* CodeGenerator::visit(ParameterDeclaration&) { return nullptr; }

llvm::Value* CodeGenerator::visit(TypeSpecifier&) { return nullptr; }

llvm::Value* CodeGenerator::visit(Declaration&) { return nullptr; }

llvm::Value* CodeGenerator::visit(Initializer&) { return nullptr; }

llvm::Value* CodeGenerator::visit(Statement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(CompoundStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(ExpressionStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(ReturnStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(BreakStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(ContinueStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(IfStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(WhileStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(ForStatement&) { return nullptr; }

llvm::Value* CodeGenerator::visit(Expression&) { return nullptr; }

llvm::Value* CodeGenerator::visit(IntegerExpression&) { return nullptr; }

llvm::Value* CodeGenerator::visit(FloatExpression&) { return nullptr; }

llvm::Value* CodeGenerator::visit(BooleanExpression&) { return nullptr; }

llvm::Value* CodeGenerator::visit(CharacterExpression&) { return nullptr; }

llvm::Value* CodeGenerator::visit(StringLiteralExpression&) { return nullptr; }

llvm::Value* CodeGenerator::visit(BinaryOperationExpression&) {
  return nullptr;
}
llvm::Value* CodeGenerator::visit(UnaryOperationExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(ConditionalExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(FunctionCall&) { return nullptr; }

void CodeGenerator::output(const std::string& filename) {
  std::error_code ec;
  std::cout << "Codegen::output: " << module_id_ << std::endl;
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

}  // namespace ntc
