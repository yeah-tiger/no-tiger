#include "codegen.hpp"
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
namespace ntc {
CodeGenerator::CodeGenerator(const std::string& module_id)
    :module_id_(module_id), module_(std::make_unique<llvm::Module>(module_id, llvm_context)),
      builder_(llvm::IRBuilder<>(llvm_context)) {}

llvm::Value* CodeGenerator::visit(AST& ast) { return ast.accept(*this); }

llvm::Value* CodeGenerator::visit(BlockItem& block_item) {
  return block_item.accept(*this);
}

llvm::Value* CodeGenerator::visit(ExternalDeclaration& external_declaration) {
  return external_declaration.accept(*this);
}

llvm::Value* CodeGenerator::visit(TranslationUnit& translation_unit) {
  // auto& decls = translation_unit.get_declarations();
  // for (auto& decl : decls) {
  //   visit(*decl);
  // }
  auto *func_type = llvm::FunctionType::get(builder_.getInt32Ty(), false);
  auto *main_func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                          "main", module_.get());
  auto *block = llvm::BasicBlock::Create(module_->getContext(), "entry", main_func);
  builder_.SetInsertPoint(block);
  builder_.CreateRet(builder_.getInt32(42));
  return nullptr;
}

llvm::Value* CodeGenerator::visit(FunctionDefinition&) { return nullptr; }

llvm::Value* CodeGenerator::visit(DeclarationSpecifier&) { return nullptr; }
llvm::Value* CodeGenerator::visit(Identifier&) { return nullptr; }
llvm::Value* CodeGenerator::visit(ParameterDeclaration&) { return nullptr; }
llvm::Value* CodeGenerator::visit(TypeSpecifier&) { return nullptr; }
llvm::Value* CodeGenerator::visit(Declaration&) { return nullptr; }
llvm::Value* CodeGenerator::visit(Initializer&) { return nullptr; }
llvm::Value* CodeGenerator::visit(Statement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(CompoundStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(ExpressionStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(ReturnStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(BreakStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(ContinueStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(IfStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(WhileStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(ForStatement&) { return nullptr; }
llvm::Value* CodeGenerator::visit(Expression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(IntegerExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(FloatExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(BooleanExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(CharacterExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(StringLiteralExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(BinaryOperationExpression&) {
  return nullptr;
}
llvm::Value* CodeGenerator::visit(UnaryOperationExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(ConditionalExpression&) { return nullptr; }
llvm::Value* CodeGenerator::visit(FunctionCall&) { return nullptr; }

void CodeGenerator::output() {
  std::error_code ec;
  std::cout << "Codegen::output: " << module_id_ << std::endl;
  llvm::raw_fd_ostream fd(module_id_, ec, llvm::sys::fs::F_None);
  module_->print(fd, nullptr);
}
}  // namespace ntc
