project(FrequencyDict)

cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(hopscotch)
add_executable(FrequencyDict src/main.cpp src/System.cpp src/Words.cpp)
target_link_libraries(FrequencyDict PRIVATE tsl::hopscotch_map)
