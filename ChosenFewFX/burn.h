#include "ofxsImageEffect.h"

class BurnPluginFactory : public OFX::PluginFactoryHelper<BurnPluginFactory>
{
public:
	BurnPluginFactory() :OFX::PluginFactoryHelper<BurnPluginFactory>("com.chosenfewsoftware.openfx.proceduralfire", 1, 0) {}
	virtual void describe(OFX::ImageEffectDescriptor &desc);
	virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
	virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
};