#include <iostream>
#include "context.hpp"
#include "driver.hpp"
#include "printer.hpp"
#include "config.hpp"
using namespace ntc;

void error_exit() {
  std::cerr << "Error occurred, exiting..." << std::endl;
  exit(1);
}

int main(int argc, char* argv[]) {
  ProgramConfig config = parse_program_options(argc, argv);
  ProgramContext context;
  Driver driver(context);
  bool res = driver.parse_file(config.input_filename);
  if (res == false) {
    error_exit();
  }
  if (config.mode == ProgramMode::DUMP_AST) {
    Printer printer(std::cout);
    context.get_program()->accept(printer);
  }
  return 0;
}