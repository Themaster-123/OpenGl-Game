#include "components.h"
#include "../scene.h"

void glg::addComponentDependencies()
{
	Object::addDependency<ModelComponent, TransformComponent>();
}