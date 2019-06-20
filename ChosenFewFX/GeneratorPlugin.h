#pragma once
#include <vector>
#include "GeneratorProcessor.h"
#include "ofxsImageEffect.h"

#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;
using namespace System::Collections::Generic;
namespace ChosenFewFX {
	class GeneratorPlugin : public OFX::ImageEffect
	{
	private:
		gcroot<NET::Plugin^> pluginHandle;
		gcroot<List<System::Reflection::FieldInfo^>^> paramFields;
	protected:
		OFX::Clip *srcClip_;
		OFX::Clip *dstClip_;
		void transferParams(const OFX::RenderArguments &args);
	public:
		GeneratorPlugin(OfxImageEffectHandle handle, NET::Plugin^ plugin) : 
			OFX::ImageEffect(handle), dstClip_(0), srcClip_(0),
			pluginHandle(plugin), paramFields(gcnew List<System::Reflection::FieldInfo^>)
		{
			srcClip_ = fetchClip(kOfxImageEffectSimpleSourceClipName);
			dstClip_ = fetchClip(kOfxImageEffectOutputClipName);

			System::Type^ pluginType = pluginHandle->GetType();
			auto fields = pluginType->GetFields();
			for each(System::Reflection::FieldInfo^ field in fields)
			{
				auto paramAttrs = field->GetCustomAttributes(NET::ParamAttribute::typeid, false);
				if (paramAttrs->Length == 1)
					paramFields->Add(field);
			}
		}
		virtual void render(const OFX::RenderArguments &args);

		virtual bool isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime);

		virtual bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod);

		virtual void getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois);
	};
}