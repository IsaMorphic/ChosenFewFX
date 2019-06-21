using SkiaSharp;

namespace ChosenFewFX.NET.Interop
{
    public class FilterPlugin : BasePlugin
    {
        protected SKBitmap sourceImage;

        public ImageWrapper SourceImage { set => sourceImage = value; }
    }
}
