#pragma once
#include <glm/glm.hpp>
#include "../../globals/macros.h"
#include "../component_system.h"
#include <thread>

namespace glg {
	class ChunkLoaderSystem : public ComponentSystem
	{
	public:


		ChunkLoaderSystem();

		~ChunkLoaderSystem();

		virtual void update();


	};
}

