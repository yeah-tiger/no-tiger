#pragma once
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

class Visitor {
 public:
  virtual ~Visitor() {}
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
}  // namespace ntc
