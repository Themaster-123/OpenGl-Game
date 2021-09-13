#pragma once
#include "../../components/components.h"
#include "../component_system.h"
#include <entt/entt.hpp>
#include "../../essential/object.h"

namespace glg {
	class RendererSystem : public ComponentSystem {
	public:

		RendererSystem();

		virtual void draw();

		static void onConstruct(entt::registry& registry, entt::entity entity);

		static void onCameraConstruct(entt::registry& registry, entt::entity entity);

		static void onLightConstruct(entt::registry& registry, entt::entity entity);

		static void onAttenuationLightConstruct(entt::registry& registry, entt::entity entity);

		static void drawModel(const Object& object);

		static void drawPhysicsModel(const Object& object);

		static glm::mat4 getViewMatrix(const Object& object);

		static glm::mat4 getProjectionMatrix(const Object& object);

		static void setLightUniforms(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, int lightType, int index, int size);
	};
}
