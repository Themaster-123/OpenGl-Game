#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform mat4 view;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
	vec3 position;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(view * vec4(light.position, 1)) - worldPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (material.diffuse * diff);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(normalize(-worldPos), reflectDir), 0), material.shininess);
	vec3 specular = light.specular * (material.specular * spec);
	
	vec3 result = (ambient + diffuse + specular);

	FragColor = texture(texture_diffuse1, texCoord) * vec4(result, 1);
	
}