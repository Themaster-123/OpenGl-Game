#pragma once
#include "shaders.h"
#include <iostream>

using namespace glg;

Shader* shaders::defaultShader;
Shader* shaders::marchingCubesShader;

void glg::shaders::registerShaders()
{
	shaders::defaultShader = addShader(new Shader("assets/shaders/default/VertexShader.vert", "assets/shaders/default/FragmentShader.frag"));
	shaders::marchingCubesShader = addShader(new Shader("assets/shaders/MarchingCubes.comp"));
	//std::cout << "defauda" << shaders::defaultShader.ID << std::endl;
}

std::vector<glg::Shader*>& shaders::getShaders()
{
	static std::vector<glg::Shader*> vector;
	return vector;
}

Shader* shaders::addShader(Shader* shader) {
	getShaders().push_back(shader);
	return shader;
}

