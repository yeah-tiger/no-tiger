#pragma once
#include <cxxopts.hpp>

enum class ProgramMode {
  EMIT_LLVM_IR,
  EMIT_ASSEMBLY,
  EMIT_OBJECT,
  DUMP_AST,
};

struct ProgramConfig {
  ProgramConfig() : mode(ProgramMode::EMIT_LLVM_IR) {}
  std::string input_filename;
  std::string output_filename;
  ProgramMode mode;
};

ProgramConfig parse_program_options(int argc, char* argv[]);