
#include <glm/glm.hpp>

#pragma once

class Shader {

public:
    Shader();
    ~Shader();

    void load(std::string vertexPath, std::string fragmentPath);
    void use();

    void setMat4(const std::string &name, const glm::mat4 &value);

private:
    unsigned int shaderProgram;

};
