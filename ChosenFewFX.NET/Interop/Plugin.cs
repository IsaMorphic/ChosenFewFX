using SkiaSharp;
using System;
namespace ChosenFewFX.NET
{
    public class Plugin
    {
        public string Id { get; protected set; }

        public int MajorVersion { get; protected set; }
        public int MinorVersion { get; protected set; }

        public string Name { get; protected set; }

        public SKBitmap DestImage { get; set; }

        public SKBitmap LinkPixelsToManagedImage(RectangleI imageBounds, IntPtr pixels)
        {
            SKImageInfo info = new SKImageInfo()
                .WithSize(imageBounds.Width, imageBounds.Height)
                .WithColorType(SKColorType.Bgra8888)
                .WithAlphaType(SKAlphaType.Premul);
            SKBitmap image = new SKBitmap(info);
            image.SetPixels(pixels);
            return image;
        }

        public virtual void PreProcess() { }

        public virtual void ProcessPixels(RectangleI region) { }

        public virtual void PostProcess() { }
    }
}
