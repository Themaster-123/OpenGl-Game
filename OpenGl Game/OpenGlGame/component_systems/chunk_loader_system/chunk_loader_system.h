#pragma once
#include "../../globals/macros.h"
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

		static std::vector<chunkVec> getClosestChunks(uint32_t chunkSize);
	};
}

