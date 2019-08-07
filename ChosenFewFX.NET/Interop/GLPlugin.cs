using OpenGL;
using OpenGL.Objects;
using OpenGL.Objects.Scene;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChosenFewFX.NET.Interop
{
    public class GLPlugin : BasePlugin
    {
        private uint Framebuffer { get; set; }
        private uint Renderbuffer { get; set; }

        protected GraphicsContext Context { get; }

        public GLPlugin()
        {
            Context = new GraphicsContext(DeviceContext.Create());
            Context.MakeCurrent(true);
        }

        public override void PreProcess()
        {
            Context.MakeCurrent(true);

            Renderbuffer = Gl.GenRenderbuffer();
            Gl.BindRenderbuffer(RenderbufferTarget.Renderbuffer, Renderbuffer);
            Gl.RenderbufferStorage(RenderbufferTarget.Renderbuffer, InternalFormat.Rgba8, DestImage.Width, DestImage.Height);

            Framebuffer = Gl.GenFramebuffer();
            Gl.BindFramebuffer(FramebufferTarget.ReadFramebuffer, Framebuffer);
            Gl.BindFramebuffer(FramebufferTarget.Framebuffer, Framebuffer);
            Gl.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, RenderbufferTarget.Renderbuffer, Renderbuffer);

            FramebufferStatus framebufferStatus = Gl.CheckFramebufferStatus(FramebufferTarget.Framebuffer);
            if (framebufferStatus != FramebufferStatus.FramebufferComplete)
                throw new InvalidOperationException("framebuffer not complete");

            Gl.DrawBuffers(Gl.COLOR_ATTACHMENT0);
        }

        public override void PostProcess()
        {
            Gl.ReadBuffer(ReadBufferMode.ColorAttachment0);

            Gl.ReadPixels(0, 0, DestImage.Width, DestImage.Height, PixelFormat.Bgra, PixelType.UnsignedByte, DestImage.GetPixels());

            Gl.DeleteFramebuffers(Framebuffer);
            Gl.DeleteRenderbuffers(Renderbuffer);
        }
    }
}
