#pragma once
#include "../resources/mesh.h"

namespace glg {
	namespace world {
		class Chunk {
		public:
			Chunk();

		protected:
			Mesh* mesh;
		};
	}
}
