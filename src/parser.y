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
  class TranslationUnit;
  class ExternalDecl;
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
%token INT FLOAT DOUBLE 
%token INTEGER REAL
%token END 0 "end of file"
%token RETURN

%type <int> INTEGER
%type <std::string> IDENTIFIER
%type <std::unique_ptr<TranslationUnit>> translation_unit
%type <std::unique_ptr<ExternalDecl>> external_declaration

%locations

%start start
%%
start
      : translation_unit END
      {
        driver.context().get_start() = std::move($1);
      }

/*** TODO: grammar rule here ***/
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

external_declaration
      : INTEGER
      {
        $$ = make_ast<ExternalDecl>($1);
      }
      ;


/*** for main only ***/
/***
function_definition
      : INT IDENTIFIER '(' ')' compound_statement
      ;

declaration_list
      : declaration
      | declaration_list declaration
      ;



statement_list
      : statement
      | statement_list statement
      ;

statement
      : return_statment
      : compound_statement
      ;


return_statment
      : RETURN expression ';'
      : RETURN ';'
      ;


compound_statement
      : '{' '}'
      | '{' statement_list '}'
      ;
***/
%%

void ntc::Parser::error(const location_type &loc, const std::string& msg) {
  driver.error(loc, msg);
}