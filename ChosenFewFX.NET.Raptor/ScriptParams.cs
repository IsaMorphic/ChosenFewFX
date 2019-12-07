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
namespace ChosenFewFX.NET.Raptor
{
    public class ScriptParams
    {
        public double Primary { get; }
        public double Secondary { get; }

        public Pixel Color { get; }

        public ScriptParams(double primary, double secondary, Pixel color)
        {
            Primary = primary;
            Secondary = secondary;
            Color = color;
        }
    }
}
