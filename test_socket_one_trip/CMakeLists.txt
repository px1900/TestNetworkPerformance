cmake_minimum_required(VERSION 3.21)
project(test_socket C)

set(CMAKE_C_STANDARD 11)

add_executable(test_socket
        socket_client.c
        socket_server.c)
