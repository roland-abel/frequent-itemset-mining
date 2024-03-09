cmake_minimum_required(VERSION 3.22.1)
project(association-rules
        VERSION 1.0.0
        DESCRIPTION "Implementation of the Apriori Algorithm"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(
        association-rules main.cpp
        src/apriori.cpp
)

add_executable(main main.cpp)

include(FetchContent)
FetchContent_Declare(
        google-test
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(google-test)

set(PROJECT_NAME association-rules)
set(UNIT_TEST_NAME unit-tests)
set(RULES_LIB rules)

add_subdirectory(test)
add_subdirectory(src)
