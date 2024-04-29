#version 330 core

uniform sampler2D tex;

in vec2 texCoord;

out vec4 colour;


void main() {
    vec4 textColour = vec4(1.0, 0.0, 0.0, 1.0);
    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r);
    colour = textColour * sampled;
}
