
#include <string>
#include <glm/glm.hpp>

#include "text.h"
#include "shader.h"

#pragma once


/// 
/// Stores information about a single article, or node, in the graph.
///
class Node {

public:
    Node();
    ~Node();

    // FIXME public but called in constructor?
    void initVAO();
    void draw();

    glm::vec3 position; 
    glm::vec3 scale;
    
private:
    
    void initShader();

    int resolution;
    unsigned int VBO;
    unsigned int VAO;

    std::string title;
    std::vector<Node> links;

    Shader *shader;
};

