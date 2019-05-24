#pragma once
#include <string>
#include "context.hpp"
#include "scanner.hpp"
#include "parser.hpp"
namespace ntc {
class ProgramContext;

class Driver {
 public:
  Scanner* scanner;
  Driver(ProgramContext& _context);

  bool parse_file(const std::string& filename);

  ProgramContext& get_context();

 private:
  ProgramContext& context_;
};
}  // namespace ntc
