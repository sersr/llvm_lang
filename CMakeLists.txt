cmake_minimum_required(VERSION 3.18)

project(llvm_wrapper)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM CONFIG REQUIRED)
message(${LLVM_LIBRARY_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})


include_directories(${LLVM_INCLUDE_DIRS})

if(WIN32)
    link_libraries(
        LLVMWindowsManifest.lib LLVMXRay.lib LLVMLibDriver.lib LLVMDlltoolDriver.lib
        LLVMTelemetry.lib LLVMTextAPIBinaryReader.lib LLVMCoverage.lib LLVMLineEditor.lib
        LLVMNVPTXCodeGen.lib LLVMNVPTXDesc.lib LLVMNVPTXInfo.lib LLVMRISCVTargetMCA.lib
        LLVMRISCVDisassembler.lib LLVMRISCVAsmParser.lib LLVMRISCVCodeGen.lib
        LLVMRISCVDesc.lib LLVMRISCVInfo.lib LLVMWebAssemblyDisassembler.lib
        LLVMWebAssemblyAsmParser.lib LLVMWebAssemblyCodeGen.lib LLVMWebAssemblyUtils.lib
        LLVMWebAssemblyDesc.lib LLVMWebAssemblyInfo.lib LLVMBPFDisassembler.lib
        LLVMBPFAsmParser.lib LLVMBPFCodeGen.lib LLVMBPFDesc.lib LLVMBPFInfo.lib
        LLVMX86TargetMCA.lib LLVMX86Disassembler.lib LLVMX86AsmParser.lib LLVMX86CodeGen.lib
        LLVMX86Desc.lib LLVMX86Info.lib LLVMARMDisassembler.lib LLVMARMAsmParser.lib
        LLVMARMCodeGen.lib LLVMARMDesc.lib LLVMARMUtils.lib LLVMARMInfo.lib
        LLVMAArch64Disassembler.lib LLVMAArch64AsmParser.lib LLVMAArch64CodeGen.lib
        LLVMAArch64Desc.lib LLVMAArch64Utils.lib LLVMAArch64Info.lib LLVMOrcDebugging.lib
        LLVMOrcJIT.lib LLVMWindowsDriver.lib LLVMMCJIT.lib LLVMJITLink.lib LLVMInterpreter.lib
        LLVMExecutionEngine.lib LLVMRuntimeDyld.lib LLVMOrcTargetProcess.lib LLVMOrcShared.lib
        LLVMDWP.lib LLVMDebugInfoLogicalView.lib LLVMDebugInfoGSYM.lib LLVMOption.lib
        LLVMObjectYAML.lib LLVMObjCopy.lib LLVMMCA.lib LLVMMCDisassembler.lib LLVMLTO.lib
        LLVMPasses.lib LLVMHipStdPar.lib LLVMCFGuard.lib LLVMCoroutines.lib LLVMipo.lib
        LLVMVectorize.lib LLVMSandboxIR.lib LLVMLinker.lib LLVMInstrumentation.lib
        LLVMFrontendOpenMP.lib LLVMFrontendOffloading.lib LLVMFrontendOpenACC.lib
        LLVMFrontendHLSL.lib LLVMFrontendDriver.lib LLVMFrontendAtomic.lib
        LLVMExtensions.lib LLVMDWARFLinkerParallel.lib LLVMDWARFLinkerClassic.lib
        LLVMDWARFLinker.lib LLVMGlobalISel.lib LLVMMIRParser.lib LLVMAsmPrinter.lib
        LLVMSelectionDAG.lib LLVMCodeGen.lib LLVMTarget.lib LLVMObjCARCOpts.lib
        LLVMCodeGenTypes.lib LLVMCGData.lib LLVMIRPrinter.lib LLVMInterfaceStub.lib
        LLVMFileCheck.lib LLVMFuzzMutate.lib LLVMScalarOpts.lib LLVMInstCombine.lib
        LLVMAggressiveInstCombine.lib LLVMTransformUtils.lib LLVMBitWriter.lib
        LLVMAnalysis.lib LLVMProfileData.lib LLVMSymbolize.lib LLVMDebugInfoBTF.lib
        LLVMDebugInfoPDB.lib LLVMDebugInfoMSF.lib LLVMDebugInfoCodeView.lib
        LLVMDebugInfoDWARF.lib LLVMObject.lib LLVMTextAPI.lib LLVMMCParser.lib
        LLVMIRReader.lib LLVMAsmParser.lib LLVMMC.lib LLVMBitReader.lib
        LLVMFuzzerCLI.lib LLVMCore.lib LLVMRemarks.lib LLVMBitstreamReader.lib
        LLVMBinaryFormat.lib LLVMTargetParser.lib LLVMTableGen.lib LLVMSupport.lib
        LLVMDemangle.lib
        ntdll
    )
    add_library(${PROJECT_NAME} SHARED
        "src/llvm_wrapper.cpp"
        llvm_wrapper.def
    )
else()
    # shared
    link_libraries(LLVM)
    add_library(${PROJECT_NAME} SHARED
    "src/llvm_wrapper.cpp"
    )
# static link
#     target_link_libraries(${PROJECT_NAME}
#      -Wl,--whole-archive 
#      LLVMWindowsManifest LLVMXRay LLVMLibDriver LLVMDlltoolDriver LLVMTextAPIBinaryReader LLVMCoverage LLVMLineEditor LLVMSandboxIR LLVMXCoreDisassembler LLVMXCoreCodeGen LLVMXCoreDesc LLVMXCoreInfo LLVMX86TargetMCA LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMX86Desc LLVMX86Info LLVMWebAssemblyDisassembler LLVMWebAssemblyAsmParser LLVMWebAssemblyCodeGen LLVMWebAssemblyUtils LLVMWebAssemblyDesc LLVMWebAssemblyInfo LLVMVEDisassembler LLVMVEAsmParser LLVMVECodeGen LLVMVEDesc LLVMVEInfo LLVMSystemZDisassembler LLVMSystemZAsmParser LLVMSystemZCodeGen LLVMSystemZDesc LLVMSystemZInfo LLVMSparcDisassembler LLVMSparcAsmParser LLVMSparcCodeGen LLVMSparcDesc LLVMSparcInfo LLVMRISCVTargetMCA LLVMRISCVDisassembler LLVMRISCVAsmParser LLVMRISCVCodeGen LLVMRISCVDesc LLVMRISCVInfo LLVMPowerPCDisassembler LLVMPowerPCAsmParser LLVMPowerPCCodeGen LLVMPowerPCDesc LLVMPowerPCInfo LLVMNVPTXCodeGen LLVMNVPTXDesc LLVMNVPTXInfo LLVMMSP430Disassembler LLVMMSP430AsmParser LLVMMSP430CodeGen LLVMMSP430Desc LLVMMSP430Info LLVMMipsDisassembler LLVMMipsAsmParser LLVMMipsCodeGen LLVMMipsDesc LLVMMipsInfo LLVMLoongArchDisassembler LLVMLoongArchAsmParser LLVMLoongArchCodeGen LLVMLoongArchDesc LLVMLoongArchInfo LLVMLanaiDisassembler LLVMLanaiCodeGen LLVMLanaiAsmParser LLVMLanaiDesc LLVMLanaiInfo LLVMHexagonDisassembler LLVMHexagonCodeGen LLVMHexagonAsmParser LLVMHexagonDesc LLVMHexagonInfo LLVMBPFDisassembler LLVMBPFAsmParser LLVMBPFCodeGen LLVMBPFDesc LLVMBPFInfo LLVMAVRDisassembler LLVMAVRAsmParser LLVMAVRCodeGen LLVMAVRDesc LLVMAVRInfo LLVMARMDisassembler LLVMARMAsmParser LLVMARMCodeGen LLVMARMDesc LLVMARMUtils LLVMARMInfo LLVMAMDGPUTargetMCA LLVMAMDGPUDisassembler LLVMAMDGPUAsmParser LLVMAMDGPUCodeGen LLVMAMDGPUDesc LLVMAMDGPUUtils LLVMAMDGPUInfo LLVMAArch64Disassembler LLVMAArch64AsmParser LLVMAArch64CodeGen LLVMAArch64Desc LLVMAArch64Utils LLVMAArch64Info LLVMOrcDebugging LLVMOrcJIT LLVMWindowsDriver LLVMMCJIT LLVMJITLink LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMOrcTargetProcess LLVMOrcShared LLVMDWP LLVMDebugInfoLogicalView LLVMDebugInfoGSYM LLVMOption LLVMObjectYAML LLVMObjCopy LLVMMCA LLVMMCDisassembler LLVMLTO LLVMFrontendOpenACC LLVMFrontendHLSL LLVMFrontendDriver LLVMExtensions Polly PollyISL LLVMPasses LLVMHipStdPar LLVMCoroutines LLVMCFGuard LLVMipo LLVMInstrumentation LLVMVectorize LLVMLinker LLVMFrontendOpenMP LLVMFrontendOffloading LLVMDWARFLinkerParallel LLVMDWARFLinkerClassic LLVMDWARFLinker LLVMCodeGenData LLVMGlobalISel LLVMMIRParser LLVMAsmPrinter LLVMSelectionDAG LLVMCodeGen LLVMTarget LLVMObjCARCOpts LLVMCodeGenTypes LLVMIRPrinter LLVMInterfaceStub LLVMFileCheck LLVMFuzzMutate LLVMScalarOpts LLVMInstCombine LLVMAggressiveInstCombine LLVMTransformUtils LLVMBitWriter LLVMAnalysis LLVMProfileData LLVMSymbolize LLVMDebugInfoBTF LLVMDebugInfoPDB LLVMDebugInfoMSF LLVMDebugInfoDWARF LLVMObject LLVMTextAPI LLVMMCParser LLVMIRReader LLVMAsmParser LLVMMC LLVMDebugInfoCodeView LLVMBitReader LLVMFuzzerCLI LLVMCore LLVMRemarks LLVMBitstreamReader LLVMBinaryFormat LLVMTargetParser LLVMTableGen LLVMSupport LLVMDemangle
#     -Wl,--no-whole-archive
# )
endif()

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

install(TARGETS
    ${PROJECT_NAME}
)

# list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
# include(HandleLLVMOptions)
# add_definitions(${LLVM_DEFINITIONS})

# target_include_directories(main PRIVATE ${LLVM_INCLUDE_DIRS})

# # Find the libraries that correspond to the LLVM components that we wish to use
# llvm_map_components_to_libnames(llvm_libs Support Core IRReader ...)

# # Link against LLVM libraries
# target_link_libraries(main PRIVATE ${llvm_libs})