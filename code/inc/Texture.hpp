#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

#include "Image.hpp"

class Texture {
public:
  Texture();
  void load(Image &img, bool mipmap_ = true);
  void bind() const;
  void setClamping (GLenum clamping) const;
  void setFiltering (GLenum filter) const;
  void setActive(int texUnit) const;
  GLuint getId() const;

  ~Texture();

private:
  GLuint id;
  std::string name;
  bool mipmap;
};

#endif

