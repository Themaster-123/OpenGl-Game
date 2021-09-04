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

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight directionalLights[MAX_LIGHTS / 3];
uniform int directionalLightsSize = 0;


struct PointLight {
	vec3 position;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform PointLight pointLights[MAX_LIGHTS / 3];
uniform int pointLightsSize = 0;

struct SpotLight {
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

uniform SpotLight spotLights[MAX_LIGHTS / 3];
uniform int spotLightsSize = 0;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir);

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
	/*vec3 viewLightPosition = vec3(view * vec4(light.position, 1));
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
	specular *= intensity;*/

	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(-fragPos);

	vec3 result = vec3(0);

	for(int i = 0; i < directionalLightsSize; i++) {
		result += calculateDirectionalLight(directionalLights[i], norm, viewDir);
	}

	for(int i = 0; i < pointLightsSize; i++) {
		result += calculatePointLight(pointLights[i], norm, viewDir);
	}

	for(int i = 0; i < spotLightsSize; i++) {
		result += calculateSpotLight(spotLights[i], norm, viewDir);
	}

	FragColor = texture(texture_diffuse1, texCoord) * vec4(result, 1);
	
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 viewLightDirection = normalize(mat3(view) * light.direction);

	float diff = max(dot(normal, -viewLightDirection), 0);

	vec3 reflectDir = reflect(viewLightDirection, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	return (ambient + diffuse + specular);
};

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 viewLightPosition = vec3(view * vec4(light.position, 1));

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

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
	vec3 viewLightPosition = vec3(view * vec4(light.position, 1));
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