#include "ofxsImageEffect.h"
#include "ManagedPluginFactory.h"
namespace OFX
{
	namespace Plugin
	{
		void getPluginIDs(OFX::PluginFactoryArray &ids)
		{
			System::Type^ pluginType = ChosenFewFX::NET::BasePlugin::typeid;
			System::Reflection::Assembly^ managedAssembly = 
				System::Reflection::Assembly::GetAssembly(pluginType);
			auto types = managedAssembly->GetTypes();
			for each (System::Type^ type in types)
			{
				if (type->IsSubclassOf(pluginType) && type->Namespace == "ChosenFewFX.NET.Plugins") 
				{
					System::Object^ pluginHandle = System::Activator::CreateInstance(type);
					static ChosenFewFX::ManagedPluginFactory p((ChosenFewFX::NET::BasePlugin^)pluginHandle);
					ids.push_back(&p);
				}
			}
		}
	}
}
