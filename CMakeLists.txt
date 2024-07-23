cmake_minimum_required(VERSION 3.5)

project(Cryptography_RSA C CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(benchmark REQUIRED)
include(FindGMP.cmake)

# add_executable(RSA_example RSA.c RSA_main.c)
# target_link_libraries(RSA ${GMP_LIBRARIES})
# target_include_directories(RSA PRIVATE ${GMP_INCLUDES})

# add_executable(RSA_bench RSA.c RSA_bench.cpp)
# target_link_libraries(RSA ${GMP_LIBRARIES} benchmark::benchmark)
# target_include_directories(RSA PRIVATE ${GMP_INCLUDES})
add_executable(RSA_Primitive RSA_Primitive.c)
target_link_libraries(RSA_Primitive benchmark gmp)

add_executable(Is_Prime_MExGMP Is_Prime_MExGMP.cpp)
target_link_libraries(Is_Prime_MExGMP benchmark gmp)

add_executable(MDC MDC.cpp)
target_link_libraries(MDC benchmark gmp)

add_executable(ExpoM ExpoM.cpp)
target_link_libraries(ExpoM benchmark gmp)

add_executable(Smart_guess_primes Smart_guess_primes.cpp)
target_link_libraries(Smart_guess_primes benchmark gmp)


