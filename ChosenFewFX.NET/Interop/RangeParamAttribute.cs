using System;

namespace ChosenFewFX.NET.Interop
{
    [AttributeUsage(AttributeTargets.Field)]
    public class RangeParamAttribute : ParamAttribute
    {
        public object MinimumValue;
        public object MaximumValue;
    }
}
