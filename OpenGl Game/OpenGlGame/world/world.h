#pragma once
#include "world_chunk.h"
#include <map>
#include <glm/glm.hpp>

namespace glg {
	namespace world {
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

			static float CHUNK_SIZE;

			World();

		protected:
			std::map<glm::ivec2, Chunk*, ChunkPositionComparator> chunks;
		};
	}
}