#pragma once
#include "../../components/components.h"
#include "../component_system.h"
#include <entt/entt.hpp>
#include "../../essential/object.h"
#include "../transform_system/transform_system.h"

namespace glg {
	struct ViewFrustum {
		enum {
			TOP, 
			BOTTOM, 
			LEFT,
			RIGHT, 
			NEARP, 
			FARP
		};

		struct ViewPlane {
			glm::vec3 position;
			glm::vec3 normal;

			ViewPlane() = default;

			ViewPlane(glm::vec3 position, glm::vec3 normal);
		};

		struct BoundingCube {
			glm::vec3 points[8];

			BoundingCube(glm::vec3 size, glm::vec3 offset = glm::vec3(0));
		};

		ViewPlane planes[6];

		ViewFrustum(const CameraComponent& camera, const TransformComponent& transform);

		bool isInside(const glm::vec3& point, float radius = 0) const;

		bool isInside(const TransformComponent& transform, glm::vec3 size, glm::vec3 offset);

	};

	class RendererSystem : public ComponentSystem {
	public:
		RendererSystem();

		virtual void draw(Scene* scene);

		void registerDependencies(Scene* scene);

		static void onConstruct(entt::registry& registry, entt::entity entity);

		static void onCameraConstruct(entt::registry& registry, entt::entity entity);

		static void onLightConstruct(entt::registry& registry, entt::entity entity);

		static void onAttenuationLightConstruct(entt::registry& registry, entt::entity entity);

		static void onLodConstruct(entt::registry& registry, entt::entity entity);

		static void drawModel(const Object& object, const CameraComponent& cameraComponent, const TransformComponent& cameraTransformComponent, ViewFrustum frustum);

		static void drawPhysicsModel(const Object& object, const CameraComponent& cameraComponent, const TransformComponent& cameraTransformComponent, ViewFrustum frustum);

		static void drawModel(const Object& object, const TransformComponent& transformComponent, const CameraComponent& cameraComponent, const TransformComponent& cameraTransformComponent, ViewFrustum frustum);

		static glm::mat4 getViewMatrix(const TransformComponent& transformComponent);

		static glm::mat4 getProjectionMatrix(const CameraComponent& cameraComponent);

		static void setLightUniforms(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, int lightType, int index, int size);

	};
}
