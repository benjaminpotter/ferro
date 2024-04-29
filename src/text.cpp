
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/matrix_transform.hpp>

#include "text.h"


Text::Text(std::string text) {
    // text = std::string("Hello, world!");
    this->text = text;

    generateBitmap();
    initTexture();
    initVAO();
    initShader();

    position = glm::vec3(250, 250, 0);
    scale = glm::vec3(100, 100, 0);
}

Text::~Text() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    delete shader;
}

void Text::initVAO() {

    glm::vec3 vertexCoords[6];
    vertexCoords[0] = { -1.0, -1.0, 0.0 };
    vertexCoords[1] = { -1.0,  1.0, 0.0 };
    vertexCoords[2] = {  1.0, -1.0, 0.0 };
    vertexCoords[3] = {  1.0, -1.0, 0.0 };
    vertexCoords[4] = { -1.0,  1.0, 0.0 };
    vertexCoords[5] = {  1.0,  1.0, 0.0 };

    glm::vec2 textureCoords[6];
    textureCoords[0] = { 0.0, 1.0 };
    textureCoords[1] = { 0.0, 0.0 };
    textureCoords[2] = { 1.0, 1.0 };
    textureCoords[3] = { 1.0, 1.0 };
    textureCoords[4] = { 0.0, 0.0 };
    textureCoords[5] = { 1.0, 0.0 };

    const size_t vertexSize = 3 + 2;
    float vertices[ vertexSize * 6 ];
    for(int i = 0; i < 6; ++i) {
        vertices[i * vertexSize + 0] = vertexCoords[i].x;
        vertices[i * vertexSize + 1] = vertexCoords[i].y;
        vertices[i * vertexSize + 2] = vertexCoords[i].z;

        vertices[i * vertexSize + 3] = textureCoords[i].x;
        vertices[i * vertexSize + 4] = textureCoords[i].y;
    }
    

    // do VAO generation 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * vertexSize, (void*)0);
    glEnableVertexAttribArray(0);      

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * vertexSize, (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);      

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::initTexture() {

    int width, height, channels;    
    
    width = bitmap.width;
    height = bitmap.rows;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
}

void Text::generateBitmap() {
    FT_Error err;

    FT_Library lib;
    err = FT_Init_FreeType(&lib);
    if(err) {
        std::cerr << "ERR::TEXT freetype: failed to init" << std::endl;
        return;
    }

    FT_Face face;
    err = FT_New_Face(lib, "data/ProggyClean.ttf", 0, &face);
    if(err) {
        std::cerr << "ERR::TEXT freetype: " << err << std::endl;
        return;
    }

    err = FT_Set_Pixel_Sizes(face, 0, 48);
    if(err) {
        std::cerr << "ERR::TEXT freetype: " << err << std::endl;
        return;
    }

    err = FT_Load_Char(face, 'W', FT_LOAD_RENDER);
    if(err) {
        std::cerr << "ERR::TEXT freetype: " << err << std::endl;
        return;
    }

    bitmap = face->glyph->bitmap; 
}

void Text::initShader() {
    shader = new Shader();
    
    std::string textSourcePaths[] = { "src/shader/text.vs", "src/shader/text.fs" };
    shader->load(textSourcePaths[0], textSourcePaths[1]); 
}

void Text::draw() {

    shader->use();

    // FIXME these should come from the "global" shader context
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);

    projection = glm::ortho(0.0f, (float) 1920, 0.0f, (float) 1080, -100.0f, 100.0f);

    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);

    shader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}



