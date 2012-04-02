; $Id$
; @file
; VBoxGuestAdditionsCommon.nsh - Common / shared utility functions.
;

;
; Copyright (C) 2006-2012 Oracle Corporation
;
; This file is part of VirtualBox Open Source Edition (OSE), as
; available from http://www.virtualbox.org. This file is free software;
; you can redistribute it and/or modify it under the terms of the GNU
; General Public License (GPL) as published by the Free Software
; Foundation, in version 2 as it comes in the "COPYING" file of the
; VirtualBox OSE distribution. VirtualBox OSE is distributed in the
; hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
;

Function Common_CopyFiles

  SetOutPath "$INSTDIR"
  SetOverwrite on

!ifdef VBOX_WITH_LICENSE_INSTALL_RTF
  ; Copy license file (if any) into the installation directory
  FILE "/oname=${LICENSE_FILE_RTF}" "$%VBOX_BRAND_LICENSE_RTF%"
!endif

  FILE "$%VBOX_PATH_DIFX%\DIFxAPI.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxDrvInst.exe"

  FILE "$%PATH_OUT%\bin\additions\VBoxVideo.inf"
!ifdef VBOX_SIGN_ADDITIONS
  FILE "$%PATH_OUT%\bin\additions\VBoxVideo.cat"
!endif

  FILE "iexplore.ico"

FunctionEnd

!ifndef UNINSTALLER_ONLY
Function ExtractFiles

  ; @todo: Use a define for all the file specs to group the files per module
  ; and keep the redundancy low

  Push $0
  StrCpy "$0" "$INSTDIR\$%BUILD_TARGET_ARCH%"

  ; Root files
  SetOutPath "$0"
!if $%VBOX_WITH_LICENSE_INSTALL_RTF% == "1"
  FILE "/oname=${LICENSE_FILE_RTF}" "$%VBOX_BRAND_LICENSE_RTF%"
!endif

  ; Video driver
  SetOutPath "$0\VBoxVideo"
  FILE "$%PATH_OUT%\bin\additions\VBoxVideo.sys"
  FILE "$%PATH_OUT%\bin\additions\VBoxVideo.inf"
!ifdef VBOX_SIGN_ADDITIONS
  FILE "$%PATH_OUT%\bin\additions\VBoxVideo.cat"
!endif
  FILE "$%PATH_OUT%\bin\additions\VBoxDisp.dll"

!if $%VBOX_WITH_CROGL% == "1"
  ; crOpenGL
  FILE "$%PATH_OUT%\bin\additions\VBoxOGLarrayspu.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxOGLcrutil.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxOGLerrorspu.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxOGLpackspu.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxOGLpassthroughspu.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxOGLfeedbackspu.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxOGL.dll"

  SetOutPath "$0\VBoxVideo\OpenGL"
  FILE "$%PATH_OUT%\bin\additions\d3d8.dll"
  FILE "$%PATH_OUT%\bin\additions\d3d9.dll"
  !if $%BUILD_TARGET_ARCH% == "x86"
    ; libWine is used for 32bit d3d only
    ; @todo: remove it for 32bit as well
    FILE "$%PATH_OUT%\bin\additions\libWine.dll"
  !endif
  FILE "$%PATH_OUT%\bin\additions\VBoxD3D8.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxD3D9.dll"
  FILE "$%PATH_OUT%\bin\additions\wined3d.dll"

  !if $%BUILD_TARGET_ARCH% == "amd64"
    ; Only 64-bit installer: Also copy 32-bit DLLs on 64-bit target
    SetOutPath "$0\VBoxVideo\OpenGL\SysWow64"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\d3d8.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\d3d9.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\libWine.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxOGLarrayspu.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxOGLcrutil.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxOGLerrorspu.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxOGLpackspu.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxOGLpassthroughspu.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxOGLfeedbackspu.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxOGL.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxD3D8.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\VBoxD3D9.dll"
    FILE "$%VBOX_PATH_ADDITIONS_WIN_X86%\wined3d.dll"
  !endif
!endif

!if $%VBOX_WITH_WDDM% == "1"
  ; WDDM Video driver
  SetOutPath "$0\VBoxVideoWddm"

  !ifdef VBOX_SIGN_ADDITIONS
    FILE "$%PATH_OUT%\bin\additions\VBoxVideoWddm.cat"
  !endif
  FILE "$%PATH_OUT%\bin\additions\VBoxVideoWddm.sys"
  FILE "$%PATH_OUT%\bin\additions\VBoxVideoWddm.inf"
  FILE "$%PATH_OUT%\bin\additions\VBoxDispD3D.dll"

  !if $%VBOX_WITH_CROGL% == "1"
    FILE "$%PATH_OUT%\bin\additions\VBoxOGLarrayspu.dll"
    FILE "$%PATH_OUT%\bin\additions\VBoxOGLcrutil.dll"
    FILE "$%PATH_OUT%\bin\additions\VBoxOGLerrorspu.dll"
    FILE "$%PATH_OUT%\bin\additions\VBoxOGLpackspu.dll"
    FILE "$%PATH_OUT%\bin\additions\VBoxOGLpassthroughspu.dll"
    FILE "$%PATH_OUT%\bin\additions\VBoxOGLfeedbackspu.dll"
    FILE "$%PATH_OUT%\bin\additions\VBoxOGL.dll"

    FILE "$%PATH_OUT%\bin\additions\VBoxD3D9wddm.dll"
    FILE "$%PATH_OUT%\bin\additions\wined3dwddm.dll"
  !endif ; $%VBOX_WITH_CROGL% == "1"

  !if $%BUILD_TARGET_ARCH% == "amd64"
    FILE "$%PATH_OUT%\bin\additions\VBoxDispD3D-x86.dll"

    !if $%VBOX_WITH_CROGL% == "1"
      FILE "$%PATH_OUT%\bin\additions\VBoxOGLarrayspu-x86.dll"
      FILE "$%PATH_OUT%\bin\additions\VBoxOGLcrutil-x86.dll"
      FILE "$%PATH_OUT%\bin\additions\VBoxOGLerrorspu-x86.dll"
      FILE "$%PATH_OUT%\bin\additions\VBoxOGLpackspu-x86.dll"
      FILE "$%PATH_OUT%\bin\additions\VBoxOGLpassthroughspu-x86.dll"
      FILE "$%PATH_OUT%\bin\additions\VBoxOGLfeedbackspu-x86.dll"
      FILE "$%PATH_OUT%\bin\additions\VBoxOGL-x86.dll"

      FILE "$%PATH_OUT%\bin\additions\VBoxD3D9wddm-x86.dll"
      FILE "$%PATH_OUT%\bin\additions\wined3dwddm-x86.dll"
    !endif ; $%VBOX_WITH_CROGL% == "1"
  !endif ; $%BUILD_TARGET_ARCH% == "amd64"
!endif ; $%VBOX_WITH_WDDM% == "1"

  ; Mouse driver
  SetOutPath "$0\VBoxMouse"
  FILE "$%PATH_OUT%\bin\additions\VBoxMouse.sys"
  FILE "$%PATH_OUT%\bin\additions\VBoxMouse.inf"
!ifdef VBOX_SIGN_ADDITIONS
  FILE "$%PATH_OUT%\bin\additions\VBoxMouse.cat"
!endif

!if $%BUILD_TARGET_ARCH% == "x86"
  SetOutPath "$0\VBoxMouse\NT4"
  FILE "$%PATH_OUT%\bin\additions\VBoxMouseNT.sys"
!endif

  ; Guest driver
  SetOutPath "$0\VBoxGuest"
  FILE "$%PATH_OUT%\bin\additions\VBoxGuest.sys"
  FILE "$%PATH_OUT%\bin\additions\VBoxGuest.inf"
!ifdef VBOX_SIGN_ADDITIONS
  FILE "$%PATH_OUT%\bin\additions\VBoxGuest.cat"
!endif
  FILE "$%PATH_OUT%\bin\additions\VBoxTray.exe"
  FILE "$%PATH_OUT%\bin\additions\VBoxHook.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxControl.exe"

!if $%BUILD_TARGET_ARCH% == "x86"
  SetOutPath "$0\VBoxGuest\NT4"
  FILE "$%PATH_OUT%\bin\additions\VBoxGuestNT.sys"
!endif

  ; VBoxService
  SetOutPath "$0\Bin"
  FILE "$%PATH_OUT%\bin\additions\VBoxService.exe"
!if $%BUILD_TARGET_ARCH% == "x86"
  FILE "$%PATH_OUT%\bin\additions\VBoxServiceNT.exe"
!endif

  ; Shared Folders
  SetOutPath "$0\VBoxSF"
  FILE "$%PATH_OUT%\bin\additions\VBoxSF.sys"
  FILE "$%PATH_OUT%\bin\additions\VBoxMRXNP.dll"
  !if $%BUILD_TARGET_ARCH% == "amd64"
    ; Only 64-bit installer: Also copy 32-bit DLLs on 64-bit target
    FILE "$%PATH_OUT%\bin\additions\VBoxMRXNP-x86.dll"
  !endif

  ; Auto-Logon
  SetOutPath "$0\AutoLogon"
  FILE "$%PATH_OUT%\bin\additions\VBoxGINA.dll"
  FILE "$%PATH_OUT%\bin\additions\VBoxCredProv.dll"

  ; Misc tools
  SetOutPath "$0\Tools"
  FILE "$%PATH_OUT%\bin\additions\VBoxDrvInst.exe"
  FILE "$%VBOX_PATH_DIFX%\DIFxAPI.dll"

!if $%BUILD_TARGET_ARCH% == "x86"
  SetOutPath "$0\Tools\NT4"
  FILE "$%PATH_OUT%\bin\additions\VBoxGuestDrvInst.exe"
  FILE "$%PATH_OUT%\bin\additions\RegCleanup.exe"
!endif

  Pop $0

FunctionEnd
!endif ; UNINSTALLER_ONLY

!macro EnableLog un
Function ${un}EnableLog

!ifdef _DEBUG
  Goto log
!endif

  StrCmp $g_bLogEnable "true" log
  Goto exit

log:

  LogSet on
  LogText "Start logging."

exit:

FunctionEnd
!macroend
!insertmacro EnableLog ""
!insertmacro EnableLog "un."

!macro WriteLogUI un
Function ${un}WriteLogUI

  IfSilent exit

!ifdef _DEBUG
  Goto log
!endif

  StrCmp $g_bLogEnable "true" log
  Goto exit

log:

  ; Dump log to see what happened
  StrCpy $0 "$INSTDIR\${un}install_ui.log"
  Push $0
  Call ${un}DumpLog

exit:

FunctionEnd
!macroend
!insertmacro WriteLogUI ""
!insertmacro WriteLogUI "un."

!macro WriteLogVBoxTray un
Function ${un}WriteLogVBoxTray

  ; Pop function parameters off the stack
  ; in reverse order
  Exch $1 ; Message type (0=Info, 1=Warning, 2=Error)
  Exch
  Exch $0 ; Body string

  ; @todo Add more paramters here!
!if $%VBOX_WITH_GUEST_INSTALL_HELPER% == "1"
  ${If} $g_bPostInstallStatus == "true"
    ; Parameters:
    ; - String: Description / Body
    ; - String: Title / Name of application
    ; - Integer: Type of message: 0 (Info), 1 (Warning), 2 (Error)
    ; - Integer: Time (in msec) to show the notification
    VBoxGuestInstallHelper::VBoxTrayShowBallonMsg "$0" "VirtualBox Guest Additions Setup" $1 5000
    Pop $0 ; Get return value (ignored for now)
  ${EndIf}
!endif
  Pop $0
  Pop $1

FunctionEnd
!macroend
!insertmacro WriteLogVBoxTray ""
!insertmacro WriteLogVBoxTray "un."

!macro CheckArchitecture un
Function ${un}CheckArchitecture

  Push $0

  System::Call "kernel32::GetCurrentProcess() i .s"
  System::Call "kernel32::IsWow64Process(i s, *i .r0)"
  ; R0 now contains 1 if we're a 64-bit process, or 0 if not

!if $%BUILD_TARGET_ARCH% == "amd64"
  IntCmp $0 0 wrong_platform
!else ; 32-bit
  IntCmp $0 1 wrong_platform
!endif

  Push 0
  Goto exit

wrong_platform:

  Push 1
  Goto exit

exit:

FunctionEnd
!macroend
!insertmacro CheckArchitecture ""
!insertmacro CheckArchitecture "un."

!macro GetWindowsVer un
Function ${un}GetWindowsVer

  ; Check if we are running on w2k or above
  ; For other windows versions (>XP) it may be necessary to change winver.nsh
  Call ${un}GetWindowsVersion
  Pop $R3     ; Windows Version

  Push $R3    ; The windows version string
  Push "NT"   ; String to search for. Win 2k family returns no string containing 'NT'
  Call ${un}StrStr
  Pop $R0
  StrCmp $R0 '' nt5plus       ; Not NT 3.XX or 4.XX

  ; Ok we know it is NT. Must be a string like NT X.XX
  Push $R3    ; The windows version string
  Push "4."   ; String to search for
  Call ${un}StrStr
  Pop $R0
  StrCmp $R0 "" nt5plus nt4   ; If empty -> not NT 4

nt5plus:    ; Windows 2000+ (XP, Vista, ...)

  StrCpy $g_strWinVersion $R3
  goto exit

nt4:        ; NT 4.0

  StrCpy $g_strWinVersion "NT4"
  goto exit

exit:

FunctionEnd
!macroend
!insertmacro GetWindowsVer ""
!insertmacro GetWindowsVer "un."

!macro GetAdditionsVersion un
Function ${un}GetAdditionsVersion

  Push $0
  Push $1

  ; Get additions version
  ReadRegStr $0 HKLM "SOFTWARE\$%VBOX_VENDOR_SHORT%\VirtualBox Guest Additions" "Version"

  ; Get revision
  ReadRegStr $g_strAddVerRev HKLM "SOFTWARE\$%VBOX_VENDOR_SHORT%\VirtualBox Guest Additions" "Revision"

  ; Extract major version
  Push "$0"       ; String
  Push "."        ; SubString
  Push ">"        ; SearchDirection
  Push "<"        ; StrInclusionDirection
  Push "0"        ; IncludeSubString
  Push "0"        ; Loops
  Push "0"        ; CaseSensitive
  Call ${un}StrStrAdv
  Pop $g_strAddVerMaj

  ; Extract minor version
  Push "$0"       ; String
  Push "."        ; SubString
  Push ">"        ; SearchDirection
  Push ">"        ; StrInclusionDirection
  Push "0"        ; IncludeSubString
  Push "0"        ; Loops
  Push "0"        ; CaseSensitive
  Call ${un}StrStrAdv
  Pop $1          ; Got first part (e.g. "1.5")

  Push "$1"       ; String
  Push "."        ; SubString
  Push ">"        ; SearchDirection
  Push "<"        ; StrInclusionDirection
  Push "0"        ; IncludeSubString
  Push "0"        ; Loops
  Push "0"        ; CaseSensitive
  Call ${un}StrStrAdv
  Pop $g_strAddVerMin   ; Extracted second part (e.g. "5" from "1.5")

  ; Extract build number
  Push "$0"       ; String
  Push "."        ; SubString
  Push "<"        ; SearchDirection
  Push ">"        ; StrInclusionDirection
  Push "0"        ; IncludeSubString
  Push "0"        ; Loops
  Push "0"        ; CaseSensitive
  Call ${un}StrStrAdv
  Pop $g_strAddVerBuild

exit:

  Pop $1
  Pop $0

FunctionEnd
!macroend
!insertmacro GetAdditionsVersion ""
!insertmacro GetAdditionsVersion "un."

!macro StopVBoxService un
Function ${un}StopVBoxService

  Push $0   ; Temp results
  Push $1
  Push $2   ; Image name of VBoxService
  Push $3   ; Safety counter

  StrCpy $3 "0" ; Init counter
  DetailPrint "Stopping VBoxService ..."

svc_stop:

  LogText "Stopping VBoxService (as service) ..."
  ${If} $g_strWinVersion == "NT4"
   nsExec::Exec '"$SYSDIR\net.exe" stop VBoxService'
  ${Else}
   nsExec::Exec '"$SYSDIR\SC.exe" stop VBoxService'
  ${EndIf}
  Sleep "1000"           ; Wait a bit

exe_stop:

!ifdef _DEBUG
  DetailPrint "Stopping VBoxService (as exe) ..."
!endif

exe_stop_loop:

  IntCmp $3 10 exit      ; Only try this loop 10 times max
  IntOp  $3 $3 + 1       ; Increment

  LogText "Try: $3"

  ${If} $g_strWinVersion == "NT4"
    StrCpy $2 "VBoxServiceNT.exe"
  ${Else}
    StrCpy $2 "VBoxService.exe"
  ${EndIf}

  ${nsProcess::FindProcess} $2 $0
  StrCmp $0 0 0 exit

  ${nsProcess::KillProcess} $2 $0
  Sleep "1000"           ; Wait a bit
  Goto exe_stop_loop

exit:

  DetailPrint "Stopping VBoxService done."

  Pop $3
  Pop $2
  Pop $1
  Pop $0

FunctionEnd
!macroend
!insertmacro StopVBoxService ""
!insertmacro StopVBoxService "un."

!macro StopVBoxTray un
Function ${un}StopVBoxTray

  Push $0   ; Temp results
  Push $1   ; Safety counter

  StrCpy $1 "0" ; Init counter
  DetailPrint "Stopping VBoxTray ..."

exe_stop:

  IntCmp $1 10 exit      ; Only try this loop 10 times max
  IntOp  $1 $1 + 1       ; Increment

  ${nsProcess::FindProcess} "VBoxTray.exe" $0
  StrCmp $0 0 0 exit

  ${nsProcess::KillProcess} "VBoxTray.exe" $0
  Sleep "1000"           ; Wait a bit
  Goto exe_stop

exit:

  DetailPrint "Stopping VBoxTray done."

  Pop $1
  Pop $0

FunctionEnd
!macroend
!insertmacro StopVBoxTray ""
!insertmacro StopVBoxTray "un."

!macro WriteRegBinR ROOT KEY NAME VALUE
  WriteRegBin "${ROOT}" "${KEY}" "${NAME}" "${VALUE}"
!macroend

!macro AbortShutdown un
Function ${un}AbortShutdown

  Push $0

  ; Try to abort the shutdown
  nsExec::ExecToLog '"$g_strSystemDir\shutdown.exe" -a' $0

  Pop $0

FunctionEnd
!macroend
!insertmacro AbortShutdown ""
!insertmacro AbortShutdown "un."

!macro CheckForWDDMCapability un
Function ${un}CheckForWDDMCapability

!if $%VBOX_WITH_WDDM% == "1"
  ; If we're on a 32-bit Windows Vista / 7 / 8 we can use the WDDM driver
  ${If} $g_strWinVersion == "Vista"
  ${OrIf} $g_strWinVersion == "7"
  ${OrIf} $g_strWinVersion == "8"
    StrCpy $g_bCapWDDM "true"
  ${EndIf}
  ; If we're on Windows 8 we *have* to use the WDDM driver, so select it
  ; by default
  ${If} $g_strWinVersion == "8"
    StrCpy $g_bWithWDDM "true"
  ${EndIf}
!endif

FunctionEnd
!macroend
!insertmacro CheckForWDDMCapability ""
!insertmacro CheckForWDDMCapability "un."

!macro CheckForCapabilities un
Function ${un}CheckForCapabilities

  Push $0

  ; Retrieve system mode and store result in
  System::Call 'user32::GetSystemMetrics(i ${SM_CLEANBOOT}) i .r0'
  StrCpy $g_iSystemMode $0

  ; Does the guest have a DLL cache?
  ${If} $g_strWinVersion == "Vista"
  ${OrIf} $g_strWinVersion == "7"
  ${OrIf} $g_strWinVersion == "8"
    StrCpy $g_bCapDllCache "true"
  ${EndIf}

  ; Check whether this OS is capable of handling WDDM drivers
  Call ${un}CheckForWDDMCapability

  Pop $0

FunctionEnd
!macroend
!insertmacro CheckForCapabilities ""
!insertmacro CheckForCapabilities "un."

; Switches (back) the path + registry view to
; 32-bit mode (SysWOW64) on 64-bit guests
!macro SetAppMode32 un
Function ${un}SetAppMode32
  !if $%BUILD_TARGET_ARCH% == "amd64"
    ${EnableX64FSRedirection}
    SetRegView 32
  !endif
FunctionEnd
!macroend
!insertmacro SetAppMode32 ""
!insertmacro SetAppMode32 "un."

; Because this NSIS installer is always built in 32-bit mode, we have to
; do some tricks for the Windows paths + registry on 64-bit guests
!macro SetAppMode64 un
Function ${un}SetAppMode64
  !if $%BUILD_TARGET_ARCH% == "amd64"
    ${DisableX64FSRedirection}
    SetRegView 64
  !endif
FunctionEnd
!macroend
!insertmacro SetAppMode64 ""
!insertmacro SetAppMode64 "un."

;
; Retrieves the vendor ("CompanyName" of FILEINFO structure)
; of a given file.
; @return  Stack: Company name, or "" on error/if not found.
; @param   Stack: File name to retrieve vendor for.
;
!macro GetFileVendor un
Function ${un}GetFileVendor

  ; Preserve values
  Exch $0 ; Stack: $0 <filename> (Get file name into $0)
  Push $1

  IfFileExists "$0" found
  Goto not_found

found:

  VBoxGuestInstallHelper::FileGetVendor "$0"
  ; Stack: <vendor> $1 $0
  Pop  $0 ; Get vendor
  Pop  $1 ; Restore $1
  Exch $0 ; Restore $0, push vendor on top of stack
  Goto end

not_found:

  Pop $1
  Pop $0
  Push "File not found"
  Goto end

end:

FunctionEnd
!macroend
!insertmacro GetFileVendor ""
!insertmacro GetFileVendor "un."

;
; Retrieves the architecture of a given file.
; @return  Stack: Architecture ("x86", "amd64") or error message.
; @param   Stack: File name to retrieve architecture for.
;
!macro GetFileArchitecture un
Function ${un}GetFileArchitecture

  ; Preserve values
  Exch $0 ; Stack: $0 <filename> (Get file name into $0)
  Push $1

  IfFileExists "$0" found
  Goto not_found

found:

  VBoxGuestInstallHelper::FileGetArchitecture "$0"
  ; Stack: <architecture> $1 $0
  Pop  $0 ; Get architecture string
  Pop  $1 ; Restore $1
  Exch $0 ; Restore $0, push vendor on top of stack
  Goto end

not_found:

  Pop $1
  Pop $0
  Push "File not found"
  Goto end

end:

FunctionEnd
!macroend
!insertmacro GetFileArchitecture ""
!insertmacro GetFileArchitecture "un."

;
; Verifies a given file by checking its file vendor and target
; architecture.
; @return  Stack: "0" if valid, "1" if not, "2" on error / not found.
; @param   Stack: Architecture ("x86" or "amd64").
; @param   Stack: Vendor.
; @param   Stack: File name to verify.
;
!macro VerifyFile un
Function ${un}VerifyFile

  ; Preserve values
  Exch $0 ; File;         S: old$0 vendor arch
  Exch    ;               S: vendor old$0 arch
  Exch $1 ; Vendor;       S: old$1 old$0 arch
  Exch    ;               S: old$0 old$1 arch
  Exch 2  ;               S: arch old$1 old$0
  Exch $2 ; Architecture; S: old$2 old$1 old$0
  Push $3 ;               S: old$3 old$2 old$1 old$0

  IfFileExists "$0" check_vendor
  Goto not_found

check_vendor:

  Push $0
  Call ${un}GetFileVendor
  Pop $3

  ${If} $3 == $1
    Goto check_arch
  ${EndIf}
  StrCpy $3 "1" ; Invalid
  Goto end

check_arch:

  Push $0
  Call ${un}GetFileArchitecture
  Pop $3

  ${If} $3 == $2
    StrCpy $3 "0" ; Valid
  ${Else}
    StrCpy $3 "1" ; Invalid
  ${EndIf}
  Goto end

not_found:

  StrCpy $3 "2" ; Not found
  Goto end

end:

  ; S: old$3 old$2 old$1 old$0
  Exch $3 ; S: $3 old$2 old$1 old$0
  Exch    ; S: old$2 $3 old$1
  Pop $2  ; S: $3 old$1 old$0
  Exch    ; S: old$1 $3 old$0
  Pop $1  ; S: $3 old$0
  Exch    ; S: old$0 $3
  Pop $0  ; S: $3

FunctionEnd
!macroend
!insertmacro VerifyFile ""
!insertmacro VerifyFile "un."

;
; Macro for accessing VerifyFile in a more convenient way by using
; a parameter list.
; @return  Stack: "0" if valid, "1" if not, "2" on error / not found.
; @param   Un/Installer prefix; either "" or "un".
; @param   Name of file to verify.
; @param   Vendor to check for.
; @param   Architecture ("x86" or "amd64") to check for.
;
!macro VerifyFileEx un File Vendor Architecture
  Push $0
  Push "${Architecture}"
  Push "${Vendor}"
  Push "${File}"
  DetailPrint "Verifying file $\"${File}$\" ..."
  Call ${un}VerifyFile
  Pop $0
  ${If} $0 == "0"
    DetailPrint "Verification of file $\"${File}$\" successful (Vendor: ${Vendor}, Architecture: ${Architecture})"
  ${ElseIf} $0 == "1"
    DetailPrint "Verification of file $\"${File}$\" failed (not Vendor: ${Vendor}, and/or not Architecture: ${Architecture})"
  ${Else}
    DetailPrint "Skipping to file $\"${File}$\"; not found"
  ${EndIf}
  ; Push result popped off the stack to stack again
  Push $0
!macroend
!define VerifyFileEx "!insertmacro VerifyFileEx"

;
; Macro for copying a file only if the source file is verified 
; to be from a certain vendor and architecture.
; @return  Stack: "0" if copied, "1" if not, "2" on error / not found.
; @param   Un/Installer prefix; either "" or "un".
; @param   Name of file to verify and copy to destination.
; @param   Destination name to copy verified file to.
; @param   Vendor to check for.
; @param   Architecture ("x86" or "amd64") to check for.
;
!macro CopyFileEx un FileSrc FileDest Vendor Architecture
  Push $0
  Push "${Architecture}"
  Push "${Vendor}"
  Push "${FileSrc}"  
  Call ${un}VerifyFile
  Pop $0
  ${If} $0 == "0"
    DetailPrint "Copying verified file $\"${FileSrc}$\" to $\"${FileDest}$\" ..."
    CopyFiles /SILENT "${FileSrc}" "${FileDest}"
  ${Else}
    DetailPrint "Skipping to copy file $\"${FileSrc}$\" to $\"${FileDest}$\" (not Vendor: ${Vendor}, Architecture: ${Architecture})"
  ${EndIf}
  ; Push result popped off the stack to stack again
  Push $0
!macroend
!define CopyFileEx "!insertmacro CopyFileEx"

;
; Macro for installing a library/DLL.
; @return  Stack: "0" if copied, "1" if not, "2" on error / not found.
; @param   Un/Installer prefix; either "" or "un".
; @param   Name of lib/DLL to verify and copy to destination.
; @param   Destination name to copy verified file to.
; @param   Temporary folder used for exchanging the (locked) lib/DLL after a reboot.
;
!macro InstallFileEx un FileSrc FileDest DirTemp
  DetailPrint "Installing library $\"${FileSrc}$\" to $\"${FileDest}$\" ..."
  ; Try the gentle way and replace the file instantly
  !insertmacro InstallLib DLL NOTSHARED NOREBOOT_NOTPROTECTED "${FileSrc}" "${FileDest}" "${DirTemp}"
  ; If the above call didn't help, use a (later) reboot to replace the file
  !insertmacro InstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "${FileSrc}" "${FileDest}" "${DirTemp}"
!macroend
!define InstallFileEx "!insertmacro InstallFileEx"

;
; Macro for installing a library/DLL.
; @return  Stack: "0" if copied, "1" if not, "2" on error / not found.
; @param   Un/Installer prefix; either "" or "un".
; @param   Name of lib/DLL to verify and copy to destination.
; @param   Destination name to copy verified file to.
; @param   Temporary folder used for exchanging the (locked) lib/DLL after a reboot.
; @param   Vendor to check for.
; @param   Architecture ("x86" or "amd64") to check for.
;
!macro InstallFileVerify un FileSrc FileDest DirTemp Vendor Architecture
  Push $0
  Push "${Architecture}"
  Push "${Vendor}"
  Push "${FileSrc}"
  DetailPrint "Verifying library $\"${FileSrc}$\" ..."
  Call ${un}VerifyFile
  Pop $0
  ${If} $0 == "0"
    ${InstallFileEx} ${un} ${FileSrc} ${FileDest} ${DirTemp}
  ${Else}
    DetailPrint "File $\"${FileSrc}$\" did not pass verification (Vendor: ${Vendor}, Architecture: ${Architecture})"
  ${EndIf}
  ; Push result popped off the stack to stack again.
  Push $0
!macroend
!define InstallFileVerify "!insertmacro InstallFileVerify"

;
; Validates backed up and replaced Direct3D files; either the d3d*.dll have
; to be from Microsoft or the (already) backed up msd3d*.dll files. If both
; don't match we have a corrupted / invalid installation.
; @return  Stack: "0" if files are valid; otherwise "1".
;
!macro ValidateFilesDirect3D un
Function ${un}ValidateD3DFiles

  Push $0

  ; We need to switch to 64-bit app mode to handle the "real" 64-bit files in
  ; ""system32" on a 64-bit guest
  Call ${un}SetAppMode64

  ; Note: Not finding a file (like *d3d8.dll) on Windows Vista/7 is fine;
  ;       it simply is not present there.
  
  ; Note 2: On 64-bit systems there are no 64-bit *d3d8 DLLs, only 32-bit ones 
  ;         in SysWOW64 (or in system32 on 32-bit systems).

!if $%BUILD_TARGET_ARCH% == "x86"
  ${VerifyFileEx} "${un}" "$SYSDIR\d3d8.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
  Pop $0
  ${If} $0 == "1"
    Goto verify_msd3d
  ${EndIf}
!endif
  
  ${VerifyFileEx} "${un}" "$SYSDIR\d3d9.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
  Pop $0
  ${If} $0 == "1"
    Goto verify_msd3d
  ${EndIf}

  ${If} $g_bCapDllCache == "true"
!if $%BUILD_TARGET_ARCH% == "x86"
    ${VerifyFileEx} "${un}" "$SYSDIR\dllcache\d3d8.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
    Pop $0
    ${If} $0 == "1"
      Goto verify_msd3d
    ${EndIf}
!endif
    ${VerifyFileEx} "${un}" "$SYSDIR\dllcache\d3d9.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
    Pop $0
    ${If} $0 == "1"
      Goto verify_msd3d
    ${EndIf}
  ${EndIf}

!if $%BUILD_TARGET_ARCH% == "amd64"
  ${VerifyFileEx} "${un}" "$g_strSysWow64\d3d8.dll" "Microsoft Corporation" "x86"
  Pop $0
  ${If} $0 == "1"
    Goto verify_msd3d
  ${EndIf}
  ${VerifyFileEx} "${un}" "$g_strSysWow64\d3d9.dll" "Microsoft Corporation" "x86"
  Pop $0
  ${If} $0 == "1"
    Goto verify_msd3d
  ${EndIf}

  ${If} $g_bCapDllCache == "true"
    ${VerifyFileEx} "${un}" "$g_strSysWow64\dllcache\d3d8.dll" "Microsoft Corporation" "x86"
    Pop $0
    ${If} $0 == "1"
      Goto verify_msd3d
    ${EndIf}
    ${VerifyFileEx} "${un}" "$g_strSysWow64\dllcache\d3d9.dll" "Microsoft Corporation" "x86"
    Pop $0
    ${If} $0 == "1"
      Goto verify_msd3d
    ${EndIf}
  ${EndIf}

!endif

  Goto valid

verify_msd3d:

!if $%BUILD_TARGET_ARCH% == "x86"
  ${VerifyFileEx} "${un}" "$SYSDIR\msd3d8.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
  Pop $0
  ${If} $0 == "1"
    Goto invalid
  ${EndIf}
!endif
  ${VerifyFileEx} "${un}" "$SYSDIR\msd3d9.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
  Pop $0
  ${If} $0 == "1"
    Goto invalid
  ${EndIf}

  ${If} $g_bCapDllCache == "true"
!if $%BUILD_TARGET_ARCH% == "x86"
    ${VerifyFileEx} "${un}" "$SYSDIR\dllcache\msd3d8.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
    Pop $0
    ${If} $0 == "1"
      Goto invalid
    ${EndIf}
!endif
    ${VerifyFileEx} "${un}" "$SYSDIR\dllcache\msd3d9.dll" "Microsoft Corporation" "$%BUILD_TARGET_ARCH%"
    Pop $0
    ${If} $0 == "1"
      Goto invalid
    ${EndIf}
  ${EndIf}

!if $%BUILD_TARGET_ARCH% == "amd64"
  ${VerifyFileEx} "${un}" "$g_strSysWow64\msd3d8.dll" "Microsoft Corporation" "x86"
  Pop $0
  ${If} $0 == "1"
    Goto invalid
  ${EndIf}
  ${VerifyFileEx} "${un}" "$g_strSysWow64\msd3d9.dll" "Microsoft Corporation" "x86"
  Pop $0
  ${If} $0 == "1"
    Goto invalid
  ${EndIf}

  ${If} $g_bCapDllCache == "true"
    ${VerifyFileEx} "${un}" "$g_strSysWow64\dllcache\msd3d8.dll" "Microsoft Corporation" "x86"
    Pop $0
    ${If} $0 == "1"
      Goto invalid
    ${EndIf}
    ${VerifyFileEx} "${un}" "$g_strSysWow64\dllcache\msd3d9.dll" "Microsoft Corporation" "x86"
    Pop $0
    ${If} $0 == "1"
      Goto invalid
    ${EndIf}
  ${EndIf}
!endif

  Goto valid

valid:

  StrCpy $0 "0" ; Installation valid
  Goto end

invalid:

  StrCpy $0 "1" ; Installation invalid / corrupted
  Goto end

end:

  Exch $0

FunctionEnd
!macroend
!insertmacro ValidateFilesDirect3D ""
!insertmacro ValidateFilesDirect3D "un."
