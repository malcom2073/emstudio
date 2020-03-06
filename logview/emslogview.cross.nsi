; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

; The name of the installer
Name "EMSLogView"

; The file to write
OutFile "EMSLogViewInstaller.exe"

; The default installation directory
InstallDir $PROGRAMFILES\EMSLogView

InstallDirRegKey HKLM "Software\EMSLogView" "Install_Dir"

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
Section "EMSLogView (Required)" ;No components page, name is not important

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  ; These files should be local to this script
  ;File "/home/michael/code/emstudio/release/emstudio.exe"
  ;File "/home/michael/code/emstudio/src/gauges.qml"
  ;File "/home/michael/code/emstudio/freeems.config.json"
  File "release/emslogview.exe"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\EMSLogView "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMSLogView" "DisplayName" "EMSLogView"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMSLogView" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMSLogView" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMSLogView" "NoRepair" 1
  WriteUninstaller "uninstall.exe"  
SectionEnd ; end the section

Section "Qt Components"

  SetOutPath $INSTDIR

  File /home/michael/QtWin32/lib/QtCore4.dll
  File /home/michael/QtWin32/lib/QtGui4.dll
  File /home/michael/QtWin32/lib/QtOpenGL4.dll
  File /home/michael/QtWin32/lib/QtSvg4.dll
  File /home/michael/QtWin32/lib/QtDeclarative4.dll
  File /home/michael/QtWin32/lib/QtGui4.dll
  File /home/michael/QtWin32/lib/QtNetwork4.dll
  File /home/michael/QtWin32/lib/QtScript4.dll
  File /home/michael/QtWin32/lib/QtSql4.dll
  File /home/michael/QtWin32/lib/QtXml4.dll
  File /home/michael/QtWin32/lib/QtXmlPatterns4.dll
  File /home/michael/QtWin32/libs/qwt/bin/qwt.dll
  File /home/michael/QtWin32/libs/qjson/bin/qjson.dll
  File /home/michael/QtWin32/lib/libgcc_s_sjlj-1.dll
  File /home/michael/QtWin32/lib/libstdc++-6.dll
  File /home/michael/QtWin32/lib/libwinpthread-1.dll

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\EMSLogView"
  CreateShortCut "$SMPROGRAMS\EMSLogView\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\EMSLogView\EMSLogView.lnk" "$INSTDIR\emslogview.exe" "" "$INSTDIR\emslogview.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\EMSLogView"
  DeleteRegKey HKLM SOFTWARE\EMSLogView

  ; Remove files and uninstaller
  Delete $INSTDIR\emslogview.exe
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\*.*"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\EMSLogView\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\EMSLogView"
  RMDir "$INSTDIR"

SectionEnd
