#pragma once

template <class GenericProcessor>
class BasicFilterPlugin : public OFX::ImageEffect
{
protected:
	OFX::Clip *dstClip_;
	OFX::Clip *srcClip_;
public:
	BasicFilterPlugin(OfxImageEffectHandle handle) : ImageEffect(handle), dstClip_(0), srcClip_(0)
	{
		dstClip_ = fetchClip(kOfxImageEffectOutputClipName);
		srcClip_ = fetchClip(kOfxImageEffectSimpleSourceClipName);
	}
	virtual void render(const OFX::RenderArguments &args)
	{
		OFX::BitDepthEnum       dstBitDepth = dstClip_->getPixelDepth();
		OFX::PixelComponentEnum dstComponents = dstClip_->getPixelComponents();
		GenericProcessor processor(*this, dstComponents, dstBitDepth);
		transferParams(processor, args);
		setupAndProcess(processor, args);
	}

	virtual void transferParams(GenericProcessor &processor, const OFX::RenderArguments &args)
	{
	}

	virtual bool isIdentity(const OFX::IsIdentityArguments &args, OFX::Clip * &identityClip, double &identityTime)
	{
		return false;
	}
	virtual void changedParam(const OFX::InstanceChangedArgs &/*args*/, const std::string &paramName)
	{

	}
	virtual void changedClip(const OFX::InstanceChangedArgs &/*args*/, const std::string &clipName)
	{
		/*if(clipName == kOfxImageEffectSimpleSourceClipName)
		  setEnabledness();*/
	}
	virtual bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
	{
		rod = srcClip_->getRegionOfDefinition(args.time);
		return true;
	}
	virtual void getRegionsOfInterest(const OFX::RegionsOfInterestArguments &args, OFX::RegionOfInterestSetter &rois)
	{
		rois.setRegionOfInterest(*srcClip_, args.regionOfInterest);
	}
	void setupAndProcess(MagickProcessor &processor, const OFX::RenderArguments &args)
	{
		std::auto_ptr<OFX::Image> dst(dstClip_->fetchImage(args.time));
		OFX::BitDepthEnum dstBitDepth = dst->getPixelDepth();
		OFX::PixelComponentEnum dstComponents = dst->getPixelComponents();
		std::auto_ptr<OFX::Image> src(srcClip_->fetchImage(args.time));

		if (src.get())
		{
			OFX::BitDepthEnum    srcBitDepth = src->getPixelDepth();
			OFX::PixelComponentEnum srcComponents = src->getPixelComponents();
			if (srcBitDepth != dstBitDepth || srcComponents != dstComponents)
				throw int(1);
		}
		processor.setDstImg(dst.get());
		processor.setSrcImg(src.get());
		processor.setRenderWindow(args.renderWindow);
		processor.process();
	}
};
