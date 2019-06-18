#pragma once
#include "ManagedProcessor.h"
namespace ChosenFewFX {
	class FilterProcessor : public ManagedProcessor
	{
	private:
		OFX::Image *_srcImg;
		gcroot<NET::FilterPlugin^> _managedHandle;
		void preProcess() override;
	public:
		FilterProcessor(OFX::ImageEffect &instance, NET::FilterPlugin^ handle) :
			ManagedProcessor(instance, handle), _managedHandle(handle) {}
		void setSrcImg(OFX::Image *v);
	};
}

