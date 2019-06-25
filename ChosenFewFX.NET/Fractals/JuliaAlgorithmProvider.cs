using MandelbrotSharp.Algorithms;
using MandelbrotSharp.Numerics;
using System.Numerics;

namespace ChosenFewFX.NET.Fractals
{
    public class JuliaAlgorithmProvider<T> : AlgorithmProvider<T> where T : struct
    {
        [Parameter(DefaultValue = 0)]
        public Number<T> RealCoord;

        [Parameter(DefaultValue = 0)]
        public Number<T> ImaginaryCoord;

        private Complex<T> Coordinate => new Complex<T>(RealCoord, ImaginaryCoord);

        protected override PointData Run(Complex<T> point)
        {
            Complex<T> z = point;
            Complex<T> c = Coordinate;
            int n = 0;
            while (z.MagnitudeSqu < 4 && n < Params.MaxIterations)
            { 
                z = z * z + c;
                n++;
            }
            return new PointData((Complex)z, n, n < Params.MaxIterations);
        }
    }
}
