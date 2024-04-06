
#include <glm/glm.hpp>

#pragma once

class Node {

public:
    Node();
    ~Node();

    void initVAO();
    void draw();

    glm::vec3 position; 
    glm::vec3 scale;

private:

    int resolution;
    unsigned int VBO;
    unsigned int VAO;

};

