/*
 *  Copyright 2019 Chosen Few Software
 *  This file is part of Chosen Few FX.
 *
 *  Chosen Few FX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Chosen Few FX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Chosen Few FX.  If not, see <https://www.gnu.org/licenses/>.
 */
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
