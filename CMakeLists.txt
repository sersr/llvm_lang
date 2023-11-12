cmake_minimum_required(VERSION 3.18)

project(llvm_wrapper)

find_package(LLVM CONFIG REQUIRED)

if(WIN32)
    set(CMAKE_CXX_FLAGS "/std:c++17")
    link_libraries(${LLVM_AVAILABLE_LIBS})
else()
    find_package(LLVM REQUIRED)
    set(CMAKE_CXX_FLAGS "-std=c++2a")
    link_libraries(LLVM)
endif()

include_directories(${LLVM_INCLUDE_DIRS})

add_executable(
    main
    "src/main.cpp"
    "src/llvm_wrapper.cpp"
)

if(WIN32)
    add_library(${PROJECT_NAME} SHARED
        "src/llvm_wrapper.cpp"
        llvm_wrapper.def
    )
else()
    add_library(${PROJECT_NAME} SHARED
        "src/llvm_wrapper.cpp"
    )
endif()

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