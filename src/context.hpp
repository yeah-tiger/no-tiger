#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include "ast.hpp"
namespace ntc {

class ProgramContext {
 public:
  auto& get_start() { return program_start; }

 private:
  std::unique_ptr<TranslationUnit> program_start;
};
}  // namespace ntc
