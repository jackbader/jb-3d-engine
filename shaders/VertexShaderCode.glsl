#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal; // Normal vector
layout (location = 2) in vec3 inVertexColor;
layout (location = 3) in vec2 aTexCoords; // Texture coordinates

out vec3 vertexColor;
out vec3 FragPos;   // Fragment position
out vec3 Normal;    // Normal
out vec2 TexCoord;  // Texture coordinates

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool useLighting;

vec4 transformedPosition = projection * view * model * vec4(aPos, 1.0);

void main()
{
  // set transformed position
  gl_Position = transformedPosition;

  // pass the vertex color data to the Fragment Shader
  vertexColor = inVertexColor;

  // pass the texture coordinates to the Fragment Shader
  TexCoord = aTexCoords;

  FragPos = vec3(model * vec4(aPos, 1.0)); // Position in world space
  Normal = mat3(transpose(inverse(model))) * inNormal; // Transform normals
}