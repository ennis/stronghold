#include "Image.hpp"
#include "stb_image.hpp"

#include <iostream>
#include <cstdlib>

Image::Image() : img(0), width(0), height(0)
{}

Image::Image (int width, int height, int num) :img(0),  width(width), height(height), n(num) {
  img = new unsigned char[num * width * height];
  for (int k = 0; k < num * width * height; k++) {
    img[k] = (unsigned char) 0;
  }
}

void Image::load(std::string filename_) {
  filename.assign(filename_);
  std::cout << "Loading " << filename_ << " ...";
  img = stbi_load(filename_.c_str(), &width, &height, &n, 0);
  if (img == NULL) {
    std::cerr << std::endl << "Failed to load the image " << filename_ << std::endl;
    assert(false);
  }
  std::cout << " loaded!" << std::endl;
  std::cout << "Dimensions : " << width << "x" << height << std::endl;
}

int Image::getWidth () const {
  return width;
}

int Image::getHeight () const {
  return height;
}

int Image::getNumComponents() const {
  return n;
}

unsigned char* Image::getPixels () const {
  return img;
}

unsigned char Image::operator() (int x, int y, int n_) const {
	assert(0 <= x && x < width && 0 <= y && y < height);
    return img[n_ * width * height + x + y * width];
}

void Image::setPixel (unsigned char p, int x, int y, int n_) {
    img[n_ * width * height + x + y * width] = p;
}

float Image::sample(float x, float y, int n_) const {
	int ix = int(x);
	int iy = int(y);

	if (ix+1 >= width) ix = width - 2;
	if (iy+1 >= height) iy = height - 2;

	assert(0 <= ix && ix < width && 0 <= iy && iy < height);
	
	float v1 = (*this)(ix, iy, n_) / 255.f;
	float v2 = (*this)(ix + 1, iy, n_) / 255.f;
	float v3 = (*this)(ix, iy + 1, n_) / 255.f;
	float v4 = (*this)(ix + 1, iy + 1, n_) / 255.f;

	// interpolation bilinéaire
	return (ix + 1 - x) * (iy + 1 - y) * v1
		+ (x - ix) * (iy + 1 - y) * v2
		+ (ix + 1 - x) * (y - iy) * v3
		+ (x - ix) * (y - iy) * v4;	
}


Image smoothing5x5(Image& img){
	int width = img.getWidth();
	int height = img.getHeight();

	int Masque[5][5];
	Masque[0][0] = 1;
	Masque[0][1] = 4;
	Masque[0][2] = 7;
	Masque[0][3] = 4;
	Masque[0][4] = 1;

	Masque[1][0] = 4;
	Masque[1][1] = 16;
	Masque[1][2] = 26;
	Masque[1][3] = 16;
	Masque[1][4] = 4;

	Masque[2][0] = 7;
	Masque[2][1] = 26;
	Masque[2][2] = 41;
	Masque[2][3] = 26;
	Masque[2][4] = 7;

	Masque[3][0] = 4;
	Masque[3][1] = 16;
	Masque[3][2] = 26;
	Masque[3][3] = 16;
	Masque[3][4] = 4;

	Masque[4][0] = 1;
	Masque[4][1] = 4;
	Masque[4][2] = 7;
	Masque[4][3] = 4;
	Masque[4][4] = 1;

	int coeffMasque = 273;

	Image output(width, height, 1);
	int sum = 0;
	for (int x = 2; x < width - 2; x++){
		for (int y = 2; y < height - 2; y++) {

			for (int i = -2; i <= 2; i++) {
				for (int j = -2; j <= 2; j++){
					sum = sum + (img(x + i, y + j) * Masque[i + 2][j + 2] / coeffMasque);
				}
			}
			// On contraint sum à être dans [0;255]
			if (sum < 0) { sum = 0; }
			if (sum > 255) { sum = 255; }

			output.setPixel(sum, x,y,0);
			sum = 0;
		}
	}

	return output;
}

Image smoothing3x3(Image& img){
	int width = img.getWidth();
	int height = img.getHeight();

	int Masque[3][3]; // 3x3  Mask
	 
	Masque[0][0] = 1;
	Masque[0][1] = 3;
	Masque[0][2] = 1;

	Masque[1][0] = 2;
	Masque[1][1] = 4;
	Masque[1][2] = 2;

	Masque[2][0] = 1;
	Masque[2][1] = 3;
	Masque[2][2] = 1;

	int coeffMasque = 16;

	Image output(width, height, 1);
	int sum = 0;
	for (int x = 2; x < width - 2; x++){
		for (int y = 2; y < height - 2; y++) {

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++){
					sum = sum + (img(x + i, y + j) * Masque[i + 1][j + 1] / coeffMasque);
				}
			}
			// On contraint sum à être dans [0;255]
			if (sum < 0) { sum = 0; }
			if (sum > 255) { sum = 255; }

			output.setPixel(sum, x, y, 0);
			sum = 0;
		}
	}

	return output;
}

void Image::free () {
  stbi_image_free(img);
}

