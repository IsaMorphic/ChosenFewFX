#include <stdio.h>
#include "ofxsImageEffect.h"
#include "glitchtile.h"
#include "burn.h"
namespace OFX 
{
  namespace Plugin 
  {
    void getPluginIDs(OFX::PluginFactoryArray &ids)
    {
      static GlitchTilePluginFactory p1;
      ids.push_back(&p1);
	  static BurnPluginFactory p2;
	  ids.push_back(&p2);
    }
  }
}
