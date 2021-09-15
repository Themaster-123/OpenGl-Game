#include "world_chunk.h"
#include "world.h"
#include "../globals/shaders.h"
#include <fastnoise/FastNoiseLite.h>
#include "../physics.h"

glg::world::Chunk::Chunk(glm::ivec2 position) : position(position)
{
	createObject();
}

glg::Object& glg::world::Chunk::createObject()
{
	Object object;
	object.addComponent<TransformComponent>(glm::vec3(position.x, 0, position.y) * world::CHUNK_SIZE, glm::identity<glm::quat>());

	Model* model = generateModel();
	object.addComponent<ModelComponent>(*model, shaders::defaultShader);

	// creates rigidbody
	rp3d::RigidBody* groundRigidbody = PHYSICS_WORLD->createRigidBody(object.get<TransformComponent>());
	groundRigidbody->setType(rp3d::BodyType::KINEMATIC);

	// create collision shape
	glg::Mesh& mesh = model->meshes[0];
	rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(mesh.vertices.size(), &mesh.vertices[0], sizeof(Vertex), mesh.indices.size() / 3, &mesh.indices[0], 3 * sizeof(unsigned int),
		rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
		rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
	rp3d::TriangleMesh* triangleMesh = PHYSICS_COMMON.createTriangleMesh();
	triangleMesh->addSubpart(triangleArray);
	rp3d::ConcaveMeshShape* concaveMesh = PHYSICS_COMMON.createConcaveMeshShape(triangleMesh);

	groundRigidbody->addCollider(concaveMesh, rp3d::Transform::identity());

	object.addComponent<PhysicsComponent>(groundRigidbody);

	return object;
}

glg::Model* glg::world::Chunk::generateModel()
{
	static size_t resolution = 32;

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

	std::vector<Texture2D> textures;

	Mesh mesh(newVertices, newIndices, textures, Material(glm::vec3(1), glm::vec3(1), glm::vec3(.3), 32));
	mesh.calculateNormals();

	Model* model = new Model();
	model->meshes.push_back(mesh);

	return model;
}
