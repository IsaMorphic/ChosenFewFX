#include "ofxsImageEffect.h"

class GlitchTilePluginFactory : public OFX::PluginFactoryHelper<GlitchTilePluginFactory>
{
public:
	GlitchTilePluginFactory() :OFX::PluginFactoryHelper<GlitchTilePluginFactory>("com.chosenfewsoftware.openfx.glitchtile", 1, 0) {}
	virtual void describe(OFX::ImageEffectDescriptor &desc);
	virtual void describeInContext(OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context);
	virtual OFX::ImageEffect* createInstance(OfxImageEffectHandle handle, OFX::ContextEnum context);
};