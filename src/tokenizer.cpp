#include "tokenizer.hpp"

namespace ntc {
namespace {
static std::unordered_map<std::string, TokenType> build_keyword_map() {
  // reversed for C++ 20
  // using enum TokenType;
  std::unordered_map<std::string, TokenType> res;
  res["auto"] = TokenType::KW_AUTO;
  res["bool"] = TokenType::KW_BOOL;
  res["break"] = TokenType::KW_BREAK;
  res["case"] = TokenType::KW_CASE;
  res["char"] = TokenType::KW_CHAR;
  res["const"] = TokenType::KW_CONST;
  res["continue"] = TokenType::KW_CONTINUE;
  res["default"] = TokenType::KW_DEFAULT;
  res["do"] = TokenType::KW_DO;
  res["double"] = TokenType::KW_DOUBLE;
  res["else"] = TokenType::KW_ELSE;
  res["enum"] = TokenType::KW_ENUM;
  res["extern"] = TokenType::KW_EXTERN;
  res["float"] = TokenType::KW_FLOAT;
  res["for"] = TokenType::KW_FOR;
  res["goto"] = TokenType::KW_GOTO;
  res["if"] = TokenType::KW_IF;
  res["inline"] = TokenType::KW_INLINE;
  res["int"] = TokenType::KW_INT;
  res["long"] = TokenType::KW_LONG;
  res["register"] = TokenType::KW_REGISTER;
  res["restrict"] = TokenType::KW_RESTRICT;
  res["return"] = TokenType::KW_RETURN;
  res["short"] = TokenType::KW_SHORT;
  res["signed"] = TokenType::KW_SIGNED;
  res["sizeof"] = TokenType::KW_SIZEOF;
  res["static"] = TokenType::KW_STATIC;
  res["struct"] = TokenType::KW_STRUCT;
  res["switch"] = TokenType::KW_SWITCH;
  res["typedef"] = TokenType::KW_TYPEDEF;
  res["union"] = TokenType::KW_UNION;
  res["unsigned"] = TokenType::KW_UNSIGNED;
  res["void"] = TokenType::KW_VOID;
  res["volatile"] = TokenType::KW_VOLATILE;
  res["while"] = TokenType::KW_WHILE;
  res["true"] = TokenType::KW_TRUE;
  res["false"] = TokenType::KW_FALSE;
  return res;
}
}  // namespace
std::unordered_map<std::string, TokenType> Tokenizer::keyword_map = build_keyword_map();
Token Tokenizer::next_token() {
  
}
}  // namespace ntc
