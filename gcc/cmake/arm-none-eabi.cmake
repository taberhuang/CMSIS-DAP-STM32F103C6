# -----------------------------------------------------------------------------
# CMake toolchain file: arm-none-eabi-gcc
# Usage:
#   cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=gcc/cmake/arm-none-eabi.cmake
# -----------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Bare-metal targets cannot link a test executable; tell CMake to compile a
# static library when probing the compiler.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Allow overriding the cross compiler prefix via -D or environment variable.
if(NOT DEFINED ARM_TOOLCHAIN_PREFIX)
    if(DEFINED ENV{ARM_TOOLCHAIN_PREFIX})
        set(ARM_TOOLCHAIN_PREFIX "$ENV{ARM_TOOLCHAIN_PREFIX}")
    else()
        set(ARM_TOOLCHAIN_PREFIX "arm-none-eabi-")
    endif()
endif()

if(CMAKE_HOST_WIN32)
    set(_exe ".exe")
else()
    set(_exe "")
endif()

set(CMAKE_C_COMPILER   "${ARM_TOOLCHAIN_PREFIX}gcc${_exe}")
set(CMAKE_CXX_COMPILER "${ARM_TOOLCHAIN_PREFIX}g++${_exe}")
set(CMAKE_ASM_COMPILER "${ARM_TOOLCHAIN_PREFIX}gcc${_exe}")
set(CMAKE_OBJCOPY      "${ARM_TOOLCHAIN_PREFIX}objcopy${_exe}" CACHE INTERNAL "")
set(CMAKE_OBJDUMP      "${ARM_TOOLCHAIN_PREFIX}objdump${_exe}" CACHE INTERNAL "")
set(CMAKE_SIZE         "${ARM_TOOLCHAIN_PREFIX}size${_exe}"    CACHE INTERNAL "")

# Keep find_xxx() from searching the host system.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
