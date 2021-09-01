#include "models.h"

using namespace glg;

Model models::defaultModel;
Model models::terrainModel;
Model models::sphereModel;

void models::registerModels()
{
	models::defaultModel = Model("assets/models/test/test.obj");
	models::terrainModel = Model("assets/models/terrain/terrain.obj");
	models::sphereModel = Model("assets/models/sphere/sphere.obj");
}
