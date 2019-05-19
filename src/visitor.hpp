#pragma once
namespace ntc {
class TranslationUnit;
class FuncDef;
class ExternalDecl;
class ReturnStmt;
class Visitor {
 public:
  virtual ~Visitor() {}
  virtual void visit(TranslationUnit&) = 0;
  virtual void visit(ExternalDecl&) = 0;
  // virtual void visit(FuncDef&) = 0;
  // virtual void visit(ReturnStmt&) = 0;
};
}  // namespace ntc
