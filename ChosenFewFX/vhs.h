#include "ofxsImageEffect.h"

class VHSPluginFactory : public OFX::PluginFactoryHelper<VHSPluginFactory>
{
public:
	VHSPluginFactory() :OFX::PluginFactoryHelper<VHSPluginFactory>("com.chosenfewsoftware.openfx.vhs", 1, 0) {}
	virtual void describe(OFX::ImageEffectDescriptor &desc);
	virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
	virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
};
