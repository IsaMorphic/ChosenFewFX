#pragma once
#include "BasePlugin.h"
namespace ChosenFewFX {
	class FilterPlugin : public BasePlugin
	{
	private:
		gcroot<NET::Interop::FilterPlugin^> pluginHandle;
		OFX::Clip *srcClip_;
	public:
		FilterPlugin(OfxImageEffectHandle handle, NET::Interop::BasePlugin^ plugin);
		void render(const OFX::RenderArguments &args);
	};
}
