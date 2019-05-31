#pragma once
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <memory>
#include <string>
#include <map>
#include "ast.hpp"
#include "visitor.hpp"
// TODO:
namespace ntc {
static llvm::LLVMContext llvm_context;
class CodeGenerator final : public IRVisitor {
 public:
  CodeGenerator(const std::string& module_id);

  virtual llvm::Value* visit( AST&) override;
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

  void output();
 protected:
  std::unique_ptr<llvm::Module> module_;
  std::map<std::string, llvm::Value*> locals_;
  llvm::IRBuilder<> builder_;
  std::string module_id_;
};
}  // namespace ntc
