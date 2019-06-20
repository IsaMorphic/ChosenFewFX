#pragma once
#include <vcclr.h>
#include "ofxsMultiThread.h"
#include "../include/ofxsProcessing.H"
namespace ChosenFewFX {
	class GeneratorProcessor : public OFX::ImageProcessor {
	private:
		gcroot<NET::Plugin^> _managedHandle;
	public:
		GeneratorProcessor(OFX::ImageEffect &instance, NET::Plugin^ handle) : 
			OFX::ImageProcessor(instance), _managedHandle(handle) {}

		SkiaSharp::SKBitmap^ linkPixelsToManagedImage(OFX::Image *img);

		virtual void preProcess();
		virtual void multiThreadProcessImages(OfxRectI procWindow);
		virtual void postProcess();
	};
}
