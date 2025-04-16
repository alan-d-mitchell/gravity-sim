#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
// We'll add some simple lighting parameters
uniform vec3 lightPos = vec3(100.0, 100.0, 100.0); // Default light position
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);     // White light
uniform vec3 viewPos = vec3(0.0, 0.0, 10.0);       // Default camera position

void main() {
    // Ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
        
    // Calculate final color
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
    
    // Add a rim lighting effect for a more sci-fi look
    float rimAmount = 0.7;
    float rimThreshold = 0.1;
    float rimStrength = 1.0 - dot(norm, viewDir);
    rimStrength = smoothstep(rimThreshold - 0.01, rimThreshold + 0.01, rimStrength);
    vec3 rim = rimStrength * rimAmount * vec3(0.3, 0.3, 1.0);
    
    FragColor = vec4(result + rim, 1.0);
}