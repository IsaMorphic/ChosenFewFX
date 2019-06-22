
using ChosenFewFX.NET.Geometry;
using ChosenFewFX.NET.Interop;
using SkiaSharp;
using System;
using System.Numerics;
using System.Threading.Tasks;

namespace ChosenFewFX.NET.Plugins
{
    public class JuliaDistortPlugin : FilterPlugin
    {
        private Func<Complex, Complex> fDir;
        private Func<Complex, Complex> fInv;

        private RectangleI ImageBounds => new RectangleI(0, 0, SourceImage.Width, SourceImage.Height);
        private RectangleD ProjectionBounds = new RectangleD(-1, 1, 1, -1);

        [RangeParam(DefaultValue = .2, Label = "Real Coordinate", MinimumValue = -2.0, MaximumValue = 2.0)]
        public double Real;

        [RangeParam(DefaultValue = .5, Label = "Imaginary Coordinate", MinimumValue = -2.0, MaximumValue = 2.0)]
        public double Imag;

        [RangeParam(DefaultValue = 4, Label = "Iteration Count", MinimumValue = 0, MaximumValue = 10)]
        public int IterCount;

        [RangeParam(DefaultValue = .5, Label = "Distortion Alpha", MinimumValue = 0.0, MaximumValue = 1.0)]
        public double Alpha;

        public JuliaDistortPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.juliadistort";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Julia Distort";
            fDir = z =>
            {
                Complex c = new Complex(Real, Imag);
                for (int i = 0; i < IterCount; i++)
                    z = z * z + c;
                return z;
            };
            fInv = z => 1 / fDir(z);
        }

        public override void ProcessPixels(RectangleI region)
        {
            ComplexGrid gridDirect = new ComplexGrid(ImageBounds, ProjectionBounds, fDir);
            ComplexGrid gridInverse = new ComplexGrid(ImageBounds, ProjectionBounds, fInv);

            Parallel.For(region.TopLeft.Y, region.BottomRight.Y, y =>
            {
                Parallel.For(region.TopLeft.X, region.BottomRight.X, x =>
                {
                    var pDir = (PointI)lerpPoint(new PointD(x, y), gridDirect[x, y], Alpha);
                    var pInv = (PointI)lerpPoint(new PointD(x, y), gridInverse[x, y], Alpha);
                    SKColor color;
                    if (IsPointInRange(pDir, ImageBounds))
                        color = SourceImage.GetPixel(pDir.X, pDir.Y);
                    else if (IsPointInRange(pInv, ImageBounds))
                        color = SourceImage.GetPixel(pInv.X, pInv.Y);
                    else
                        color = SKColor.Empty;
                    DestImage.SetPixel(x, y, color);
                });
            });
        }

        private bool IsPointInRange(PointD p, RectangleD bounds)
        {
            bool isNaN = double.IsNaN(p.X) || double.IsNaN(p.Y);
            bool tooBig = p.X >= bounds.BottomRight.X || p.Y >= bounds.BottomRight.Y;
            bool tooSmall = p.X < bounds.TopLeft.X || p.Y < bounds.TopLeft.Y;
            return !(isNaN || tooBig || tooSmall);
        }

        private PointD lerpPoint(PointD p0, PointD p1, double alpha)
        {
            double newX = lerpValue(p0.X, p1.X, alpha);
            double newY = p0.Y + (newX - p0.X) * (p1.Y - p0.Y) / (p1.X - p0.X);
            return new PointD(newX, newY);
        }

        private double lerpValue(double v0, double v1, double t)
        {
            return (1 - t) * v0 + t * v1;
        }
    }
}
