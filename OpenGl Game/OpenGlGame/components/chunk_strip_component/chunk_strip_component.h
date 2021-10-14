#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../globals/macros.h"

namespace glg {
	struct ChunkStripComponent {
		std::vector<chunkVec> chunks;

		ChunkStripComponent();

		virtual ChunkStripComponent& operator=(const ChunkStripComponent& other);
	};
}
