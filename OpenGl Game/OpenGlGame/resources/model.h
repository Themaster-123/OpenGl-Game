#pragma once
#include <vector>
#include "shader.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

namespace glg {
	class Model {
	public:

		Model() {

		}

		Model(const char* path) {
			loadModel(path);
		}

		void draw(Shader& shader) const {
			for (size_t i = 0; i < meshes.size(); i++) {
				meshes[i].draw(shader);
			}
		}

		std::vector<Mesh> getMeshes() const {
			return meshes;
		}

	private:
		std::map<std::string, Texture2D> loadedTextures;
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(std::string path) {
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

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

		Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<Texture2D> textures;

			for (size_t i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;

				vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				
				vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

				if (mesh->mTextureCoords[0]) {
					vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}
				vertices.push_back(vertex);
			}

			for (size_t i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (size_t j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				std::vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}

		std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
			std::vector<Texture2D> textures;
			for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
				aiString str;
				mat->GetTexture(type, (unsigned int) i, &str);
				if (loadedTextures.find(str.C_Str()) != loadedTextures.end()) {
					textures.push_back(loadedTextures[str.C_Str()]);
				}
				else if (loadedTextures.find(str.C_Str()) == loadedTextures.end()){
					std::string fullPath = std::string((directory + '/' + std::string(str.C_Str())).c_str());
					Texture2D texture(fullPath.c_str(), 0);
					textures.push_back(texture);
					loadedTextures[str.C_Str()] = texture;
				}
			}
			return textures;
		}
	};
}
