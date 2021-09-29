#pragma once
#include "world_chunk.h"
#include <map>
#include <glm/glm.hpp>
#include <mutex>
#include "../globals/macros.h"
#include <boost/container_hash/hash.hpp>

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

			std::unordered_map<chunkVec, std::shared_ptr<Chunk>, ChunkPositionComparator> chunks;

			World();

			void loadChunk(chunkVec chunkPos);

			void loadChunk(chunkVec chunkPos, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh);

			void unloadChunk(const chunkVec& chunkPos);

			bool isChunkLoaded(const chunkVec& chunkPos) const;

			static chunkVec getChunkPosition(glm::vec3 position);

			static int getChunkDistance(const chunkVec& chunkPos1, const chunkVec& chunkPos2);

		protected:
			mutable std::mutex chunksMutex;

		};
	}
}