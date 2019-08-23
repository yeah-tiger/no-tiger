#include "tokenizer.hpp"
#include <cctype>
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

Tokenizer::Tokenizer(const std::string& filename): filename(filename), line(1), column(0), current_char('\0'), error_flag(true) {
  in.open(this->filename);
  if (!in.is_open()) {
    diag("failed to open file: " + filename);
  }
}

Token Tokenizer::next_token() {
  SourceLocation cur_loc = make_location();
  if (error_flag) {
    return make_token(TokenType::RESERVED, cur_loc);
  }
  skip_whitespaces();
  skip_comments();
  if (in.eof()) {
    return make_token(TokenType::END_OF_FILE, cur_loc);
  }
  // dispatch
  if (std::isalpha(current_char) || current_char == '_') {
    return handle_identifer_or_keyword();
  } else if (std::isdigit(current_char) || (current_char == '.' && std::isdigit(peek())) ) {
    return handle_number();
  } else if (current_char == '\"') {
    return handle_string();
  } else if (current_char == '\'') {
    return handle_char();
  } else {
    return handle_symbol_or_op();
  }
}

void Tokenizer::advance() {
  current_char = in.get();
  if (current_char == '\n') {
    line += 1;
    column = 0;
  } else {
    column += 1;
  }
}

char Tokenizer::peek() {
  return in.peek();
}

void Tokenizer::skip_whitespaces() {
  while (std::isspace(current_char)) {
    advance();
  }
}

void Tokenizer::skip_comments() {
  // line comment
  if (current_char == '/' && peek() == '/') {
    // advance to the second '/'
    advance();
    // advance to the first character after "//"
    advance();
    while (!current_char == '\n') {
      advance();
    }
    advance();
  }
  // block comment
  if (current_char == '/' && peek() == '*') {
    advance();
    advance();
    while (!(current_char == '*' && peek() == '/') ) {
      advance();
    }
    advance();
    advance();
  }
}

Token Tokenizer::make_token(TokenType type, SourceLocation location) {
  Token res(type, location, token_buffer);
  token_buffer.clear();
  return res;
}

SourceLocation Tokenizer::make_location() {
  return SourceLocation(line, column);
}

Token Tokenizer::handle_identifer_or_keyword() {

}

Token Tokenizer::handle_number() {

}

Token Tokenizer::handle_string() {

}

Token Tokenizer::handle_char() {

}

Token Tokenizer::handle_symbol_or_op() {

}

void Tokenizer::diag(const std::string& msg) {
  error_flag = true;
  error_msgs.push_back(msg);
}
}  // namespace ntc
