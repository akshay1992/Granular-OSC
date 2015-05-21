project(hw2)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC)

file(GLOB SRC
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC -std=c++11 lo)
