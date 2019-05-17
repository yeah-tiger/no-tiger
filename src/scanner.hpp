#ifndef NTC_SCANNER_H
#define NTC_SCANNER_H


// define the signature of yylex
#ifndef YY_DECL
#define YY_DECL                               \
    ntc::Parser::token_type                   \
    ntc::Scanner::lex(                        \
        ntc::Parser::semantic_type* lval,   \
        ntc::Parser::location_type* location    \
    )
#endif

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

// for token_type
#include "parser.hpp"

namespace ntc {
  class Scanner: public yyFlexLexer {
    public:
      Scanner(std::istream *in);
      virtual ~Scanner();

      virtual Parser::token_type lex(Parser::semantic_type* lval, Parser::location_type *location);

      void error(const char* yytext, Parser)
  }
}


#endif