#pragma once
#include <string>
namespace ntc {
namespace type {
  enum class Specifier {
    UNDEFINED,
    INT,
    FLOAT,
    DOUBLE,
    SHORT,
    LONG,
    CHAR,
    VOID,
    BOOL,
    STRING,
  };

  enum class BinaryOp {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUAL,
    NOT_EQUAL,
    LOGIC_OR,
    LOGIC_AND,
    ASSIGN
  };

  enum class UnaryOp {
    POSITIVIZE,
    NEGATE,
    LOGIC_NOT
  };
  
  std::string to_string(Specifier specifier);

  std::string to_string(BinaryOp op);

  std::string to_string(UnaryOp op);
}

}  // namespace ntc
