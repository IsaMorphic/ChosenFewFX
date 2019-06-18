using System;
using System.Collections.Generic;
using System.Text;

namespace ChosenFewFX.NET
{
    public struct Rectangle
    {
        public Point TopLeft { get; private set; }
        public Point BottomRight { get; private set; }

        public double Width => BottomRight.X - TopLeft.X;
        public double Height => BottomRight.Y - TopLeft.Y;

        public Point BottomLeft => new Point(TopLeft.X, BottomRight.Y);
        public Point TopRight => new Point(BottomRight.X, TopLeft.Y);

        public Point Center => new Point(TopLeft.X + Width / 2, TopLeft.Y + Height / 2);

        public Rectangle(Point topLeft, Point bottomRight)
        {
            TopLeft = topLeft;
            BottomRight = bottomRight;
        }

        public Rectangle(double x1, double y1, double x2, double y2)
        {
            TopLeft = new Point(x1, y1);
            BottomRight = new Point(x2, y2);
        }
    }
}
