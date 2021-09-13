#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "resources/shader.h"
#include "resources/texture.h"
#include "entities/camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utility>
#include <vector>
#include "screen.h"
#include "entities/Alive/player.h"
#include "resources/model.h"
#include "entities/visible_entity.h"
#include "globals/shaders.h"
#include "globals/models.h"
#include <reactphysics3d/reactphysics3d.h>
#include "physics.h"
#include "entities/object/sphere_entity.h"
#include "entities/light/attenuation/point_light_entity.h"
#include "entities/light/attenuation/spot_light_entity.h"
#include "entities/light/directional_light_entity.h"
#include "essential/object.h"
#include "components/components.h"
#include "component_systems/component_system.h"

using namespace glg;

Camera camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 70, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);

void loadOpenGlFunctions();

int main() {
	
	// initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	// window creation
	GLFWwindow* window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test");
	lockCursor(window, true);
	// load all OpenGl function pointers
	loadOpenGlFunctions();
	shaders::registerShaders();
	models::registerModels();
	registerPhysics();
	ComponentSystem::addSystems();
	
	//shader.use();

	Object worldLight;
	worldLight.addComponent<DirectionalLightComponent>();

	// creates ground
	{
		Object ground;
		ground.addComponent<TransformComponent>(glm::vec3(0, -10, 0), glm::identity<glm::quat>());
		ground.addComponent<ModelComponent>(models::terrainModel, shaders::defaultShader);

		rp3d::RigidBody* groundRigidbody = PHYSICS_WORLD->createRigidBody(ground.get<TransformComponent>());
		groundRigidbody->setType(rp3d::BodyType::KINEMATIC);
		glg::Mesh* mesh = new Mesh(models::terrainModel.getMeshes()[0]);
		rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(mesh->vertices.size(), &mesh->vertices[0], sizeof(Vertex), mesh->indices.size() / 3, &mesh->indices[0], 3 * sizeof(unsigned int),
			rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
		rp3d::TriangleMesh* triangleMesh = PHYSICS_COMMON.createTriangleMesh();
		triangleMesh->addSubpart(triangleArray);
		rp3d::ConcaveMeshShape* concaveMesh = PHYSICS_COMMON.createConcaveMeshShape(triangleMesh);
		groundRigidbody->addCollider(concaveMesh, rp3d::Transform::identity());
		ground.addComponent<PhysicsComponent>(groundRigidbody);
	}

	// creates other stuff
	Object playerObject;
	playerObject.addComponent<CameraComponent>();
	playerObject.addComponent<PlayerComponent>();
	playerObject.addComponent<SpotLightComponent>();

	rp3d::SphereShape* sphereShape = PHYSICS_COMMON.createSphereShape(1.0f);

	for (int i = 0; i < 100; i++) {
		Object obj;
		obj.addComponent<ModelComponent>(models::sphereModel, shaders::defaultShader);
		rp3d::RigidBody* body = PHYSICS_WORLD->createRigidBody(obj.get<TransformComponent>());
		body->addCollider(sphereShape, rp3d::Transform::identity());
		obj.addComponent<PhysicsComponent>(body);
	}

	Player player(camera.getPosition(), camera.getRotation(), camera);
	SphereEntity test(glm::vec3(0, 0, 0), rp3d::Quaternion::identity());
	DirectionalLightEntity light(glm::vec3(-53, 0, 0), glm::vec3(.1f), glm::vec3(1, 1, 1), glm::vec3(1));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	startRenderLoop();

	glfwTerminate();
	return 0;
}

void loadOpenGlFunctions() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("No glad just sad");
	}
}

//void setTextureSetting() {
//	glTexParameteri(GL_TEXTURE_2D)
//}


