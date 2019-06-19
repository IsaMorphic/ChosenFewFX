#ifdef _WINDOWS
#include <windows.h>
#endif

#include <msclr\marshal_cppstd.h>
using namespace msclr::interop;

#include "ofxsImageEffect.h"
using namespace OFX;

#include "Plugin.h"
#include "FilterPlugin.h"
#include "ManagedProcessor.h"
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
}

ImageEffect* ChosenFewFX::ManagedPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	if(pluginHandle->GetType()->BaseType == NET::FilterPlugin::typeid)
		return new FilterPlugin(handle, pluginHandle);
	else
		return new Plugin(handle, pluginHandle);
}
