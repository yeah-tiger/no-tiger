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
#include <stdexcept>
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

  // Statement
  class Statement;
  class StatementList;
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
%token INTEGER REAL BOOLEAN CHARACTER STRING_LITERAL
%token END 0 "end of file"
%token RETURN IF ELSE WHILE FOR BREAK CONTINUE

%type <int> INTEGER
%type <double> REAL
%type <bool> BOOLEAN
%type <std::string> IDENTIFIER CHARACTER STRING_LITERAL
%type <std::unique_ptr<TypeSpecifier>> type_specifier
%type <std::unique_ptr<DeclarationSpecifier>> declaration_specifiers
%type <std::unique_ptr<ParameterDeclaration>> parameter_declaration
%type <std::unique_ptr<ParameterList>> parameter_list
%type <std::unique_ptr<ConstantExpression>> constant_expression
%type <std::unique_ptr<Expression>> expression primary_expression
%type <std::unique_ptr<ExpressionStatement>> expression_statement
%type <std::unique_ptr<CompoundStatement>> compound_statement
%type <std::unique_ptr<JumpStatement>> jump_statement
%type <std::unique_ptr<SelectionStatement>> selection_statement
%type <std::unique_ptr<IterationStatement>> iteration_statement
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
        //std::cout << "start" << std::endl;
        driver.context().get_start() = std::move($1);
      }

type_specifier
      : INT
      {
        //std::cout << "type_specifier int" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::INT);
      }
      | FLOAT
      {
        //std::cout << "type_specifier float" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::FLOAT);
      }
      | DOUBLE
      {
        //std::cout << "type_specifier double" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::DOUBLE);
      }
      | CHAR
      {
        //std::cout << "type_specifier CHAR" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::CHAR);
      }
      | SHORT
      {
        //std::cout << "type_specifier SHORT" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::SHORT);
      }
      | LONG
      {
        //std::cout << "type_specifier LONG" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::LONG);
      }
      | VOID
      {
        //std::cout << "type_specifier VOID" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::VOID);
      }
      | UNSIGNED
      {
        //std::cout << "type_specifier UNSIGNED" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::UNSIGNED);
      }
      | SIGNED
      {
        //std::cout << "type_specifier SIGNED" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::SIGNED);
      }
      | BOOL
      {
        //std::cout << "type_specifier BOOL" << std::endl;
        $$ = make_ast<TypeSpecifier>(ntc::type::Specifier::BOOL);
      }
      ;

declaration_specifiers
      : type_specifier
      {
        //std::cout << "declaration_specifiers type_specifier" << std::endl;
        $$ = make_ast<DeclarationSpecifier>(std::move($1));
      }
      | type_specifier declaration_specifiers
      {
        //std::cout << "declaration_specifiers type_specifier and" << std::endl;
        $$ = std::move($2);
        $$->add_type_specifier(std::move($1));
      }
      | CONST
      {
        //std::cout << "declaration_specifiers CONST" << std::endl;
        $$ = make_ast<DeclarationSpecifier>(true);
      }
      | CONST declaration_specifiers
      {
        //std::cout << "declaration_specifiers CONST and " << std::endl;
        $$ = std::move($2);
        $$->set_const(true);
      }
      ;

parameter_declaration
      : declaration_specifiers IDENTIFIER
      {
        //std::cout << "parameter_declaration IDENTIFIER" << std::endl;
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<ParameterDeclaration>(std::move($1), std::move(identifier));
      }
      ;

parameter_list
      : parameter_declaration
      {
        //std::cout << "parameter_list parameter_declaration" << std::endl;
        $$ = make_ast<ParameterList>(std::move($1));
      }
      | parameter_list ',' parameter_declaration
      {
        //std::cout << "parameter_list and" << std::endl;
        $$ = std::move($1);
        $$->add_parameter_declaration(std::move($3));
      }
      ;

constant_expression
      : INTEGER
      {
        //std::cout << "constant_expression INTEGER" << std::endl;
        $$ = make_ast<IntegerExpression>($1);
      }
      | REAL
      {
        //std::cout << "constant_expression REAL" << std::endl;
        $$ = make_ast<FloatExpression>($1);
      }
      | BOOLEAN
      {
        //std::cout << "constant_expression BOOLEAN" << std::endl;
        $$ = make_ast<BooleanExpression>($1);
      }
      | CHARACTER
      {
        //std::cout << "constant_expression CHARACTER" << std::endl;
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
        //std::cout << "constant_expression STRING_LITERAL" << std::endl;
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

expression
      : primary_expression
      {
        //std::cout << "expression primary_expression" << std::endl;
        $$ = std::move($1);
      }
      ;

expression_statement
      : ';'
      {
        //std::cout << "expression_statement nullptr" << std::endl;
        $$ = make_ast<ExpressionStatement>(nullptr);;
      }
      | expression ';'
      {
        //std::cout << "expression_statement expression" << std::endl;
        $$ = make_ast<ExpressionStatement>(std::move($1));
      }
      ;

jump_statement
      : RETURN expression ';'
      {
        //std::cout << "return_statment expression" << std::endl;
        $$ = make_ast<ReturnStatement>(std::move($2));
      }
      | RETURN ';'
      {
        //std::cout << "return_statment nullptr" << std::endl;
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
        //std::cout << "compound_statement nullptr" << std::endl;
        $$ = make_ast<CompoundStatement>(nullptr);
      }
      | '{' statement_list '}'
      {
        //std::cout << "compound_statement statement_list" << std::endl;
        $$ = make_ast<CompoundStatement>(std::move($2));
      }
      ;

selection_statement
      : IF '(' expression ')' '{' statement '}'
      {
        $$ = make_ast<IfStatement>(std::move($3), std::move($6));
      }
      | IF '(' expression ')' '{' statement '}' ELSE '{' statement '}'
      {
        $$ = make_ast<IfStatement>(std::move($3), std::move($6), std::move($10));
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
      | FOR '(' expression_statement expression_statement expression ')' statement {
        $$ = make_ast<ForStatement>(std::move($3), std::move($4), std::move($7), std::move($5));
      }
      ;

statement
      : jump_statement
      {
        //std::cout << "statement return_statment" << std::endl;
        $$ = std::move($1);
      }
      | compound_statement
      {
        //std::cout << "statement compound_statement" << std::endl;
        $$ = std::move($1);
      }
      | expression_statement
      {
        //std::cout << "statement expression_statement" << std::endl;
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

statement_list
      : statement
      {
        //std::cout << "statement_list statement" << std::endl;
        $$ = make_ast<StatementList>(std::move($1));
      }
      | statement_list statement
      {
        //std::cout << "statement_list and" << std::endl;
        $$ = std::move($1);
        $$->add_statement(std::move($2));
      }
      ;

function_definition
      : declaration_specifiers IDENTIFIER '(' ')' compound_statement
      {
        //std::cout << "function def 1" << std::endl;
        auto identifier = make_ast<Identifier>($2);
        //std::cout << "function def 1 mid" << std::endl;
        $$ = make_ast<FunctionDefinition>(std::move($1), std::move(identifier), nullptr, std::move($5));
        //std::cout << "function def 1 finish" << std::endl;
      }
      | declaration_specifiers IDENTIFIER '(' parameter_list ')' compound_statement
      {
        //std::cout << "function def 2" << std::endl;
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<FunctionDefinition>(std::move($1), std::move(identifier), std::move($4), std::move($6));
      }
      | declaration_specifiers IDENTIFIER '(' VOID ')' compound_statement
      {
        //std::cout << "function def 3" << std::endl;
        auto identifier = make_ast<Identifier>($2);
        $$ = make_ast<FunctionDefinition>(std::move($1), std::move(identifier), nullptr, std::move($6));
      }
      ;

external_declaration
      : function_definition
      {
        
        //std::cout << "external_declaration def " << std::endl;
        $$ = std::move($1);
      }
      ;

translation_unit
      : external_declaration
      {
        //std::cout << "translation_unit external " << std::endl;
        $$ = make_ast<TranslationUnit>(std::move($1));
      }
      | translation_unit external_declaration
      {
        
        //std::cout << "translation_unit and " << std::endl;
        $$ = std::move($1);
        $$->add_external_declaration(std::move($2));
      }
      ;
%%

void ntc::Parser::error(const location_type &loc, const std::string& msg) {
  std::cerr << loc << ": " << msg << std::endl;
  throw std::logic_error("Parser: invalid syntax");
}