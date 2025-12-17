cmake_minimum_required(VERSION 3.16)

#project name 
project(DesktopMate)


set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Create an executable named "MyMate" from "main.cpp"
add_executable(MyMate main.cpp)

# Link the Qt libraries to executable
target_link_libraries(MyMate PRIVATE Qt6::Widgets)