#include "printer.hpp"
#include <iostream>
namespace ntc {
Printer::Printer(std::ostream& _os) : os(_os), whitespace_cnt(0) {}

void Printer::indent(int cnt) { whitespace_cnt += cnt; }

void Printer::dedent(int cnt) { whitespace_cnt -= cnt; }

void Printer::output_space() { os << std::string(whitespace_cnt, ' '); }

void Printer::visit(AST& ast) { ast.accept(*this); }

void Printer::visit(BlockItem& block_item) { block_item.accept(*this); }

void Printer::visit(ExternalDeclaration& external_declaration) {
  external_declaration.accept(*this);
}

void Printer::visit(TranslationUnit& translation_unit) {
  output_space();
  os << "<TranslationUnit name=\"" << translation_unit.get_name() << "\">"
     << std::endl;
  indent();
  auto& decls = translation_unit.get_declarations();
  for (auto& decl : decls) {
    visit(*decl);
  }
  dedent();
  output_space();
  os << "</TranslationUnit>" << std::endl;
}

void Printer::visit(FunctionDefinition& function_definition) {
  output_space();
  os << "<FunctionDefinition>" << std::endl;
  indent();
  visit(*(function_definition.get_declaration_specifier()));
  visit(*(function_definition.get_identifier()));
  auto& parameter_list = function_definition.get_parameter_list();
  for (auto& parameter : parameter_list) {
    visit(*parameter);
  }
  visit(*(function_definition.get_compound_statement()));
  dedent();
  output_space();
  os << "</FunctionDefinition>" << std::endl;
}

void Printer::visit(DeclarationSpecifier& declaration_specifier) {
  output_space();
  os << "<DeclarationSpecifier const=\"" << std::boolalpha
     << declaration_specifier.get_is_const() << "\">" << std::endl;
  indent();
  auto& type_specifiers = declaration_specifier.get_type_specifiers();
  for (auto& type_specifier : type_specifiers) {
    visit(*type_specifier);
  }
  dedent();
  output_space();
  os << "</DeclarationSpecifier>" << std::endl;
}

void Printer::visit(Identifier& identifier) {
  output_space();
  os << "<Identifier "
     << "name=\"" << identifier.get_name() << "\">" << std::endl;
  output_space();
  os << "</Identifier>" << std::endl;
}

void Printer::visit(ParameterDeclaration& parameter_declaration) {
  output_space();
  os << "<ParameterDeclaration>" << std::endl;
  indent();
  visit(*(parameter_declaration.get_declaration_specifier()));
  visit(*(parameter_declaration.get_identifier()));
  dedent();
  output_space();
  os << "</ParameterDeclaration>" << std::endl;
}

void Printer::visit(TypeSpecifier& type_specifier) {
  output_space();
  os << "<TypeSpecifier "
     << "type=\"" << type::to_string(type_specifier.get_specifier()) << "\">"
     << std::endl;
  output_space();
  os << "</TypeSpecifier>" << std::endl;
}

void Printer::visit(Declaration& declaration) {
  output_space();
  os << "<Declaration>" << std::endl;
  indent();
  visit(*(declaration.get_type_specifier()));
  visit(*(declaration.get_identifier()));
  visit(*(declaration.get_initializer()));
  dedent();
  output_space();
  os << "</Declaration>" << std::endl;
}
void Printer::visit(Initializer& initializer) {
  output_space();
  os << "<Initializer>" << std::endl;
  indent();
  visit(*(initializer.get_expression()));
  dedent();
  output_space();
  os << "</Initializer>" << std::endl;
}

void Printer::visit(Statement& statement) { statement.accept(*this); }

void Printer::visit(CompoundStatement& compound_statement) {
  output_space();
  os << "<CompoundStatement>" << std::endl;
  indent();
  auto& block_item_list = compound_statement.get_block_item_list();
  for (auto& block_item : block_item_list) {
    visit(*block_item);
  }
  dedent();
  output_space();
  os << "</CompoundStatement>" << std::endl;
}

void Printer::visit(ExpressionStatement& expression_statement) {
  output_space();
  os << "<ExpressionStatement>" << std::endl;
  indent();
  auto& expression = expression_statement.get_expression();
  if (expression != nullptr) {
    visit(*expression);
  }
  dedent();
  output_space();
  os << "</ExpressionStatement>" << std::endl;
}

void Printer::visit(ReturnStatement& return_statement) {
  output_space();
  os << "<ReturnStatement>" << std::endl;
  indent();
  auto& expression = return_statement.get_expression();
  if (expression != nullptr) {
    visit(*expression);
  }
  dedent();
  output_space();
  os << "</ReturnStatement>" << std::endl;
}
void Printer::visit(BreakStatement& break_statement) {
  output_space();
  os << "<BreakStatement>" << std::endl;
  output_space();
  os << "</BreakStatement>" << std::endl;
}
void Printer::visit(ContinueStatement& continue_statement) {
  output_space();
  os << "<ContinueStatement>" << std::endl;
  output_space();
  os << "</ContinueStatement>" << std::endl;
}
void Printer::visit(IfStatement& if_statement) {
  output_space();
  os << "<IfStatement>" << std::endl;
  indent();
  visit(*(if_statement.get_if_expression()));
  visit(*(if_statement.get_then_statment()));
  auto& else_statement = if_statement.get_else_statement();
  if (else_statement != nullptr) {
    visit(*else_statement);
  }
  dedent();
  output_space();
  os << "</IfStatement>" << std::endl;
}
void Printer::visit(WhileStatement& while_statement) {
  output_space();
  os << "<WhileStatement>" << std::endl;
  indent();
  visit(*(while_statement.get_while_expression()));
  visit(*(while_statement.get_loop_statement()));
  dedent();
  output_space();
  os << "</WhileStatement>" << std::endl;
}
void Printer::visit(ForStatement& for_statement) {
  output_space();
  os << "<ForStatement>" << std::endl;
  indent();
  visit(*(for_statement.get_init_clause()));
  visit(*(for_statement.get_cond_expression()));
  auto& iteration_expression = for_statement.get_iteration_expression();
  if (iteration_expression != nullptr) {
    visit(*iteration_expression);
  }
  visit(*(for_statement.get_loop_statement()));
  dedent();
  output_space();
  os << "</ForStatement>" << std::endl;
}

void Printer::visit(Expression& expression) { expression.accept(*this); }

void Printer::visit(IntegerExpression& integer_expression) {
  output_space();
  os << "<IntegerExpression val=\"" << integer_expression.get_val() << "\">"
     << std::endl;
  output_space();
  os << "</IntegerExpression>" << std::endl;
}

void Printer::visit(FloatExpression& float_expression) {
  output_space();
  os << "<FloatExpression val=\"" << float_expression.get_val() << "\">"
     << std::endl;
  output_space();
  os << "</FloatExpression>" << std::endl;
}

void Printer::visit(BooleanExpression& boolean_expression) {
  output_space();
  os << "<BooleanExpression val=\"" << boolean_expression.get_val() << "\">"
     << std::endl;
  output_space();
  os << "</BooleanExpression>" << std::endl;
}

void Printer::visit(CharacterExpression& character_expression) {
  output_space();
  os << "<CharacterExpression val=\"" << character_expression.get_val() << "\">"
     << std::endl;
  output_space();
  os << "</CharacterExpression>" << std::endl;
}

void Printer::visit(StringLiteralExpression& string_literal_expression) {
  output_space();
  os << "<StringLiteralExpression val=\"" << string_literal_expression.get_val()
     << "\">" << std::endl;
  output_space();
  os << "</StringLiteralExpression>" << std::endl;
}

void Printer::visit(BinaryOperationExpression& binary_operation_expression) {
  output_space();
  os << "<BinaryOperationExpression op=\""
     << type::to_string(binary_operation_expression.get_op_type()) << "\">"
     << std::endl;
  indent();
  visit(*(binary_operation_expression.get_lhs()));
  visit(*(binary_operation_expression.get_rhs()));
  dedent();
  output_space();
  os << "</BinaryOperationExpression>" << std::endl;
}

void Printer::visit(UnaryOperationExpression& unary_operation_expression) {
  output_space();
  os << "<UnaryOperationExpression op=\"" << type::to_string(unary_operation_expression.get_op_type())<< "\">" << std::endl;
  indent();
  visit(*(unary_operation_expression.get_operand()));
  dedent();
  output_space();
  os << "</UnaryOperationExpression>" << std::endl;
}

void Printer::visit(ConditionalExpression& conditional_expression) {
  output_space();
  os << "<ConditionalExpression>" << std::endl;
  indent();
  visit(*(conditional_expression.get_cond_expression()));
  visit(*(conditional_expression.get_true_expression()));
  visit(*(conditional_expression.get_false_expression()));  
  dedent();
  output_space();
  os << "</ConditionalExpression>" << std::endl;
}

void Printer::visit(FunctionCall& function_call) {
  output_space();
  os << "<FunctionCall>" << std::endl;
  indent();
  visit(*(function_call.get_target()));
  auto& argument_list = function_call.get_argument_list();
  for (auto& argument : argument_list) {
    visit(*argument);
  }
  dedent();
  output_space();
  os << "</FunctionCall>" << std::endl;
}

}  // namespace ntc
