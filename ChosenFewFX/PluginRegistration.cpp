#include "ofxsImageEffect.h"
#include "ManagedPluginFactory.h"
#define __DEF_PLUG(PLUGIN_TYPE, n) \
static ChosenFewFX::ManagedPluginFactory<PLUGIN_TYPE> p##n(gcnew PLUGIN_TYPE()); \
ids.push_back(&p##n);
#define _DEF_PLUG(x, y) __DEF_PLUG(x, y)
#define DEF_PLUG(x) _DEF_PLUG(x, __COUNTER__)

class Loader {
	friend class constructor;

	struct constructor {
		constructor() {
			System::AppDomain^ currentDomain = System::AppDomain::CurrentDomain;
			currentDomain->AssemblyResolve += gcnew System::ResolveEventHandler(LoadFromSameFolder);
		}
	};

	static constructor cons;

	static System::Reflection::Assembly^ LoadFromSameFolder(System::Object^ sender, System::ResolveEventArgs^ args)
	{
		System::String^ folderPath = System::IO::Path::GetDirectoryName(System::Reflection::Assembly::GetExecutingAssembly()->Location);
		System::String^ assemblyPath = System::IO::Path::Combine(folderPath, (gcnew System::Reflection::AssemblyName(args->Name))->Name + ".dll");
		if (!System::IO::File::Exists(assemblyPath)) return nullptr;
		System::Reflection::Assembly^ assembly = System::Reflection::Assembly::LoadFrom(assemblyPath);
		return assembly;
	}
};

// C++ needs to define static members externally.
Loader::constructor Loader::cons;

namespace OFX
{
	namespace Plugin
	{
		void getPluginIDs(OFX::PluginFactoryArray &ids)
		{		
			DEF_PLUG(ChosenFewFX::NET::Plugins::RedFilterPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::JuliaDistortPlugin);
		}
	}
}
