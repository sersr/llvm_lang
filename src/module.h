#ifndef K_MODULE_H
#define K_MODULE_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Target/TargetMachine.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Object/ArchiveWriter.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "type.h"

#include <vector>

using namespace llvm;

class KModule {
public:
  Module *getModule() const { return module; }
  LLVMContext *getContext() const { return &module->getContext(); }
  TargetMachine *getTargetMachine() const { return targetMachine; }
  legacy::FunctionPassManager *getFPM() const { return FPM; };
  KModule(Module *module, legacy::FunctionPassManager *fpm);

  void init();
  void writeOutput(int index, char *name);
  void initTargetMachine();
  ~KModule();

private:
  Module *module;
  TargetMachine *targetMachine;
  legacy::FunctionPassManager *FPM;
};

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(KModule, KModuleRef)

KModule *getM(KModuleRef ref);
#endif