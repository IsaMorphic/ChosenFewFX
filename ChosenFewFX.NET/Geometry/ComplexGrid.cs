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
using MandelbrotSharp.Numerics;
using System;

namespace ChosenFewFX.NET.Geometry
{
    public class ComplexGrid
    {
        private Func<Complex<double>, Complex<double>> Func;

        private RectangleD OuterPlane;
        private RectangleD InnerPlane;

        public ComplexGrid(RectangleD OuterPlane, RectangleD InnerPlane, Func<Complex<double>, Complex<double>> Func)
        {
            this.OuterPlane = OuterPlane;
            this.InnerPlane = InnerPlane;
            this.Func = Func;
        }

        private Complex<double> MapPoint(PointD p)
        {
            PointD translated = p - OuterPlane.TopLeft;

            PointD normalized = new PointD(translated.X / OuterPlane.Width, translated.Y / OuterPlane.Height);
            PointD dialated = new PointD(normalized.X * InnerPlane.Width, normalized.Y * InnerPlane.Height);

            PointD projected = dialated + InnerPlane.TopLeft;

            return new Complex<double>(projected.X, projected.Y);
        }

        private PointD MapPoint(Complex<double> z)
        {
            PointD p = new PointD(z.Real.Value, z.Imag.Value);

            PointD translated = p - InnerPlane.TopLeft;

            PointD normalized = new PointD(translated.X / InnerPlane.Width, translated.Y / InnerPlane.Height);
            PointD dialated = new PointD(normalized.X * OuterPlane.Width, normalized.Y * OuterPlane.Height);

            PointD projected = dialated + OuterPlane.TopLeft;

            return projected;
        }

        public PointD this[double x, double y]
        {
            get
            {
                PointD p = new PointD(x, y);
                Complex<double> z = MapPoint(p);
                Complex<double> w = Func(z);
                return MapPoint(w);
            }
        }
    }
}
