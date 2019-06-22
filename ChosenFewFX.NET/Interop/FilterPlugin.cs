using SkiaSharp;

namespace ChosenFewFX.NET.Interop
{
    public class FilterPlugin : BasePlugin
    {
        protected SKBitmap SourceImage;

        public ImageWrapper _SourceImage { set => SourceImage = value; }
    }
}
