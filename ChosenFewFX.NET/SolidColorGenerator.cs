using SkiaSharp;

namespace ChosenFewFX.NET.Plugins
{
    public class SolidColorGenerator : Plugin
    {
        public SolidColorGenerator() : base()
        {
            Id = "com.chosenfewsoftware.openfx.solidcolor";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Solid Color";
        }
        public override void PreProcess()
        {
            base.PreProcess();
            DestImage.Erase(new SKColor(255, 255, 255, 255));
        }
    }
}
