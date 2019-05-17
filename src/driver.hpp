#pragma once
#include <string>
#include "scanner.hpp"
#include "parser.hpp"
namespace ntc {
class ProgramContext;

class Driver {
 public:
  Scanner* scanner;
  Driver(ProgramContext& _context);

  bool parse_file(const std::string& filename);

  void error(const std::string& msg);

  void error(const location& loc, const std::string& msg);

  ProgramContext& context();

 private:
  ProgramContext& _context;
};
}  // namespace ntc
