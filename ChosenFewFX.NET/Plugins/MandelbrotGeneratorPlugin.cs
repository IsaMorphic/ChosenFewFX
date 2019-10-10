using MandelbrotSharp.Algorithms;
using MandelbrotSharp.Numerics;
using MandelbrotSharp.Rendering;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChosenFewFX.NET.Plugins
{
    public class MandelbrotGeneratorPlugin : FractalGeneratorPlugin
    {
        protected override IFractalRenderer InitializeRenderer(int width, int height)
        {
            return new DefaultRenderer<double, SquareMandelbrotAlgorithm<double>>(width, height);
        }

        protected override IAlgorithmParams GetParams()
        {
            return new SquareMandelbrotParams<double>
            {
                MaxIterations = MaxIterations,
                Magnification = Math.Pow(2, Magnification),
                Location = new Complex<double>(Real, Imag),
                EscapeRadius = 4.0
            };
        }
    }
}
