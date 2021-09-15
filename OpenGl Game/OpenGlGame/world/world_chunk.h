#pragma once
#include "../resources/mesh.h"
#include "../resources/model.h"
#include "../components/components.h"

namespace glg {
	namespace world {
		class Chunk {
		public:
			glm::ivec2 position;

			Chunk(glm::ivec2 position);

		protected:

			Object& createObject();

			Model* generateModel();
		};
	}
}
