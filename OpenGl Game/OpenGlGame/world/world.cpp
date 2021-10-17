#include "world.h"
#include "../screen.h"
#include "../components/components.h"
#include "../globals/shaders.h"
#include "../globals/textures.h"

glm::vec3 glg::world::CHUNK_SIZE = glm::vec3(32);

unsigned int glg::world::CHUNK_LOAD_SIZE = 6;

glm::ivec3 glg::world::CHUNK_RESOLUTION = glm::ivec3(32);

glg::world::NoiseSettings glg::world::NOISE_SETTINGS;

void glg::world::setNoiseSetting()
{
	/*FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.02);
	noise.SetFractalOctaves(5);
	noise.SetFractalGain(0.5);
	noise.SetFractalLacunarity(2.00);
	noise.SetFractalType(noise.FractalType_FBm);*/
	FastNoise::SmartNode<> noise = FastNoise::NewFromEncodedNodeTree("EABcjyJAEQACAAAAAAAgQBAAAAAAQBkAEwDD9Sg/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPwEEAAAAAAAAAEBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM3MTD4AMzMzPwAAAAA/AAAAgD8=");
	//FastNoise::SmartNode<> noise = FastNoise::New<FastNoise::Simplex>();
	NOISE_SETTINGS = NoiseSettings(noise, 78);
}

glg::world::NoiseSettings::NoiseSettings(FastNoise::SmartNode<>& noise, float displacementHeight)
{
	this->noise = noise;
	this->displacementHeight = displacementHeight;
}

glg::world::World::World() : chunks(), chunkModels()
{
}

void glg::world::World::loadChunk(const chunkVec& chunkPos)
{
	if (!isChunkLoaded(chunkPos)) {
		std::shared_ptr<Model> model = glg::world::Chunk::generateModel(chunkPos);

		auto [triangleArray, triangleMesh, concaveMesh] = glg::world::Chunk::generateConcaveMeshShape(model);
		chunks.insert(std::pair<chunkVec, std::shared_ptr<Chunk>>(chunkPos, std::make_shared<Chunk>(chunkPos)));
		loadChunk(chunkPos, model, triangleArray, triangleMesh, concaveMesh);
	}
}

void glg::world::World::loadChunk(const chunkVec& chunkPos, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh)
{
	if (!isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		auto extent = loadChunkModel(chunkPos, model);

		std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkPos, model, triangleArray, triangleMesh, concaveMesh);
		chunk->modelExtent = extent;
		chunks.insert(std::pair<chunkVec, std::shared_ptr<Chunk>>(chunkPos, chunk));

		chunksMutex.unlock();
	}
}

int glg::world::World::loadChunkModel(const chunkVec& chunkPos, std::shared_ptr<Model> model) {
	glm::ivec2 modelPos(chunkPos.x, chunkPos.z);

	if (!chunkModels.contains(modelPos)) {
		Object object;
		Model* cModel = new Model();
		cModel->meshes.emplace_back(std::vector<Vertex>(), std::vector<unsigned int>(), std::vector<Texture2D> { *textures::defaultTexture }, Material(), true, false);
		object.addComponent<TransformComponent>(glm::vec3(chunkPos.x * CHUNK_SIZE.x, 0, chunkPos.z * CHUNK_SIZE.z));
		object.addComponent<ModelComponent>(cModel, shaders::defaultShader);
		object.addComponent<ChunkStripComponent>();
		object.addComponent<BoxCullComponent>(glm::vec3(CHUNK_SIZE.x / 2, CHUNK_LOAD_SIZE * CHUNK_SIZE.y, CHUNK_SIZE.z / 2), glm::vec3(CHUNK_SIZE.x / 2, 0, CHUNK_SIZE.z / 2));


		chunkModels.insert(std::pair<const glm::ivec2, Object>(modelPos, object));
	}
	auto [modelComponent, chunkStripComponent, boxCullComponent] = chunkModels[modelPos].get<ModelComponent, ChunkStripComponent, BoxCullComponent>();

	Model* cModel = modelComponent.model;
	Mesh& cMesh = cModel->meshes[0];
	Mesh& mesh = model->meshes[0];

	cMesh.vertices.reserve(cMesh.vertices.size() + mesh.vertices.size());
	//cMesh.indices.reserve(cMesh.indices.size() + mesh.indices.size());

	int oldSize = cMesh.vertices.size();

	for (int i = 0; i < mesh.vertices.size(); i++) {
		cMesh.vertices.emplace_back(mesh.vertices[i].position + glm::vec3(0, chunkPos.y * CHUNK_SIZE.y, 0), mesh.vertices[i].normal, mesh.vertices[i].texCoords);
	}

	cMesh.setupMesh();

	chunkStripComponent.chunks.push_back(chunkPos);


	updateBoxCull(chunkStripComponent, boxCullComponent);
	return mesh.vertices.size();
}

void glg::world::World::unloadChunk(const chunkVec& chunkPos)
{
	if (isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		unloadChunkModel(chunkPos);
		chunks.erase(chunkPos);
		chunksMutex.unlock();
	}
}

void glg::world::World::unloadChunkModel(const chunkVec& chunkPos) {
	glm::ivec2 modelPos(chunkPos.x, chunkPos.z);

	if (!chunkModels.contains(modelPos)) return;

	std::shared_ptr<Chunk> uChunk = chunks[chunkPos];

	auto [modelComponent, chunkStripComponent, boxCullComponent] = chunkModels[modelPos].get<ModelComponent, ChunkStripComponent, BoxCullComponent>();

	if (find(chunkStripComponent.chunks.begin(), chunkStripComponent.chunks.end(), chunkPos) == chunkStripComponent.chunks.end()) {
		return;
	}

	Model* model = modelComponent.model;
	Mesh& mesh = model->meshes[0];

	int index = chunkToIndex(chunkPos);

	//if (index > mesh.vertices.size() || index + uChunk->modelExtent > mesh.vertices.size()) {
	//	return;
	//}

	mesh.vertices.erase(mesh.vertices.begin() + index, mesh.vertices.begin() + index + uChunk->modelExtent);
	//chunkStripComponent.chunks.erase(std::remove(chunkStripComponent.chunks.begin(), chunkStripComponent.chunks.end(), chunkPos), chunkStripComponent.chunks.end());

	for (int i = 0; i < chunkStripComponent.chunks.size(); i++) {
		if (chunkPos == chunkStripComponent.chunks[i]) {
			chunkStripComponent.chunks.erase(chunkStripComponent.chunks.begin() + i);
			break;
		}
	}

	if (mesh.vertices.size() == 0) {
		chunkModels[modelPos].destory();
		delete model;
		chunkModels.erase(modelPos);
	}
	else {
		updateBoxCull(chunkStripComponent, boxCullComponent);
	}
	
	/*else {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

		glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(Vertex), uChunk->modelExtent * sizeof(Vertex), NULL);
	}*/
}

bool glg::world::World::isChunkLoaded(const chunkVec& chunkPos) const {
	chunksMutex.lock();
	bool contains = chunks.contains(chunkPos);
	chunksMutex.unlock();
	return contains;
}

chunkVec glg::world::World::getChunkPosition(glm::vec3 position)
{
	chunkVec chunkPos;

	if (position.x < 0) {
		chunkPos.x = int(std::floor(position.x / CHUNK_SIZE.x));
	}
	else {
		chunkPos.x = int(position.x / CHUNK_SIZE.x);
	}

	if (position.y < 0) {
		chunkPos.y = int(std::floor(position.y / CHUNK_SIZE.y));
	}
	else {
		chunkPos.y = int(position.y / CHUNK_SIZE.y);
	}

	if (position.z < 0) {
		chunkPos.z = int(std::floor(position.z / CHUNK_SIZE.z));
	}
	else {
		chunkPos.z = int(position.z / CHUNK_SIZE.z);
	}

	return chunkPos;
}

int glg::world::World::getChunkDistance(const chunkVec& chunkPos1, const chunkVec& chunkPos2)
{
	chunkVec pos = chunkPos1 - chunkPos2;
	return std::max(std::max(abs(pos.x), abs(pos.y)), abs(pos.z));
}

int glg::world::World::chunkToIndex(const chunkVec& chunkPos) const
{
	glm::ivec2 modelPos(chunkPos.x, chunkPos.z);

	auto it = chunkModels.find(modelPos);

	if (it != chunkModels.end()) {
		int index = 0;

		const Object& object = it->second;

		const auto& chunkStripComponent = object.get<ChunkStripComponent>();
		const auto& sChunks = chunkStripComponent.chunks;

		auto stripChunkIt = find(sChunks.begin(), sChunks.end(), chunkPos);

		//if (stripChunkIt == sChunks.end()) {
		//	return -1;
		//}

		int stripIndex = std::distance(sChunks.begin(), stripChunkIt);

		for (int i = 0; i < stripIndex; i++) {
			auto chunkIt = chunks.find(sChunks[i]);

			if (chunkIt != chunks.end()) {
				std::shared_ptr<Chunk> chunk = chunkIt->second;
				index += chunk->modelExtent;
			}
		}

		return index;
	}

	return -1;
}

void glg::world::World::updateBoxCull(const ChunkStripComponent& stripChunkComp, BoxCullComponent& boxCullComp) {
	int minY = std::numeric_limits<int>::max();
	int maxY = std::numeric_limits<int>::min();

	const auto& sChunks = stripChunkComp.chunks;

	for (int i = 0; i < sChunks.size(); i++) {
		const chunkVec& chunkPos = sChunks[i];

		if (chunkPos.y < minY) {
			minY = chunkPos.y;
		}

		if (chunkPos.y > maxY) {
			maxY = chunkPos.y;
		}
	}

	if (minY == std::numeric_limits<int>::max()) {
		minY = 0;
		maxY = 0;
	}

	float worldMinY = minY * CHUNK_SIZE.y;
	float worldMaxY = maxY * CHUNK_SIZE.y;

	boxCullComp.offset.y = (worldMinY + worldMaxY) / 2;
	boxCullComp.size.y = abs(worldMaxY - worldMinY) / 2;
}

bool glg::world::DistanceCompare(const chunkVec& vec1, const chunkVec& vec2) {
	return glg::world::World::getChunkDistance(chunkVec(0), vec1) < glg::world::World::getChunkDistance(chunkVec(0), vec2);
}

float glg::world::NoiseSettings::getNoise(float x, float y, float z, float frequency, int seed)
{
	return noise->GenSingle3D(x * frequency, y * frequency, z * frequency, seed);
}

std::vector<float> glg::world::NoiseSettings::GenUniformNoise3D(glm::ivec3 start, glm::ivec3 size, float frequency, int seed)
{
	std::vector<float> tempOutput(size.x * size.y * size.z);
	std::vector<float> output;
	output.reserve(size.x * size.y * size.z);

	noise->GenUniformGrid3D(tempOutput.data(), start.x, start.y, start.z, size.x, size.y, size.z, frequency, seed);

	for (int i = 0, z = 0; z < size.x; z++) {
		for (int y = 0; y < size.y; y++) {
			for (int x = 0; x < size.z; x++, i++) {
				output.push_back(tempOutput[i]);
			}
		}
	}

	return output;
}
