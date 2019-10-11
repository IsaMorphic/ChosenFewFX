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
#include "BaseProcessor.h"
namespace ChosenFewFX {
	class FilterProcessor : public BaseProcessor {
	private:
		gcroot<NET::Interop::FilterPlugin^> _managedHandle;
		OFX::Image *_srcImg;
	public:
		FilterProcessor(OFX::ImageEffect &instance, NET::Interop::FilterPlugin^ handle) :
			BaseProcessor(instance, handle), _srcImg(0),
			_managedHandle(handle) {}
		void preProcess();
		void setSrcImg(OFX::Image *v);
	};
}