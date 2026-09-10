cmake_minimum_required(VERSION 3.25)
project(MOZA_ATS_Blinker LANGUAGES CXX)

if(WIN32)
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "Build this project as Windows x64.")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32)
    message(FATAL_ERROR "This project must be built with a Windows x64 toolchain.")
else()
    message(WARNING
        "This project is intended for Windows x64. The IDE can still configure it, but actual compilation requires a Windows x64 toolchain (Visual Studio or MinGW)."
    )
endif()

add_library(moza_ats_blinker SHARED
    src/moza_ats_blinker.cpp
)

target_compile_features(moza_ats_blinker PRIVATE cxx_std_17)
target_compile_definitions(moza_ats_blinker PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
target_include_directories(moza_ats_blinker PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/scs
    ${CMAKE_CURRENT_SOURCE_DIR}/moza/include
)

target_link_libraries(moza_ats_blinker PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/moza/MOZA_SDK.lib
)

set_target_properties(moza_ats_blinker PROPERTIES
    OUTPUT_NAME "moza_ats_blinker"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

add_custom_command(TARGET moza_ats_blinker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/moza/MOZA_SDK.dll"
        "$<TARGET_FILE_DIR:moza_ats_blinker>/MOZA_SDK.dll"
)
