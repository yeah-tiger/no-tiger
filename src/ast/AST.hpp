#pragma once
#include <deque>
#include <memory>

#include "../context.hpp"

namespace ntc {
class AST : public std::enable_shared_from_this<AST> {
 public:
  virtual ~AST() noexcept = default;

  std::deque<std::shared_ptr<AST>>& children() { return children_; }

  std::weak_ptr<AST>& parent() { return parent_; }

  void add_child(const std::shared_ptr<AST>& child) {
    children_.push_back(child);
    child->parent() = this->shared_from_this();
  }

  virtual bool codegen(ProgramContext& context) = 0;

 protected:
  std::deque<std::shared_ptr<AST>> children_;
  std::weak_ptr<AST> parent_;
};
}  // namespace ntc
