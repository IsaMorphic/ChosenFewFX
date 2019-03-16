
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
#include "vhs.h"
using namespace Magick;
class MagickProcessor : public OFX::ImageProcessor {
protected:
	OFX::Image *_srcImg;
	Magick::Image _managedImage;
	OFX::PixelComponentEnum _comp;
	OFX::BitDepthEnum _depth;
public:
	/** @brief no arg ctor */
	MagickProcessor(OFX::ImageEffect &instance, OFX::PixelComponentEnum comp, OFX::BitDepthEnum depth)
		: OFX::ImageProcessor(instance)
		, _srcImg(0)
		, _comp(comp)
		, _depth(depth)
		, _managedImage()
	{
	}

	void CopySrcToManaged() {
		OfxRectI bounds = _srcImg->getBounds();
		int width = bounds.x2 - bounds.x1;
		int height = bounds.y2 - bounds.y1;
		_managedImage = Magick::Image(width, height, "RGBA", FloatPixel, _srcImg->getPixelData());
	}

	void CopyManagedToDst() {
		OfxRectI bounds = _dstImg->getBounds();
		int width = bounds.x2 - bounds.x1;
		int height = bounds.y2 - bounds.y1;
		Magick::StorageType storageType = (Magick::StorageType)(_depth - 1);
		_managedImage.write(0, 0, width, height, "RGBA", FloatPixel, _dstImg->getPixelData());
	}

	void preProcess() {
		_managedImage.modifyImage();
	}

	void multiThreadProcessImages(OfxRectI procWindow)
	{
		int width = procWindow.x2 - procWindow.x1;
		int height = procWindow.y2 - procWindow.y1;
		Magick::Quantum *pixels = _managedImage.getPixels(procWindow.x1, procWindow.y1, width, height);
		processPixels(width, height, pixels);
		_managedImage.syncPixels();
	}

	void postProcess() {
		CopyManagedToDst();
	}

	virtual void processPixels(int width, int height, Magick::Quantum *pixels) {

	}
	/** @brief set the src image */
	void setSrcImg(OFX::Image *v) { _srcImg = v; CopySrcToManaged(); }
};

class VHSProcessor : public MagickProcessor {
public:
	// ctor
	VHSProcessor(OFX::ImageEffect &instance, OFX::PixelComponentEnum comp, OFX::BitDepthEnum depth)
		: MagickProcessor(instance, comp, depth)
	{}

	void processPixels(int width, int height, Magick::Quantum *pixels) {
		auto pixelAddr = [width, pixels](int x, int y) {
			int index = 4 * (x + y * width);
			Quantum *pixel = &pixels[index];
			return pixel;
		};
		for (int x = 1; x < width - 1; ++x)
			for (int y = 1; y < height - 1; ++y)
			{
				Quantum *topleft = pixelAddr(x - 1, y - 1);
				Quantum *middle = pixelAddr(x, y);
				Quantum *btmright = pixelAddr(x + 1, y + 1);
				middle[0] = topleft[0];
				middle[2] = btmright[2];
			}
	}
};

template <class GenericProcessor>
class BasicFilterPlugin : public OFX::ImageEffect
{
protected:
	OFX::Clip *dstClip_;
	OFX::Clip *srcClip_;
public:
	BasicFilterPlugin(OfxImageEffectHandle handle) : ImageEffect(handle), dstClip_(0), srcClip_(0)
	{
		dstClip_ = fetchClip(kOfxImageEffectOutputClipName);
		srcClip_ = fetchClip(kOfxImageEffectSimpleSourceClipName);
		// Params here
	}
	virtual void render(const OFX::RenderArguments &args)
	{
		OFX::BitDepthEnum       dstBitDepth = dstClip_->getPixelDepth();
		OFX::PixelComponentEnum dstComponents = dstClip_->getPixelComponents();
		GenericProcessor processor(*this, dstComponents, dstBitDepth);
		setupAndProcess(processor, args);
	}
	virtual bool isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime)
	{
		return false;
	}
	virtual void changedParam(const OFX::InstanceChangedArgs &/*args*/, const std::string &paramName)
	{

	}
	virtual void changedClip(const OFX::InstanceChangedArgs &/*args*/, const std::string &clipName)
	{
		/*if(clipName == kOfxImageEffectSimpleSourceClipName)
		  setEnabledness();*/
	}
	virtual bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
	{
		rod = srcClip_->getRegionOfDefinition(args.time);
		return true;
	}
	virtual void getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois)
	{
		rois.setRegionOfInterest(*srcClip_, args.regionOfInterest);
	}
	void setupAndProcess(MagickProcessor &processor, const OFX::RenderArguments &args)
	{
		std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
		OFX::BitDepthEnum dstBitDepth = dst->getPixelDepth();
		OFX::PixelComponentEnum dstComponents = dst->getPixelComponents();
		std::auto_ptr<OFX::Image> src(srcClip_->fetchImage(args.time));

		if (src.get())
		{
			OFX::BitDepthEnum    srcBitDepth = src->getPixelDepth();
			OFX::PixelComponentEnum srcComponents = src->getPixelComponents();
			if (srcBitDepth != dstBitDepth || srcComponents != dstComponents)
				throw int(1);
		}
		processor.setDstImg(dst.get());
		processor.setSrcImg(src.get());
		processor.setRenderWindow(args.renderWindow);
		processor.process();
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
	desc.setSingleInstance(false);
	desc.setHostFrameThreading(false);
	desc.setSupportsMultiResolution(true);
	desc.setSupportsTiles(true);
	desc.setTemporalClipAccess(false);
	desc.setRenderTwiceAlways(false);
	desc.setSupportsMultipleClipPARs(false);
	desc.setRenderThreadSafety(eRenderFullySafe);
}

static
IntParamDescriptor *defineIntParam(OFX::ImageEffectDescriptor &desc,
	const std::string &name, const std::string &label, const std::string &hint,
	GroupParamDescriptor *parent, int min, int max, int def)
{
	IntParamDescriptor *param = desc.defineIntParam(name);
	param->setLabels(label, label, label);
	param->setScriptName(name);
	param->setHint(hint);
	param->setDefault(def);
	param->setRange(min, max);
	param->setDisplayRange(min, max);
	if (parent)
		param->setParent(*parent);
	return param;
}

static
BooleanParamDescriptor *defineBoolParam(OFX::ImageEffectDescriptor &desc,
	const std::string &name, const std::string &label, const std::string &hint,
	GroupParamDescriptor *parent, bool def)
{
	BooleanParamDescriptor *param = desc.defineBooleanParam(name);
	param->setLabels(label, label, label);
	param->setScriptName(name);
	param->setHint(hint);
	param->setDefault(def);
	if (parent)
		param->setParent(*parent);
	return param;
}

void VHSPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
{
	ClipDescriptor *srcClip = desc.defineClip(kOfxImageEffectSimpleSourceClipName);
	srcClip->addSupportedComponent(ePixelComponentRGBA);
	srcClip->setTemporalClipAccess(false);
	srcClip->setSupportsTiles(true);
	srcClip->setIsMask(false);

	ClipDescriptor *dstClip = desc.defineClip(kOfxImageEffectOutputClipName);
	dstClip->addSupportedComponent(ePixelComponentRGBA);
	dstClip->setSupportsTiles(true);

	PageParamDescriptor *page = desc.definePageParam("Controls");

	IntParamDescriptor *paramFreq = defineIntParam(desc, "frequency", "Frequency", "The frequency at which tiles appear and disappear (in 20ths of a second, lower is faster).", 0, 1, 20, 5);
	page->addChild(*paramFreq);
	IntParamDescriptor *paramTile = defineIntParam(desc, "maxTiles", "Maximum # of Tiles", "The maximum amount of tiles that can be displayed on screen at once.", 0, 1, 100, 10);
	page->addChild(*paramTile);
	IntParamDescriptor *paramSeed = defineIntParam(desc, "seed", "Random Seed", "The seed to use for the random number generator.", 0, 0, INT_MAX, INT_MAX / 2);
	page->addChild(*paramSeed);
	BooleanParamDescriptor *paramColor = defineBoolParam(desc, "glitchColors", "Should Glitch Colors", "Whether or not to randomly change the hue of tiles", 0, true);
	page->addChild(*paramColor);

}

ImageEffect* VHSPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	return new BasicFilterPlugin<VHSProcessor>(handle);
}

