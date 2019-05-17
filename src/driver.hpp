#pragma once
#include <string>


namespace ntc {
class ProgramContext;

class Driver {
 public:
  Driver(ProgramContext& _context);

  bool parse_file(const std::string& filename);

  void error(const std::string& msg);

  void error(const location& loc, const std::string& msg);

  ProgramContext& context();
 private:
  Scanner* scanner;
  ProgramContext& context;
};
}  // namespace ntc
