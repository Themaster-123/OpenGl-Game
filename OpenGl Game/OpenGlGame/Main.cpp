#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "resources/shader.h"
#include "resources/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utility>
#include <vector>
#include "screen.h"
#include "resources/model.h"
#include "globals/shaders.h"
#include "globals/models.h"
#include <reactphysics3d/reactphysics3d.h>
#include "physics.h"
#include "essential/object.h"
#include "components/components.h"
#include "component_systems/component_system.h"
#include "globals/textures.h"

using namespace glg;

void loadOpenGlFunctions();

void cleanUp();

int main() {
	// initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
	textures::registerTextures();
	registerPhysics();
	ComponentSystem::addSystems();

	Scene scene;

	// creates world
	Object world(&scene);
	world.addComponent<WorldComponent>();

	Object worldLight(&scene);
	worldLight.addComponent<TransformComponent>(world, glm::vec3(), glm::vec3(-45, 0, 0));
	worldLight.addComponent<DirectionalLightComponent>();
	worldLight.get<LightComponent>().ambient = glm::vec3(.4f);
	worldLight.get<LightComponent>().diffuse = glm::vec3(.7f);

	// creates other stuff
	Object playerObject(&scene);
	playerObject.addComponent<TransformComponent>(world);
	playerObject.addComponent<CameraComponent>();
	playerObject.addComponent<PlayerComponent>();
	playerObject.addComponent<SpotLightComponent>();
	rp3d::RigidBody* body = PHYSICS_WORLD->createRigidBody(playerObject.get<TransformComponent>());
	body->setType(rp3d::BodyType::KINEMATIC);
	body->addCollider(PHYSICS_COMMON.createCapsuleShape(1.0f, 2.0f), rp3d::Transform::identity());
	playerObject.addComponent<PhysicsComponent>(body);

	rp3d::SphereShape* sphereShape = PHYSICS_COMMON.createSphereShape(1.0f);

	for (int i = 0; i < 0; i++) {
		Object obj(&scene);
		obj.addComponent<TransformComponent>(glm::vec3(-28, 100, -28));
		obj.addComponent<ModelComponent>(models::sphereModel, shaders::defaultShader);
		rp3d::RigidBody* body = PHYSICS_WORLD->createRigidBody(obj.get<TransformComponent>());
		body->addCollider(sphereShape, rp3d::Transform::identity());
		obj.addComponent<PhysicsComponent>(body);
	}
	//std::this_thread::sleep_for(std::chrono::seconds(50));
	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glfwSwapInterval(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//scene::WORLD.loadChunk(chunkVec(0, 0, 0));
	//scene::WORLD.loadChunk(chunkVec(0, 0, 1));
	//scene::WORLD.loadChunk(chunkVec(1, 0, 0));
	//scene::WORLD.loadChunk(chunkVec(1, 0, 1));
	//scene::WORLD.loadChunk(chunkVec(1, 0, 2));
	//scene::WORLD.loadChunk(chunkVec(1, 0, 3));
	//scene::WORLD.loadChunk(chunkVec(0, 0, 2));
	//scene::WORLD.loadChunk(chunkVec(0, 0, 3));
	//scene::WORLD.loadChunk(chunkVec(0, 0, 4));
	//scene::WORLD.loadChunk(chunkVec(0, 0, 5));
	//scene::WORLD.loadChunk(chunkVec(1, 0, 1));
	//scene::WORLD.loadChunk(chunkVec(0, 1, 0));
	//scene::WORLD.loadChunk(chunkVec(0, 1, 1));
	//scene::WORLD.loadChunk(chunkVec(1, 1, 0));
	//scene::WORLD.loadChunk(chunkVec(1, 1, 1));

	startRenderLoop();

	glfwTerminate();

	cleanUp();

	return 0;
}

void loadOpenGlFunctions() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("No glad just sad");
	}
}

void cleanUp()
{
	for (auto* system : Scene::getGlobalSystems()) {
		delete system;
	}
}

