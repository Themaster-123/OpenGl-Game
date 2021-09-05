#include "world_chunk.h"

glg::world::Chunk::Chunk()
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0)));
	vertices.push_back(Vertex(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, 1)));
	vertices.push_back(Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec2(1, 1)));
	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);
	std::vector<Texture2D> textures;
	Mesh mesh(vertices, indices, textures);
	this->mesh = &mesh;
}
