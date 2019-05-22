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
  int res = 1;
  try {
    res = parser.parse();
  } catch (std::logic_error& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return res == 0;
}

ProgramContext& Driver::context() { return _context; }

}  // namespace ntc