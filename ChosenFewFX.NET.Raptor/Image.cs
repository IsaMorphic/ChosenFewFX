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

namespace ChosenFewFX.NET.Raptor
{
    public class Image
    {
        private SKBitmap Bitmap { get; }

        public int Width => Bitmap.Width;
        public int Height => Bitmap.Height;

        public Image(SKBitmap bitmap)
        {
            Bitmap = bitmap;
        }

        public Pixel Read(int x, int y)
        {
            SKColor color = Bitmap.GetPixel(x, y);
            return new Pixel(color.Blue, color.Green, color.Red, color.Alpha);
        }
    }
}
