cmake_minimum_required(VERSION 3.25)
project(Innertia)

set(CMAKE_CXX_STANDARD 20)

set(SR_UTILS_ASSIMP OFF CACHE INTERNAL "" FORCE)
set(SR_UTILS_STATIC_LIBRARY ON)
set(SR_UTILS_FMT_HEADER_ONLY ON)

add_subdirectory(libs/Utils)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE libs/asio/include)

add_executable(Innertia cxx/Innertia.cxx main.cpp)

target_include_directories(Innertia PUBLIC inc ${SR_UTILS_INCLUDE_DIRECTORIES})
target_link_libraries(Innertia PUBLIC asio Utils)