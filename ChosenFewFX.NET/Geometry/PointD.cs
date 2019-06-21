
namespace ChosenFewFX.NET.Geometry
{
    public struct PointD
    {
        public double X { get; private set; }
        public double Y { get; private set; }

        public PointD(double x, double y)
        {
            X = x;
            Y = y;
        }

        public static PointD operator +(PointD left, PointD right)
        {
            return new PointD(left.X + right.X, left.Y + right.Y);
        }

        public static PointD operator -(PointD left, PointD right)
        {
            return new PointD(left.X - right.X, left.Y - right.Y);
        }

        public static PointD operator /(PointD left, double right)
        {
            return new PointD(left.X / right, left.Y / right);
        }

        public static PointD operator *(PointD left, double right)
        {
            return new PointD(left.X * right, left.Y * right);
        }

        public static explicit operator PointI(PointD point)
        {
            return new PointI((int)point.X, (int)point.Y);
        }
    }
}
