/*
OFX Gamma Example plugin, a plugin that illustrates the use of the OFX Support library.

Copyright (C) 2004-2005 The Open Effects Association Ltd
Author Bruno Nicoletti bruno@thefoundry.co.uk

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name The Open Effects Association Ltd, nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The Open Effects Association Ltd
1 Wardour St
London W1D 6PA
England


*/

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
#include "PerlinNoise.h"
#include "FloatImage.h"
#include "../include/ofxsProcessing.H"

#include "burn.h"

struct Circle {
	float _centerX;
	float _centerY;
	float _radius;
};

struct BurnSpot : Circle {
	float _growthRate;
	int _spawnTime;
	BurnSpot(float x, float y, int spawnTime) {
		_growthRate = std::rand() % 10 + 5;
		_spawnTime = spawnTime;
		_centerX = x;
		_centerY = y;
		_radius = 0;
	}
};

class Fire {
private:
	FloatImage * _buffer;
	FloatImage * _coolingMap;
	PerlinNoise _noise;
	int _width = 0;
	int _height = 0;
	float _scroll = 0;
	float _scrollFactor = 0;
	float _falloff = 0;
public:
	FloatImage * _bufferOut;
	Fire(int width, int height, float scrollFactor = 1.0f, float falloff = .04f) {
		_width = width;
		_height = height;
		_scrollFactor = scrollFactor;
		_falloff = falloff;
		_coolingMap = new FloatImage(_width, _height);
		_buffer = new FloatImage(_width, _height);
		_bufferOut = new FloatImage(_width, _height);
	}
	void NextFrame() {
		float* srcPtr = _bufferOut->GetPixel(0, 0);
		float* dstPtr = _buffer->GetPixel(0, 0);
		memcpy(dstPtr, srcPtr, sizeof(float) * 4 * _width * _height);

		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				double dx = (float)x / (float)_width;
				double dy = (float)y / (float)_height;
				_coolingMap->SetPixel(x, y, _noise.noise(dx * 50, (dy + _scroll) * 50, 0.5) * _falloff);
			}
		}
		for (int x = 1; x < _width - 1; x++) {
			for (int y = _height - 1; y > 1; y--) {
				float top = *_buffer->GetPixel(x, y + 1);
				float bottom = *_buffer->GetPixel(x, y - 1);
				float left = *_buffer->GetPixel(x - 1, y);
				float right = *_buffer->GetPixel(x + 1, y);

				float c = *_coolingMap->GetPixel(x, y);

				float p = (top + bottom + left + right) / 4;
				p = (std::max)(p - c, 0.0f);

				_bufferOut->SetPixel(x, y + 1, p);
			}
		}
		_scroll += _scrollFactor;
	}
};

class BurnAccumulator {
private:
	Fire * _fire;
	int _width = 0;
	int _height = 0;
	int frameCount;
	float _burnMultiplier;
	FloatImage * _buffer;
	std::vector<BurnSpot*> spots;
public:
	FloatImage * _bufferOut;
	BurnAccumulator(int width, int height, float burnMultiplier = .05f) {
		_width = width;
		_height = height;
		_burnMultiplier = burnMultiplier;
		_buffer = new FloatImage(_width, _height);
		_bufferOut = new FloatImage(_width, _height);
		_fire = new Fire(_width, _height);
	}

	void Cleanup() {
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				_fire->_bufferOut->SetPixel(x, y, 0.0f);
				_bufferOut->SetPixel(x, y, 0.0f);
			}
		}
		for (int i = 0; i < spots.size(); i++) {
			delete spots[i];
		}
		spots.clear();
	}

	void NextFrame() {
		if (frameCount % 10 == 0) {
			float x = std::rand() % _width;
			float y = std::rand() % _height;
			bool insideSpot = false;
			for (int i = 0; i < spots.size(); i++) {
				BurnSpot* spot = spots[i];
				if (std::hypotf(x - spot->_centerX, y - spot->_centerY) < spot->_radius * .85) {
					insideSpot = true;
					break;
				}
			}
			if (!insideSpot) {
				BurnSpot* spot = new BurnSpot(
					x,
					y,
					frameCount);
				spots.push_back(spot);
			}
		}
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				for (int i = 0; i < spots.size(); i++) {
					BurnSpot* spot = spots[i];
					if (std::hypotf(x - spot->_centerX, y - spot->_centerY) < spot->_radius)
						_fire->_bufferOut->SetPixel(x, y, 0.6f);
					spot->_radius = spot->_growthRate * log((float)frameCount - spot->_spawnTime);
				}
			}
		}
		float* srcPtr = _bufferOut->GetPixel(0, 0);
		float* dstPtr = _buffer->GetPixel(0, 0);
		memcpy(dstPtr, srcPtr, sizeof(float) * 4 * _width * _height);
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				float burnVal = *_fire->_bufferOut->GetPixel(x, y);
				float srcVal = *_buffer->GetPixel(x, y);
				float dstVal = (std::min)(srcVal + burnVal * _burnMultiplier, 1.0f);
				_bufferOut->SetPixel(x, y, dstVal);
			}
		}
		_fire->NextFrame();
		frameCount++;
	}
};

class BurningBase : public OFX::ImageProcessor {
protected:
	OFX::Image *_srcImg;
	FloatImage *_burnMap;
public:
	/** @brief no arg ctor */
	BurningBase(OFX::ImageEffect &instance)
		: OFX::ImageProcessor(instance)
		, _srcImg(0)
		, _burnMap(0)
	{
	}

	/** @brief set the src image */
	void setSrcImg(OFX::Image *v) { _srcImg = v; }
	void setBurnMap(FloatImage *v) { _burnMap = v; }
};

template <class PIX, int nComponents, int max>
class BurningFilter : public BurningBase {
public:
	// ctor
	BurningFilter(OFX::ImageEffect &instance)
		: BurningBase(instance)
	{}

	// and do some processing
	void multiThreadProcessImages(OfxRectI procWindow)
	{
		for (int y = procWindow.y1; y < procWindow.y2; y++) {
			if (_effect.abort()) break;

			PIX *dstPix = (PIX *)_dstImg->getPixelAddress(procWindow.x1, y);

			for (int x = procWindow.x1; x < procWindow.x2; x++) {

				PIX *srcPix = (PIX *)(_srcImg ? _srcImg->getPixelAddress(x, y) : 0);

				if (srcPix) {

					float* bufferPix = _burnMap->GetPixel(x, y);
					for (int c = 0; c < nComponents; c++) {
						float val = (bufferPix[c] * max);
						dstPix[c] = (srcPix[c] < val) ? 0 : srcPix[c] - (PIX)val;
					}
				}
				// increment the dst pixel
				dstPix += nComponents;
			}
		}
	}
};

class BurnPlugin : public OFX::ImageEffect
{
protected:
	OFX::Clip *dstClip_;
	OFX::Clip *srcClip_;

	OFX::IntParam  *frequency_;
	OFX::IntParam  *maxTiles_;
	OFX::IntParam  *seed_;
	OFX::BooleanParam *glitchColors_;

	OfxTime prevTime = -1;

	BurnAccumulator *burnAccumulator_;

public:
	BurnPlugin(OfxImageEffectHandle handle) : ImageEffect(handle), dstClip_(0), srcClip_(0), frequency_(0), maxTiles_(0), seed_(0), glitchColors_(0)
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
	void setupAndProcess(BurningBase&, const OFX::RenderArguments &args);
};

void BurnPlugin::setupAndProcess(BurningBase &processor, const OFX::RenderArguments &args)
{
	int maxTiles, frequency;
	maxTiles = maxTiles_->getValueAtTime(args.time);
	frequency = frequency_->getValueAtTime(args.time);
	bool glitchColors = glitchColors_->getValue();

	int frameWidth = args.renderWindow.x2 - args.renderWindow.x1;
	int frameHeight = args.renderWindow.y2 - args.renderWindow.y1;

	if (prevTime < 0) {
		burnAccumulator_ = new BurnAccumulator(frameWidth, frameHeight);
	}
	if (prevTime < args.time) {
		burnAccumulator_->NextFrame();
	}
	else
	{
		burnAccumulator_->Cleanup();
	}
	prevTime = args.time;


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
	processor.setBurnMap(burnAccumulator_->_bufferOut);
	processor.setRenderWindow(args.renderWindow);
	processor.process();
}

bool BurnPlugin::getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
{
	rod = srcClip_->getRegionOfDefinition(args.time);
	return true;
}

void BurnPlugin::getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois)
{
	rois.setRegionOfInterest(*srcClip_, args.regionOfInterest);
}

void BurnPlugin::render(const OFX::RenderArguments &args)
{
	OFX::BitDepthEnum       dstBitDepth = dstClip_->getPixelDepth();
	OFX::PixelComponentEnum dstComponents = dstClip_->getPixelComponents();
	if (dstComponents == OFX::ePixelComponentRGBA)
	{
		switch (dstBitDepth)
		{
		case OFX::eBitDepthUByte:
		{
			BurningFilter<unsigned char, 4, 255> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthUShort:
		{
			BurningFilter<unsigned short, 4, 65535> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthFloat:
		{
			BurningFilter<float, 4, 1> fred(*this);
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
			BurningFilter<unsigned char, 1, 255> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthUShort:
		{
			BurningFilter<unsigned short, 1, 65535> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		case OFX::eBitDepthFloat:
		{
			BurningFilter<float, 1, 1> fred(*this);
			setupAndProcess(fred, args);
			break;
		}
		default:
			OFX::throwSuiteStatusException(kOfxStatErrUnsupported);
		}
	}
}

bool BurnPlugin::isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime)
{
	return false;
}

void BurnPlugin::changedParam(const OFX::InstanceChangedArgs &/*args*/, const std::string &paramName)
{
	if (paramName == "seed") {
		int seed = seed_->getValue();
		std::srand(seed);
	}
}

void BurnPlugin::changedClip(const OFX::InstanceChangedArgs &/*args*/, const std::string &clipName)
{
	/*if(clipName == kOfxImageEffectSimpleSourceClipName)
	  setEnabledness();*/
}

using namespace OFX;

void BurnPluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
	desc.setLabels("Hell", "Hell", "Hell");
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

void BurnPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context)
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

ImageEffect* BurnPluginFactory::createInstance(OfxImageEffectHandle handle, OFX::ContextEnum /*context*/)
{
	return new BurnPlugin(handle);
}

