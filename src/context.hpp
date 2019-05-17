#pragma once
#include <memory>
#include <cassert>
#include <vector>
// #include "./ast/"
namespace ntc
{

class ProgramContext
{
public:
  bool codegen_x86();

  bool codegen_llvm();

private:
  // TODO: llvm facilties
  std::unique_ptr<Program> program_;
  
};
} // namespace ntc
