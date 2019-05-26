%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {ntc}
%define parser_class_name {Parser}
%define api.value.type variant
%locations

%code requires {
#include "ast.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>
namespace ntc{
  class Driver;
  class Scanner;
  
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

  // Statement
  class Statement;
  class CompoundStatement;
  class ExpressionStatement;
  class JumpStatement;
  class ReturnStatement;
  class BreakStatement;
  class ContinueStatement;
  class SelectionStatement;
  class IfStatement;
  class IterationStatement;
  class WhileStatement;
  class ForStatement;

  // Expression
  class Expression;
  class ConstantExpression;
  class IntegerExpression;
  class FloatExpression;
  class BooleanExpression;
  class CharacterExpression;
  class StringLiteralExpression;
  class BinaryOperationExpression;
  class UnaryOperationExpression;
  class ConditionalExpression;
  class FunctionCall;

  template <typename T> class ASTList;
  
  using BlockItemList = ASTList<BlockItem>;
  using ParameterList = ASTList<ParameterDeclaration>;
  using ArgumentList = ASTList<Expression>;
}
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { Scanner& scanner }
%parse-param { Driver& driver }


%code {
#include "driver.hpp"
#include "type.hpp"
#undef yylex
#define yylex scanner.yylex
using namespace ntc;
}

%define parse.error verbose

%define parse.assert

%token IDENTIFIER
%token INT FLOAT DOUBLE SHORT LONG CHAR VOID BOOL SIGNED UNSIGNED
%token CONST
%token INTEGER REAL BOOLEAN CHARACTER STRING_LITERAL
%token END 0 "end of file"
%token RETURN IF ELSE WHILE FOR BREAK CONTINUE
%token AND_OP OR_OP LE_OP GE_OP NE_OP EQ_OP

%type <int> INTEGER
%type <double> REAL
%type <bool> BOOLEAN
%type <std::string> IDENTIFIER CHARACTER STRING_LITERAL
%type <std::unique_ptr<TypeSpecifier>> type_specifier
%type <std::unique_ptr<DeclarationSpecifier>> declaration_specifiers
%type <std::unique_ptr<ParameterDeclaration>> parameter_declaration
%type <std::unique_ptr<ParameterList>> parameter_list
%type <std::unique_ptr<BlockItem>> block_item
%type <std::unique_ptr<BlockItemList>> block_item_list
%type <std::unique_ptr<Initializer>> initializer
%type <std::unique_ptr<Declaration>> declaration
%type <std::unique_ptr<ArgumentList>> argument_expression_list
%type <std::unique_ptr<ConstantExpression>> constant_expression
%type <std::unique_ptr<Expression>> expression primary_expression postfix_expression unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression
%type <std::unique_ptr<ExpressionStatement>> expression_statement
%type <std::unique_ptr<CompoundStatement>> compound_statement
%type <std::unique_ptr<JumpStatement>> jump_statement
%type <std::unique_ptr<SelectionStatement>> selection_statement
%type <std::unique_ptr<IterationStatement>> iteration_statement
%type <std::unique_ptr<Statement>> statement
%type <std::unique_ptr<FunctionDefinition>> function_definition
%type <std::unique_ptr<ExternalDeclaration>> external_declaration
%type <std::unique_ptr<TranslationUnit>> translation_unit
%locations

%start start
%%
start
      : translation_unit END
      {
        driver.get_context().get_program() = std::move($1);
      }

type_specifier
      : INT
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::INT);
      }
      | FLOAT
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::FLOAT);
      }
      | DOUBLE
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::DOUBLE);
      }
      | CHAR
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::CHAR);
      }
      | SHORT
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::SHORT);
      }
      | LONG
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::LONG);
      }
      | VOID
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::VOID);
      }
      | UNSIGNED
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::UNSIGNED);
      }
      | SIGNED
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::SIGNED);
      }
      | BOOL
      {
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::BOOL);
      }
      ;

declaration_specifiers
      : type_specifier
      {
        $$ = make_ast<DeclarationSpecifier>(std::move($1));
      }
      | type_specifier declaration_specifiers
      {
        $$ = std::move($2);
        $$->add_type_specifier(std::move($1));
      }
      | CONST
      {
        $$ = make_ast<DeclarationSpecifier>(true);
      }
      | CONST declaration_specifiers
      {
        $$ = std::move($2);
        $$->set_const(true);
      }
      ;

parameter_declaration
      : declaration_specifiers IDENTIFIER
      {
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<ParameterDeclaration>(std::move($1), std::move(identifier));
      }
      ;

parameter_list
      : parameter_declaration
      {
        $$ = make_ast<ParameterList>(std::move($1));
      }
      | parameter_list ',' parameter_declaration
      {
        $$ = std::move($1);
        $$->add_item(std::move($3));
      }
      ;

constant_expression
      : INTEGER
      {
        $$ = make_ast<IntegerExpression>($1);
      }
      | REAL
      {
        $$ = make_ast<FloatExpression>($1);
      }
      | BOOLEAN
      {
        $$ = make_ast<BooleanExpression>($1);
      }
      | CHARACTER
      {
        if (CharacterExpression::check_character($1)) {
          $$ = make_ast<CharacterExpression>($1[0]);
        } else if ($1.length() == 0) {
          error(@1, "empty character constant");
        } else {
          error(@1, "multi-character character constant");
        }
      }
      | STRING_LITERAL
      {
        $$ = make_ast<StringLiteralExpression>($1);
      }
      ;


primary_expression
      : constant_expression
      {
        $$ = std::move($1);
      }
      | IDENTIFIER
      {
        auto identifier = make_ast<Identifier>($1);
        $$ = std::move(identifier);
      }
      | '(' expression ')'
      {
        $$ = std::move($2);
      }
      ;

argument_expression_list
      : assignment_expression
      {
        $$ = make_ast<ArgumentList>(std::move($1));
      }
      | argument_expression_list ',' assignment_expression
      {
        $$ = std::move($1);
        $$->add_item(std::move($3));
      }
      ;

postfix_expression
      : primary_expression
      {
        $$ = std::move($1);
        // TODO: no support for array, pointer, struct now
      }
      | postfix_expression '(' ')'
      {
        $$ = make_ast<FunctionCall>(std::move($1), nullptr);
      }
      | postfix_expression '(' argument_expression_list ')'
      {
        $$ = make_ast<FunctionCall>(std::move($1), std::move($3));
      }
      ;

unary_expression
      : postfix_expression
      {
        $$ = std::move($1);
      }
      | '+' cast_expression
      {
        $$ = make_ast<UnaryOperationExpression>(ntc::type::UnaryOp::POSITIVIZE, std::move($2));
      }
      | '-' cast_expression
      {
        $$ = make_ast<UnaryOperationExpression>(ntc::type::UnaryOp::NEGATE, std::move($2));
      }
      | '!' cast_expression
      {
        $$ = make_ast<UnaryOperationExpression>(ntc::type::UnaryOp::LOGIC_NOT, std::move($2));
      }
      ;

cast_expression
      : unary_expression
      {
        $$ = std::move($1);
        // TODO: no support for cast now
      }
      ;

multiplicative_expression
      : cast_expression
      {
        $$ = std::move($1);
      }
      | multiplicative_expression '*' cast_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::MUL, std::move($1), std::move($3));
      }
      | multiplicative_expression '/' cast_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::DIV, std::move($1), std::move($3));
      }
      | multiplicative_expression '%' cast_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::MOD, std::move($1), std::move($3));
      }
      ;


additive_expression
      : multiplicative_expression
      {
        $$ = std::move($1);
      }
      | additive_expression '+' multiplicative_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::ADD, std::move($1), std::move($3));
      }
      | additive_expression '-' multiplicative_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::SUB, std::move($1), std::move($3));
      }
      ;

shift_expression
      : additive_expression
      {
        $$ = std::move($1);
        // TODO: no support for bitshift now
      }
      ;

relational_expression
      : shift_expression 
      {
        $$ = std::move($1);
      }
      | relational_expression '<' shift_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::LESS, std::move($1), std::move($3));
      }
      | relational_expression '>' shift_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::GREATER, std::move($1), std::move($3));
      }
      | relational_expression LE_OP shift_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::LESS_EQUAL, std::move($1), std::move($3));
      }
      | relational_expression GE_OP shift_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::GREATER_EQUAL, std::move($1), std::move($3));
      }
      ;

equality_expression
      : relational_expression
      {
        $$ = std::move($1);
      }
      | equality_expression EQ_OP relational_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::EQUAL, std::move($1), std::move($3));
      }
      | equality_expression NE_OP relational_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::NOT_EQUAL, std::move($1), std::move($3));
      }
      ;

and_expression
      : equality_expression
      {
        $$ = std::move($1);
      }
      ;

exclusive_or_expression
      : and_expression
      {
        $$ = std::move($1);
      }
      ;

inclusive_or_expression
      : exclusive_or_expression
      {
        $$ = std::move($1);
      }
      ;

logical_and_expression
      : inclusive_or_expression
      {
        $$ = std::move($1);
      }
      | logical_and_expression AND_OP inclusive_or_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::LOGIC_AND, std::move($1), std::move($3));
      }
      ;


logical_or_expression
      : logical_and_expression
      {
        $$ = std::move($1);
      }
      | logical_or_expression OR_OP logical_and_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::LOGIC_OR, std::move($1), std::move($3));
      }
      ;

conditional_expression
      : logical_or_expression
      {
        $$ = std::move($1);
      }
      | logical_or_expression '?' expression ':' conditional_expression
      {
        $$ = make_ast<ConditionalExpression>(std::move($1), std::move($3), std::move($5));
      }
      ;

assignment_expression
      : conditional_expression
      {
        $$ = std::move($1);
      }
      | unary_expression '=' assignment_expression
      {
        $$ = make_ast<BinaryOperationExpression>(ntc::type::BinaryOp::ASSIGN, std::move($1), std::move($3));
      }
      ;


expression
      : assignment_expression
      {
        $$ = std::move($1);
      }
      ;

expression_statement
      : ';'
      {
        $$ = make_ast<ExpressionStatement>(nullptr);
      }
      | expression ';'
      {
        $$ = make_ast<ExpressionStatement>(std::move($1));
      }
      ;

jump_statement
      : RETURN expression ';'
      {
        $$ = make_ast<ReturnStatement>(std::move($2));
      }
      | RETURN ';'
      {
        $$ = make_ast<ReturnStatement>();
      }
      | BREAK ';'
      {
        $$ = make_ast<BreakStatement>();
      }
      | CONTINUE ';'
      {
        $$ = make_ast<ContinueStatement>();
      }
      ;

compound_statement
      : '{' '}'
      {
        $$ = make_ast<CompoundStatement>(nullptr);
      }
      | '{' block_item_list '}'
      {
        $$ = make_ast<CompoundStatement>(std::move($2)); 
      }
      ;

selection_statement
      : IF '(' expression ')' compound_statement
      {
        $$ = make_ast<IfStatement>(std::move($3), std::move($5));
      }
      | IF '(' expression ')' compound_statement ELSE statement
      {
        $$ = make_ast<IfStatement>(std::move($3), std::move($5), std::move($7));
      }
      ;

iteration_statement
      : WHILE '(' expression ')' statement
      {
        $$ = make_ast<WhileStatement>(std::move($3), std::move($5));
      }
      | FOR '(' expression_statement expression_statement ')' statement
      {
        $$ = make_ast<ForStatement>(std::move($3), std::move($4), std::move($6));
      }
      | FOR '(' expression_statement expression_statement expression ')' statement 
      {
        $$ = make_ast<ForStatement>(std::move($3), std::move($4), std::move($7), std::move($5));
      }
      ;

statement
      : jump_statement
      {
        $$ = std::move($1);
      }
      | compound_statement
      {
        $$ = std::move($1);
      }
      | expression_statement
      {
        $$ = std::move($1);
      }
      | selection_statement
      {
        $$ = std::move($1);
      }
      | iteration_statement
      {
        $$ = std::move($1);
      }
      ;


initializer
      : expression
      {
        $$ = make_ast<Initializer>(std::move($1));
      }
      ;

declaration
      : type_specifier IDENTIFIER ';'
      {
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<Declaration>(std::move($1), std::move(identifier));
      }
      | type_specifier IDENTIFIER '=' initializer ';'
      {
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<Declaration>(std::move($1), std::move(identifier), std::move($4));
      }
      ;

block_item
      : declaration
      {
        $$ = std::move($1);
      }
      | statement
      {
        $$ = std::move($1);
      }
      ;

block_item_list
      : block_item
      {
        $$ = make_ast<BlockItemList>(std::move($1));
      }
      | block_item_list block_item
      {
        $$ = std::move($1);
        $$->add_item(std::move($2));
      }
      ;

function_definition
      : declaration_specifiers IDENTIFIER '(' ')' compound_statement
      {
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<FunctionDefinition>(std::move($1), std::move(identifier), nullptr, std::move($5));
      }
      | declaration_specifiers IDENTIFIER '(' parameter_list ')' compound_statement
      {
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<FunctionDefinition>(std::move($1), std::move(identifier), std::move($4), std::move($6));
      }
      | declaration_specifiers IDENTIFIER '(' VOID ')' compound_statement
      {
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<FunctionDefinition>(std::move($1), std::move(identifier), nullptr, std::move($6));
      }
      ;

external_declaration
      : function_definition
      {
        $$ = std::move($1);
      }
      ;

translation_unit
      : external_declaration
      {
        $$ = make_ast<TranslationUnit>(std::move($1), driver.get_context().get_name());
      }
      | translation_unit external_declaration
      {
        $$ = std::move($1);
        $$->add_external_declaration(std::move($2));
      }
      ;
%%

void ntc::Parser::error(const location_type &loc, const std::string& msg) {
  std::cerr << loc << ": " << msg << std::endl;
  throw std::logic_error("Parser: invalid syntax");
}