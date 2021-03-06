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
		bool setupMeshV;
		bool useIndices;
		unsigned int VAO = 0, VBO, EBO;
		uint32_t meshSize;

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture2D>& textures, const Material material = Material(), bool setupMesh = true, bool useIndices = true) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			this->material = material;
			this->setupMeshV = setupMesh;
			this->useIndices = useIndices;

			if (setupMeshV) {
				this->setupMesh();
			}
		}

		Mesh(const Mesh& mesh) {
			vertices = mesh.vertices;
			indices = mesh.indices;
			textures = mesh.textures;
			material = mesh.material;
			setupMeshV = mesh.setupMeshV;
			useIndices = mesh.useIndices;

			if (setupMeshV) {
				setupMesh();
			}
		}

		~Mesh() {
			if (vertices.size() == 0) return;

			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		void draw(Shader& shader) const {
			if (vertices.size() == 0) return;

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
			
			shader.use();
			glBindVertexArray(VAO);
			if (useIndices) glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
			else glDrawArrays(GL_TRIANGLES, 0, (int)meshSize);
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
			if (vertices.size() == 0) return;

			if (VAO == 0) {
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);
			}

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
			meshSize = vertices.size();
			if (indices.size() != 0) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
			}

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
	};
}
