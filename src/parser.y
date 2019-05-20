%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {ntc}
%define parser_class_name {Parser}
%define api.value.type variant
%locations

%code requires {
#include <iostream>
#include <string>
#include <memory>
#include <utility>
namespace ntc{
  class Driver;
  class Scanner;
  
  class AST;
  class ExternalDeclaration;
  class TranslationUnit;
  class FunctionDefinition;
  class DeclarationSpecifier;
  class Identifier;
  class ParameterDeclaration;
  class ParameterList;
  class TypeSpecifier;

  class Statement;
  class StatementList;
  class CompoundStatement;
  class ExpressionStatement;
  class ReturnStatement;

  class Expression;
  class ConstantExpression;
  class IntegerExpression;
  class FloatExpression;
  class BooleanExpression;

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
#include "ast.hpp"
#include "driver.hpp"
#undef yylex
#define yylex scanner.yylex
using namespace ntc;
}

%define parse.error verbose

%define parse.assert

%token IDENTIFIER
%token INT FLOAT DOUBLE SHORT LONG CHAR VOID BOOL SIGNED UNSIGNED
%token CONST
%token INTEGER REAL BOOLEAN
%token END 0 "end of file"
%token RETURN

%type <int> INTEGER
%type <double> REAL
%type <bool> BOOLEAN
%type <std::string> IDENTIFIER
%type <std::unique_ptr<TypeSpecifier>> type_specifier
%type <std::unique_ptr<DeclarationSpecifier>> declaration_specifiers
%type <std::unique_ptr<ParameterDeclaration>> parameter_declaration
%type <std::unique_ptr<ParameterList>> parameter_list
%type <std::unique_ptr<ConstantExpression>> constant_expression
%type <std::unique_ptr<Expression>> expression
%type <std::unique_ptr<ExpressionStatement>> expression_statement
%type <std::unique_ptr<ReturnStatement>> return_statment
%type <std::unique_ptr<CompoundStatement>> compound_statement
%type <std::unique_ptr<Statement>> statement
%type <std::unique_ptr<StatementList>> statement_list
%type <std::unique_ptr<FunctionDefinition>> function_definition
%type <std::unique_ptr<ExternalDeclaration>> external_declaration
%type <std::unique_ptr<TranslationUnit>> translation_unit
%locations

%start start
%%
start
      : translation_unit END
      {
        driver.context().get_start() = std::move($1);
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
        $$->add_parameter_declaration(std::move($3));
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
      ;

/*** 
primary_expression
      : constant_expression
      ;
TODO: comment out for now
replace constant for primary in expression
***/

expression
      : constant_expression
      {
        $$ = std::move($1);
      }
      ;

expression_statement
      : ';'
      {
        $$ = nullptr;
      }
      | expression ';'
      {
        $$ = make_ast<ExpressionStatement>(std::move($1));
      }

return_statment
      : RETURN expression ';'
      {
        $$ = make_ast<ReturnStatement>(std::move($2));
      }
      | RETURN ';'
      {
        $$ = make_ast<ReturnStatement>(nullptr);
      }
      ;

compound_statement
      : '{' '}'
      {
        $$ = make_ast<CompoundStatement>(nullptr);
      }
      | '{' statement_list '}'
      {
        $$ = make_ast<CompoundStatement>(std::move($2));
      }
      ;

statement
      : return_statment
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
      ;

statement_list
      : statement
      {
        $$ = make_ast<StatementList>(std::move($1));
      }
      | statement_list statement
      {
        $$ = std::move($1);
        $$->add_statement(std::move($2));
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
        $$ = make_ast<TranslationUnit>(std::move($1));
      }
      | translation_unit external_declaration
      {
        $$ = std::move($1);
        $$->add_external_declaration(std::move($2));
      }
      ;
%%

void ntc::Parser::error(const location_type &loc, const std::string& msg) {
  driver.error(loc, msg);
}