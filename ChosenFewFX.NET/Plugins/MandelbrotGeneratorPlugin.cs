using MandelbrotSharp.Algorithms;
using MandelbrotSharp.Numerics;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChosenFewFX.NET.Plugins
{
    public class MandelbrotGeneratorPlugin : FractalGeneratorPlugin<SquareMandelbrotAlgorithm<double>>
    {
        protected override AlgorithmParams<double> GetParams()
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
