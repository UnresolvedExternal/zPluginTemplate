cmake_minimum_required(VERSION 3.21)

project(Service)

add_library(Service INTERFACE)
target_include_directories(Service INTERFACE "src")