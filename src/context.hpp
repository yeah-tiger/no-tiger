#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include "ast.hpp"
namespace ntc {

class ProgramContext {
 public:
  auto& get_program() { return program_; }

  void set_name(const std::string& name) { name_ = name; }

  const std::string& get_name() const { return name_; }

 private:
  std::unique_ptr<TranslationUnit> program_;
  std::string name_;
};
}  // namespace ntc
