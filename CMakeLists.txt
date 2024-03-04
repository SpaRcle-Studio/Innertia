cmake_minimum_required(VERSION 3.25)
project(Innertia)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SR_UTILS_CMAKE_ASSIMP OFF CACHE INTERNAL "" FORCE)
set(SR_UTILS_STATIC_LIBRARY ON)
set(SR_UTILS_FMT_HEADER_ONLY ON)
set(CRYPTIA_STATIC_LIBRARY ON)

add_subdirectory(libs/Utils)
add_subdirectory(libs/Cryptia)

add_executable(Innertia cxx/Innertia.cxx cxx/Window.cxx main.cpp)

target_include_directories(Innertia PUBLIC inc)

target_link_libraries(Innertia PUBLIC Utils Cryptia)