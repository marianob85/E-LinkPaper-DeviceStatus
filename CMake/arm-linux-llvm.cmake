SET(CMAKE_SYSTEM_NAME Linux)

find_package(LLVM REQUIRED CONFIG)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(triple arm-linux-gnueabihf)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_COMPILER "${LLVM_TOOLS_BINARY_DIR}/clang++")
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_C_COMPILER "${LLVM_TOOLS_BINARY_DIR}/clang")
set(CMAKE_C_COMPILER_TARGET ${triple})
