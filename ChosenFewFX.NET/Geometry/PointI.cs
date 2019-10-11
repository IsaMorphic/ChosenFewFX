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
    public struct PointI
    {
        public int X { get; private set; }
        public int Y { get; private set; }

        public PointI(int x, int y)
        {
            X = x;
            Y = y;
        }

        public static PointI operator +(PointI left, PointI right)
        {
            return new PointI(left.X + right.X, left.Y + right.Y);
        }

        public static PointI operator -(PointI left, PointI right)
        {
            return new PointI(left.X - right.X, left.Y - right.Y);
        }

        public static PointI operator /(PointI left, int right)
        {
            return new PointI(left.X / right, left.Y / right);
        }

        public static PointI operator *(PointI left, int right)
        {
            return new PointI(left.X * right, left.Y * right);
        }

        public static implicit operator PointD(PointI point)
        {
            return new PointD(point.X, point.Y);
        }
    }
}
