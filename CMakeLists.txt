cmake_minimum_required(VERSION 3.10)
project(GameOfLife VERSION 0.1.0 LANGUAGES C CXX)

# === 改动：不再联网下载，直接用你刚才放进去的本地文件 ===

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(libs/raylib-5.0)

# === 构建你的程序 ===
add_executable(GameOfLife 
    main.cpp 
    game_logic.cpp
    )

# === 链接库 ===
target_link_libraries(GameOfLife PRIVATE raylib)