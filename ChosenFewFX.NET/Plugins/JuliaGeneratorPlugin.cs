using System.Collections.Generic;
using ChosenFewFX.NET.Fractals;
using ChosenFewFX.NET.Interop;
using MandelbrotSharp.Rendering;

namespace ChosenFewFX.NET.Plugins
{
    public class JuliaGeneratorPlugin : MandelbrotGeneratorPlugin
    {
        public JuliaGeneratorPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.julia";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Julia Generator";
        }

        [RangeParam(DefaultValue = .2, Label = "Real Coordinate", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double RealCoord;

        [RangeParam(DefaultValue = .5, Label = "Imaginary Coordinate", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double ImaginaryCoord;

        protected override void Configure(RenderSettings settings)
        {
            settings.AlgorithmType = typeof(JuliaAlgorithmProvider<>);
            settings.ExtraParams = new Dictionary<string, object>
            {
                { "RealCoord", RealCoord }, { "ImaginaryCoord", ImaginaryCoord }
            };
        }
    }
}
