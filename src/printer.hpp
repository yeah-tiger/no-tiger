#pragma once
#include "visitor.hpp"
#include "ast.hpp"
namespace ntc
{
  class Printer final: public Visitor {
    public:
      virtual void visit(TranslationUnit& translation_unit) override;

      virtual void visit(ExternalDecl& external_declaration) override;
  };
} // namespace ntc
