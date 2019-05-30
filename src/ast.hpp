// AST definition, refer parser.y for grammar
#pragma once
#include <llvm/IR/Value.h>
#include <cassert>
#include <deque>
#include <iostream>
#include <memory>
#include <vector>
#include "type.hpp"
#include "visitor.hpp"
namespace ntc {
// Language construct
class AST;
class BlockItem;
class ExternalDeclaration;
class TranslationUnit;
class FunctionDefinition;
class DeclarationSpecifier;
class Identifier;
class ParameterDeclaration;
class TypeSpecifier;
class Declaration;
class Initializer;
// Statement
class Statement;
class CompoundStatement;
class ExpressionStatement;
class JumpStatement;
class ReturnStatement;
class BreakStatement;
class ContinueStatement;
class SelectionStatement;
class IfStatement;
class IterationStatement;
class WhileStatement;
class ForStatement;

// Expression
class Expression;
class ConstantExpression;
class IntegerExpression;
class FloatExpression;
class BooleanExpression;
class CharacterExpression;
class StringLiteralExpression;
class BinaryOperationExpression;
class UnaryOperationExpression;
class ConditionalExpression;
class FunctionCall;

class AST {
 public:
  virtual ~AST() noexcept = default;
  virtual void accept(ASTVisitor& vistior) = 0;
  virtual llvm::Value* accept(IRVisitor& visitor) = 0;
};

class BlockItem : public AST {
 public:
  virtual ~BlockItem(){};
};

template <typename T>
class ASTList final : public AST {
 public:
  ASTList(std::unique_ptr<T>&& item) { add_item(std::move(item)); }

  void add_item(std::unique_ptr<T>&& item) {
    item_list_.push_back(std::move(item));
  }

  virtual void accept(ASTVisitor& ASTVisitor) override { assert(false); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    assert(false);
    return nullptr;
  }

  auto& get_item_list() { return item_list_; }

 protected:
  std::vector<std::unique_ptr<T>> item_list_;
};

using BlockItemList = ASTList<BlockItem>;

using ParameterList = ASTList<ParameterDeclaration>;

using ArgumentList = ASTList<Expression>;

class Statement : public BlockItem {
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
      std::unique_ptr<ExternalDeclaration>&& external_declaration,
      const std::string& name)
      : name_(name) {
    add_external_declaration(std::move(external_declaration));
  }

  void add_external_declaration(
      std::unique_ptr<ExternalDeclaration>&& external_declaration) {
    external_declarations_.push_back(std::move(external_declaration));
  }

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_declarations() { return external_declarations_; }

  std::string get_name() { return name_; }

 protected:
  std::vector<std::unique_ptr<ExternalDeclaration>> external_declarations_;
  std::string name_;
};

class ParameterDeclaration final : public AST {
 public:
  ParameterDeclaration(
      std::unique_ptr<DeclarationSpecifier>&& declaration_specifier,
      std::unique_ptr<Identifier>&& identifier)
      : declaration_specifier_(std::move(declaration_specifier)),
        identifier_(std::move(identifier)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_declaration_specifier() { return declaration_specifier_; }

  auto& get_identifier() { return identifier_; }

 protected:
  std::unique_ptr<DeclarationSpecifier> declaration_specifier_;
  std::unique_ptr<Identifier> identifier_;
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
      parameter_list_ = std::move(parameter_list->get_item_list());
    }
  }

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_declaration_specifier() { return declaration_specifier_; }

  auto& get_identifier() { return identifier_; }

  auto& get_parameter_list() { return parameter_list_; }

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

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_type_specifiers() { return type_specifiers_; }

  void add_type_specifier(std::unique_ptr<TypeSpecifier>&& type_specifier) {
    assert(type_specifier != nullptr);
    type_specifiers_.push_back(std::move(type_specifier));
  }

  void set_const(bool is_const) { is_const_ = is_const; }

  bool get_is_const() const { return is_const_; }

 protected:
  std::vector<std::unique_ptr<TypeSpecifier>> type_specifiers_;
  bool is_const_;
};

class Identifier final : public Expression {
 public:
  Identifier(const std::string& name) : name_(name) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  const std::string& get_name() const { return name_; }

 protected:
  std::string name_;
};

class TypeSpecifier final : public AST {
 public:
  TypeSpecifier(type::Specifier specififer) : specifier_(specififer) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  type::Specifier get_specifier() const { return specifier_; }

 private:
  type::Specifier specifier_;
};

class Declaration final : public BlockItem {
 public:
  Declaration(std::unique_ptr<TypeSpecifier>&& type_specifier,
              std::unique_ptr<Identifier>&& identifer,
              std::unique_ptr<Initializer>&& initializer = nullptr)
      : type_specifier_(std::move(type_specifier)),
        identifer_(std::move(identifer)),
        initializer_(std::move(initializer)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_type_specifier() { return type_specifier_; }

  auto& get_identifier() { return identifer_; }

  auto& get_initializer() { return initializer_; }

 protected:
  std::unique_ptr<TypeSpecifier> type_specifier_;
  std::unique_ptr<Identifier> identifer_;
  std::unique_ptr<Initializer> initializer_;
};

class Initializer final : public AST {
 public:
  Initializer(std::unique_ptr<Expression>&& expression)
      : expression_(std::move(expression)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_expression() { return expression_; }

 protected:
  std::unique_ptr<Expression> expression_;
};

// statement
class CompoundStatement final : public Statement {
 public:
  CompoundStatement(std::unique_ptr<BlockItemList>&& block_item_list) {
    if (block_item_list != nullptr) {
      block_item_list_ = std::move(block_item_list->get_item_list());
    }
  }

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_block_item_list() { return block_item_list_; }

 protected:
  std::vector<std::unique_ptr<BlockItem>> block_item_list_;
};

class ExpressionStatement final : public Statement {
 public:
  ExpressionStatement(std::unique_ptr<Expression>&& expression)
      : expression_(std::move(expression)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_expression() { return expression_; }

 protected:
  std::unique_ptr<Expression> expression_;
};

class JumpStatement : public Statement {
 public:
  virtual ~JumpStatement() {}
};

class ReturnStatement final : public JumpStatement {
 public:
  ReturnStatement(std::unique_ptr<Expression>&& expression = nullptr)
      : expression_(std::move(expression)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_expression() { return expression_; }

 protected:
  std::unique_ptr<Expression> expression_;
};

class BreakStatement final : public JumpStatement {
 public:
  BreakStatement() = default;
  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

 protected:
};

class ContinueStatement final : public JumpStatement {
 public:
  ContinueStatement() = default;

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

 protected:
};

class SelectionStatement : public Statement {
 public:
  virtual ~SelectionStatement() {}
};

class IfStatement final : public SelectionStatement {
 public:
  IfStatement(std::unique_ptr<Expression>&& if_expression,
              std::unique_ptr<Statement>&& then_statement,
              std::unique_ptr<Statement>&& else_statement = nullptr)
      : if_expression_(std::move(if_expression)),
        then_statement_(std::move(then_statement)),
        else_statement_(std::move(else_statement)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_if_expression() { return if_expression_; }

  auto& get_then_statment() { return then_statement_; }

  auto& get_else_statement() { return else_statement_; }

 protected:
  std::unique_ptr<Expression> if_expression_;
  std::unique_ptr<Statement> then_statement_;
  std::unique_ptr<Statement> else_statement_;
};

class IterationStatement : public Statement {
 public:
  virtual ~IterationStatement() {}
};

class WhileStatement final : public IterationStatement {
 public:
  WhileStatement(std::unique_ptr<Expression>&& while_expression,
                 std::unique_ptr<Statement>&& loop_statement)
      : while_expression_(std::move(while_expression)),
        loop_statement_(std::move(loop_statement)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_while_expression() { return while_expression_; }

  auto& get_loop_statement() { return loop_statement_; }

 protected:
  std::unique_ptr<Expression> while_expression_;
  std::unique_ptr<Statement> loop_statement_;
};

class ForStatement final : public IterationStatement {
 public:
  ForStatement(std::unique_ptr<ExpressionStatement>&& init_clause,
               std::unique_ptr<ExpressionStatement>&& cond_expression,
               std::unique_ptr<Statement>&& loop_statement,
               std::unique_ptr<Expression>&& iteraion_expression = nullptr)
      : init_clause_(std::move(init_clause)),
        cond_expression_(std::move(cond_expression)),
        iteraion_expression_(std::move(iteraion_expression)),
        loop_statement_(std::move(loop_statement)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_init_clause() { return init_clause_; }

  auto& get_cond_expression() { return cond_expression_; }

  auto& get_iteration_expression() { return iteraion_expression_; }

  auto& get_loop_statement() { return loop_statement_; }

 protected:
  std::unique_ptr<ExpressionStatement> init_clause_;
  std::unique_ptr<ExpressionStatement> cond_expression_;
  std::unique_ptr<Expression> iteraion_expression_;
  std::unique_ptr<Statement> loop_statement_;
};

/* Expression */
class ConstantExpression : public Expression {
 public:
  virtual ~ConstantExpression() {}
};

class IntegerExpression final : public ConstantExpression {
 public:
  IntegerExpression(int val) : val_(val) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  int get_val() { return val_; }

 protected:
  int val_;
};

class FloatExpression final : public ConstantExpression {
 public:
  FloatExpression(double val) : val_(val) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  double get_val() { return val_; }

 protected:
  double val_;
};

class BooleanExpression final : public ConstantExpression {
 public:
  BooleanExpression(bool val) : val_(val) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  bool get_val() { return val_; }

 protected:
  bool val_;
};

class CharacterExpression final : public ConstantExpression {
 public:
  CharacterExpression(char val) : val_(val) {}

  static bool check_character(const std::string& input) {
    return input.length() == 1;
  }

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  char get_val() { return val_; }

 protected:
  char val_;
};

class StringLiteralExpression final : public ConstantExpression {
 public:
  StringLiteralExpression(const std::string& val) : val_(val) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  std::string get_val() { return val_; }

 protected:
  std::string val_;
};

class BinaryOperationExpression final : public Expression {
 public:
  BinaryOperationExpression(type::BinaryOp op_type,
                            std::unique_ptr<Expression>&& lhs,
                            std::unique_ptr<Expression>&& rhs)
      : op_type_(op_type), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_lhs() { return lhs_; }

  auto& get_rhs() { return rhs_; }

  type::BinaryOp get_op_type() { return op_type_; }

 protected:
  type::BinaryOp op_type_;
  std::unique_ptr<Expression> lhs_;
  std::unique_ptr<Expression> rhs_;
};

class UnaryOperationExpression final : public Expression {
 public:
  UnaryOperationExpression(type::UnaryOp op_type,
                           std::unique_ptr<Expression>&& operand)

      : op_type_(op_type), operand_(std::move(operand)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_operand() { return operand_; }

  type::UnaryOp get_op_type() { return op_type_; }

 protected:
  type::UnaryOp op_type_;
  std::unique_ptr<Expression> operand_;
};

class ConditionalExpression final : public Expression {
 public:
  ConditionalExpression(std::unique_ptr<Expression>&& cond_expression,
                        std::unique_ptr<Expression>&& true_expression,
                        std::unique_ptr<Expression>&& false_expression)
      : cond_expression_(std::move(cond_expression)),
        true_expression_(std::move(true_expression)),
        false_expression_(std::move(false_expression)) {}

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_cond_expression() { return cond_expression_; }

  auto& get_true_expression() { return true_expression_; }

  auto& get_false_expression() { return false_expression_; }

 protected:
  std::unique_ptr<Expression> cond_expression_;
  std::unique_ptr<Expression> true_expression_;
  std::unique_ptr<Expression> false_expression_;
};

class FunctionCall final : public Expression {
 public:
  FunctionCall(std::unique_ptr<Expression>&& target,
               std::unique_ptr<ArgumentList>&& argument_list)
      : target_(std::move(target)) {
    if (argument_list != nullptr) {
      argument_list_ = std::move(argument_list->get_item_list());
    }
  }

  virtual void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

  virtual llvm::Value* accept(IRVisitor& visitor) override {
    return visitor.visit(*this);
  }

  auto& get_target() { return target_; }

  auto& get_argument_list() { return argument_list_; }

 protected:
  std::unique_ptr<Expression> target_;
  std::vector<std::unique_ptr<Expression>> argument_list_;
};

// factory function
template <typename AstType, typename... Args>
std::unique_ptr<AstType> make_ast(Args&&... args) {
  return std::make_unique<AstType>(std::forward<Args>(args)...);
}

}  // namespace ntc
