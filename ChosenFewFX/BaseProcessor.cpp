#include "BaseProcessor.h"

SkiaSharp::SKBitmap^ ChosenFewFX::BaseProcessor::linkPixelsToManagedImage(OFX::Image *img)
{
	OfxRectI bounds = img->getBounds();
	int rowBytes = img->getRowBytes();
	NET::RectangleI managedBounds = NET::RectangleI(bounds.x1, bounds.y1, bounds.x2, bounds.y2);
	System::IntPtr pixelPointer = System::IntPtr(img->getPixelData());
	return _managedHandle->LinkPixelsToManagedImage(managedBounds, pixelPointer);
}

void ChosenFewFX::BaseProcessor::preProcess() 
{
	_managedHandle->DestImage = linkPixelsToManagedImage(_dstImg);
	_managedHandle->PreProcess();
}

void ChosenFewFX::BaseProcessor::multiThreadProcessImages(OfxRectI procWindow) 
{
	NET::RectangleI region = NET::RectangleI(procWindow.x1, procWindow.y1, procWindow.x2, procWindow.y2);
	_managedHandle->ProcessPixels(region);
}

void ChosenFewFX::BaseProcessor::postProcess() 
{
	_managedHandle->PostProcess();
}