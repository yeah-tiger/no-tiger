%{
#include <string>
#include <algorithm>
#include "scanner.hpp"

using token = ntc::Parser::token;
using token_type = ntc::Parser::token_type

#define yyterminate() return token::END

#define YY_USER_ACTION location->columns(yyleng);

%}

%option c++
%option stack
%option yyclass="Scanner"
%option nodefault
%option noyywrap
%%

%{
    location->step();
%}

"+"             { return token::ADD; }
"-"             { return token::SUB; }
"*"             { return token::MUL; }
"/"             { return token::DIV; }

[0-9]+          { 
                    lval->build(std::stoi(yytext)); 
                    return token::INTEGER;
                }

[0-9]+\.[0-9]+  {
                    lval->build(std::stod(yytext));
                    return token::REAL;
                }

[\n]            { 
                    location->lines(); 
                }

[ \t]+          { 
                    continue; 
                }

.		        {
                    std::cerr << "Error at " << *location << "" <<  ":" << std::endl;
                    throw std::logic_error("Invalid character\n");
                }


%%

namespace ntc {
    Scanner::Scanner(std::istream *in): FlexLexer(in) {}

    Scanner::~Scanner() {}
}
