; ============================================================
; Section: XGRPH
; VA: 0x001F0F60 - 0x001F125C
; Size: 764 bytes (0.7 KB)
; Functions: 8
; Instructions: 296
; ============================================================


; ============================================================
; Function: sub_001F0F60
; Start: 0x001F0F60  End: 0x001F0F77  Size: 23 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_001F0F88, sub_001F0F9B
; ============================================================
sub_001F0F60:
  0x001F0F60  8b442404                mov      eax, dword ptr [esp + 4]       
  0x001F0F64  83f80c                  cmp      eax, 0xc                       
  0x001F0F67  740e                    je       0x1f0f77                       
  0x001F0F69  83f80d                  cmp      eax, 0xd                       
  0x001F0F6C  7605                    jbe      0x1f0f73                       
  0x001F0F6E  83f80f                  cmp      eax, 0xf                       
  0x001F0F71  7604                    jbe      0x1f0f77                       
                                        ; XREF: 0x001F0F6C (cond_jump)
  0x001F0F73  32c0                    xor      al, al                         
  0x001F0F75  eb02                    jmp      0x1f0f79                       
; end of function
                                        ; XREF: 0x001F0F67 (cond_jump), 0x001F0F71 (cond_jump)
  0x001F0F77  b001                    mov      al, 1                          
                                        ; XREF: 0x001F0F75 (jump)
  0x001F0F79  c20400                  ret      4                              

; ============================================================
; Function: sub_001F0F7C
; Start: 0x001F0F7C  End: 0x001F0F88  Size: 12 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_001F0F7C:
  0x001F0F7C  51                      push     ecx                            
  0x001F0F7D  894c2400                mov      dword ptr [esp], ecx           
  0x001F0F81  0fbc442400              bsf      eax, dword ptr [esp]           
  0x001F0F86  59                      pop      ecx                            
  0x001F0F87  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_001F0F88
; Start: 0x001F0F88  End: 0x001F0F9B  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_001F0F60
; ============================================================
sub_001F0F88:
  0x001F0F88  ff742404                push     dword ptr [esp + 4]            
  0x001F0F8C  e8cfffffff              call     0x1f0f60                       ; -> sub_001F0F60
  0x001F0F91  f6d8                    neg      al                             
  0x001F0F93  1bc0                    sbb      eax, eax                       
  0x001F0F95  83e002                  and      eax, 2                         
  0x001F0F98  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_001F0F9B
; Start: 0x001F0F9B  End: 0x001F100C  Size: 113 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_001F0F60
; Called by: sub_001F1145, sub_001F1193
; ============================================================
sub_001F0F9B:
  0x001F0F9B  55                      push     ebp                            
  0x001F0F9C  8bec                    mov      ebp, esp                       
  0x001F0F9E  83ec14                  sub      esp, 0x14                      
  0x001F0FA1  8b4d18                  mov      ecx, dword ptr [ebp + 0x18]    
  0x001F0FA4  8a8118121f00            mov      al, byte ptr [ecx + 0x1f1218]  
  0x001F0FAA  33d2                    xor      edx, edx                       
  0x001F0FAC  8ad0                    mov      dl, al                         
  0x001F0FAE  53                      push     ebx                            
  0x001F0FAF  8a5d24                  mov      bl, byte ptr [ebp + 0x24]      
  0x001F0FB2  56                      push     esi                            
  0x001F0FB3  57                      push     edi                            
  0x001F0FB4  33f6                    xor      esi, esi                       
  0x001F0FB6  8975fc                  mov      dword ptr [ebp - 4], esi       
  0x001F0FB9  83e23c                  and      edx, 0x3c                      
  0x001F0FBC  a801                    test     al, 1                          
  0x001F0FBE  8bfa                    mov      edi, edx                       
  0x001F0FC0  897dec                  mov      dword ptr [ebp - 0x14], edi    
  0x001F0FC3  7547                    jne      0x1f100c                       
  0x001F0FC5  51                      push     ecx                            
  0x001F0FC6  e895ffffff              call     0x1f0f60                       ; -> sub_001F0F60
  0x001F0FCB  84c0                    test     al, al                         
  0x001F0FCD  753d                    jne      0x1f100c                       
  0x001F0FCF  33c0                    xor      eax, eax                       
  0x001F0FD1  394514                  cmp      dword ptr [ebp + 0x14], eax    
  0x001F0FD4  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x001F0FD7  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x001F0FDA  7507                    jne      0x1f0fe3                       
  0x001F0FDC  c7451401000000          mov      dword ptr [ebp + 0x14], 1      
                                        ; XREF: 0x001F0FDA (cond_jump)
  0x001F0FE3  8b4d1c                  mov      ecx, dword ptr [ebp + 0x1c]    
  0x001F0FE6  3bc8                    cmp      ecx, eax                       
  0x001F0FE8  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x001F0FEB  750e                    jne      0x1f0ffb                       
  0x001F0FED  0faff8                  imul     edi, eax                       
  0x001F0FF0  c1ef03                  shr      edi, 3                         
  0x001F0FF3  83c73f                  add      edi, 0x3f                      
  0x001F0FF6  83e7c0                  and      edi, 0xffffffc0                
  0x001F0FF9  8bcf                    mov      ecx, edi                       
                                        ; XREF: 0x001F0FEB (cond_jump)
  0x001F0FFB  894510                  mov      dword ptr [ebp + 0x10], eax    
  0x001F0FFE  8b450c                  mov      eax, dword ptr [ebp + 0xc]     
  0x001F1001  8945f0                  mov      dword ptr [ebp - 0x10], eax    
  0x001F1004  0fafc1                  imul     eax, ecx                       
  0x001F1007  e9bf000000              jmp      0x1f10cb                       
; end of function
                                        ; XREF: 0x001F0FC3 (cond_jump), 0x001F0FCD (cond_jump)
  0x001F100C  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x001F100F  e868ffffff              call     0x1f0f7c                       ; -> sub_001F0F7C
  0x001F1014  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x001F1017  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x001F101A  e85dffffff              call     0x1f0f7c                       ; -> sub_001F0F7C
  0x001F101F  8b4d10                  mov      ecx, dword ptr [ebp + 0x10]    
  0x001F1022  8bf8                    mov      edi, eax                       
  0x001F1024  897df4                  mov      dword ptr [ebp - 0xc], edi     
  0x001F1027  e850ffffff              call     0x1f0f7c                       ; -> sub_001F0F7C
  0x001F102C  ff7518                  push     dword ptr [ebp + 0x18]         
  0x001F102F  83651000                and      dword ptr [ebp + 0x10], 0      
  0x001F1033  8365f000                and      dword ptr [ebp - 0x10], 0      
  0x001F1037  8bf0                    mov      esi, eax                       
  0x001F1039  e84affffff              call     0x1f0f88                       ; -> sub_001F0F88
  0x001F103E  837d1400                cmp      dword ptr [ebp + 0x14], 0      
  0x001F1042  894508                  mov      dword ptr [ebp + 8], eax       
  0x001F1045  751e                    jne      0x1f1065                       
  0x001F1047  3bfe                    cmp      edi, esi                       
  0x001F1049  8bcf                    mov      ecx, edi                       
  0x001F104B  7702                    ja       0x1f104f                       
  0x001F104D  8bce                    mov      ecx, esi                       
                                        ; XREF: 0x001F104B (cond_jump)
  0x001F104F  394df8                  cmp      dword ptr [ebp - 8], ecx       
  0x001F1052  7605                    jbe      0x1f1059                       
  0x001F1054  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x001F1057  eb08                    jmp      0x1f1061                       
                                        ; XREF: 0x001F1052 (cond_jump)
  0x001F1059  3bfe                    cmp      edi, esi                       
  0x001F105B  8bcf                    mov      ecx, edi                       
  0x001F105D  7702                    ja       0x1f1061                       
  0x001F105F  8bce                    mov      ecx, esi                       
                                        ; XREF: 0x001F1057 (jump), 0x001F105D (cond_jump)
  0x001F1061  41                      inc      ecx                            
  0x001F1062  894d14                  mov      dword ptr [ebp + 0x14], ecx    
                                        ; XREF: 0x001F1045 (cond_jump)
  0x001F1065  8b4d14                  mov      ecx, dword ptr [ebp + 0x14]    
  0x001F1068  85c9                    test     ecx, ecx                       
  0x001F106A  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x001F106D  897d24                  mov      dword ptr [ebp + 0x24], edi    
  0x001F1070  8bfe                    mov      edi, esi                       
  0x001F1072  7442                    je       0x1f10b6                       
  0x001F1074  894d0c                  mov      dword ptr [ebp + 0xc], ecx     
                                        ; XREF: 0x001F10B4 (cond_jump)
  0x001F1077  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x001F107A  3bc2                    cmp      eax, edx                       
  0x001F107C  7602                    jbe      0x1f1080                       
  0x001F107E  8bd0                    mov      edx, eax                       
                                        ; XREF: 0x001F107C (cond_jump)
  0x001F1080  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x001F1083  394d24                  cmp      dword ptr [ebp + 0x24], ecx    
  0x001F1086  7603                    jbe      0x1f108b                       
  0x001F1088  8b4d24                  mov      ecx, dword ptr [ebp + 0x24]    
                                        ; XREF: 0x001F1086 (cond_jump)
  0x001F108B  03ca                    add      ecx, edx                       
  0x001F108D  33d2                    xor      edx, edx                       
  0x001F108F  42                      inc      edx                            
  0x001F1090  03cf                    add      ecx, edi                       
  0x001F1092  d3e2                    shl      edx, cl                        
  0x001F1094  0faf55ec                imul     edx, dword ptr [ebp - 0x14]    
  0x001F1098  c1ea03                  shr      edx, 3                         
  0x001F109B  0155fc                  add      dword ptr [ebp - 4], edx       
  0x001F109E  85c0                    test     eax, eax                       
  0x001F10A0  7601                    jbe      0x1f10a3                       
  0x001F10A2  48                      dec      eax                            
                                        ; XREF: 0x001F10A0 (cond_jump)
  0x001F10A3  837d2400                cmp      dword ptr [ebp + 0x24], 0      
  0x001F10A7  7603                    jbe      0x1f10ac                       
  0x001F10A9  ff4d24                  dec      dword ptr [ebp + 0x24]         
                                        ; XREF: 0x001F10A7 (cond_jump)
  0x001F10AC  85ff                    test     edi, edi                       
  0x001F10AE  7601                    jbe      0x1f10b1                       
  0x001F10B0  4f                      dec      edi                            
                                        ; XREF: 0x001F10AE (cond_jump)
  0x001F10B1  ff4d0c                  dec      dword ptr [ebp + 0xc]          
  0x001F10B4  75c1                    jne      0x1f1077                       
                                        ; XREF: 0x001F1072 (cond_jump)
  0x001F10B6  84db                    test     bl, bl                         
  0x001F10B8  8b4d1c                  mov      ecx, dword ptr [ebp + 0x1c]    
  0x001F10BB  7411                    je       0x1f10ce                       
  0x001F10BD  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x001F10C0  83c07f                  add      eax, 0x7f                      
  0x001F10C3  83e080                  and      eax, 0xffffff80                
  0x001F10C6  8d0440                  lea      eax, [eax + eax*2]             
  0x001F10C9  d1e0                    shl      eax, 1                         
                                        ; XREF: 0x001F1007 (jump)
  0x001F10CB  8945fc                  mov      dword ptr [ebp - 4], eax       
                                        ; XREF: 0x001F10BB (cond_jump)
  0x001F10CE  c1e604                  shl      esi, 4                         
  0x001F10D1  0b75f4                  or       esi, dword ptr [ebp - 0xc]     
  0x001F10D4  33c0                    xor      eax, eax                       
  0x001F10D6  c1e604                  shl      esi, 4                         
  0x001F10D9  0b75f8                  or       esi, dword ptr [ebp - 8]       
  0x001F10DC  5f                      pop      edi                            
  0x001F10DD  c1e604                  shl      esi, 4                         
  0x001F10E0  0b7514                  or       esi, dword ptr [ebp + 0x14]    
  0x001F10E3  c1e608                  shl      esi, 8                         
  0x001F10E6  0b7518                  or       esi, dword ptr [ebp + 0x18]    
  0x001F10E9  c1e604                  shl      esi, 4                         
  0x001F10EC  384528                  cmp      byte ptr [ebp + 0x28], al      
  0x001F10EF  0f95c0                  setne    al                             
  0x001F10F2  40                      inc      eax                            
  0x001F10F3  40                      inc      eax                            
  0x001F10F4  0bf0                    or       esi, eax                       
  0x001F10F6  c1e604                  shl      esi, 4                         
  0x001F10F9  33c0                    xor      eax, eax                       
  0x001F10FB  384520                  cmp      byte ptr [ebp + 0x20], al      
  0x001F10FE  0f95c0                  setne    al                             
  0x001F1101  40                      inc      eax                            
  0x001F1102  0bf0                    or       esi, eax                       
  0x001F1104  8b452c                  mov      eax, dword ptr [ebp + 0x2c]    
  0x001F1107  f6db                    neg      bl                             
  0x001F1109  1bdb                    sbb      ebx, ebx                       
  0x001F110B  83e304                  and      ebx, 4                         
  0x001F110E  0bf3                    or       esi, ebx                       
  0x001F1110  83ce08                  or       esi, 8                         
  0x001F1113  8930                    mov      dword ptr [eax], esi           
  0x001F1115  8b4510                  mov      eax, dword ptr [ebp + 0x10]    
  0x001F1118  85c0                    test     eax, eax                       
  0x001F111A  5e                      pop      esi                            
  0x001F111B  5b                      pop      ebx                            
  0x001F111C  741a                    je       0x1f1138                       
  0x001F111E  8b55f0                  mov      edx, dword ptr [ebp - 0x10]    
  0x001F1121  c1e906                  shr      ecx, 6                         
  0x001F1124  49                      dec      ecx                            
  0x001F1125  c1e10c                  shl      ecx, 0xc                       
  0x001F1128  4a                      dec      edx                            
  0x001F1129  0bca                    or       ecx, edx                       
  0x001F112B  c1e10c                  shl      ecx, 0xc                       
  0x001F112E  48                      dec      eax                            
  0x001F112F  0bc8                    or       ecx, eax                       
  0x001F1131  8b4530                  mov      eax, dword ptr [ebp + 0x30]    
  0x001F1134  8908                    mov      dword ptr [eax], ecx           
  0x001F1136  eb06                    jmp      0x1f113e                       
                                        ; XREF: 0x001F111C (cond_jump)
  0x001F1138  8b4530                  mov      eax, dword ptr [ebp + 0x30]    
  0x001F113B  832000                  and      dword ptr [eax], 0             
                                        ; XREF: 0x001F1136 (jump)
  0x001F113E  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x001F1141  c9                      leave                                   
  0x001F1142  c22c00                  ret      0x2c                           

; ============================================================
; Function: sub_001F1145
; Start: 0x001F1145  End: 0x001F1193  Size: 78 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_001F0F9B
; Called by: sub_001F11D6
; ============================================================
sub_001F1145:
  0x001F1145  55                      push     ebp                            
  0x001F1146  8bec                    mov      ebp, esp                       
  0x001F1148  56                      push     esi                            
  0x001F1149  8b7530                  mov      esi, dword ptr [ebp + 0x30]    
  0x001F114C  57                      push     edi                            
  0x001F114D  8d4610                  lea      eax, [esi + 0x10]              
  0x001F1150  50                      push     eax                            
  0x001F1151  8d7e0c                  lea      edi, [esi + 0xc]               
  0x001F1154  57                      push     edi                            
  0x001F1155  ff7528                  push     dword ptr [ebp + 0x28]         
  0x001F1158  ff7524                  push     dword ptr [ebp + 0x24]         
  0x001F115B  6a01                    push     1                              
  0x001F115D  ff7520                  push     dword ptr [ebp + 0x20]         
  0x001F1160  ff751c                  push     dword ptr [ebp + 0x1c]         
  0x001F1163  ff7514                  push     dword ptr [ebp + 0x14]         
  0x001F1166  ff7510                  push     dword ptr [ebp + 0x10]         
  0x001F1169  ff750c                  push     dword ptr [ebp + 0xc]          
  0x001F116C  ff7508                  push     dword ptr [ebp + 8]            
  0x001F116F  e827feffff              call     0x1f0f9b                       ; -> sub_001F0F9B
  0x001F1174  f6451a01                test     byte ptr [ebp + 0x1a], 1       
  0x001F1178  7403                    je       0x1f117d                       
  0x001F117A  8327f7                  and      dword ptr [edi], 0xfffffff7    
                                        ; XREF: 0x001F1178 (cond_jump)
  0x001F117D  8b452c                  mov      eax, dword ptr [ebp + 0x2c]    
  0x001F1180  83660800                and      dword ptr [esi + 8], 0         
  0x001F1184  5f                      pop      edi                            
  0x001F1185  c70601000400            mov      dword ptr [esi], 0x40001       
  0x001F118B  894604                  mov      dword ptr [esi + 4], eax       
  0x001F118E  5e                      pop      esi                            
  0x001F118F  5d                      pop      ebp                            
  0x001F1190  c22c00                  ret      0x2c                           
; end of function

; ============================================================
; Function: sub_001F1193
; Start: 0x001F1193  End: 0x001F11D6  Size: 67 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_001F0F9B
; Called by: sub_0011B8C0, sub_001C1D0F
; ============================================================
sub_001F1193:
  0x001F1193  55                      push     ebp                            
  0x001F1194  8bec                    mov      ebp, esp                       
  0x001F1196  56                      push     esi                            
  0x001F1197  8b7514                  mov      esi, dword ptr [ebp + 0x14]    
  0x001F119A  8d4610                  lea      eax, [esi + 0x10]              
  0x001F119D  50                      push     eax                            
  0x001F119E  8d460c                  lea      eax, [esi + 0xc]               
  0x001F11A1  50                      push     eax                            
  0x001F11A2  6a00                    push     0                              
  0x001F11A4  6a00                    push     0                              
  0x001F11A6  6a01                    push     1                              
  0x001F11A8  ff751c                  push     dword ptr [ebp + 0x1c]         
  0x001F11AB  ff7510                  push     dword ptr [ebp + 0x10]         
  0x001F11AE  6a01                    push     1                              
  0x001F11B0  6a01                    push     1                              
  0x001F11B2  ff750c                  push     dword ptr [ebp + 0xc]          
  0x001F11B5  ff7508                  push     dword ptr [ebp + 8]            
  0x001F11B8  e8defdffff              call     0x1f0f9b                       ; -> sub_001F0F9B
  0x001F11BD  83660800                and      dword ptr [esi + 8], 0         
  0x001F11C1  8b4518                  mov      eax, dword ptr [ebp + 0x18]    
  0x001F11C4  83661400                and      dword ptr [esi + 0x14], 0      
  0x001F11C8  c70601000500            mov      dword ptr [esi], 0x50001       
  0x001F11CE  894604                  mov      dword ptr [esi + 4], eax       
  0x001F11D1  5e                      pop      esi                            
  0x001F11D2  5d                      pop      ebp                            
  0x001F11D3  c21800                  ret      0x18                           
; end of function

; ============================================================
; Function: sub_001F11D6
; Start: 0x001F11D6  End: 0x001F1200  Size: 42 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_001F1145
; Called by: sub_0006CF80, sub_000A45C0, sub_0017FB00
; ============================================================
sub_001F11D6:
  0x001F11D6  55                      push     ebp                            
  0x001F11D7  8bec                    mov      ebp, esp                       
  0x001F11D9  ff7520                  push     dword ptr [ebp + 0x20]         
  0x001F11DC  ff7524                  push     dword ptr [ebp + 0x24]         
  0x001F11DF  6a00                    push     0                              
  0x001F11E1  6a00                    push     0                              
  0x001F11E3  ff7528                  push     dword ptr [ebp + 0x28]         
  0x001F11E6  ff7518                  push     dword ptr [ebp + 0x18]         
  0x001F11E9  ff7514                  push     dword ptr [ebp + 0x14]         
  0x001F11EC  ff7510                  push     dword ptr [ebp + 0x10]         
  0x001F11EF  6a01                    push     1                              
  0x001F11F1  ff750c                  push     dword ptr [ebp + 0xc]          
  0x001F11F4  ff7508                  push     dword ptr [ebp + 8]            
  0x001F11F7  e849ffffff              call     0x1f1145                       ; -> sub_001F1145
  0x001F11FC  5d                      pop      ebp                            
  0x001F11FD  c22400                  ret      0x24                           
; end of function

; ============================================================
; Function: sub_001F1200
; Start: 0x001F1200  End: 0x001F1218  Size: 24 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00087100
; ============================================================
sub_001F1200:
  0x001F1200  8b442414                mov      eax, dword ptr [esp + 0x14]    
  0x001F1204  8b4c2418                mov      ecx, dword ptr [esp + 0x18]    
  0x001F1208  83600800                and      dword ptr [eax + 8], 0         
  0x001F120C  c70001000100            mov      dword ptr [eax], 0x10001       
  0x001F1212  894804                  mov      dword ptr [eax + 4], ecx       
  0x001F1215  c21800                  ret      0x18                           
; end of function
  0x001F1218  0909                    or       dword ptr [ecx], ecx           
  0x001F121A  11911191a1a1            adc      dword ptr [ecx - 0x5e5e6eef], edx 
  0x001F1220  0000                    add      byte ptr [eax], al             
  0x001F1222  0009                    add      byte ptr [ecx], cl             
  0x001F1224  0400                    add      al, 0                          
  0x001F1226  0808                    or       byte ptr [eax], cl             
  0x001F1228  1292a20a0000            adc      dl, byte ptr [edx + 0xaa2]     
  0x001F122E  1212                    adc      dl, byte ptr [edx]             
  0x001F1230  0009                    add      byte ptr [ecx], cl             
  0x001F1232  110a                    adc      dword ptr [edx], ecx           
  0x001F1234  92                      xchg     edx, eax                       
  0x001F1235  12a20a120000            adc      ah, byte ptr [edx + 0x120a]    
  0x001F123B  0020                    add      byte ptr [eax], ah             
  0x001F123D  2000                    and      byte ptr [eax], al             
  0x001F123F  1111                    adc      dword ptr [ecx], edx           
  0x001F1241  116161                  adc      dword ptr [ecx + 0x61], esp    
  0x001F1244  51                      push     ecx                            
  0x001F1245  51                      push     ecx                            
  0x001F1246  626252                  bound    esp, qword ptr [edx + 0x52]    
  0x001F1249  52                      push     edx                            
  0x001F124A  1121                    adc      dword ptr [ecx], esp           
  0x001F124C  0012                    add      byte ptr [edx], dl             
  0x001F124E  0012                    add      byte ptr [edx], dl             
  0x001F1250  1111                    adc      dword ptr [ecx], edx           
  0x001F1252  2121                    and      dword ptr [ecx], esp           
  0x001F1254  2112                    and      dword ptr [edx], edx           
  0x001F1256  1222                    adc      ah, byte ptr [edx]             
  0x001F1258  2222                    and      ah, byte ptr [edx]             
  0x001F125A  0000                    add      byte ptr [eax], al             
