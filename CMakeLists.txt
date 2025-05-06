cmake_minimum_required(VERSION 3.4)
project(Yadro2025)

#use g++
set(CMAKE_CXX_STANDARD 20)
add_library(headers INTERFACE)

target_include_directories(headers INTERFACE inc)

add_library(SRC
    src/ComputerClub.cpp
)
target_link_libraries(SRC PRIVATE headers)

add_executable(club
    src/run_club.cpp)
target_link_libraries(club PRIVATE headers)
target_link_libraries(club PRIVATE SRC)
