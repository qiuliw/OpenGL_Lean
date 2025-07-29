#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture; // 贴图对象

void main()
{
    FragColor = texture(ourTexture, TexCoord); // 贴图对象与纹理坐标进行匹配，返回颜色
}