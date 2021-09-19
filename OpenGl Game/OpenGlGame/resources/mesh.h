#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "texture.h"
#include "shader.h"
#include "material.h"

namespace glg {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;

		Vertex() = default;

		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) {
			this->position = position;
			this->normal = normal;
			this->texCoords = texCoords;
		}
	};

	class Mesh {
	public:
		std::vector<Vertex>	vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture2D> textures;
		Material material;

		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture2D>& textures, Material material = Material(), bool setupMesh = true) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			this->material = material;

			if (setupMesh) {
				this->setupMesh();
			}
		}

		void draw(Shader& shader) const {
			unsigned int diffuseNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++) {
				std::string number;
				std::string name = "diffuse";
				if (name == "diffuse") {
					number = std::to_string(diffuseNr);
					diffuseNr++;
				}

				shader.setInt("texture_" + name + number, i);
				textures[i].activate(i);
			}
			shader.setMaterial("material", material);
			
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, (int) indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);

		}

		void calculateNormals() {
			for (size_t i = 0; i < indices.size(); i += 3) {
				size_t index1, index2, index3;
				index1 = indices[i];
				index2 = indices[i + 1];
				index3 = indices[i + 2];

				glm::vec3 u = vertices[index2].position - vertices[index1].position;
				glm::vec3 v = vertices[index3].position - vertices[index1].position;
				
				glm::vec3 normal = glm::normalize(glm::cross(u, v));
				vertices[index1].normal = normal;
				vertices[index2].normal = normal;
				vertices[index3].normal = normal;
			}
		}

		void setupMesh() {
			if (VAO == 0) {
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);
			}

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

			glBindVertexArray(0);
		}

	private:
		unsigned int VAO = 0, VBO, EBO;
	};
}
