/*
 *  Copyright 2019 Chosen Few Software
 *  This file is part of Chosen Few FX.
 *
 *  Chosen Few FX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Chosen Few FX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Chosen Few FX.  If not, see <https://www.gnu.org/licenses/>.
 */
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