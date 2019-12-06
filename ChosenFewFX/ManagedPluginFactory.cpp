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
#include <msclr\marshal_cppstd.h>
using namespace msclr::interop;

#include "ofxsImageEffect.h"
using namespace OFX;

#include "BasePlugin.h"
#include "FilterPlugin.h"
#include "BaseProcessor.h"
#include "utils.h"

template<class T>
void ChosenFewFX::ManagedPluginFactory<T>::describe(OFX::ImageEffectDescriptor &desc)
{
	std::string label = marshal_as<std::string>(pluginHandle->Name);
	desc.setLabels(label, label, label);
	desc.setPluginGrouping("Chosen Few FX");

	if (pluginHandle->GetType()->IsSubclassOf(NET::Interop::FilterPlugin::typeid))
		desc.addSupportedContext(eContextFilter);
	else
		desc.addSupportedContext(eContextGenerator);

	desc.addSupportedContext(eContextGeneral);

	desc.addSupportedBitDepth(eBitDepthUByte);

	desc.setRenderThreadSafety(eRenderUnsafe);
	desc.setHostFrameThreading(false);
	desc.setRenderTwiceAlways(false);
	desc.setSingleInstance(true);
}

template<class T>
void ChosenFewFX::ManagedPluginFactory<T>::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
{
	switch (context) {
	case eContextFilter:
		ClipDescriptor *srcClip = desc.defineClip(kOfxImageEffectSimpleSourceClipName);
		srcClip->addSupportedComponent(ePixelComponentRGBA);
		srcClip->addSupportedComponent(ePixelComponentAlpha);
		srcClip->setSupportsTiles(true);
		break;
	}

	ClipDescriptor *dstClip = desc.defineClip(kOfxImageEffectOutputClipName);
	dstClip->addSupportedComponent(ePixelComponentRGBA);
	dstClip->addSupportedComponent(ePixelComponentAlpha);
	dstClip->setSupportsTiles(true);

	PageParamDescriptor *page = desc.definePageParam("Controls");

	System::Type^ pluginType = pluginHandle->GetType();
	auto fields = pluginType->GetFields();
	for each(System::Reflection::FieldInfo^ field in fields)
	{
		auto paramAttrs = field->GetCustomAttributes(NET::Interop::ParamAttribute::typeid, false);
		if (paramAttrs->Length == 1)
		{
			NET::Interop::ParamAttribute^ paramAttr = (NET::Interop::ParamAttribute^)paramAttrs[0];

			std::string name = marshal_as<std::string>(field->Name);
			std::string label = marshal_as<std::string>(System::String::Copy(paramAttr->Label));
			std::string hint = marshal_as<std::string>(System::String::Copy(paramAttr->Hint));
			System::Object^ def = paramAttr->DefaultValue;

			ParamDescriptor *param;

			if (field->FieldType == System::Boolean::typeid) {
				param = defineBoolParam(desc, name, label, hint, NULL, safe_cast<bool>(def));
			}
			else if (field->FieldType == NET::Interop::Color::typeid) {
				array<byte>^ defaultValue = safe_cast<array<byte>^>(def);
				param = defineColorParam(desc, name, label, hint, NULL, NET::Interop::Color::Color(
					defaultValue[0], defaultValue[1], defaultValue[2], defaultValue[3]));
			}
			else if (field->FieldType == System::String::typeid)
			{
				NET::Interop::StringParamAttribute^ stringAttr = (NET::Interop::StringParamAttribute^)paramAttrs[0];
				std::string defaultValue = marshal_as<std::string>(safe_cast<System::String^>(def));
				param = defineStringParam(desc, name, label, hint, NULL, defaultValue, static_cast<OFX::StringTypeEnum>(stringAttr->StringType));
			}
			else {
				NET::Interop::RangeParamAttribute^ rangeAttr = (NET::Interop::RangeParamAttribute^)paramAttrs[0];

				System::Object^ min = rangeAttr->MinimumValue;
				System::Object^ max = rangeAttr->MaximumValue;

				if (field->FieldType == System::Int32::typeid)
					param = defineIntParam(desc, name, label, hint, NULL,
						safe_cast<int>(min), safe_cast<int>(max), safe_cast<int>(def));

				else if (field->FieldType == System::Double::typeid)
					param = defineDoubleParam(desc, name, label, hint, NULL,
						safe_cast<double>(min), safe_cast<double>(max), safe_cast<double>(def));
			}

			page->addChild(*param);
		}
	}
}

template<class T>
ImageEffect* ChosenFewFX::ManagedPluginFactory<T>::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	if (pluginHandle->GetType()->IsSubclassOf(NET::Interop::FilterPlugin::typeid))
		return new FilterPlugin(handle, pluginHandle);
	else
		return new BasePlugin(handle, pluginHandle);
}
