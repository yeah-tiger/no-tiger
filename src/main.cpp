#include <cxxopts.hpp>
#include <iostream>
#include "printer.hpp"
#include "driver.hpp"
#include "context.hpp"
#include "codegen.hpp"
using namespace ntc;
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

ProgramConfig parse_program_options(int argc, char* argv[]) {
  using namespace cxxopts;
  try {
    cxxopts::Options options(argv[0], "- ntc: No-Tiger Lang Compiler`");
    options.positional_help("[optional args]").show_positional_help();
    options.add_options()
    ("i, input", "Input file", cxxopts::value<std::string>(), "FILE")
    ("l", "Emit llvm IR")
    ("s", "Emit assembly code")
    ("c", "Emit object code")
    ("o, output", "Output file", cxxopts::value<std::string>()->default_value("[same-as-input]"), "FILE")
    ("d, dump-ast", "Dump AST in XML format")
    ("h, help", "Show help");
    auto parse_result = options.parse(argc, argv);
    if (parse_result.count("h")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }
    ProgramConfig config_result;
    if (parse_result.count("i")) {
      std::string input_filename = parse_result["i"].as<std::string>();
      config_result.input_filename = input_filename;
    } else {
      std::cerr << argv[0] << ": fatal no input file" << std::endl;
      exit(4);
    }
    if (parse_result.count("s")) {
      config_result.mode = ProgramMode::EMIT_ASSEMBLY;
    }
    if (parse_result.count("c")) {
      config_result.mode = ProgramMode::EMIT_OBJECT;
    }
    if (parse_result.count("l")) {
      config_result.mode = ProgramMode::EMIT_LLVM_IR;
    }
    if (parse_result.count("d")) {
      config_result.mode = ProgramMode::DUMP_AST;
    }
    if (parse_result.count("o")) {
      std::string output_filename = parse_result["i"].as<std::string>();
      config_result.output_filename = output_filename;
    } else {
      std::string output_filename = config_result.input_filename;
      auto pos = output_filename.find_last_of('.');
      if (pos != std::string::npos) {
        output_filename.erase(pos);
      }
      switch (config_result.mode) {
        case ProgramMode::EMIT_ASSEMBLY: {
          config_result.output_filename = output_filename + ".s";
        }
        break;
        case ProgramMode::EMIT_OBJECT: {
          config_result.output_filename = output_filename + ".o";
        }
        break;
        case ProgramMode::EMIT_LLVM_IR: {
          config_result.output_filename = output_filename + ".ll";
        }
        default:
          break;
      }
    }
    return config_result;

  } catch (const cxxopts::OptionException& e) {
    std::cerr << "Error: Parsing options: " << e.what() << std::endl;
    exit(2);
  }
}

int main(int argc, char* argv[]) {
  ProgramConfig config = parse_program_options(argc, argv);
  ProgramContext context;
  Driver driver(context);
  std::cout << "in :" << config.input_filename << " out :" << config.output_filename << std::endl;
  bool res = driver.parse_file(config.input_filename);
  if (res == false) {
    std::cerr << "Error occurred, exiting..." << std::endl;
    return 1;
  }
  if (config.mode == ProgramMode::DUMP_AST) {
    Printer printer(std::cout);
    context.get_program()->accept(printer);
  } else {
    //TODO: add semantic check
    // SemanticChecker checker;
    // context.get_program()->accept(checker);
    CodeGenerator generator(config.input_filename);
    context.get_program()->accept(generator);
    if (config.mode == ProgramMode::EMIT_LLVM_IR) {
      generator.output(config.output_filename);
    }
  }
  return 0;
}