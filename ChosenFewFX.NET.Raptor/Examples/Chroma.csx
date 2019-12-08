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
// Chromatic abberation filter, splits image into component colors and offsets them.  

// Parameter Description:
// Primary Parameter (Amount): determines the amount of displacement across color channels.
// Secondary Parameter (Angle): determines the angle of displacement.  

// HOW THIS FILTER WORKS:
// Calculate the x and y offsets based on Angle and Amount using trigonometry.
// For each pixel in the source image:
// Construct a new pixel using:
// * the red value from a neighboring pixel (x - offsetX, y - offsetY).
// * the green value of the current pixel.
// * the blue value from a neighboring pixel in the opposite direction of the "red pixel" (x + offsetX, y + offsetY).
// And write the new pixel to the destination image (in the same place as the current pixel).

using static System.Math;

(api, src, x, y) =>
{
    double offsetX = api.Param.Primary * Cos(api.Param.Secondary);
    double offsetY = api.Param.Primary * Sin(api.Param.Secondary);

    int redX = Min(Max(x - (int)offsetX, 0), api.Image.Width);
    int redY = Min(Max(y - (int)offsetY, 0), api.Image.Height);

    int blueX = Min(Max(x + (int)offsetX, 0), api.Image.Width);
    int blueY = Min(Max(y + (int)offsetY, 0), api.Image.Height);

    Pixel redPixel = api.Image.Read(redX, redY);
    Pixel bluePixel = api.Image.Read(blueX, blueY);

    return new Pixel(redPixel.Red, src.Green, bluePixel.Blue);
}