
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "node.h"


Node::Node() {
    initVAO();
}

void Node::initVAO() {
    resolution = 100;

    float angleSpacing = 2 * M_PI / resolution;
    float vertices[resolution * 3];

    for(int i = 0; i < resolution; ++i) {
        float angle = angleSpacing * i;
    
        int index = i*3;
        vertices[index + 0] = cos(angle); 
        vertices[index + 1] = sin(angle); 
        vertices[index + 2] = 0.0; 
    }

    #if 0
    for(int i = 0; i < resolution; ++i) {
        int index = i*3;
        std::cout << vertices[index + 0] << " "  
            << vertices[index + 1] << " "
            << vertices[index + 2]
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

Node::~Node() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Node::draw() {

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, resolution);

}

