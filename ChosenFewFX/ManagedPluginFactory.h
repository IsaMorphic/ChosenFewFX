#pragma once
#include <vcclr.h>
#include <msclr\marshal_cppstd.h>

#include "ofxsImageEffect.h"

using namespace msclr::interop;
namespace ChosenFewFX {
	template<class T>
	class ManagedPluginFactory : public OFX::PluginFactoryHelper<T>
	{
	private:
		gcroot<NET::Interop::BasePlugin^> pluginHandle;
	public:
		ManagedPluginFactory(NET::Interop::BasePlugin^ handle) : 
			OFX::PluginFactoryHelper<T>(
				marshal_as<std::string>(handle->Id), 
				handle->MajorVersion, handle->MinorVersion), 
			pluginHandle(handle) {}
		virtual void describe(OFX::ImageEffectDescriptor &desc);
		virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
		virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
	};
}

#include "ManagedPluginFactory.tpp"