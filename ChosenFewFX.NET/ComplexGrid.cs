using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace ChosenFewFX.NET
{
    public class ComplexGrid
    {
        private Func<Complex, Complex> Func;

        private Rectangle OuterPlane;
        private Rectangle InnerPlane;

        public ComplexGrid(Rectangle OuterPlane, Rectangle InnerPlane, Func<Complex, Complex> Func)
        {
            this.OuterPlane = OuterPlane;
            this.InnerPlane = InnerPlane;
            this.Func = Func;
        }

        private Complex MapPoint(Point p)
        {
            Point translated = p - OuterPlane.TopLeft;

            Point normalized = new Point(translated.X / OuterPlane.Width, translated.Y / OuterPlane.Height);
            Point dialated = new Point(normalized.X * InnerPlane.Width, normalized.Y * InnerPlane.Height);

            Point projected = dialated + InnerPlane.TopLeft;

            return new Complex(projected.X, projected.Y);
        }

        private Point MapPoint(Complex z)
        {
            Point p = new Point(z.Real, z.Imaginary);

            Point translated = p - InnerPlane.TopLeft;

            Point normalized = new Point(translated.X / InnerPlane.Width, translated.Y / InnerPlane.Height);
            Point dialated = new Point(normalized.X * OuterPlane.Width, normalized.Y * OuterPlane.Height);

            Point projected = dialated + OuterPlane.TopLeft;

            return projected;
        }

        public Point this[double x, double y]
        {
            get
            {
                Point p = new Point(x, y);
                Complex z = MapPoint(p);
                Complex w = Func(z);
                return MapPoint(w);
            }
        }
    }
}
