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



"return"        { return token::RETURN; }

"int"           { return token::INT; }
"float"         { return token::FLOAT; }
"double"        { return token::DOUBLE; }

"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }

";"             { return ';'; }

"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }


[0-9]+          { 
                    /* TODO: add lex definition here  */
                    yylval->build(std::stoi(yytext)); 
                    return token::INTEGER;
                }

[0-9]+\.[0-9]+  {
                    yylval->build(std::stod(yytext));
                    return token::REAL;
                }


[_a-zA-Z]([_a-zA-Z]|[0-9])+
                { 
                    yylval->build(yytext);
                    return token::IDENTIFIER;
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
