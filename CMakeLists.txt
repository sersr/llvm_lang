cmake_minimum_required(VERSION 3.18)

project(llvm_wrapper)

if(WIN32)
    set(CMAKE_CXX_FLAGS "/std:c++17")
else()
    set(CMAKE_CXX_FLAGS "-std=c++2a -Wl,-force_load,/usr/local/lib/libLLVMTarget.a,/usr/local/lib/libLLVMDebuginfod.a,/usr/local/lib/libLLVMCore.a -Wl,-L/usr/local/lib")
endif()

find_package(LLVM REQUIRED)

set(LLVM_LIBS_C ${LLVM_AVAILABLE_LIBS})

# 不需要，编译错误
list(REMOVE_ITEM LLVM_LIBS_C LLVMLineEditor)

# 下面两个是动态库，不需要，已经有静态库了
list(REMOVE_ITEM LLVM_LIBS_C LTO)
list(REMOVE_ITEM LLVM_LIBS_C Remarks)

include_directories(${LLVM_INCLUDE_DIRS})
link_libraries(${LLVM_LIBS_C})

add_executable(
    main
    "src/main.cpp"
    "src/module.cpp"
    "src/ast.cpp"
    "src/llvm_wrapper.cpp"
)

add_library(${PROJECT_NAME} SHARED
    "src/llvm_wrapper.cpp"
    "src/module.cpp"
)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")
install(TARGETS
    ${PROJECT_NAME}
)