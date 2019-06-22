#include "FilterProcessor.h"

void ChosenFewFX::FilterProcessor::setSrcImg(OFX::Image *v)
{
	_srcImg = v;
}

void ChosenFewFX::FilterProcessor::preProcess() 
{
	_managedHandle->_SourceImage = linkPixelsToManagedImage(_srcImg, NET::Interop::ImageType::Bgra);
	BaseProcessor::preProcess();
}