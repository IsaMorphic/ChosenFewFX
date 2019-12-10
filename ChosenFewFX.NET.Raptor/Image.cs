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

using SkiaSharp;
using System.Threading;
using System.Threading.Tasks;

namespace ChosenFewFX.NET.Raptor
{
    public class Image
    {
        private SKBitmap Bitmap { get; }

        public int Width => Bitmap.Width;
        public int Height => Bitmap.Height;

        public Pixel AverageColor { get; }

        public Image(SKBitmap bitmap)
        {
            Bitmap = bitmap;
            AverageColor = CalculateAverageColor();
        }

        private Pixel CalculateAverageColor()
        {
            int redSum = 0;
            int greenSum = 0;
            int blueSum = 0;
            int alphaSum = 0;

            Parallel.For(0, Height, y => 
            {
                Parallel.For(0, Width, x =>
                {
                    Pixel pixel = Read(x, y);
                    Interlocked.Add(ref redSum, pixel.Red);
                    Interlocked.Add(ref greenSum, pixel.Green);
                    Interlocked.Add(ref blueSum, pixel.Blue);
                    Interlocked.Add(ref alphaSum, pixel.Alpha);
                });
            });

            int totalNumPixels = Width * Height;

            int averageRed = redSum / totalNumPixels;
            int averageGreen = greenSum / totalNumPixels;
            int averageBlue = blueSum / totalNumPixels;
            int averageAlpha = alphaSum / totalNumPixels;

            return new Pixel((byte)averageRed, (byte)averageGreen, (byte)averageBlue, (byte)averageAlpha);
        }

        public Pixel Read(int x, int y)
        {
            SKColor color = Bitmap.GetPixel(x, y);
            return new Pixel(color.Blue, color.Green, color.Red, color.Alpha);
        }
    }
}
