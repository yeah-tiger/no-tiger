#pragma once
#include <cstddef>
#include <string>
namespace ntc {
namespace {

struct SourceLocation {
  SourceLocation() : line(0), column(0) {}
  SourceLocation(size_t line, size_t column) : line(line), column(column) {}
  size_t line;
  size_t column;
};
}  // namespace
enum class TokenType {
  // reserved
  RESERVED,
  END_OF_FILE,
  // keyword
  KW_AUTO,
  KW_BOOL,
  KW_BREAK,
  KW_CASE,
  KW_CHAR,
  KW_CONST,
  KW_CONTINUE,
  KW_DEFAULT,
  KW_DO,
  KW_DOUBLE,
  KW_ELSE,
  KW_ENUM,
  KW_EXTERN,
  KW_FLOAT,
  KW_FOR,
  KW_GOTO,
  KW_IF,
  KW_INLINE,
  KW_INT,
  KW_LONG,
  KW_REGISTER,
  KW_RESTRICT,
  KW_RETURN,
  KW_SHORT,
  KW_SIGNED,
  KW_SIZEOF,
  KW_STATIC,
  KW_STRUCT,
  KW_SWITCH,
  KW_TYPEDEF,
  KW_UNION,
  KW_UNSIGNED,
  KW_VOID,
  KW_VOLATILE,
  KW_WHILE,
  KW_TRUE,
  KW_FALSE,
  // symbol
  LPARENT,
  RPARENT,
  LBRACKET,
  RBRACKET,
  LBRACE,
  RBRACE,
  COLON,
  SEMI,
  COMMA,
  ARROR,     // ->
  DOT,       // .
  ELLIPSIS,  // ...

  // op
  OP_LESS,
  OP_LEQ,
  OP_GREATER,
  OP_GEQ,
  OP_LSHIFT,
  OP_RSHFIT,
  OP_PLUS,
  OP_PLUS_PLUS,
  OP_MINUS,
  OP_MINUS_MINUS,
  OP_STAR,
  OP_DIV,
  OP_MOD,

  OP_AND,
  OP_OR,
  OP_AND_AND,
  OP_OR_OR,
  OP_CARET,  // ^
  OP_NOT,
  OP_TILDE,  // ~

  OP_ASSIGN,
  OP_STAR_ASSIGN,
  OP_DIV_ASSIGN,
  OP_MOD_ASSIGN,
  OP_PLUS_ASSIGN,
  OP_MINUS_ASSIGN,
  OP_LSHIFT_ASSIGN,
  OP_RSHFIT_ASSIGN,
  OP_AND_ASSIGN,
  OP_XOR_ASSIGN,
  OP_OR_ASSIGN,
  OP_EQ,
  OP_NEQ,

  OP_QUESTION,

  IDENTIFIER,
  INTERGER_LITERAL,
  FLOATING_LITERAL,
  CHARACTER_LITERAL,
  STRING_LITERAL,
};
struct Token {
  Token() : type(TokenType::RESERVED) {}
  Token(TokenType type, SourceLocation location, const std::string& content)
      : type(type), location(location), content(content) {}
  TokenType type;
  SourceLocation location;
  std::string content;
};
}  // namespace ntc
