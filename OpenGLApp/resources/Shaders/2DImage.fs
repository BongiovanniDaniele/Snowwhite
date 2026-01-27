#version 330 core
in vec2 TexCoords;
out vec4 color;

// 2D sprite shader.
// The vertex shader transforms 2D vertices using model and projection matrices
// and passes texture coordinates to the fragment shader.
// The fragment shader samples the sprite texture and discards fragments with low alpha
// to handle transparency, outputting the final textured color.

uniform sampler2D image;

void main()
{    
    
    vec4 texColor = texture(image, TexCoords);
    if(texColor.a < 0.1)
        discard; // it eliminates completely transparent pixels (a = alpha)
    color = texColor;
}  