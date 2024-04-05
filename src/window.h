
#include <vector>

// https://learnopengl.com/Getting-started/Hello-Window
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "node.h"
#include "shader.h"

#pragma once

class Window {
    
public:
    Window(const char *title);
    ~Window();

    bool init();
    void run();

private:
    void initGLFW();
    void initShaders();

private:
    const char *title;
    GLFWwindow* window;
    Shader *shader;

    std::vector<Node*> nodes;
};
