#ifndef LLVM_WRAPPER_H
#define LLVM_WRAPPER_H

#ifdef __cplusplus
#define EXPORT extern "C" {
#define EXPORTEND }
#else
#ifdef FFIGEN
// Declare all of the target-initialization functions that are available.
#define LLVM_TARGET(TargetName) void LLVMInitialize##TargetName##TargetInfo();
#include "llvm/Config/Targets.def"

#define LLVM_TARGET(TargetName) void LLVMInitialize##TargetName##Target();
#include "llvm/Config/Targets.def"

// Declare all of the target-MC-initialization functions that are available.
#define LLVM_TARGET(TargetName) void LLVMInitialize##TargetName##TargetMC();
#include "llvm/Config/Targets.def"

// Declare all of the available assembly printer initialization functions.
#define LLVM_ASM_PRINTER(TargetName)                                           \
  void LLVMInitialize##TargetName##AsmPrinter();
#include "llvm/Config/AsmPrinters.def"

// Declare all of the available assembly parser initialization functions.
#define LLVM_ASM_PARSER(TargetName)                                            \
  void LLVMInitialize##TargetName##AsmParser();
#include "llvm/Config/AsmParsers.def"

// Declare all of the available disassembler initialization functions.
#define LLVM_DISASSEMBLER(TargetName)                                          \
  void LLVMInitialize##TargetName##Disassembler();
#include "llvm/Config/Disassemblers.def"

// Declare all of the available TargetMCA initialization functions.
#define LLVM_TARGETMCA(TargetName) void LLVMInitialize##TargetName##TargetMCA();
#include "llvm/Config/TargetMCAs.def"

#endif

#define EXPORT
#define EXPORTEND
#endif

typedef enum {
#define GET_ATTR_ENUM(ENUM_NAME, DISPLAY_NAME)
#include "llvm/IR/Attributes.inc"
} LLVMAttr;

#include "llvm-c/Core.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

#include "module.h"

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

LLVMAttributeRef LLVMCreateStructRetAttr(LLVMContextRef C, LLVMTypeRef Ty);

LLVMMetadataRef LLVMCreateCompileUnit(LLVMDIBuilderRef builder, char *fileName,
                                      char *dirName);

void LLVMIRReader(LLVMContextRef context, char *name, char *outName);

void optimize(KModuleRef kModule, LLVMRustPassBuilderOptLevel OptLevelRust,
              LLVMRustOptStage OptStage, bool NoPrepopulatePasses,
              bool VerifyIR, bool UseThinLTOBuffers, bool MergeFunctions,
              bool UnrollLoops, bool SLPVectorize, bool LoopVectorize,
              bool DisableSimplifyLibCalls);
EXPORTEND

#endif