#include "driver.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace ntc {
Driver::Driver(ProgramContext& context)
    : context_(context), scanner(nullptr) {}

bool Driver::parse_file(const std::string& filename) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    return false;
  }
  context_.set_name(filename);
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

ProgramContext& Driver::get_context() { return context_; }

}  // namespace ntc