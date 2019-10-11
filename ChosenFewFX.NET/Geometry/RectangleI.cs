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
namespace ChosenFewFX.NET.Geometry
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

        public static implicit operator RectangleD(RectangleI rect)
        {
            return new RectangleD(rect.TopLeft, rect.BottomRight);
        }
    }
}
