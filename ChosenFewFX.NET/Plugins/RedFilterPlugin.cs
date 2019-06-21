using SkiaSharp;

namespace ChosenFewFX.NET.Plugins
{
    public class RedFilterPlugin : FilterPlugin
    {
        public RedFilterPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.red";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Red";
        }

        [Param(DefaultValue = true, Label = "Red Only")]
        public bool RedOnly;

        public unsafe override void ProcessPixels(RectangleI region)
        {
            byte* sourcePixels = (byte*)SourceImage.GetPixels().ToPointer();
            byte* destPixels = (byte*)DestImage.GetPixels().ToPointer();
            for (int x = region.TopLeft.X; x < region.BottomRight.X; x++)
                for (int y = region.TopLeft.Y; y < region.BottomRight.Y; y++)
                {
                    int idx = (y * SourceImage.Width + x) * 4;
                    destPixels[idx + 0] = sourcePixels[idx + 0];                     // red
                    destPixels[idx + 1] = RedOnly ? (byte)0 : sourcePixels[idx + 0]; // green
                    destPixels[idx + 2] = RedOnly ? (byte)0 : sourcePixels[idx + 0]; // blue
                    destPixels[idx + 3] = sourcePixels[idx + 3];                     // alpha
                }
        }
    }
}
