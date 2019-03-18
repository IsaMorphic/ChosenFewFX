#include "ofxsImageEffect.h"
#include "ofxsMultiThread.h"
#include "../include/ofxsProcessing.H"
#include <Magick++.h>

using namespace OFX;
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
		Magick::StorageType storageType;
		switch (_srcImg->getPixelDepth()) {
		case eBitDepthFloat:
			storageType = FloatPixel;
			break;
		case eBitDepthUByte:
			storageType = CharPixel;
			break;
		case eBitDepthUShort:
			storageType = ShortPixel;
		}
		_managedImage = Magick::Image(width, height, "RGBA", storageType, _srcImg->getPixelData());
	}

	void CopyManagedToDst() {
		OfxRectI bounds = _dstImg->getBounds();
		int width = bounds.x2 - bounds.x1;
		int height = bounds.y2 - bounds.y1;
		Magick::StorageType storageType;
		switch (_srcImg->getPixelDepth()) {
		case eBitDepthFloat:
			storageType = FloatPixel;
			break;
		case eBitDepthUByte:
			storageType = CharPixel;
			break;
		case eBitDepthUShort:
			storageType = ShortPixel;
		}
		_managedImage.write(0, 0, width, height, "RGBA", storageType, _dstImg->getPixelData());
	}

	void preProcess() {
		_managedImage.modifyImage();
	}

	void multiThreadProcessImages(OfxRectI procWindow)
	{
		int width = procWindow.x2 - procWindow.x1;
		int height = procWindow.y2 - procWindow.y1;
		Magick::Quantum *pixels = _managedImage.getPixels(procWindow.x1, procWindow.y1, width, height);
		processPixels(procWindow, pixels);
		_managedImage.syncPixels();
	}

	void postProcess() {
		CopyManagedToDst();
	}

	virtual void processPixels(OfxRectI procWindow, Magick::Quantum *pixels) {

	}
	/** @brief set the src image */
	void setSrcImg(OFX::Image *v) { _srcImg = v; CopySrcToManaged(); }
};
