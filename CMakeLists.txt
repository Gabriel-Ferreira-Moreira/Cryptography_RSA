cmake_minimum_required(VERSION 3.5)

project(Cryptography_RSA C CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(benchmark REQUIRED)
include(FindGMP.cmake)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(RSA_Primitive RSA_Primitive.c)
target_include_directories(RSA_Primitive PRIVATE ${GMP_INCLUDES})
target_link_libraries(RSA_Primitive benchmark::benchmark ${GMP_LIBRARIES})

add_executable(crivo crivo.c)
target_include_directories(crivo PRIVATE ${GMP_INCLUDES})
target_link_libraries(crivo benchmark::benchmark ${GMP_LIBRARIES})

add_executable(Is_Prime_MExGMP Is_Prime_MExGMP.cpp)
target_include_directories(Is_Prime_MExGMP PRIVATE ${GMP_INCLUDES})
target_link_libraries(Is_Prime_MExGMP benchmark::benchmark ${GMP_LIBRARIES})

add_executable(ExpoM ExpoM.cpp)
target_include_directories(ExpoM PRIVATE ${GMP_INCLUDES})
target_link_libraries(ExpoM benchmark::benchmark ${GMP_LIBRARIES})

add_executable(Smart_guess_primes Smart_guess_primes.cpp)
target_include_directories(Smart_guess_primes PRIVATE ${GMP_INCLUDES})
target_link_libraries(Smart_guess_primes benchmark::benchmark ${GMP_LIBRARIES})

add_executable(Multiplication Multiplication.c)
target_include_directories(Multiplication PRIVATE ${GMP_INCLUDES})
target_link_libraries(Multiplication benchmark::benchmark ${GMP_LIBRARIES})

add_executable(Addition Addition.c)
target_include_directories(Addition PRIVATE ${GMP_INCLUDES})
target_link_libraries(Addition benchmark::benchmark ${GMP_LIBRARIES})
