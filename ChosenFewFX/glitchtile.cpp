
#ifdef _WINDOWS
#include <windows.h>
#endif
#define FX_FRACTION 20

#include <stdio.h>
#include <math.h>
#include <vector>
#include <array>
#include "ofxsImageEffect.h"
#include "ofxsMultiThread.h"
#include "../include/ofxsProcessing.H"

#include "glitchtile.h"

struct GlitchTile {
	OfxRectI region;
	OfxTime time;
	std::array<bool, 4> maxComps;
	GlitchTile(int x1, int x2, int y1, int y2, OfxTime t, 
		std::array<bool, 4> comps = { false, false, false, false }) {
		OfxRectI *new_rect = new OfxRectI;
		new_rect->x1 = x1;
		new_rect->x2 = x2;
		new_rect->y1 = y1;
		new_rect->y2 = y2;
		region = *new_rect;
		time = t;
		maxComps = comps;
	}
};

class GlitcherBase : public OFX::ImageProcessor {
protected:
	OFX::Image *_srcImg;
	std::array<bool, 4> maxComps;
public:
	/** @brief no arg ctor */
	GlitcherBase(OFX::ImageEffect &instance)
		: OFX::ImageProcessor(instance)
		, _srcImg(0)
	{
	}

	/** @brief set the src image */
	void setSrcImg(OFX::Image *v) { _srcImg = v; }
	void setMaxComps(std::array<bool, 4> comps) { maxComps = comps; }
};

template <class PIX, int nComponents, int max>
class ImageGlitcher : public GlitcherBase {
public:
	// ctor
	ImageGlitcher(OFX::ImageEffect &instance)
		: GlitcherBase(instance)
	{}

	// and do some processing
	void multiThreadProcessImages(OfxRectI procWindow)
	{
		for (int y = procWindow.y1; y < procWindow.y2; y++) {
			if (_effect.abort()) break;

			PIX *dstPix = (PIX *)_dstImg->getPixelAddress(procWindow.x1, y);

			for (int x = procWindow.x1; x < procWindow.x2; x++) {

				PIX *srcPix = (PIX *)(_srcImg ? _srcImg->getPixelAddress(x, y) : 0);

				// do we have a source image to scale up
				if (srcPix) {
					for (int c = 0; c < nComponents; c++) {
						if (maxComps[c])
							dstPix[c] = max;
						else
							dstPix[c] = srcPix[c];
					}
				}
				else {
					// no src pixel here, be black and transparent
					for (int c = 0; c < nComponents; c++) {
						dstPix[c] = 0;
					}
				}

				// increment the dst pixel
				dstPix += nComponents;
			}
		}
	}
};

class GlitchTilePlugin : public OFX::ImageEffect
{
protected:
	OFX::Clip *dstClip_;
	OFX::Clip *srcClip_;

	OFX::IntParam  *frequency_;
	OFX::IntParam  *maxTiles_;
	OFX::IntParam  *seed_;
	OFX::BooleanParam *glitchColors_;

	std::vector<GlitchTile> tiles;
public:
	GlitchTilePlugin(OfxImageEffectHandle handle) : ImageEffect(handle), dstClip_(0), srcClip_(0), frequency_(0), maxTiles_(0), seed_(0), glitchColors_(0)
	{
		dstClip_ = fetchClip(kOfxImageEffectOutputClipName);
		srcClip_ = fetchClip(kOfxImageEffectSimpleSourceClipName);
		frequency_ = fetchIntParam("frequency");
		maxTiles_ = fetchIntParam("maxTiles");
		seed_ = fetchIntParam("seed");
		glitchColors_ = fetchBooleanParam("glitchColors");
	}
	virtual void render(const OFX::RenderArguments &args);
	virtual bool isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime);
	virtual void changedParam(const OFX::InstanceChangedArgs &args, const std::string &paramName);
	virtual void changedClip(const OFX::InstanceChangedArgs &args, const std::string &clipName);
	virtual bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod);
	virtual void getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois);
	void setupAndProcess(GlitcherBase&, const OFX::RenderArguments &args);
};

void GlitchTilePlugin::setupAndProcess(GlitcherBase &processor, const OFX::RenderArguments &args)
{
	while (tiles.size() > 1) {
		if (tiles[1].time > args.time)
			tiles.erase(tiles.begin() + 1);
		else
			break;
	}

	int maxTiles, frequency;
	maxTiles = maxTiles_->getValueAtTime(args.time);
	frequency = frequency_->getValueAtTime(args.time);
	bool glitchColors = glitchColors_->getValue();

	int frameWidth = args.renderWindow.x2 - args.renderWindow.x1;
	int frameHeight = args.renderWindow.y2 - args.renderWindow.y1;
	GlitchTile *cur_frame = new GlitchTile(0, frameWidth, 0, frameHeight, args.time);
	if (tiles.size() < 1)
		tiles.push_back(*cur_frame);
	else
		tiles[0] = *cur_frame;
	if ((int)(args.time * FX_FRACTION) % frequency == 0) {
		int x1 = std::rand() % (frameWidth);
		int y1 = std::rand() % (frameHeight);
		int x2 = x1 + std::rand() % (frameWidth);
		int y2 = y1 + std::rand() % (frameHeight);
		GlitchTile *newTile = new GlitchTile(x1, x2, y1, y2, args.time);
		if (glitchColors) {
			for (int i = 0; i < 4; i++) {
				newTile->maxComps[i] = (std::rand() % 4 == 0);
			}
		}
		if (tiles.size() > maxTiles)
		{
			tiles.erase(tiles.begin() + 1);
		}
		tiles.push_back(*newTile);
	}

	std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
	OFX::BitDepthEnum dstBitDepth = dst->getPixelDepth();
	OFX::PixelComponentEnum dstComponents = dst->getPixelComponents();
	for (int i = 0; i < tiles.size(); i++) {
		std::auto_ptr<OFX::Image> src(srcClip_->fetchImage(tiles[i].time));
		if (src.get())
		{
			OFX::BitDepthEnum    srcBitDepth = src->getPixelDepth();
			OFX::PixelComponentEnum srcComponents = src->getPixelComponents();
			if (srcBitDepth != dstBitDepth || srcComponents != dstComponents)
				throw int(1);
		}
		OfxRectI *region = new OfxRectI;
		region->x1 = args.renderWindow.x1 + tiles[i].region.x1;
		region->x2 = args.renderWindow.x1 + tiles[i].region.x2;
		region->y1 = args.renderWindow.y1 + tiles[i].region.y1;
		region->y2 = args.renderWindow.y1 + tiles[i].region.y2;
		processor.setDstImg(dst.get());
		processor.setSrcImg(src.get());
		processor.setMaxComps(tiles[i].maxComps);
		processor.setRenderWindow(*region);
		processor.process();
	}
}

bool GlitchTilePlugin::getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
{
	rod = srcClip_->getRegionOfDefinition(args.time);
	return true;
}

void GlitchTilePlugin::getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois)
{
	rois.setRegionOfInterest(*srcClip_, args.regionOfInterest);
}

void GlitchTilePlugin::render(const OFX::RenderArguments &args)
{
	OFX::BitDepthEnum       dstBitDepth = dstClip_->getPixelDepth();
	OFX::PixelComponentEnum dstComponents = dstClip_->getPixelComponents();
	if (dstComponents == OFX::ePixelComponentRGBA)
	{
		switch (dstBitDepth)
		{
		case OFX::eBitDepthUByte:
		{
			ImageGlitcher<unsigned char, 4, 255> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthUShort:
		{
			ImageGlitcher<unsigned short, 4, 65535> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthFloat:
		{
			ImageGlitcher<float, 4, 1> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		default:
			OFX::throwSuiteStatusException(kOfxStatErrUnsupported);
		}
	}
	else
	{
		switch (dstBitDepth)
		{
		case OFX::eBitDepthUByte:
		{
			ImageGlitcher<unsigned char, 1, 255> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthUShort:
		{
			ImageGlitcher<unsigned short, 1, 65535> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthFloat:
		{
			ImageGlitcher<float, 1, 1> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		default:
			OFX::throwSuiteStatusException(kOfxStatErrUnsupported);
		}
	}
}

bool GlitchTilePlugin::isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime)
{
	return false;
}

void GlitchTilePlugin::changedParam(const OFX::InstanceChangedArgs &/*args*/, const std::string &paramName)
{
	if (paramName == "seed") {
		int seed = seed_->getValue();
		std::srand(seed);
	}
	else if (paramName == "maxTiles")
	{
		tiles.clear();
	}
}

void GlitchTilePlugin::changedClip(const OFX::InstanceChangedArgs &/*args*/, const std::string &clipName)
{
	/*if(clipName == kOfxImageEffectSimpleSourceClipName)
	  setEnabledness();*/
}

using namespace OFX;

void GlitchTilePluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
	desc.setLabels("Tile Glitch", "Tile Glitch", "Tile Glitch");
	desc.setPluginGrouping("Chosen Few FX");
	desc.addSupportedContext(eContextFilter);
	desc.addSupportedContext(eContextGeneral);
	desc.addSupportedBitDepth(eBitDepthUByte);
	desc.addSupportedBitDepth(eBitDepthUShort);
	desc.addSupportedBitDepth(eBitDepthFloat);
	desc.setSingleInstance(true);
	desc.setHostFrameThreading(false);
	desc.setSupportsMultiResolution(true);
	desc.setSupportsTiles(true);
	desc.setTemporalClipAccess(false);
	desc.setRenderTwiceAlways(false);
	desc.setSupportsMultipleClipPARs(false);
	desc.setRenderThreadSafety(eRenderUnsafe);
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

void GlitchTilePluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
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

	IntParamDescriptor *paramFreq = defineIntParam(desc, "frequency", "Frequency", "The frequency at which tiles appear and disappear (in 20ths of a second, lower is faster).", 0, 1, FX_FRACTION, 5);
	page->addChild(*paramFreq);
	IntParamDescriptor *paramTile = defineIntParam(desc, "maxTiles", "Maximum # of Tiles", "The maximum amount of tiles that can be displayed on screen at once.", 0, 1, 100, 10);
	page->addChild(*paramTile);
	IntParamDescriptor *paramSeed = defineIntParam(desc, "seed", "Random Seed", "The seed to use for the random number generator.", 0, 0, INT_MAX, INT_MAX / 2);
	page->addChild(*paramSeed);
	BooleanParamDescriptor *paramColor = defineBoolParam(desc, "glitchColors", "Should Glitch Colors", "Whether or not to randomly change the hue of tiles", 0, true);
	page->addChild(*paramColor);

}

ImageEffect* GlitchTilePluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	return new GlitchTilePlugin(handle);
}

