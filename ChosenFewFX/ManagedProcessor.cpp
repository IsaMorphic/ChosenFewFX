#include "ManagedProcessor.h"

SkiaSharp::SKBitmap^ ChosenFewFX::ManagedProcessor::linkPixelsToManagedImage(OFX::Image *img)
{
	OfxRectI bounds = img->getBounds();
	int rowBytes = img->getRowBytes();
	NET::Rectangle managedBounds = NET::Rectangle(bounds.x1, bounds.y1, bounds.x2, bounds.y2);
	System::IntPtr pixelPointer = System::IntPtr(img->getPixelData());
	return _managedHandle->LinkPixelsToManagedImage(managedBounds, rowBytes, pixelPointer);
}

void ChosenFewFX::ManagedProcessor::preProcess() 
{
	_managedHandle->DestImage = linkPixelsToManagedImage(_dstImg);
	_managedHandle->PreProcess();
}

void ChosenFewFX::ManagedProcessor::multiThreadProcessImages(OfxRectI procWindow) 
{
	NET::Rectangle roi = NET::Rectangle(procWindow.x1, procWindow.y1, procWindow.x2, procWindow.y2);
	_managedHandle->ProcessPixels(roi);
}

void ChosenFewFX::ManagedProcessor::postProcess() 
{
	_managedHandle->PostProcess();
}