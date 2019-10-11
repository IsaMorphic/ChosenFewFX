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
using MandelbrotSharp.Imaging;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChosenFewFX.NET.Fractals
{
    static class FractalUtils
    {
        public static Gradient LoadPallete(string path)
        {
            List<RgbaValue> pallete = new List<RgbaValue>();
            StreamReader palleteData = new StreamReader(path);
            while (!palleteData.EndOfStream)
            {
                try
                {
                    string palleteString = palleteData.ReadLine();
                    string[] palleteTokens = palleteString.Split(new char[1] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    byte r = (byte)int.Parse(palleteTokens[0]);
                    byte g = (byte)int.Parse(palleteTokens[1]);
                    byte b = (byte)int.Parse(palleteTokens[2]);
                    RgbaValue color = new RgbaValue(r, g, b);
                    pallete.Add(color);
                }
                catch (FormatException) { }
            }
            return new Gradient(pallete.ToArray(), 256);
        }
    }
}
