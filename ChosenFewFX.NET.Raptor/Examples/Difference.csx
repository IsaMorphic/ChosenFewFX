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
// Changes the colors of the image in interesting and dynamic ways.

// Parameter Description:
// Color Parameter (Color): The color that the value of each pixel is subracted from

// HOW THIS FILTER WORKS:
// For each pixel in the source image:
// Subtract each component of the current pixel from each component of the Color parameter.
// Construct a new pixel from these differences.
// Write the new pixel to the destination image (in the same place as the current pixel).

(api, src, x, y) =>
{
    int newRed = api.Param.Color.Red - src.Red;
    int newGreen = api.Param.Color.Green - src.Green;
    int newBlue = api.Param.Color.Blue - src.Blue;
    return new Pixel((byte)newRed, (byte)newGreen, (byte)newBlue);
}