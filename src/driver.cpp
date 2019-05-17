#include "driver.hpp"
#include "scanner.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace ntc {
Driver::Driver(ProgramContext& _context)
    : context(_context), scanner(nullptr) {}

bool Driver::parse_file(const std::string& filename) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    return false;
  }
  Scanner scanner(&in);

  Parser parser(*this);
  try {
    int res = parser.parse();
  } catch(std::logic_error& ) {

  }
  return res == 0;
}

ProgramContext& Driver::context() { return context; }

void Driver::error(const std::string& msg) { std::cerr << msg << std::endl; }

void Driver::error(const location& loc, const std::string& msg) {
  std::cerr << loc << ": " << msg << std::endl;
}

}  // namespace ntc