#include "TextureManager.hpp"
#include <iostream>

TextureManager::TextureManager() : texturePrefix("textures/") {

}

TextureManager::~TextureManager() {

}

Texture *TextureManager::loadTexture(const char *texName, bool mipmap) {

	std::pair<std::map<std::string, Texture*>::iterator, bool> ins =
		texturesMap.insert(std::make_pair(texName, static_cast<Texture*>(NULL)));

	Texture *tex;

	// On a déja chargé cette texture auparavant
	if (ins.second) {
		// just created
		tex = new Texture();
		ins.first->second = tex;
		// load image
		Image img;
		img.load(texName);
		tex->load(img, mipmap);
		std::clog << "Creating texture " << texName << "\n";
	}
	else {
		// already loaded
		tex = ins.first->second;
		//std::clog << "Returning loaded texture " << texName << "(" << tex->getId() << ")\n";
	}

	return tex;
}
