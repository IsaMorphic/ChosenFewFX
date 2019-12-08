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