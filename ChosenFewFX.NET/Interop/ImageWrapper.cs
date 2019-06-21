using SkiaSharp;

namespace ChosenFewFX.NET.Interop
{
    public class ImageWrapper
    {
        SKBitmap Image;

        public ImageWrapper(SKBitmap image)
        {
            Image = image;
        }

        public static implicit operator SKBitmap(ImageWrapper wrapper)
        {
            return wrapper.Image;
        }
    }
}
