using System;

namespace ChosenFewFX.NET.Interop
{
    [AttributeUsage(AttributeTargets.Field)]
    public class ParamAttribute : Attribute
    {
        public string Label;
        public string Hint = "Nothing to see here...";
        public object DefaultValue;        
    }
}
