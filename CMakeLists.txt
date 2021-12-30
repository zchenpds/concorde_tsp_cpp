cmake_minimum_required(VERSION 3.0.0)
project(concorde_tsp_cpp)
include_directories(include)

add_executable(concorde_example concorde_example.cpp)
target_link_libraries(
  concorde_example
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/concorde.a
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/qsopt.a
)