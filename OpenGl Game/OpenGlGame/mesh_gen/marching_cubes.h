#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "../resources/model.h"
#include <boost/multi_array.hpp>

namespace glg {
	struct Voxel {
		float value;
		glm::vec3 position;
	};

	struct Cell {
		Voxel voxels[8];

		Cell(std::initializer_list<Voxel> voxels);

		Voxel& operator[] (size_t index);

		const Voxel& operator[] (size_t index) const;
	};

	class MarchingCubes {
	public:
		boost::multi_array<Voxel, 3> voxels;
		glm::vec3 voxelSize;

		MarchingCubes(const boost::multi_array<Voxel, 3>& voxels, const glm::vec3& voxelSize);

		std::shared_ptr<Model> createModel(float isoLevel, std::vector<Texture2D> textures) const;

		void triangulateCell(const Cell& cell, std::vector<glg::Vertex>& vertices, std::vector<unsigned int>& indices, float isoLevel) const;

		glm::vec3 interpolateVertexPosition(const Voxel& voxel1, const Voxel& voxel2, float isoLevel) const;

		Voxel& getVoxel(const glm::ivec3& pos);

		const Voxel& getVoxel(const glm::ivec3& pos) const;
	};
}