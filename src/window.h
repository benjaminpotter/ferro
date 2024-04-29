
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
    void initImGui();
    void initShaders();
    void initNodes();

    void drawNodes();

private:
    const char *title;
    GLFWwindow* window;

    std::vector<Text*> texts;
    std::vector<Node*> nodes;
};
