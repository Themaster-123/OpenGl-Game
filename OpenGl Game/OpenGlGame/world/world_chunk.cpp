#include "world_chunk.h"
#include "world.h"
#include "../globals/shaders.h"
#include <fastnoise/FastNoiseLite.h>
#include "../physics.h"
#include "../components/components.h"
#include "../globals/models.h"
#include "../globals/textures.h"
#include "../mesh_gen/marching_cubes.h"
#include <boost/multi_array.hpp>

glg::world::Chunk::Chunk(chunkVec position)
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
	object.addComponent<TransformComponent>(glm::vec3(position.x, 0, position.y) * world::CHUNK_SIZE, glm::identity<glm::quat>());

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
	//size_t resolution = CHUNK_RESOLUTION;

	//std::vector<Vertex> vertices;
	//vertices.reserve((resolution + 1) * (resolution + 1));
	//std::vector<unsigned int> indices(resolution * resolution * 6);

	//for (int x = 0; x <= resolution; x++) {
	//	for (int z = 0; z <= resolution; z++) {
	//		float localX, localZ;
	//		localX = float(x) / resolution * world::CHUNK_SIZE;
	//		localZ = float(z) / resolution * world::CHUNK_SIZE;

	//		float displacement = world::NOISE_SETTINGS.noise.GetNoise(float(localX + (position.x * world::CHUNK_SIZE)), float(localZ + (position.y * world::CHUNK_SIZE))) * world::NOISE_SETTINGS.displacementHeight;

	//		vertices.emplace_back(glm::vec3(localX, displacement, localZ), glm::vec3(0, 1, 0), glm::vec2(0, 0));
	//	}
	//}

	//for (size_t ii = 0, vi = 0, x = 0; x < resolution; x++, vi++) {
	//	for (size_t z = 0; z < resolution; z++, ii += 6, vi++) {
	//		indices[ii] = vi;
	//		indices[ii + 4] = indices[ii + 1] = vi + 1;
	//		indices[ii + 3] = indices[ii + 2] = vi + resolution + 1;
	//		indices[ii + 5] = vi + resolution + 2;
	//	}
	//}

	//std::vector<Vertex> newVertices;
	//newVertices.reserve(indices.size());
	//std::vector<unsigned int> newIndices;
	//newIndices.reserve(indices.size());
	//for (size_t i = 0; i < indices.size(); i++) {
	//	newVertices.push_back(vertices[indices[i]]);
	//	newIndices.emplace_back(i);
	//}

	//std::vector<Texture2D> textures { *textures::defaultTexture };


	//Mesh mesh(newVertices, newIndices, textures, Material(glm::vec3(1), glm::vec3(1), glm::vec3(.3), 32));
	//mesh.calculateNormals();

	//auto model = std::make_shared<Model>();

	//model->meshes.push_back(mesh);

	glm::ivec3 resolution = CHUNK_RESOLUTION + glm::ivec3(1);
	//glm::ivec3 borderChunkSize = CHUNK_SIZE + (c)

	boost::multi_array<Voxel, 3> voxels(boost::extents[(size_t)resolution.x][(size_t)resolution.y][(size_t)resolution.z]);
	//voxels.reserve();

	/*std::cout << voxels.size();

	float maxNoiseValue = 0;

	for (int x = 0; x < resolution.x; x++) {
		for (int z = 0; z < resolution.z; z++) {
			float worldX, worldZ, worldY;
			worldX = float(x) / resolution.x * world::CHUNK_SIZE.x;
			worldZ = float(z) / resolution.z * world::CHUNK_SIZE.z;
			float noiseValue = world::NOISE_SETTINGS.noise.GetNoise(float(worldX + (position.x * world::CHUNK_SIZE.x)), float(worldZ + (position.y * world::CHUNK_SIZE.y)));
			noiseValue = ((noiseValue + 1) / 2);

			float displacementValue = noiseValue * world::NOISE_SETTINGS.displacementHeight / resolution.y * CHUNK_SIZE.y;

			for (int y = 0; y < resolution.y; y++) {
				worldY = float(y) / resolution.y * world::CHUNK_SIZE.y;

				if (worldY > displacementValue) {
					noiseValue = 1.1;
				}
				else {
					noiseValue = displacementValue / world::NOISE_SETTINGS.displacementHeight;
				}

				//std::cout << x + resolution.x * (y + resolution.z * z) << std::endl;

				voxels[x][y][z] = { noiseValue, glm::vec3(worldX, worldY, worldZ) };
			}
		}
	}*/

	float worldX, worldZ, worldY;

	for (int y = 0; y < resolution.y; y++) {
		for (int x = 0; x < resolution.x; x++) {
			for (int z = 0; z < resolution.z; z++) {
				worldX = float(x) / CHUNK_RESOLUTION.x * world::CHUNK_SIZE.x;
				worldY = float(y) / CHUNK_RESOLUTION.y * world::CHUNK_SIZE.y;
				worldZ = float(z) / CHUNK_RESOLUTION.z * world::CHUNK_SIZE.z;
				float noiseValue = world::NOISE_SETTINGS.noise.GetNoise(float(worldX + (position.x * world::CHUNK_SIZE.x)), float(worldY),
					float(worldZ + (position.y * world::CHUNK_SIZE.z)));
				noiseValue = ((noiseValue + 1) / 2);

				//std::cout << noiseValue << std::endl;

				voxels[x][y][z] = { noiseValue, glm::vec3(worldX, worldY, worldZ) };
			}
		}
	}

	MarchingCubes cubes(voxels, glm::vec3(1));

	std::vector<Texture2D> textures{ *textures::defaultTexture };

	std::shared_ptr<Model> model = cubes.createModel(.6, textures);
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
