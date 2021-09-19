#pragma once
#include "../component_system.h"
#include <thread>

namespace glg {
	class ChunkLoaderSystem : public ComponentSystem
	{
	public:
		std::thread loadThread;

		ChunkLoaderSystem();

		~ChunkLoaderSystem();

		virtual void update();

		static void chunkLoadLoop();
	};
}

