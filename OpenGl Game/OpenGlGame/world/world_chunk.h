#pragma once
#include "../resources/mesh.h"
#include "../resources/model.h"
#include "../essential/object.h"

namespace glg {
	namespace world {
		class Chunk {
		public:
			glm::ivec2 position;

			Chunk(glm::ivec2 position);

			~Chunk();

		protected:
			Object object;

			Object& createObject();

			Model* generateModel(int quality);
		};
	}
}
