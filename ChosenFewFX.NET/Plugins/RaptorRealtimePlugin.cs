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

using ChosenFewFX.NET.Interop;
using ChosenFewFX.NET.Raptor;
using Microsoft.CodeAnalysis.CSharp.Scripting;
using Microsoft.CodeAnalysis.Scripting;
using SkiaSharp;
using System;

namespace ChosenFewFX.NET.Plugins
{
    public class RaptorRealtimePlugin : RaptorBasePlugin
    {
        private const string NewLines = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";

        private static readonly SKPaint ErrorPaint = new SKPaint
        {
            TextSize = 13,
            TextAlign = SKTextAlign.Center,
            Color = SKColors.Red
        };

        private string LatestError { get; set; }
        private string PreviousCode { get; set; }

        [StringParam(DefaultValue = "(api, src, x, y) => src" + NewLines, Hint = "C# code for filter mapping", Label = "Raptor Code", StringType = StringType.MultiLine)]
        public string FilterCode;

        public RaptorRealtimePlugin()
        {
            Id = "com.chosenfewsoftware.openfx.raptor_realtime";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Raptor (Realtime)";
        }

        public override void PreProcess()
        {
            if (FilterCode == PreviousCode)
                return;
            try
            {
                ScriptOptions scriptOptions = ScriptOptions.Default
                    .AddReferences(typeof(Api).Assembly)
                    .AddImports("System", "ChosenFewFX.NET.Raptor");
                FilterFunc = 
                    CSharpScript.EvaluateAsync<Func<Api, Pixel, int, int, Pixel>>
                    (FilterCode, scriptOptions).Result;
                EncounteredError = false;
                LatestError = null;
            }
            catch (CompilationErrorException e)
            {
                LatestError = e.Message;
            }
            PreviousCode = FilterCode;
        }

        public override void PostProcess()
        {
            if (string.IsNullOrEmpty(LatestError))
                return;
            var canvas = new SKCanvas(DestImage);
            canvas.Translate(new SKPoint(DestImage.Width / 2, DestImage.Height / 2));
            canvas.Scale(1, -1);
            canvas.DrawText(LatestError, 0, 0, ErrorPaint);
        }
    }
}
