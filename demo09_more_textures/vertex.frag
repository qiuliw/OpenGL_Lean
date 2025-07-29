#version 330 core
layout (location = 0) in vec3 aPos; // 位置
layout (location = 1) in vec3 aColor; // 颜色
layout (location = 2) in vec2 aTexCoord; // 纹理坐标

out vec3 ourColor; // 输出颜色
out vec2 TexCoord; // 输出纹理坐标

void main()
{
    gl_Position = vec4(aPos, 1.0); // 输出位置
    ourColor = aColor; 
    TexCoord = aTexCoord;
}