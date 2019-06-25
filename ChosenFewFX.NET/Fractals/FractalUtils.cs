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
