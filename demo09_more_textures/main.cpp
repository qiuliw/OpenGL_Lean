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

    // 从文件读取shader
    Shader *shader = new Shader("vertex.frag","fragment.frag");

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

    ///****** Texture 纹理处理 - 多纹理 ********

    // 纹理A
    unsigned int textureA;
    glGenTextures(1, &textureA);
    // glActiveTexture(GL_TEXTURE0); 默认激活
    glBindTexture(GL_TEXTURE_2D, textureA);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char *dataA = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (dataA)
    {
        // 设置基础层级2D纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataA);
        glGenerateMipmap(GL_TEXTURE_2D); // 在设置完基础层级（level=0）后，调用此函数可自动生成所有更低层级的 Mipmap
    }
    else
    {
        std::cout << "Failed to load textureA" << std::endl;
    }
    stbi_image_free(dataA);

    // 纹理B
    unsigned int textureB;
    glGenTextures(1, &textureB);
    glActiveTexture(GL_TEXTURE1); // 激活纹理单元1,往后设置纹理时，都绑定这个纹理单元
    glBindTexture(GL_TEXTURE_2D, textureB);
    
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    unsigned char *dataB = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (dataB)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataB);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load textureB" << std::endl;
    }

    stbi_image_free(dataB);

    shader->use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    // 向shader程序传入纹理ID
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1); // 或者使用着色器类设置
        
    // EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 主渲染循环
    // 当前输入作用于当前帧
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // 获取键盘和鼠标事件传递到window上下文
        processInput(window); // 判断上下文中的事件

        // 设置背景色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空缓冲区后填充的颜色，默认值，背景色
        glClear(GL_COLOR_BUFFER_BIT); // 清空缓冲区颜色

        // 绘制三角形
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);// 通过EBO索引绘制

        // 交换缓冲区
        glfwSwapBuffers(window); // 双缓冲区，一个用于显示，一个用于绘制
    }

    glfwTerminate();

    return 0;
}

