
#include "llvm_wrapper.h"
#include <functional>
#include <vector>

// KModule *unwrap(KModuleRef module) {
//   return reinterpret_cast<KModule *>(module);
// }

// KModuleRef wrap(KModule *module) {
//   return wrap(module);
// }

void initLLVM() {
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmParsers();
  InitializeAllAsmPrinters();
}

LLVMValueRef getOrInsertFunction(const char *name, LLVMModuleRef module,
                                 LLVMTypeRef functionTy) {
  return wrap(unwrap(module)
                  ->getOrInsertFunction(StringRef(name),
                                        unwrap<FunctionType>(functionTy))
                  .getCallee());
}

KModuleRef createKModule(char *name) {
  auto context = new LLVMContext();
  auto m = new Module(name, *context);
  auto FPM = new legacy::FunctionPassManager(m);
  auto module = new KModule(m, FPM);
  module->init();
  FPM->add(createPromoteMemoryToRegisterPass());

  return reinterpret_cast<KModuleRef>(module);
}

void destory(KModuleRef module) {
  auto m = unwrap(module);
  delete m;
}

void kModuleInit(KModuleRef module) { unwrap(module)->init(); }

void writeOutput(KModuleRef module) { unwrap(module)->writeOutput(); }

LLVMModuleRef getModule(KModuleRef ref) {
  return wrap(unwrap(ref)->getModule());
}

KModule *getM(KModuleRef ref) { return unwrap(ref); }

LLVMContextRef getLLVMContext(KModuleRef ref) {
  return wrap(unwrap(ref)->getContext());
}

LLVMPassManagerRef getFPM(KModuleRef ref) {
  return wrap(unwrap(ref)->getFPM());
}

// LLVMBuilderRef createBuilder(KModuleRef module) {
//   return LLVMCreateBuilderInContext(wrap(unwrap(module)->getContext()));
// }

// LLVMBasicBlockRef createBaseBlock(KModuleRef module, char *name) {
//   return LLVMCreateBasicBlockInContext(wrap(unwrap(module)->getContext()),
//                                        name);
// }
