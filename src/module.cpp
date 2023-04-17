
#include "module.h"
KModule::KModule(  Module* module,   legacy::FunctionPassManager* fpm):module(module), FPM(std::move(fpm)) {

}
void KModule::init() {

  auto TargetTriple = sys::getDefaultTargetTriple();
  // module->setTargetTriple(TargetTriple);
  // auto  x = Triple();
  // x.setArch(Triple::x86_64);
  // x.setVendor(Triple::AMD);
  // x.setOS(Triple::MacOSX);
  // auto TargetTriple = x.getTriple();
  errs() << TargetTriple << "\n" << sys::getHostCPUName();

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);
  TargetOptions opt;

  auto RM = std::optional<Reloc::Model>();
  auto CPU = "generic";
  auto Features = "";

  targetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  module->setDataLayout(targetMachine->createDataLayout());
}

void KModule::writeOutput() {
  auto fileName = "out.o";
  std::error_code EC;
  raw_fd_ostream fd(fileName, EC, (sys::fs::OpenFlags)0);
  if (EC) {
    errs() << EC.message();
    return;
  }
  legacy::PassManager ff;
  auto FileType = CGFT_ObjectFile;
  if (getTargetMachine()->addPassesToEmitFile(ff, fd, nullptr, FileType)) {
    errs() << "TheTargetMachine can't emit a file of this type";
    return;
  }

  ff.run(*module);
  fd.flush();

  // auto writer = NewArchiveMember::getFile(fileName, false);
  // auto members = std::vector<NewArchiveMember>();
  // if (writer) {
  //   members.push_back(std::move(*writer));
  // }
  // auto result =
  //     writeArchive("testx", members, false,
  //                  object::Archive::getDefaultKindForHost(), true, false);
  // if (!result) {
  //   // outs() << toString(std::move(result)) << "\n";
  // }
}

KModule::~KModule() {
  delete module;
}
