#pragma once
#include <glm/glm.hpp>

namespace glg {
	struct BoxCullComponent {
		glm::vec3 size;
		glm::vec3 offset;

		BoxCullComponent(glm::vec3 size = glm::vec3(1), glm::vec3 offset = glm::vec3(0));

		virtual BoxCullComponent& operator=(const BoxCullComponent& other);
	};
}
