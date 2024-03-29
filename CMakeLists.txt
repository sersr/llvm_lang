cmake_minimum_required(VERSION 3.18)

project(llvm_wrapper)

find_package(LLVM CONFIG REQUIRED)

if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS "/std:c++17")
    set(llvm_libs ${LLVM_AVAILABLE_LIBS})
    list(REMOVE_ITEM llvm_libs "LLVM-C")
    link_libraries(${llvm_libs})
else()
    link_libraries(LLVM)
endif()

if(DEFINED VCPKG_INSTALLED_DIR)
    include_directories(${LLVM_INCLUDE_DIRS})
endif()

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