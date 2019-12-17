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
using ChosenFewFX.NET.Fractals;
using ChosenFewFX.NET.Interop;
using MandelbrotSharp.Algorithms;
using MandelbrotSharp.Algorithms.Coloring;
using MandelbrotSharp.Imaging;
using MandelbrotSharp.Processing;
using SkiaSharp;
using System;
using System.IO;
using System.Threading;

namespace ChosenFewFX.NET.Plugins
{
    public abstract class FractalGeneratorPlugin : BasePlugin
    {
        class SkiaImageBuilder : ImageBuilder
        {
            public SKBitmap Bitmap { get; }

            public SkiaImageBuilder(SKBitmap bitmap)
            {
                Bitmap = bitmap;
            }

            public override void InitializeImage(int width, int height) { return; }

            public override void WritePixel(int x, int y, RgbaValue color)
            {
                Bitmap.SetPixel(x, y, new SKColor(color.Blue, color.Green, color.Red, color.Alpha));
            }
        }

        private Gradient OuterColors { get; set; } = new Gradient(new RgbaValue[]
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

        private Gradient InnerColors { get; set; } = new Gradient(new RgbaValue[] { new RgbaValue(0, 0, 0), new RgbaValue(0, 0, 0) }, 1);

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

        public override void PreProcess()
        {
            if (File.Exists(PalettePath))
                OuterColors = FractalUtils.LoadPallete(PalettePath);
        }

        public override void PostProcess()
        {
            int threadCount = Math.Max(Environment.ProcessorCount - 2, 1);

            IProcessor<PointData> fractalProcessor = InitializeRenderer(DestImage.Width, DestImage.Height);
            IProcessor<double> innerColorProcessor = new ColorProcessor<SingleColorAlgorithm>(DestImage.Width, DestImage.Height);
            IProcessor<double> outerColorProcessor = new ColorProcessor<SmoothColoringAlgorithm>(DestImage.Width, DestImage.Height);

            SkiaImageBuilder imageBuilder = new SkiaImageBuilder(DestImage);

            fractalProcessor.SetupAsync(new ProcessorConfig
            {
                ThreadCount = threadCount,
                Params = GetParams()
            }, CancellationToken.None).Wait();
            PointData[,] inputData = fractalProcessor.ProcessAsync(CancellationToken.None).Result;

            innerColorProcessor.SetupAsync(new ColorProcessorConfig
            {
                ThreadCount = threadCount,
                Params = new EmptyColoringParams(),
                PointClass = PointClass.Inner,
                InputData = inputData
            }, CancellationToken.None).Wait();
            double[,] innerIndicies = innerColorProcessor.ProcessAsync(CancellationToken.None).Result;

            outerColorProcessor.SetupAsync(new ColorProcessorConfig
            {
                ThreadCount = threadCount,
                Params = new EmptyColoringParams(),
                PointClass = PointClass.Outer,
                InputData = inputData
            }, CancellationToken.None).Wait();
            double[,] outerIndicies = outerColorProcessor.ProcessAsync(CancellationToken.None).Result;

            imageBuilder.CreateImage(outerIndicies, innerIndicies, OuterColors, InnerColors);
        }

        protected abstract IFractalParams GetParams();

        protected abstract IProcessor<PointData> InitializeRenderer(int width, int height);
    }
}
