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
  } // namespace type
  
} // namespace ntc
