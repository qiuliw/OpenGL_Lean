cmake_minimum_required(VERSION 3.10.0)
project(shader_interpolate VERSION 0.1.0 LANGUAGES C CXX)

add_executable(shader_interpolate main.cpp)

include_directories(
    "D:/Software/glfw-3.4.bin.WIN64/include"
    "D:/Software/glew-2.1.0-win32/glew-2.1.0/include"
)
target_link_libraries(shader_interpolate 
    "D:/Software/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3dll.a"
    "D:/Software/glew-2.1.0-win32/glew-2.1.0/lib/Release/x64/glew32.lib"
    "opengl32.lib"
)