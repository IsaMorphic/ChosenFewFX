/*
 *  Copyright 2019 Chosen Few Software
 *  This file is part of Chosen Few FX.
 *
 *  Chosen Few FX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Chosen Few FX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Chosen Few FX.  If not, see <https://www.gnu.org/licenses/>.
 */
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
        protected override Complex<TNumber> DoIteration(Complex<TNumber> z, Complex<TNumber> c)
        {
            if (z == Complex<TNumber>.Zero)
                z = c;
            return z * z + Params.Coordinates;
        }
    }
}
