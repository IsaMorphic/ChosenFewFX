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
using ChosenFewFX.NET.Geometry;
using SkiaSharp;
using System;
namespace ChosenFewFX.NET.Interop
{
    public enum ImageType
    {
        Rgba = 4,
        Bgra = 6
    }
    public class BasePlugin
    {
        protected SKBitmap DestImage;

        public string Id { get; protected set; }

        public int MajorVersion { get; protected set; }
        public int MinorVersion { get; protected set; }

        public string Name { get; protected set; }

        public ImageWrapper _DestImage { set => DestImage = value; }

        public double CurrentTime;

        public ImageWrapper LinkPixelsToManagedImage(RectangleI imageBounds, IntPtr pixels, ImageType imageType)
        {
            SKImageInfo info = new SKImageInfo()
                .WithSize(imageBounds.Width, imageBounds.Height)
                .WithColorType((SKColorType)imageType)
                .WithAlphaType(SKAlphaType.Premul);
            SKBitmap image = new SKBitmap(info);
            image.SetPixels(pixels);
            return new ImageWrapper(image);
        }

        public virtual void PreProcess() { }

        public virtual void ProcessPixels(RectangleI region) { }

        public virtual void PostProcess() { }

        public virtual void ParamUpdated(string paramName) { }
    }
}
