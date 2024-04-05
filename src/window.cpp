
#include <iostream>

#include "window.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Window::Window(const char *title): title(title) {
    initGLFW();

    shader = new Shader();
}

Window::~Window() {

    for(auto node : nodes)
        delete node;

    delete shader;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::initShaders() {
    std::string vertexPath = "../src/shader/vertex.vs";
    std::string fragmentPath = "../src/shader/fragment.fs";

    shader->load(vertexPath, fragmentPath); 
}

bool Window::init() {

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    if(!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return false;
    }

    glfwMakeContextCurrent(window); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }   

    initShaders();

    nodes.push_back(new Node());

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    
    return true;
}

void Window::run() {
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        for(unsigned int i = 0; i < nodes.size(); ++i)
            nodes[i]->draw();

        #if 0
        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "ERR::WINDOW opengl: " << err << std::endl;
        } 
        #endif

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}

