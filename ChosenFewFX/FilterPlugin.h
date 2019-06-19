#pragma once
#include "Plugin.h"
namespace ChosenFewFX {
	class FilterPlugin : public Plugin
	{
	private:
		gcroot<NET::FilterPlugin^> pluginHandle;
	public:
		FilterPlugin(OfxImageEffectHandle handle, NET::Plugin^ plugin) :
			Plugin(handle, plugin), pluginHandle((NET::FilterPlugin^)plugin) {}
		void render(const OFX::RenderArguments &args);
	};
}
