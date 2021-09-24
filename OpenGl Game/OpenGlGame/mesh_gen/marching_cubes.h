#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "../resources/model.h"

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
		std::vector<Voxel> voxels;
		glm::ivec3 resolution;
		glm::vec3 voxelSize;

		MarchingCubes(const std::vector<Voxel>& voxels, const glm::ivec3& resolution, const glm::vec3& voxelSize);

		std::shared_ptr<Model> createModel(float isoLevel, std::vector<Texture2D> textures) const;

		void triangulateCell(const Cell& cell, std::vector<glg::Vertex>& vertices, std::vector<unsigned int>& indices, float isoLevel) const;

		glm::vec3 interpolateVertexPosition(const Voxel& voxel1, const Voxel& voxel2, float isoLevel) const;

		Voxel& getVoxel(const glm::ivec3& pos);

		Voxel& getVoxel(int index, const glm::ivec3& offset);

		const Voxel& getVoxel(const glm::ivec3& pos) const;

		const Voxel& getVoxel(int index, const glm::ivec3 offset) const;
	};
}