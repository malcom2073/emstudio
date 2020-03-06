; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

; The name of the installer
Name "EMStudio"

; The file to write
OutFile "EMStudioInstaller.exe"

; The default installation directory
InstallDir $PROGRAMFILES\EMStudio

InstallDirRegKey HKLM "Software\EMStudio" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "EMStudio (Required)" ;No components page, name is not important

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  ; These files should be local to this script
  ;File "/home/michael/code/emstudio/release/emstudio.exe"
  ;File "/home/michael/code/emstudio/src/gauges.qml"
  ;File "/home/michael/code/emstudio/freeems.config.json"
  File "core/release/emstudio.exe"
  
  

  SetOutPath "$INSTDIR\definitions"
  File "core/freeems.config.json"
  File "core/decodersettings.json"
  SetOutPath "$INSTDIR\dashboards"
  File "core/src/gauges.qml"
  SetOutPath "$INSTDIR\wizards"
  File "core/wizards/BenchTest.qml"
  File "core/wizards/DecoderOffset.qml"
  File "core/wizards/wizard.qml"
  SetOutPath "$INSTDIR\plugins"
!ifndef PLUGIN
  File "core/plugins/libfreeemsplugin.a"
!else
  File "core/plugins/${PLUGIN}"
!endif
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\EMStudio "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMStudio" "DisplayName" "EMStudio"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMStudio" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMStudio" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FreeTune" "NoRepair" 1
  WriteUninstaller "uninstall.exe"  
SectionEnd ; end the section

Section "Qt Components"

  SetOutPath $INSTDIR

  File /home/michael/QtWin64/lib/QtCore4.dll
  File /home/michael/QtWin64/lib/QtGui4.dll
  File /home/michael/QtWin64/lib/QtOpenGL4.dll
  File /home/michael/QtWin64/lib/QtSvg4.dll
  File /home/michael/QtWin64/lib/QtDeclarative4.dll
  File /home/michael/QtWin64/lib/QtGui4.dll
  File /home/michael/QtWin64/lib/QtNetwork4.dll
  File /home/michael/QtWin64/lib/QtScript4.dll
  File /home/michael/QtWin64/lib/QtSql4.dll
  File /home/michael/QtWin64/lib/QtXml4.dll
  File /home/michael/QtWin64/lib/QtXmlPatterns4.dll
  File /home/michael/QtWin64/libs/qjson/bin/qjson.dll
  File /home/michael/QtWin64/lib/libstdc++-6.dll
  File /home/michael/QtWin64/lib/libgcc_s_sjlj-1.dll
  File /home/michael/QtWin64/lib/libwinpthread-1.dll

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\EMStudio"
  CreateShortCut "$SMPROGRAMS\EMStudio\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\EMStudio\EMStudio.lnk" "$INSTDIR\EMStudio.exe" "" "$INSTDIR\EMStudio.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMStudio"
  DeleteRegKey HKLM SOFTWARE\EMStudio

  ; Remove files and uninstaller
  Delete $INSTDIR\EMStudio.exe
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\*.*"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\EMStudio\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\EMStudio"
  RMDir "$INSTDIR"

SectionEnd
