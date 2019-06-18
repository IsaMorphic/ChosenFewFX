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
#include "projection.h"

using namespace System;
using namespace Numerics;
class ProjectionProcessor : public MagickProcessor {
public:
	double intensity;
	int* mapping;
	ProjectionProcessor(OFX::ImageEffect &instance, OFX::PixelComponentEnum comp, OFX::BitDepthEnum depth)
		: MagickProcessor(instance, comp, depth)
	{}
	void preProcess() {
		Geometry dim = _managedImage.size();
		MandelbrotDistort::Point topLeft = MandelbrotDistort::Point(0, 0);
		MandelbrotDistort::Point bottomRight = MandelbrotDistort::Point(dim.width(), dim.height());
		MandelbrotDistort::Rectangle imageBounds = MandelbrotDistort::Rectangle(topLeft, bottomRight);

		topLeft = MandelbrotDistort::Point(-2, 2);
		bottomRight = MandelbrotDistort::Point(2, -2);
		MandelbrotDistort::Rectangle projectionBounds = MandelbrotDistort::Rectangle::Rectangle(topLeft, bottomRight);

		MandelbrotDistort::ComplexGrid::ComplexGrid(imageBounds, projectionBounds);
	}

	Complex julia(Complex z) 
	{
		Complex c = Complex(1, .2);
		for (int i = 0; i < 4; i++)
			z = z * z + c;
		return z;
	}
};

class ProjectionPlugin : public BasicFilterPlugin<ProjectionProcessor> {
	OFX::DoubleParam *intensity;
public:
	ProjectionPlugin(OfxImageEffectHandle handle) : BasicFilterPlugin(handle) {
		intensity = fetchDoubleParam("intensity");
	}

	void transferParams(ProjectionProcessor &processor, const OFX::RenderArguments &args) {
		processor.intensity = intensity->getValueAtTime(args.time);
	}
};

void ProjectionPluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
	desc.setLabels("Convoluted Projection", "Convoluted Projection", "Convoluted Projection");
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

void ProjectionPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
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

ImageEffect* ProjectionPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	return new ProjectionPlugin(handle);
}