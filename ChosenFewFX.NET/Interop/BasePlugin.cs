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
    }
}
