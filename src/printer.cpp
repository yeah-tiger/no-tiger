#include "printer.hpp"
#include <iostream>
namespace ntc
{
  void Printer::visit(TranslationUnit& translation_unit) {
    std::cout << "Translation Unit: "<< std::endl;
    auto& decls = translation_unit.get_declarations();
    for (auto& decl: decls) {
      decl->accept(*this);
    }
  }  

  void Printer::visit(ExternalDecl& external_declaration) {
    std::cout << "External Declaraiton: " << std::endl;
    std::cout << "val: " << external_declaration.get_val() << std::endl;  
  }
} // namespace ntc
