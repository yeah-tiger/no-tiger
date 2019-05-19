// AST definition, refer parser.y for grammar
#pragma once
#include <deque>
#include <memory>
#include <vector>

#include "visitor.hpp"

namespace ntc {

class AST {
 public:
  virtual ~AST() noexcept = default;
  virtual void accept(Visitor& vistior) = 0;
};

// using ExternalDecl = AST;
class ExternalDecl : public AST {
 public:
  ExternalDecl(int _val) : val(_val) {}

  virtual void accept(Visitor& visitor) { visitor.visit(*this); }

  int get_val() const { return val; }

 protected:
  int val;
};

class TranslationUnit final : public AST {
 public:
  TranslationUnit(std::unique_ptr<ExternalDecl>&& external_declaration) {
    add_external_declaration(std::move(external_declaration));
  }

  void add_external_declaration(
      std::unique_ptr<ExternalDecl>&& external_declaration) {
    external_declarations_.push_back(std::move(external_declaration));
  }

  virtual void accept(Visitor& vistor) override { vistor.visit(*this); }

  auto& get_declarations() const { return external_declarations_; }

 protected:
  std::vector<std::unique_ptr<ExternalDecl>> external_declarations_;
};

// class FuncDef final : public ExternalDecl {
//   public:
//   FuncDef(std::unique_ptr<)
//   protected:
// };

// class Stmt : public AST {
//  public:
//   virtual ~Stmt() {}
// };

// class EmptyStmt : public Stmt {};

// using StmtList = std::vector<std::unique_ptr<Stmt>>;

// class CompoundStmt : public Stmt {};

// class ReturnStmt : public Stmt {};

// factory function
template <typename AstType, typename... Args>
std::unique_ptr<AstType> make_ast(Args&&... args) {
  return std::make_unique<AstType>(std::forward<Args>(args)...);
}

}  // namespace ntc
