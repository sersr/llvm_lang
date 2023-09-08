#ifndef LLVM_WRAPPER_H
#define LLVM_WRAPPER_H

#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/IR/AutoUpgrade.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Object/ArchiveWriter.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Config/llvm-config.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <llvm/Support/PGOOptions.h>
#include <llvm/Support/VirtualFileSystem.h>

/// Transform
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Instrumentation.h"
#include "llvm/Transforms/Instrumentation/AddressSanitizer.h"
#include "llvm/Transforms/Instrumentation/GCOVProfiler.h"
#include "llvm/Transforms/Instrumentation/HWAddressSanitizer.h"
#include "llvm/Transforms/Instrumentation/InstrProfiling.h"
#include "llvm/Transforms/Instrumentation/MemorySanitizer.h"
#include "llvm/Transforms/Instrumentation/ThreadSanitizer.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/CanonicalizeAliases.h"
#include "llvm/Transforms/Utils/NameAnonGlobals.h"
#include <llvm/Transforms/IPO/LowerTypeTests.h>

#include <llvm-c/TargetMachine.h>

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
// #include "llvm/IR/Module.h"

using namespace llvm;
DEFINE_STDCXX_CONVERSION_FUNCTIONS(TargetMachine, LLVMTargetMachineRef)
DEFINE_STDCXX_CONVERSION_FUNCTIONS(Target, LLVMTargetRef)

// #include "module.h"

enum LLVMRustPassBuilderOptLevel {
  O0,
  O1,
  O2,
  O3,
  Os,
  Oz,
};
enum LLVMRustOptStage {
  PreLinkNoLTO,
  PreLinkThinLTO,
  PreLinkFatLTO,
  ThinLTO,
  FatLTO,
};
typedef struct {
  bool SanitizeAddress;
  bool SanitizeAddressRecover;
  bool SanitizeMemory;
  bool SanitizeMemoryRecover;
  int SanitizeMemoryTrackOrigins;
  bool SanitizeThread;
  bool SanitizeHWAddress;
  bool SanitizeHWAddressRecover;
  bool SanitizeKernelAddress;
  bool SanitizeKernelAddressRecover;
  bool SanitizeCFI;
} LLVMRustSanitizerOptions;

EXPORT

void initLLVM();

LLVMValueRef getOrInsertFunction(const char *name, LLVMModuleRef module,
                                 LLVMTypeRef functionTy);

LLVMMetadataRef LLVMCreateCompileUnit(LLVMDIBuilderRef builder, char *fileName,
                                      char *dirName);

LLVMTargetMachineRef createTarget(LLVMModuleRef M, char *tripleStr);

void optimize(LLVMModuleRef M, LLVMTargetMachineRef target,
              LLVMRustPassBuilderOptLevel OptLevelRust,
              LLVMRustOptStage OptStage, bool NoPrepopulatePasses,
              bool VerifyIR, bool UseThinLTOBuffers, bool MergeFunctions,
              bool UnrollLoops, bool SLPVectorize, bool LoopVectorize,
              bool DisableSimplifyLibCalls);

void writeOutput(LLVMModuleRef M, LLVMTargetMachineRef target, int index,
                 char *name);
EXPORTEND

#endif