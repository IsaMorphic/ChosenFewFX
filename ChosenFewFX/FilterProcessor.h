#pragma once
#include "GeneratorProcessor.h"
namespace ChosenFewFX {
	class FilterProcessor : public GeneratorProcessor {
	private:
		gcroot<NET::FilterPlugin^> _managedHandle;
		OFX::Image *_srcImg;
	public:
		FilterProcessor(OFX::ImageEffect &instance, NET::FilterPlugin^ handle) :
			GeneratorProcessor(instance, handle), _srcImg(0),
			_managedHandle(handle) {}
		void preProcess();
		void setSrcImg(OFX::Image *v);
	};
}