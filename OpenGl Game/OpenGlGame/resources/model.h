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

		void loadModel(std::string path) {
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				std::cout << "Could not load model " << importer.GetErrorString() << std::endl;
				return;
			}

			directory = path.substr(0, path.find_last_of('/'));

			processNode(scene->mRootNode, scene);
		}

		void processNode(aiNode* node, const aiScene* scene) {
			// processes all the node's meshes
			for (size_t i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}

			// processes all the nodes children
			for (size_t i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}
