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
#pragma once
static
IntParamDescriptor *defineIntParam(OFX::ImageEffectDescriptor &desc,
	const std::string &name, const std::string &label, const std::string &hint,
	GroupParamDescriptor *parent, int min, int max, int def)
{
	IntParamDescriptor *param = desc.defineIntParam(name);
	param->setLabels(label, label, label);
	param->setScriptName(name);
	param->setHint(hint);
	param->setDefault(def);
	param->setRange(min, max);
	param->setDisplayRange(min, max);
	param->setAnimates(false);
	if (parent)
		param->setParent(*parent);
	return param;
}

static
DoubleParamDescriptor *defineDoubleParam(OFX::ImageEffectDescriptor &desc,
	const std::string &name, const std::string &label, const std::string &hint,
	GroupParamDescriptor *parent, double min, double max, double def)
{
	DoubleParamDescriptor *param = desc.defineDoubleParam(name);
	param->setLabels(label, label, label);
	param->setScriptName(name);
	param->setHint(hint);
	param->setDefault(def);
	param->setRange(min, max);
	param->setDisplayRange(min, max);
	if (parent)
		param->setParent(*parent);
	return param;
}

static
BooleanParamDescriptor *defineBoolParam(OFX::ImageEffectDescriptor &desc,
	const std::string &name, const std::string &label, const std::string &hint,
	GroupParamDescriptor *parent, bool def)
{
	BooleanParamDescriptor *param = desc.defineBooleanParam(name);
	param->setLabels(label, label, label);
	param->setScriptName(name);
	param->setHint(hint);
	param->setDefault(def);
	param->setAnimates(false);
	if (parent)
		param->setParent(*parent);
	return param;
}

static
StringParamDescriptor *defineStringParam(OFX::ImageEffectDescriptor &desc,
	const std::string &name, const std::string &label, const std::string &hint,
	GroupParamDescriptor *parent, const std::string &def, OFX::StringTypeEnum type)
{
	StringParamDescriptor *param = desc.defineStringParam(name);
	param->setLabels(label, label, label);
	param->setScriptName(name);
	param->setHint(hint);
	param->setDefault(def);
	param->setStringType(type);
	param->setAnimates(false);
	if (parent)
		param->setParent(*parent);
	return param;
}

static
RGBAParamDescriptor *defineColorParam(OFX::ImageEffectDescriptor &desc,
	const std::string &name, const std::string &label, const std::string &hint,
	GroupParamDescriptor *parent, ChosenFewFX::NET::Interop::Color def)
{
	RGBAParamDescriptor *param = desc.defineRGBAParam(name);
	param->setLabels(label, label, label);
	param->setScriptName(name);
	param->setHint(hint);
	param->setDefault((double)def.Red / 255, (double)def.Green / 255, (double)def.Blue / 255, (double)def.Alpha / 255);
	if (parent)
		param->setParent(*parent);
	return param;
}