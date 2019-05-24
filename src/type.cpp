#include "type.hpp"
namespace ntc
{
  namespace type
  {
    std::string to_string(Specifier specifier) {
    static std::string prefix("specifier::");
    switch (specifier)
    {
    case Specifier::INT:
      return prefix + "int";
    case Specifier::FLOAT:
      return prefix + "float";
    case Specifier::DOUBLE:
      return prefix + "double";
    case Specifier::SHORT :
      return prefix + "short";
    case Specifier::LONG:
      return prefix + "long";
    case Specifier::CHAR:
      return prefix + "char";
    case Specifier::VOID:
      return prefix + "void";
    case Specifier::BOOL:
      return prefix + "bool";
    case Specifier::UNSIGNED:
      return prefix + "unsigned";
    case Specifier::SIGNED:
      return prefix + "signed";
    default:
      return prefix + "undefined";
    }
  }

  std::string to_string(BinaryOp op) {
    static std::string prefix("BinaryOp::");
    switch (op)
    {
    case BinaryOp::ADD:
      return prefix + "ADD";
    case BinaryOp::SUB:
      return prefix + "SUB";
    case BinaryOp::MUL:
      return prefix + "MUL";
    case BinaryOp::DIV:
      return prefix + "DIV";
    case BinaryOp::MOD:
      return prefix + "MOD";
    case BinaryOp::LESS:
      return prefix + "LESS";
    case BinaryOp::GREATER:
      return prefix + "GREATER";
    case BinaryOp::LESS_EQUAL:
      return prefix + "LESS_EQUAL";
    case BinaryOp::GREATER_EQUAL:
      return prefix + "GREATER_EQUAL";
    case BinaryOp::EQUAL:
      return prefix + "EQUAL";
    case BinaryOp::NOT_EQUAL:
      return prefix + "NOT_EQUAL";
    case BinaryOp::LOGIC_OR:
      return prefix + "LOGIC_OR";
    case BinaryOp::LOGIC_AND:
      return prefix + "LOGIC_AND";
    case BinaryOp::ASSIGN:
      return prefix + "ASSIGN";
    default:
      return prefix + "UNKNOWN";
    }
  }

  std::string to_string(UnaryOp op) {
    static std::string prefix("UnaryOp");
    switch (op)
    {
    case UnaryOp::POSITIVIZE:
      return prefix + "POSITIVIZE";
    case UnaryOp::NEGATE:
      return prefix + "NEGATE";
    case UnaryOp::LOGIC_NOT:
      return prefix + "LOGIC_NOT";
    default:
      return prefix + "UNKNWON";
    }
  } 
  } // namespace type
  
} // namespace ntc
