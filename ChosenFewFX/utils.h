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
	param->setEvaluateOnChange(false);
	param->setFilePathExists(false);
	param->setIsPersistant(false);
	if (parent)
		param->setParent(*parent);
	return param;
}