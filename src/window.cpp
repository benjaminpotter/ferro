
#include <iostream>
#include <fstream>
#include <sstream>

#include "window.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Window::Window(const char *title): title(title) {
    initGLFW();
}

Window::~Window() {

    for(auto node : nodes)
        delete node;

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
    
    std::string vertexPath, fragmentPath;
    vertexPath = "../src/shader/vertex.vs";
    fragmentPath = "../src/shader/fragment.fs";

    std::string vertexCode, fragmentCode;
    std::ifstream vertexFile, fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();

    } catch(std::ifstream::failure &e) {
        std::cerr << "ERR::WINDOW shader file not read: " << e.what() << std::endl;
    }

    int success;
    char log[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const char* vertexShaderBuffer = vertexCode.c_str();    
    glShaderSource(vertexShader, 1, &vertexShaderBuffer, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cerr << "ERR::WINDOW vertex shader not compiled: " << log << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);     

    const char* fragmentShaderBuffer = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderBuffer, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        std::cerr << "ERR::WINDOW fragment shader not compiled: " << log << std::endl;
    }
    
    // shader program
    shaderProgram = glCreateProgram(); 
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        std::cerr << "ERR::WINDOW shader program not linked: " << log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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

        glUseProgram(shaderProgram);
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

