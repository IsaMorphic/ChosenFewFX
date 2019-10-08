#pragma once
#include <vcclr.h>
#include "ofxsMultiThread.h"
#include "ofxsProcessing.H"
namespace ChosenFewFX {
	class BaseProcessor : public OFX::ImageProcessor {
	private:
		gcroot<NET::Interop::BasePlugin^> _managedHandle;
	public:
		BaseProcessor(OFX::ImageEffect &instance, NET::Interop::BasePlugin^ handle) : 
			OFX::ImageProcessor(instance), _managedHandle(handle) {}

		NET::Interop::ImageWrapper^ linkPixelsToManagedImage(OFX::Image *img, NET::Interop::ImageType imageType);

		void setCurrentTime(OfxTime time);

		virtual void preProcess();
		virtual void multiThreadProcessImages(OfxRectI procWindow);
		virtual void postProcess();
	};
}
