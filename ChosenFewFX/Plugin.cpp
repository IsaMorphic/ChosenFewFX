#include "Plugin.h"

void ChosenFewFX::Plugin::render(const OFX::RenderArguments &args)
{
	ManagedProcessor processor(*this, pluginHandle);
	std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
	processor.setDstImg(dst.get());
	processor.setRenderWindow(args.renderWindow);
	transferParams(args);
	processor.process();
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
	rois.setRegionOfInterest(*srcClip_, args.regionOfInterest);
}

bool ChosenFewFX::Plugin::getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
{
	rod = srcClip_->getRegionOfDefinition(args.time);
    return true;
}