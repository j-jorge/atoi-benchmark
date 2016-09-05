project( atoi-benchmark )
cmake_minimum_required( VERSION 3.5 )

if( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message( FATAL_ERROR "In-source builds are not allowed." )
endif()

add_executable(
  benchmark

  src/benchmark.cpp
  src/boost_lexical_cast.cpp
  src/c_strtoul.cpp
  src/enforce.cpp
  src/naive.cpp
  src/recursive.cpp
  src/std_strtoul.cpp
  src/table_pow.cpp
  src/unrolled_4.cpp
  )

target_include_directories( benchmark PUBLIC include )

add_executable(
  all-in-one-benchmark

  src/all-in-one.cpp
  )

target_include_directories( all-in-one-benchmark PUBLIC include src )
