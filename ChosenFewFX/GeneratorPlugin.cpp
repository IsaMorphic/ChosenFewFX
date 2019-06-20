#include "GeneratorPlugin.h"

void ChosenFewFX::GeneratorPlugin::render(const OFX::RenderArguments &args)
{
	GeneratorProcessor processor(*this, pluginHandle);
	std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
	processor.setDstImg(dst.get());
	processor.setRenderWindow(args.renderWindow);
	transferParams(args);
	processor.process();
}

bool ChosenFewFX::GeneratorPlugin::isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime)
{
	return false;
}

void ChosenFewFX::GeneratorPlugin::transferParams(const OFX::RenderArguments &args)
{
	for each (System::Reflection::FieldInfo^ field in (List<System::Reflection::FieldInfo^>^)paramFields)
	{
		std::string name = marshal_as<std::string>(field->Name);
		if (field->FieldType == System::Boolean::typeid)
			field->SetValue(pluginHandle, fetchBooleanParam(name)->getValueAtTime(args.time));

		else if (field->FieldType == System::Int32::typeid)
			field->SetValue(pluginHandle, fetchIntParam(name)->getValueAtTime(args.time));

		else if (field->FieldType == System::Double::typeid)
			field->SetValue(pluginHandle, fetchDoubleParam(name)->getValueAtTime(args.time));
	}
}

void ChosenFewFX::GeneratorPlugin::getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois)
{
	rois.setRegionOfInterest(*srcClip_, args.regionOfInterest);
}

bool ChosenFewFX::GeneratorPlugin::getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
{
	rod = srcClip_->getRegionOfDefinition(args.time);
	return true;
}