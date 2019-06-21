
namespace ChosenFewFX.NET.Geometry
{
    public struct RectangleD
    {
        public PointD TopLeft { get; private set; }
        public PointD BottomRight { get; private set; }

        public double Width => BottomRight.X - TopLeft.X;
        public double Height => BottomRight.Y - TopLeft.Y;

        public PointD BottomLeft => new PointD(TopLeft.X, BottomRight.Y);
        public PointD TopRight => new PointD(BottomRight.X, TopLeft.Y);

        public PointD Center => new PointD(TopLeft.X + Width / 2, TopLeft.Y + Height / 2);

        public RectangleD(PointD topLeft, PointD bottomRight)
        {
            TopLeft = topLeft;
            BottomRight = bottomRight;
        }

        public RectangleD(double x1, double y1, double x2, double y2)
        {
            TopLeft = new PointD(x1, y1);
            BottomRight = new PointD(x2, y2);
        }
    }
}
