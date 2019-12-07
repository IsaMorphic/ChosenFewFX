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
using ChosenFewFX.NET.Raptor;
using SkiaSharp;
using System;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ChosenFewFX.NET.Plugins
{
    public class RaptorBasePlugin : FilterPlugin
    {
        protected static readonly Func<Api, Pixel, int, int, Pixel> DefaultFilter = (api, src, x, y) => src;

        protected bool EncounteredError { get; set; }
        protected Func<Api, Pixel, int, int, Pixel> FilterFunc { get; set; }

        [RangeParam(DefaultValue = 0.0, Hint = "Primary script parameter (behavior depends on script)", Label = "Primary Parameter", MaximumValue = 10.0, MinimumValue = -10.0)]
        public double PrimaryParam;

        [RangeParam(DefaultValue = 0.0, Hint = "Secondary script parameter (behavior depends on script)", Label = "Secondary Parameter", MaximumValue = 10.0, MinimumValue = -10.0)]
        public double SecondaryParam;

        [Param(DefaultValue = new byte[] { 255, 0, 0, 255 }, Hint = "Script color parameter (behavior depends on script)", Label = "Color Parameter")]
        public Color ColorParam;

        public override void ProcessPixels(RectangleI region)
        {
            Api api = new Api(
                image: new Image(SourceImage),
                param: new ScriptParams(
                    PrimaryParam,
                    SecondaryParam,
                    (Pixel)ColorParam
                    )
                );
            Parallel.For(region.TopLeft.Y, region.BottomRight.Y, (y, outer) =>
            {
                if (EncounteredError)
                {
                    outer.Break();
                    return;
                }
                Parallel.For(region.TopLeft.X, region.BottomRight.X, (x, inner) =>
                {
                    if (EncounteredError)
                    {
                        inner.Break();
                        return;
                    }
                    try
                    {
                        SKColor src = SourceImage.GetPixel(x, y);
                        Pixel colorIn = new Pixel(src.Blue, src.Green, src.Red, src.Alpha);
                        Pixel colorOut = FilterFunc(api, colorIn, x, y);
                        SKColor dst = new SKColor(colorOut.Blue, colorOut.Green, colorOut.Red, colorOut.Alpha);
                        DestImage.SetPixel(x, y, dst);
                    }
                    catch (Exception e)
                    {
                        if (!EncounteredError)
                        {
                            EncounteredError = true;
                            MessageBox.Show($"Your code encountered an exception at runtime.\n{e.Message}",
                                "Runtime Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                });
            });
        }
    }
}
