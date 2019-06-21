using System;

namespace ChosenFewFX.NET.Interop
{
    [AttributeUsage(AttributeTargets.Field)]
    public class ParamAttribute : Attribute
    {
        public string Label;
        public object DefaultValue;
    }
}
