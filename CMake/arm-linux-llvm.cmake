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

#A toolchain file for clang is a little more complicated, because it doesn’t really understand the gcc multilib layout, so it needs to be told where all the include and lib directories are for the target system, for both the C and C++ compiler
SET(CMAKE_CXX_FLAGS "\
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/../../../../arm-linux-gnueabihf/include/c++/6 \
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/../../../../arm-linux-gnueabihf/include/c++/6/arm-linux-gnueabihf \
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/../../../../arm-linux-gnueabihf/include/c++/6/backward \
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/include \
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/include-fixed \
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/../../../../arm-linux-gnueabihf/include"
  CACHE STRING "CXX_FLAGS" FORCE)

SET(CMAKE_C_FLAGS "\
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/include \
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/include-fixed \
 -isystem /usr/lib/gcc-cross/arm-linux-gnueabihf/6/../../../../arm-linux-gnueabihf/include"
  CACHE STRING "C_FLAGS" FORCE)

SET(CMAKE_EXE_LINKER_FLAGS "\
 -L /usr/lib/gcc-cross/arm-linux-gnueabihf/6 \
 -L /usr/lib/gcc-cross/arm-linux-gnueabihf/6/../../../../arm-linux-gnueabihf/lib/../lib \
 -L /usr/lib/gcc-cross/arm-linux-gnueabihf/6/../../../../arm-linux-gnueabihf/lib \
 -static-libgcc -static-libstdc++"
  CACHE STRING "LINKER FLAGS" FORCE)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
