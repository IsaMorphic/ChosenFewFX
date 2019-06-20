using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChosenFewFX.NET
{
    [AttributeUsage(AttributeTargets.Field)]
    public class RangeParamAttribute : ParamAttribute
    {
        public object MinimumValue;
        public object MaximumValue;
    }
}
