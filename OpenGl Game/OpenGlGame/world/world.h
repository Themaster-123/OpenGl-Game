#pragma once
#include "world_chunk.h"
#include <map>
#include <glm/glm.hpp>
#include <mutex>
#include "../globals/macros.h"

namespace glg {
	namespace world {

		extern unsigned int CHUNK_LOAD_SIZE;

		extern glm::ivec3 CHUNK_RESOLUTION;

		extern glm::vec3 CHUNK_SIZE;

		void setNoiseSetting();

		struct NoiseSettings {
			FastNoiseLite noise;

			float displacementHeight;

			NoiseSettings() = default;

			NoiseSettings(FastNoiseLite& noise, float displacementHeight);
		};

		extern NoiseSettings NOISE_SETTINGS;

		class World {
		public:
			struct ChunkPositionComparator {
				size_t operator()(const chunkVec& vec) const {
					size_t lhs = std::hash<int>()(vec.x);
					return lhs ^ (std::hash<int>()(vec.y) + 0x9e3779b9 + (lhs << 6) + (lhs >> 2));
				}

			};

			std::unordered_map<chunkVec, std::shared_ptr<Chunk>, ChunkPositionComparator> chunks;

			World();

			void loadChunk(chunkVec chunkPos);

			void loadChunk(chunkVec chunkPos, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh);

			void unloadChunk(const chunkVec& chunkPos);

			bool isChunkLoaded(const chunkVec& chunkPos) const;

			static chunkVec getChunkPosition(glm::vec3 position);

		protected:
			mutable std::mutex chunksMutex;

		};
	}
}