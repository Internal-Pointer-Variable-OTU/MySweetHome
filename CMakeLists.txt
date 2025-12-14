cmake_minimum_required(VERSION 3.10)
project(MySweetHome VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(msh src/main.cpp)