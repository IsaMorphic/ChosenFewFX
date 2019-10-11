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
