#include "world_chunk.h"
#include "world.h"
#include "../globals/shaders.h"
#include "../physics.h"
#include "../components/components.h"
#include "../globals/models.h"
#include "../globals/textures.h"
#include "../mesh_gen/marching_cubes.h"
#include <boost/multi_array.hpp>
#include <algorithm>
#include <fastnoise/FastNoise.h>

glg::world::Chunk::Chunk(chunkVec position) : position(position)
{
	this->model = generateModel(position);
	glg::Mesh& mesh = model->meshes[0];
	mesh.setupMesh();

	//std::tie(triangleArray, triangleMesh, concaveMesh) = generateConcaveMeshShape(model);

	object = createObject();
}

glg::world::Chunk::Chunk(chunkVec position, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh) : position(position)/*, triangleArray(triangleArray)
, triangleMesh(triangleMesh), concaveMesh(concaveMesh)*/
{
	this->model = model;
	object = createObject();
}

glg::world::Chunk::~Chunk()
{
	object.destory();
	PHYSICS_MUTEX.lock();
	//delete triangleArray;
	//PHYSICS_COMMON.destroyTriangleMesh(triangleMesh);
	//PHYSICS_COMMON.destroyConcaveMeshShape(concaveMesh);
	PHYSICS_MUTEX.unlock();
}

glg::Object glg::world::Chunk::createObject()
{
	Object object;
	object.addComponent<TransformComponent>(glm::vec3(position.x, position.y, position.z) * world::CHUNK_SIZE, glm::identity<glm::quat>());

	object.addComponent<ModelComponent>(model.get(), shaders::defaultShader);

	float highestY = -INFINITY;

	for (const Vertex& vertex : model->meshes[0].vertices) {
		if (vertex.position.y > highestY) {
			highestY = vertex.position.y;
		}
	}

	object.addComponent<BoxCullComponent>(glm::vec3(world::CHUNK_SIZE.x / 2, highestY, world::CHUNK_SIZE.z / 2), glm::vec3(world::CHUNK_SIZE.x / 2, 0, world::CHUNK_SIZE.x / 2));

	//std::vector<LodModel> models = { LodModel(model, 0), LodModel(generateModel(2), 32 * 3), LodModel(generateModel(4), 32 * 6), LodModel(generateModel(8), 32 * 14) };

	//object.addComponent<LodComponent>(models);

	// creates rigidbody
	//rp3d::RigidBody* groundRigidbody = PHYSICS_WORLD->createRigidBody(object.get<TransformComponent>());
	//groundRigidbody->setType(rp3d::BodyType::KINEMATIC);

	//groundRigidbody->addCollider(concaveMesh, rp3d::Transform::identity());

	//object.addComponent<PhysicsComponent>(groundRigidbody);

	return object;
}

std::shared_ptr<glg::Model> glg::world::Chunk::generateModel(const chunkVec& position)
{
	glm::ivec3 resolution = CHUNK_RESOLUTION + glm::ivec3(1);

	MarchingCubes cubes(resolution);

	float worldX, worldZ, worldY;
	std::vector<float> noiseOutput = world::NOISE_SETTINGS.GenUniformNoise3D(glm::ivec3(position.x * CHUNK_RESOLUTION.x, position.y * CHUNK_RESOLUTION.y, position.z * CHUNK_RESOLUTION.z),
		glm::ivec3(resolution.x, resolution.y, resolution.z), .0050f, 0);

	for (int i = 0, x = 0; x < resolution.x; x++) {
		for (int z = 0; z < resolution.z; z++) {
			worldX = float(x) / CHUNK_RESOLUTION.x * world::CHUNK_SIZE.x;
			worldZ = float(z) / CHUNK_RESOLUTION.z * world::CHUNK_SIZE.z;
			//float noiseValue = world::NOISE_SETTINGS.noise.GetNoise(float(worldX + (position.x * world::CHUNK_SIZE.x)), float(worldZ + (position.z * world::CHUNK_SIZE.y)));
			//noiseValue = ((noiseValue + 1) / 2);

			//float displacementValue = noiseValue * world::NOISE_SETTINGS.displacementHeight / CHUNK_RESOLUTION.y * world::CHUNK_SIZE.y + 15;

			for (int y = 0; y < resolution.y; y++, i++) {
				worldY = float(y) / CHUNK_RESOLUTION.y * world::CHUNK_SIZE.y;

				//float noiseValue3D = world::NOISE_SETTINGS.noise->ge(float(worldX + (position.x * world::CHUNK_SIZE.x)), float(worldY) + (position.y * world::CHUNK_SIZE.y),
				//	float(worldZ + (position.z * world::CHUNK_SIZE.z)));
				////noiseValue3D = -((noiseValue3D + 1) / 2);
				//noiseValue3D *= world::NOISE_SETTINGS.displacementHeight;

				//float value = float(worldY) + (position.y * world::CHUNK_SIZE.y);

				//value += noiseValue3D;

				float value = noiseOutput[(z * resolution.x * resolution.y) + (y * resolution.x) + x];

				//if (noiseValue3D > .3)

				cubes.getVoxel(glm::ivec3(x, y, z)) = Voxel(glm::vec3(worldX, worldY, worldZ), value);
			}
		}
	}

	std::vector<Texture2D> textures{ *textures::defaultTexture };

	std::shared_ptr<Model> model = cubes.createModel(0.0f, textures);
	model->meshes[0].calculateNormals();

	return model;
}

std::tuple<rp3d::TriangleVertexArray*, rp3d::TriangleMesh*, rp3d::ConcaveMeshShape*> glg::world::Chunk::generateConcaveMeshShape(std::shared_ptr<const Model> model)
{
	const Mesh& mesh = model->meshes[0];

	//rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(mesh.vertices.size(), &mesh.vertices[0], sizeof(Vertex), mesh.indices.size() / 3, &mesh.indices[0], 3 * sizeof(unsigned int),
	//	rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
	//	rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
	//PHYSICS_MUTEX.lock();
	//rp3d::TriangleMesh* triangleMesh = PHYSICS_COMMON.createTriangleMesh();
	//triangleMesh->addSubpart(triangleArray);
	//rp3d::ConcaveMeshShape* concaveMesh = PHYSICS_COMMON.createConcaveMeshShape(triangleMesh);
	//PHYSICS_MUTEX.unlock();

	//return { triangleArray, triangleMesh, concaveMesh };
	return { nullptr, nullptr, nullptr };
}
