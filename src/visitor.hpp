#pragma once
#include <llvm/IR/Value.h>
namespace ntc {
class AST;
class BlockItem;
class ExternalDeclaration;
class TranslationUnit;
class FunctionDefinition;
class DeclarationSpecifier;
class Identifier;
class ParameterDeclaration;
class TypeSpecifier;
class Declaration;
class Initializer;

class Statement;
class CompoundStatement;
class ExpressionStatement;
class ReturnStatement;
class BreakStatement;
class ContinueStatement;
class IfStatement;
class WhileStatement;
class ForStatement;

class Expression;
class IntegerExpression;
class FloatExpression;
class BooleanExpression;
class CharacterExpression;
class StringLiteralExpression;
class BinaryOperationExpression;
class UnaryOperationExpression;
class ConditionalExpression;
class FunctionCall;

class ASTVisitor {
 public:
  virtual ~ASTVisitor() {}
  virtual void visit(AST&) = 0;
  virtual void visit(BlockItem&) = 0;
  virtual void visit(ExternalDeclaration&) = 0;
  virtual void visit(TranslationUnit&) = 0;
  virtual void visit(FunctionDefinition&) = 0;
  virtual void visit(DeclarationSpecifier&) = 0;
  virtual void visit(Identifier&) = 0;
  virtual void visit(ParameterDeclaration&) = 0;
  virtual void visit(TypeSpecifier&) = 0;
  virtual void visit(Declaration&) = 0;
  virtual void visit(Initializer&) = 0;

  virtual void visit(Statement&) = 0;
  virtual void visit(CompoundStatement&) = 0;
  virtual void visit(ExpressionStatement&) = 0;
  virtual void visit(ReturnStatement&) = 0;
  virtual void visit(BreakStatement&) = 0;
  virtual void visit(ContinueStatement&) = 0;
  virtual void visit(IfStatement&) = 0;
  virtual void visit(WhileStatement&) = 0;
  virtual void visit(ForStatement&) = 0;

  virtual void visit(Expression&) = 0;
  virtual void visit(IntegerExpression&) = 0;
  virtual void visit(FloatExpression&) = 0;
  virtual void visit(BooleanExpression&) = 0;
  virtual void visit(CharacterExpression&) = 0;
  virtual void visit(StringLiteralExpression&) = 0;
  virtual void visit(BinaryOperationExpression&) = 0;
  virtual void visit(UnaryOperationExpression&) = 0;
  virtual void visit(ConditionalExpression&) = 0;
  virtual void visit(FunctionCall&) = 0;
};

class IRVisitor {
 public:
  virtual ~IRVisitor() {}
  virtual llvm::Value* visit(AST&) = 0;
  virtual llvm::Value* visit(BlockItem&) = 0;
  virtual llvm::Value* visit(ExternalDeclaration&) = 0;
  virtual llvm::Value* visit(TranslationUnit&) = 0;
  virtual llvm::Value* visit(FunctionDefinition&) = 0;
  virtual llvm::Value* visit(DeclarationSpecifier&) = 0;
  virtual llvm::Value* visit(Identifier&) = 0;
  virtual llvm::Value* visit(ParameterDeclaration&) = 0;
  virtual llvm::Value* visit(TypeSpecifier&) = 0;
  virtual llvm::Value* visit(Declaration&) = 0;
  virtual llvm::Value* visit(Initializer&) = 0;
  virtual llvm::Value* visit(Statement&) = 0;
  virtual llvm::Value* visit(CompoundStatement&) = 0;
  virtual llvm::Value* visit(ExpressionStatement&) = 0;
  virtual llvm::Value* visit(ReturnStatement&) = 0;
  virtual llvm::Value* visit(BreakStatement&) = 0;
  virtual llvm::Value* visit(ContinueStatement&) = 0;
  virtual llvm::Value* visit(IfStatement&) = 0;
  virtual llvm::Value* visit(WhileStatement&) = 0;
  virtual llvm::Value* visit(ForStatement&) = 0;
  virtual llvm::Value* visit(Expression&) = 0;
  virtual llvm::Value* visit(IntegerExpression&) = 0;
  virtual llvm::Value* visit(FloatExpression&) = 0;
  virtual llvm::Value* visit(BooleanExpression&) = 0;
  virtual llvm::Value* visit(CharacterExpression&) = 0;
  virtual llvm::Value* visit(StringLiteralExpression&) = 0;
  virtual llvm::Value* visit(BinaryOperationExpression&) = 0;
  virtual llvm::Value* visit(UnaryOperationExpression&) = 0;
  virtual llvm::Value* visit(ConditionalExpression&) = 0;
  virtual llvm::Value* visit(FunctionCall&) = 0;
};
}  // namespace ntc
