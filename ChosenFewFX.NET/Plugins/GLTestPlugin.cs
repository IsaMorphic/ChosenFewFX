using ChosenFewFX.NET.Geometry;
using ChosenFewFX.NET.Interop;
using OpenGL;
using OpenGL.Objects.Scene;
using OpenGL.Objects.State;
using System;

namespace ChosenFewFX.NET.Plugins
{
    public class GLTestPlugin : GLPlugin
    {
        private SceneGraph SceneGraph { get; }

        private SceneObjectLightDirectional GlobalLightObject { get; }

        private SceneObject Model { get; set; }

        public GLTestPlugin() : base()
        {
            Id = "com.chosenfewsoftware.openfx.opengltest";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "OpenGL Test";
            
            SceneGraph = new SceneGraph(SceneGraphFlags.CullingViewFrustum | SceneGraphFlags.StateSorting | SceneGraphFlags.Lighting | SceneGraphFlags.ShadowMaps);
            SceneGraph.SceneRoot = new SceneObjectGeometry();
            SceneGraph.SceneRoot.ObjectState.DefineState(new DepthTestState(DepthFunction.Less));

            SceneGraph.CurrentView = new SceneObjectCamera();
            SceneGraph.SceneRoot.Link(SceneGraph.CurrentView);

            SceneObjectLightZone globalLightZone = new SceneObjectLightZone();

            GlobalLightObject = new SceneObjectLightDirectional();
            GlobalLightObject.Direction = (-Vertex3f.UnitX + Vertex3f.UnitY - Vertex3f.UnitZ).Normalized;
            globalLightZone.Link(GlobalLightObject);

            SceneGraph.SceneRoot.Link(globalLightZone);


            Model = SceneObjectCodec.Instance.Load("D:\\cube.obj");
            Model.ObjectState.DefineState(new CullFaceState(FrontFaceDirection.Ccw, CullFaceMode.Back));

            globalLightZone.Link(Model);
            SceneGraph.Create(Context);
        }

        public override void ProcessPixels(RectangleI region)
        {
            Gl.Viewport(0, 0, DestImage.Width, DestImage.Height);
            Gl.Clear(ClearBufferMask.ColorBufferBit);

            SceneGraph.Draw(Context);
        }
    }
}
