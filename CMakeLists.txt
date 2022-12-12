cmake_minimum_required(VERSION 3.20)

set(PROJECT_VERSION "0.1.0")
set(PROJECT_NAME "cpu-ut")

project(${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES C
    DESCRIPTION "CPU Usage Tracker"
)
set(CMAKE_C_STANDARD 11)

set(INCLUDES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCES_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADERS
    "${INCLUDES_DIR}/core/types.h"
    "${INCLUDES_DIR}/core/reader.h"
    "${INCLUDES_DIR}/core/analyzer.h"
    "${INCLUDES_DIR}/core/ring_buffer.h"
)

set(SOURCES
    "${SOURCES_FILES_DIR}/main.c"
    "${SOURCES_FILES_DIR}/core/types.c"
    "${SOURCES_FILES_DIR}/core/reader.c"
    "${SOURCES_FILES_DIR}/core/analyzer.c"
    "${SOURCES_FILES_DIR}/core/ring_buffer.c"
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${INCLUDES_DIR}>
)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
  $<$<CXX_COMPILER_ID:Clang>:-Weverything>
)