#include <stdio.h>
#include "ofxsImageEffect.h"
#include "vhs.h"
namespace OFX 
{
  namespace Plugin 
  {
    void getPluginIDs(OFX::PluginFactoryArray &ids)
    {
      static VHSPluginFactory p1;
      ids.push_back(&p1);
    }
  }
}
