#pragma once
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include "ast.hpp"
#include "visitor.hpp"
#include "config.hpp"

namespace ntc {
static llvm::LLVMContext llvm_context;

struct SymbolRecord {
  SymbolRecord(){};

  SymbolRecord(llvm::Value* _val, llvm::Type* _type, bool _is_const)
      : val(_val), is_const(_is_const), type(_type) {}
  llvm::Value* val;
  llvm::Type* type;
  bool is_const;
};

class SymbolTable {
 public:
  void push_table();

  void pop_table();

  bool find_symbol(const std::string& name);

  bool find_symbol_local(const std::string& name);

  void add_symbol(const std::string& name, llvm::Value* val, llvm::Type* type,
                  bool is_const);

  SymbolRecord* get_symbol(const std::string& name);

 private:
  std::deque<std::map<std::string, SymbolRecord>> table_stack_;
};

class CodeGenerator final : public IRVisitor {
 public:
  CodeGenerator(const std::string& module_id);

  virtual llvm::Value* visit(AST&) override;
  virtual llvm::Value* visit(BlockItem&) override;
  virtual llvm::Value* visit(ExternalDeclaration&) override;
  virtual llvm::Value* visit(TranslationUnit&) override;
  virtual llvm::Value* visit(FunctionDefinition&) override;
  virtual llvm::Value* visit(DeclarationSpecifier&) override;
  virtual llvm::Value* visit(Identifier&) override;
  virtual llvm::Value* visit(ParameterDeclaration&) override;
  virtual llvm::Value* visit(TypeSpecifier&) override;
  virtual llvm::Value* visit(Declaration&) override;
  virtual llvm::Value* visit(Initializer&) override;
  virtual llvm::Value* visit(Statement&) override;
  virtual llvm::Value* visit(CompoundStatement&) override;
  virtual llvm::Value* visit(ExpressionStatement&) override;
  virtual llvm::Value* visit(ReturnStatement&) override;
  virtual llvm::Value* visit(BreakStatement&) override;
  virtual llvm::Value* visit(ContinueStatement&) override;
  virtual llvm::Value* visit(IfStatement&) override;
  virtual llvm::Value* visit(WhileStatement&) override;
  virtual llvm::Value* visit(ForStatement&) override;
  virtual llvm::Value* visit(Expression&) override;
  virtual llvm::Value* visit(IntegerExpression&) override;
  virtual llvm::Value* visit(FloatExpression&) override;
  virtual llvm::Value* visit(BooleanExpression&) override;
  virtual llvm::Value* visit(CharacterExpression&) override;
  virtual llvm::Value* visit(StringLiteralExpression&) override;
  virtual llvm::Value* visit(BinaryOperationExpression&) override;
  virtual llvm::Value* visit(UnaryOperationExpression&) override;
  virtual llvm::Value* visit(ConditionalExpression&) override;
  virtual llvm::Value* visit(FunctionCall&) override;

  void output(const std::string& filename, ProgramMode mode);

 protected:
  std::unique_ptr<llvm::Module> module_;
  std::map<std::string, llvm::Value*> locals_;
  llvm::IRBuilder<> builder_;
  std::string module_id_;
  llvm::Type* cur_function_return_type_;
  std::string cur_function_name_;
  bool is_func_def;
  SymbolTable symbol_table_;

  llvm::Type* get_llvm_type(DeclarationSpecifier& declaration_specifier);

  llvm::Value* get_identifier_ptr(Identifier* identifier);

  bool get_const(DeclarationSpecifier& declaration_specifier);

  void codegen_error(const std::string& msg);

  void assignment_type_check(llvm::Type* lhs_type, llvm::Type* rhs_type, llvm::Value** rhs);

  llvm::Value* print_call(llvm::Value* arg, bool newline);

  llvm::Value* input_call(Expression& expr);

  void emit_code(llvm::raw_fd_ostream &fd, llvm::TargetMachine::CodeGenFileType type);
};
}  // namespace ntc
