#ifndef LLVM_WRAPPER_H
#define LLVM_WRAPPER_H

#include "module.h"
// #include "wrapper.def"
#include "llvm-c/Core.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;
 
#ifdef __cplusplus
#define EXPORT extern "C" {
#define EXPORTEND }
#else
#define EXPORT
#define EXPORTEND
#endif

EXPORT

typedef struct KOpaqueModule *KModuleRef;

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(KModule, KModuleRef)
void initLLVM();
void destory(KModuleRef module);

LLVMValueRef getOrInsertFunction(const char *name, LLVMModuleRef module,
                                 LLVMTypeRef functionTy);

KModuleRef createKModule(char *name);
void kModuleInit(KModuleRef module);
void writeOutput(KModuleRef module);
LLVMModuleRef getModule(KModuleRef ref);
LLVMContextRef getLLVMContext(KModuleRef ref);
LLVMPassManagerRef getFPM(KModuleRef ref);
KModule * getM(KModuleRef ref);

 LLVMAttributeRef LLVMCreateStructRetAttr(LLVMContextRef C, LLVMTypeRef Ty);
// void insertBBFunction( LLVMBuilderRef ref, LLVMBasicBlockRef bb);
// LLVMBuilderRef createBuilder(KModuleRef module);
// LLVMBasicBlockRef createBaseBlock(KModuleRef module, char *name);

EXPORTEND

#endif