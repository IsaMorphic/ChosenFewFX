using System;
using System.Collections.Generic;
using System.Text;

namespace ChosenFewFX.NET
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
