cmake_minimum_required(VERSION 3.15)
project(OGLCPP)

set(SOURCES src/glad.c)

include_directories(BEFORE include/)

add_library(glad STATIC ${SOURCES})