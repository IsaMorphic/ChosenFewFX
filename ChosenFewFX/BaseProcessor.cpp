#include "BaseProcessor.h"

ChosenFewFX::NET::Interop::ImageWrapper^ ChosenFewFX::BaseProcessor::linkPixelsToManagedImage(OFX::Image *img, NET::Interop::ImageType imageType)
{
	OfxRectI bounds = img->getBounds();
	int rowBytes = img->getRowBytes();
	NET::Geometry::RectangleI managedBounds = NET::Geometry::RectangleI(bounds.x1, bounds.y1, bounds.x2, bounds.y2);
	System::IntPtr pixelPointer = System::IntPtr(img->getPixelData());
	return _managedHandle->LinkPixelsToManagedImage(managedBounds, pixelPointer, imageType);
}

void ChosenFewFX::BaseProcessor::setCurrentTime(OfxTime time) 
{
	_managedHandle->CurrentTime = time;
}

void ChosenFewFX::BaseProcessor::preProcess() 
{
	_managedHandle->_DestImage = linkPixelsToManagedImage(_dstImg, NET::Interop::ImageType::Rgba);
	_managedHandle->PreProcess();
}

void ChosenFewFX::BaseProcessor::multiThreadProcessImages(OfxRectI procWindow) 
{
	NET::Geometry::RectangleI region = NET::Geometry::RectangleI(procWindow.x1, procWindow.y1, procWindow.x2, procWindow.y2);
	_managedHandle->ProcessPixels(region);
}

void ChosenFewFX::BaseProcessor::postProcess() 
{
	_managedHandle->PostProcess();
}