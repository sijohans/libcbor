cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers)

SET(CBOR_VERSION_MAJOR "0")
SET(CBOR_VERSION_MINOR "5")
SET(CBOR_VERSION_PATCH "0")
SET(CBOR_RESTRICT_SPECIFIER "")
SET(CBOR_INLINE_SPECIFIER "inline")
set(CBOR_BUFFER_GROWTH "2" CACHE STRING "Factor for buffer growth & shrinking")

set(CBOR_SOURCES
    ../src/cbor.c 
    ../src/cbor/arrays.c 
    ../src/cbor/common.c 
    ../src/cbor/ints.c 
    ../src/cbor/streaming.c 
    ../src/cbor/bytestrings.c 
    ../src/cbor/encoding.c 
    ../src/cbor/maps.c 
    ../src/cbor/strings.c 
    ../src/cbor/callbacks.c 
    ../src/cbor/floats_ctrls.c 
    ../src/cbor/serialization.c 
    ../src/cbor/tags.c 
    ../src/cbor/internal/builder_callbacks.c 
    ../src/cbor/internal/loaders.c 
    ../src/cbor/internal/stack.c 
    ../src/cbor/internal/encoders.c 
    ../src/cbor/internal/memory_utils.c 
    ../src/cbor/internal/unicode.c 
)

add_library(libcbor ${CBOR_SOURCES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../src/cbor/configuration.h.in ${PROJECT_BINARY_DIR}/cbor/configuration.h)
include_directories(${PROJECT_BINARY_DIR})
include_directories(../src)

add_library(main_entry main_entry.c)

if(USE_SANITIZERS)
    add_sanitizers(libcbor)
    add_sanitizers(main_entry)
else(USE_SANITIZERS)
endif(USE_SANITIZERS)

macro(add_fuzz arg)
    add_executable(${arg} ${arg}.c)
    if(USE_SANITIZERS)
        add_sanitizers(${arg})
    else(USE_SANITIZERS)
    endif(USE_SANITIZERS)
    target_link_libraries(${arg} ${ARGN})

endmacro(add_fuzz)

add_fuzz(fuzz_libcbor libcbor main_entry)