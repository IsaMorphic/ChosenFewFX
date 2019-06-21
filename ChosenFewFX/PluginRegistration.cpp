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
			DEF_PLUG(ChosenFewFX::NET::Plugins::RedFilterPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::JuliaDistortPlugin);
			/*System::Type^ pluginType = ChosenFewFX::NET::BasePlugin::typeid;
			System::Reflection::Assembly^ managedAssembly = 
				System::Reflection::Assembly::GetAssembly(pluginType);
			auto types = managedAssembly->GetTypes();
			for each (System::Type^ type in types)
			{
				if (type->IsSubclassOf(pluginType) && type->Namespace == "ChosenFewFX.NET.Plugins") 
				{
					System::Object^ pluginHandle = System::Activator::CreateInstance(type);
					ids.push_back(new ChosenFewFX::ManagedPluginFactory((ChosenFewFX::NET::BasePlugin^)pluginHandle));
				}
			}*/
		}
	}
}
