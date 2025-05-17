cmake_minimum_required(VERSION 3.10)
project(private_member_access)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(friend_insertion friend_insertion.cpp)
