using System;
using System.Collections.Generic;
using System.Text;

namespace ChosenFewFX.NET
{
    public struct RectangleI
    {
        public PointI TopLeft { get; private set; }
        public PointI BottomRight { get; private set; }

        public int Width => BottomRight.X - TopLeft.X;
        public int Height => BottomRight.Y - TopLeft.Y;

        public PointI BottomLeft => new PointI(TopLeft.X, BottomRight.Y);
        public PointI TopRight => new PointI(BottomRight.X, TopLeft.Y);

        public PointI Center => new PointI(TopLeft.X + Width / 2, TopLeft.Y + Height / 2);

        public RectangleI(PointI topLeft, PointI bottomRight)
        {
            TopLeft = topLeft;
            BottomRight = bottomRight;
        }

        public RectangleI(int x1, int y1, int x2, int y2)
        {
            TopLeft = new PointI(x1, y1);
            BottomRight = new PointI(x2, y2);
        }
    }
}
