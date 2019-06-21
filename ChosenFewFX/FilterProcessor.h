#pragma once
#include "BaseProcessor.h"
namespace ChosenFewFX {
	class FilterProcessor : public BaseProcessor {
	private:
		gcroot<NET::Interop::FilterPlugin^> _managedHandle;
		OFX::Image *_srcImg;
	public:
		FilterProcessor(OFX::ImageEffect &instance, NET::Interop::FilterPlugin^ handle) :
			BaseProcessor(instance, handle), _srcImg(0),
			_managedHandle(handle) {}
		void preProcess();
		void setSrcImg(OFX::Image *v);
	};
}