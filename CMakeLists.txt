cmake_minimum_required(VERSION 3.00)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(EFM-Blink)

set(EXECUTABLE ${PROJECT_NAME}.axf)
# Select linker script
set(LINKER_SCRIPT "../device/efm32g.ld")

# Select CPU options

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CPU_OPTIONS -mcpu=cortex-m3 -mthumb)
# optimizations (-O0 -O1 -O2 -O3 -Os -Ofast -Og -flto)
set(CMAKE_C_FLAGS_DEBUG -O0 -g3)
set(CMAKE_C_FLAGS_RELEASE -Os -flto)

#List of source files
set(SRC_FILES 
        device/startup/startup_efm32g.c
        device/system/system_efm32g.c
        
        emlib/src/em_cmu.c
        emlib/src/em_core.c
        emlib/src/em_msc.c
        emlib/src/em_gpio.c
        emlib/src/em_usart.c
        emlib/src/em_system.c

        src/flash/flash.c
        src/serial/serial.c
        src/main.c
)

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        -DEFM32G890F128=1
        )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
        device/system/
        device/
        device/inc/
        CMSIS/
        emlib/inc/
        src/flash/
        src/serial/
        src
        )

target_compile_options( ${EXECUTABLE} PRIVATE
    ${CPU_OPTIONS}
    ${CMAKE_C_FLAGS_DEBUG}
    $<$<COMPILE_LANGUAGE:C>:-std=c99>

    -Wall
    -Wextra
    -c
    -fmessage-length=0
    -mno-sched-prolog
    -fno-builtin
    -ffunction-sections
    -fdata-sections
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        -T${LINKER_SCRIPT}
        ${CPU_OPTIONS}
        -specs=nosys.specs
        -Xlinker
        -gc-sections
        -Xlinker
        --specs=nano.specs
        -Wl,-Map=${PROJECT_NAME}.map
        -Wl,--print-memory-usage
        -Wl,--start-group
        -lgcc 
        -lc 
        -lnosys 
        -Wl,--end-group
)

#optional: Print executable size as a part of post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
