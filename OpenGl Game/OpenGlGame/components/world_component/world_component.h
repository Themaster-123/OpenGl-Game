#pragma once
#include "../../globals/macros.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <boost/container_hash/hash.hpp>

namespace glg {
	class Object;

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

	struct Chunk {
		Chunk(chunkVec position, int modelExtent);

		chunkVec position;
		int modelExtent;
	};

	struct WorldComponent {
		std::unordered_map<chunkVec, std::shared_ptr<Chunk>, ChunkPositionComparator> chunks;

		std::unordered_map<glm::ivec2, Object, ChunkPositionComparator2D> chunkModels;

		WorldComponent();

		WorldComponent& operator=(const WorldComponent& other);
	};
}
