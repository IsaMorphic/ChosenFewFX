; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Chosen Few FX"
#define MyAppVersion "1.0.2"
#define MyAppPublisher "Chosen Few Software"
#define MyAppURL "https://www.chosenfewsoftware.com"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{17A04035-B0B7-4F24-8163-D2918C5287B8}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
DefaultDirName={pf}\VEGAS\VEGAS Pro 14.0\OFX Video Plug-Ins\
InfoBeforeFile=INSTALL_README.md
LicenseFile=..\LICENSE.md
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultGroupName={#MyAppName}
OutputDir=.\bin\
OutputBaseFilename=ChosenFewFX
Compression=lzma
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64 ia64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\Release-Win32\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: not IsWin64
Source: "..\Release-x64\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Check: IsWin64
Source: "..\ChosenFewFX.NET\bin\Release\net461\*.dll"; DestDir: "{app}\ChosenFewFX.ofx.bundle\Contents\"; Flags: ignoreversion;
Source: "..\LICENSE.md"; DestDir: "{app}\ChosenFewFX.ofx.bundle\"; Flags: ignoreversion;

Source: "..\openfx\Support\LICENSE"; DestDir: "{app}\ChosenFewFX.ofx.bundle\"; DestName: "OFX-SUPPORT.md"; Flags: ignoreversion;

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

