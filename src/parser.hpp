#pragma once
#include "tokenizer.hpp"
#include "ast.hpp"
namespace ntc {
class Parser {
 public:
 std::unique_ptr<TranslationUnit> parse(const std::string& filename);
 private:
  // Tokenizer tokenizer;
};
}  // namespace ntc
