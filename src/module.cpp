
#include "module.h"
#include <llvm-c/Core.h>
#include <llvm/Analysis/TargetLibraryInfo.h>

KModule::KModule(Module *module, legacy::FunctionPassManager *fpm)
    : module(module), FPM(std::move(fpm)) {}

void KModule::init() {

  auto TargetTriple = "arm64-apple-macosx14.0.0";
  // sys::getDefaultTargetTriple();
  module->setTargetTriple(TargetTriple);
  // auto  x = Triple();
  // x.setArch(Triple::x86_64);
  // x.setVendor(Triple::AMD);
  // x.setOS(Triple::MacOSX);
  // auto TargetTriple = x.getTriple();

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  TargetOptions opt;

  auto CPU = "";
  auto Features = "";

  targetMachine = Target->createTargetMachine(
      TargetTriple, CPU, Features, opt, Reloc::PIC_, module->getCodeModel());

  module->setDataLayout(targetMachine->createDataLayout());
}

void KModule::writeOutput(int index, char *name) {
  auto fileName = name;
  std::error_code EC;
  raw_fd_ostream fd(fileName, EC, sys::fs::OF_None);

  if (EC) {
    errs() << EC.message();
    return;
  }
  legacy::PassManager ff;

  TargetLibraryInfoImpl TLII(Triple(module->getTargetTriple()));

  ff.add(new TargetLibraryInfoWrapperPass(TLII));

  auto FileType = static_cast<CodeGenFileType>(index);
  if (getTargetMachine()->addPassesToEmitFile(ff, fd, nullptr, FileType,
                                              true)) {
    errs() << "TheTargetMachine can't emit a file of this type";
    return;
  }

  ff.run(*module);
}

KModule::~KModule() { delete module; }

KModule *getM(KModuleRef ref) { return unwrap(ref); }