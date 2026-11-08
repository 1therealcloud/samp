; EliRT.asm - x86 OMF reconstruction from EliRT.obj
; Original module name: EliRT.asm
; Original embedded string: EliRT 1.01 / Copyright (C) 2001, 2003 Radim Picha

.386
OPTION CASEMAP:NONE
OPTION SEGMENT:USE32

EXTRN CreateRemoteThread:NEAR32
EXTRN GetCurrentProcessId:NEAR32
EXTRN VirtualAlloc:NEAR32
EXTRN VirtualFree:NEAR32
EXTRN SetLastError:NEAR32
EXTRN GetPriorityClass:NEAR32
EXTRN OpenProcess:NEAR32
EXTRN CloseHandle:NEAR32
EXTRN ResumeThread:NEAR32

; Public symbols are listed in their original OMF PUBDEF record order.
PUBLIC xVirtualAllocEx
PUBLIC xOpenThread
PUBLIC xCreateRemoteThread
PUBLIC xVirtualFreeEx
PUBLIC RT_GetVersion

_TEXT SEGMENT PARA PUBLIC USE32 'CODE'
ASSUME CS:_TEXT, DS:NOTHING, ES:NOTHING, SS:NOTHING

elirt_init:
    mov    eax,OFFSET g_init                        ; 0000: B8 00 00 00 00 [OMF FIXUPP 0001 ABS _DATA:g_init]
    cld                                             ; 0005: FC
    cmp    BYTE PTR [eax],0h                        ; 0006: 80 38 00
    mov    edx,OFFSET g_api                         ; 0009: BA 00 00 00 00 [OMF FIXUPP 000A ABS _BSS:g_api]
    je     SHORT loc_0011                                 ; 000E: 74 01
init_ready:
    ret                                             ; 0010: C3
loc_0011:
    cmp    BYTE PTR [eax+1h],0h                     ; 0011: 80 78 01 00
    je     SHORT init_do_init                             ; 0015: 74 07
loc_0017:
    cmp    BYTE PTR [eax],0h                        ; 0017: 80 38 00
    je     SHORT loc_0017                                 ; 001A: 74 FB
    jmp    SHORT init_ready                               ; 001C: EB F2
init_do_init:
    inc    BYTE PTR [eax+1h]                        ; 001E: FE 40 01
    push   ebx                                      ; 0021: 53
    push   ebp                                      ; 0022: 55
    push   esi                                      ; 0023: 56
    push   edi                                      ; 0024: 57
    push   eax                                      ; 0025: 50
    mov    esi,OFFSET resolve_export                ; 0026: BE 63 02 00 00 [OMF FIXUPP 0027 ABS _TEXT:resolve_export]
    db 02Bh,0C0h                                    ; 002B: sub    eax,eax [exact encoding]
    db 08Bh,0FAh                                    ; 002D: mov    edi,edx [exact encoding]
    push   OFFSET seh_init                          ; 002F: 68 28 02 00 00 [OMF FIXUPP 0030 ABS _TEXT:seh_init]
    push   DWORD PTR fs:[eax]                       ; 0034: 64 FF 30
    mov    DWORD PTR fs:[eax],esp                   ; 0037: 64 89 20
    push   0Dh                                      ; 003A: 6A 0D
    pop    ecx                                      ; 003C: 59
    rep stosd                                       ; 003D: F3 AB
    db 08Bh,0FAh                                    ; 003F: mov    edi,edx [exact encoding]
    mov    ecx,DWORD PTR fs:[eax+030h]              ; 0041: 64 8B 48 30
    mov    edx,ds                                   ; 0045: 8C DA
    test   dl,4h                                    ; 0047: F6 C2 04
    jne    SHORT init_win9x                               ; 004A: 75 74
    mov    edx,07FFE026Ch                           ; 004C: BA 6C 02 FE 7F
    mov    ah,BYTE PTR [edx]                        ; 0051: 8A 22
    cmp    ah,4h                                    ; 0053: 80 FC 04
    mov    al,BYTE PTR [edx+4h]                     ; 0056: 8A 42 04
    jb     SHORT loc_0060                                 ; 0059: 72 05
    cmp    ah,5h                                    ; 005B: 80 FC 05
    jbe    SHORT loc_0064                                 ; 005E: 76 04
loc_0060:
    mov    ax,0333h                                 ; 0060: 66 B8 33 03
loc_0064:
    shl    eax,010h                                 ; 0064: C1 E0 10
    mov    ax,0101h                                 ; 0067: 66 B8 01 01
    stosd                                           ; 006B: AB
    mov    edx,DWORD PTR [ecx+0Ch]                  ; 006C: 8B 51 0C
    mov    eax,DWORD PTR [edx+01Ch]                 ; 006F: 8B 42 1C
    mov    ebx,DWORD PTR [eax+8h]                   ; 0072: 8B 58 08
    mov    eax,OFFSET name_RtlNtStatusToDosError    ; 0075: B8 E8 02 00 00 [OMF FIXUPP 0076 ABS _TEXT:name_RtlNtStatusToDosError]
    call   esi                                      ; 007A: FF D6
    stosd                                           ; 007C: AB
    mov    eax,OFFSET name_NtAllocateVirtualMemory  ; 007D: B8 FE 02 00 00 [OMF FIXUPP 007E ABS _TEXT:name_NtAllocateVirtualMemory]
    call   esi                                      ; 0082: FF D6
    test   eax,eax                                  ; 0084: 85 C0
    stosd                                           ; 0086: AB
    je     SHORT loc_008E                                 ; 0087: 74 05
    mov    eax,OFFSET nt_virtual_alloc              ; 0089: B8 C0 03 00 00 [OMF FIXUPP 008A ABS _TEXT:nt_virtual_alloc]
loc_008E:
    stosd                                           ; 008E: AB
    mov    eax,OFFSET name_NtFreeVirtualMemory      ; 008F: B8 16 03 00 00 [OMF FIXUPP 0090 ABS _TEXT:name_NtFreeVirtualMemory]
    call   esi                                      ; 0094: FF D6
    test   eax,eax                                  ; 0096: 85 C0
    stosd                                           ; 0098: AB
    je     SHORT loc_00A0                                 ; 0099: 74 05
    mov    eax,OFFSET nt_virtual_free               ; 009B: B8 8C 04 00 00 [OMF FIXUPP 009C ABS _TEXT:nt_virtual_free]
loc_00A0:
    stosd                                           ; 00A0: AB
    mov    eax,OFFSET name_NtOpenThread             ; 00A1: B8 2A 03 00 00 [OMF FIXUPP 00A2 ABS _TEXT:name_NtOpenThread]
    call   esi                                      ; 00A6: FF D6
    test   eax,eax                                  ; 00A8: 85 C0
    stosd                                           ; 00AA: AB
    je     SHORT loc_00B2                                 ; 00AB: 74 05
    mov    eax,OFFSET nt_open_thread                ; 00AD: B8 EB 05 00 00 [OMF FIXUPP 00AE ABS _TEXT:nt_open_thread]
loc_00B2:
    stosd                                           ; 00B2: AB
    mov    eax,OFFSET CreateRemoteThread            ; 00B3: B8 00 00 00 00 [OMF FIXUPP 00B4 ABS EXT:CreateRemoteThread]
    stosd                                           ; 00B8: AB
    stosd                                           ; 00B9: AB
    stosd                                           ; 00BA: AB
    jmp    init_done                                ; 00BB: E9 54 01 00 00
init_win9x:
    mov    ax,0840Ah                                ; 00C0: 66 B8 0A 84
    mov    ebx,0BFF70000h                           ; 00C4: BB 00 00 F7 BF
    cmp    dl,060h                                  ; 00C9: 80 FA 60
    mov    edx,05058454Bh                           ; 00CC: BA 4B 45 58 50
    jae    SHORT loc_00D5                                 ; 00D1: 73 02
    mov    al,3h                                    ; 00D3: B0 03
loc_00D5:
    cmp    DWORD PTR [ebx+01000h],edx               ; 00D5: 39 93 00 10 00 00
    je     SHORT loc_00EF                                 ; 00DB: 74 12
    cmp    al,05Ah                                  ; 00DD: 3C 5A
    je     SHORT loc_00EB                                 ; 00DF: 74 0A
    sub    ebx,010000h                              ; 00E1: 81 EB 00 00 01 00
    mov    al,05Ah                                  ; 00E7: B0 5A
    jmp    SHORT loc_00D5                                 ; 00E9: EB EA
loc_00EB:
    db 02Bh,0DBh                                    ; 00EB: sub    ebx,ebx [exact encoding]
    jmp    SHORT loc_0117                                 ; 00ED: EB 28
loc_00EF:
    mov    edx,DWORD PTR [ebx+01008h]               ; 00EF: 8B 93 08 10 00 00
    test   dl,3h                                    ; 00F5: F6 C2 03
    jne    SHORT loc_00EB                                 ; 00F8: 75 F1
    db 03Bh,0D3h                                    ; 00FA: cmp    edx,ebx [exact encoding]
    jbe    SHORT loc_00EB                                 ; 00FC: 76 ED
    push   ecx                                      ; 00FE: 51
    lea    ecx,[ebx+070000h]                        ; 00FF: 8D 8B 00 00 07 00
    db 03Bh,0D1h                                    ; 0105: cmp    edx,ecx [exact encoding]
    pop    ecx                                      ; 0107: 59
    jae    SHORT loc_00EB                                 ; 0108: 73 E1
    db 08Bh,0EAh                                    ; 010A: mov    ebp,edx [exact encoding]
    mov    edx,DWORD PTR [edx]                      ; 010C: 8B 12
    mov    edx,DWORD PTR [edx]                      ; 010E: 8B 12
    cmp    DWORD PTR [edx],6h                       ; 0110: 83 3A 06
    jne    SHORT loc_0117                                 ; 0113: 75 02
    db 08Ah,0C3h                                    ; 0115: mov    al,bl [exact encoding]
loc_0117:
    shl    eax,010h                                 ; 0117: C1 E0 10
    mov    ax,0101h                                 ; 011A: 66 B8 01 01
    push   ecx                                      ; 011E: 51
    stosd                                           ; 011F: AB
    db 036h                                     ; 0120: SS override
    call GetCurrentProcessId                        ; 0121: E8 00 00 00 00 [OMF FIXUPP 0122 REL EXT:GetCurrentProcessId]
    pop    ecx                                      ; 0126: 59
    db 033h,0C1h                                    ; 0127: xor    eax,ecx [exact encoding]
    stosd                                           ; 0129: AB
    test   ebx,ebx                                  ; 012A: 85 DB
    je     init_done                                ; 012C: 0F 84 E2 00 00 00
    push   047h                                     ; 0132: 6A 47
    pop    eax                                      ; 0134: 58
    call   esi                                      ; 0135: FF D6
    stosd                                           ; 0137: AB
    mov    eax,DWORD PTR [edx+068h]                 ; 0138: 8B 42 68
    db 003h,0C3h                                    ; 013B: add    eax,ebx [exact encoding]
    mov    DWORD PTR [edi+4h],eax                   ; 013D: 89 47 04
    mov    eax,DWORD PTR [edx+06Ch]                 ; 0140: 8B 42 6C
    db 003h,0C3h                                    ; 0143: add    eax,ebx [exact encoding]
    mov    DWORD PTR [edi+0Ch],eax                  ; 0145: 89 47 0C
    mov    eax,DWORD PTR [edx+058h]                 ; 0148: 8B 42 58
    db 003h,0C3h                                    ; 014B: add    eax,ebx [exact encoding]
    lea    ecx,[edi+020h]                           ; 014D: 8D 4F 20
    push   ecx                                      ; 0150: 51
    call   eax                                      ; 0151: FF D0
    cmp    BYTE PTR [edi-0Ah],bl                    ; 0153: 38 5F F6
    jne    SHORT loc_015B                                 ; 0156: 75 03
    sub    ebp,8h                                   ; 0158: 83 ED 08
loc_015B:
    mov    eax,DWORD PTR [ebp+030h]                 ; 015B: 8B 45 30
    db 03Bh,0C3h                                    ; 015E: cmp    eax,ebx [exact encoding]
    jae    SHORT loc_0165                                 ; 0160: 73 03
    mov    eax,DWORD PTR [edi+020h]                 ; 0162: 8B 47 20
loc_0165:
    mov    DWORD PTR [edi+024h],eax                 ; 0165: 89 47 24
    mov    eax,OFFSET win9x_virtual_alloc           ; 0168: B8 7D 03 00 00 [OMF FIXUPP 0169 ABS _TEXT:win9x_virtual_alloc]
    stosd                                           ; 016D: AB
    scasd                                           ; 016E: AF
    mov    eax,OFFSET win9x_virtual_free            ; 016F: B8 47 04 00 00 [OMF FIXUPP 0170 ABS _TEXT:win9x_virtual_free]
    stosd                                           ; 0174: AB
    scasd                                           ; 0175: AF
    mov    eax,OFFSET win9x_open_thread_worker      ; 0176: B8 3B 05 00 00 [OMF FIXUPP 0177 ABS _TEXT:win9x_open_thread_worker]
    stosd                                           ; 017B: AB
    mov    eax,OFFSET name_ReadProcessMemory        ; 017C: B8 37 03 00 00 [OMF FIXUPP 017D ABS _TEXT:name_ReadProcessMemory]
    call   esi                                      ; 0181: FF D6
    stosd                                           ; 0183: AB
    db 03Bh,0C3h                                    ; 0184: cmp    eax,ebx [exact encoding]
    lea    ebp,[ebx+030000h]                        ; 0186: 8D AB 00 00 03 00
    jb     SHORT loc_01A3                                 ; 018C: 72 15
    db 03Bh,0C5h                                    ; 018E: cmp    eax,ebp [exact encoding]
    jae    SHORT loc_01A3                                 ; 0190: 73 11
    sub    eax,0F1h                                 ; 0192: 2D F1 00 00 00
    push   05Fh                                     ; 0197: 6A 5F
    pop    ecx                                      ; 0199: 59
    call   scan_stub                                ; 019A: E8 A3 00 00 00
    test   ecx,ecx                                  ; 019F: 85 C9
    jne    SHORT loc_0209                                 ; 01A1: 75 66
loc_01A3:
    mov    eax,OFFSET name_SetThreadAffinityMask    ; 01A3: B8 49 03 00 00 [OMF FIXUPP 01A4 ABS _TEXT:name_SetThreadAffinityMask]
    call   esi                                      ; 01A8: FF D6
    db 03Bh,0C3h                                    ; 01AA: cmp    eax,ebx [exact encoding]
    jb     SHORT loc_01DF                                 ; 01AC: 72 31
    db 03Bh,0C5h                                    ; 01AE: cmp    eax,ebp [exact encoding]
    jae    SHORT loc_01DF                                 ; 01B0: 73 2D
    push   048h                                     ; 01B2: 6A 48
    pop    ecx                                      ; 01B4: 59
    add    eax,055h                                 ; 01B5: 83 C0 55
    cmp    DWORD PTR [eax],ecx                      ; 01B8: 39 08
    je     SHORT loc_01CA                                 ; 01BA: 74 0E
    add    eax,0Eh                                  ; 01BC: 83 C0 0E
    cmp    DWORD PTR [eax],ecx                      ; 01BF: 39 08
    je     SHORT loc_01CA                                 ; 01C1: 74 07
    add    eax,058h                                 ; 01C3: 83 C0 58
    cmp    DWORD PTR [eax],ecx                      ; 01C6: 39 08
    jne    SHORT loc_01DF                                 ; 01C8: 75 15
loc_01CA:
    add    eax,019h                                 ; 01CA: 83 C0 19
    mov    cl,BYTE PTR [eax-5h]                     ; 01CD: 8A 48 FB
    cmp    cl,0E8h                                  ; 01D0: 80 F9 E8
    je     SHORT loc_01DA                                 ; 01D3: 74 05
    cmp    cl,0CCh                                  ; 01D5: 80 F9 CC
    jne    SHORT loc_01DF                                 ; 01D8: 75 05
loc_01DA:
    add    eax,DWORD PTR [eax-4h]                   ; 01DA: 03 40 FC
    jmp    SHORT loc_0209                                 ; 01DD: EB 2A
loc_01DF:
    lea    eax,[ebx+024F00h]                        ; 01DF: 8D 83 00 4F 02 00
    mov    ecx,02800h                               ; 01E5: B9 00 28 00 00
    call   scan_stub                                ; 01EA: E8 53 00 00 00
    test   ecx,ecx                                  ; 01EF: 85 C9
    jne    SHORT loc_0209                                 ; 01F1: 75 16
    lea    eax,[ebx+02BD00h]                        ; 01F3: 8D 83 00 BD 02 00
    mov    ecx,01000h                               ; 01F9: B9 00 10 00 00
    call   scan_stub                                ; 01FE: E8 3F 00 00 00
    test   ecx,ecx                                  ; 0203: 85 C9
    jne    SHORT loc_0209                                 ; 0205: 75 02
    db 02Bh,0C0h                                    ; 0207: sub    eax,eax [exact encoding]
loc_0209:
    test   eax,eax                                  ; 0209: 85 C0
    stosd                                           ; 020B: AB
    je     SHORT loc_0213                                 ; 020C: 74 05
    mov    eax,OFFSET win9x_create_remote_thread    ; 020E: B8 45 06 00 00 [OMF FIXUPP 020F ABS _TEXT:win9x_create_remote_thread]
loc_0213:
    stosd                                           ; 0213: AB
init_done:
    db 064h,08Fh,005h,000h,000h,000h,000h ; 0214: 64 8F 05 00 00 00 00 [exact 32-bit absolute FS POP]
    pop    ecx                                      ; 021B: 59
    pop    eax                                      ; 021C: 58
    pop    edi                                      ; 021D: 5F
    pop    esi                                      ; 021E: 5E
    pop    ebp                                      ; 021F: 5D
    pop    ebx                                      ; 0220: 5B
    inc    BYTE PTR [eax]                           ; 0221: FE 00
    jmp    elirt_init                               ; 0223: E9 D8 FD FF FF
seh_init:
    mov    edx,DWORD PTR [esp+0Ch]                  ; 0228: 8B 54 24 0C
    add    edx,0B8h                                 ; 022C: 81 C2 B8 00 00 00
    mov    eax,DWORD PTR [esp+8h]                   ; 0232: 8B 44 24 08
    mov    DWORD PTR [edx],OFFSET init_done         ; 0236: C7 02 14 02 00 00 [OMF FIXUPP 0238 ABS _TEXT:init_done]
    mov    DWORD PTR [edx+0Ch],eax                  ; 023C: 89 42 0C
    db 033h,0C0h                                    ; 023F: xor    eax,eax [exact encoding]
    ret                                             ; 0241: C3
scan_stub:
    cmp    DWORD PTR [eax],0FFFFF000h               ; 0242: 81 38 00 F0 FF FF
    jne    SHORT loc_0257                                 ; 0248: 75 0D
    mov    dl,BYTE PTR [eax+5h]                     ; 024A: 8A 50 05
    cmp    dl,0E8h                                  ; 024D: 80 FA E8
    je     SHORT loc_025C                                 ; 0250: 74 0A
    cmp    dl,0CCh                                  ; 0252: 80 FA CC
    je     SHORT loc_025C                                 ; 0255: 74 05
loc_0257:
    dec    eax                                      ; 0257: 48
    dec    ecx                                      ; 0258: 49
    jne    SHORT scan_stub                                ; 0259: 75 E7
    ret                                             ; 025B: C3
loc_025C:
    add    eax,0Ah                                  ; 025C: 83 C0 0A
    add    eax,DWORD PTR [eax-4h]                   ; 025F: 03 40 FC
    ret                                             ; 0262: C3
resolve_export:
    push   esi                                      ; 0263: 56
    push   edi                                      ; 0264: 57
    test   ebx,ebx                                  ; 0265: 85 DB
    je     SHORT loc_02E3                                 ; 0267: 74 7A
    mov    edx,DWORD PTR [ebx+03Ch]                 ; 0269: 8B 53 3C
    mov    edi,DWORD PTR [ebx+edx*1+07Ch]           ; 026C: 8B 7C 13 7C
    test   edi,edi                                  ; 0270: 85 FF
    je     SHORT loc_02E3                                 ; 0272: 74 6F
    mov    edx,DWORD PTR [ebx+edx*1+078h]           ; 0274: 8B 54 13 78
    test   edx,edx                                  ; 0278: 85 D2
    je     SHORT loc_02E3                                 ; 027A: 74 67
    db 08Bh,0F2h                                    ; 027C: mov    esi,edx [exact encoding]
    db 003h,0FEh                                    ; 027E: add    edi,esi [exact encoding]
    db 003h,0D3h                                    ; 0280: add    edx,ebx [exact encoding]
    cmp    eax,010000h                              ; 0282: 3D 00 00 01 00
    jae    SHORT loc_028E                                 ; 0287: 73 05
    sub    eax,DWORD PTR [edx+010h]                 ; 0289: 2B 42 10
    jmp    SHORT loc_02CD                                 ; 028C: EB 3F
loc_028E:
    push   ebp                                      ; 028E: 55
    push   esi                                      ; 028F: 56
    push   edi                                      ; 0290: 57
    xchg   ebp,eax                                  ; 0291: 95
    mov    ecx,DWORD PTR [edx+018h]                 ; 0292: 8B 4A 18
    mov    esi,DWORD PTR [edx+020h]                 ; 0295: 8B 72 20
    db 003h,0F3h                                    ; 0298: add    esi,ebx [exact encoding]
    push   edx                                      ; 029A: 52
loc_029B:
    jecxz  loc_02B6                                 ; 029B: E3 19
    mov    eax,DWORD PTR [esi]                      ; 029D: 8B 06
    db 02Bh,0FFh                                    ; 029F: sub    edi,edi [exact encoding]
    add    esi,4h                                   ; 02A1: 83 C6 04
    db 003h,0C3h                                    ; 02A4: add    eax,ebx [exact encoding]
    dec    ecx                                      ; 02A6: 49
loc_02A7:
    mov    dl,BYTE PTR [edi+eax*1]                  ; 02A7: 8A 14 07
    cmp    dl,BYTE PTR [edi+ebp*1]                  ; 02AA: 3A 14 2F
    jne    SHORT loc_029B                                 ; 02AD: 75 EC
    inc    edi                                      ; 02AF: 47
    test   dl,dl                                    ; 02B0: 84 D2
    jne    SHORT loc_02A7                                 ; 02B2: 75 F3
    db 02Bh,0C0h                                    ; 02B4: sub    eax,eax [exact encoding]
loc_02B6:
    pop    edx                                      ; 02B6: 5A
    pop    edi                                      ; 02B7: 5F
    pop    esi                                      ; 02B8: 5E
    pop    ebp                                      ; 02B9: 5D
    test   eax,eax                                  ; 02BA: 85 C0
    jne    SHORT loc_02E3                                 ; 02BC: 75 25
    sub    ecx,DWORD PTR [edx+018h]                 ; 02BE: 2B 4A 18
    mov    eax,DWORD PTR [edx+024h]                 ; 02C1: 8B 42 24
    inc    ecx                                      ; 02C4: 41
    neg    ecx                                      ; 02C5: F7 D9
    db 003h,0C3h                                    ; 02C7: add    eax,ebx [exact encoding]
    movzx  eax,WORD PTR [eax+ecx*2]                 ; 02C9: 0F B7 04 48
loc_02CD:
    db 08Bh,0CBh                                    ; 02CD: mov    ecx,ebx [exact encoding]
    add    ecx,DWORD PTR [edx+01Ch]                 ; 02CF: 03 4A 1C
    lea    edx,[ecx+eax*4]                          ; 02D2: 8D 14 81
    mov    eax,DWORD PTR [edx]                      ; 02D5: 8B 02
    db 03Bh,0C6h                                    ; 02D7: cmp    eax,esi [exact encoding]
    jb     SHORT loc_02DF                                 ; 02D9: 72 04
    db 03Bh,0C7h                                    ; 02DB: cmp    eax,edi [exact encoding]
    jb     SHORT loc_02E3                                 ; 02DD: 72 04
loc_02DF:
    db 003h,0C3h                                    ; 02DF: add    eax,ebx [exact encoding]
    jmp    SHORT loc_02E5                                 ; 02E1: EB 02
loc_02E3:
    db 02Bh,0C0h                                    ; 02E3: sub    eax,eax [exact encoding]
loc_02E5:
    pop    edi                                      ; 02E5: 5F
    pop    esi                                      ; 02E6: 5E
    ret                                             ; 02E7: C3
name_RtlNtStatusToDosError LABEL BYTE
    db "RtlNtStatusToDosError",0

name_NtAllocateVirtualMemory LABEL BYTE
    db "NtAllocateVirtualMemory",0

name_NtFreeVirtualMemory LABEL BYTE
    db "NtFreeVirtualMemory",0

name_NtOpenThread LABEL BYTE
    db "NtOpenThread",0

name_ReadProcessMemory LABEL BYTE
    db "ReadProcessMemory",0

name_SetThreadAffinityMask LABEL BYTE
    db "SetThreadAffinityMask",0

RT_GetVersion:
    call   elirt_init                               ; 035F: E8 9C FC FF FF
    mov    eax,DWORD PTR [edx]                      ; 0364: 8B 02
    ret    4h                                       ; 0366: C2 04 00
xVirtualAllocEx:
    call   elirt_init                               ; 0369: E8 92 FC FF FF
    mov    ecx,DWORD PTR [edx+0Ch]                  ; 036E: 8B 4A 0C
    jecxz  loc_0375                                 ; 0371: E3 02
    jmp    ecx                                      ; 0373: FF E1
loc_0375:
    call   fail_no_function                         ; 0375: E8 4E 01 00 00
    ret    014h                                     ; 037A: C2 14 00
win9x_virtual_alloc:
    push   ebp                                      ; 037D: 55
    db 08Bh,0ECh                                    ; 037E: mov    ebp,esp [exact encoding]
    push   DWORD PTR [ebp+8h]                       ; 0380: FF 75 08
    call   query_remote                             ; 0383: E8 59 01 00 00
    test   eax,eax                                  ; 0388: 85 C0
    je     SHORT loc_03B2                                 ; 038A: 74 26
    mov    edx,DWORD PTR [ebp+014h]                 ; 038C: 8B 55 14
    test   ecx,ecx                                  ; 038F: 85 C9
    je     SHORT loc_03A2                                 ; 0391: 74 0F
    test   DWORD PTR [eax+020h],030800008h          ; 0393: F7 40 20 08 00 80 30
    jne    SHORT loc_03B6                                 ; 039A: 75 1A
    or     edx,08000000h                            ; 039C: 81 CA 00 00 00 08
loc_03A2:
    push   DWORD PTR [ebp+018h]                     ; 03A2: FF 75 18
    push   edx                                      ; 03A5: 52
    push   DWORD PTR [ebp+010h]                     ; 03A6: FF 75 10
    push   DWORD PTR [ebp+0Ch]                      ; 03A9: FF 75 0C
    db 036h                                     ; 03AC: SS override
    call VirtualAlloc                               ; 03AD: E8 00 00 00 00 [OMF FIXUPP 03AE REL EXT:VirtualAlloc]
loc_03B2:
    leave                                           ; 03B2: C9
    ret    014h                                     ; 03B3: C2 14 00
loc_03B6:
    push   5h                                       ; 03B6: 6A 05
    pop    ecx                                      ; 03B8: 59
    call   fail_set_error                           ; 03B9: E8 0D 01 00 00
    jmp    SHORT loc_03B2                                 ; 03BE: EB F2
nt_virtual_alloc:
    push   ebp                                      ; 03C0: 55
    db 08Bh,0ECh                                    ; 03C1: mov    ebp,esp [exact encoding]
    lea    ecx,[ebp+0Ch]                            ; 03C3: 8D 4D 0C
    mov    eax,DWORD PTR [ecx]                      ; 03C6: 8B 01
    test   eax,eax                                  ; 03C8: 85 C0
    je     SHORT loc_03D5                                 ; 03CA: 74 09
    cmp    eax,010000h                              ; 03CC: 3D 00 00 01 00
    jb     SHORT loc_040F                                 ; 03D1: 72 3C
    db 02Bh,0C0h                                    ; 03D3: sub    eax,eax [exact encoding]
loc_03D5:
    push   edx                                      ; 03D5: 52
    push   OFFSET seh_nt_alloc                      ; 03D6: 68 19 04 00 00 [OMF FIXUPP 03D7 ABS _TEXT:seh_nt_alloc]
    push   DWORD PTR fs:[eax]                       ; 03DB: 64 FF 30
    mov    DWORD PTR fs:[eax],esp                   ; 03DE: 64 89 20
    lea    eax,[ebp+010h]                           ; 03E1: 8D 45 10
    push   DWORD PTR [ebp+018h]                     ; 03E4: FF 75 18
    push   DWORD PTR [ebp+014h]                     ; 03E7: FF 75 14
    push   eax                                      ; 03EA: 50
    push   0h                                       ; 03EB: 6A 00
    push   ecx                                      ; 03ED: 51
    push   DWORD PTR [ebp+8h]                       ; 03EE: FF 75 08
    call   DWORD PTR [edx+8h]                       ; 03F1: FF 52 08
nt_alloc_finish:
    test   eax,eax                                  ; 03F4: 85 C0
    db 064h,08Fh,005h,000h,000h,000h,000h ; 03F6: 64 8F 05 00 00 00 00 [exact 32-bit absolute FS POP]
    pop    ecx                                      ; 03FD: 59
    pop    edx                                      ; 03FE: 5A
    js     SHORT loc_0408                                 ; 03FF: 78 07
    mov    eax,DWORD PTR [ebp+0Ch]                  ; 0401: 8B 45 0C
loc_0404:
    leave                                           ; 0404: C9
    ret    014h                                     ; 0405: C2 14 00
loc_0408:
    call   fail_ntstatus                            ; 0408: E8 C8 00 00 00
    jmp    SHORT loc_0404                                 ; 040D: EB F5
loc_040F:
    push   057h                                     ; 040F: 6A 57
    pop    ecx                                      ; 0411: 59
    call   fail_set_error                           ; 0412: E8 B4 00 00 00
    jmp    SHORT loc_0404                                 ; 0417: EB EB
seh_nt_alloc:
    mov    edx,DWORD PTR [esp+0Ch]                  ; 0419: 8B 54 24 0C
    add    edx,0B8h                                 ; 041D: 81 C2 B8 00 00 00
    mov    eax,DWORD PTR [esp+8h]                   ; 0423: 8B 44 24 08
    mov    DWORD PTR [edx],OFFSET nt_alloc_finish   ; 0427: C7 02 F4 03 00 00 [OMF FIXUPP 0429 ABS _TEXT:nt_alloc_finish]
    mov    DWORD PTR [edx+0Ch],eax                  ; 042D: 89 42 0C
    db 033h,0C0h                                    ; 0430: xor    eax,eax [exact encoding]
    ret                                             ; 0432: C3
xVirtualFreeEx:
    call   elirt_init                               ; 0433: E8 C8 FB FF FF
    mov    ecx,DWORD PTR [edx+014h]                 ; 0438: 8B 4A 14
    jecxz  loc_043F                                 ; 043B: E3 02
    jmp    ecx                                      ; 043D: FF E1
loc_043F:
    call   fail_no_function                         ; 043F: E8 84 00 00 00
    ret    010h                                     ; 0444: C2 10 00
win9x_virtual_free:
    push   ebp                                      ; 0447: 55
    db 08Bh,0ECh                                    ; 0448: mov    ebp,esp [exact encoding]
    push   DWORD PTR [ebp+8h]                       ; 044A: FF 75 08
    call   query_remote                             ; 044D: E8 8F 00 00 00
    test   eax,eax                                  ; 0452: 85 C0
    je     SHORT loc_0477                                 ; 0454: 74 21
    mov    edx,DWORD PTR [ebp+0Ch]                  ; 0456: 8B 55 0C
    test   ecx,ecx                                  ; 0459: 85 C9
    je     SHORT loc_046A                                 ; 045B: 74 0D
    test   DWORD PTR [eax+020h],030800008h          ; 045D: F7 40 20 08 00 80 30
    jne    SHORT loc_047B                                 ; 0464: 75 15
    test   edx,edx                                  ; 0466: 85 D2
    jns    SHORT loc_0480                                 ; 0468: 79 16
loc_046A:
    push   DWORD PTR [ebp+014h]                     ; 046A: FF 75 14
    push   DWORD PTR [ebp+010h]                     ; 046D: FF 75 10
    push   edx                                      ; 0470: 52
    db 036h                                     ; 0471: SS override
    call VirtualFree                                ; 0472: E8 00 00 00 00 [OMF FIXUPP 0473 REL EXT:VirtualFree]
loc_0477:
    leave                                           ; 0477: C9
    ret    010h                                     ; 0478: C2 10 00
loc_047B:
    push   5h                                       ; 047B: 6A 05
    pop    ecx                                      ; 047D: 59
    jmp    SHORT loc_0485                                 ; 047E: EB 05
loc_0480:
    mov    ecx,01E7h                                ; 0480: B9 E7 01 00 00
loc_0485:
    call   fail_set_error                           ; 0485: E8 41 00 00 00
    jmp    SHORT loc_0477                                 ; 048A: EB EB
nt_virtual_free:
    push   ebp                                      ; 048C: 55
    db 08Bh,0ECh                                    ; 048D: mov    ebp,esp [exact encoding]
    lea    eax,[ebp+010h]                           ; 048F: 8D 45 10
    test   BYTE PTR [ebp+015h],080h                 ; 0492: F6 45 15 80
    je     SHORT loc_049D                                 ; 0496: 74 05
    cmp    DWORD PTR [eax],0h                       ; 0498: 83 38 00
    jne    SHORT loc_04BE                                 ; 049B: 75 21
loc_049D:
    lea    ecx,[ebp+0Ch]                            ; 049D: 8D 4D 0C
    push   edx                                      ; 04A0: 52
    push   DWORD PTR [ebp+014h]                     ; 04A1: FF 75 14
    push   eax                                      ; 04A4: 50
    push   ecx                                      ; 04A5: 51
    push   DWORD PTR [ebp+8h]                       ; 04A6: FF 75 08
    call   DWORD PTR [edx+010h]                     ; 04A9: FF 52 10
    test   eax,eax                                  ; 04AC: 85 C0
    pop    edx                                      ; 04AE: 5A
    js     SHORT loc_04B7                                 ; 04AF: 78 06
    mov    al,1h                                    ; 04B1: B0 01
loc_04B3:
    leave                                           ; 04B3: C9
    ret    010h                                     ; 04B4: C2 10 00
loc_04B7:
    call   fail_ntstatus                            ; 04B7: E8 19 00 00 00
    jmp    SHORT loc_04B3                                 ; 04BC: EB F5
loc_04BE:
    push   057h                                     ; 04BE: 6A 57
    pop    ecx                                      ; 04C0: 59
    call   fail_set_error                           ; 04C1: E8 05 00 00 00
    jmp    SHORT loc_04B3                                 ; 04C6: EB EB
fail_no_function:
    push   078h                                     ; 04C8: 6A 78
    pop    ecx                                      ; 04CA: 59
fail_set_error:
    push   ecx                                      ; 04CB: 51
    db 036h                                     ; 04CC: SS override
    call SetLastError                               ; 04CD: E8 00 00 00 00 [OMF FIXUPP 04CE REL EXT:SetLastError]
    db 02Bh,0C0h                                    ; 04D2: sub    eax,eax [exact encoding]
    ret                                             ; 04D4: C3
fail_ntstatus:
    mov    ecx,DWORD PTR [edx+4h]                   ; 04D5: 8B 4A 04
    jecxz  fail_no_function                         ; 04D8: E3 EE
    push   eax                                      ; 04DA: 50
    call   ecx                                      ; 04DB: FF D1
    db 08Bh,0C8h                                    ; 04DD: mov    ecx,eax [exact encoding]
    jmp    SHORT fail_set_error                           ; 04DF: EB EA
query_remote:
    push   ebx                                      ; 04E1: 53
    db 08Bh,0DAh                                    ; 04E2: mov    ebx,edx [exact encoding]
    mov    edx,DWORD PTR [esp+8h]                   ; 04E4: 8B 54 24 08
    push   edx                                      ; 04E8: 52
    push   edx                                      ; 04E9: 52
    db 036h                                     ; 04EA: SS override
    call GetPriorityClass                           ; 04EB: E8 00 00 00 00 [OMF FIXUPP 04EC REL EXT:GetPriorityClass]
    test   eax,eax                                  ; 04F0: 85 C0
    pop    edx                                      ; 04F2: 5A
    je     SHORT loc_0537                                 ; 04F3: 74 42
    db 02Bh,0C9h                                    ; 04F5: sub    ecx,ecx [exact encoding]
    cmp    edx,07FFFFFFFh                           ; 04F7: 81 FA FF FF FF 7F
    mov    eax,DWORD PTR fs:[ecx+030h]              ; 04FD: 64 8B 41 30
    je     SHORT loc_0537                                 ; 0501: 74 34
    cmp    edx,010000000h                           ; 0503: 81 FA 00 00 00 10
    xchg   ecx,eax                                  ; 0509: 91
    jae    SHORT loc_0537                                 ; 050A: 73 2B
    cmp    BYTE PTR [ebx+2h],al                     ; 050C: 38 43 02
    je     SHORT loc_0514                                 ; 050F: 74 03
    shr    edx,2h                                   ; 0511: C1 EA 02
loc_0514:
    inc    edx                                      ; 0514: 42
    shl    edx,3h                                   ; 0515: C1 E2 03
    push   edx                                      ; 0518: 52
    push   ecx                                      ; 0519: 51
    push   DWORD PTR [ebx+030h]                     ; 051A: FF 73 30
    call   DWORD PTR [ebx+010h]                     ; 051D: FF 53 10
    pop    ecx                                      ; 0520: 59
    pop    edx                                      ; 0521: 5A
    add    edx,DWORD PTR [ecx+044h]                 ; 0522: 03 51 44
    mov    eax,DWORD PTR [edx]                      ; 0525: 8B 02
    db 03Bh,0C8h                                    ; 0527: cmp    ecx,eax [exact encoding]
    jne    SHORT loc_052D                                 ; 0529: 75 02
    db 02Bh,0C9h                                    ; 052B: sub    ecx,ecx [exact encoding]
loc_052D:
    push   eax                                      ; 052D: 50
    push   ecx                                      ; 052E: 51
    push   DWORD PTR [ebx+030h]                     ; 052F: FF 73 30
    call   DWORD PTR [ebx+018h]                     ; 0532: FF 53 18
    pop    ecx                                      ; 0535: 59
    pop    eax                                      ; 0536: 58
loc_0537:
    pop    ebx                                      ; 0537: 5B
    ret    4h                                       ; 0538: C2 04 00
win9x_open_thread_worker:
    push   ebp                                      ; 053B: 55
    db 08Bh,0ECh                                    ; 053C: mov    ebp,esp [exact encoding]
    add    esp,-4                                   ; 053E: 83 C4 FC
    push   ebx                                      ; 0541: 53
    db 08Bh,0DAh                                    ; 0542: mov    ebx,edx [exact encoding]
    db 02Bh,0C9h                                    ; 0544: sub    ecx,ecx [exact encoding]
    push   6h                                       ; 0546: 6A 06
    pop    edx                                      ; 0548: 5A
    mov    eax,DWORD PTR fs:[ecx+030h]              ; 0549: 64 8B 41 30
    cmp    BYTE PTR [ebx+2h],cl                     ; 054D: 38 4B 02
    jne    SHORT loc_0555                                 ; 0550: 75 03
    dec    edx                                      ; 0552: 4A
    mov    DWORD PTR [eax],edx                      ; 0553: 89 10
loc_0555:
    mov    BYTE PTR [eax],dl                        ; 0555: 88 10
    xor    eax,DWORD PTR [ebx+4h]                   ; 0557: 33 43 04
    push   eax                                      ; 055A: 50
    push   DWORD PTR [ebp+0Ch]                      ; 055B: FF 75 0C
    push   DWORD PTR [ebp+8h]                       ; 055E: FF 75 08
    db 036h                                     ; 0561: SS override
    call OpenProcess                                ; 0562: E8 00 00 00 00 [OMF FIXUPP 0563 REL EXT:OpenProcess]
    test   eax,eax                                  ; 0567: 85 C0
    je     SHORT loc_05D2                                 ; 0569: 74 67
    push   eax                                      ; 056B: 50
    push   DWORD PTR [ebx+030h]                     ; 056C: FF 73 30
    call   DWORD PTR [ebx+010h]                     ; 056F: FF 53 10
    push   DWORD PTR [ebp+010h]                     ; 0572: FF 75 10
    call   DWORD PTR [ebx+8h]                       ; 0575: FF 53 08
    test   eax,eax                                  ; 0578: 85 C0
    sete   BYTE PTR [ebp-1h]                        ; 057A: 0F 94 45 FF
    pop    eax                                      ; 057E: 58
    je     SHORT loc_05B5                                 ; 057F: 74 34
    db 02Bh,0C9h                                    ; 0581: sub    ecx,ecx [exact encoding]
    cmp    BYTE PTR [ebx+2h],cl                     ; 0583: 38 4B 02
    mov    edx,DWORD PTR fs:[ecx+030h]              ; 0586: 64 8B 51 30
    push   eax                                      ; 058A: 50
    je     SHORT loc_0590                                 ; 058B: 74 03
    shr    eax,2h                                   ; 058D: C1 E8 02
loc_0590:
    inc    eax                                      ; 0590: 40
    shl    eax,3h                                   ; 0591: C1 E0 03
    add    eax,DWORD PTR [edx+044h]                 ; 0594: 03 42 44
    mov    ecx,DWORD PTR [ebp+010h]                 ; 0597: 8B 4D 10
    xor    ecx,DWORD PTR [ebx+4h]                   ; 059A: 33 4B 04
    mov    DWORD PTR [eax],ecx                      ; 059D: 89 08
    cmp    BYTE PTR [ecx],7h                        ; 059F: 80 39 07
    pop    eax                                      ; 05A2: 58
    jae    SHORT loc_05AD                                 ; 05A3: 73 08
    inc    DWORD PTR [ecx+4h]                       ; 05A5: FF 41 04
    dec    DWORD PTR [edx+4h]                       ; 05A8: FF 4A 04
    jmp    SHORT loc_05B5                                 ; 05AB: EB 08
loc_05AD:
    inc    WORD PTR [ecx+2h]                        ; 05AD: 66 FF 41 02
    dec    WORD PTR [edx+2h]                        ; 05B1: 66 FF 4A 02
loc_05B5:
    push   eax                                      ; 05B5: 50
    push   DWORD PTR [ebx+030h]                     ; 05B6: FF 73 30
    call   DWORD PTR [ebx+018h]                     ; 05B9: FF 53 18
    pop    eax                                      ; 05BC: 58
    cmp    BYTE PTR [ebp-1h],0h                     ; 05BD: 80 7D FF 00
    je     SHORT loc_05D2                                 ; 05C1: 74 0F
    push   eax                                      ; 05C3: 50
    db 036h                                     ; 05C4: SS override
    call CloseHandle                                ; 05C5: E8 00 00 00 00 [OMF FIXUPP 05C6 REL EXT:CloseHandle]
    push   057h                                     ; 05CA: 6A 57
    pop    ecx                                      ; 05CC: 59
    call   fail_set_error                           ; 05CD: E8 F9 FE FF FF
loc_05D2:
    pop    ebx                                      ; 05D2: 5B
    leave                                           ; 05D3: C9
    ret    0Ch                                      ; 05D4: C2 0C 00
xOpenThread:
    call   elirt_init                               ; 05D7: E8 24 FA FF FF
    mov    ecx,DWORD PTR [edx+01Ch]                 ; 05DC: 8B 4A 1C
    jecxz  loc_05E3                                 ; 05DF: E3 02
    jmp    ecx                                      ; 05E1: FF E1
loc_05E3:
    call   fail_no_function                         ; 05E3: E8 E0 FE FF FF
    ret    0Ch                                      ; 05E8: C2 0C 00
nt_open_thread:
    push   ebp                                      ; 05EB: 55
    db 08Bh,0ECh                                    ; 05EC: mov    ebp,esp [exact encoding]
    add    esp,-28                                  ; 05EE: 83 C4 E4
    push   edi                                      ; 05F1: 57
    push   edx                                      ; 05F2: 52
    push   DWORD PTR [ebp+010h]                     ; 05F3: FF 75 10
    push   0h                                       ; 05F6: 6A 00
    lea    edi,[ebp-01Ch]                           ; 05F8: 8D 7D E4
    push   esp                                      ; 05FB: 54
    push   edi                                      ; 05FC: 57
    push   DWORD PTR [ebp+8h]                       ; 05FD: FF 75 08
    push   018h                                     ; 0600: 6A 18
    pop    eax                                      ; 0602: 58
    stosd                                           ; 0603: AB
    db 02Bh,0C0h                                    ; 0604: sub    eax,eax [exact encoding]
    push   5h                                       ; 0606: 6A 05
    pop    ecx                                      ; 0608: 59
    rep stosd                                       ; 0609: F3 AB
    push   edi                                      ; 060B: 57
    stosd                                           ; 060C: AB
    cmp    DWORD PTR [ebp+0Ch],eax                  ; 060D: 39 45 0C
    setne  al                                       ; 0610: 0F 95 C0
    shl    al,1                                     ; 0613: D0 E0
    or     BYTE PTR [ebp-010h],al                   ; 0615: 08 45 F0
    call   DWORD PTR [edx+018h]                     ; 0618: FF 52 18
    test   eax,eax                                  ; 061B: 85 C0
    pop    ecx                                      ; 061D: 59
    pop    ecx                                      ; 061E: 59
    pop    edx                                      ; 061F: 5A
    js     SHORT loc_062A                                 ; 0620: 78 08
    mov    eax,DWORD PTR [ebp-4h]                   ; 0622: 8B 45 FC
loc_0625:
    pop    edi                                      ; 0625: 5F
    leave                                           ; 0626: C9
    ret    0Ch                                      ; 0627: C2 0C 00
loc_062A:
    call   fail_ntstatus                            ; 062A: E8 A6 FE FF FF
    jmp    SHORT loc_0625                                 ; 062F: EB F4
xCreateRemoteThread:
    call   elirt_init                               ; 0631: E8 CA F9 FF FF
    mov    ecx,DWORD PTR [edx+028h]                 ; 0636: 8B 4A 28
    jecxz  loc_063D                                 ; 0639: E3 02
    jmp    ecx                                      ; 063B: FF E1
loc_063D:
    call   fail_no_function                         ; 063D: E8 86 FE FF FF
    ret    01Ch                                     ; 0642: C2 1C 00
win9x_create_remote_thread:
    push   ebp                                      ; 0645: 55
    db 08Bh,0ECh                                    ; 0646: mov    ebp,esp [exact encoding]
    add    esp,-4                                   ; 0648: 83 C4 FC
    push   ebx                                      ; 064B: 53
    mov    ecx,DWORD PTR [ebp+0Ch]                  ; 064C: 8B 4D 0C
    db 08Bh,0DAh                                    ; 064F: mov    ebx,edx [exact encoding]
    mov    DWORD PTR [ebp-4h],ecx                   ; 0651: 89 4D FC
    jecxz  loc_0665                                 ; 0654: E3 0F
    cmp    DWORD PTR [ecx],0Ch                      ; 0656: 83 39 0C
    jne    loc_0765                                 ; 0659: 0F 85 06 01 00 00
    push   DWORD PTR [ecx+8h]                       ; 065F: FF 71 08
    pop    DWORD PTR [ebp-4h]                       ; 0662: 8F 45 FC
loc_0665:
    push   DWORD PTR [ebp+8h]                       ; 0665: FF 75 08
    call   query_remote                             ; 0668: E8 74 FE FF FF
    test   eax,eax                                  ; 066D: 85 C0
    je     loc_075B                                 ; 066F: 0F 84 E6 00 00 00
    mov    edx,DWORD PTR [ebp+010h]                 ; 0675: 8B 55 10
    test   ecx,ecx                                  ; 0678: 85 C9
    je     SHORT loc_0692                                 ; 067A: 74 16
    test   DWORD PTR [eax+020h],030800008h          ; 067C: F7 40 20 08 00 80 30
    jne    loc_0760                                 ; 0683: 0F 85 D7 00 00 00
    neg    edx                                      ; 0689: F7 DA
    jne    SHORT loc_0692                                 ; 068B: 75 05
    mov    edx,0FFFCF000h                           ; 068D: BA 00 F0 FC FF
loc_0692:
    push   ecx                                      ; 0692: 51
    push   048h                                     ; 0693: 6A 48
    push   DWORD PTR [ebp+018h]                     ; 0695: FF 75 18
    push   DWORD PTR [ebp+014h]                     ; 0698: FF 75 14
    push   edx                                      ; 069B: 52
    push   eax                                      ; 069C: 50
    test   ecx,ecx                                  ; 069D: 85 C9
    je     SHORT loc_06EF                                 ; 069F: 74 4E
    mov    edx,DWORD PTR [eax+050h]                 ; 06A1: 8B 50 50
    test   edx,edx                                  ; 06A4: 85 D2
    jns    SHORT loc_06EA                                 ; 06A6: 79 42
    mov    ecx,DWORD PTR [edx]                      ; 06A8: 8B 0A
    test   ecx,ecx                                  ; 06AA: 85 C9
    jns    SHORT loc_06EA                                 ; 06AC: 79 3C
    mov    eax,DWORD PTR [ecx+8h]                   ; 06AE: 8B 41 08
    test   eax,eax                                  ; 06B1: 85 C0
    jns    SHORT loc_06EA                                 ; 06B3: 79 35
    push   eax                                      ; 06B5: 50
    xor    eax,DWORD PTR [ebx+4h]                   ; 06B6: 33 43 04
    push   eax                                      ; 06B9: 50
    call   DWORD PTR [ebx+8h]                       ; 06BA: FF 53 08
    test   eax,eax                                  ; 06BD: 85 C0
    pop    eax                                      ; 06BF: 58
    je     SHORT loc_06EA                                 ; 06C0: 74 28
    mov    ecx,DWORD PTR [ebx+020h]                 ; 06C2: 8B 4B 20
    jecxz  loc_06EA                                 ; 06C5: E3 23
    cmp    BYTE PTR [ebx+2h],0h                     ; 06C7: 80 7B 02 00
    jne    SHORT loc_06D0                                 ; 06CB: 75 03
    add    eax,8h                                   ; 06CD: 83 C0 08
loc_06D0:
    mov    eax,DWORD PTR [eax+0Ch]                  ; 06D0: 8B 40 0C
    sub    eax,4h                                   ; 06D3: 83 E8 04
    push   0h                                       ; 06D6: 6A 00
    db 08Bh,0D4h                                    ; 06D8: mov    edx,esp [exact encoding]
    push   0h                                       ; 06DA: 6A 00
    push   4h                                       ; 06DC: 6A 04
    push   edx                                      ; 06DE: 52
    push   eax                                      ; 06DF: 50
    push   DWORD PTR [ebp+8h]                       ; 06E0: FF 75 08
    call   ecx                                      ; 06E3: FF D1
    pop    ecx                                      ; 06E5: 59
    test   ecx,ecx                                  ; 06E6: 85 C9
    jns    SHORT loc_06EF                                 ; 06E8: 79 05
loc_06EA:
    or     BYTE PTR [esp+010h],010h                 ; 06EA: 80 4C 24 10 10
loc_06EF:
    mov    al,010h                                  ; 06EF: B0 10
    test   BYTE PTR [ebp+01Ch],al                   ; 06F1: 84 45 1C
    je     SHORT loc_06FA                                 ; 06F4: 74 04
    or     BYTE PTR [esp+010h],al                   ; 06F6: 08 44 24 10
loc_06FA:
    push   DWORD PTR [ebx+02Ch]                     ; 06FA: FF 73 2C
    call   DWORD PTR [ebx+010h]                     ; 06FD: FF 53 10
    push   DWORD PTR [ebx+030h]                     ; 0700: FF 73 30
    call   DWORD PTR [ebx+010h]                     ; 0703: FF 53 10
    call   DWORD PTR [ebx+024h]                     ; 0706: FF 53 24
    push   eax                                      ; 0709: 50
    push   DWORD PTR [ebx+030h]                     ; 070A: FF 73 30
    call   DWORD PTR [ebx+018h]                     ; 070D: FF 53 18
    push   DWORD PTR [ebx+02Ch]                     ; 0710: FF 73 2C
    call   DWORD PTR [ebx+018h]                     ; 0713: FF 53 18
    pop    eax                                      ; 0716: 58
    pop    ecx                                      ; 0717: 59
    test   eax,eax                                  ; 0718: 85 C0
    je     SHORT loc_075B                                 ; 071A: 74 3F
    jecxz  loc_072E                                 ; 071C: E3 10
    push   045h                                     ; 071E: 6A 45
    pop    edx                                      ; 0720: 5A
    cmp    BYTE PTR [ebx+2h],05Ah                   ; 0721: 80 7B 02 5A
    jne    SHORT loc_072A                                 ; 0725: 75 03
    add    edx,8h                                   ; 0727: 83 C2 08
loc_072A:
    or     BYTE PTR [edx+eax*1],8h                  ; 072A: 80 0C 02 08
loc_072E:
    xor    eax,DWORD PTR [ebx+4h]                   ; 072E: 33 43 04
    mov    ecx,DWORD PTR [ebp+020h]                 ; 0731: 8B 4D 20
    push   eax                                      ; 0734: 50
    push   DWORD PTR [ebp-4h]                       ; 0735: FF 75 FC
    push   01F03FFh                                 ; 0738: 68 FF 03 1F 00
    jecxz  loc_0741                                 ; 073D: E3 02
    mov    DWORD PTR [ecx],eax                      ; 073F: 89 01
loc_0741:
    db 08Bh,0D3h                                    ; 0741: mov    edx,ebx [exact encoding]
    call   win9x_open_thread_worker                 ; 0743: E8 F3 FD FF FF
    test   eax,eax                                  ; 0748: 85 C0
    je     SHORT loc_075B                                 ; 074A: 74 0F
    test   BYTE PTR [ebp+01Ch],4h                   ; 074C: F6 45 1C 04
    jne    SHORT loc_075B                                 ; 0750: 75 09
    push   eax                                      ; 0752: 50
    push   eax                                      ; 0753: 50
    db 036h                                     ; 0754: SS override
    call ResumeThread                               ; 0755: E8 00 00 00 00 [OMF FIXUPP 0756 REL EXT:ResumeThread]
    pop    eax                                      ; 075A: 58
loc_075B:
    pop    ebx                                      ; 075B: 5B
    leave                                           ; 075C: C9
    ret    01Ch                                     ; 075D: C2 1C 00
loc_0760:
    push   5h                                       ; 0760: 6A 05
    pop    ecx                                      ; 0762: 59
    jmp    SHORT loc_0768                                 ; 0763: EB 03
loc_0765:
    push   057h                                     ; 0765: 6A 57
    pop    ecx                                      ; 0767: 59
loc_0768:
    call   fail_set_error                           ; 0768: E8 5E FD FF FF
    jmp    SHORT loc_075B                                 ; 076D: EB EC

elirt_banner LABEL BYTE
    db 13,10,"EliRT 1.01",13,10
    db "Copyright (C) 2001, 2003 Radim Picha",13,10
    db 090h

_TEXT ENDS

_DATA SEGMENT PARA PUBLIC USE32 'DATA'
g_init  db 0, 0       ; byte 0 = initialized, byte 1 = initializing
_DATA ENDS

_BSS SEGMENT PARA PUBLIC USE32 'BSS'
g_api   dd 13 DUP (?) ; 0x34 bytes; API/runtime function pointer table
_BSS ENDS

END
