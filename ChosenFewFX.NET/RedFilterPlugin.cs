using SkiaSharp;

namespace ChosenFewFX.NET.Plugins
{
    public class RedFilterPlugin : FilterPlugin
    {
        public RedFilterPlugin() : base()
        {
            Id = "com.chosenfewsoftware.openfx.red";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Red";
        }
        public unsafe override void ProcessPixels(RectangleI region)
        {
            byte* sourcePixels = (byte*)SourceImage.GetPixels().ToPointer();
            byte* destPixels = (byte*)DestImage.GetPixels().ToPointer();
            for (int x = region.TopLeft.X; x < region.BottomRight.X; x++)
                for (int y = region.TopLeft.Y; y < region.BottomRight.Y; y++)
                {
                    int idx = (y * SourceImage.Width + x) * 4;
                    destPixels[idx + 0] = sourcePixels[idx + 0]; // red
                    destPixels[idx + 1] = 0;                     // green
                    destPixels[idx + 2] = 0;                     // blue
                    destPixels[idx + 3] = sourcePixels[idx + 3]; // alpha
                }
        }
    }
}
