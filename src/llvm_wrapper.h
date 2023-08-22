#ifndef LLVM_WRAPPER_H
#define LLVM_WRAPPER_H

#include "type.h"
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

void initLLVM();
void destory(KModuleRef module);

LLVMValueRef getOrInsertFunction(const char *name, LLVMModuleRef module,
                                 LLVMTypeRef functionTy);

KModuleRef createKModule(char *name);
void kModuleInit(KModuleRef module);
void writeOutput(KModuleRef module, int index, char *name);
LLVMModuleRef getModule(KModuleRef ref);
LLVMContextRef getLLVMContext(KModuleRef ref);
LLVMPassManagerRef getFPM(KModuleRef ref);

LLVMAttributeRef LLVMCreateStructRetAttr(LLVMContextRef C, LLVMTypeRef Ty);

LLVMMetadataRef LLVMCreateCompileUnit(LLVMDIBuilderRef builder, char *fileName,
                                      char *dirName);
EXPORTEND

#endif