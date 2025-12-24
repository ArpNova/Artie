cmake_minimum_required(VERSION 3.16)
 
project(artie)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)


include_directories(include)

add_executable(artie src/main.cpp src/artie.cpp include/artie.h)

target_link_libraries(artie PRIVATE Qt6::Widgets)