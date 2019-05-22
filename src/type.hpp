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
    UNSIGNED,
    SIGNED,
  };

  enum class BinaryOp {

  };

  enum class UnaryOp {

  };
  
  std::string to_string(Specifier specifier);
}

}  // namespace ntc
