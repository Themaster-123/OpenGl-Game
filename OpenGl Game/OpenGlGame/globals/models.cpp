#include "models.h"

using namespace glg;

Model models::defaultModel;

void glg::models::registerModels()
{
	models::defaultModel = Model("assets/models/test/test.obj");
}
