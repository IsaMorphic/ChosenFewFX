#pragma once
#include "ofxsImageEffect.h"

class FloatImage
{
private:
	int _width;
	int _height;
	OFX::ImageMemory *_handle;
	float *_imgPtr;
public:
	void SetPixel(int x, int y, float *pix);
	void SetPixel(int x, int y, float r, float g, float b, float a);
	void SetPixel(int x, int y, float v);
	float* GetPixel(int x, int y);
	FloatImage(int width, int height);
	~FloatImage();
};

