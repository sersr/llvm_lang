#ifndef K_MODULE_H
#define K_MODULE_H

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

using namespace llvm;

typedef struct KOpaqueModule *KModuleRef;

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
// extern "C" typedef void (*LLVMRustSelfProfileBeforePassCallback)(
//     void *,        // LlvmSelfProfiler
//     const char *,  // pass name
//     const char *); // IR name
// extern "C" typedef void (*LLVMRustSelfProfileAfterPassCallback)(
//     void *); // LlvmSelfProfiler
#include <llvm-c/TargetMachine.h>

DEFINE_STDCXX_CONVERSION_FUNCTIONS(TargetMachine, LLVMTargetMachineRef)

class KModule {
public:
  Module *getModule() const { return module; }
  LLVMContext *getContext() const { return &module->getContext(); }
  TargetMachine *getTargetMachine() const { return targetMachine; }
  // legacy::FunctionPassManager *getFPM() const { return FPM; };
  KModule(Module *module);

  void init();
  void writeOutput(int index, char *name);
  void initTargetMachine();

  void optimize(LLVMRustPassBuilderOptLevel OptLevelRust,
                LLVMRustOptStage OptStage, bool NoPrepopulatePasses,
                bool VerifyIR, bool UseThinLTOBuffers, bool MergeFunctions,
                bool UnrollLoops, bool SLPVectorize, bool LoopVectorize,
                bool DisableSimplifyLibCalls);

  ~KModule();

private:
  Module *module;
  TargetMachine *targetMachine;
  // legacy::FunctionPassManager *FPM;
};

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(KModule, KModuleRef)

KModule *getM(KModuleRef ref);
#endif