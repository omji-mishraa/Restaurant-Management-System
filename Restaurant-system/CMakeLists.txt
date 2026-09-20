cmake_minimum_required(VERSION 3.10)
project(SmartRestaurant)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add all source files to the executable
add_executable(SmartRestaurant main.cpp Order.cpp Restaurant.cpp)
