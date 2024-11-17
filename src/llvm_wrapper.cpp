
#include "llvm_wrapper.h"
#include <functional>
#include <llvm-c/IRReader.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IRReader/IRReader.h>

#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Support/SourceMgr.h>

#include <llvm-c/DebugInfo.h>

#include "llvm/Support/TargetSelect.h"

void initLLVM() {
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmPrinters();
  InitializeAllAsmParsers();
  InitializeAllTargetInfos();
}

LLVMValueRef getOrInsertFunction(const char *name, LLVMModuleRef module,
                                 LLVMTypeRef functionTy) {
  return wrap(unwrap(module)
                  ->getOrInsertFunction(StringRef(name),
                                        unwrap<FunctionType>(functionTy))
                  .getCallee());
}

LLVMMetadataRef LLVMCreateCompileUnit(LLVMDIBuilderRef builder, char *fileName,
                                      char *dirName) {
  auto dBuilder = unwrap(builder);
  auto file = dBuilder->createFile(fileName, dirName);
  auto unit =
      dBuilder->createCompileUnit(dwarf::DW_LANG_C11, file, "", false, "", 0,"", DICompileUnit::FullDebug,0,false);

  return wrap(unit);
}

LLVMTargetMachineRef createTarget(LLVMModuleRef M, char *tripleStr) {
  Triple TargetTriple(Triple::normalize(tripleStr));
  auto module = unwrap(M);
  module->setTargetTriple(TargetTriple.getTriple());

  std::string Error;
  const llvm::Target *Target =
      TargetRegistry::lookupTarget(TargetTriple.getTriple(), Error);

  if (Target == nullptr) {
    errs() << "error\n" << Error;
    return nullptr;
  }

  TargetOptions opt;

  auto CPU = "";
  auto Features = "";

  auto RM = std::optional<Reloc::Model>();

  auto targetMachine = Target->createTargetMachine(TargetTriple.getTriple(),
                                                   CPU, Features, opt, RM);

  auto dataLayout = targetMachine->createDataLayout();

  module->setDataLayout(dataLayout);
  return wrap(targetMachine);
}

void LLVMAddFlag(LLVMModuleRef M,
                           int MergeBehavior,
                           const char *Name, uint32_t Value) {
  unwrap(M)->addModuleFlag(static_cast<Module::ModFlagBehavior>(MergeBehavior), Name, Value);
}

void writeOutput(LLVMModuleRef M, LLVMTargetMachineRef target, int index,
                 char *name) {
  auto fileName = name;
  std::error_code EC;
  raw_fd_ostream fd(fileName, EC, sys::fs::OF_None);

  auto module = unwrap(M);
  auto targetMachine = unwrap(target);
  if (EC) {
    errs() << EC.message();
    return;
  }
  legacy::PassManager ff;
  LLVMAddAnalysisPasses(target, wrap(&ff));

  TargetLibraryInfoImpl TLII(Triple(module->getTargetTriple()));

  ff.add(new TargetLibraryInfoWrapperPass(TLII));

  auto FileType = static_cast<CodeGenFileType>(index);
  if (targetMachine->addPassesToEmitFile(ff, fd, nullptr, FileType, false)) {
    errs() << "TheTargetMachine can't emit a file of this type";
    return;
  }

  ff.run(*module);
}

static OptimizationLevel fromRust(LLVMRustPassBuilderOptLevel Level) {
  switch (Level) {
  case LLVMRustPassBuilderOptLevel::O0:
    return OptimizationLevel::O0;
  case LLVMRustPassBuilderOptLevel::O1:
    return OptimizationLevel::O1;
  case LLVMRustPassBuilderOptLevel::O2:
    return OptimizationLevel::O2;
  case LLVMRustPassBuilderOptLevel::O3:
    return OptimizationLevel::O3;
  case LLVMRustPassBuilderOptLevel::Os:
    return OptimizationLevel::Os;
  case LLVMRustPassBuilderOptLevel::Oz:
    return OptimizationLevel::Oz;
  default:
    report_fatal_error("Bad PassBuilderOptLevel.");
  }
}
#define LLVM_VERSION_GE(major, minor)                                          \
  (LLVM_VERSION_MAJOR > (major) ||                                             \
   LLVM_VERSION_MAJOR == (major) && LLVM_VERSION_MINOR >= (minor))

/// TODO:
void optimize(LLVMModuleRef M, LLVMTargetMachineRef target,
              LLVMRustPassBuilderOptLevel OptLevelRust,
              LLVMRustOptStage OptStage, bool NoPrepopulatePasses,
              bool VerifyIR, bool UseThinLTOBuffers, bool MergeFunctions,
              bool UnrollLoops, bool SLPVectorize, bool LoopVectorize,
              bool DisableSimplifyLibCalls) {
  Module *TheModule = unwrap(M);
  TargetMachine *TM = unwrap(target);
  OptimizationLevel OptLevel = fromRust(OptLevelRust);
  LLVMRustSanitizerOptions *SanitizerOptions = nullptr;

  PipelineTuningOptions PTO;
  PTO.LoopUnrolling = UnrollLoops;
  PTO.LoopInterleaving = UnrollLoops;
  PTO.LoopVectorization = LoopVectorize;
  PTO.SLPVectorization = SLPVectorize;
  PTO.MergeFunctions = MergeFunctions;

  // FIXME: We may want to expose this as an option.
  bool DebugPassManager = false;

  char *PGOGenPath = nullptr;
  char *PGOUsePath = nullptr;
  bool InstrumentCoverage = false;
  char *InstrProfileOutput;
  bool InstrumentGCOV = false;
  char *PGOSampleUsePath = nullptr;
  bool DebugInfoForProfiling = false;
  //  void *LlvmSelfProfiler,
  //  LLVMRustSelfProfileBeforePassCallback BeforePassCallback,
  //  LLVMRustSelfProfileAfterPassCallback AfterPassCallback,
  char *ExtraPasses = nullptr;
  int ExtraPassesLen = 0;
  char *LLVMPlugins = nullptr;
  int LLVMPluginsLen = 0;

  PassInstrumentationCallbacks PIC;

  StandardInstrumentations SI(TheModule->getContext(), DebugPassManager);

  SI.registerCallbacks(PIC);

  // if (LlvmSelfProfiler) {
  //   LLVMSelfProfileInitializeCallbacks(PIC, LlvmSelfProfiler,
  //                                      BeforePassCallback,
  //                                      AfterPassCallback);
  // }

  std::optional<PGOOptions> PGOOpt;
  auto FS = vfs::getRealFileSystem();
  std::optional<PGOOptions::PGOAction> pGAction;

  auto path = "";

  if (PGOGenPath) {
    assert(!PGOUsePath && !PGOSampleUsePath);
    pGAction = PGOOptions::IRInstr;
    path = PGOGenPath;
  } else if (PGOUsePath) {
    assert(!PGOSampleUsePath);
    pGAction = PGOOptions::IRUse;
    path = PGOUsePath;
  } else if (PGOSampleUsePath) {
    pGAction = PGOOptions::SampleUse;
    path = PGOSampleUsePath;
  } else if (DebugInfoForProfiling) {
    pGAction = PGOOptions::NoAction;
  }

  if (pGAction.has_value()) {
    PGOOpt = PGOOptions(path, "", "",
#if LLVM_VERSION_GE(17, 0)
                        "", FS,
#endif
                        pGAction.value(), PGOOptions::NoCSAction,
                        DebugInfoForProfiling);
  }

  PassBuilder PB(TM, PTO, PGOOpt, &PIC);
  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;

  FAM.registerPass([&] { return PB.buildDefaultAAPipeline(); });

  Triple TargetTriple(TheModule->getTargetTriple());
  std::unique_ptr<TargetLibraryInfoImpl> TLII(
      new TargetLibraryInfoImpl(TargetTriple));
  if (DisableSimplifyLibCalls)
    TLII->disableAllFunctions();
  FAM.registerPass([&] { return TargetLibraryAnalysis(*TLII); });

  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  // We manually collect pipeline callbacks so we can apply them at O0, where
  // the PassBuilder does not create a pipeline.
  std::vector<std::function<void(ModulePassManager &, OptimizationLevel)>>
      PipelineStartEPCallbacks;
  std::vector<std::function<void(ModulePassManager &, OptimizationLevel)>>
      OptimizerLastEPCallbacks;

  if (SanitizerOptions && SanitizerOptions->SanitizeCFI &&
      !NoPrepopulatePasses) {
    PipelineStartEPCallbacks.push_back(
        [](ModulePassManager &MPM, OptimizationLevel Level) {
          MPM.addPass(LowerTypeTestsPass(/*ExportSummary=*/nullptr,
                                         /*ImportSummary=*/nullptr,
                                         /*DropTypeTests=*/false));
        });
  }

  if (VerifyIR) {
    PipelineStartEPCallbacks.push_back(
        [VerifyIR](ModulePassManager &MPM, OptimizationLevel Level) {
          MPM.addPass(VerifierPass());
        });
  }

  if (InstrumentGCOV) {
    PipelineStartEPCallbacks.push_back(
        [](ModulePassManager &MPM, OptimizationLevel Level) {
          MPM.addPass(GCOVProfilerPass(GCOVOptions::getDefault()));
        });
  }

  if (InstrumentCoverage) {
    PipelineStartEPCallbacks.push_back(
        [InstrProfileOutput](ModulePassManager &MPM, OptimizationLevel Level) {
          InstrProfOptions Options;
          if (InstrProfileOutput) {
            Options.InstrProfileOutput = InstrProfileOutput;
          }
          MPM.addPass(InstrProfilingLoweringPass(Options, false));
        });
  }

  if (SanitizerOptions) {
    if (SanitizerOptions->SanitizeMemory) {
      MemorySanitizerOptions Options(
          SanitizerOptions->SanitizeMemoryTrackOrigins,
          SanitizerOptions->SanitizeMemoryRecover,
          /*CompileKernel=*/false,
          /*EagerChecks=*/true);
      OptimizerLastEPCallbacks.push_back(
          [Options](ModulePassManager &MPM, OptimizationLevel Level) {
            MPM.addPass(MemorySanitizerPass(Options));
          });
    }

    if (SanitizerOptions->SanitizeThread) {
      OptimizerLastEPCallbacks.push_back([](ModulePassManager &MPM,
                                            OptimizationLevel Level) {
        MPM.addPass(ModuleThreadSanitizerPass());
        MPM.addPass(createModuleToFunctionPassAdaptor(ThreadSanitizerPass()));
      });
    }

    if (SanitizerOptions->SanitizeAddress ||
        SanitizerOptions->SanitizeKernelAddress) {
      OptimizerLastEPCallbacks.push_back(
          [SanitizerOptions](ModulePassManager &MPM, OptimizationLevel Level) {
            auto CompileKernel = SanitizerOptions->SanitizeKernelAddress;
            AddressSanitizerOptions opts = AddressSanitizerOptions{
                CompileKernel,
                SanitizerOptions->SanitizeAddressRecover ||
                    SanitizerOptions->SanitizeKernelAddressRecover,
                /*UseAfterScope=*/true,
                AsanDetectStackUseAfterReturnMode::Runtime,
            };
            MPM.addPass(AddressSanitizerPass(opts));
          });
    }
    if (SanitizerOptions->SanitizeHWAddress) {
      OptimizerLastEPCallbacks.push_back(
          [SanitizerOptions](ModulePassManager &MPM, OptimizationLevel Level) {
            HWAddressSanitizerOptions opts(
                /*CompileKernel=*/false,
                SanitizerOptions->SanitizeHWAddressRecover,
                /*DisableOptimization=*/false);
            MPM.addPass(HWAddressSanitizerPass(opts));
          });
    }
  }

  if (LLVMPluginsLen) {
    auto PluginsStr = StringRef(LLVMPlugins, LLVMPluginsLen);
    SmallVector<StringRef> Plugins;
    PluginsStr.split(Plugins, ',', -1, false);
    for (auto PluginPath : Plugins) {
      auto Plugin = PassPlugin::Load(PluginPath.str());
      if (!Plugin) {
        errs() << "Failed to load pass plugin" + PluginPath.str();
        return;
      }
      Plugin->registerPassBuilderCallbacks(PB);
    }
  }

  ModulePassManager MPM;
  bool NeedThinLTOBufferPasses = UseThinLTOBuffers;
  if (!NoPrepopulatePasses) {
    // The pre-link pipelines don't support O0 and require using
    // budilO0DefaultPipeline() instead. At the same time, the LTO pipelines do
    // support O0 and using them is required.
    bool IsLTO = OptStage == LLVMRustOptStage::ThinLTO ||
                 OptStage == LLVMRustOptStage::FatLTO;
    if (OptLevel == OptimizationLevel::O0 && !IsLTO) {
      for (const auto &C : PipelineStartEPCallbacks)
        PB.registerPipelineStartEPCallback(C);
      for (const auto &C : OptimizerLastEPCallbacks)
        PB.registerOptimizerLastEPCallback(C);

      // Pass false as we manually schedule ThinLTOBufferPasses below.
      MPM = PB.buildO0DefaultPipeline(OptLevel, /* PreLinkLTO */ false);
    } else {
      for (const auto &C : PipelineStartEPCallbacks)
        PB.registerPipelineStartEPCallback(C);
      if (OptStage != LLVMRustOptStage::PreLinkThinLTO) {
        for (const auto &C : OptimizerLastEPCallbacks)
          PB.registerOptimizerLastEPCallback(C);
      }

      switch (OptStage) {
      case LLVMRustOptStage::PreLinkNoLTO:
        MPM = PB.buildPerModuleDefaultPipeline(OptLevel, DebugPassManager);
        break;
      case LLVMRustOptStage::PreLinkThinLTO:
        MPM = PB.buildThinLTOPreLinkDefaultPipeline(OptLevel);
        // The ThinLTOPreLink pipeline already includes ThinLTOBuffer passes.
        // However, callback passes may still run afterwards. This means we need
        // to run the buffer passes again.
        // FIXME: In LLVM 13, the ThinLTOPreLink pipeline also runs
        // OptimizerLastEPCallbacks before the RequiredLTOPreLinkPasses, in
        // which case we can remove these hacks.
        if (OptimizerLastEPCallbacks.empty())
          NeedThinLTOBufferPasses = false;
        for (const auto &C : OptimizerLastEPCallbacks)
          C(MPM, OptLevel);
        break;
      case LLVMRustOptStage::PreLinkFatLTO:
        MPM = PB.buildLTOPreLinkDefaultPipeline(OptLevel);
        NeedThinLTOBufferPasses = false;
        break;
      case LLVMRustOptStage::ThinLTO:
        // FIXME: Does it make sense to pass the ModuleSummaryIndex?
        // It only seems to be needed for C++ specific optimizations.
        MPM = PB.buildThinLTODefaultPipeline(OptLevel, nullptr);
        break;
      case LLVMRustOptStage::FatLTO:
        MPM = PB.buildLTODefaultPipeline(OptLevel, nullptr);
        break;
      }
    }
  } else {
    // We're not building any of the default pipelines but we still want to
    // add the verifier, instrumentation, etc passes if they were requested
    for (const auto &C : PipelineStartEPCallbacks)
      C(MPM, OptLevel);
    for (const auto &C : OptimizerLastEPCallbacks)
      C(MPM, OptLevel);
  }

  if (ExtraPassesLen) {
    if (auto Err =
            PB.parsePassPipeline(MPM, StringRef(ExtraPasses, ExtraPassesLen))) {
      std::string ErrMsg = toString(std::move(Err));
      errs() << ErrMsg.c_str() << '\n';
      return;
    }
  }

  if (NeedThinLTOBufferPasses) {
    MPM.addPass(CanonicalizeAliasesPass());
    MPM.addPass(NameAnonGlobalPass());
  }

  // Upgrade all calls to old intrinsics first.
  for (Module::iterator I = TheModule->begin(), E = TheModule->end(); I != E;)
    UpgradeCallsToIntrinsic(&*I++); // must be post-increment, as we remove

  MPM.run(*TheModule, MAM);
  return;
}
