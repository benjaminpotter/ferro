
#pragma once

class Shader {

public:
    Shader();
    ~Shader();

    void load(std::string vertexPath, std::string fragmentPath);
    void use();

private:
    unsigned int shaderProgram;

};
