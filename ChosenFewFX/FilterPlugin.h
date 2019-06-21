#pragma once
#include "BasePlugin.h"
namespace ChosenFewFX {
	class FilterPlugin : public BasePlugin
	{
	private:
		gcroot<NET::Interop::FilterPlugin^> pluginHandle;
	public:
		FilterPlugin(OfxImageEffectHandle handle, NET::Interop::BasePlugin^ plugin) :
			BasePlugin(handle, plugin), pluginHandle((NET::Interop::FilterPlugin^)plugin) {}
		void render(const OFX::RenderArguments &args);
	};
}
