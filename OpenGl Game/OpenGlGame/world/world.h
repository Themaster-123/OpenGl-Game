#pragma once
#include "world_chunk.h"
#include <map>
#include <glm/glm.hpp>
#include <mutex>

namespace glg {
	namespace world {

		extern unsigned int CHUNK_LOAD_SIZE;

		extern size_t CHUNK_RESOLUTION;

		extern float CHUNK_SIZE;

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
				size_t operator()(const glm::ivec2& vec) const {
					size_t lhs = std::hash<int>()(vec.x);
					return lhs ^ (std::hash<int>()(vec.y) + 0x9e3779b9 + (lhs << 6) + (lhs >> 2));
				}

			};

			std::unordered_map<glm::ivec2, Chunk*, ChunkPositionComparator> chunks;

			World();

			void loadChunk(glm::ivec2 chunkPos);

			void loadChunk(glm::ivec2 chunkPos, Model* model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh);

			void unloadChunk(const glm::ivec2& chunkPos);

			bool isChunkLoaded(const glm::ivec2& chunkPos) const;

			static glm::ivec2 getChunkPosition(glm::vec3 position);

		protected:

		};
	}
}