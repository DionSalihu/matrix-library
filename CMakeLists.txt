cmake_minimum_required(VERSION 3.10)
project(MatrixLibrary VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)  # Changed to Release for performance testing
endif()

# Find threading library
find_package(Threads REQUIRED)

# Compiler-specific optimizations
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Wall")
endif()

# Source files
set(MATRIX_SOURCES
    matrix.cpp
)

set(MATRIX_HEADERS
    matrix.h
)

# Create static library
add_library(matrix_static STATIC ${MATRIX_SOURCES} ${MATRIX_HEADERS})
target_include_directories(matrix_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(matrix_static Threads::Threads)

# Create shared library (optional)
add_library(matrix_shared SHARED ${MATRIX_SOURCES} ${MATRIX_HEADERS})
target_include_directories(matrix_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(matrix_shared Threads::Threads)

# Set library properties
set_target_properties(matrix_static PROPERTIES
    OUTPUT_NAME matrix
    CLEAN_DIRECT_OUTPUT 1
)

set_target_properties(matrix_shared PROPERTIES
    OUTPUT_NAME matrix
    CLEAN_DIRECT_OUTPUT 1
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# Test executable
add_executable(matrix_tests matrix_tests.cpp)
target_link_libraries(matrix_tests matrix_static Threads::Threads)

# Performance test executable
add_executable(performance_test performance_test.cpp)
target_link_libraries(performance_test matrix_static Threads::Threads)

# Enable testing
enable_testing()
add_test(NAME MatrixTests COMMAND matrix_tests)
add_test(NAME PerformanceTest COMMAND performance_test)

# Installation rules
install(TARGETS matrix_static matrix_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${MATRIX_HEADERS}
    DESTINATION include
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "Threading support: ${CMAKE_USE_PTHREADS_INIT}")

# Build instructions for user
message(STATUS "")
message(STATUS "Build Instructions:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake ..")
message(STATUS "  make")
message(STATUS "  ./matrix_tests          # Run functionality tests")
message(STATUS "  ./performance_test      # Run performance benchmarks")
message(STATUS "")