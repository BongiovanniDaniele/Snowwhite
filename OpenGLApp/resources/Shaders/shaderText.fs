#version 330 core
in vec2 TexCoords;
out vec4 color;

// Text rendering shader.
// The vertex shader transforms 2D strings using an orthographic projection
// and passes texture coordinates to the fragment shader.
// The fragment shader samples a glyph (string) texture where the red channel represents
// the glyph alpha, applies a uniform text color, and outputs the final colored text.

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}