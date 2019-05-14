%{
  #include <cstdio>
  #include "y.tab.h"
  int line_no = 1;
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
"const"                             { return CONST; }
"continue"                          { return CONTINUE; }
"default"                           { return DEFAULT; }
"do"                                { return DO; }
"else"                              { return ELSE; }
"enum"                              { return ENUM; }
"for"                               { return FOR; }
"if"                                { return IF; }
"return"                            { return RETURN; }
"sizeof"                            { return SIZEOF; }
"static"                            { return STATIC; }
"struct"                            { return STRUCT; }
"switch"                            { return SWITCH; }
"typedef"                           { return TYPEDEF; }
"union"                             { return UNION; }
"void"                              { return VOID; }
"while"                             { return WHILE; }


"i8"                                { return }
"i16"                               { return }
"i32"                               { return }
"i64"                               { return }
"u8"                                { return }
"u16"                               { return }
"u32"                               { return }
"u64"                               { return }  
"f32"                               { return }
"f64"                               { return }
"char"                              { return }

[a-zA-Z_]([a-zA-Z0-9_])* {
    return ID;
}

[0-9]+                    { return INTEGER; }
[0-9]+"."[0-9]+           { return REAL; }




. {}

%%