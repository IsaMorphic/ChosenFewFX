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
