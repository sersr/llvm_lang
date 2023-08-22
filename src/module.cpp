
#include "module.h"
KModule::KModule(Module *module, legacy::FunctionPassManager *fpm)
    : module(module), FPM(std::move(fpm)) {}

void KModule::init() {

  auto TargetTriple = sys::getDefaultTargetTriple();
  // module->setTargetTriple(TargetTriple);
  // auto  x = Triple();
  // x.setArch(Triple::x86_64);
  // x.setVendor(Triple::AMD);
  // x.setOS(Triple::MacOSX);
  // auto TargetTriple = x.getTriple();

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  TargetOptions opt;

  auto RM = std::optional<Reloc::Model>();
  auto CPU = "generic";
  auto Features = "";

  targetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  module->setTargetTriple(TargetTriple);
  module->setDataLayout(targetMachine->createDataLayout());
}

void KModule::writeOutput(int index, char *name) {
  auto fileName = name;
  std::error_code EC;
  raw_fd_ostream fd(fileName, EC, (sys::fs::OpenFlags)0);
  if (EC) {
    errs() << EC.message();
    return;
  }
  legacy::PassManager ff;
  auto FileType = static_cast<CodeGenFileType>(index);
  if (getTargetMachine()->addPassesToEmitFile(ff, fd, nullptr, FileType)) {
    errs() << "TheTargetMachine can't emit a file of this type";
    return;
  }

  ff.run(*module);
  fd.flush();
}

KModule::~KModule() { delete module; }

KModule *getM(KModuleRef ref) { return unwrap(ref); }