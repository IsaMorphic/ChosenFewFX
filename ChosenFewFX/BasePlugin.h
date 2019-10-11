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
#include <vector>
#include "BaseProcessor.h"
#include "ofxsImageEffect.h"

#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;
using namespace System::Collections::Generic;
namespace ChosenFewFX {
	class BasePlugin : public OFX::ImageEffect
	{
	private:
		bool shouldUpdateParams;
		std::vector<std::string> updatedParams;
		gcroot<NET::Interop::BasePlugin^> pluginHandle;
		gcroot<List<System::Reflection::FieldInfo^>^> paramFields;
	protected:
		OFX::Clip *dstClip_;
		void transferParams(OfxTime time);
	public:
		BasePlugin(OfxImageEffectHandle handle, NET::Interop::BasePlugin^ plugin);

		virtual void render(const OFX::RenderArguments &args);

		virtual bool isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime);

		virtual void changedParam(const OFX::InstanceChangedArgs &args, const std::string &paramName);
	};
}