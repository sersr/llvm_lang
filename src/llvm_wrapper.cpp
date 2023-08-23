
#include "llvm_wrapper.h"
#include "module.h"
#include <functional>
#include <llvm-c/IRReader.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IRReader/IRReader.h>

#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Support/SourceMgr.h>

#include <llvm-c/DebugInfo.h>
#include <vector>

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

void writeOutput(KModuleRef module, int index, char *name) {
  unwrap(module)->writeOutput(index, name);
}

LLVMModuleRef getModule(KModuleRef ref) {
  return wrap(unwrap(ref)->getModule());
}

LLVMContextRef getLLVMContext(KModuleRef ref) {
  return wrap(unwrap(ref)->getContext());
}

LLVMPassManagerRef getFPM(KModuleRef ref) {
  return wrap(unwrap(ref)->getFPM());
}

LLVMAttributeRef LLVMCreateStructRetAttr(LLVMContextRef C, LLVMTypeRef Ty) {
  return wrap(Attribute::getWithStructRetType(*unwrap(C), unwrap(Ty)));
}

LLVMMetadataRef LLVMCreateCompileUnit(LLVMDIBuilderRef builder, char *fileName,
                                      char *dirName) {
  auto dBuilder = unwrap(builder);
  auto file = dBuilder->createFile(fileName, dirName);
  auto unit =
      dBuilder->createCompileUnit(dwarf::DW_LANG_C11, file, "", false, "", 0);

  return wrap(unit);
}

void LLVMIRReader(LLVMContextRef context, char *name, char *outName) {
  SMDiagnostic e;
  auto module = llvm::parseIRFile(name, e, *unwrap(context)).release();
  auto FPM = new legacy::FunctionPassManager(module);

  auto kModule = KModule(module, FPM);
  kModule.init();
  kModule.writeOutput(1, outName);
}