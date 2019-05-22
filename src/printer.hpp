#pragma once
#include <ostream>
#include "ast.hpp"
#include "visitor.hpp"
namespace ntc {
class Printer final : public Visitor {
 public:
  Printer(std::ostream& _os);
  virtual void visit(TranslationUnit& translation_unit) override;

  virtual void visit(FunctionDefinition& function_definition) override;

  virtual void visit(DeclarationSpecifier& declaration_specifier) override;

  virtual void visit(Identifier& identifier) override;

  virtual void visit(ParameterDeclaration& parameter_declaration) override;

  virtual void visit(TypeSpecifier& type_specifier) override;

  virtual void visit(Declaration& declaration);

  virtual void visit(Initializer& initializer);

  virtual void visit(CompoundStatement& compound_statement) override;

  virtual void visit(ExpressionStatement& expression_statement) override;

  virtual void visit(ReturnStatement& return_statement) override;

  virtual void visit(BreakStatement& break_statement) override;

  virtual void visit(ContinueStatement& continue_statement) override;

  virtual void visit(IfStatement& if_statement) override;

  virtual void visit(WhileStatement& while_statement) override;

  virtual void visit(ForStatement& for_statement) override;

  virtual void visit(IntegerExpression& integer_expression) override;

  virtual void visit(FloatExpression& float_expression) override;

  virtual void visit(BooleanExpression& boolean_expression) override;

  virtual void visit(CharacterExpression& character_expression) override;

  virtual void visit(
      StringLiteralExpression& string_literal_expression) override;
  
  virtual void visit(BinaryOperationExpression& binary_operation_expression);

  virtual void visit(UnaryOperationExpression& unary_operation_expression);

  virtual void visit(ConditionalExpression& conditional_expression);

  virtual void visit(FunctionCall& function_call);

 private:
  void indent(int cnt = 2);
  void dedent(int cnt = 2);
  void output_space();

  std::ostream& os;
  int whitespace_cnt;
};
}  // namespace ntc
