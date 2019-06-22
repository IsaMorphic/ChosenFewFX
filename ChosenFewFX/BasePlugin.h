#pragma once
#include <vector>
#include "BaseProcessor.h"
#include "ofxsImageEffect.h"

#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;
using namespace System::Collections::Generic;
namespace ChosenFewFX {
	class BasePlugin : public OFX::ImageEffect
	{
	private:
		gcroot<NET::Interop::BasePlugin^> pluginHandle;
		gcroot<List<System::Reflection::FieldInfo^>^> paramFields;
	protected:
		OFX::Clip *srcClip_;
		OFX::Clip *dstClip_;
		void transferParams(OfxTime time);
	public:
		BasePlugin(OfxImageEffectHandle handle, NET::Interop::BasePlugin^ plugin);

		virtual void render(const OFX::RenderArguments &args);

		virtual bool isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime);

		virtual bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod);

		virtual void getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois);
	};
}