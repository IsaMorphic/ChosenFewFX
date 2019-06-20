#pragma once
#include "GeneratorPlugin.h"
namespace ChosenFewFX {
	class FilterPlugin : public GeneratorPlugin
	{
	private:
		gcroot<NET::FilterPlugin^> pluginHandle;
	public:
		FilterPlugin(OfxImageEffectHandle handle, NET::Plugin^ plugin) :
			GeneratorPlugin(handle, plugin), pluginHandle((NET::FilterPlugin^)plugin) {}
		void render(const OFX::RenderArguments &args);
	};
}
