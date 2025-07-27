#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

uniform float uniFloat;

void main()
{
    FragColor = vec4(ourColor.xy,uniFloat,1.0);
}