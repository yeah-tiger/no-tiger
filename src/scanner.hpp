#ifndef NTC_SCANNER_H
#define NTC_SCANNER_H

#if ! defined( yyFlexLexerOnce )
#include <FlexLexer.h>
#endif

// for token_type
#include "parser.hpp"
#include "location.hh"

namespace ntc {
  class Scanner: public yyFlexLexer {
    public:
      Scanner(std::istream *in): yyFlexLexer(in) {};

      virtual ~Scanner() {};

      using FlexLexer::yylex;

      virtual int yylex(ntc::Parser::semantic_type* lval, ntc::Parser::location_type *location);

  private:
    ntc::Parser::semantic_type* yylval = nullptr;
  };
}


#endif