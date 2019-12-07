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
using ChosenFewFX.NET.Raptor;
using Microsoft.CodeAnalysis.CSharp.Scripting;
using Microsoft.CodeAnalysis.Scripting;
using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace ChosenFewFX.NET.Plugins
{
    public class RaptorScriptingPlugin : RaptorBasePlugin
    {
        private Dictionary<string, Func<Api, Pixel, int, int, Pixel>> Scripts { get; set; }

        [StringParam(DefaultValue = "", Hint = "Path to Raptor script file", Label = "Raptor Script File (.CSX)", StringType = StringType.FilePath)]
        public string ScriptPath;

        public RaptorScriptingPlugin()
        {
            Id = "com.chosenfewsoftware.openfx.raptor_scripting";
            MajorVersion = 1;
            MinorVersion = 0;
            Name = "Raptor (Scripting)";
            Scripts = new Dictionary<string, Func<Api, Pixel, int, int, Pixel>>();
            FilterFunc = DefaultFilter;
        }

        public override void PreProcess()
        {
            LoadScript();
        }

        public override void PostProcess()
        {
            FilterFunc = DefaultFilter;
        }

        public void LoadScript()
        {
            if (Scripts.ContainsKey(ScriptPath))
            {
                FilterFunc = Scripts[ScriptPath];
            }
            else if(File.Exists(ScriptPath))
            {
                try
                {
                    string scriptCode = File.ReadAllText(ScriptPath);

                    ScriptOptions scriptOptions = ScriptOptions.Default
                        .AddReferences(typeof(Api).Assembly)
                        .AddImports("System", "ChosenFewFX.NET.Raptor");

                    Func<Api, Pixel, int, int, Pixel> scriptFunc = 
                        CSharpScript.EvaluateAsync<Func<Api, Pixel, int, int, Pixel>>
                        (scriptCode, scriptOptions).Result;

                    Scripts.Add(ScriptPath, scriptFunc);
                    FilterFunc = scriptFunc;

                    EncounteredError = false;
                }
                catch (CompilationErrorException e)
                {
                    MessageBox.Show($"Script compilation failed!\n{e.Message}", 
                        "Compilation Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
    }
}
