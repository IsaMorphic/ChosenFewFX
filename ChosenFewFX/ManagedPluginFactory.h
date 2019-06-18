#pragma once
#include <vcclr.h>
#include <msclr\marshal_cppstd.h>

#include "ofxsImageEffect.h"

using namespace msclr::interop;
namespace ChosenFewFX {
	class ManagedPluginFactory : public OFX::PluginFactoryHelper<ManagedPluginFactory>
	{
	private:
		gcroot<NET::Plugin^> pluginHandle;
	public:
		ManagedPluginFactory(NET::Plugin^ handle) : 
			OFX::PluginFactoryHelper<ManagedPluginFactory>(
				marshal_as<std::string>(handle->Id), 
				handle->MajorVersion, handle->MinorVersion), 
			pluginHandle(handle) {}
		virtual void describe(OFX::ImageEffectDescriptor &desc);
		virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
		virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
	};
}