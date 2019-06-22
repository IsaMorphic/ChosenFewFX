using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChosenFewFX.NET.Interop
{
    public enum StringType
    {
        SingleLine = 0,
        MultiLine = 1,
        FilePath = 2,
        DirectoryPath = 3
    }
    public class StringParamAttribute : ParamAttribute
    {
        public StringType StringType;
    }
}
