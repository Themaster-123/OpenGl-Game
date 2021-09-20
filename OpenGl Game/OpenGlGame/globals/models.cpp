#include "models.h"

using namespace glg;

Model* models::defaultModel;
Model* models::terrainModel;
Model* models::sphereModel;

void models::registerModels()
{
	models::defaultModel = new Model("assets/models/test/test.obj");
	models::terrainModel = new Model("assets/models/terrain/terrain.obj");
	models::sphereModel = new Model("assets/models/sphere/sphere.obj");
}
