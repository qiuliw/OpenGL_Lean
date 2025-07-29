#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
};


void processInput(GLFWwindow *window) {
    // 检查ESC键状态
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true); // 退出程序
    }
}

int main(int, char**) {
    
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // 设置OpenGL版本和配置文件
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心配置
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // 退出
        return -1;
    }

    // 设置当前上下文
    glfwMakeContextCurrent(window); // 将这个窗口作为当前glfw上下文

    // 初始化GLEW
    glewExperimental = GL_TRUE;  // 需要在glewInit()之前设置
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 设置视口，左下角起始坐标点，可以绘制的宽高
    glViewport(0, 0, 800, 600);
    // 开启面剔除
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK); // 剔除背面

    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO 位置
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // set VBO 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // set VBO 颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // set VBO 纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    ///****** Texture 纹理处理 ********
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // 设置基础层级2D纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // 在设置完基础层级（level=0）后，调用此函数可自动生成所有更低层级的 Mipmap
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
        
    // EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // // vertex shader
    // unsigned int vertexShader;
    // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // count: 几个字串
    // glCompileShader(vertexShader);

    // // fragment shader
    // unsigned int fragmentShader;
    // fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);

    // 从文件读取shader
    Shader *shader = new Shader("vertex.frag","fragment.frag");
    
    // 主渲染循环
    // 当前输入作用于当前帧
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // 获取键盘和鼠标事件传递到window上下文
        processInput(window); // 判断上下文中的事件

        // 设置背景色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空缓冲区后填充的颜色，默认值，背景色
        glClear(GL_COLOR_BUFFER_BIT); // 清空缓冲区颜色

        shader->use(); // 激活shader程序

        // 更新uniform颜色
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shader->ID, "uniColor"); // 从着色器中获取uniform变量ourColor的存储位置
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        shader->setFloat("uniFloat", greenValue); // 设置uniform变量ourColor的值
        
        // 绘制三角形
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawArrays(GL_TRIANGLES, 0, 6); // 绘制三角形, count: 顶点数量, 直接通过VBO顶点绘制
       glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);// 通过EBO索引绘制

        // 交换缓冲区
        glfwSwapBuffers(window); // 双缓冲区，一个用于显示，一个用于绘制
    }

    glfwTerminate();

    return 0;
}

