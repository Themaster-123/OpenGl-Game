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

#define MAX_LIGHTS 100

struct Light {
	vec4 position;
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

uniform Light lights[MAX_LIGHTS];
uniform int lightsSize = 0;

vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 viewDir);

vec3 calculatePointLight(Light light, vec3 normal, vec3 viewDir);

vec3 calculateSpotLight(Light light, vec3 normal, vec3 viewDir);

vec3 calculateLight(Light light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(-fragPos);

	vec3 result = vec3(0);

	for(int i = 0; i < lightsSize; i++) {
		result += calculateLight(lights[i], norm, viewDir);
	}

	FragColor = texture(texture_diffuse1, texCoord) * vec4(result, 1);
	
}

vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 viewLightDirection = normalize(mat3(view) * light.direction);

	float diff = max(dot(normal, -viewLightDirection), 0);

	vec3 reflectDir = reflect(viewLightDirection, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	return (ambient + diffuse + specular);
};

vec3 calculatePointLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 viewLightPosition = vec3(view * vec4(light.position.xyz, 1));

	vec3 lightDir = normalize(viewLightPosition - fragPos);
	
	float diff = max(dot(normal, lightDir), 0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	
	float dist = length(viewLightPosition - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
};

vec3 calculateSpotLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 viewLightPosition = vec3(view * vec4(light.position.xyz, 1));
	vec3 viewLightDirection = normalize(mat3(view) * light.direction);

	vec3 lightDir = normalize(viewLightPosition - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	
	float dist = length(viewLightPosition - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float theta = dot(lightDir, -viewLightDirection);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0, 1);

	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}

vec3 calculateLight(Light light, vec3 normal, vec3 viewDir) {
	if (light.position.w == 0) {
		return calculateDirectionalLight(light, normal, viewDir);
	} else if (light.position.w == 1) {
		return calculatePointLight(light, normal, viewDir);
	} else {
		return calculateSpotLight(light, normal, viewDir);
	}

}