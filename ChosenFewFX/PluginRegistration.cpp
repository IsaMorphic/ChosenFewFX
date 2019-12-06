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
#include "ofxsImageEffect.h"
#include "ManagedPluginFactory.h"
#define __DEF_PLUG(PLUGIN_TYPE, n) \
static ChosenFewFX::ManagedPluginFactory<PLUGIN_TYPE> p##n(gcnew PLUGIN_TYPE()); \
ids.push_back(&p##n);
#define _DEF_PLUG(x, y) __DEF_PLUG(x, y)
#define DEF_PLUG(x) _DEF_PLUG(x, __COUNTER__)

namespace OFX
{
	namespace Plugin
	{
		void getPluginIDs(OFX::PluginFactoryArray &ids)
		{
			DEF_PLUG(ChosenFewFX::NET::Plugins::JuliaDistortPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::WaveFormGeneratorPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::SoftEnginePlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::MandelbrotGeneratorPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::JuliaGeneratorPlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::RaptorRealtimePlugin);
			DEF_PLUG(ChosenFewFX::NET::Plugins::RaptorScriptingPlugin);
		}
	}
}
