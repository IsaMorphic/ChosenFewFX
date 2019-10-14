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
using ChosenFewFX.NET.Interop;
using SharpDX;
using SkiaSharp;
using SoftEngine;
using System;
using System.Collections.Generic;
using System.IO;

namespace ChosenFewFX.NET.Plugins
{
    public class SoftEnginePlugin : FilterPlugin
    {
        private Device Device { get; set; }
        private Camera Camera { get; set; }

        private Mesh Mesh { get; set; }

        private Dictionary<string, Mesh> ModelCache { get; }

        [StringParam(DefaultValue = "", Hint = "OBJ Model file to render", Label = "OBJ 3D Model File", StringType = StringType.FilePath)]
        public string ModelPath;

        [RangeParam(DefaultValue = 0.0, Hint = "The camera's translation in the left/right direction", Label = "Camera Translation X", MaximumValue = 500.0, MinimumValue = -500.0)]
        public double CameraX;

        [RangeParam(DefaultValue = 0.0, Hint = "The camera's translation in the up/down direction", Label = "Camera Translation Y", MaximumValue = 500.0, MinimumValue = -500.0)]
        public double CameraY;

        [RangeParam(DefaultValue = 250.0, Hint = "The camera's translation in the direction pointing away/towards the screen", Label = "Camera Translation Z", MaximumValue = 500.0, MinimumValue = -500.0)]
        public double CameraZ;

        [RangeParam(DefaultValue = 0.0, Hint = "Model Yaw", Label = "Model Yaw", MaximumValue = Math.PI * 2, MinimumValue = -Math.PI * 2)]
        public double ModelYaw;

        [RangeParam(DefaultValue = 0.0, Hint = "Model Pitch", Label = "Model Pitch", MaximumValue = Math.PI * 2, MinimumValue = -Math.PI * 2)]
        public double ModelPitch;

        [RangeParam(DefaultValue = 0.0, Hint = "Model Roll", Label = "Model Roll", MaximumValue = Math.PI * 2, MinimumValue = -Math.PI * 2)]
        public double ModelRoll;


        [RangeParam(DefaultValue = 0.0, Hint = "The model's translation in the left/right direction", Label = "Model Translation X", MaximumValue = 500.0, MinimumValue = -500.0)]
        public double ModelX;

        [RangeParam(DefaultValue = 0.0, Hint = "The model's translation in the up/down direction", Label = "Model Translation Y", MaximumValue = 500.0, MinimumValue = -500.0)]
        public double ModelY;

        [RangeParam(DefaultValue = 0.0, Hint = "The model's translation in the direction pointing away/towards the screen", Label = "Model Translation Z", MaximumValue = 500.0, MinimumValue = -500.0)]
        public double ModelZ;

        public SoftEnginePlugin()
        {
            Id = "com.chosenfewsoftware.openfx.softengine";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "SoftEngine 3D";

            ModelCache = new Dictionary<string, Mesh>();
        }

        public override void PreProcess()
        {
            Device = new Device(DestImage.Width, DestImage.Height);
            Device.Clear(0, 0, 0, 0);

            Camera = new Camera();

            Device.LightPos = Camera.Position = new Vector3((float)CameraX, (float)CameraY, (float)CameraZ);

            LoadModel();

            if (Mesh == null)
                return;

            Mesh.Texture = new Texture(SourceImage);

            Mesh.Position = new Vector3((float)ModelX, (float)ModelY, (float)ModelZ);
            Mesh.Rotation = new Vector3((float)ModelYaw, (float)ModelPitch, (float)ModelRoll);

            Device.Render(Camera, new Mesh[] { Mesh });
        }

        public override void PostProcess()
        {
            SKBitmap rendered = Device.Present();
            for (int y = 0; y < DestImage.Height; y++)
            {
                for (int x = 0; x < DestImage.Width; x++)
                {
                    SKColor pixel = rendered.GetPixel(x, DestImage.Height - (y + 1));
                    DestImage.SetPixel(x, y, pixel);
                }
            }
            Mesh = null;
        }

        private void LoadModel()
        {
            if (ModelCache.ContainsKey(ModelPath))
            {
                Mesh = ModelCache[ModelPath];
            }
            else if (File.Exists(ModelPath))
            {
                Mesh mesh = Mesh.LoadOBJ(ModelPath);
                ModelCache.Add(ModelPath, mesh);
                Mesh = mesh;
            }
        }
    }
}
