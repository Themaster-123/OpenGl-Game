#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	FragColor = texture(texture_diffuse1, texCoord); //* vec4(ambient.rgb, 1);
}