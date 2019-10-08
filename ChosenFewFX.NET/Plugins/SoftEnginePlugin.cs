using ChosenFewFX.NET.Interop;
using SharpDX;
using SkiaSharp;
using SoftEngine;
using System;

namespace ChosenFewFX.NET.Plugins
{
    public class SoftEnginePlugin : BasePlugin
    {
        private Device Device { get; set; }
        private Camera Camera { get; set; }
        private Mesh[] Meshes { get; set; }

        public SoftEnginePlugin()
        {
            Id = "com.chosenfewsoftware.openfx.softengine";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "SoftEngine 3D";

            Camera = new Camera();
            Camera.Position = new Vector3(0.0f, 0.0f, 10.0f);
            Camera.Target = Vector3.Zero;

            Meshes = Device.LoadJSONFile("monkey.babylon");
        }

        public override void PostProcess()
        {
            Device = new Device(DestImage.Width, DestImage.Height);
            Device.Clear(0, 0, 0, 255);
            Device.Render(Camera, Meshes);
            SKBitmap rendered = Device.Present();
            for (int y = 0; y < DestImage.Height; y++)
            {
                for (int x = 0; x < DestImage.Width; x++)
                {
                    SKColor pixel = rendered.GetPixel(x, DestImage.Height - (y + 1));
                    DestImage.SetPixel(x, y, new SKColor(pixel.Blue, pixel.Green, pixel.Red));
                }
            }
        }
    }
}
