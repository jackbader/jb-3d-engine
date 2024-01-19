#version 410 core

out vec4 FragColor;
in vec3 vertexColor;
in vec3 Normal;     // Normal vector
in vec3 FragPos;    // Fragment position
in vec2 TexCoord;   // Texture coordinates

uniform bool useTexture; // Add this line
uniform sampler2D texture1; // Texture sampler

// Light properties
uniform vec3 lightPos;       // Light position
uniform vec3 lightColor;     // Light color
uniform vec3 viewPos;        // Camera position

uniform bool useLighting;

void main()
{
   // Ambient
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   // Diffuse 
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   // Specular
   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;  

   vec4 texColor = useTexture ? texture(texture1, TexCoord) : vec4(1.0); // Check useTexture before sampling

   if (useLighting) {
      // Perform lighting calculations
      vec3 result = (ambient + diffuse + specular) * texColor.rgb; // uses texture color here!!
      FragColor = vec4(result, 1.0);
   } else {
      // Output the color without lighting
      FragColor = vec4(vertexColor, 1.0);
   }
}