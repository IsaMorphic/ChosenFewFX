#include "ofxsImageEffect.h"
#include "ManagedPluginFactory.h"
#define __DEF_PLUG(PLUGIN_TYPE, n) \
static ChosenFewFX::ManagedPluginFactory<PLUGIN_TYPE> p##n(gcnew PLUGIN_TYPE()); \
ids.push_back(&p##n);
#define _DEF_PLUG(x, y) __DEF_PLUG(x, y)
#define DEF_PLUG(x) _DEF_PLUG(x, __COUNTER__)

namespace OFX
{
	namespace Plugin
	{
		void getPluginIDs(OFX::PluginFactoryArray &ids)
		{
			DEF_PLUG(ChosenFewFX::NET::Plugins::JuliaDistortPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::WaveFormGeneratorPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::SoftEnginePlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::MandelbrotGeneratorPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::JuliaGeneratorPlugin);
		}
	}
}
