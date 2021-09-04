#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

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

/*struct Light {
	vec3 position;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};*/

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Light light;

void main()
{
	vec3 viewLightPosition = vec3(view * vec4(light.position, 1));
	vec3 viewLightDirection = normalize(mat3(view) * light.direction);

	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(normal);

	vec3 lightDir = normalize(viewLightPosition - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (material.diffuse * diff);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(normalize(-fragPos), reflectDir), 0), material.shininess);
	vec3 specular = light.specular * (material.specular * spec);
	
	float dist = length(viewLightPosition - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result;

	float theta = dot(lightDir, -viewLightDirection);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0, 1);

	diffuse *= intensity;
	specular *= intensity;

	result = (ambient + diffuse + specular);

	FragColor = texture(texture_diffuse1, texCoord) * vec4(result, 1);
	
}