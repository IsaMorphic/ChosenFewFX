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
// appears to "tear" the image into four distinct parts, in a noisy and scrambled fashion

// Parameter Description:
// Primary Parameter (horizontalAmount): determines the maximum amount of horizontal displacement for each pixel
// Secondary Parameter (verticalAmount): determines the maximum amount of vertical displacement for each pixel

// HOW THIS SCRIPT WORKS:
// For each pixel in the source image:
// Suppose there is a little "mouse" that is sitting on the current pixel.
// Then flip two coins.
// Depending on how the first coin lands, the mouse steps horizontalAmount pixels to the left or to the right of the current pixel.
// Depending on how the second coin lands, the mouse steps verticalAmount pixels above or below the current pixel.
// Finally, write the pixel that the "mouse" has ultimately landed on to the destination image (in the same place as the current pixel).

using static System.Math;

(api, src, x, y) =>
{
    int horizontalAmount = (int)api.Param.Primary;
    int verticalAmount = (int)api.Param.Secondary;

    int randX = api.Random.NextDouble() < .5 ? x - horizontalAmount : x + horizontalAmount;
    int randY = api.Random.NextDouble() < .5 ? y - verticalAmount : y + verticalAmount;

    int clampedX = Min(Max(randX, 0), api.Image.Width);
    int clampedY = Min(Max(randY, 0), api.Image.Height);

    return api.Image.Read(clampedX, clampedY);
}