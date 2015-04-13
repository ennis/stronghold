#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image {
public:
  Image ();
  Image (int width, int height, int num = 1);
  void load(std::string filename);

  int getWidth () const;
  int getHeight () const;
  int getNumComponents() const;

  unsigned char* getPixels () const;
  unsigned char operator() (int x, int y, int n_ = 0) const;
  void setPixel (unsigned char p, int x, int y, int n_ = 0);
  void free ();

  float sample(float x, float y, int n_ = 0) const;

private:
  std::string filename;
  unsigned char* img;
  int width;
  int height;
  int n;
};

Image smoothing5x5(Image& img);
Image smoothing3x3(Image& img);

#endif

