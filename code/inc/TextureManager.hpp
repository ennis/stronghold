#ifndef _TEXTURE_MANAGER_
#define _TEXTURE_MANAGER_

#include <map>
#include <string>
#include "Texture.hpp"


class TextureManager
{
public:
  TextureManager();
  ~TextureManager();

  /*
   * Cree une texture à partir d'un image, et la renvoie.
   * Si la texture a deja été créée, on la renvoie juste.
   */
  Texture *loadTexture(const char *texName, bool mipmap = true);


  static TextureManager& Instance() {
    static  TextureManager singleton;
    return singleton;
  }

private:
  std::map<std::string, Texture*> texturesMap;
  std::string texturePrefix;

};

#endif
