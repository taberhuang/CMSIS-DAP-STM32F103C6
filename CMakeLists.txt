cmake_minimum_required(VERSION 3.20)

# The toolchain file must be set before project().
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_LIST_DIR}/gcc/cmake/arm-none-eabi.cmake"
        CACHE FILEPATH "arm-none-eabi toolchain")
endif()

project(CMSIS-DAP-STM32F103C6 C ASM)

# ============================================================================
# User-configurable defines (mirror of the original Keil project "Define" list)
# ============================================================================
set(STM32_DEFINES
    USE_STDPERIPH_DRIVER
    STM32F10X_LD
    BLUEPILL
    CDC_UART_USE_USART1_PA9_PA10
    # NRESET_1MS_TOGGLE_TEST   # off by default to avoid fighting the host reset control
    CACHE STRING "Compile-time defines for board / features")

# ============================================================================
# STM32F1 device header (stm32f10x.h)
# ----------------------------------------------------------------------------
# Shipped in-tree together with the SPL it pairs with:
#   Firmware/User/stm32f10x.h            (ST CMSIS Device V3.5.0)
#   Firmware/User/system_stm32f10x.h     (system clock init header)
#   Firmware/User/stm32f10x_conf.h       (SPL module switches)
#   Firmware/Driver/inc/stm32f10x_*.h    (SPL peripheral headers)
#   Firmware/Driver/src/stm32f10x_*.c    (SPL peripheral sources, V3.5.0)
# Firmware/User is already on the include path, no extra entry needed here.
# This removes any Keil Pack / network dependency: local and CI builds match.
# ============================================================================
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/Firmware/User/stm32f10x.h")
    message(FATAL_ERROR "Missing Firmware/User/stm32f10x.h; repository layout is broken.")
endif()
message(STATUS "STM32F10x device headers: Firmware/User (in-tree, ST CMSIS V3.5.0)")

# ============================================================================
# Compile / link flags
# ============================================================================
set(MCU_FLAGS
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft)

add_compile_options(
    ${MCU_FLAGS}
    -fdata-sections
    -ffunction-sections
    -fno-common
    -fno-builtin
    -fms-extensions             # anonymous struct/union used by usb_def.h
    -fno-strict-aliasing
    -fshort-wchar               # make L"..." 16-bit to match USB string descriptors
    -Wall
    -Wno-unknown-pragmas        # ARMCC `#pragma thumb` / `#pragma O3` etc.
    -Wno-comment
    -Wno-unused-function
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-implicit-fallthrough
    -Wno-array-bounds
    -Wno-attributes             # `__packed` is silently ignored in a few spots; #pragma pack(1) covers it
    -Wno-builtin-macro-redefined
    "$<$<CONFIG:Debug>:-Og;-g3;-gdwarf-2>"
    "$<$<CONFIG:Release>:-Os;-g>"
    "$<$<CONFIG:RelWithDebInfo>:-Os;-g>"
    "$<$<CONFIG:MinSizeRel>:-Os>"
)

add_compile_definitions(${STM32_DEFINES})

# Force-include the ARMCC -> GCC compatibility shim into every translation unit
# (keeps the original source untouched).
add_compile_options(-include "${CMAKE_CURRENT_LIST_DIR}/gcc/compat/gcc_compat.h")

add_link_options(
    ${MCU_FLAGS}
    -T${CMAKE_CURRENT_LIST_DIR}/gcc/linker/STM32F103C6.ld
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--print-memory-usage
    -Wl,--no-wchar-size-warning
    -Wl,--no-warn-rwx-segments
    --specs=nano.specs
    # Intentionally NOT using --specs=nosys.specs / -lnosys: libnosys emits
    # "_close/_read/... is not implemented" linker warnings. Equivalent stubs
    # are provided by gcc/syscalls/syscalls.c instead.
    -static
    -Wl,--start-group
    -lc -lm
    -Wl,--end-group
)

# ============================================================================
# Source files
# ============================================================================
set(SRC_USER
    Firmware/User/main.c
    Firmware/User/system_stm32f10x.c
    # stm32f10x_it.c is intentionally excluded; its handlers conflict with
    # those in main.c (same as the original Keil project).
)

set(SRC_CMSIS_DAP
    Firmware/CMSIS-DAP/DAP.c
    Firmware/CMSIS-DAP/JTAG_DP.c
    Firmware/CMSIS-DAP/SW_DP.c
    Firmware/CMSIS-DAP/SWO.c
)

set(SRC_STDPERIPH
    Firmware/Driver/src/misc.c
    Firmware/Driver/src/stm32f10x_gpio.c
    Firmware/Driver/src/stm32f10x_flash.c
    Firmware/Driver/src/stm32f10x_rcc.c
)

set(SRC_ARM_DRIVER
    Firmware/Driver/STM32F10x_ARM_Driver/DMA_STM32F10x.c
    Firmware/Driver/STM32F10x_ARM_Driver/GPIO_STM32F10x.c
    Firmware/Driver/STM32F10x_ARM_Driver/USART_STM32F10x.c
)

set(SRC_USB
    Firmware/USB/Keil_USBStack/src/usbd_core.c
    Firmware/USB/Keil_USBStack/src/usbd_core_hid.c
    Firmware/USB/Keil_USBStack/src/usbd_core_cdc.c
    Firmware/USB/Keil_USBStack/src/usbd_hid.c
    Firmware/USB/Keil_USBStack/src/usbd_cdc_acm.c
    Firmware/USB/usb_config.c
    Firmware/USB/usbd_STM32F103.c
    Firmware/USB/usbd_user_cdc_acm.c
    Firmware/USB/usbd_user_hid.c
)

set(SRC_GCC
    gcc/startup/startup_stm32f10x_ld.s
    gcc/syscalls/syscalls.c     # replaces libnosys to silence "_close/_read/... not implemented"
)

set(TARGET_ELF ${PROJECT_NAME}.elf)
add_executable(${TARGET_ELF}
    ${SRC_USER}
    ${SRC_CMSIS_DAP}
    ${SRC_STDPERIPH}
    ${SRC_ARM_DRIVER}
    ${SRC_USB}
    ${SRC_GCC}
)

target_include_directories(${TARGET_ELF} PRIVATE
    # GCC compatibility (provides cmsis_gcc.h to cmsis_compiler.h)
    gcc/compat
    # Original project layout (kept identical to the Keil project)
    Firmware/CMSIS-DAP
    Firmware/Core
    Firmware/User
    Firmware/USB
    Firmware/USB/Keil_USBStack/inc
    Firmware/Driver/inc
    Firmware/Driver/STM32F10x_ARM_Driver
    Firmware/ARM/CMSIS/INC
    Firmware/ARM/CMSIS/Driver/INC
    # Note: stm32f10x.h (the CMSIS device header) and the SPL V3.5.0 live in
    #   - Firmware/User/        (stm32f10x.h / system_stm32f10x.h / stm32f10x_conf.h)
    #   - Firmware/Driver/inc/  (SPL peripheral headers stm32f10x_*.h)
    # Both paths are already on this list; no extra entry is required.
)

# ============================================================================
# Post-build: produce .hex / .bin and print the size report
# ============================================================================
add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET_ELF}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_ELF}> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${TARGET_ELF}>
    COMMENT "Generating .hex / .bin and size report"
    VERBATIM
)
