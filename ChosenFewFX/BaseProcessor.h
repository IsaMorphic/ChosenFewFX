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
#pragma once
#include <vcclr.h>
#include "ofxsMultiThread.h"
#include "ofxsProcessing.H"
namespace ChosenFewFX {
	class BaseProcessor : public OFX::ImageProcessor {
	private:
		gcroot<NET::Interop::BasePlugin^> _managedHandle;
	public:
		BaseProcessor(OFX::ImageEffect &instance, NET::Interop::BasePlugin^ handle) : 
			OFX::ImageProcessor(instance), _managedHandle(handle) {}

		NET::Interop::ImageWrapper^ linkPixelsToManagedImage(OFX::Image *img, NET::Interop::ImageType imageType);

		void setCurrentTime(OfxTime time);

		virtual void preProcess();
		virtual void multiThreadProcessImages(OfxRectI procWindow);
		virtual void postProcess();
	};
}
