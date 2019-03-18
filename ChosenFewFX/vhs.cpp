
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
#include "vhs.h"
using namespace Magick;
using namespace OFX;

#define PI 3.14159265

typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
	hsv         out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0;
		out.h = 0; // undefined, maybe nan?
		return out;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0              
		// s = 0, h is undefined
		out.s = 0.0;
		out.h = NAN;                            // its now undefined
		return out;
	}
	if (in.r >= max)                           // > is bogus, just keeps compilor happy
		out.h = (in.g - in.b) / delta;        // between yellow & magenta
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}


rgb hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

class VHSProcessor : public MagickProcessor {
private:
	Magick::Image _originalImage;
public:
	int aberrationParam;
	int hueAdjustParam;
	double saturationAdjustParam;
	double luminanceAdjustParam;
	int ribbonDistanceParam;
	int ribbonWidthParam;
	int aberrationAngleParam;
	// ctor
	VHSProcessor(OFX::ImageEffect &instance, OFX::PixelComponentEnum comp, OFX::BitDepthEnum depth)
		: MagickProcessor(instance, comp, depth)
	{}
	void preProcess() {
		Geometry dim = _managedImage.size();
		_managedImage.liquidRescale(Geometry(dim.width() / 2, dim.height() / 2));
		_managedImage.resize(dim);
		_originalImage = Magick::Image(_managedImage);
		MagickProcessor::preProcess();
	}
	void processPixels(OfxRectI procWindow, Magick::Quantum *pixels) {
		int width = procWindow.x2 - procWindow.x1;
		int height = procWindow.y2 - procWindow.y1;
		auto pixelAddr = [width](int x, int y, Magick::Quantum *ptr) {
			int index = 4 * (x + y * width);
			Quantum *pixel = &ptr[index];
			return pixel;
		};
		Quantum *origPixels = _originalImage.getPixels(procWindow.x1, procWindow.y1, width, height);
		double offX = aberrationParam * cos(aberrationAngleParam * PI / 180);
		double offY = aberrationParam * sin(aberrationAngleParam * PI / 180);
		for (int x = 0; x < width; ++x)
			for (int y = 0; y < height; ++y)
			{
				Quantum *topleft = pixelAddr(min(max(x - offX, 0), width - 1), min(max(y - offY, 0), height - 1), origPixels);
				Quantum *middle = pixelAddr(x, y, pixels);
				Quantum *btmright = pixelAddr(min(max(x + offX, 0), width - 1), min(max(y + offY, 0), height - 1), origPixels);
				middle[0] = topleft[0];
				middle[2] = btmright[2];
			}
		if (ribbonDistanceParam == 0 || ribbonWidthParam == 0)
			return;
		_managedImage.syncPixels();
		for (int y = 0; y < height; y += std::rand() % ribbonDistanceParam)
			for (int i = 0; i < std::rand() % ribbonWidthParam && y < height; ++i, ++y)
				for (int x = 0; x < width; ++x)
				{
					Quantum *pixel = pixelAddr(x, y, pixels);
					rgb colorOrig;
					colorOrig.r = (double)pixel[0] / QuantumRange;
					colorOrig.g = (double)pixel[1] / QuantumRange;
					colorOrig.b = (double)pixel[2] / QuantumRange;
					hsv colorConverted = rgb2hsv(colorOrig);
					colorConverted.h = min(max(colorConverted.h + hueAdjustParam, 0), 360);
					colorConverted.s = min(max(colorConverted.s + saturationAdjustParam, 0), 1);
					colorConverted.v = min(max(colorConverted.v + luminanceAdjustParam, 0), 1);
					rgb colorAdjusted = hsv2rgb(colorConverted);
					pixel[0] = colorAdjusted.r * QuantumRange;
					pixel[1] = colorAdjusted.g * QuantumRange;
					pixel[2] = colorAdjusted.b * QuantumRange;
				}
		_managedImage.syncPixels();
		for (int y = 0; y < height; ++y)
				for (int x = 0; x < width; ++x)
				{
					Quantum *pixel = pixelAddr(x, y, pixels);
					pixel[0] = min(pixel[0] * 1.2, QuantumRange);
					pixel[1] = min(pixel[1] * 1.2, QuantumRange);
					pixel[2] = min(pixel[2] * 1.2, QuantumRange);
				}
	}
};

class VHSPlugin : public BasicFilterPlugin<VHSProcessor>
{
private:
	OFX::IntParam *aberration;
	OFX::IntParam *aberrationAngle;
	OFX::IntParam *hueAdjust;
	OFX::IntParam *saturationAdjust;
	OFX::IntParam *luminanceAdjust;
	OFX::IntParam *ribbonWidth;
	OFX::IntParam *ribbonDistance;
public:
	VHSPlugin(OfxImageEffectHandle handle) : BasicFilterPlugin(handle) {
		aberration = fetchIntParam("aberration");
		aberrationAngle = fetchIntParam("aberrationAngle");
		hueAdjust = fetchIntParam("hueAdjust");
		saturationAdjust = fetchIntParam("saturationAdjust");
		luminanceAdjust = fetchIntParam("luminanceAdjust");
		ribbonDistance = fetchIntParam("ribbonDistance");
		ribbonWidth = fetchIntParam("ribbonWidth");
	}
	void transferParams(VHSProcessor &processor, const OFX::RenderArguments &args)
	{
		processor.aberrationParam = aberration->getValueAtTime(args.time);
		processor.aberrationAngleParam = aberrationAngle->getValueAtTime(args.time);
		processor.hueAdjustParam = hueAdjust->getValueAtTime(args.time);
		processor.saturationAdjustParam = saturationAdjust->getValueAtTime(args.time) / 100.0;
		processor.luminanceAdjustParam = luminanceAdjust->getValueAtTime(args.time) / 100.0;
		processor.ribbonDistanceParam = ribbonDistance->getValueAtTime(args.time);
		processor.ribbonWidthParam = ribbonWidth->getValueAtTime(args.time);
	}
};
using namespace OFX;

void VHSPluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
	desc.setLabels("VHS Filter", "VHS Filter", "VHS Filter");
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

void VHSPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
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

	IntParamDescriptor *paramAberr = defineIntParam(desc, "aberration", "Chromatic Aberration Amount", "The amount of chromatic aberration to apply", 0, 0, 256, 3);
	page->addChild(*paramAberr);

	IntParamDescriptor *paramAberrAngle = defineIntParam(desc, "aberrationAngle", "Chromatic Aberration Angle", "The angle of the chromatic displacement", 0, 0, 360, 45);
	page->addChild(*paramAberrAngle);

	IntParamDescriptor *paramHue = defineIntParam(desc, "hueAdjust", "Color Ribbon Hue Adjust", "The amount of degrees to adjust the hue of a color ribbon", 0, -180, 180, 20);
	page->addChild(*paramHue);

	IntParamDescriptor *paramSaturation = defineIntParam(desc, "saturationAdjust", "Color Ribbon Saturation Adjust", "The percentage amount to adjust the saturation of a color ribbon", 0, -100, 100, 30);
	page->addChild(*paramSaturation);

	IntParamDescriptor *paramLuminance = defineIntParam(desc, "luminanceAdjust", "Color Ribbon Luminance Adjust", "The percentage amount to adjust the luminance of a color ribbon", 0, -100, 100, 30);
	page->addChild(*paramLuminance);

	IntParamDescriptor *paramMaxRibbonDistance = defineIntParam(desc, "ribbonDistance", "Maximum Color Ribbon Distance", "The maximum distance between each color ribbon", 0, 0, 500, 120);
	page->addChild(*paramMaxRibbonDistance);

	IntParamDescriptor *paramMaxRibbonWidth = defineIntParam(desc, "ribbonWidth", "Maximum Color Ribbon Width", "The maximum width of a color ribbon", 0, 0, 200, 40);
	page->addChild(*paramMaxRibbonWidth);

}

ImageEffect* VHSPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	return new VHSPlugin(handle);
}

