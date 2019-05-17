#pragma once
#include <deque>
#include <memory>

namespace ntc {
class AST : public std::enable_shared_from_this<AST> {
 public:
  virtual ~AST() {}

  std::deque<std::shared_ptr<AST>>& children() { return children; }

  std::weak_ptr<AST>& parent() { return parent; }

  void add_children(const std::shared_ptr<AST>& child) {
    children().push_back(child);
    child->parent() = this->shared_from_this();
  }

  virtual int evaluate() = 0;

 protected:
  std::deque<std::shared_ptr<AST>> children;
  std::weak_ptr<AST> parent;
};
}  // namespace ntc
