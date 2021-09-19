#pragma once
#include "../resources/mesh.h"
#include "../resources/model.h"
#include "../essential/object.h"
#include <reactphysics3d/reactphysics3d.h>

namespace glg {
	namespace world {
		class Chunk {
		public:
			glm::ivec2 position;

			Chunk(glm::ivec2 position);

			Chunk(glm::ivec2 position, Model* model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh);

			~Chunk();

			static Model* generateModel(glm::ivec2 position);

		protected:
			Model* model;
			rp3d::TriangleVertexArray* triangleArray;
			rp3d::TriangleMesh* triangleMesh;
			rp3d::ConcaveMeshShape* concaveMesh;

			Object object;

			Object& createObject();
		};
	}
}
