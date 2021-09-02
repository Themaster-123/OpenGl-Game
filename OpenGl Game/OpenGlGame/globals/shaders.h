#pragma once
#include "../resources/shader.h"

namespace glg {
	namespace shaders {
		extern Shader* defaultShader;

		void registerShaders();

		std::vector<glg::Shader*>& getShaders();

		Shader* addShader(Shader* shader);
	}
}
