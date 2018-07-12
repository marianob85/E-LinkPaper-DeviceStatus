


# E-PaperProjectStatus
Use e-paper display as status information. 

![Screenshot](Screens/image.bmp)

# Usage 

## CMake
### gcc
1. cd CMake
2. cmake .
3. make
### clang
1. cd CMake
2. cmake -DCMAKE_TOOLCHAIN_FILE=llvm.cmake -Wno-dev
3. make

## autoconf
 - cd autoconf
 - ./configure
 - make
 
# CrossBuild:
## RaspberryPI:
 - Tools: apt-get install crossbuild-essential-armhf 
### GCC
 - cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-gnuaebihf.cmake
### LLVM ( clang )
- cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-llvm.cmake -Wno-dev
