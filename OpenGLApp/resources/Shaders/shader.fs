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

// NUOVI PARAMETRI DI LUCE
uniform float specularStrength; // Quanto è lucido l'oggetto?
uniform float emissionStrength; // Quanto brilla al buio?

void main()
{
    // --- 1. AMBIENT ---
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * lightColor;

    // --- 2. DIFFUSE ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // --- 3. SPECULAR ---
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // --- 4. RIM LIGHTING (NUOVO!) ---
    // Calcola quanto la superficie è perpendicolare alla vista.
    // Più il bordo è "di taglio", più si illumina.
    float rimDot = 1.0 - max(dot(viewDir, norm), 0.0);
    // Intensifichiamo l'effetto (pow) e lo moltiplichiamo per un colore (bianco/viola)
    float rimAmount = pow(rimDot, 4.0);
    // Lo applichiamo solo se l'oggetto è lucido (evitiamo sul cestino opaco)
    vec3 rim = rimAmount * vec3(1.0, 1.0, 1.0) * specularStrength;

    vec4 resultColor;

    if (useTexture) {
        // CESTINO (Texture, niente Rim Light forte)
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        vec3 lighting = (ambient + diffuse + specular) * texColor.rgb;
        resultColor = vec4(lighting, texColor.a);
    }
    else {
        // MELE e DIAMANTI (Colore + Rim Light + Emissione)

        // Base Lighting
        vec3 lighting = (ambient + diffuse + specular) * objectColor;

        // Aggiungiamo Rim Light + Emissione
        vec3 finalRGB = lighting + (objectColor * emissionStrength) + rim;

        // Qui decidiamo l'Alpha (Trasparenza).
        // Se è un diamante (lo capiremo dall'objectColor ciano), lo facciamo trasparente.
        float alpha = 1.0;

        // Trucco sporco ma efficace: se l'oggetto è ciano/blu (Diamante), riduciamo l'alpha
        if(objectColor.g > 0.8 && objectColor.b > 0.8 && objectColor.r < 0.2) {
             alpha = 0.6; // 60% visibile, 40% trasparente
             finalRGB += vec3(0.2); // Aggiungiamo un po' di bianco per farlo brillare
        }

        resultColor = vec4(finalRGB, alpha);
    }

    FragColor = resultColor;
}