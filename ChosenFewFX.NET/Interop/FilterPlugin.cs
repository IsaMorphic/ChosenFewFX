using SkiaSharp;

namespace ChosenFewFX.NET.Interop
{
    public class FilterPlugin : BasePlugin
    {
        protected SKBitmap SourceImage { get; set; }

        public ImageWrapper _SourceImage { set => SourceImage = value; }
    }
}
