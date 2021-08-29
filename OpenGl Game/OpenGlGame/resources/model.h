#pragma once
#include <vector>
#include "shader.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace glg {
	class Model {
	public:
		Model(const char* path) {
			loadModel(path);
		}

		void draw(Shader& shader) {
			for (size_t i = 0; i < meshes.size(); i++) {
				meshes[i].draw(shader);
			}
		}

	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(const char* path);

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}
