#pragma once
#include "world_chunk.h"
#include <map>
#include <glm/glm.hpp>

namespace glg {
	namespace world {
		struct NoiseSettings {
			FastNoiseLite noise;

			float displacementHeight;

			NoiseSettings() = default;

			NoiseSettings(FastNoiseLite& noise, float displacementHeight);
		};

		class World {
		public:
			struct ChunkPositionComparator {
				size_t operator()(const glm::ivec2& vec) const {
					size_t lhs = std::hash<int>()(vec.x);
					return lhs ^ (std::hash<int>()(vec.y) + 0x9e3779b9 + (lhs << 6) + (lhs >> 2));
				}

				bool operator()(const glm::ivec2& vecA, const glm::ivec2& vecB) const {
					return (vecA.x * vecA.y) < (vecB.x * vecB.y);
				}

			};

			World();

		protected:
			std::map<glm::ivec2, Chunk*, ChunkPositionComparator> chunks;
		};

		extern NoiseSettings NOISE_SETTINGS;

		extern float CHUNK_SIZE;

		void setNoiseSetting();
	}
}