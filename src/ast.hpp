// AST definition, refer parser.y for grammar
#pragma once
#include <cassert>
#include <deque>
#include <memory>
#include <vector>

#include "type.hpp"
#include "visitor.hpp"
namespace ntc {
class AST;
class ExternalDeclaration;
class TranslationUnit;
class FunctionDefinition;
class DeclarationSpecifier;
class Identifier;
class ParameterDeclaration;
class ParameterList;
class TypeSpecifier;

class Statement;
class StatementList;
class CompoundStatement;
class ExpressionStatement;
class ReturnStatement;

class Expression;
class ConstantExpression;
class IntegerExpression;
class FloatExpression;
class BooleanExpression;

class AST {
 public:
  virtual ~AST() noexcept = default;
  virtual void accept(Visitor& vistior) = 0;
};

class Statement : public AST {
 public:
  virtual ~Statement() {}
};

class Expression : public Statement {
 public:
  virtual ~Expression() {}
};

class ExternalDeclaration : public AST {
  public:
  virtual ~ExternalDeclaration() {}
};

class TranslationUnit final : public AST {
 public:
  explicit TranslationUnit(
      std::unique_ptr<ExternalDeclaration>&& external_declaration) {
    add_external_declaration(std::move(external_declaration));
  }

  void add_external_declaration(
      std::unique_ptr<ExternalDeclaration>&& external_declaration) {
    external_declarations_.push_back(std::move(external_declaration));
  }

  virtual void accept(Visitor& vistor) override { vistor.visit(*this); }

  auto& get_declarations() { return external_declarations_; }

 protected:
  std::vector<std::unique_ptr<ExternalDeclaration>> external_declarations_;
};

class ParameterDeclaration final : public AST {
 public:
  ParameterDeclaration(std::unique_ptr<DeclarationSpecifier>&& declaration_specifier,
                       std::unique_ptr<Identifier>&& identifier)
      : declaration_specifier_(std::move(declaration_specifier)),
        identifier_(std::move(identifier)) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

 protected:
  std::unique_ptr<DeclarationSpecifier> declaration_specifier_;
  std::unique_ptr<Identifier> identifier_;
};

class ParameterList final : public AST {
 public:
  ParameterList(std::unique_ptr<ParameterDeclaration>&& parameter_declaration) {
    add_parameter_declaration(std::move(parameter_declaration));
  }

  virtual void accept(Visitor& visitor) override { assert(false); }

  void add_parameter_declaration(
      std::unique_ptr<ParameterDeclaration>&& parameter_declaration) {
    parameter_list_.push_back(std::move(parameter_declaration));
  }

  auto& get_parameter_list() {return parameter_list_; }

 protected:
  std::vector<std::unique_ptr<ParameterDeclaration>> parameter_list_;
};

class FunctionDefinition final : public ExternalDeclaration {
 public:
  explicit FunctionDefinition(
      std::unique_ptr<DeclarationSpecifier>&& declaration_specifier,
      std::unique_ptr<Identifier>&& identifier,
      std::unique_ptr<ParameterList>&& parameter_list,
      std::unique_ptr<CompoundStatement>&& compound_statement)
      : declaration_specifier_(std::move(declaration_specifier)),
        identifier_(std::move(identifier)),
        compound_statement_(std::move(compound_statement)) {
          if (parameter_list != nullptr) {
            parameter_list_ = std::move(parameter_list->get_parameter_list());
          }
        }

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

  auto& get_declaration_specifier()  { return declaration_specifier_; }

  auto& get_identifier() { return identifier_; }

  auto& get_parameter_list()  { return parameter_list_; }

  auto& get_compound_statement() { return compound_statement_; }

 protected:
  std::unique_ptr<DeclarationSpecifier> declaration_specifier_;
  std::unique_ptr<Identifier> identifier_;
  std::vector<std::unique_ptr<ParameterDeclaration>> parameter_list_;
  std::unique_ptr<CompoundStatement> compound_statement_;
};

class DeclarationSpecifier final : public AST {
 public:
  explicit DeclarationSpecifier(std::unique_ptr<TypeSpecifier>&& type_specifier)
      : is_const_(false) {
    add_type_specifier(std::move(type_specifier));
  }

  explicit DeclarationSpecifier(bool is_const) : is_const_(true) {
    assert(is_const == true);
  }

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

  void add_type_specifier(std::unique_ptr<TypeSpecifier>&& type_specifier) {
    assert(type_specifier != nullptr);
    type_specifiers_.push_back(std::move(type_specifier));
  }

  void set_const(bool is_const) { is_const_ = is_const; }

 protected:
  std::vector<std::unique_ptr<TypeSpecifier>> type_specifiers_;
  bool is_const_;
};

class Identifier final : public Expression {
 public:
  Identifier(const std::string& name) : name_(name) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

  const std::string& get_name() const { return name_; }

 protected:
  std::string name_;
};


class TypeSpecifier final : public AST {
 public:
  TypeSpecifier(type::Specifier specififer) : specifier_(specififer) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

  type::Specifier get_specifier() const { return specifier_; }

 private:
  type::Specifier specifier_;
};

class StatementList final : public AST {
 public:
  StatementList(std::unique_ptr<Statement>&& statement) {
    add_statement(std::move(statement));
  }

  virtual void accept(Visitor& visitor) override { assert(false); }

  void add_statement(std::unique_ptr<Statement>&& statement) {
    statement_list_.push_back(std::move(statement));
  }

 protected:
  std::vector<std::unique_ptr<Statement>> statement_list_;
};

class CompoundStatement final : public Statement {
 public:
  CompoundStatement(std::unique_ptr<StatementList>&& statement_list)
      : statement_list_(std::move(statement_list)) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

 protected:
  std::unique_ptr<StatementList>&& statement_list_;
};

class ExpressionStatement final : public Statement {
 public:
  ExpressionStatement(std::unique_ptr<Expression>&& expression)
      : expression_(std::move(expression)) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

 protected:
  std::unique_ptr<Expression> expression_;
};

class ReturnStatement final : public Statement {
 public:
  ReturnStatement(std::unique_ptr<Expression>&& expression)
      : expression_(std::move(expression)) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

 protected:
  std::unique_ptr<Expression> expression_;
};

class ConstantExpression : public Expression {};

class IntegerExpression final : public ConstantExpression {
 public:
  IntegerExpression(int val) : val_(val) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

 protected:
  int val_;
};

class FloatExpression final : public ConstantExpression {
 public:
  FloatExpression(double val) : val_(val) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }
  
 protected:
  double val_;
};

class BooleanExpression final : public ConstantExpression {
 public:
  BooleanExpression(bool val) : val_(val) {}

  virtual void accept(Visitor& visitor) override { visitor.visit(*this); }

 protected:
  bool val_;
};

// factory function
template <typename AstType, typename... Args>
std::unique_ptr<AstType> make_ast(Args&&... args) {
  return std::make_unique<AstType>(std::forward<Args>(args)...);
}

}  // namespace ntc
