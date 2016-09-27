project( atoi-benchmark )
cmake_minimum_required( VERSION 3.0 )

if( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message( FATAL_ERROR "In-source builds are not allowed." )
endif()

find_package( Boost 1.42 REQUIRED )

add_executable(
  benchmark

  src/benchmark.cpp
  src/boost_lexical_cast.cpp
  src/boost_spirit_qi.cpp
  src/c_strtoul.cpp
  src/enforce.cpp
  src/naive.cpp
  src/recursive.cpp
  src/std_strtoul.cpp
  src/table_pow.cpp
  src/unrolled_4.cpp
  )

target_include_directories(
  benchmark
  PUBLIC
  include
  ${Boost_INCLUDE_DIRS}
)

add_executable(
  all-in-one-benchmark

  src/all-in-one.cpp
  )

target_include_directories(
  all-in-one-benchmark
  PUBLIC
  include
  src
  ${Boost_INCLUDE_DIRS}
)
