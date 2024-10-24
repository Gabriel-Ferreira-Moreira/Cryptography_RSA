cmake_minimum_required(VERSION 3.5)

project(Cryptography_RSA C CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(benchmark REQUIRED)
include(FindGMP.cmake)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(RSA_Primitive RSA_Primitive.c)
target_link_libraries(RSA_Primitive benchmark gmp)

add_executable(crivo crivo.c)
target_link_libraries(crivo benchmark gmp)


add_executable(MontgomeryMul MontgomeryMul.cpp)
target_link_libraries(MontgomeryMul benchmark gmp)

add_executable(ExpoM ExpoM.cpp)
target_link_libraries(ExpoM benchmark gmp)

add_executable(MontMul MontMul.cpp)
target_link_libraries(MontMul benchmark gmp)


add_executable(add add.c)
target_link_libraries(add benchmark gmp)


