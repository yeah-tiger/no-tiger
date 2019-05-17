%{
#include <string>
#include <algorithm>

#include "scanner.hpp"
// define the signature of yylex
#undef YY_DECL
#define YY_DECL int ntc::Scanner::yylex( ntc::Parser::semantic_type* lval, ntc::Parser::location_type* location )


using token = ntc::Parser::token;

#define yyterminate() return token::END

#define YY_USER_ACTION location->step(); location->columns(yyleng);

#define YY_NO_UNISTD_H
%}

%option c++
%option stack
%option yyclass="ntc::Scanner"
%option nodefault
%option noyywrap
%%

%{
    yylval = lval;
%}




"+"             { return token::ADD; }
"-"             { return token::SUB; }
"*"             { return token::MUL; }
"/"             { return token::DIV; }
";"             { return token::SEMI; }


[0-9]+          { 
                    /* TODO: add lex definition here  */
                    yylval->build(std::stoi(yytext)); 
                    return token::INTEGER;
                }

[0-9]+\.[0-9]+  {
                    yylval->build(std::stod(yytext));
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
