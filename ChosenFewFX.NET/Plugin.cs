using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SkiaSharp;
namespace ChosenFewFX.NET
{
    public class Plugin
    {
        public string Id { get; protected set; }

        public int MajorVersion { get; protected set; }
        public int MinorVersion { get; protected set; }

        public string Name { get; protected set; }

        public SKBitmap DestImage { get; set; }

        public SKBitmap LinkPixelsToManagedImage(Rectangle imageBounds, int rowBytes, IntPtr pixels)
        {
            SKImageInfo info = new SKImageInfo((int)imageBounds.Width, (int)imageBounds.Height);
            SKBitmap image = new SKBitmap(info, rowBytes);
            image.SetPixels(pixels);
            return image;
        }

        public virtual void PreProcess() { }

        public virtual void ProcessPixels(Rectangle regionOfInterest) { }

        public virtual void PostProcess() { }
    }
}
