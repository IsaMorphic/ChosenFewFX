#include "Plugin.h"

void ChosenFewFX::Plugin::render(const OFX::RenderArguments &args)
{
	ManagedProcessor processor(*this, pluginHandle);
	transferParams(args);
	setup(processor, args);
}

void ChosenFewFX::Plugin::setup(ManagedProcessor &processor, const OFX::RenderArguments &args) 
{
	processor.setRenderWindow(args.renderWindow);

	std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
	processor.setDstImg(dst.get());
}

bool ChosenFewFX::Plugin::isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime)
{
	return false;
}

void ChosenFewFX::Plugin::transferParams(const OFX::RenderArguments &args) 
{

}

void ChosenFewFX::Plugin::getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois)
{
	rois.setRegionOfInterest(*dstClip_, args.regionOfInterest);
}

bool ChosenFewFX::Plugin::getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
{
	rod = dstClip_->getRegionOfDefinition(args.time);
	return true;
}