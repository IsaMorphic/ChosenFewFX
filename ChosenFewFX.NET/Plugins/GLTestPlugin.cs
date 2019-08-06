using ChosenFewFX.NET.Geometry;
using ChosenFewFX.NET.Interop;
using OpenGL;
using System;

namespace ChosenFewFX.NET.Plugins
{
    public class GLTestPlugin : GLPlugin
    {
        public GLTestPlugin() : base()
        {
            Id = "com.chosenfewsoftware.openfx.opengltest";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "OpenGL Test";
        }

        public override void ProcessPixels(RectangleI region)
        {
            Gl.Viewport(0, 0, DestImage.Width, DestImage.Height);
            Gl.Clear(ClearBufferMask.ColorBufferBit);

            Gl.MatrixMode(MatrixMode.Projection);
            Gl.LoadIdentity();
            Gl.Ortho(0.0, 1.0f, 0.0, 1.0, 0.0, 1.0);
            Gl.MatrixMode(MatrixMode.Modelview);
            Gl.LoadIdentity();

            Gl.Begin(PrimitiveType.Triangles);
            Gl.Color3(1.0f, 0.0f, 0.0f); Gl.Vertex2(0.0f, 0.0f);
            Gl.Color3(0.0f, 1.0f, 0.0f); Gl.Vertex2(0.5f, 1.0f);
            Gl.Color3(0.0f, 0.0f, 1.0f); Gl.Vertex2(1.0f, 0.0f);
            Gl.End();
        }
    }
}
