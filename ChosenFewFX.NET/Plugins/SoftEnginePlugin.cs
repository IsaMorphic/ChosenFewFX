using ChosenFewFX.NET.Interop;
using SharpDX;
using SkiaSharp;
using SoftEngine;
using System;

namespace ChosenFewFX.NET.Plugins
{
    public class SoftEnginePlugin : BasePlugin
    {
        private Device Device { get; set; }
        private Camera Camera { get; set; }
        private Mesh Mesh { get; set; }

        [StringParam(DefaultValue = "", Hint = "OBJ Model file to render", Label = "OBJ 3D Model File", StringType = StringType.FilePath)]
        public string ModelPath;

        [StringParam(DefaultValue = "", Hint = "Texture to use with model (only one texture is currently supported)", Label = "Texture File", StringType = StringType.FilePath)]
        public string TexturePath;

        [RangeParam(DefaultValue = 0.0, Hint = "The camera's translation in the left/right direction", Label = "Camera Translation X", MaximumValue = 1000.0, MinimumValue = -1000.0)]
        public double CameraX;

        [RangeParam(DefaultValue = 0.0, Hint = "The camera's translation in the up/down direction", Label = "Camera Translation Y", MaximumValue = 1000.0, MinimumValue = -1000.0)]
        public double CameraY;

        [RangeParam(DefaultValue = 0.0, Hint = "The camera's translation in the direction pointing away/towards the screen", Label = "Camera Translation Z", MaximumValue = 1000.0, MinimumValue = -1000.0)]
        public double CameraZ;

        [RangeParam(DefaultValue = 0.0, Hint = "Model Yaw", Label = "Model Yaw", MaximumValue = Math.PI * 2, MinimumValue = 0.0)]
        public double ModelYaw;

        [RangeParam(DefaultValue = 0.0, Hint = "Model Pitch", Label = "Model Pitch", MaximumValue = Math.PI * 2, MinimumValue = 0.0)]
        public double ModelPitch;

        [RangeParam(DefaultValue = 0.0, Hint = "Model Roll", Label = "Model Roll", MaximumValue = Math.PI * 2, MinimumValue = 0.0)]
        public double ModelRoll;


        [RangeParam(DefaultValue = 0.0, Hint = "The model's translation in the left/right direction", Label = "Model Translation X", MaximumValue = 1000.0, MinimumValue = -1000.0)]
        public double ModelX;

        [RangeParam(DefaultValue = 0.0, Hint = "The model's translation in the up/down direction", Label = "Model Translation Y", MaximumValue = 1000.0, MinimumValue = -1000.0)]
        public double ModelY;

        [RangeParam(DefaultValue = 0.0, Hint = "The model's translation in the direction pointing away/towards the screen", Label = "Model Translation Z", MaximumValue = 1000.0, MinimumValue = -1000.0)]
        public double ModelZ;

        public SoftEnginePlugin()
        {
            Id = "com.chosenfewsoftware.openfx.softengine";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "SoftEngine 3D";
        }

        public override void PreProcess()
        {
            Device = new Device(DestImage.Width, DestImage.Height);
            Device.Clear(0, 0, 0, 0);

            Camera = new Camera();

            Device.LightPos = Camera.Position = new Vector3((float)CameraX, (float)CameraY, (float)CameraZ);

            if (Mesh == null)
                return;

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
                    DestImage.SetPixel(x, y, new SKColor(pixel.Blue, pixel.Green, pixel.Red, pixel.Alpha));
                }
            }
        }

        public override void ParamUpdated(string paramName)
        {
            switch (paramName)
            {
                case "ModelPath":
                    if (!string.IsNullOrEmpty(ModelPath))
                        Mesh = Mesh.LoadOBJ(ModelPath);
                    break;
                case "TexturePath":
                    if (!string.IsNullOrEmpty(TexturePath))
                        Mesh.Texture = new Texture(TexturePath);
                    break;
            }
        }
    }
}
