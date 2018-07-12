SET(CMAKE_SYSTEM_NAME Linux)

# specify the cross compiler
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc -mfloat-abi=hard)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++ -mfloat-abi=hard)
set(CMAKE_AR arm-linux-gnueabihf-ar CACHE FILEPATH "Archiver")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
