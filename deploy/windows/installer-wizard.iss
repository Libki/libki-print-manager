#define AppVer GetFileVersion('LibkiPrintManager.exe')

[Setup]
AppName=Libki Print Manager
AppVersion={#AppVer}
AppPublisher=Kyle M Hall
AppPublisherURL=http://kylehall.info/
AppSupportURL=http://libki.org/
AppUpdatesURL=http://libki.org/
DefaultDirName={pf}\Libki
DefaultGroupName=Libki Print Manager
OutputBaseFilename=Libki_Print_Manager_Installer
Compression=lzma
AllowNoIcons=yes

[Files]
Source: "LibkiPrintManager.exe"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0

Source: "C:\Qt\5.5\mingw492_32\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\5.5\mingw492_32\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\5.5\mingw492_32\bin\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\5.5\mingw492_32\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\5.5\mingw492_32\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\5.5\mingw492_32\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\5.5\mingw492_32\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\5.5\mingw492_32\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\Tools\mingw492_32\opt\bin\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0
Source: "C:\Qt\Tools\mingw492_32\opt\bin\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion; MinVersion: 0.0,5.0

[Registry]
Root: "HKLM"; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: String; ValueName: "Libki"; ValueData: "{app}\LibkiPrintManager.exe"; Flags: createvalueifdoesntexist uninsdeletekey; MinVersion: 0.0,5.0;

[CustomMessages]
NameAndVersion=%1 version %2
AdditionalIcons=Additional icons:
CreateDesktopIcon=Create a &desktop icon
CreateQuickLaunchIcon=Create a &Quick Launch icon
ProgramOnTheWeb=%1 on the Web
UninstallProgram=Uninstall %1
LaunchProgram=Launch %1
AssocFileExtension=&Associate %1 with the %2 file extension
AssocingFileExtension=Associating %1 with the %2 file extension...

[INI]
Filename: "{commonappdata}\Libki\Libki Print Manager.ini"; Section: "settings"; Key: "server"; String: "{code:GetScheme}://{code:GetHost}:{code:GetPort}"

[Code]
var
  ServerPage: TInputQueryWizardPage;

procedure InitializeWizard;
begin
  { Create the pages }
  
  ServerPage := CreateInputQueryPage(wpWelcome,
    'Server Information', 'Libki server data',
    'Please specify the Libki server data.');
  ServerPage.Add('Scheme:', False);
  ServerPage.Add('Host:', False);
  ServerPage.Add('Port:', False);
  
  { Set default values, using settings that were stored last time if possible }
end;

function GetScheme(Param: String): String;
begin
  Result := ServerPage.Values[0];
end;

function GetHost(Param: String): String;
begin
  Result := ServerPage.Values[1];
end;

function GetPort(Param: String): String;
begin
  Result := ServerPage.Values[2];
end;
