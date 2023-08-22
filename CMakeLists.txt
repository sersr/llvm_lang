cmake_minimum_required(VERSION 3.18)

project(llvm_wrapper)

if(WIN32)
    set(CMAKE_CXX_FLAGS "/std:c++17")
else()
    set(CMAKE_CXX_FLAGS "-std=c++2a")
endif()

find_package(LLVM REQUIRED)

include_directories(${LLVM_INCLUDE_DIRS})

link_libraries(LLVM)

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