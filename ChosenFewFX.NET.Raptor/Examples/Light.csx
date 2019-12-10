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

// Filter Description:
// Changes the color of the ambient light in the image.

// Parameter Description:
// Color: determines the new color for the ambient light in the image.

// HOW THIS FILTER WORKS:
// Really not sure exactly how this filter works, I wrote the actual algorithm years ago. I know it has something 
// to do with dividing each pixel by the average color in the image and then multiplying it by the desired light color, 
// or something of the sort.  

using static System.Math;

(api, src, x, y) =>
{
    double len = Sqrt(src.Red * src.Red + src.Green * src.Green + src.Blue * src.Blue);

    Pixel avg = api.Image.AverageColor;
    Pixel light = api.Param.Color;

    double avgRed = avg.Red / len * 128;
    double avgGreen = avg.Green / len * 128;
    double avgBlue = avg.Blue / len * 128;
    double avgAlpha = avg.Alpha / len * 128;

    double newRed = (light.Red + avgRed) / 2 * (src.Red / avgRed);
    double newGreen = (light.Green + avgGreen) / 2 * (src.Green / avgGreen);
    double newBlue = (light.Blue + avgBlue) / 2 * (src.Blue / avgBlue);

    return new Pixel(
        (byte)Math.Min(newRed, 255), 
        (byte)Math.Min(newGreen, 255), 
        (byte)Math.Min(newBlue, 255)
        );
}