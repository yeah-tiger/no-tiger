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
%type <std::unique_ptr<BlockItem>> block_item
%type <std::unique_ptr<BlockItemList>> block_item_list
%type <std::unique_ptr<Initializer>> initializer
%type <std::unique_ptr<Declaration>> declaration
%type <std::unique_ptr<ConstantExpression>> constant_expression
%type <std::unique_ptr<Expression>> expression primary_expression
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

argument_expression_list
      : assignment_expression
      | argument_expression_list ',' assignment_expression
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

postfix_expression
      : primary_expression
      {
        $$ = std::move($1);
        // TODO: no support for array, pointer, struct now
      }
      | postfix_expression '(' ')'
      | postfix_expression '(' argument_expression_list ')'
      ;

unary_expression
      : postfix_expression
      {
        $$ = std::move($1);
      }
      | unary_operator cast_expression
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
      | multiplicative_expression '/' cast_expression
      | multiplicative_expression '%' cast_expression
      ;


additive_expression
      : multiplicative_expression
      {
        $$ = std::move($1);
      }
      | additive_expression '+' multiplicative_expression
      | additive_expression '-' multiplicative_expression
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
      | relational_expression '>' shift_expression
      | relational_expression LE_OP shift_expression
      | relational_expression GE_OP shift_expression
      ;

equality_expression
      : relational_expression
      {
        $$ = std::move($1);
      }
      | equality_expression EQ_OP relational_expression
      | equality_expression NE_OP relational_expression
      ;

and_expression
      : equality_expression
      {
        $$ = std::move($1);
      }
      | and_expression '&' equality_expression
      ;

exclusive_or_expression
      : and_expression
      {
        $$ = std::move($1);
      }
      | exclusive_or_expression '^' and_expression
      ;

inclusive_or_expression
      : exclusive_or_expression
      {
        $$ = std::move($1);
      }
      | inclusive_or_expression '|' exclusive_or_expression
      ;

logic_and_expression
      : inclusive_or_expression
      {
        $$ = std::move($1);
      }
      | logic_and_expression AND_OP inclusive_or_expression
      ;


logic_or_expression
      : logic_and_expression
      {
        $$ = std::move($1);
      }
      | logical_or_expression OR_OP logic_and_expression
      {
        // binaryOp
      }
      ;

conditional_expression
      : logic_or_expression
      {
        $$ = std::move($1);
      }
      | logic_or_expression '?' expression ':' conditional_expression

assignment_expression
      : conditional_expression
      {
        $$ = std::move($1);
      }
      | unary_expression assignment_operator assignment_expression
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
        $$ = make_ast<ExpressionStatement>(nullptr);;
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
  std::cerr << loc << ": " << msg << std::endl;
  throw std::logic_error("Parser: invalid syntax");
}