#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform bool useTexture;

// New parameters
uniform float specularStrength;
uniform float emissionStrength;

void main()
{
    // AMBIENT
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * lightColor;

    // DIFFUSE
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // SPECULAR
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // RIM LIGHTING, TO LIGHT CUTTING EDGES
    float rimDot = 1.0 - max(dot(viewDir, norm), 0.0);
    float rimAmount = pow(rimDot, 4.0); //  pow intensify the effect
    vec3 rim = rimAmount * vec3(1.0, 1.0, 1.0) * specularStrength; // for lucid objects

    vec4 resultColor;

    if (useTexture) {
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        vec3 lighting = (ambient + diffuse + specular) * texColor.rgb;
        resultColor = vec4(lighting, texColor.a);
    }
    else {
        // Base Lighting
        vec3 lighting = (ambient + diffuse + specular) * objectColor;

        // Rim Light + Emission
        vec3 finalRGB = lighting + (objectColor * emissionStrength) + rim;

        // Transparence
        float alpha = 1.0;

        // Reducing alpha if it is a diamond (color blue)
        if(objectColor.g > 0.8 && objectColor.b > 0.8 && objectColor.r < 0.2) {
             alpha = 0.6;
             finalRGB += vec3(0.2); // Adding a white component
        }

        resultColor = vec4(finalRGB, alpha);
    }

    FragColor = resultColor;
}