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
#include "BasePlugin.h"

ChosenFewFX::BasePlugin::BasePlugin(OfxImageEffectHandle handle, NET::Interop::BasePlugin^ plugin) :
	OFX::ImageEffect(handle), dstClip_(0), pluginHandle(plugin), 
	paramFields(gcnew List<System::Reflection::FieldInfo^>)
{
	dstClip_ = fetchClip(kOfxImageEffectOutputClipName);

	System::Type^ pluginType = pluginHandle->GetType();
	auto fields = pluginType->GetFields();
	for each(System::Reflection::FieldInfo^ field in fields)
	{
		auto paramAttrs = field->GetCustomAttributes(NET::Interop::ParamAttribute::typeid, false);
		if (paramAttrs->Length == 1)
			paramFields->Add(field);
	}
}

void ChosenFewFX::BasePlugin::render(const OFX::RenderArguments &args)
{
	if (shouldUpdateParams == true) 
	{
		transferParams(args.time);
		for each (std::string param in updatedParams)
		{
			pluginHandle->ParamUpdated(marshal_as<System::String^>(param));
		}
		shouldUpdateParams = false;
		updatedParams.clear();
	}
	BaseProcessor processor(*this, pluginHandle);
	std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
	processor.setDstImg(dst.get());
	processor.setRenderWindow(args.renderWindow);
	processor.setCurrentTime(args.time);
	transferParams(args.time);
	processor.process();
}

bool ChosenFewFX::BasePlugin::isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime)
{
	return false;
}

void ChosenFewFX::BasePlugin::transferParams(OfxTime time)
{
	for each (System::Reflection::FieldInfo^ field in (List<System::Reflection::FieldInfo^>^)paramFields)
	{
		std::string name = marshal_as<std::string>(field->Name);
		if (field->FieldType == System::Boolean::typeid)
			field->SetValue(pluginHandle, fetchBooleanParam(name)->getValue());

		else if (field->FieldType == System::Int32::typeid)
			field->SetValue(pluginHandle, fetchIntParam(name)->getValue());

		else if (field->FieldType == System::Double::typeid)
			field->SetValue(pluginHandle, fetchDoubleParam(name)->getValueAtTime(time));

		else if (field->FieldType == NET::Interop::Color::typeid) {
			double r, g, b, a;
			fetchRGBAParam(name)->getValueAtTime(time, r, g, b, a);
			NET::Interop::Color color = NET::Interop::Color::Color((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255));
			field->SetValue(pluginHandle, color);
		}
		else if (field->FieldType == System::String::typeid) {
			std::string value;
			fetchStringParam(name)->getValue(value);
			field->SetValue(pluginHandle, marshal_as<System::String^>(value));
		}
	}
}

void ChosenFewFX::BasePlugin::changedParam(const OFX::InstanceChangedArgs &args, const std::string &paramName)
{
	shouldUpdateParams = true;
	updatedParams.push_back(paramName);
}