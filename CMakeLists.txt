cmake_minimum_required(VERSION 3.27)
project(Innertia)

set(CMAKE_CXX_STANDARD 20)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE libs/asio/include)

add_executable(Innertia Innertia.cxx main.cpp)

target_include_directories(Innertia PUBLIC inc)
target_link_libraries(Innertia PUBLIC asio)