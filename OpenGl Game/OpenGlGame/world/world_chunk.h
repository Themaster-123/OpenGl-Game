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

			Chunk(glm::ivec2 position, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh);

			~Chunk();

			static std::shared_ptr<Model> generateModel(glm::ivec2 position);

			static std::tuple<rp3d::TriangleVertexArray*, rp3d::TriangleMesh*, rp3d::ConcaveMeshShape*> generateConcaveMeshShape(std::shared_ptr<const Model> model);

		protected:
			std::shared_ptr<Model> model;
			rp3d::TriangleVertexArray* triangleArray;
			rp3d::TriangleMesh* triangleMesh;
			rp3d::ConcaveMeshShape* concaveMesh;

			Object object;

			Object createObject();
		};
	}
}
