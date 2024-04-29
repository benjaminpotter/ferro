#version 330 core

layout (location = 0) in vec3 attrPos;
layout (location = 1) in vec2 attrTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoord;

void main() {
    
    texCoord = attrTexCoord;
    gl_Position = projection * view * model * vec4(attrPos, 1.0);
}

