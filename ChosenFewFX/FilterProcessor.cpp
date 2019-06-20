#include "FilterProcessor.h"
void ChosenFewFX::FilterProcessor::setSrcImg(OFX::Image *v)
{
	_srcImg = v;
}

void ChosenFewFX::FilterProcessor::preProcess() 
{
	_managedHandle->SourceImage = linkPixelsToManagedImage(_srcImg);
	GeneratorProcessor::preProcess();
}