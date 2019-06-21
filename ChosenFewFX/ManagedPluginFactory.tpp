#ifdef _WINDOWS
#include <windows.h>
#endif

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

	if (pluginHandle->GetType()->BaseType == NET::Interop::FilterPlugin::typeid)
		desc.addSupportedContext(eContextFilter);
	else
		desc.addSupportedContext(eContextGenerator);

	desc.addSupportedContext(eContextGeneral);
	desc.addSupportedBitDepth(eBitDepthFloat);
	desc.addSupportedBitDepth(eBitDepthUByte);
	desc.addSupportedBitDepth(eBitDepthUShort);
	desc.setRenderThreadSafety(eRenderUnsafe);
	desc.setRenderTwiceAlways(false);
}

template<class T>
void ChosenFewFX::ManagedPluginFactory<T>::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
{
	ClipDescriptor *srcClip = desc.defineClip(kOfxImageEffectSimpleSourceClipName);
	srcClip->addSupportedComponent(ePixelComponentRGBA);
	srcClip->addSupportedComponent(ePixelComponentAlpha);
	srcClip->setSupportsTiles(true);

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

			ParamDescriptor *param;

			if (field->FieldType == System::Boolean::typeid)
				param = defineBoolParam(desc, name, label, "Nothing to see here...", NULL, safe_cast<bool>(paramAttr->DefaultValue));

			else {
				NET::Interop::RangeParamAttribute^ rangeAttr = (NET::Interop::RangeParamAttribute^)paramAttrs[0];

				if (field->FieldType == System::Int32::typeid)
					param = defineIntParam(desc, name, label, "Nothing to see here...", NULL,
						safe_cast<int>(rangeAttr->MinimumValue), safe_cast<int>(rangeAttr->MaximumValue), safe_cast<int>(rangeAttr->DefaultValue));

				else if(field->FieldType == System::Double::typeid)
					param = defineDoubleParam(desc, name, label, "Nothing to see here...", NULL,
						safe_cast<double>(rangeAttr->MinimumValue), safe_cast<double>(rangeAttr->MaximumValue), safe_cast<double>(rangeAttr->DefaultValue));
			}

			page->addChild(*param);
		}
	}
}

template<class T>
ImageEffect* ChosenFewFX::ManagedPluginFactory<T>::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	if (pluginHandle->GetType()->BaseType == NET::Interop::FilterPlugin::typeid)
		return new FilterPlugin(handle, pluginHandle);
	else
		return new BasePlugin(handle, pluginHandle);
}
