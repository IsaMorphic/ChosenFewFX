#include "FloatImage.h"



FloatImage::FloatImage(int width, int height) {
	_width = width;
	_height = height;
	_handle = new OFX::ImageMemory(sizeof(float) * 4 * width * height);
	_imgPtr = (float*)_handle->lock();
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			SetPixel(x, y, 0.0f);
		}
	}
}

FloatImage::~FloatImage()
{
	_handle->unlock();
	delete _handle;
}

float* FloatImage::GetPixel(int x, int y)
{
	float *pix = _imgPtr + x + y * _width;
	return pix;
}

void FloatImage::SetPixel(int x, int y, float* pix)
{
	float *dstPix = _imgPtr + x + y * _width;
	memcpy(dstPix, pix, sizeof(float) * 4);
}

void FloatImage::SetPixel(int x, int y,
	float r,
	float g,
	float b,
	float a)
{
	float *pix = _imgPtr + x + y * _width;
	pix[0] = r;
	pix[1] = g;
	pix[2] = b;
	pix[3] = a;
}

void FloatImage::SetPixel(int x, int y, float v)
{
	float *pix = _imgPtr + x + y * _width;
	pix[0] = v;
	pix[1] = v;
	pix[2] = v;
	pix[3] = v;
}