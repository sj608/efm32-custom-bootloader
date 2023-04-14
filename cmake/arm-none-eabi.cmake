# A toolchain file specifying information about the compiler and utility paths for cross-compilation
# This script file is invoked with cmake -DCMAKE_TOOLCHAIN_FILE=path/to/file

#################
# System Config #
#################
set(CMAKE_SYSTEM_NAME Generic) # Name of the operating system for which CMake to build
set(CMAKE_SYSTEM_PROCESSOR arm) # Name of the system processor
set(ARM_TOOLCHAIN_DIR "/opt/homebrew/bin") # set arm toolchain directory path
set(BINUTILS_PATH ${ARM_TOOLCHAIN_DIR}) # set binary utility directory path

####################
# Toolchain Config #
####################
set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)
set(AS                  arm-none-eabi-as)
set(CMAKE_AR            arm-none-eabi-gcc-ar)
set(OBJCOPY             arm-none-eabi-objcopy)
set(OBJDUMP             arm-none-eabi-objdump)
set(SIZE                arm-none-eabi-size)

set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_OBJCOPY ${OBJCOPY} CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${SIZE} CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)