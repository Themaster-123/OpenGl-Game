#pragma once
#include "world_chunk.h"
#include <map>
#include <glm/glm.hpp>
#include <mutex>
#include "../globals/macros.h"
#include <boost/container_hash/hash.hpp>
#include <fastnoise/FastNoise.h>
#include <fastnoise/SmartNode.h>
#include "../components/components.h"

namespace glg {
	namespace world {

		extern unsigned int CHUNK_LOAD_SIZE;

		extern glm::ivec3 CHUNK_RESOLUTION;

		extern glm::vec3 CHUNK_SIZE;

		void setNoiseSetting();

		struct NoiseSettings {
			FastNoise::SmartNode<> noise;

			float displacementHeight;

			NoiseSettings() = default;

			NoiseSettings(FastNoise::SmartNode<>& noise, float displacementHeight);

			float getNoise(float x, float y, float z, float frequency, int seed);

			std::vector<float> GenUniformNoise3D(glm::ivec3 start, glm::ivec3 size, float frequency, int seed);
		};

		extern NoiseSettings NOISE_SETTINGS;

		bool DistanceCompare(const chunkVec& vec1, const chunkVec& vec2);

		class World {
		public:
			struct ChunkPositionComparator {
				size_t operator()(const chunkVec& vec) const {
					size_t hash = 0;
					boost::hash_combine(hash, vec.x);
					boost::hash_combine(hash, vec.y);
					boost::hash_combine(hash, vec.z);
					return hash;
				}
			};

			struct ChunkPositionComparator2D {
				size_t operator()(const glm::ivec2& vec) const {
					size_t hash = 0;
					boost::hash_combine(hash, vec.x);
					boost::hash_combine(hash, vec.y);
					return hash;
				}
			};

			std::unordered_map<chunkVec, std::shared_ptr<Chunk>, ChunkPositionComparator> chunks;

			std::unordered_map<glm::ivec2, Object, ChunkPositionComparator2D> chunkModels;

			World();

			void loadChunk(const chunkVec& chunkPos);

			void loadChunk(const chunkVec& chunkPos, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh);

			int loadChunkModel(const chunkVec& chunkPos, std::shared_ptr<Model> model);

			void unloadChunk(const chunkVec& chunkPos);

			void unloadChunkModel(const chunkVec& chunkPos);

			bool isChunkLoaded(const chunkVec& chunkPos) const;

			int chunkToIndex(const chunkVec& chunkPos) const;

			void updateBoxCull(const ChunkStripComponent& stripChunkComp, BoxCullComponent& boxCullComp);

			static chunkVec getChunkPosition(glm::vec3 position);

			static int getChunkDistance(const chunkVec& chunkPos1, const chunkVec& chunkPos2);

		protected:
			mutable std::mutex chunksMutex;

		};
	}
}