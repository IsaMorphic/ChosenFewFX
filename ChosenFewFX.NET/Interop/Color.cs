using SkiaSharp;

namespace ChosenFewFX.NET.Interop
{
    public struct Color
    {
        public byte Red { get; private set; }
        public byte Green { get; private set; }
        public byte Blue { get; private set; }
        public byte Alpha { get; private set; }

        public Color(byte r, byte g, byte b, byte a)
        {
            Red = r;
            Green = g;
            Blue = b;
            Alpha = a;
        }

        public Color(byte r, byte g, byte b)
        {
            Red = r;
            Green = g;
            Blue = b;
            Alpha = 255;
        }

        public static implicit operator SKColor(Color c)
        {
            return new SKColor(c.Red, c.Green, c.Blue, c.Alpha);
        }
    }
}
