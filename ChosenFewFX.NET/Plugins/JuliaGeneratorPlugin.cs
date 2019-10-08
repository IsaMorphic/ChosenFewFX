using System;
using System.Collections.Generic;
using ChosenFewFX.NET.Fractals;
using ChosenFewFX.NET.Interop;
using MandelbrotSharp.Algorithms;
using MandelbrotSharp.Numerics;
using MandelbrotSharp.Rendering;

namespace ChosenFewFX.NET.Plugins
{
    public class JuliaGeneratorPlugin : FractalGeneratorPlugin<JuliaAlgorithmProvider<double>>
    {
        public JuliaGeneratorPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.julia";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Julia Generator";
        }

        [RangeParam(DefaultValue = .2, Label = "Real Coordinate", Hint = "The real part of a constant complex value used to render the fractal", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double RealCoord;

        [RangeParam(DefaultValue = .5, Label = "Imaginary Coordinate", Hint = "The imaginary part of a constant complex value used to render the fractal", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double ImaginaryCoord;

        protected override AlgorithmParams<double> GetParams()
        {
            return new JuliaParams<double>
            {
                MaxIterations = MaxIterations,
                Magnification = Math.Pow(2, Magnification),
                Location = new Complex<double>(Real, Imag),
                EscapeRadius = 4.0,

                Coordinates = new Complex<double>(RealCoord, ImaginaryCoord)
            };
        }
    }
}
