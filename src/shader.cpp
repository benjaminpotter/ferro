
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

Shader::Shader() {

}

Shader::~Shader() {

}


void Shader::load(std::string vertexPath, std::string fragmentPath) {

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


void Shader::use() {
    glUseProgram(shaderProgram);
}
