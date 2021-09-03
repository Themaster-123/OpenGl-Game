#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform mat4 view;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(view * vec4(lightPos, 1)) - worldPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;

	float specularStrength = .5;
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(normalize(-worldPos), reflectDir), 0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diff + specular);

	FragColor = texture(texture_diffuse1, texCoord) * vec4(result, 1);
	
}