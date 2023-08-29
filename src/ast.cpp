#include "ast.h"

#include "llvm/IR/BasicBlock.h"

Type *getType(LLVMContext *context, LLType type) {
  return Type::getDoubleTy(*context);
}

FunctionAst::FunctionAst(Pos pos, string name, LLType returnType,
                         vector<Param> &params)
    : pos(pos), name(name), returnType(returnType), params(params) {}

Value *FunctionAst::codegen(KModule &module) {
  auto builder = IRBuilder(*module.getContext());
  auto para = vector<Type *>(params.size());
  for (Param p : params) {
    para.push_back(p.getType(module.getContext()));
  }
  auto functionType =
      FunctionType::get(getType(module.getContext(), returnType), para, false);
  auto function = Function::Create(functionType, Function::ExternalLinkage,
                                   name, *module.getModule());
  auto bb = BasicBlock::Create(*module.getContext(), "entry", function);

  builder.SetInsertPoint(bb);

  auto then = BasicBlock::Create(*module.getContext(), "then", function);
  auto elseF = BasicBlock::Create(*module.getContext(), "else");
  auto after = BasicBlock::Create(*module.getContext(), "after");

  auto booltype = Type::getInt32Ty(*module.getContext());
  auto v = ConstantInt::getBool(*module.getContext(), false);
  builder.CreateCondBr(v, then, elseF);
  builder.SetInsertPoint(then);
  builder.CreateBr(after);

  function->insert(function->end(), elseF);
  builder.SetInsertPoint(elseF);
  builder.CreateBr(after);
  function->insert(function->end(), after);
  builder.SetInsertPoint(after);
  auto ret = ConstantFP::get(*module.getContext(), APFloat(1.0));
  builder.CreateRet(ret);
  // builder.CreateRetVoid();
  verifyFunction(*function, &errs());
  return function;
}
