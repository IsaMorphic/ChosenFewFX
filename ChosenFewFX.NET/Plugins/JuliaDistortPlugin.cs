
using SkiaSharp;
using System;
using System.Numerics;

namespace ChosenFewFX.NET.Plugins
{
    public class JuliaDistortPlugin : FilterPlugin
    {
        private Func<Complex, Complex> fDir;
        private Func<Complex, Complex> fInv;

        private RectangleI ImageBounds => new RectangleI(0, 0, SourceImage.Width, SourceImage.Height);
        private RectangleD ProjectionBounds = new RectangleD(-1, 1, 1, -1);

        [RangeParam(DefaultValue = .2, Label = "Real Coordinate", MinimumValue = -64.0, MaximumValue = 64.0)]
        public double Real;

        [RangeParam(DefaultValue = .5, Label = "Imaginary Coordinate", MinimumValue = -64.0, MaximumValue = 64.0)]
        public double Imag;

        public JuliaDistortPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.juliadistort";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Julia Distort";
            fDir = z =>
            {
                Complex c = new Complex(Real % 2, Imag % 2);
                for (int i = 0; i < 4; i++)
                    z = z * z + c;
                return z;
            };
            fInv = z => 1 / fDir(z);
        }

        public override void ProcessPixels(RectangleI region)
        {
            ComplexGrid gridDirect = new ComplexGrid(ImageBounds, ProjectionBounds, fDir);
            ComplexGrid gridInverse = new ComplexGrid(ImageBounds, ProjectionBounds, fInv);

            for (int y = region.TopLeft.Y; y < region.BottomRight.Y; y++)
            {
                for (int x = region.TopLeft.X; x < region.BottomRight.X; x++)
                {
                    var pDir = (PointI)gridDirect[x, y];
                    var pInv = (PointI)gridInverse[x, y];
                    SKColor color;
                    if (IsPointInRange(pDir, ImageBounds))
                        color = SourceImage.GetPixel(pDir.X, pDir.Y);
                    else if (IsPointInRange(pInv, ImageBounds))
                        color = SourceImage.GetPixel(pInv.X, pInv.Y);
                    else
                        color = SKColor.Empty;
                    DestImage.SetPixel(x, y, color);
                }
            }
        }

        private bool IsPointInRange(PointD p, RectangleD bounds)
        {
            bool isNaN = double.IsNaN(p.X) || double.IsNaN(p.Y);
            bool tooBig = p.X >= bounds.BottomRight.X || p.Y >= bounds.BottomRight.Y;
            bool tooSmall = p.X < bounds.TopLeft.X || p.Y < bounds.TopLeft.Y;
            return !(isNaN || tooBig || tooSmall);
        }
    }
}
