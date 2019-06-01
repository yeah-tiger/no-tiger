%{
#include <string>
#include <algorithm>
#include <iostream>
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

"/*"            { 
                    int c;
                    while ((c = yyinput())) {
                      if (c == '*') {
                        if ((c = yyinput()) == '/') {
                            break;
                        } else {
                            unput(c);
                        }
                      } else if (c == '\n') {
                        location->lines();
                      } else if (c == '\0') {
                        std::cerr << "unexpected EOF inside comment at" << *location << std::endl;
                        throw std::logic_error("Invalid character\n");
                      }
                    }
                }

"//".*          { continue; }

"return"        { return token::RETURN; }
"if"            { return token::IF; }
"else"          { return token::ELSE; }
"while"         { return token::WHILE; }
"for"           { return token::FOR; }
"break"         { return token::BREAK; }
"continue"      { return token::CONTINUE; }

"int"           { return token::INT; }
"float"         { return token::FLOAT; }
"double"        { return token::DOUBLE; }
"short"         { return token::SHORT; }
"long"          { return token::LONG; }
"char"          { return token::CHAR; }
"void"          { return token::VOID; }
"bool"          { return token::BOOL; }
"string"        { return token::STRING; }

"true"          { 
                  yylval->build(true);
                  return token::BOOLEAN;
                }

"false"         {
                  yylval->build(false);
                  return token::BOOLEAN;
                }

"const"         { return token::CONST; }


[0-9]+          {
                    yylval->build(std::stoi(yytext)); 
                    return token::INTEGER;
                }

[0-9]+\.[0-9]+  {
                    yylval->build(std::stod(yytext));
                    return token::REAL;
                }

'(\\.|[^\\'])*' {
                    yytext[yyleng - 1] = 0;
                    yylval->build(std::string(yytext + 1));
                    return token::CHARACTER;
                }

\"(\\.|[^\\"])*\" {
                    yytext[yyleng - 1] = 0;
                    yylval->build(std::string(yytext + 1));
                    return token::STRING_LITERAL;
                }

([_a-zA-Z])([_a-zA-Z0-9])* {
                    yylval->build(std::string(yytext));
                    return token::IDENTIFIER;
                }

"&&"            { return token::AND_OP; }
"||"            { return token::OR_OP; }
"<="            { return token::LE_OP; }
">="            { return token::GE_OP; }
"=="            { return token::EQ_OP; }
"!="            { return token::NE_OP; }
""

"+"             { return ('+'); }
"-"             { return ('-'); }
"*"             { return ('*'); }
"/"             { return ('/'); }
"="             { return ('='); }
"<"             { return ('<'); }
">"             { return ('>'); }
"!"             { return ('!'); }

":"             { return (':'); }
"?"             { return ('?'); }
","             { return (','); }
";"             { return (';'); }
"("			        { return ('('); }
")"			        { return (')'); }
"{"		          { return ('{'); }
"}"		          { return ('}'); }





[\n\r]          { 
                    location->lines();
                }

[ \t]+          { 
                    continue; 
                }

.		            {
                    std::cerr << "Scanner: Error at " << *location << "" <<  ":" << std::endl;
                    throw std::logic_error("Scanner: Invalid character\n");
                }


%%
