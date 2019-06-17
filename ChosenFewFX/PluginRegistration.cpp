#include <stdio.h>
#include "ofxsImageEffect.h"
#include <Magick++.h>
#include "liquid.h"
#include "vhs.h"
namespace OFX
{
	namespace Plugin
	{
		void getPluginIDs(OFX::PluginFactoryArray &ids)
		{
			static VHSPluginFactory p1;
			ids.push_back(&p1);
			static LiquidPluginFactory p2;
			ids.push_back(&p2);
		}
	}
}
