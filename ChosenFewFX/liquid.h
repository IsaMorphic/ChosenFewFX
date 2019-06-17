#pragma once
class LiquidPluginFactory : public OFX::PluginFactoryHelper<LiquidPluginFactory>
{
public:
	LiquidPluginFactory() :OFX::PluginFactoryHelper<LiquidPluginFactory>("com.chosenfewsoftware.openfx.liquid", 1, 0) {}
	virtual void describe(OFX::ImageEffectDescriptor &desc);
	virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
	virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
};