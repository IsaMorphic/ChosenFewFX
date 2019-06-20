#pragma once
#include "BasePlugin.h"
namespace ChosenFewFX {
	class FilterPlugin : public BasePlugin
	{
	private:
		gcroot<NET::FilterPlugin^> pluginHandle;
	public:
		FilterPlugin(OfxImageEffectHandle handle, NET::BasePlugin^ plugin) :
			BasePlugin(handle, plugin), pluginHandle((NET::FilterPlugin^)plugin) {}
		void render(const OFX::RenderArguments &args);
	};
}
