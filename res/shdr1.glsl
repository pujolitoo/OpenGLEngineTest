#vertex
#version 420 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoord;

out vec3 vertexColor;
out vec2 texCoord;

uniform mat4 u_MVP;

void main()
{
    vertexColor = vColor;
    texCoord = vTexCoord;
    gl_Position = vec4(vPosition, 1.0) * u_MVP;
} 

#fragment
#version 420 core
in vec3 vertexColor;
in vec2 texCoord;

out vec4 fColor;

uniform sampler2D ourTexture;

void main()
{
    fColor = texture(ourTexture, texCoord);
}