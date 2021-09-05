#pragma once
#include "world_chunk.h"
#include <map>
#include <glm/glm.hpp>

namespace glg {
	namespace world {
		class World {
		public:
			World();

			void draw();

		protected:
			std::map <glm::vec2, Chunk&> chunks;
		};
	}
}