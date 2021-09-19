#include "world_chunk.h"
#include "world.h"
#include "../globals/shaders.h"
#include <fastnoise/FastNoiseLite.h>
#include "../physics.h"
#include "../components/components.h"
#include "../globals/models.h"
#include "../globals/textures.h"

glg::world::Chunk::Chunk(glm::ivec2 position)
{
	this->model = generateModel(position);
	glg::Mesh& mesh = model->meshes[0];
	mesh.setupMesh();

	// create collision shape
	triangleArray = new rp3d::TriangleVertexArray(mesh.vertices.size(), &mesh.vertices[0], sizeof(Vertex), mesh.indices.size() / 3, &mesh.indices[0], 3 * sizeof(unsigned int),
	rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
	rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
	triangleMesh = PHYSICS_COMMON.createTriangleMesh();
	triangleMesh->addSubpart(triangleArray);
	concaveMesh = PHYSICS_COMMON.createConcaveMeshShape(triangleMesh);

	object = createObject();
}

glg::world::Chunk::Chunk(glm::ivec2 position, Model* model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh) : position(position), triangleArray(triangleArray)
, triangleMesh(triangleMesh), concaveMesh(concaveMesh)
{
	this->model = model;
	object = createObject();
}

glg::world::Chunk::~Chunk()
{
	object.destory();
	delete triangleArray;
	PHYSICS_COMMON.destroyTriangleMesh(triangleMesh);
	PHYSICS_COMMON.destroyConcaveMeshShape(concaveMesh);
	delete model;
}

glg::Object& glg::world::Chunk::createObject()
{
	Object object;
	object.addComponent<TransformComponent>(glm::vec3(position.x, 0, position.y) * world::CHUNK_SIZE, glm::identity<glm::quat>());

	object.addComponent<ModelComponent>(model, shaders::defaultShader);

	//std::vector<LodModel> models = { LodModel(model, 0), LodModel(generateModel(2), 32 * 3), LodModel(generateModel(4), 32 * 6), LodModel(generateModel(8), 32 * 14) };

	//object.addComponent<LodComponent>(models);

	// creates rigidbody
	rp3d::RigidBody* groundRigidbody = PHYSICS_WORLD->createRigidBody(object.get<TransformComponent>());
	groundRigidbody->setType(rp3d::BodyType::KINEMATIC);

	groundRigidbody->addCollider(concaveMesh, rp3d::Transform::identity());

	object.addComponent<PhysicsComponent>(groundRigidbody);

	return object;
}

glg::Model* glg::world::Chunk::generateModel(glm::ivec2 position)
{
	size_t resolution = CHUNK_RESOLUTION;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices(resolution * resolution * 6);

	for (int x = 0; x <= resolution; x++) {
		for (int z = 0; z <= resolution; z++) {
			float localX, localZ;
			localX = float(x) / resolution * world::CHUNK_SIZE;
			localZ = float(z) / resolution * world::CHUNK_SIZE;

			float displacement = world::NOISE_SETTINGS.noise.GetNoise(float(localX + (position.x * world::CHUNK_SIZE)), float(localZ + (position.y * world::CHUNK_SIZE))) * world::NOISE_SETTINGS.displacementHeight;

			vertices.push_back(Vertex(glm::vec3(localX, displacement, localZ), glm::vec3(0, 1, 0), glm::vec2(0, 0)));
		}
	}

	for (size_t ii = 0, vi = 0, x = 0; x < resolution; x++, vi++) {
		for (size_t z = 0; z < resolution; z++, ii += 6, vi++) {
			indices[ii] = vi;
			indices[ii + 4] = indices[ii + 1] = vi + 1;
			indices[ii + 3] = indices[ii + 2] = vi + resolution + 1;
			indices[ii + 5] = vi + resolution + 2;
		}
	}

	std::vector<Vertex> newVertices;
	std::vector<unsigned int> newIndices;
	for (size_t i = 0; i < indices.size(); i++) {
		newVertices.push_back(vertices[indices[i]]);
		newIndices.push_back(i);
	}

	std::vector<Texture2D> textures{*textures::defaultTexture};

	Mesh mesh(newVertices, newIndices, textures, Material(glm::vec3(1), glm::vec3(1), glm::vec3(.3), 32));
	mesh.calculateNormals();

	Model* model = new Model();
	model->meshes.push_back(mesh);

	return model;
}
