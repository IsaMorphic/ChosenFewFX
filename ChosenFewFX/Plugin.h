#pragma once
#include "ManagedProcessor.h"
#include "ofxsImageEffect.h"
namespace ChosenFewFX {
	class Plugin : public OFX::ImageEffect
	{
	private:
		OFX::Clip *srcClip_;
		OFX::Clip *dstClip_;
		gcroot<NET::Plugin^> pluginHandle;
		void transferParams(const OFX::RenderArguments &args);
	public:
		Plugin(OfxImageEffectHandle handle, NET::Plugin^ plugin) : 
			OFX::ImageEffect(handle), dstClip_(0), srcClip_(0),
			pluginHandle(plugin)
		{
			srcClip_ = fetchClip(kOfxImageEffectSimpleSourceClipName);
			dstClip_ = fetchClip(kOfxImageEffectOutputClipName);
		}
		virtual void render(const OFX::RenderArguments &args);

		virtual void setup(ManagedProcessor &processor, const OFX::RenderArguments &args);

		virtual bool isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime);

		virtual bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod);

		virtual void getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois);
	};
}