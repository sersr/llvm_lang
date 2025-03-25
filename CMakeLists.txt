cmake_minimum_required(VERSION 3.18)

project(llvm_wrapper)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM CONFIG REQUIRED)
message(${LLVM_LIBRARY_DIRS})

link_directories(${LLVM_LIBRARY_DIRS})

include_directories(${LLVM_INCLUDE_DIRS})

execute_process(
    COMMAND llvm-config --shared-mode
    OUTPUT_VARIABLE LLVM_MODE
)
execute_process(
    COMMAND llvm-config --libnames
    OUTPUT_VARIABLE llvm_libs
)
# execute_process(
#     COMMAND llvm-config --system-libs
#     OUTPUT_VARIABLE system_libs
# )
string(REPLACE "\n" "" llvm_libs ${llvm_libs})
string(REPLACE "\n" "" LLVM_MODE ${LLVM_MODE})
# string(REPLACE "\n" "" system_libs ${system_libs})

if(WIN32)
    link_libraries(${llvm_libs})

    if(${LLVM_MODE} STREQUAL "static")
        link_libraries(ntdll)
    endif()

    add_library(${PROJECT_NAME} SHARED
        "src/llvm_wrapper.cpp"
        llvm_wrapper.def
    )
else()
    # shared
    if(${LLVM_MODE} STREQUAL "shared")
        link_libraries(LLVM)
    endif()

    add_library(${PROJECT_NAME} SHARED
        "src/llvm_wrapper.cpp"
    )

    # static link
    if(${LLVM_MODE} STREQUAL "static")
        target_link_libraries(${PROJECT_NAME}
            -Wl,--whole-archive
            LLVMWindowsManifest LLVMXRay LLVMLibDriver LLVMDlltoolDriver LLVMTextAPIBinaryReader LLVMCoverage LLVMLineEditor LLVMSandboxIR LLVMXCoreDisassembler LLVMXCoreCodeGen LLVMXCoreDesc LLVMXCoreInfo LLVMX86TargetMCA LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMX86Desc LLVMX86Info LLVMWebAssemblyDisassembler LLVMWebAssemblyAsmParser LLVMWebAssemblyCodeGen LLVMWebAssemblyUtils LLVMWebAssemblyDesc LLVMWebAssemblyInfo LLVMVEDisassembler LLVMVEAsmParser LLVMVECodeGen LLVMVEDesc LLVMVEInfo LLVMSystemZDisassembler LLVMSystemZAsmParser LLVMSystemZCodeGen LLVMSystemZDesc LLVMSystemZInfo LLVMSparcDisassembler LLVMSparcAsmParser LLVMSparcCodeGen LLVMSparcDesc LLVMSparcInfo LLVMRISCVTargetMCA LLVMRISCVDisassembler LLVMRISCVAsmParser LLVMRISCVCodeGen LLVMRISCVDesc LLVMRISCVInfo LLVMPowerPCDisassembler LLVMPowerPCAsmParser LLVMPowerPCCodeGen LLVMPowerPCDesc LLVMPowerPCInfo LLVMNVPTXCodeGen LLVMNVPTXDesc LLVMNVPTXInfo LLVMMSP430Disassembler LLVMMSP430AsmParser LLVMMSP430CodeGen LLVMMSP430Desc LLVMMSP430Info LLVMMipsDisassembler LLVMMipsAsmParser LLVMMipsCodeGen LLVMMipsDesc LLVMMipsInfo LLVMLoongArchDisassembler LLVMLoongArchAsmParser LLVMLoongArchCodeGen LLVMLoongArchDesc LLVMLoongArchInfo LLVMLanaiDisassembler LLVMLanaiCodeGen LLVMLanaiAsmParser LLVMLanaiDesc LLVMLanaiInfo LLVMHexagonDisassembler LLVMHexagonCodeGen LLVMHexagonAsmParser LLVMHexagonDesc LLVMHexagonInfo LLVMBPFDisassembler LLVMBPFAsmParser LLVMBPFCodeGen LLVMBPFDesc LLVMBPFInfo LLVMAVRDisassembler LLVMAVRAsmParser LLVMAVRCodeGen LLVMAVRDesc LLVMAVRInfo LLVMARMDisassembler LLVMARMAsmParser LLVMARMCodeGen LLVMARMDesc LLVMARMUtils LLVMARMInfo LLVMAMDGPUTargetMCA LLVMAMDGPUDisassembler LLVMAMDGPUAsmParser LLVMAMDGPUCodeGen LLVMAMDGPUDesc LLVMAMDGPUUtils LLVMAMDGPUInfo LLVMAArch64Disassembler LLVMAArch64AsmParser LLVMAArch64CodeGen LLVMAArch64Desc LLVMAArch64Utils LLVMAArch64Info LLVMOrcDebugging LLVMOrcJIT LLVMWindowsDriver LLVMMCJIT LLVMJITLink LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMOrcTargetProcess LLVMOrcShared LLVMDWP LLVMDebugInfoLogicalView LLVMDebugInfoGSYM LLVMOption LLVMObjectYAML LLVMObjCopy LLVMMCA LLVMMCDisassembler LLVMLTO LLVMFrontendOpenACC LLVMFrontendHLSL LLVMFrontendDriver LLVMExtensions Polly PollyISL LLVMPasses LLVMHipStdPar LLVMCoroutines LLVMCFGuard LLVMipo LLVMInstrumentation LLVMVectorize LLVMLinker LLVMFrontendOpenMP LLVMFrontendOffloading LLVMDWARFLinkerParallel LLVMDWARFLinkerClassic LLVMDWARFLinker LLVMCodeGenData LLVMGlobalISel LLVMMIRParser LLVMAsmPrinter LLVMSelectionDAG LLVMCodeGen LLVMTarget LLVMObjCARCOpts LLVMCodeGenTypes LLVMIRPrinter LLVMInterfaceStub LLVMFileCheck LLVMFuzzMutate LLVMScalarOpts LLVMInstCombine LLVMAggressiveInstCombine LLVMTransformUtils LLVMBitWriter LLVMAnalysis LLVMProfileData LLVMSymbolize LLVMDebugInfoBTF LLVMDebugInfoPDB LLVMDebugInfoMSF LLVMDebugInfoDWARF LLVMObject LLVMTextAPI LLVMMCParser LLVMIRReader LLVMAsmParser LLVMMC LLVMDebugInfoCodeView LLVMBitReader LLVMFuzzerCLI LLVMCore LLVMRemarks LLVMBitstreamReader LLVMBinaryFormat LLVMTargetParser LLVMTableGen LLVMSupport LLVMDemangle
            -Wl,--no-whole-archive
        )
    endif()
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