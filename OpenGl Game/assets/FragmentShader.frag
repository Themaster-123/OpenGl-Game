#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D fTexture;
void main()
{
	FragColor = texture(fTexture, texCoord) * vec4(vertexColor, 1);
}