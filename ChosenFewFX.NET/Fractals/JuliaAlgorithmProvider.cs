using MandelbrotSharp.Algorithms;
using MandelbrotSharp.Numerics;

namespace ChosenFewFX.NET.Fractals
{
    public class JuliaParams<TNumber> : MTypeParams<TNumber> where TNumber : struct
    {
        public Complex<TNumber> Coordinates { get; set; }

        public override IAlgorithmParams Copy()
        {
            return new JuliaParams<TNumber>
            {
                MaxIterations = MaxIterations,
                Magnification = Magnification,
                Location = Location,
                EscapeRadius = EscapeRadius,

                Coordinates = Coordinates
            };
        }
    }

    public class JuliaAlgorithmProvider<TNumber> : MTypeAlgorithm<TNumber, JuliaParams<TNumber>> where TNumber : struct
    {
        private Complex<TNumber> Coordinate;

        protected override Complex<TNumber> DoIteration(Complex<TNumber> z, Complex<TNumber> c)
        {
            if (z == Complex<TNumber>.Zero)
                z = c;
            return z * z + Coordinate;
        }
    }
}
