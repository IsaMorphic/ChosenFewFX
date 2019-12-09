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
using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;

namespace ChosenFewFX.NET.Plugins
{
    public class StutterMosherPlugin : BasePlugin
    {
        private Process Process { get; set; }

        public StutterMosherPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.stuttermosher";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "StutterMosher (Extension)";
        }

        public override void PostProcess()
        {
            if (Process == null || Process.HasExited)
            {
                string tempPath = Path.GetTempPath();
                string path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string exePath = Path.Combine(path, "StutterMosher.WinForms.exe");
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = exePath,
                    WorkingDirectory = tempPath
                };
                Process = Process.Start(startInfo);
            }
        }
    }
}
