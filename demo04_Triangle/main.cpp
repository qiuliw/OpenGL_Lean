#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // 0
    0.5f, -0.5f, 0.0f, // 1
    0.0f,  0.5f, 0.0f, // 2
    -0.5f, 0.7f, 0.0f // 3
};

unsigned indices[] = { // 注意索引从0开始
	0, 1, 3, // 第一个三角形
	1, 2, 3 // 第二个三角形
};

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // 绘制一个橙色三角形
    }
)";

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

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // count: 几个字串
    glCompileShader(vertexShader);

    // fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 主渲染循环
    // 当前输入作用于当前帧
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // 获取键盘和鼠标事件传递到window上下文
        processInput(window); // 判断上下文中的事件

        // 设置背景色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空缓冲区后填充的颜色，默认值，背景色
        glClear(GL_COLOR_BUFFER_BIT); // 清空缓冲区颜色

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        // glDrawArrays(GL_TRIANGLES, 0, 6); // 绘制三角形, count: 顶点数量, 直接通过VBO顶点绘制

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);// 通过EBO索引绘制

        // 交换缓冲区
        glfwSwapBuffers(window); // 双缓冲区，一个用于显示，一个用于绘制
    }

    glfwTerminate();

    return 0;
}

