#pragma once
#include <cassert>
#include <vector>
// #include "./ast/"
namespace ntc
{
  // TODO: remove test ASTs, add defs to context
class CalcNode
{
public:
  virtual ~CalcNode()
  {
  }

  virtual double evaluate() const = 0;
};

class ValNode final : public CalcNode
{
public:
  explicit ValNode(double _val) : val(_val)
  {
  }

  virtual double evaluate() const override
  {
    return val;
  }

private:
  double val;
};

class OpNode final : public CalcNode
{
public:
  explicit OpNode(CalcNode *_left, CalcNode *_right, char _op) : left(_left), right(_right), op(_op)
  {
  }

  virtual double evaluate() const override
  {

    if (op == '+')
    {
      return left->evaluate() + right->evaluate();
    }
    else if (op == '-')
    {

      return left->evaluate() - right->evaluate();
    }
    else if (op == '*')
    {

      return left->evaluate() * right->evaluate();
    }
    else if (op == '/')
    {
      return left->evaluate() / right->evaluate();
    }
  }

private:
  char op;
  CalcNode *left;
  CalcNode *right;
};

class ProgramContext
{
public:
  ProgramContext(): start(nullptr) {}
  double evaluate() const
  {
    if (start == nullptr) {
      return 0.0;
    }
    return start->evaluate();
  }
  void set_start(CalcNode *_start)
  {
    assert(_start != nullptr);
    start = _start;
  }

private:
  CalcNode *start;
};
} // namespace ntc
