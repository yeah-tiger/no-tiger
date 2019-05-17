%code requires {

#include <iostream>
#include <string>

class Scanner;
}

%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%start start
%name-prefix="ntc"
%define "parser_class_name" "Parser"
%define api.value.type variant
%locations

%parse-parm { Driver& driver }

%code {
#include "scanner.hpp"

#undef yylex
#define yylex scanner.yylex

}



