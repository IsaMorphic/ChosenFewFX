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
#include "ManagedPluginFactory.h"
#include "utils.h"
void ChosenFewFX::ManagedPluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
	std::string label = marshal_as<std::string>(pluginHandle->Name);
	desc.setLabels(label, label, label);
	desc.setPluginGrouping("Chosen Few FX");

	if (pluginHandle->GetType()->BaseType == NET::FilterPlugin::typeid)
		desc.addSupportedContext(eContextFilter);
	else
		desc.addSupportedContext(eContextGenerator);

	desc.addSupportedContext(eContextGeneral);
	desc.addSupportedBitDepth(eBitDepthFloat);
	desc.addSupportedBitDepth(eBitDepthUByte);
	desc.addSupportedBitDepth(eBitDepthUShort);
	desc.setHostFrameThreading(false);
	desc.setRenderTwiceAlways(false);
	desc.setRenderThreadSafety(eRenderInstanceSafe);
}

void ChosenFewFX::ManagedPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
{
	ClipDescriptor *srcClip = desc.defineClip(kOfxImageEffectSimpleSourceClipName);
	srcClip->addSupportedComponent(ePixelComponentRGBA);
	srcClip->addSupportedComponent(ePixelComponentAlpha);
	srcClip->setTemporalClipAccess(false);
	srcClip->setSupportsTiles(true);
	srcClip->setIsMask(false);

	ClipDescriptor *dstClip = desc.defineClip(kOfxImageEffectOutputClipName);
	dstClip->addSupportedComponent(ePixelComponentRGBA);
	dstClip->addSupportedComponent(ePixelComponentAlpha);
	dstClip->setSupportsTiles(true);

	PageParamDescriptor *page = desc.definePageParam("Controls");

	System::Type^ pluginType = pluginHandle->GetType();
	auto fields = pluginType->GetFields();
	for each(System::Reflection::FieldInfo^ field in fields)
	{
		auto paramAttrs = field->GetCustomAttributes(NET::ParamAttribute::typeid, false);
		if (paramAttrs->Length == 1)
		{
			NET::ParamAttribute^ paramAttr = (NET::ParamAttribute^)paramAttrs[0];

			std::string name = marshal_as<std::string>(field->Name);
			std::string label = marshal_as<std::string>(System::String::Copy(paramAttr->Label));

			ParamDescriptor *param;

			if (field->FieldType == System::Boolean::typeid)
				param = defineBoolParam(desc, name, label, "Nothing to see here...", NULL, safe_cast<bool>(paramAttr->DefaultValue));

			else {
				NET::RangeParamAttribute^ rangeAttr = (NET::RangeParamAttribute^)paramAttr;

				if (field->FieldType == System::Int32::typeid)
					param = defineIntParam(desc, name, label, "Nothing to see here...", NULL,
					(int)rangeAttr->DefaultValue, (int)rangeAttr->MinimumValue, (int)rangeAttr->MaximumValue);

				else if(field->FieldType == System::Double::typeid)
					param = defineDoubleParam(desc, name, label, "Nothing to see here...", NULL,
					(double)rangeAttr->DefaultValue, (double)rangeAttr->MinimumValue, (double)rangeAttr->MaximumValue);
			}

			page->addChild(*param);
		}
	}
}

ImageEffect* ChosenFewFX::ManagedPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	if (pluginHandle->GetType()->BaseType == NET::FilterPlugin::typeid)
		return new FilterPlugin(handle, pluginHandle);
	else
		return new BasePlugin(handle, pluginHandle);
}
