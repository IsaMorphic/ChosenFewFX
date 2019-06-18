using System;
using System.Collections.Generic;
using System.Text;

namespace ChosenFewFX.NET
{
    public struct Point
    {
        public double X { get; private set; }
        public double Y { get; private set; }

        public Point(double x, double y)
        {
            X = x;
            Y = y;
        }

        public static Point operator +(Point left, Point right)
        {
            return new Point(left.X + right.X, left.Y + right.Y);
        }

        public static Point operator -(Point left, Point right)
        {
            return new Point(left.X - right.X, left.Y - right.Y);
        }

        public static Point operator /(Point left, double right)
        {
            return new Point(left.X / right, left.Y / right);
        }

        public static Point operator *(Point left, double right)
        {
            return new Point(left.X * right, left.Y * right);
        }
    }
}
