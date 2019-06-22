using ChosenFewFX.NET.Interop;
using MandelbrotSharp.Rendering;
using MandelbrotSharp.Imaging;
using MandelbrotSharp.Numerics;
using SkiaSharp;
using ChosenFewFX.NET.Geometry;
using System.Threading.Tasks;

namespace ChosenFewFX.NET.Plugins
{
    public class MandelbrotGeneratorPlugin : BasePlugin
    {
        private RgbaValue[] Colors = new RgbaValue[] 
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
        };

        [RangeParam(DefaultValue = 400, Label = "Maximum Iterations", MaximumValue = int.MaxValue, MinimumValue = 0)]
        public int MaxIterations;

        [RangeParam(DefaultValue = 1.0, Label = "Magnification", MaximumValue = (double)((long)1 << 49), MinimumValue = 0.0)]
        public double Magnification;

        [RangeParam(DefaultValue = 0.0, Label = "X Offset", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double Real;

        [RangeParam(DefaultValue = 0.0, Label = "Y Offset", MaximumValue = 2.0, MinimumValue = -2.0)]
        public double Imag;

        public MandelbrotGeneratorPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.mandelbrot";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Mandelbrot Generator";
        }

        public override void PreProcess()
        {
            MandelbrotRenderer renderer = new MandelbrotRenderer(DestImage.Width, DestImage.Height);
            Gradient gradient = new Gradient(Colors, 256);
            RenderSettings settings = new RenderSettings
            {
                OuterColors = gradient,
                Magnification = Magnification, 
                MaxIterations = MaxIterations,
                Location = new Complex<BigDecimal>(Real, Imag)
            };
            renderer.Setup(settings);
            renderer.FrameFinished += Renderer_FrameFinished;
            renderer.StartRenderFrame().Wait();
        }

        private void Renderer_FrameFinished(object sender, FrameEventArgs e)
        {
            for (int y = 0; y < DestImage.Height; y++)
            {
                for (int x = 0; x < DestImage.Width; x++)
                {
                    RgbaValue color = e.Frame.GetPixel(x, y);
                    DestImage.SetPixel(x, y, new SKColor(color.blue, color.green, color.red));
                }
            }
        }
    }
}
