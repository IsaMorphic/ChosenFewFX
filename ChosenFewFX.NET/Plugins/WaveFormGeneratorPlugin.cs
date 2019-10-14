/*
 *  Copyright 2019 Chosen Few Software
 *  This file is part of Chosen Few FX.
 *
 *  Chosen Few FX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Chosen Few FX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Chosen Few FX.  If not, see <https://www.gnu.org/licenses/>.
 */
using ChosenFewFX.NET.Geometry;
using ChosenFewFX.NET.Interop;
using NAudio.Wave;
using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;

namespace ChosenFewFX.NET.Plugins
{
    public class WaveFormGeneratorPlugin : BasePlugin
    {
        private SKCanvas Canvas { get; set; }
        private SKPath WaveFormPath { get; set; }

        private WaveStream WaveStream { get; set; }

        private Dictionary<string, AudioFileReader> AudioFiles { get; set; }


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
            AudioFiles = new Dictionary<string, AudioFileReader>();
        }

        public override void PreProcess()
        {
            Canvas = new SKCanvas(DestImage);
            Canvas.Clear();
            WaveFormPath = new SKPath();
        }

        public override void ProcessPixels(RectangleI region)
        {
            LoadAudio();
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
            WaveStream = null;
        }

        private void LoadAudio()
        {
            if (AudioFiles.ContainsKey(FilePath))
            {
                WaveStream = AudioFiles[FilePath];
            }
            else if (File.Exists(FilePath))
            {
                AudioFileReader reader = new AudioFileReader(FilePath);
                AudioFiles.Add(FilePath, reader);
                WaveStream = reader;
            }
        }
    }
}
