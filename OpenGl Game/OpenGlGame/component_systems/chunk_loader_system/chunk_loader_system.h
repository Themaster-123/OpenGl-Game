#pragma once
#include "../component_system.h"

namespace glg {
	class ChunkLoaderSystem : public ComponentSystem
	{
	public:
		ChunkLoaderSystem();

		virtual void update();
	};
}

