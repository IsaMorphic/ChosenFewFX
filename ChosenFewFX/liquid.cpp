#ifdef _WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include <math.h>
#include <vector>
#include <array>
#include "ofxsImageEffect.h"
#include "ofxsMultiThread.h"
#include "../include/ofxsProcessing.H"
#include <Magick++.h>
#include "magickprocessor.h"
#include "basicfilterplugin.h"
#include "utils.h"
#include "liquid.h"

class LiquidProcessor : public MagickProcessor {
public:
	double intensity;
	LiquidProcessor(OFX::ImageEffect &instance, OFX::PixelComponentEnum comp, OFX::BitDepthEnum depth)
		: MagickProcessor(instance, comp, depth)
	{}
	void preProcess() {
		Geometry dim = _managedImage.size();
		_managedImage.liquidRescale(Geometry(dim.width() / intensity, dim.height() / intensity));
		_managedImage.resize(dim);
	}
};

class LiquidPlugin : public BasicFilterPlugin<LiquidProcessor> {
	OFX::DoubleParam *intensity;
public:
	LiquidPlugin(OfxImageEffectHandle handle) : BasicFilterPlugin(handle) {
		intensity = fetchDoubleParam("intensity");
	}

	void transferParams(LiquidProcessor &processor, const OFX::RenderArguments &args) {
		processor.intensity = intensity->getValueAtTime(args.time);
	}
};

void LiquidPluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
	desc.setLabels("Image Liquifier", "Image Liquifier", "Image Liquifier");
	desc.setPluginGrouping("Chosen Few FX");
	desc.addSupportedContext(eContextFilter);
	desc.addSupportedContext(eContextGeneral);
	desc.addSupportedBitDepth(eBitDepthFloat);
	desc.addSupportedBitDepth(eBitDepthUByte);
	desc.addSupportedBitDepth(eBitDepthUShort);
	desc.setSingleInstance(false);
	desc.setHostFrameThreading(false);
	desc.setSupportsMultiResolution(true);
	desc.setSupportsTiles(true);
	desc.setTemporalClipAccess(false);
	desc.setRenderTwiceAlways(false);
	desc.setSupportsMultipleClipPARs(false);
	desc.setRenderThreadSafety(eRenderFullySafe);
}

void LiquidPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
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

	DoubleParamDescriptor *paramIntensity = defineDoubleParam(desc, "intensity", "Intensity", "The intensity of the effect", 0, 1, 10, 3);
	page->addChild(*paramIntensity);
}

ImageEffect* LiquidPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	return new LiquidPlugin(handle);
}