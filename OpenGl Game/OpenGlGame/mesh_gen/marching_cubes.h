#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "../resources/model.h"
#include <boost/multi_array.hpp>

namespace glg {
	struct Voxel {
		glm::vec4 position;
		float value;
		//float padding[3];

		Voxel() = default;

		Voxel(glm::vec3 position, float value);

		bool operator< (const Voxel& other) const;
	};

	struct Cell {
		Voxel voxels[8];

		Cell(std::initializer_list<Voxel> voxels);

		Voxel& operator[] (size_t index);

		const Voxel& operator[] (size_t index) const;
	};

	class MarchingCubes {
	public:
		static unsigned int indexTableBufferObject;
		static unsigned int ib;
		static unsigned int gb;
		static unsigned int ac;
		static unsigned int MAX_TRIANGLES;

		std::vector<Voxel> voxels;
		glm::ivec3 resolution;

		MarchingCubes(glm::ivec3 resolution);

		std::shared_ptr<Model> createModel(float isoLevel, std::vector<Texture2D> textures) const;

		static void triangulateCell(const Cell& cell, std::vector<glg::Vertex>& vertices, std::vector<unsigned int>& indices, float isoLevel);

		static glm::vec3 interpolateVertexPosition(const Voxel& voxel1, const Voxel& voxel2, float isoLevel);

		Voxel& getVoxel(const glm::ivec3& pos);

		const Voxel& getVoxel(const glm::ivec3& pos) const;
	};
}