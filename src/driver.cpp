#include "driver.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace ntc {
Driver::Driver(ProgramContext& __context)
    : _context(__context), scanner(nullptr) {}

bool Driver::parse_file(const std::string& filename) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    return false;
  }
  Scanner scanner(&in);
  Parser parser(scanner, *this);
  int res = parser.parse();
  return res == 0;
}

ProgramContext& Driver::context() { return _context; }

void Driver::error(const std::string& msg) { std::cerr << msg << std::endl; }

void Driver::error(const class location& loc, const std::string& msg) {
  std::cerr << loc << ": " << msg << std::endl;
}

}  // namespace ntc