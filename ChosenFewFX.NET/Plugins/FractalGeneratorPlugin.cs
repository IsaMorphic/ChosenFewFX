using ChosenFewFX.NET.Interop;
using MandelbrotSharp.Rendering;
using MandelbrotSharp.Imaging;
using SkiaSharp;
using ChosenFewFX.NET.Fractals;
using System;
using System.Threading.Tasks;
using MandelbrotSharp.Algorithms;

namespace ChosenFewFX.NET.Plugins
{
    public abstract class FractalGeneratorPlugin : BasePlugin
    {
        private IFractalRenderer Renderer = null;

        private Gradient Colors = new Gradient(new RgbaValue[]
        {
            new RgbaValue(9, 1, 47),
            new RgbaValue(4, 4, 73),
            new RgbaValue(0, 7, 100),
            new RgbaValue(12, 44, 138),
            new RgbaValue(24, 82, 177),
            new RgbaValue(57, 125, 209),
            new RgbaValue(134, 181, 229),
            new RgbaValue(211, 236, 248),
            new RgbaValue(241, 233, 191),
            new RgbaValue(248, 201, 95),
            new RgbaValue(255, 170, 0),
            new RgbaValue(204, 128, 0),
            new RgbaValue(153, 87, 0),
            new RgbaValue(106, 52, 3),
            new RgbaValue(66, 30, 15),
            new RgbaValue(25, 7, 26),
        }, 256);

        [StringParam(DefaultValue = "", Label = "Palette File", Hint = "Path to palette data. There should be a 'Palettes' folder in the same location that you installed Chosen Few FX.", StringType = StringType.FilePath)]
        public string PalettePath;

        [RangeParam(DefaultValue = 400, Label = "Maximum Iterations", Hint = "Number of times the calculation is iterated.  More iterations means more detail.", MaximumValue = int.MaxValue, MinimumValue = 0)]
        public int MaxIterations;

        [RangeParam(DefaultValue = 0.0, Label = "Magnification (Power of 2)", Hint = "How much the image is to be magnified (increases exponentially, i.e. increasing this value by 1 increases the magnification by 100%)", MaximumValue = 64.0, MinimumValue = 0.0)]
        public double Magnification;

        [RangeParam(DefaultValue = 0.0, Label = "X Offset", Hint = "How much to offset the image in the horizontal direction.", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double Real;

        [RangeParam(DefaultValue = 0.0, Label = "Y Offset", Hint = "How much to offset the image in the vertical direction.", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double Imag;

        public FractalGeneratorPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.mandelbrot";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Mandelbrot Generator";
        }

        public override void PreProcess()
        {
            if (Renderer?.RenderStatus == TaskStatus.Running)
                Renderer?.StopRenderFrame();
            Renderer = InitializeRenderer(DestImage.Width, DestImage.Height);
            if (!string.IsNullOrWhiteSpace(PalettePath))
                Colors = FractalUtils.LoadPallete(PalettePath);
            RenderSettings settings = new RenderSettings
            {
                OuterColors = Colors,
                InnerColor = new RgbaValue(0, 0, 0),
                ThreadCount = Math.Max(Environment.ProcessorCount - 2, 1),
                Params = GetParams()
            };

            Renderer.Setup(settings);
            Renderer.FrameFinished += Renderer_FrameFinished;
            Renderer.StartRenderFrame().Wait();
        }

        protected abstract IAlgorithmParams GetParams();
        
        protected abstract IFractalRenderer InitializeRenderer(int width, int height);

        private void Renderer_FrameFinished(object sender, FrameEventArgs e)
        {
            for (int y = 0; y < DestImage.Height; y++)
            {
                for (int x = 0; x < DestImage.Width; x++)
                {
                    RgbaValue color = e.Frame.GetPixel(x, y);
                    DestImage.SetPixel(x, y, new SKColor(color.Blue, color.Green, color.Red));
                }
            }
        }
    }
}
