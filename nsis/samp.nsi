SetCompressor lzma
!include "WinMessages.nsh"
!addplugindir "resources"

Var GUI0
Var GUI1
Var GUI2

BrandingText "Nullsoft Install System v2.46"
Name "San Andreas Multiplayer 0.3.DL-R1"

LangString LS_LicenseText 1033 "If you accept the terms of the agreement, click I Agree to continue. You must accept the agreement to install $(LS_ProductName)."

LangString LS_I_37 1033 "License Agreement"
LangString LS_I_38 1033 "Please review the license terms before installing $(LS_ProductName)."
LangString LS_I_39 1033 "Press Page Down to see the rest of the agreement."
LangString LS_I_40 1033 "Choose Install Location"
LangString LS_I_41 1033 "Choose the folder in which to install $(LS_ProductName)."
LangString LS_I_42 1033 "Installing"
LangString LS_I_43 1033 "Please wait while $(LS_ProductName) is being installed."
LangString LS_I_44 1033 "Installation Complete"
LangString LS_I_45 1033 "Setup was completed successfully."
LangString LS_I_46 1033 "Installation Aborted"
LangString LS_I_47 1033 "Setup was not completed successfully."
LangString LS_I_48 1033 "&Finish"
LangString LS_I_49 1033 "Your computer must be restarted in order to complete the installation of $(LS_ProductName). Do you want to reboot now?"
LangString LS_I_50 1033 "Reboot now"
LangString LS_I_51 1033 "I want to manually reboot later"
LangString LS_I_52 1033 "$(LS_ProductName) has been installed on your computer.$\r$\n$\r$\nClick Finish to close this wizard."
LangString LS_I_57 1033 "Are you sure you want to quit $(^Name) Setup?"
LangString LS_U_36 1033 "Uninstalling"
LangString LS_U_37 1033 "Please wait while $(LS_ProductName) is being uninstalled."
LangString LS_U_38 1033 "Uninstallation Complete"
LangString LS_U_39 1033 "Uninstall was completed successfully."
LangString LS_U_40 1033 "Uninstallation Aborted"
LangString LS_U_41 1033 "Uninstall was not completed successfully."
LangString LS_U_42 1033 "&Finish"
LangString LS_U_43 1033 "Completing the $(LS_ProductName) Uninstall Wizard"
LangString LS_U_44 1033 "Your computer must be restarted in order to complete the uninstallation of $(LS_ProductName). Do you want to reboot now?"
LangString LS_U_45 1033 "Reboot now"
LangString LS_U_46 1033 "I want to manually reboot later"
LangString LS_U_47 1033 "$(LS_ProductName) has been uninstalled from your computer.$\r$\n$\r$\nClick Finish to close this wizard."
LangString LS_ProductName 1033 "San Andreas Multiplayer 0.3.DL-R1"

OutFile "sa-mp-0.3.DL-R1-install.exe"
Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
InstallColors /windows
LicenseBkColor /windows
SubCaption 4 " "
UninstallSubCaption 2 " "
AutoCloseWindow true
ShowInstDetails hide
ShowUninstDetails hide

InstallDir "$PROGRAMFILES\Rockstar Games\GTA San Andreas\"
InstallDirRegKey HKLM "Software\Rockstar Games\GTA San Andreas\Installation" "ExePath"

DirText "Please select your Grand Theft Auto: San Andreas directory:" "Destination Folder" "B&rowse..." "Select the folder to install $(LS_ProductName) in:"

PageEx license
  Caption " "
  LicenseText "$(LS_LicenseText)"
  LicenseData "..\samp-license.txt"
  PageCallbacks LicensePre LicenseShow LicenseLeave
PageExEnd
PageEx directory
  PageCallbacks DirectoryPre DirectoryShow DirectoryLeave
  Caption " "
PageExEnd
PageEx instfiles
  PageCallbacks InstFilesPre InstFilesShow InstFilesLeave
  Caption " "
PageExEnd
PageEx custom
  PageCallbacks CustomFinishPage CustomFinishLeave
  Caption " "
PageExEnd

PageEx un.instfiles
  Caption " "
  PageCallbacks un.InstFilesPre un.InstFilesShow un.InstFilesLeave
PageExEnd
PageEx un.custom
  Caption " "
  PageCallbacks un.CustomFinishPage un.CustomFinishLeave
PageExEnd

Function LicensePre
  GetDlgItem $GUI0 $HWNDPARENT 1037
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_37)"
  GetDlgItem $GUI0 $HWNDPARENT 1038
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_38)"
FunctionEnd
Function LicenseShow
  FindWindow $GUI0 "#32770" "" $HWNDPARENT
  GetDlgItem $GUI0 $GUI0 1040
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_39)"
FunctionEnd
Function LicenseLeave
FunctionEnd
Function DirectoryPre
  GetDlgItem $GUI0 $HWNDPARENT 1037
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_40)"
  GetDlgItem $GUI0 $HWNDPARENT 1038
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_41)"
FunctionEnd
Function DirectoryShow
FunctionEnd
Function DirectoryLeave
FunctionEnd
Function InstFilesPre
  GetDlgItem $GUI0 $HWNDPARENT 1037
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_42)"
  GetDlgItem $GUI0 $HWNDPARENT 1038
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_43)"
FunctionEnd
Function InstFilesShow
FunctionEnd
Function InstFilesLeave
  IfAbort aborted
    GetDlgItem $GUI0 $HWNDPARENT 1037
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_44)"
    GetDlgItem $GUI0 $HWNDPARENT 1038
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_45)"
    Goto done
  aborted:
    GetDlgItem $GUI0 $HWNDPARENT 1037
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_46)"
    GetDlgItem $GUI0 $HWNDPARENT 1038
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_I_47)"
  done:
    IfAbort finish_return
  finish_return:
FunctionEnd
Function CustomFinishPage
  Push "$(LS_I_48)"
  Call EscapeINI
  Pop $GUI0
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Settings" "NextButtonText" "$GUI0"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 2" "Bottom" "38"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Top" "45"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 2" "Text" "Installation Complete."
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Bottom" "185"

  IfRebootFlag 0 no_reboot
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Bottom" "85"
    Push "$(LS_I_49)"
    Call EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Text" "$GUI0"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Settings" "Numfields" "5"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Type" "RadioButton"
    Push "$(LS_I_50)"
    Call EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Text" "$GUI0"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Left" "120"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Right" "321"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Top" "90"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Bottom" "100"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Type" "RadioButton"
    Push "$(LS_I_51)"
    Call EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Text" "$GUI0"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Left" "120"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Right" "321"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Top" "110"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Bottom" "120"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "State" "1"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "State" "0"
    Goto display
  no_reboot:
    Push "$(LS_I_52)"
    Call EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Text" "$GUI0"
  display:
  LockWindow on
  GetDlgItem $GUI0 $HWNDPARENT 1028
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1256
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1035
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1037
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1038
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1039
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1045
  ShowWindow $GUI0 1
  LockWindow off
  InstallOptions::initDialog "$PLUGINSDIR\ioSpecial.ini"
  Pop $GUI2
  SetCtlColors $GUI2 "" 0xFFFFFF
  GetDlgItem $GUI0 $GUI2 1201
  SetCtlColors $GUI0 "" 0xFFFFFF
  CreateFont $GUI1 "$(^Font)" 12 700
  SendMessage $GUI0 ${WM_SETFONT} $GUI1 0
  GetDlgItem $GUI0 $GUI2 1202
  SetCtlColors $GUI0 "" 0xFFFFFF
  IfRebootFlag 0 skip_radio_colors
    GetDlgItem $GUI0 $GUI2 1203
    SetCtlColors $GUI0 "" 0xFFFFFF
    GetDlgItem $GUI0 $GUI2 1204
    SetCtlColors $GUI0 "" 0xFFFFFF
    Goto skip_radio_colors
  skip_radio_colors:
  Push $0
  InstallOptions::show
  Pop $0
  Pop $0
  LockWindow on
  GetDlgItem $GUI0 $HWNDPARENT 1028
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1256
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1035
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1037
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1038
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1039
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1045
  ShowWindow $GUI0 0
  LockWindow off
FunctionEnd
Function CustomFinishLeave
  IfRebootFlag 0 no_reboot
  ReadINIStr $GUI0 "$PLUGINSDIR\ioSpecial.ini" "Field 4" "State"
  StrCmp $GUI0 "1" 0 done
  Reboot
  done:
  Return
  no_reboot:
FunctionEnd
Function .onGUIInit
  InitPluginsDir
  File /oname=$PLUGINSDIR\ioSpecial.ini "resources\ioSpecial.ini"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Settings" "RTL" "$(^RTL)"
  File /oname=$PLUGINSDIR\modern-wizard.bmp "resources\modern-wizard.bmp"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 1" "Text" "$PLUGINSDIR\modern-wizard.bmp"
  GetDlgItem $GUI0 $HWNDPARENT 1037
  CreateFont $GUI1 "$(^Font)" "$(^FontSize)" 700
  SendMessage $GUI0 ${WM_SETFONT} $GUI1 0
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1038
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1034
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1039
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1028
  SetCtlColors $GUI0 /BRANDING
  GetDlgItem $GUI0 $HWNDPARENT 1256
  SetCtlColors $GUI0 /BRANDING
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(^Branding) "
FunctionEnd
Function .onUserAbort
  MessageBox MB_YESNO|MB_ICONEXCLAMATION "$(LS_I_57)" IDYES confirmed
  Abort
  confirmed:
FunctionEnd
Function EscapeINI
  Exch $0
  Push $1
  Push $2
  Push $3
  Push $4
  StrCpy $1 ""
  StrLen $3 $0
  IntOp $3 $3 - 1
  StrCpy $4 0
  Goto escape_check
  escape_inc:
    IntOp $4 $4 + 1
  escape_check:
    IntCmp $4 $3 0 0 escape_done
    StrCpy $2 $0 1 $4
    StrCmp $2 "\" 0 escape_cr
      StrCpy $2 "\\"
      Goto escape_append
  escape_cr:
    StrCmp $2 "$\r" 0 escape_lf
      StrCpy $2 "\r"
      Goto escape_append
  escape_lf:
    StrCmp $2 "$\n" 0 escape_tab
      StrCpy $2 "\n"
      Goto escape_append
  escape_tab:
    StrCmp $2 "$\t" 0 escape_append
      StrCpy $2 "\t"
  escape_append:
    StrCpy $1 "$1$2"
    Goto escape_inc
    Goto escape_inc
    Goto escape_done
  escape_done:
    StrCpy $0 $1
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Exch $0
FunctionEnd
Function .onVerifyInstDir
  IfFileExists "$INSTDIR\gta_sa.exe" valid
  Abort
  valid:
FunctionEnd
Section "-San Andreas Multiplayer"
  SetOutPath "$INSTDIR"
  File "..\tools\exgui\samp.exe"
  File "..\build\binaries\release\samp.dll"
  File "..\third-party\bass\bass.dll"
  File "..\build\binaries\release\samp.saa"
  File "..\build\binaries\release\rcon.exe"
  File "..\build\binaries\release\samp_debug.exe"
  File "payload\sampgui.png"
  File "payload\mouse.png"
  File "payload\gtaweap3.ttf"
  File "payload\sampaux3.ttf"
  File "..\samp-license.txt"

  Delete "$INSTDIR\models\samp.img"
  Delete "$INSTDIR\models\sampcol.img"
  Delete "$INSTDIR\models\samp.ide"
  Delete "$INSTDIR\models\samp.ipl"

  SetOutPath "$INSTDIR\SAMP"
  File "payload\SAMP\SAMP.img"
  File "payload\SAMP\SAMPCOL.img"
  File "payload\SAMP\SAMP.ide"
  File "payload\SAMP\SAMP.ipl"
  File "payload\SAMP\samaps.txd"
  File "payload\SAMP\blanktex.txd"
  SetOverwrite off
  File "payload\SAMP\custom.img"
  File "payload\SAMP\CUSTOM.ide"
  SetOverwrite on

  SetOutPath "$SYSDIR"
  File "d3dx9_25.dll"
  SetOutPath "$INSTDIR"
  WriteUninstaller "SAMPUninstall.exe"
  CreateDirectory "$SMPROGRAMS\San Andreas Multiplayer"
  CreateShortCut "$SMPROGRAMS\San Andreas Multiplayer\San Andreas Multiplayer.lnk" "$INSTDIR\samp.exe"
  CreateShortCut "$SMPROGRAMS\San Andreas Multiplayer\Uninstall.lnk" "$INSTDIR\SAMPUninstall.exe"
  WriteRegStr HKCR "samp" "" "San Andreas Multiplayer"
  WriteRegStr HKCR "samp" "Url Protocol" ""
  WriteRegStr HKCR "samp\shell\open\command" "" '"$INSTDIR\samp.exe" "%1"'
  WriteRegStr HKCU "Software\SAMP" "gta_sa_exe" "$INSTDIR\gta_sa.exe"
  CreateDirectory "$DOCUMENTS\GTA San Andreas User Files"
  CreateDirectory "$DOCUMENTS\GTA San Andreas User Files\SAMP"
  CopyFiles "$INSTDIR\userdata.dat" "$DOCUMENTS\GTA San Andreas User Files\SAMP"
  Delete "$INSTDIR\userdata.dat"
SectionEnd
Function un.InstFilesPre
  GetDlgItem $GUI0 $HWNDPARENT 1037
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_U_36)"
  GetDlgItem $GUI0 $HWNDPARENT 1038
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_U_37)"
FunctionEnd
Function un.InstFilesShow
FunctionEnd
Function un.InstFilesLeave
  IfAbort aborted
    GetDlgItem $GUI0 $HWNDPARENT 1037
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_U_38)"
    GetDlgItem $GUI0 $HWNDPARENT 1038
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_U_39)"
    Goto done
  aborted:
    GetDlgItem $GUI0 $HWNDPARENT 1037
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_U_40)"
    GetDlgItem $GUI0 $HWNDPARENT 1038
    SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(LS_U_41)"
  done:
FunctionEnd
Function un.CustomFinishPage
  Push "$(LS_U_42)"
  Call un.EscapeINI
  Pop $GUI0
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Settings" "NextButtonText" "$GUI0"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 2" "Bottom" "38"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Top" "45"
  Push "$(LS_U_43)"
  Call un.EscapeINI
  Pop $GUI0
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 2" "Text" "$GUI0"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Bottom" "185"
  IfRebootFlag 0 no_reboot
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Bottom" "85"
    Push "$(LS_U_44)"
    Call un.EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Text" "$GUI0"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Settings" "Numfields" "5"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Type" "RadioButton"
    Push "$(LS_U_45)"
    Call un.EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Text" "$GUI0"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Left" "120"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Right" "321"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Top" "90"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Bottom" "100"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Type" "RadioButton"
    Push "$(LS_U_46)"
    Call un.EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Text" "$GUI0"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Left" "120"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Right" "321"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Top" "110"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "Bottom" "120"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "State" "1"
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 5" "State" "0"
    Goto display
  no_reboot:
    Push "$(LS_U_47)"
    Call un.EscapeINI
    Pop $GUI0
    WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 3" "Text" "$GUI0"
  display:
  LockWindow on
  GetDlgItem $GUI0 $HWNDPARENT 1028
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1256
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1035
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1037
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1038
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1039
  ShowWindow $GUI0 0
  GetDlgItem $GUI0 $HWNDPARENT 1045
  ShowWindow $GUI0 1
  LockWindow off
  InstallOptions::initDialog "$PLUGINSDIR\ioSpecial.ini"
  Pop $GUI2
  SetCtlColors $GUI2 "" 0xFFFFFF
  GetDlgItem $GUI0 $GUI2 1201
  SetCtlColors $GUI0 "" 0xFFFFFF
  CreateFont $GUI1 "$(^Font)" 12 700
  SendMessage $GUI0 ${WM_SETFONT} $GUI1 0
  GetDlgItem $GUI0 $GUI2 1202
  SetCtlColors $GUI0 "" 0xFFFFFF
  IfRebootFlag 0 skip_radio_colors
    GetDlgItem $GUI0 $GUI2 1203
    SetCtlColors $GUI0 "" 0xFFFFFF
    GetDlgItem $GUI0 $GUI2 1204
    SetCtlColors $GUI0 "" 0xFFFFFF
    Goto skip_radio_colors
  skip_radio_colors:
  Push $0
  InstallOptions::show
  Pop $0
  Pop $0
  LockWindow on
  GetDlgItem $GUI0 $HWNDPARENT 1028
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1256
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1035
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1037
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1038
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1039
  ShowWindow $GUI0 1
  GetDlgItem $GUI0 $HWNDPARENT 1045
  ShowWindow $GUI0 0
  LockWindow off
FunctionEnd
Function un.CustomFinishLeave
  IfRebootFlag 0 no_reboot
  ReadINIStr $GUI0 "$PLUGINSDIR\ioSpecial.ini" "Field 4" "State"
  StrCmp $GUI0 "1" 0 done
  Reboot
  done:
  Return
  no_reboot:
FunctionEnd
Function un.onGUIInit
  InitPluginsDir
  File /oname=$PLUGINSDIR\ioSpecial.ini "resources\ioSpecial.ini"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Settings" "RTL" "$(^RTL)"
  File /oname=$PLUGINSDIR\modern-wizard.bmp "resources\modern-wizard.bmp"
  WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 1" "Text" "$PLUGINSDIR\modern-wizard.bmp"
  GetDlgItem $GUI0 $HWNDPARENT 1037
  CreateFont $GUI1 "$(^Font)" "$(^FontSize)" 700
  SendMessage $GUI0 ${WM_SETFONT} $GUI1 0
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1038
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1034
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1039
  SetCtlColors $GUI0 "" 0xFFFFFF
  GetDlgItem $GUI0 $HWNDPARENT 1028
  SetCtlColors $GUI0 /BRANDING
  GetDlgItem $GUI0 $HWNDPARENT 1256
  SetCtlColors $GUI0 /BRANDING
  SendMessage $GUI0 ${WM_SETTEXT} 0 "STR:$(^Branding) "
  SetAutoClose true
FunctionEnd
Function un.onUserAbort
FunctionEnd
Function un.EscapeINI
  Exch $0
  Push $1
  Push $2
  Push $3
  Push $4
  StrCpy $1 ""
  StrLen $3 $0
  IntOp $3 $3 - 1
  StrCpy $4 0
  Goto un_escape_check
  un_escape_inc:
    IntOp $4 $4 + 1
  un_escape_check:
    IntCmp $4 $3 0 0 un_escape_done
    StrCpy $2 $0 1 $4
    StrCmp $2 "\" 0 un_escape_cr
      StrCpy $2 "\\"
      Goto un_escape_append
  un_escape_cr:
    StrCmp $2 "$\r" 0 un_escape_lf
      StrCpy $2 "\r"
      Goto un_escape_append
  un_escape_lf:
    StrCmp $2 "$\n" 0 un_escape_tab
      StrCpy $2 "\n"
      Goto un_escape_append
  un_escape_tab:
    StrCmp $2 "$\t" 0 un_escape_append
      StrCpy $2 "\t"
  un_escape_append:
    StrCpy $1 "$1$2"
    Goto un_escape_inc
    Goto un_escape_inc
    Goto un_escape_done
  un_escape_done:
    StrCpy $0 $1
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Exch $0
FunctionEnd
Section "Uninstall"
  Delete "$INSTDIR\samp.exe"
  Delete "$INSTDIR\samp.dll"
  Delete "$INSTDIR\bass.dll"
  Delete "$INSTDIR\samp.saa"
  Delete "$INSTDIR\rcon.exe"
  Delete "$INSTDIR\samp_debug.exe"
  Delete "$INSTDIR\SAMPUninstall.exe"
  Delete "$INSTDIR\sampgui.png"
  Delete "$INSTDIR\mouse.png"
  Delete "$INSTDIR\gtaweap3.ttf"
  Delete "$INSTDIR\sampaux3.ttf"
  Delete "$INSTDIR\samp-license.txt"

  Delete "$INSTDIR\SAMP\samp.img"
  Delete "$INSTDIR\SAMP\sampcol.img"
  Delete "$INSTDIR\SAMP\custom.img"
  Delete "$INSTDIR\SAMP\samp.ide"
  Delete "$INSTDIR\SAMP\samp.ipl"
  Delete "$INSTDIR\SAMP\custom.ide"
  Delete "$INSTDIR\SAMP\samaps.txd"
  Delete "$INSTDIR\SAMP\blanktex.txd"
  RMDir "$INSTDIR\SAMP"
  Delete "$SMPROGRAMS\San Andreas Multiplayer\San Andreas Multiplayer.lnk"
  Delete "$SMPROGRAMS\San Andreas Multiplayer\Uninstall.lnk"
  RMDir "$SMPROGRAMS\San Andreas Multiplayer"
SectionEnd
