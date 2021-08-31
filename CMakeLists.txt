cmake_minimum_required(VERSION 3.10)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(TOOLCHAIN_PATH /opt/arm/gcc-arm-none-eabi-8-2018-q4-major/bin/)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-g++)
set(AS ${TOOLCHAIN_PATH}/arm-none-eabi-as)
set(AR ${TOOLCHAIN_PATH}/arm-none-eabi-ar)
set(OBJCOPY ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy)
set(OBJDUMP ${TOOLCHAIN_PATH}/arm-none-eabi-objdump)
set(SIZE ${TOOLCHAIN_PATH}/arm-none-eabi-size)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

set(MDR32F9Qx_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork -msoft-float")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -Wall ${MDR32F9Qx_FLAGS} -std=c99 -ffunction-sections -fdata-sections ")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -Wall --std=c++14 ${MDR32F9Qx_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcpu=cortex-m3 -Wl,-gc-sections,--print-memory-usage -nostartfiles \
                                -lc -lnosys --specs=rdimon.specs -Wl,--gc-sections \
                                -T ${LIB_DIR}/ldscripts/MDR32F9Qx.ld")

# Project
project(mdr_blink C ASM)

add_definitions(-DUSE_MDR32F9Q2_Rev1)

# CMSIS Library
include_directories(
        ${LIB_DIR}/CMSIS/CM3/CoreSupport
        ${LIB_DIR}/CMSIS/CM3/DeviceSupport/MDR32F9Qx/inc
        ${LIB_DIR}/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc
)

list(APPEND CMSIS_SRC "${LIB_DIR}/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/startup_MDR32F9Qx.S")
list(APPEND CMSIS_SRC "${LIB_DIR}/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/system_MDR32F9Qx.c")

# StdPeriph Driver Library
include_directories(
        ${LIB_DIR}/MDR32F9Qx_StdPeriph_Driver/inc
        ${LIB_DIR}/MDR32F9Qx_StdPeriph_Driver/inc/USB_Library
)

file(GLOB_RECURSE STD_PERIPH_SRC "${LIB_DIR}/MDR32F9Qx_StdPeriph_Driver/src/*.c")
list(REMOVE_ITEM STD_PERIPH_SRC "${LIB_DIR}/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_eth.c")
list(REMOVE_ITEM STD_PERIPH_SRC "${LIB_DIR}/MDR32F9Qx_StdPeriph_Driver/src/MDR32F9Qx_mil_std_1553.c")


# Project Files
include_directories(
        src
)

file(GLOB_RECURSE PROJECT_SRC "src/*.c")


add_executable(${PROJECT_NAME}.elf ${PROJECT_SRC} ${CMSIS_SRC})

set(HEX_FILE "${PROJECT_NAME}.hex")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMENT "Building ${HEX_FILE}"
)
