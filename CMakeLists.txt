cmake_minimum_required(VERSION 3.1)
project(tp_highload_cpu_cache_latency)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

set(SOURCE_FILES main.c)
add_executable(tp_highload_cpu_cache_latency ${SOURCE_FILES})