#pragma once
#include "../../globals/macros.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <boost/container_hash/hash.hpp>
#include <mutex>
#include <FastNoise/FastNoise.h>

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

	struct NoiseSettings {
		FastNoise::SmartNode<> noise;

		NoiseSettings() = default;

		NoiseSettings(FastNoise::SmartNode<>& noise);

		float getNoise(float x, float y, float z, float frequency, int seed);

		std::vector<float> GenUniformNoise3D(glm::ivec3 start, glm::ivec3 size, float frequency, int seed);
	};

	struct Chunk {
		Chunk(chunkVec position, int modelExtent);

		chunkVec position;
		int modelExtent;
	};

	struct WorldComponent {
		std::unordered_map<chunkVec, std::shared_ptr<Chunk>, ChunkPositionComparator> chunks;

		std::unordered_map<glm::ivec2, Object, ChunkPositionComparator2D> chunkModels;

		NoiseSettings noiseSettings;

		mutable std::mutex chunksMutex;

		WorldComponent();

		WorldComponent(NoiseSettings noiseSettings);

		WorldComponent& operator=(const WorldComponent& other);
	};
}
