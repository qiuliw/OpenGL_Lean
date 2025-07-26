cmake_minimum_required(VERSION 3.10.0)
project(usenuget VERSION 0.1.0 LANGUAGES C CXX)

add_executable(usenuget main.cpp)

# GLFW
include_directories("D:/Software/glfw-3.4.bin.WIN64/include")
target_link_libraries(usenuget "D:/Software/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3dll.a")

# GLEW
include_directories("D:/Software/glew-2.1.0-win32/glew-2.1.0/include") # 头文件
target_link_libraries(usenuget "D:/Software/glew-2.1.0-win32/glew-2.1.0/lib/Release/x64/glew32.lib")

target_link_libraries(usenuget "opengl32.lib")