#include "Texture.hpp"

#include <iostream>

Texture::Texture() : id(0), mipmap(false)
{}

void Texture::load(Image &img, bool mipmap_) {
  name = "";
  mipmap = mipmap_;

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
	
  GLenum internalFormat;
  GLenum format;
  switch (img.getNumComponents()) {
  case 1:
    internalFormat = GL_RED;
    format = GL_RED;
    break;
  case 2:
    internalFormat = GL_RG;
    format = GL_RG;
    break;
  case 3:
    internalFormat = GL_RGB;
    format = GL_RGB;
    break;
  default:
    internalFormat = GL_RGBA;
    format = GL_RGBA;
    break;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img.getWidth(), img.getHeight(), 0, format, GL_UNSIGNED_BYTE, img.getPixels());

  if (mipmap) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  // default parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

}

void Texture::setClamping (GLenum clamping) const {
    bind();
    // Texture coordinates : (s, t, r)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamping);
}

void Texture::setFiltering (GLenum filter) const {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::setActive(int texUnit) const {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

GLuint Texture::getId() const {
	return id;
}

Texture::~Texture () {
    glDeleteTextures(1, &id);
}

