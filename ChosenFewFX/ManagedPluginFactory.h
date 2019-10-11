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
#include <msclr\marshal_cppstd.h>

#include "ofxsImageEffect.h"

using namespace msclr::interop;
namespace ChosenFewFX {
	template<class T>
	class ManagedPluginFactory : public OFX::PluginFactoryHelper<T>
	{
	private:
		gcroot<NET::Interop::BasePlugin^> pluginHandle;
	public:
		ManagedPluginFactory(NET::Interop::BasePlugin^ handle) : 
			OFX::PluginFactoryHelper<T>(
				marshal_as<std::string>(handle->Id), 
				handle->MajorVersion, handle->MinorVersion), 
			pluginHandle(handle) {}
		virtual void describe(OFX::ImageEffectDescriptor &desc);
		virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
		virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
	};
}

#include "ManagedPluginFactory.cpp"