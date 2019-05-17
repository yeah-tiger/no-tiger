%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {ntc}
%define parser_class_name {Parser}
%define api.value.type variant
%locations

%code requires {
namespace ntc{
  class Driver;
  class Scanner;
  class CalcNode;
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

%start start


%code {
#include <iostream>
#include <string>
#include "context.hpp"
#include "driver.hpp"

#undef yylex
#define yylex scanner.yylex
using namespace ntc;
}

%define parse.error verbose

%define parse.assert

%token ADD SUB MUL DIV
%token IDENTIFIER
%token INTEGER REAL
%token SEMI
%token LPARENT RPARENT LCURLY RCURLY
%token END 0 "end of file"

%type <int> INTEGER
%type <std::string> IDENTIFIER
%type <double> REAL

%locations

%% 
/*** TODO: grammar rule here ***/

start:  /* empty */
        
        | start SEMI
        | start addexpr END
        {
          driver.context().set_start($2);
        }
        | start addexpr SEMI
        {
          driver.context().set_start($2);
        };

constant: INTEGER
          {
            $$ = new ValNode(static_cast<double>($1));
          }
          | REAL
          {
            $$ = new ValNode($1);
          };

mulexpr: constant
          {
            $$ = $1;
          }
          | mulexpr MUL constant
          {
            $$ = new OpNode($1, $3, '*');
          }
          | mulexpr DIV constant
          {
            $$ = new OpNode($1, $3, '/');
          };


addexpr: mulexpr
          {
            $$ = $1;
          }
          | addexpr ADD mulexpr
          {
            $$ = new OpNode($1, $3, '+');
          }
          | addexpr SUB mulexpr
          {
            $$ = new OpNode($1, $3, '-');
          };

%%

void ntc::Parser::error(const location_type &loc, const std::string& msg) {
  driver.error(loc, msg);
}