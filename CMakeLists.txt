cmake_minimum_required(VERSION 3.14)

project(ViariantVisitorInterface LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


######################### Examples ############################
add_executable(Example
  Examples/example_VariantVisitor.cpp
)
target_include_directories(Example PRIVATE inc)


