
#include <string>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"

#pragma once

///
/// Provide geometry that corresponds to the bitmap generated from the text string.
///
class Text {

public: 
    Text(std::string text);
    ~Text();

    // FIXME public but called in constructor?
    void initVAO();
    void draw();

    glm::vec3 position; 
    glm::vec3 scale;

private:

    void generateBitmap();
    void initTexture();
    void initShader();

    std::string text;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int texture;

    FT_Bitmap bitmap;
    Shader *shader;

};
