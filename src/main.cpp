#include <iostream>
#include "context.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "config.hpp"
using namespace ntc;

void error_exit() {
  std::cerr << "Error occurred, exiting..." << std::endl;
  exit(1);
}

int main(int argc, char* argv[]) {
  ProgramConfig config = parse_program_options(argc, argv);
  Parser parser;
  
  return 0;
}