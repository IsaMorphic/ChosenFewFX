#include "Plugin.h"
#include <iostream>

void ChosenFewFX::Plugin::render(const OFX::RenderArguments &args)
{
	ManagedProcessor processor(*this, (ChosenFewFX::NET::Plugin ^)pluginHandle);
	transferParams(args);
	setup(processor, args);
}

void ChosenFewFX::Plugin::setup(ManagedProcessor &processor, const OFX::RenderArguments &args) 
{
	std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
	std::auto_ptr<OFX::Image> src(srcClip_->fetchImage(args.time));
	processor.setRenderWindow(args.renderWindow);
	processor.setDstImg(dst.get());
	processor.setSrcImg(src.get());
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