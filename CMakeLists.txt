cmake_minimum_required(VERSION 3.29)
project(P1_Solution C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)

configure_file(data/flights.csv data/flights.csv COPYONLY)
configure_file(data/trainroutes.csv data/trainroutes.csv COPYONLY)
