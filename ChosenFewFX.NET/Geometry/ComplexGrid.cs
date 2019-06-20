using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace ChosenFewFX.NET
{
    public class ComplexGrid
    {
        private Func<Complex, Complex> Func;

        private RectangleD OuterPlane;
        private RectangleD InnerPlane;

        public ComplexGrid(RectangleD OuterPlane, RectangleD InnerPlane, Func<Complex, Complex> Func)
        {
            this.OuterPlane = OuterPlane;
            this.InnerPlane = InnerPlane;
            this.Func = Func;
        }

        private Complex MapPoint(PointD p)
        {
            PointD translated = p - OuterPlane.TopLeft;

            PointD normalized = new PointD(translated.X / OuterPlane.Width, translated.Y / OuterPlane.Height);
            PointD dialated = new PointD(normalized.X * InnerPlane.Width, normalized.Y * InnerPlane.Height);

            PointD projected = dialated + InnerPlane.TopLeft;

            return new Complex(projected.X, projected.Y);
        }

        private PointD MapPoint(Complex z)
        {
            PointD p = new PointD(z.Real, z.Imaginary);

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
                Complex z = MapPoint(p);
                Complex w = Func(z);
                return MapPoint(w);
            }
        }
    }
}
