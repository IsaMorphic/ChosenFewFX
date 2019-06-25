using ChosenFewFX.NET.Geometry;
using ChosenFewFX.NET.Interop;
using NAudio.Wave;
using SkiaSharp;
using System;

namespace ChosenFewFX.NET.Plugins
{
    public class WaveFormGeneratorPlugin : BasePlugin
    {
        private WaveStream WaveStream;
        private SKCanvas Canvas;
        private SKPath WaveFormPath;


        [RangeParam(DefaultValue = 300, Label = "Sample Count", Hint = "The number of samples to graph", MaximumValue = 48000, MinimumValue = 4)]
        public int SampleCount;

        [RangeParam(DefaultValue = 10.0, Label = "Line Width", Hint = "The width of the line that represents the wave-form", MaximumValue = 50.0, MinimumValue = 0.0)]
        public double LineWidth;

        [RangeParam(DefaultValue = 0.0, Label = "Time (in seconds)", Hint = "The amount of time that has passed. (You need to keyframe this)", MaximumValue = 3600.0, MinimumValue = 0.0)]
        public double Time;

        [StringParam(DefaultValue = "", Label = "Audio File", Hint = "The audio file that is to be graphed", StringType = StringType.FilePath)]
        public string FilePath;

        [Param(DefaultValue = new byte[] { 255, 0, 0, 255 }, Label = "Wave Color", Hint = "The color of the line that is graphed")]
        public Color WaveColor;

        public WaveFormGeneratorPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.waveform";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Wave Form";
        }

        public override void PreProcess()
        {
            if (!string.IsNullOrEmpty(FilePath))
                WaveStream = new AudioFileReader(FilePath);
            Canvas = new SKCanvas(DestImage);
            Canvas.Clear();
            WaveFormPath = new SKPath();
        }

        public override void ProcessPixels(RectangleI region)
        {
            if (WaveStream == null)
                return;
            float[] samples = new float[SampleCount];
            WaveStream.CurrentTime = TimeSpan.FromSeconds(Time);
            WaveStream.ToSampleProvider().Read(samples, 0, samples.Length);
            WaveFormPath.MoveTo(0, DestImage.Height / 2 * (samples[0] + 1));
            for (int i = 0; i < samples.Length; i++)
            {
                WaveFormPath.LineTo((float)DestImage.Width / samples.Length * i, DestImage.Height / 2 * (samples[i] + 1));
            }
        }

        public override void PostProcess()
        {
            Canvas.DrawPath(WaveFormPath, new SKPaint { Color = WaveColor, StrokeWidth = (float)LineWidth, IsStroke = true });
        }
    }
}
