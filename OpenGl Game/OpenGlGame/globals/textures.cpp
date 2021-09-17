#include "textures.h"

glg::Texture2D* glg::textures::defaultTexture;

void glg::textures::registerTextures()
{
	defaultTexture = new Texture2D("assets/textures/crate.jpg", 0);
}
