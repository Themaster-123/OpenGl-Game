#include "light_entity.h"
#include "../../scene.h"

glg::LightEntity::LightEntity(glm::vec3 position, glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority = 0) : Entity(position, rotation), ambient(ambient), diffuse(diffuse), 
specular(specular), priority(priority)
{
	scene::getLights().push_back(this);
	std::sort(scene::getLights().begin(), scene::getLights().end(), sortLightByPriority);
}

glg::LightEntity::LightEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority = 0) : Entity(position, rotation), ambient(ambient), diffuse(diffuse), 
specular(specular), priority(priority)
{
	scene::getLights().push_back(this);
	std::sort(scene::getLights().begin(), scene::getLights().end(), sortLightByPriority);
}

glg::LightEntity::~LightEntity()
{
	int eraseIndex = -1;
	for (int i = 0; i < scene::getLights().size(); i++) {
		if (this == scene::getLights()[i]) {
			eraseIndex = i;
		}
	}
	scene::getLights().erase(scene::getLights().begin() + eraseIndex);
}

bool glg::LightEntity::sortLightByPriority(LightEntity* light1, LightEntity* light2)
{
	return light1->priority > light2->priority;
}