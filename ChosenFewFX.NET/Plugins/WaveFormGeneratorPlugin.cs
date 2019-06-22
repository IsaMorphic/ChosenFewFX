using ChosenFewFX.NET.Geometry;
using ChosenFewFX.NET.Interop;
using NAudio.Wave;
using SkiaSharp;
using System;

namespace ChosenFewFX.NET.Plugins
{
    public class WaveFormGeneratorPlugin : BasePlugin
    {
        WaveStream WaveStream;
        SKCanvas Canvas;
        SKPath WaveFormPath;

        public int SampleCount = 300;

        [RangeParam(DefaultValue = 0.0, Label = "Time (in seconds)", MaximumValue = 123456789.0, MinimumValue = 0.0)]
        public double Time;

        public WaveFormGeneratorPlugin()
        {
            WaveStream = new AudioFileReader("D:\\chosenfewfx.wav");

            Id = "com.chosenfewsoftware.openfx.waveform";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Wave Form Generator";
        }

        public override void PreProcess()
        {
            Canvas = new SKCanvas(DestImage);
            Canvas.Clear();
            WaveFormPath = new SKPath();
        }

        public override void ProcessPixels(RectangleI region)
        {
            float[] samples = new float[SampleCount];
            WaveStream.CurrentTime = TimeSpan.FromSeconds(Time);
            WaveStream.ToSampleProvider().Read(samples, 0, samples.Length);
            WaveFormPath.MoveTo(0, DestImage.Height / 2 * (samples[0] + 1));
            for (int i = 0; i < samples.Length; i++)
            {
                WaveFormPath.LineTo(DestImage.Width / samples.Length * i, DestImage.Height / 2 * (samples[i] + 1));
            }
        }

        public override void PostProcess()
        {
            Canvas.DrawPath(WaveFormPath, new SKPaint { Color = SKColors.Red, StrokeWidth = 10, IsStroke = true });
        }
    }
}
