cmake_minimum_required(VERSION 3.10.0)
project(glfw_glew VERSION 0.1.0 LANGUAGES C CXX)

add_executable(glfw_glew main.cpp)

# GLFW
include_directories("D:/Software/glfw-3.4.bin.WIN32/include")
target_link_libraries(glfw_glew "D:/Software/glfw-3.4.bin.WIN32/lib-mingw-w64/libglfw3dll.a")

# GLEW
include_directories("D:/Software/glew-2.1.0-win32/glew-2.1.0/include") # 头文件
target_link_libraries(glfw_glew "D:/Software/glew-2.1.0-win32/glew-2.1.0/lib/Release/Win32/glew32.lib")

