
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/matrix_transform.hpp>

#include "node.h"


Node::Node() {
    initVAO();
    initShader();

    position = glm::vec3(250, 250, 0);
    scale = glm::vec3(100, 100, 0);
}

Node::~Node() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    delete shader;
}

void Node::initVAO() {
    resolution = 100;

    float angleSpacing = 2 * M_PI / (resolution-2);
    float vertices[resolution * 3]; // 3 coords for position

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;

    for(int i = 1; i < resolution; ++i) {
        float angle = angleSpacing * i;
        
        float x = cos(angle);
        float y = sin(angle);
    
        int index = i*3;
        vertices[index + 0] = x; 
        vertices[index + 1] = y; 
        vertices[index + 2] = 0.0f;
    }

    #if 0
    for(int i = 0; i < resolution; ++i) {
        int index = i*3;
        std::cout 
            << vertices[index + 0] << " "  
            << vertices[index + 1] << " "
            << vertices[index + 2] << " "
            << std::endl;
    }
    #endif

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);      

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Node::initShader() {
    shader = new Shader();

    std::string nodeSourcePaths[] = { "src/shader/node.vs", "src/shader/node.fs" };
    shader->load(nodeSourcePaths[0], nodeSourcePaths[1]); 
}


void Node::draw() {
    
    shader->use();
    shader->setGlobalContext();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);

    shader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, resolution);

}

