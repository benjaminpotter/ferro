
#pragma once

class Node {

public:
    Node();
    ~Node();

    void initVAO();
    void draw();

private:

    int resolution;
    unsigned int VBO;
    unsigned int VAO;
    
    
    
};

