%{
#include <cstdio>
#include "y.tab.h"
%}

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]




%%
"/*"                                { handle_comment(); }
"//".*                              {} 

"break"                             { return BREAK; }
"case"                              { return CASE; }
"char"                              { return CHAR; }
"const"                             { return CONST; }
"continue"                          { return CONTINUE; }
"default"                           { return DEFAULT; }
"do"                                { return DO; }
"double"                            { return DOUBLE; }
"else"                              { return ELSE; }
"enum"                              { return ENUM; }
"float"                             { return FLOAT; }
"for"                               { return FOR; }
"if"                                { return IF; }
"int"                               { return INT; }
"long"                              { return LONG; }
"return"                            { return RETURN; }
"signed"                            { return SIGNED; }
"sizeof"                            { return SIZEOF; }
"static"                            { return STATIC; }
"struct"                            { return STRUCT; }
"switch"                            { return SWITCH; }
"typedef"                           { return TYPEDEF; }
"union"                             { return UNION; }
"unsigned"                          { return UNSIGNED; }
"void"                              { return VOID; }
"while"                             { return WHILE; }




%%