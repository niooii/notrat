cmake_minimum_required(VERSION 3.25)
project(notrat C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
add_executable(notrat main.c)

IF (WIN32)
    TARGET_LINK_LIBRARIES(notrat PRIVATE wsock32 ws2_32)
ENDIF ()