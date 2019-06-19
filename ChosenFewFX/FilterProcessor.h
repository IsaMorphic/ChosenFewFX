#pragma once
#include "ManagedProcessor.h"
namespace ChosenFewFX {
	class FilterProcessor : public ManagedProcessor {
	private:
		gcroot<NET::FilterPlugin^> _managedHandle;
		OFX::Image *_srcImg;
	public:
		FilterProcessor(OFX::ImageEffect &instance, NET::FilterPlugin^ handle) :
			ManagedProcessor(instance, handle), _srcImg(0),
			_managedHandle(handle) {}
		void preProcess();
		void setSrcImg(OFX::Image *v);
	};
}