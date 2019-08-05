#include "FilterPlugin.h"
#include "FilterProcessor.h"

ChosenFewFX::FilterPlugin::FilterPlugin(OfxImageEffectHandle handle, NET::Interop::BasePlugin^ plugin) :
	BasePlugin(handle, plugin),
	pluginHandle((NET::Interop::FilterPlugin^)plugin),
	srcClip_(0)
{
	srcClip_ = fetchClip(kOfxImageEffectSimpleSourceClipName);
}

void ChosenFewFX::FilterPlugin::render(const OFX::RenderArguments &args)
{
	FilterProcessor processor(*this, pluginHandle);

	std::auto_ptr<OFX::Image> src(srcClip_->fetchImage(args.time));
	std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));

	processor.setSrcImg(src.get());
	processor.setDstImg(dst.get());

	processor.setRenderWindow(args.renderWindow);
	transferParams(args.time);

	processor.process();
}
