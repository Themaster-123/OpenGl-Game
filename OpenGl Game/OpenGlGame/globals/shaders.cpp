#pragma once
#include "shaders.h"
#include <iostream>

using namespace glg;

Shader shaders::defaultShader;

void glg::shaders::registerShaders()
{
	shaders::defaultShader = Shader("assets/shaders/default/VertexShader.vert", "assets/shaders/default/FragmentShader.frag");
}
