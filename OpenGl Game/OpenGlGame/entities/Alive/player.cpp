#include "player.h"
#include <utility>
#include "../../screen.h"
#include <iostream>
#include "../../globals/shaders.h"
#include "../../globals/models.h"

using namespace glg;

Player::Player(glm::vec3 position, glm::quat rotation, const Camera& camera, float sensitivity) : PhysicsEntity(position, rotation), camera(camera), sensitivity(sensitivity)
{
	this->collisionBody = getCollisionBody();
	setPosition(position);
	setRotation(rotation);
	MAIN_PLAYER = this;
}

Player::Player(glm::vec3 position, glm::vec3 rotation, const Camera& camera, float sensitivity) : PhysicsEntity(position, rotation), camera(camera), sensitivity(sensitivity)
{
	this->collisionBody = getCollisionBody();
	setPosition(position);
	setRotation(rotation);
	MAIN_PLAYER = this;
}

void Player::setPosition(glm::vec3 position)
{
	PhysicsEntity::setPosition(position);
	camera.setPosition(position);
}

void Player::setRotation(glm::quat rotation)
{
	PhysicsEntity::setRotation(rotation);
	glm::vec3 euler = glm::eulerAngles(rotation);
	this->rotation = glm::quat(glm::vec3(0, euler.y, 0));
	camera.setRotation(rotation);
	updateVectors();
}

void Player::setRotation(glm::vec3 rotation)
{
	setRotation(glm::quat(glm::radians(rotation)));
}

void glg::Player::setLookRotation(glm::vec2 rotation)
{
	setRotation(glm::vec3(rotation.x, rotation.y, 0));
	lookRotation = rotation;
}

glm::mat4 glg::Player::getModelMatrix() const
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), glm::vec3(0, 1, 0));
	return modelMatrix;
}

void Player::update()
{
	PhysicsEntity::update();
	processInput();
	setAspectRatio();
}

void glg::Player::physicsUpdate()
{
	//PhysicsEntity::physicsUpdate();
	rp3d::Transform transform = collisionBody->getTransform();
	transform.setOrientation(rp3d::Quaternion::identity());
	collisionBody->setTransform(transform);
}

void glg::Player::processInput()
{
	const float deltaSpeed = speed * DELTA_TIME;

	if (getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(GAME_WINDOW, true);
	}
	if (getKey(GLFW_KEY_W) == GLFW_PRESS) {
		move(glm::normalize(camera.getFront()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_S) == GLFW_PRESS) {
		move(glm::normalize(-camera.getFront()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_A) == GLFW_PRESS) {
		move(glm::normalize(-camera.getRight()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_D) == GLFW_PRESS) {
		move(glm::normalize(camera.getRight()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
		move(glm::normalize(camera.getUp()) * deltaSpeed);
	}
}

void Player::onMouseMovement(float xOffset, float yOffset, float xPos, float yPos)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	glm::vec2 lookRot = getLookRotation();
	lookRot.y -= xOffset;
	lookRot.x -= yOffset;
	lookRot.x = std::max(std::min(lookRot.x, 90.0f), -90.0f);
	setLookRotation(lookRot);
}

void glg::Player::draw()
{
}

Shader& glg::Player::getShader()
{
	return *shaders::defaultShader;
}

Model& glg::Player::getModel()
{
	return models::defaultModel;
}

rp3d::CollisionBody* glg::Player::getCollisionBody()
{
	rp3d::Transform transform = getTransform();
	rp3d::RigidBody* body = physicsWorld->createRigidBody(transform);
	body->setType(rp3d::BodyType::KINEMATIC);
	body->addCollider(physicsCommon.createCapsuleShape(1.0f, 2.0f), rp3d::Transform::identity());
	//rp3d::Material& mat = body->getCollider(0)->getMaterial();
	//mat.setFrictionCoefficient(9999);
	//mat.setBounciness(0);
	//mat.setRollingResistance(9999);
	//body->getCollider(0)->setMaterial(mat);
	return body;
}

void glg::Player::updateRotationToBodyRotation(rp3d::Transform transform)
{
}

void glg::Player::setAspectRatio()
{
	camera.aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
}

void glg::Player::setShaderProperties()
{
	for (Shader* shader : shaders::getShaders()) {
		shader->setMat4("view", camera.getViewMatrix());
		shader->setMat4("projection", camera.getProjectionMatrix());
		shader->setVec3("light.position", getPosition());
		shader->setVec3("light.ambient", glm::vec3(.1, .1, .1));
		shader->setVec3("light.diffuse", glm::vec3(1, 1, 1));
		shader->setVec3("light.specular", glm::vec3(1, 1, 1));
		shader->setFloat("light.constant", 1.0f);
		shader->setFloat("light.linear", 0.045f);
		shader->setFloat("light.quadratic", 0.0075f);
		shader->setVec3("light.direction", camera.getFront());
		shader->setFloat("light.cutOff", glm::cos(glm::radians(17.5f)));
		shader->setFloat("light.outerCutOff", glm::cos(glm::radians(29.5f)));
	}
}
