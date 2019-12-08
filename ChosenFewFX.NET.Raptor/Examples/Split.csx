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
// seemingly "fractures" the source image along a pattern of horizontal and vertical lines.  

// Parameter Description:
// Primary Parameter (horizontalAmount): determines how much to displace certain pixels horizontally
// Secondary Parameter (verticalAmount): determines how much to displace certain pixels vertically

// HOW THIS SCRIPT WORKS:
// For each pixel in the source image:
// Suppose there is a little "mouse" that is sitting on the current pixel.
// Divide the current pixel's X and Y coordinates by horizontalAmount and verticalAmount, respectively.
// Then, check if the remainder of each division is less than half of the divisor.
// Depending on whether this is true for the X coordinate, the mouse steps horizontalAmount pixels to the left or to the right of the current pixel.
// Depending on whether this is true for the Y coordinate, the mouse steps verticalAmount pixels above or below the current pixel.
// Finally, write the pixel that the "mouse" has ultimately landed on to the destination image (in the same place as the current pixel).

using static System.Math;

(api, src, x, y) =>
{
    int horizontalAmount = (int)api.Param.Primary;
    int verticalAmount = (int)api.Param.Secondary;

    int randX = x;
    int randY = y;

    if (horizontalAmount != 0)
        randX = x % horizontalAmount < horizontalAmount / 2 ? x - horizontalAmount : x + horizontalAmount;
    if (verticalAmount != 0)
        randY = y % verticalAmount < verticalAmount / 2 ? y - verticalAmount : y + verticalAmount;

    int clampedX = Min(Max(randX, 0), api.Image.Width);
    int clampedY = Min(Max(randY, 0), api.Image.Height);

    return api.Image.Read(clampedX, clampedY);
}
