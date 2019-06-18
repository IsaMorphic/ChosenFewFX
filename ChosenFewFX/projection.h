#pragma once
class ProjectionPluginFactory : public OFX::PluginFactoryHelper<ProjectionPluginFactory>
{
public:
	ProjectionPluginFactory() :OFX::PluginFactoryHelper<ProjectionPluginFactory>("com.chosenfewsoftware.openfx.projection", 1, 0) {}
	virtual void describe(OFX::ImageEffectDescriptor &desc);
	virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
	virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
};
