; ============================================================
; Section: D3DX
; VA: 0x001EE0A0 - 0x001F0F48
; Size: 11944 bytes (11.7 KB)
; Functions: 14
; Instructions: 4727
; ============================================================


; ============================================================
; Function: sub_001EE0A0
; Start: 0x001EE0A0  End: 0x001EE0AE  Size: 14 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_0006F310
; ============================================================
sub_001EE0A0:
  0x001EE0A0  837c240400              cmp      dword ptr [esp + 4], 0         
  0x001EE0A5  7507                    jne      0x1ee0ae                       
  0x001EE0A7  b86c087688              mov      eax, 0x8876086c                
  0x001EE0AC  eb60                    jmp      0x1ee10e                       
; end of function
                                        ; XREF: 0x001EE0A5 (cond_jump)
  0x001EE0AE  53                      push     ebx                            
  0x001EE0AF  8b5c240c                mov      ebx, dword ptr [esp + 0xc]     
  0x001EE0B3  85db                    test     ebx, ebx                       
  0x001EE0B5  56                      push     esi                            
  0x001EE0B6  57                      push     edi                            
  0x001EE0B7  7507                    jne      0x1ee0c0                       
  0x001EE0B9  b86c087688              mov      eax, 0x8876086c                
  0x001EE0BE  eb4b                    jmp      0x1ee10b                       
                                        ; XREF: 0x001EE0B7 (cond_jump)
  0x001EE0C0  6a18                    push     0x18                           
  0x001EE0C2  e8f998fcff              call     0x1b79c0                       ; -> sub_001B79C0
  0x001EE0C7  85c0                    test     eax, eax                       
  0x001EE0C9  59                      pop      ecx                            
  0x001EE0CA  740b                    je       0x1ee0d7                       
  0x001EE0CC  8bc8                    mov      ecx, eax                       
  0x001EE0CE  e8340a0000              call     0x1eeb07                       ; -> sub_001EEB07
  0x001EE0D3  8bf0                    mov      esi, eax                       
  0x001EE0D5  eb02                    jmp      0x1ee0d9                       
                                        ; XREF: 0x001EE0CA (cond_jump)
  0x001EE0D7  33f6                    xor      esi, esi                       
                                        ; XREF: 0x001EE0D5 (jump)
  0x001EE0D9  85f6                    test     esi, esi                       
  0x001EE0DB  7507                    jne      0x1ee0e4                       
  0x001EE0DD  b80e000780              mov      eax, 0x8007000e                
  0x001EE0E2  eb27                    jmp      0x1ee10b                       
                                        ; XREF: 0x001EE0DB (cond_jump)
  0x001EE0E4  ff742410                push     dword ptr [esp + 0x10]         
  0x001EE0E8  8bce                    mov      ecx, esi                       
  0x001EE0EA  e856060000              call     0x1ee745                       ; -> sub_001EE745
  0x001EE0EF  8bf8                    mov      edi, eax                       
  0x001EE0F1  85ff                    test     edi, edi                       
  0x001EE0F3  7d12                    jge      0x1ee107                       
  0x001EE0F5  8bce                    mov      ecx, esi                       
  0x001EE0F7  e8d4050000              call     0x1ee6d0                       ; -> sub_001EE6D0
  0x001EE0FC  56                      push     esi                            
  0x001EE0FD  e8f596fcff              call     0x1b77f7                       ; -> sub_001B77F7
  0x001EE102  59                      pop      ecx                            
  0x001EE103  8bc7                    mov      eax, edi                       
  0x001EE105  eb04                    jmp      0x1ee10b                       
                                        ; XREF: 0x001EE0F3 (cond_jump)
  0x001EE107  8933                    mov      dword ptr [ebx], esi           
  0x001EE109  33c0                    xor      eax, eax                       
                                        ; XREF: 0x001EE0BE (jump), 0x001EE0E2 (jump), 0x001EE105 (jump)
  0x001EE10B  5f                      pop      edi                            
  0x001EE10C  5e                      pop      esi                            
  0x001EE10D  5b                      pop      ebx                            
                                        ; XREF: 0x001EE0AC (jump)
  0x001EE10E  c20800                  ret      8                              

; ============================================================
; Function: sub_001EE111
; Start: 0x001EE111  End: 0x001EE1AA  Size: 153 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_001EE111:
  0x001EE111  8b442408                mov      eax, dword ptr [esp + 8]       
  0x001EE115  8b54240c                mov      edx, dword ptr [esp + 0xc]     
  0x001EE119  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x001EE11D  d900                    fld      dword ptr [eax]                
  0x001EE11F  d80a                    fmul     dword ptr [edx]                
  0x001EE121  d900                    fld      dword ptr [eax]                
  0x001EE123  d84a08                  fmul     dword ptr [edx + 8]            
  0x001EE126  d900                    fld      dword ptr [eax]                
  0x001EE128  d84a04                  fmul     dword ptr [edx + 4]            
  0x001EE12B  d900                    fld      dword ptr [eax]                
  0x001EE12D  d84a0c                  fmul     dword ptr [edx + 0xc]          
  0x001EE130  d9cb                    fxch     st(3)                          
  0x001EE132  d94004                  fld      dword ptr [eax + 4]            
  0x001EE135  d84a10                  fmul     dword ptr [edx + 0x10]         
  0x001EE138  d94004                  fld      dword ptr [eax + 4]            
  0x001EE13B  d84a18                  fmul     dword ptr [edx + 0x18]         
  0x001EE13E  d94004                  fld      dword ptr [eax + 4]            
  0x001EE141  d84a14                  fmul     dword ptr [edx + 0x14]         
  0x001EE144  d94004                  fld      dword ptr [eax + 4]            
  0x001EE147  d84a1c                  fmul     dword ptr [edx + 0x1c]         
  0x001EE14A  d9cb                    fxch     st(3)                          
  0x001EE14C  dec4                    faddp    st(4)                          
  0x001EE14E  dec4                    faddp    st(4)                          
  0x001EE150  dec4                    faddp    st(4)                          
  0x001EE152  dec4                    faddp    st(4)                          
  0x001EE154  d94008                  fld      dword ptr [eax + 8]            
  0x001EE157  d84a20                  fmul     dword ptr [edx + 0x20]         
  0x001EE15A  d94008                  fld      dword ptr [eax + 8]            
  0x001EE15D  d84a28                  fmul     dword ptr [edx + 0x28]         
  0x001EE160  d94008                  fld      dword ptr [eax + 8]            
  0x001EE163  d84a24                  fmul     dword ptr [edx + 0x24]         
  0x001EE166  d94008                  fld      dword ptr [eax + 8]            
  0x001EE169  d84a2c                  fmul     dword ptr [edx + 0x2c]         
  0x001EE16C  d9cb                    fxch     st(3)                          
  0x001EE16E  dec4                    faddp    st(4)                          
  0x001EE170  dec4                    faddp    st(4)                          
  0x001EE172  dec4                    faddp    st(4)                          
  0x001EE174  dec4                    faddp    st(4)                          
  0x001EE176  d9400c                  fld      dword ptr [eax + 0xc]          
  0x001EE179  d84a30                  fmul     dword ptr [edx + 0x30]         
  0x001EE17C  d9400c                  fld      dword ptr [eax + 0xc]          
  0x001EE17F  d84a38                  fmul     dword ptr [edx + 0x38]         
  0x001EE182  d9400c                  fld      dword ptr [eax + 0xc]          
  0x001EE185  d84a34                  fmul     dword ptr [edx + 0x34]         
  0x001EE188  d9400c                  fld      dword ptr [eax + 0xc]          
  0x001EE18B  d84a3c                  fmul     dword ptr [edx + 0x3c]         
  0x001EE18E  d9cb                    fxch     st(3)                          
  0x001EE190  dec4                    faddp    st(4)                          
  0x001EE192  dec4                    faddp    st(4)                          
  0x001EE194  dec4                    faddp    st(4)                          
  0x001EE196  dec4                    faddp    st(4)                          
  0x001EE198  d919                    fstp     dword ptr [ecx]                
  0x001EE19A  d95904                  fstp     dword ptr [ecx + 4]            
  0x001EE19D  d95908                  fstp     dword ptr [ecx + 8]            
  0x001EE1A0  d9590c                  fstp     dword ptr [ecx + 0xc]          
  0x001EE1A3  8b442404                mov      eax, dword ptr [esp + 4]       
  0x001EE1A7  c20c00                  ret      0xc                            
; end of function

; ============================================================
; Function: sub_001EE1AA
; Start: 0x001EE1AA  End: 0x001EE217  Size: 109 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_000E31E0, sub_000E3F30, sub_001EE4C4
; ============================================================
sub_001EE1AA:
  0x001EE1AA  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x001EE1AE  d94104                  fld      dword ptr [ecx + 4]            
  0x001EE1B1  8b442404                mov      eax, dword ptr [esp + 4]       
  0x001EE1B5  3bc1                    cmp      eax, ecx                       
  0x001EE1B7  d94110                  fld      dword ptr [ecx + 0x10]         
  0x001EE1BA  d95804                  fstp     dword ptr [eax + 4]            
  0x001EE1BD  d95810                  fstp     dword ptr [eax + 0x10]         
  0x001EE1C0  8b5120                  mov      edx, dword ptr [ecx + 0x20]    
  0x001EE1C3  d94108                  fld      dword ptr [ecx + 8]            
  0x001EE1C6  895008                  mov      dword ptr [eax + 8], edx       
  0x001EE1C9  d95820                  fstp     dword ptr [eax + 0x20]         
  0x001EE1CC  8b5130                  mov      edx, dword ptr [ecx + 0x30]    
  0x001EE1CF  d9410c                  fld      dword ptr [ecx + 0xc]          
  0x001EE1D2  89500c                  mov      dword ptr [eax + 0xc], edx     
  0x001EE1D5  d95830                  fstp     dword ptr [eax + 0x30]         
  0x001EE1D8  8b5124                  mov      edx, dword ptr [ecx + 0x24]    
  0x001EE1DB  d94118                  fld      dword ptr [ecx + 0x18]         
  0x001EE1DE  895018                  mov      dword ptr [eax + 0x18], edx    
  0x001EE1E1  d95824                  fstp     dword ptr [eax + 0x24]         
  0x001EE1E4  8b5134                  mov      edx, dword ptr [ecx + 0x34]    
  0x001EE1E7  d9411c                  fld      dword ptr [ecx + 0x1c]         
  0x001EE1EA  89501c                  mov      dword ptr [eax + 0x1c], edx    
  0x001EE1ED  d95834                  fstp     dword ptr [eax + 0x34]         
  0x001EE1F0  8b5138                  mov      edx, dword ptr [ecx + 0x38]    
  0x001EE1F3  d9412c                  fld      dword ptr [ecx + 0x2c]         
  0x001EE1F6  89502c                  mov      dword ptr [eax + 0x2c], edx    
  0x001EE1F9  d95838                  fstp     dword ptr [eax + 0x38]         
  0x001EE1FC  7416                    je       0x1ee214                       
  0x001EE1FE  8b11                    mov      edx, dword ptr [ecx]           
  0x001EE200  8910                    mov      dword ptr [eax], edx           
  0x001EE202  8b5114                  mov      edx, dword ptr [ecx + 0x14]    
  0x001EE205  895014                  mov      dword ptr [eax + 0x14], edx    
  0x001EE208  8b5128                  mov      edx, dword ptr [ecx + 0x28]    
  0x001EE20B  895028                  mov      dword ptr [eax + 0x28], edx    
  0x001EE20E  8b493c                  mov      ecx, dword ptr [ecx + 0x3c]    
  0x001EE211  89483c                  mov      dword ptr [eax + 0x3c], ecx    
                                        ; XREF: 0x001EE1FC (cond_jump)
  0x001EE214  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_001EE217
; Start: 0x001EE217  End: 0x001EE2F9  Size: 226 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_001EE4C4
; ============================================================
sub_001EE217:
  0x001EE217  55                      push     ebp                            
  0x001EE218  8bec                    mov      ebp, esp                       
  0x001EE21A  83ec10                  sub      esp, 0x10                      
  0x001EE21D  8b450c                  mov      eax, dword ptr [ebp + 0xc]     
  0x001EE220  d905cc0e2500            fld      dword ptr [0x250ecc]           
  0x001EE226  d900                    fld      dword ptr [eax]                
  0x001EE228  d8c9                    fmul     st(1)                          
  0x001EE22A  d95dfc                  fstp     dword ptr [ebp - 4]            
  0x001EE22D  d94004                  fld      dword ptr [eax + 4]            
  0x001EE230  d8c9                    fmul     st(1)                          
  0x001EE232  d95d0c                  fstp     dword ptr [ebp + 0xc]          
  0x001EE235  d84808                  fmul     dword ptr [eax + 8]            
  0x001EE238  d945fc                  fld      dword ptr [ebp - 4]            
  0x001EE23B  d8480c                  fmul     dword ptr [eax + 0xc]          
  0x001EE23E  d9450c                  fld      dword ptr [ebp + 0xc]          
  0x001EE241  d8480c                  fmul     dword ptr [eax + 0xc]          
  0x001EE244  d9c2                    fld      st(2)                          
  0x001EE246  d8480c                  fmul     dword ptr [eax + 0xc]          
  0x001EE249  d945fc                  fld      dword ptr [ebp - 4]            
  0x001EE24C  d808                    fmul     dword ptr [eax]                
  0x001EE24E  d95df0                  fstp     dword ptr [ebp - 0x10]         
  0x001EE251  d9450c                  fld      dword ptr [ebp + 0xc]          
  0x001EE254  d808                    fmul     dword ptr [eax]                
  0x001EE256  d95dfc                  fstp     dword ptr [ebp - 4]            
  0x001EE259  d9c3                    fld      st(3)                          
  0x001EE25B  d808                    fmul     dword ptr [eax]                
  0x001EE25D  d95df8                  fstp     dword ptr [ebp - 8]            
  0x001EE260  d9450c                  fld      dword ptr [ebp + 0xc]          
  0x001EE263  d84804                  fmul     dword ptr [eax + 4]            
  0x001EE266  d95df4                  fstp     dword ptr [ebp - 0xc]          
  0x001EE269  d9c3                    fld      st(3)                          
  0x001EE26B  d84804                  fmul     dword ptr [eax + 4]            
  0x001EE26E  d95d0c                  fstp     dword ptr [ebp + 0xc]          
  0x001EE271  d9c3                    fld      st(3)                          
  0x001EE273  d84808                  fmul     dword ptr [eax + 8]            
  0x001EE276  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x001EE279  d9e8                    fld1                                    
  0x001EE27B  d865f4                  fsub     dword ptr [ebp - 0xc]          
  0x001EE27E  d8e1                    fsub     st(1)                          
  0x001EE280  d918                    fstp     dword ptr [eax]                
  0x001EE282  d945fc                  fld      dword ptr [ebp - 4]            
  0x001EE285  d8c2                    fadd     st(2)                          
  0x001EE287  d95804                  fstp     dword ptr [eax + 4]            
  0x001EE28A  d945f8                  fld      dword ptr [ebp - 8]            
  0x001EE28D  d8e3                    fsub     st(3)                          
  0x001EE28F  d95808                  fstp     dword ptr [eax + 8]            
  0x001EE292  d9ee                    fldz                                    
  0x001EE294  d9580c                  fstp     dword ptr [eax + 0xc]          
  0x001EE297  d945fc                  fld      dword ptr [ebp - 4]            
  0x001EE29A  d8e2                    fsub     st(2)                          
  0x001EE29C  d95810                  fstp     dword ptr [eax + 0x10]         
  0x001EE29F  d9e8                    fld1                                    
  0x001EE2A1  d865f0                  fsub     dword ptr [ebp - 0x10]         
  0x001EE2A4  d95508                  fst      dword ptr [ebp + 8]            
  0x001EE2A7  d8e1                    fsub     st(1)                          
  0x001EE2A9  d95814                  fstp     dword ptr [eax + 0x14]         
  0x001EE2AC  ddd8                    fstp     st(0)                          
  0x001EE2AE  ddd8                    fstp     st(0)                          
  0x001EE2B0  d9450c                  fld      dword ptr [ebp + 0xc]          
  0x001EE2B3  d8c2                    fadd     st(2)                          
  0x001EE2B5  d95818                  fstp     dword ptr [eax + 0x18]         
  0x001EE2B8  d9ee                    fldz                                    
  0x001EE2BA  d9581c                  fstp     dword ptr [eax + 0x1c]         
  0x001EE2BD  d945f8                  fld      dword ptr [ebp - 8]            
  0x001EE2C0  d8c1                    fadd     st(1)                          
  0x001EE2C2  d95820                  fstp     dword ptr [eax + 0x20]         
  0x001EE2C5  ddd8                    fstp     st(0)                          
  0x001EE2C7  d9450c                  fld      dword ptr [ebp + 0xc]          
  0x001EE2CA  d8e1                    fsub     st(1)                          
  0x001EE2CC  d95824                  fstp     dword ptr [eax + 0x24]         
  0x001EE2CF  ddd8                    fstp     st(0)                          
  0x001EE2D1  ddd8                    fstp     st(0)                          
  0x001EE2D3  d94508                  fld      dword ptr [ebp + 8]            
  0x001EE2D6  d865f4                  fsub     dword ptr [ebp - 0xc]          
  0x001EE2D9  d95828                  fstp     dword ptr [eax + 0x28]         
  0x001EE2DC  d9ee                    fldz                                    
  0x001EE2DE  d9582c                  fstp     dword ptr [eax + 0x2c]         
  0x001EE2E1  d9ee                    fldz                                    
  0x001EE2E3  d95830                  fstp     dword ptr [eax + 0x30]         
  0x001EE2E6  d9ee                    fldz                                    
  0x001EE2E8  d95834                  fstp     dword ptr [eax + 0x34]         
  0x001EE2EB  d9ee                    fldz                                    
  0x001EE2ED  d95838                  fstp     dword ptr [eax + 0x38]         
  0x001EE2F0  d9e8                    fld1                                    
  0x001EE2F2  d9583c                  fstp     dword ptr [eax + 0x3c]         
  0x001EE2F5  c9                      leave                                   
  0x001EE2F6  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_001EE2F9
; Start: 0x001EE2F9  End: 0x001EE38D  Size: 148 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_000CEFD0
; ============================================================
sub_001EE2F9:
  0x001EE2F9  55                      push     ebp                            
  0x001EE2FA  8bec                    mov      ebp, esp                       
  0x001EE2FC  83ec10                  sub      esp, 0x10                      
  0x001EE2FF  d9450c                  fld      dword ptr [ebp + 0xc]          
  0x001EE302  8d450c                  lea      eax, [ebp + 0xc]               
  0x001EE305  d80dd00e2500            fmul     dword ptr [0x250ed0]           
  0x001EE30B  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x001EE30E  8d45fc                  lea      eax, [ebp - 4]                 
  0x001EE311  8945f0                  mov      dword ptr [ebp - 0x10], eax    
  0x001EE314  d95df8                  fstp     dword ptr [ebp - 8]            
  0x001EE317  8b45f0                  mov      eax, dword ptr [ebp - 0x10]    
  0x001EE31A  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
  0x001EE31D  d945f8                  fld      dword ptr [ebp - 8]            
  0x001EE320  d9fb                    fsincos                                 
  0x001EE322  d91a                    fstp     dword ptr [edx]                
  0x001EE324  d918                    fstp     dword ptr [eax]                
  0x001EE326  d9450c                  fld      dword ptr [ebp + 0xc]          
  0x001EE329  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x001EE32C  d875fc                  fdiv     dword ptr [ebp - 4]            
  0x001EE32F  d94510                  fld      dword ptr [ebp + 0x10]         
  0x001EE332  d8f9                    fdivr    st(1)                          
  0x001EE334  d918                    fstp     dword ptr [eax]                
  0x001EE336  d9ee                    fldz                                    
  0x001EE338  d95804                  fstp     dword ptr [eax + 4]            
  0x001EE33B  d9ee                    fldz                                    
  0x001EE33D  d95808                  fstp     dword ptr [eax + 8]            
  0x001EE340  d9ee                    fldz                                    
  0x001EE342  d9580c                  fstp     dword ptr [eax + 0xc]          
  0x001EE345  d9ee                    fldz                                    
  0x001EE347  d95810                  fstp     dword ptr [eax + 0x10]         
  0x001EE34A  d95814                  fstp     dword ptr [eax + 0x14]         
  0x001EE34D  d9ee                    fldz                                    
  0x001EE34F  d95818                  fstp     dword ptr [eax + 0x18]         
  0x001EE352  d9ee                    fldz                                    
  0x001EE354  d9581c                  fstp     dword ptr [eax + 0x1c]         
  0x001EE357  d9ee                    fldz                                    
  0x001EE359  d95820                  fstp     dword ptr [eax + 0x20]         
  0x001EE35C  d9ee                    fldz                                    
  0x001EE35E  d95824                  fstp     dword ptr [eax + 0x24]         
  0x001EE361  d94518                  fld      dword ptr [ebp + 0x18]         
  0x001EE364  d86514                  fsub     dword ptr [ebp + 0x14]         
  0x001EE367  d87d18                  fdivr    dword ptr [ebp + 0x18]         
  0x001EE36A  d95028                  fst      dword ptr [eax + 0x28]         
  0x001EE36D  d9e8                    fld1                                    
  0x001EE36F  d9582c                  fstp     dword ptr [eax + 0x2c]         
  0x001EE372  d9ee                    fldz                                    
  0x001EE374  d95830                  fstp     dword ptr [eax + 0x30]         
  0x001EE377  d9ee                    fldz                                    
  0x001EE379  d95834                  fstp     dword ptr [eax + 0x34]         
  0x001EE37C  d84d14                  fmul     dword ptr [ebp + 0x14]         
  0x001EE37F  d9e0                    fchs                                    
  0x001EE381  d95838                  fstp     dword ptr [eax + 0x38]         
  0x001EE384  d9ee                    fldz                                    
  0x001EE386  d9583c                  fstp     dword ptr [eax + 0x3c]         
  0x001EE389  c9                      leave                                   
  0x001EE38A  c21400                  ret      0x14                           
; end of function

; ============================================================
; Function: sub_001EE38D
; Start: 0x001EE38D  End: 0x001EE3E1  Size: 84 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_001EE4C4
; ============================================================
sub_001EE38D:
  0x001EE38D  8b442404                mov      eax, dword ptr [esp + 4]       
  0x001EE391  d9ee                    fldz                                    
  0x001EE393  d95838                  fstp     dword ptr [eax + 0x38]         
  0x001EE396  d9ee                    fldz                                    
  0x001EE398  d95834                  fstp     dword ptr [eax + 0x34]         
  0x001EE39B  d9ee                    fldz                                    
  0x001EE39D  d95830                  fstp     dword ptr [eax + 0x30]         
  0x001EE3A0  d9ee                    fldz                                    
  0x001EE3A2  d9582c                  fstp     dword ptr [eax + 0x2c]         
  0x001EE3A5  d9ee                    fldz                                    
  0x001EE3A7  d95824                  fstp     dword ptr [eax + 0x24]         
  0x001EE3AA  d9ee                    fldz                                    
  0x001EE3AC  d95820                  fstp     dword ptr [eax + 0x20]         
  0x001EE3AF  d9ee                    fldz                                    
  0x001EE3B1  d9581c                  fstp     dword ptr [eax + 0x1c]         
  0x001EE3B4  d9ee                    fldz                                    
  0x001EE3B6  d95818                  fstp     dword ptr [eax + 0x18]         
  0x001EE3B9  d9ee                    fldz                                    
  0x001EE3BB  d95810                  fstp     dword ptr [eax + 0x10]         
  0x001EE3BE  d9ee                    fldz                                    
  0x001EE3C0  d9580c                  fstp     dword ptr [eax + 0xc]          
  0x001EE3C3  d9ee                    fldz                                    
  0x001EE3C5  d95808                  fstp     dword ptr [eax + 8]            
  0x001EE3C8  d9ee                    fldz                                    
  0x001EE3CA  d95804                  fstp     dword ptr [eax + 4]            
  0x001EE3CD  d9e8                    fld1                                    
  0x001EE3CF  d9583c                  fstp     dword ptr [eax + 0x3c]         
  0x001EE3D2  d9e8                    fld1                                    
  0x001EE3D4  d95828                  fstp     dword ptr [eax + 0x28]         
  0x001EE3D7  d9e8                    fld1                                    
  0x001EE3D9  d95814                  fstp     dword ptr [eax + 0x14]         
  0x001EE3DC  d9e8                    fld1                                    
  0x001EE3DE  d918                    fstp     dword ptr [eax]                
  0x001EE3E0  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_001EE3E1
; Start: 0x001EE3E1  End: 0x001EE4C4  Size: 227 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_000E31E0, sub_000E3F30, sub_001EE4C4
; ============================================================
sub_001EE3E1:
  0x001EE3E1  55                      push     ebp                            
  0x001EE3E2  8bec                    mov      ebp, esp                       
  0x001EE3E4  83ec40                  sub      esp, 0x40                      
  0x001EE3E7  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x001EE3EA  53                      push     ebx                            
  0x001EE3EB  56                      push     esi                            
  0x001EE3EC  8b7510                  mov      esi, dword ptr [ebp + 0x10]    
  0x001EE3EF  3bf0                    cmp      esi, eax                       
  0x001EE3F1  57                      push     edi                            
  0x001EE3F2  7571                    jne      0x1ee465                       
  0x001EE3F4  39450c                  cmp      dword ptr [ebp + 0xc], eax     
  0x001EE3F7  745e                    je       0x1ee457                       
  0x001EE3F9  8b5d08                  mov      ebx, dword ptr [ebp + 8]       
  0x001EE3FC  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x001EE3FF  8b5510                  mov      edx, dword ptr [ebp + 0x10]    
  0x001EE402  bffcffffff              mov      edi, 0xfffffffc                
                                        ; XREF: 0x001EE453 (cond_jump)
  0x001EE407  bef0ffffff              mov      esi, 0xfffffff0                
  0x001EE40C  d944ba10                fld      dword ptr [edx + edi*4 + 0x10] 
  0x001EE410  d944ba20                fld      dword ptr [edx + edi*4 + 0x20] 
  0x001EE414  d944ba30                fld      dword ptr [edx + edi*4 + 0x30] 
  0x001EE418  d944ba40                fld      dword ptr [edx + edi*4 + 0x40] 
                                        ; XREF: 0x001EE445 (cond_jump)
  0x001EE41C  d9c3                    fld      st(3)                          
  0x001EE41E  d84cb140                fmul     dword ptr [ecx + esi*4 + 0x40] 
  0x001EE422  d9c3                    fld      st(3)                          
  0x001EE424  d84cb144                fmul     dword ptr [ecx + esi*4 + 0x44] 
  0x001EE428  d9c3                    fld      st(3)                          
  0x001EE42A  d84cb148                fmul     dword ptr [ecx + esi*4 + 0x48] 
  0x001EE42E  d9c3                    fld      st(3)                          
  0x001EE430  d84cb14c                fmul     dword ptr [ecx + esi*4 + 0x4c] 
  0x001EE434  d9cb                    fxch     st(3)                          
  0x001EE436  dec1                    faddp    st(1)                          
  0x001EE438  d9ca                    fxch     st(2)                          
  0x001EE43A  dec1                    faddp    st(1)                          
  0x001EE43C  dec1                    faddp    st(1)                          
  0x001EE43E  d95cb340                fstp     dword ptr [ebx + esi*4 + 0x40] 
  0x001EE442  83c604                  add      esi, 4                         
  0x001EE445  75d5                    jne      0x1ee41c                       
  0x001EE447  ddc3                    ffree    st(3)                          
  0x001EE449  ddc2                    ffree    st(2)                          
  0x001EE44B  ddc1                    ffree    st(1)                          
  0x001EE44D  ddc0                    ffree    st(0)                          
  0x001EE44F  8d5b04                  lea      ebx, [ebx + 4]                 
  0x001EE452  47                      inc      edi                            
  0x001EE453  75b2                    jne      0x1ee407                       
  0x001EE455  eb66                    jmp      0x1ee4bd                       
                                        ; XREF: 0x001EE3F7 (cond_jump)
  0x001EE457  6a10                    push     0x10                           
  0x001EE459  59                      pop      ecx                            
  0x001EE45A  8d7dc0                  lea      edi, [ebp - 0x40]              
  0x001EE45D  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x001EE45F  8d4dc0                  lea      ecx, [ebp - 0x40]              
  0x001EE462  894d10                  mov      dword ptr [ebp + 0x10], ecx    
                                        ; XREF: 0x001EE3F2 (cond_jump)
  0x001EE465  8b5d08                  mov      ebx, dword ptr [ebp + 8]       
  0x001EE468  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x001EE46B  8b5510                  mov      edx, dword ptr [ebp + 0x10]    
  0x001EE46E  bffcffffff              mov      edi, 0xfffffffc                
                                        ; XREF: 0x001EE4BB (cond_jump)
  0x001EE473  befcffffff              mov      esi, 0xfffffffc                
  0x001EE478  d901                    fld      dword ptr [ecx]                
  0x001EE47A  d94104                  fld      dword ptr [ecx + 4]            
  0x001EE47D  d94108                  fld      dword ptr [ecx + 8]            
  0x001EE480  d9410c                  fld      dword ptr [ecx + 0xc]          
                                        ; XREF: 0x001EE4AA (cond_jump)
  0x001EE483  d9c3                    fld      st(3)                          
  0x001EE485  d84cb210                fmul     dword ptr [edx + esi*4 + 0x10] 
  0x001EE489  d9c3                    fld      st(3)                          
  0x001EE48B  d84cb220                fmul     dword ptr [edx + esi*4 + 0x20] 
  0x001EE48F  d9c3                    fld      st(3)                          
  0x001EE491  d84cb230                fmul     dword ptr [edx + esi*4 + 0x30] 
  0x001EE495  d9c3                    fld      st(3)                          
  0x001EE497  d84cb240                fmul     dword ptr [edx + esi*4 + 0x40] 
  0x001EE49B  d9cb                    fxch     st(3)                          
  0x001EE49D  dec1                    faddp    st(1)                          
  0x001EE49F  d9ca                    fxch     st(2)                          
  0x001EE4A1  dec1                    faddp    st(1)                          
  0x001EE4A3  dec1                    faddp    st(1)                          
  0x001EE4A5  d95cb310                fstp     dword ptr [ebx + esi*4 + 0x10] 
  0x001EE4A9  46                      inc      esi                            
  0x001EE4AA  75d7                    jne      0x1ee483                       
  0x001EE4AC  ddc3                    ffree    st(3)                          
  0x001EE4AE  ddc2                    ffree    st(2)                          
  0x001EE4B0  ddc1                    ffree    st(1)                          
  0x001EE4B2  ddc0                    ffree    st(0)                          
  0x001EE4B4  8d4910                  lea      ecx, [ecx + 0x10]              
  0x001EE4B7  8d5b10                  lea      ebx, [ebx + 0x10]              
  0x001EE4BA  47                      inc      edi                            
  0x001EE4BB  75b6                    jne      0x1ee473                       
                                        ; XREF: 0x001EE455 (jump)
  0x001EE4BD  5f                      pop      edi                            
  0x001EE4BE  5e                      pop      esi                            
  0x001EE4BF  5b                      pop      ebx                            
  0x001EE4C0  c9                      leave                                   
  0x001EE4C1  c20c00                  ret      0xc                            
; end of function

; ============================================================
; Function: sub_001EE4C4
; Start: 0x001EE4C4  End: 0x001EE62E  Size: 362 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_001EE1AA, sub_001EE217, sub_001EE38D, sub_001EE3E1
; ============================================================
sub_001EE4C4:
  0x001EE4C4  55                      push     ebp                            
  0x001EE4C5  8d6c24a4                lea      ebp, [esp - 0x5c]              
  0x001EE4C9  81ecc0000000            sub      esp, 0xc0                      
  0x001EE4CF  8b4570                  mov      eax, dword ptr [ebp + 0x70]    
  0x001EE4D2  85c0                    test     eax, eax                       
  0x001EE4D4  56                      push     esi                            
  0x001EE4D5  57                      push     edi                            
  0x001EE4D6  0f8452010000            je       0x1ee62e                       
  0x001EE4DC  837d6c00                cmp      dword ptr [ebp + 0x6c], 0      
  0x001EE4E0  d9ee                    fldz                                    
  0x001EE4E2  0f84f2000000            je       0x1ee5da                       
  0x001EE4E8  d95d54                  fstp     dword ptr [ebp + 0x54]         
  0x001EE4EB  8b08                    mov      ecx, dword ptr [eax]           
  0x001EE4ED  d9ee                    fldz                                    
  0x001EE4EF  ff756c                  push     dword ptr [ebp + 0x6c]         
  0x001EE4F2  d95d50                  fstp     dword ptr [ebp + 0x50]         
  0x001EE4F5  894d1c                  mov      dword ptr [ebp + 0x1c], ecx    
  0x001EE4F8  d9ee                    fldz                                    
  0x001EE4FA  8b4804                  mov      ecx, dword ptr [eax + 4]       
  0x001EE4FD  d95d4c                  fstp     dword ptr [ebp + 0x4c]         
  0x001EE500  8b4008                  mov      eax, dword ptr [eax + 8]       
  0x001EE503  d9ee                    fldz                                    
  0x001EE505  894544                  mov      dword ptr [ebp + 0x44], eax    
  0x001EE508  8d45dc                  lea      eax, [ebp - 0x24]              
  0x001EE50B  d95d48                  fstp     dword ptr [ebp + 0x48]         
  0x001EE50E  50                      push     eax                            
  0x001EE50F  d9ee                    fldz                                    
  0x001EE511  894d30                  mov      dword ptr [ebp + 0x30], ecx    
  0x001EE514  d95d40                  fstp     dword ptr [ebp + 0x40]         
  0x001EE517  d9ee                    fldz                                    
  0x001EE519  d95d3c                  fstp     dword ptr [ebp + 0x3c]         
  0x001EE51C  d9ee                    fldz                                    
  0x001EE51E  d95d38                  fstp     dword ptr [ebp + 0x38]         
  0x001EE521  d9ee                    fldz                                    
  0x001EE523  d95d34                  fstp     dword ptr [ebp + 0x34]         
  0x001EE526  d9ee                    fldz                                    
  0x001EE528  d95d2c                  fstp     dword ptr [ebp + 0x2c]         
  0x001EE52B  d9ee                    fldz                                    
  0x001EE52D  d95d28                  fstp     dword ptr [ebp + 0x28]         
  0x001EE530  d9ee                    fldz                                    
  0x001EE532  d95d24                  fstp     dword ptr [ebp + 0x24]         
  0x001EE535  d9ee                    fldz                                    
  0x001EE537  d95d20                  fstp     dword ptr [ebp + 0x20]         
  0x001EE53A  d9e8                    fld1                                    
  0x001EE53C  d95d58                  fstp     dword ptr [ebp + 0x58]         
  0x001EE53F  e8d3fcffff              call     0x1ee217                       ; -> sub_001EE217
  0x001EE544  8b7d68                  mov      edi, dword ptr [ebp + 0x68]    
  0x001EE547  85ff                    test     edi, edi                       
  0x001EE549  8d45dc                  lea      eax, [ebp - 0x24]              
  0x001EE54C  50                      push     eax                            
  0x001EE54D  746a                    je       0x1ee5b9                       
  0x001EE54F  8d459c                  lea      eax, [ebp - 0x64]              
  0x001EE552  50                      push     eax                            
  0x001EE553  e852fcffff              call     0x1ee1aa                       ; -> sub_001EE1AA
  0x001EE558  8b7564                  mov      esi, dword ptr [ebp + 0x64]    
  0x001EE55B  56                      push     esi                            
  0x001EE55C  e82cfeffff              call     0x1ee38d                       ; -> sub_001EE38D
  0x001EE561  d94630                  fld      dword ptr [esi + 0x30]         
  0x001EE564  d827                    fsub     dword ptr [edi]                
  0x001EE566  59                      pop      ecx                            
  0x001EE567  8d459c                  lea      eax, [ebp - 0x64]              
  0x001EE56A  50                      push     eax                            
  0x001EE56B  d95e30                  fstp     dword ptr [esi + 0x30]         
  0x001EE56E  56                      push     esi                            
  0x001EE56F  d94634                  fld      dword ptr [esi + 0x34]         
  0x001EE572  56                      push     esi                            
  0x001EE573  d86704                  fsub     dword ptr [edi + 4]            
  0x001EE576  d95e34                  fstp     dword ptr [esi + 0x34]         
  0x001EE579  d94638                  fld      dword ptr [esi + 0x38]         
  0x001EE57C  d86708                  fsub     dword ptr [edi + 8]            
  0x001EE57F  d95e38                  fstp     dword ptr [esi + 0x38]         
  0x001EE582  e85afeffff              call     0x1ee3e1                       ; -> sub_001EE3E1
  0x001EE587  8d451c                  lea      eax, [ebp + 0x1c]              
  0x001EE58A  50                      push     eax                            
  0x001EE58B  56                      push     esi                            
  0x001EE58C  56                      push     esi                            
  0x001EE58D  e84ffeffff              call     0x1ee3e1                       ; -> sub_001EE3E1
  0x001EE592  8d45dc                  lea      eax, [ebp - 0x24]              
  0x001EE595  50                      push     eax                            
  0x001EE596  56                      push     esi                            
  0x001EE597  56                      push     esi                            
  0x001EE598  e844feffff              call     0x1ee3e1                       ; -> sub_001EE3E1
  0x001EE59D  d94630                  fld      dword ptr [esi + 0x30]         
  0x001EE5A0  d807                    fadd     dword ptr [edi]                
  0x001EE5A2  d95e30                  fstp     dword ptr [esi + 0x30]         
  0x001EE5A5  d94704                  fld      dword ptr [edi + 4]            
  0x001EE5A8  d84634                  fadd     dword ptr [esi + 0x34]         
  0x001EE5AB  d95e34                  fstp     dword ptr [esi + 0x34]         
  0x001EE5AE  d94708                  fld      dword ptr [edi + 8]            
  0x001EE5B1  d84638                  fadd     dword ptr [esi + 0x38]         
  0x001EE5B4  d95e38                  fstp     dword ptr [esi + 0x38]         
  0x001EE5B7  eb7f                    jmp      0x1ee638                       
                                        ; XREF: 0x001EE54D (cond_jump)
  0x001EE5B9  8b7564                  mov      esi, dword ptr [ebp + 0x64]    
  0x001EE5BC  56                      push     esi                            
  0x001EE5BD  e8e8fbffff              call     0x1ee1aa                       ; -> sub_001EE1AA
  0x001EE5C2  8d451c                  lea      eax, [ebp + 0x1c]              
  0x001EE5C5  50                      push     eax                            
  0x001EE5C6  56                      push     esi                            
  0x001EE5C7  56                      push     esi                            
  0x001EE5C8  e814feffff              call     0x1ee3e1                       ; -> sub_001EE3E1
  0x001EE5CD  8d45dc                  lea      eax, [ebp - 0x24]              
  0x001EE5D0  50                      push     eax                            
  0x001EE5D1  56                      push     esi                            
  0x001EE5D2  56                      push     esi                            
  0x001EE5D3  e809feffff              call     0x1ee3e1                       ; -> sub_001EE3E1
  0x001EE5D8  eb5e                    jmp      0x1ee638                       
                                        ; XREF: 0x001EE4E2 (cond_jump)
  0x001EE5DA  8b7564                  mov      esi, dword ptr [ebp + 0x64]    
  0x001EE5DD  d95e38                  fstp     dword ptr [esi + 0x38]         
  0x001EE5E0  d9ee                    fldz                                    
  0x001EE5E2  d95e34                  fstp     dword ptr [esi + 0x34]         
  0x001EE5E5  d9ee                    fldz                                    
  0x001EE5E7  d95e30                  fstp     dword ptr [esi + 0x30]         
  0x001EE5EA  d9ee                    fldz                                    
  0x001EE5EC  d95e2c                  fstp     dword ptr [esi + 0x2c]         
  0x001EE5EF  d9ee                    fldz                                    
  0x001EE5F1  d95e24                  fstp     dword ptr [esi + 0x24]         
  0x001EE5F4  d9ee                    fldz                                    
  0x001EE5F6  d95e20                  fstp     dword ptr [esi + 0x20]         
  0x001EE5F9  d9ee                    fldz                                    
  0x001EE5FB  d95e1c                  fstp     dword ptr [esi + 0x1c]         
  0x001EE5FE  d9ee                    fldz                                    
  0x001EE600  d95e18                  fstp     dword ptr [esi + 0x18]         
  0x001EE603  d9ee                    fldz                                    
  0x001EE605  d95e10                  fstp     dword ptr [esi + 0x10]         
  0x001EE608  d9ee                    fldz                                    
  0x001EE60A  d95e0c                  fstp     dword ptr [esi + 0xc]          
  0x001EE60D  d9ee                    fldz                                    
  0x001EE60F  d95e08                  fstp     dword ptr [esi + 8]            
  0x001EE612  d9ee                    fldz                                    
  0x001EE614  d95e04                  fstp     dword ptr [esi + 4]            
  0x001EE617  d9e8                    fld1                                    
  0x001EE619  8b08                    mov      ecx, dword ptr [eax]           
  0x001EE61B  890e                    mov      dword ptr [esi], ecx           
  0x001EE61D  8b4804                  mov      ecx, dword ptr [eax + 4]       
  0x001EE620  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x001EE623  8b4008                  mov      eax, dword ptr [eax + 8]       
  0x001EE626  d95e3c                  fstp     dword ptr [esi + 0x3c]         
  0x001EE629  894628                  mov      dword ptr [esi + 0x28], eax    
  0x001EE62C  eb0a                    jmp      0x1ee638                       
; end of function
                                        ; XREF: 0x001EE4D6 (cond_jump)
  0x001EE62E  8b7564                  mov      esi, dword ptr [ebp + 0x64]    
  0x001EE631  56                      push     esi                            
  0x001EE632  e856fdffff              call     0x1ee38d                       ; -> sub_001EE38D
  0x001EE637  59                      pop      ecx                            
                                        ; XREF: 0x001EE5B7 (jump), 0x001EE5D8 (jump), 0x001EE62C (jump)
  0x001EE638  837d7800                cmp      dword ptr [ebp + 0x78], 0      
  0x001EE63C  7459                    je       0x1ee697                       
  0x001EE63E  ff7578                  push     dword ptr [ebp + 0x78]         
  0x001EE641  8d45dc                  lea      eax, [ebp - 0x24]              
  0x001EE644  50                      push     eax                            
  0x001EE645  e8cdfbffff              call     0x1ee217                       ; -> sub_001EE217
  0x001EE64A  8b7d74                  mov      edi, dword ptr [ebp + 0x74]    
  0x001EE64D  85ff                    test     edi, edi                       
  0x001EE64F  8d45dc                  lea      eax, [ebp - 0x24]              
  0x001EE652  50                      push     eax                            
  0x001EE653  56                      push     esi                            
  0x001EE654  56                      push     esi                            
  0x001EE655  743b                    je       0x1ee692                       
  0x001EE657  d94630                  fld      dword ptr [esi + 0x30]         
  0x001EE65A  d827                    fsub     dword ptr [edi]                
  0x001EE65C  d95e30                  fstp     dword ptr [esi + 0x30]         
  0x001EE65F  d94634                  fld      dword ptr [esi + 0x34]         
  0x001EE662  d86704                  fsub     dword ptr [edi + 4]            
  0x001EE665  d95e34                  fstp     dword ptr [esi + 0x34]         
  0x001EE668  d94638                  fld      dword ptr [esi + 0x38]         
  0x001EE66B  d86708                  fsub     dword ptr [edi + 8]            
  0x001EE66E  d95e38                  fstp     dword ptr [esi + 0x38]         
  0x001EE671  e86bfdffff              call     0x1ee3e1                       ; -> sub_001EE3E1
  0x001EE676  d94630                  fld      dword ptr [esi + 0x30]         
  0x001EE679  d807                    fadd     dword ptr [edi]                
  0x001EE67B  d95e30                  fstp     dword ptr [esi + 0x30]         
  0x001EE67E  d94634                  fld      dword ptr [esi + 0x34]         
  0x001EE681  d84704                  fadd     dword ptr [edi + 4]            
  0x001EE684  d95e34                  fstp     dword ptr [esi + 0x34]         
  0x001EE687  d94638                  fld      dword ptr [esi + 0x38]         
  0x001EE68A  d84708                  fadd     dword ptr [edi + 8]            
  0x001EE68D  d95e38                  fstp     dword ptr [esi + 0x38]         
  0x001EE690  eb05                    jmp      0x1ee697                       
                                        ; XREF: 0x001EE655 (cond_jump)
  0x001EE692  e84afdffff              call     0x1ee3e1                       ; -> sub_001EE3E1
                                        ; XREF: 0x001EE63C (cond_jump), 0x001EE690 (jump)
  0x001EE697  8b457c                  mov      eax, dword ptr [ebp + 0x7c]    
  0x001EE69A  85c0                    test     eax, eax                       
  0x001EE69C  741a                    je       0x1ee6b8                       
  0x001EE69E  d94630                  fld      dword ptr [esi + 0x30]         
  0x001EE6A1  d800                    fadd     dword ptr [eax]                
  0x001EE6A3  d95e30                  fstp     dword ptr [esi + 0x30]         
  0x001EE6A6  d94004                  fld      dword ptr [eax + 4]            
  0x001EE6A9  d84634                  fadd     dword ptr [esi + 0x34]         
  0x001EE6AC  d95e34                  fstp     dword ptr [esi + 0x34]         
  0x001EE6AF  d94008                  fld      dword ptr [eax + 8]            
  0x001EE6B2  d84638                  fadd     dword ptr [esi + 0x38]         
  0x001EE6B5  d95e38                  fstp     dword ptr [esi + 0x38]         
                                        ; XREF: 0x001EE69C (cond_jump)
  0x001EE6B8  5f                      pop      edi                            
  0x001EE6B9  8bc6                    mov      eax, esi                       
  0x001EE6BB  5e                      pop      esi                            
  0x001EE6BC  83c55c                  add      ebp, 0x5c                      
  0x001EE6BF  c9                      leave                                   
  0x001EE6C0  c21c00                  ret      0x1c                           
  0x001EE6C3  8b442404                mov      eax, dword ptr [esp + 4]       
  0x001EE6C7  ff4004                  inc      dword ptr [eax + 4]            
  0x001EE6CA  8b4004                  mov      eax, dword ptr [eax + 4]       
  0x001EE6CD  c20400                  ret      4                              

; ============================================================
; Function: sub_001EE6D0
; Start: 0x001EE6D0  End: 0x001EE6FF  Size: 47 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_001E9AED
; ============================================================
sub_001EE6D0:
  0x001EE6D0  56                      push     esi                            
  0x001EE6D1  8bf1                    mov      esi, ecx                       
  0x001EE6D3  837e0800                cmp      dword ptr [esi + 8], 0         
  0x001EE6D7  c70650802100            mov      dword ptr [esi], 0x218050      
  0x001EE6DD  7420                    je       0x1ee6ff                       
  0x001EE6DF  8b4614                  mov      eax, dword ptr [esi + 0x14]    
  0x001EE6E2  85c0                    test     eax, eax                       
  0x001EE6E4  7406                    je       0x1ee6ec                       
  0x001EE6E6  50                      push     eax                            
  0x001EE6E7  e801b4ffff              call     0x1e9aed                       ; -> sub_001E9AED
                                        ; XREF: 0x001EE6E4 (cond_jump)
  0x001EE6EC  8b7610                  mov      esi, dword ptr [esi + 0x10]    
  0x001EE6EF  85f6                    test     esi, esi                       
  0x001EE6F1  7406                    je       0x1ee6f9                       
  0x001EE6F3  56                      push     esi                            
  0x001EE6F4  e8f4b3ffff              call     0x1e9aed                       ; -> sub_001E9AED
                                        ; XREF: 0x001EE6F1 (cond_jump)
  0x001EE6F9  5e                      pop      esi                            
  0x001EE6FA  e901dffeff              jmp      0x1dc600                       
; end of function
                                        ; XREF: 0x001EE6DD (cond_jump)
  0x001EE6FF  5e                      pop      esi                            
  0x001EE700  c3                      ret                                     

; ============================================================
; Function: sub_001EE701
; Start: 0x001EE701  End: 0x001EE73A  Size: 57 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_001EE701:
  0x001EE701  55                      push     ebp                            
  0x001EE702  8bec                    mov      ebp, esp                       
  0x001EE704  56                      push     esi                            
  0x001EE705  8b750c                  mov      esi, dword ptr [ebp + 0xc]     
  0x001EE708  57                      push     edi                            
  0x001EE709  6a04                    push     4                              
  0x001EE70B  59                      pop      ecx                            
  0x001EE70C  bf70802100              mov      edi, 0x218070                  
  0x001EE711  33c0                    xor      eax, eax                       
  0x001EE713  f3a7                    repe cmpsd dword ptr [esi], dword ptr es:[edi] 
  0x001EE715  7411                    je       0x1ee728                       
  0x001EE717  8b750c                  mov      esi, dword ptr [ebp + 0xc]     
  0x001EE71A  6a04                    push     4                              
  0x001EE71C  59                      pop      ecx                            
  0x001EE71D  bf64bd2100              mov      edi, 0x21bd64                  
  0x001EE722  33c0                    xor      eax, eax                       
  0x001EE724  f3a7                    repe cmpsd dword ptr [esi], dword ptr es:[edi] 
  0x001EE726  7512                    jne      0x1ee73a                       
                                        ; XREF: 0x001EE715 (cond_jump)
  0x001EE728  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x001EE72B  8b4d10                  mov      ecx, dword ptr [ebp + 0x10]    
  0x001EE72E  8901                    mov      dword ptr [ecx], eax           
  0x001EE730  8b08                    mov      ecx, dword ptr [eax]           
  0x001EE732  50                      push     eax                            
  0x001EE733  ff5104                  call     dword ptr [ecx + 4]            
  0x001EE736  33c0                    xor      eax, eax                       
  0x001EE738  eb05                    jmp      0x1ee73f                       
; end of function
                                        ; XREF: 0x001EE726 (cond_jump)
  0x001EE73A  b802400080              mov      eax, 0x80004002                
                                        ; XREF: 0x001EE738 (jump)
  0x001EE73F  5f                      pop      edi                            
  0x001EE740  5e                      pop      esi                            
  0x001EE741  5d                      pop      ebp                            
  0x001EE742  c20c00                  ret      0xc                            

; ============================================================
; Function: sub_001EE745
; Start: 0x001EE745  End: 0x001EE767  Size: 34 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_001EE745:
  0x001EE745  81ecdc000000            sub      esp, 0xdc                      
  0x001EE74B  8b8424e0000000          mov      eax, dword ptr [esp + 0xe0]    
  0x001EE752  57                      push     edi                            
  0x001EE753  33ff                    xor      edi, edi                       
  0x001EE755  3bc7                    cmp      eax, edi                       
  0x001EE757  894c2408                mov      dword ptr [esp + 8], ecx       
  0x001EE75B  750a                    jne      0x1ee767                       
  0x001EE75D  b86c087688              mov      eax, 0x8876086c                
  0x001EE762  e941020000              jmp      0x1ee9a8                       
; end of function
                                        ; XREF: 0x001EE75B (cond_jump)
  0x001EE767  53                      push     ebx                            
  0x001EE768  55                      push     ebp                            
  0x001EE769  56                      push     esi                            
  0x001EE76A  894108                  mov      dword ptr [ecx + 8], eax       
  0x001EE76D  e86edefeff              call     0x1dc5e0                       ; -> sub_001DC5E0
  0x001EE772  8d442418                lea      eax, [esp + 0x18]              
  0x001EE776  50                      push     eax                            
  0x001EE777  e8c4dbfeff              call     0x1dc340                       ; -> sub_001DC340
  0x001EE77C  6a02                    push     2                              
  0x001EE77E  5e                      pop      esi                            
  0x001EE77F  33db                    xor      ebx, ebx                       
  0x001EE781  6a03                    push     3                              
  0x001EE783  897c2414                mov      dword ptr [esp + 0x14], edi    
  0x001EE787  43                      inc      ebx                            
  0x001EE788  5d                      pop      ebp                            
                                        ; XREF: 0x001EE99D (cond_jump)
  0x001EE789  e8deaaffff              call     0x1e926c                       ; -> sub_001E926C
  0x001EE78E  6844010000              push     0x144                          
  0x001EE793  881d9dde1e00            mov      byte ptr [0x1ede9d], bl        
  0x001EE799  e8721bffff              call     0x1e0310                       ; -> sub_001E0310
  0x001EE79E  830d68ad1e000f          or       dword ptr [0x1ead68], 0xf      
  0x001EE7A5  68021b0000              push     0x1b02                         
  0x001EE7AA  881dd7df1e00            mov      byte ptr [0x1edfd7], bl        
  0x001EE7B0  893dd0b01e00            mov      dword ptr [0x1eb0d0], edi      
  0x001EE7B6  881df7df1e00            mov      byte ptr [0x1edff7], bl        
  0x001EE7BC  e8affcfeff              call     0x1de470                       ; -> sub_001DE470
  0x001EE7C1  57                      push     edi                            
  0x001EE7C2  881dffdf1e00            mov      byte ptr [0x1edfff], bl        
  0x001EE7C8  e883f9feff              call     0x1de150                       ; -> sub_001DE150
  0x001EE7CD  57                      push     edi                            
  0x001EE7CE  881df5df1e00            mov      byte ptr [0x1edff5], bl        
  0x001EE7D4  e8b7fdfeff              call     0x1de590                       ; -> sub_001DE590
  0x001EE7D9  66810d68ad1e000108      or       word ptr [0x1ead68], 0x801     
  0x001EE7E2  6a04                    push     4                              
  0x001EE7E4  58                      pop      eax                            
  0x001EE7E5  57                      push     edi                            
  0x001EE7E6  57                      push     edi                            
  0x001EE7E7  881df2df1e00            mov      byte ptr [0x1edff2], bl        
  0x001EE7ED  891d3cb11e00            mov      dword ptr [0x1eb13c], ebx      
  0x001EE7F3  881d41e01e00            mov      byte ptr [0x1ee041], bl        
  0x001EE7F9  a3a0ad1e00              mov      dword ptr [0x1eada0], eax      
  0x001EE7FE  881d49e01e00            mov      byte ptr [0x1ee049], bl        
  0x001EE804  8935a8ad1e00            mov      dword ptr [0x1eada8], esi      
  0x001EE80A  881d4de01e00            mov      byte ptr [0x1ee04d], bl        
  0x001EE810  893dacad1e00            mov      dword ptr [0x1eadac], edi      
  0x001EE816  881d51e01e00            mov      byte ptr [0x1ee051], bl        
  0x001EE81C  a3b0ad1e00              mov      dword ptr [0x1eadb0], eax      
  0x001EE821  881d59e01e00            mov      byte ptr [0x1ee059], bl        
  0x001EE827  8935b8ad1e00            mov      dword ptr [0x1eadb8], esi      
  0x001EE82D  881d5de01e00            mov      byte ptr [0x1ee05d], bl        
  0x001EE833  893dbcad1e00            mov      dword ptr [0x1eadbc], edi      
  0x001EE839  881d42e01e00            mov      byte ptr [0x1ee042], bl        
  0x001EE83F  891d20ae1e00            mov      dword ptr [0x1eae20], ebx      
  0x001EE845  881d52e01e00            mov      byte ptr [0x1ee052], bl        
  0x001EE84B  891d30ae1e00            mov      dword ptr [0x1eae30], ebx      
  0x001EE851  881d1de01e00            mov      byte ptr [0x1ee01d], bl        
  0x001EE857  89357cad1e00            mov      dword ptr [0x1ead7c], esi      
  0x001EE85D  881d21e01e00            mov      byte ptr [0x1ee021], bl        
  0x001EE863  893580ad1e00            mov      dword ptr [0x1ead80], esi      
  0x001EE869  881d25e01e00            mov      byte ptr [0x1ee025], bl        
  0x001EE86F  893d84ad1e00            mov      dword ptr [0x1ead84], edi      
  0x001EE875  881d81e01e00            mov      byte ptr [0x1ee081], bl        
  0x001EE87B  e860fdfeff              call     0x1de5e0                       ; -> sub_001DE5E0
  0x001EE880  66810d68ad1e000104      or       word ptr [0x1ead68], 0x401     
  0x001EE889  f644244410              test     byte ptr [esp + 0x44], 0x10    
  0x001EE88E  881d11e01e00            mov      byte ptr [0x1ee011], bl        
  0x001EE894  892d70ad1e00            mov      dword ptr [0x1ead70], ebp      
  0x001EE89A  881d15e01e00            mov      byte ptr [0x1ee015], bl        
  0x001EE8A0  892d74ad1e00            mov      dword ptr [0x1ead74], ebp      
  0x001EE8A6  881d65e01e00            mov      byte ptr [0x1ee065], bl        
  0x001EE8AC  893dc4ad1e00            mov      dword ptr [0x1eadc4], edi      
  0x001EE8B2  745f                    je       0x1ee913                       
  0x001EE8B4  f644244820              test     byte ptr [esp + 0x48], 0x20    
  0x001EE8B9  7458                    je       0x1ee913                       
  0x001EE8BB  8bd3                    mov      edx, ebx                       
  0x001EE8BD  b904030400              mov      ecx, 0x40304                   
  0x001EE8C2  881dbadf1e00            mov      byte ptr [0x1edfba], bl        
  0x001EE8C8  e8e3f5feff              call     0x1ddeb0                       ; -> sub_001DDEB0
  0x001EE8CD  ba02030000              mov      edx, 0x302                     
  0x001EE8D2  b944030400              mov      ecx, 0x40344                   
  0x001EE8D7  891d5cb01e00            mov      dword ptr [0x1eb05c], ebx      
  0x001EE8DD  881dbddf1e00            mov      byte ptr [0x1edfbd], bl        
  0x001EE8E3  e8c8f5feff              call     0x1ddeb0                       ; -> sub_001DDEB0
  0x001EE8E8  ba03030000              mov      edx, 0x303                     
  0x001EE8ED  b948030400              mov      ecx, 0x40348                   
  0x001EE8F2  c70568b01e0002030000    mov      dword ptr [0x1eb068], 0x302    
  0x001EE8FC  881dbedf1e00            mov      byte ptr [0x1edfbe], bl        
  0x001EE902  e8a9f5feff              call     0x1ddeb0                       ; -> sub_001DDEB0
  0x001EE907  c7056cb01e0003030000    mov      dword ptr [0x1eb06c], 0x303    
  0x001EE911  eb54                    jmp      0x1ee967                       
                                        ; XREF: 0x001EE8B2 (cond_jump), 0x001EE8B9 (cond_jump)
  0x001EE913  8bd3                    mov      edx, ebx                       
  0x001EE915  b900030400              mov      ecx, 0x40300                   
  0x001EE91A  881dbbdf1e00            mov      byte ptr [0x1edfbb], bl        
  0x001EE920  e88bf5feff              call     0x1ddeb0                       ; -> sub_001DDEB0
  0x001EE925  ba06020000              mov      edx, 0x206                     
  0x001EE92A  b93c030400              mov      ecx, 0x4033c                   
  0x001EE92F  891d60b01e00            mov      dword ptr [0x1eb060], ebx      
  0x001EE935  881db9df1e00            mov      byte ptr [0x1edfb9], bl        
  0x001EE93B  e870f5feff              call     0x1ddeb0                       ; -> sub_001DDEB0
  0x001EE940  6a7f                    push     0x7f                           
  0x001EE942  5a                      pop      edx                            
  0x001EE943  b940030400              mov      ecx, 0x40340                   
  0x001EE948  c70558b01e0006020000    mov      dword ptr [0x1eb058], 0x206    
  0x001EE952  881dbcdf1e00            mov      byte ptr [0x1edfbc], bl        
  0x001EE958  e853f5feff              call     0x1ddeb0                       ; -> sub_001DDEB0
  0x001EE95D  c70564b01e007f000000    mov      dword ptr [0x1eb064], 0x7f     
                                        ; XREF: 0x001EE911 (jump)
  0x001EE967  397c2410                cmp      dword ptr [esp + 0x10], edi    
  0x001EE96B  7545                    jne      0x1ee9b2                       
  0x001EE96D  57                      push     edi                            
  0x001EE96E  57                      push     edi                            
  0x001EE96F  881d98de1e00            mov      byte ptr [0x1ede98], bl        
  0x001EE975  e8f6ecfeff              call     0x1dd670                       ; -> sub_001DD670
  0x001EE97A  57                      push     edi                            
  0x001EE97B  57                      push     edi                            
  0x001EE97C  57                      push     edi                            
  0x001EE97D  881d9fde1e00            mov      byte ptr [0x1ede9f], bl        
  0x001EE983  e8c815ffff              call     0x1dff50                       ; -> sub_001DFF50
  0x001EE988  8b442414                mov      eax, dword ptr [esp + 0x14]    
  0x001EE98C  83c010                  add      eax, 0x10                      
                                        ; XREF: 0x001EE9B9 (jump)
  0x001EE98F  50                      push     eax                            
  0x001EE990  e8e9b1ffff              call     0x1e9b7e                       ; -> sub_001E9B7E
  0x001EE995  ff442410                inc      dword ptr [esp + 0x10]         
  0x001EE999  39742410                cmp      dword ptr [esp + 0x10], esi    
  0x001EE99D  0f82e6fdffff            jb       0x1ee789                       
  0x001EE9A3  5e                      pop      esi                            
  0x001EE9A4  5d                      pop      ebp                            
  0x001EE9A5  33c0                    xor      eax, eax                       
  0x001EE9A7  5b                      pop      ebx                            
                                        ; XREF: 0x001EE762 (jump)
  0x001EE9A8  5f                      pop      edi                            
  0x001EE9A9  81c4dc000000            add      esp, 0xdc                      
  0x001EE9AF  c20400                  ret      4                              
                                        ; XREF: 0x001EE96B (cond_jump)
  0x001EE9B2  8b442414                mov      eax, dword ptr [esp + 0x14]    
  0x001EE9B6  83c014                  add      eax, 0x14                      
  0x001EE9B9  ebd4                    jmp      0x1ee98f                       
  0x001EE9BB  8b442408                mov      eax, dword ptr [esp + 8]       
  0x001EE9BF  85c0                    test     eax, eax                       
  0x001EE9C1  7507                    jne      0x1ee9ca                       
  0x001EE9C3  b86c087688              mov      eax, 0x8876086c                
  0x001EE9C8  eb10                    jmp      0x1ee9da                       
                                        ; XREF: 0x001EE9C1 (cond_jump)
  0x001EE9CA  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x001EE9CE  8b4908                  mov      ecx, dword ptr [ecx + 8]       
  0x001EE9D1  8908                    mov      dword ptr [eax], ecx           
  0x001EE9D3  e808dcfeff              call     0x1dc5e0                       ; -> sub_001DC5E0
  0x001EE9D8  33c0                    xor      eax, eax                       
                                        ; XREF: 0x001EE9C8 (jump)
  0x001EE9DA  c20800                  ret      8                              
  0x001EE9DD  56                      push     esi                            
  0x001EE9DE  8b742408                mov      esi, dword ptr [esp + 8]       
  0x001EE9E2  837e0c00                cmp      dword ptr [esi + 0xc], 0       
  0x001EE9E6  7407                    je       0x1ee9ef                       
  0x001EE9E8  b805400080              mov      eax, 0x80004005                
  0x001EE9ED  eb19                    jmp      0x1eea08                       
                                        ; XREF: 0x001EE9E6 (cond_jump)
  0x001EE9EF  ff7610                  push     dword ptr [esi + 0x10]         
  0x001EE9F2  e836aeffff              call     0x1e982d                       ; -> sub_001E982D
  0x001EE9F7  ff7614                  push     dword ptr [esi + 0x14]         
  0x001EE9FA  e8a6acffff              call     0x1e96a5                       ; -> sub_001E96A5
  0x001EE9FF  c7460c01000000          mov      dword ptr [esi + 0xc], 1       
  0x001EEA06  33c0                    xor      eax, eax                       
                                        ; XREF: 0x001EE9ED (jump)
  0x001EEA08  5e                      pop      esi                            
  0x001EEA09  c20400                  ret      4                              

; ============================================================
; Function: sub_001EEA0C
; Start: 0x001EEA0C  End: 0x001EEA99  Size: 141 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_001EEA0C:
  0x001EEA0C  55                      push     ebp                            
  0x001EEA0D  8bec                    mov      ebp, esp                       
  0x001EEA0F  83ec74                  sub      esp, 0x74                      
  0x001EEA12  56                      push     esi                            
  0x001EEA13  8b7514                  mov      esi, dword ptr [ebp + 0x14]    
  0x001EEA16  85f6                    test     esi, esi                       
  0x001EEA18  57                      push     edi                            
  0x001EEA19  7410                    je       0x1eea2b                       
  0x001EEA1B  8b06                    mov      eax, dword ptr [esi]           
  0x001EEA1D  d9e8                    fld1                                    
  0x001EEA1F  8945dc                  mov      dword ptr [ebp - 0x24], eax    
  0x001EEA22  d95de4                  fstp     dword ptr [ebp - 0x1c]         
  0x001EEA25  8b4604                  mov      eax, dword ptr [esi + 4]       
  0x001EEA28  8945e0                  mov      dword ptr [ebp - 0x20], eax    
                                        ; XREF: 0x001EEA19 (cond_jump)
  0x001EEA2B  8b5518                  mov      edx, dword ptr [ebp + 0x18]    
  0x001EEA2E  85d2                    test     edx, edx                       
  0x001EEA30  7410                    je       0x1eea42                       
  0x001EEA32  8b02                    mov      eax, dword ptr [edx]           
  0x001EEA34  d9ee                    fldz                                    
  0x001EEA36  8945e8                  mov      dword ptr [ebp - 0x18], eax    
  0x001EEA39  d95df0                  fstp     dword ptr [ebp - 0x10]         
  0x001EEA3C  8b4204                  mov      eax, dword ptr [edx + 4]       
  0x001EEA3F  8945ec                  mov      dword ptr [ebp - 0x14], eax    
                                        ; XREF: 0x001EEA30 (cond_jump)
  0x001EEA42  d9451c                  fld      dword ptr [ebp + 0x1c]         
  0x001EEA45  d815f80e2500            fcom     dword ptr [0x250ef8]           
  0x001EEA4B  dfe0                    fnstsw   ax                             
  0x001EEA4D  f6c444                  test     ah, 0x44                       
  0x001EEA50  7b1e                    jnp      0x1eea70                       
  0x001EEA52  d9ee                    fldz                                    
  0x001EEA54  d95dcc                  fstp     dword ptr [ebp - 0x34]         
  0x001EEA57  d9ee                    fldz                                    
  0x001EEA59  d95dd0                  fstp     dword ptr [ebp - 0x30]         
  0x001EEA5C  d90554112500            fld      dword ptr [0x251154]           
  0x001EEA62  d8c9                    fmul     st(1)                          
  0x001EEA64  d9c0                    fld      st(0)                          
  0x001EEA66  d9fe                    fsin                                    
  0x001EEA68  d95dd4                  fstp     dword ptr [ebp - 0x2c]         
  0x001EEA6B  d9ff                    fcos                                    
  0x001EEA6D  d95dd8                  fstp     dword ptr [ebp - 0x28]         
                                        ; XREF: 0x001EEA50 (cond_jump)
  0x001EEA70  8b4d20                  mov      ecx, dword ptr [ebp + 0x20]    
  0x001EEA73  85c9                    test     ecx, ecx                       
  0x001EEA75  7410                    je       0x1eea87                       
  0x001EEA77  8b01                    mov      eax, dword ptr [ecx]           
  0x001EEA79  d9ee                    fldz                                    
  0x001EEA7B  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x001EEA7E  d95dfc                  fstp     dword ptr [ebp - 4]            
  0x001EEA81  8b4104                  mov      eax, dword ptr [ecx + 4]       
  0x001EEA84  8945f8                  mov      dword ptr [ebp - 8], eax       
                                        ; XREF: 0x001EEA75 (cond_jump)
  0x001EEA87  d81df80e2500            fcomp    dword ptr [0x250ef8]           
  0x001EEA8D  dfe0                    fnstsw   ax                             
  0x001EEA8F  f6c444                  test     ah, 0x44                       
  0x001EEA92  7b05                    jnp      0x1eea99                       
  0x001EEA94  8d45cc                  lea      eax, [ebp - 0x34]              
  0x001EEA97  eb02                    jmp      0x1eea9b                       
; end of function
                                        ; XREF: 0x001EEA92 (cond_jump)
  0x001EEA99  33c0                    xor      eax, eax                       
                                        ; XREF: 0x001EEA97 (jump)
  0x001EEA9B  f7d9                    neg      ecx                            
  0x001EEA9D  1bc9                    sbb      ecx, ecx                       
  0x001EEA9F  8d7df4                  lea      edi, [ebp - 0xc]               
  0x001EEAA2  23cf                    and      ecx, edi                       
  0x001EEAA4  51                      push     ecx                            
  0x001EEAA5  50                      push     eax                            
  0x001EEAA6  f7da                    neg      edx                            
  0x001EEAA8  1bd2                    sbb      edx, edx                       
  0x001EEAAA  8d45e8                  lea      eax, [ebp - 0x18]              
  0x001EEAAD  23d0                    and      edx, eax                       
  0x001EEAAF  f7de                    neg      esi                            
  0x001EEAB1  52                      push     edx                            
  0x001EEAB2  1bf6                    sbb      esi, esi                       
  0x001EEAB4  8d45dc                  lea      eax, [ebp - 0x24]              
  0x001EEAB7  23f0                    and      esi, eax                       
  0x001EEAB9  56                      push     esi                            
  0x001EEABA  6a00                    push     0                              
  0x001EEABC  6a00                    push     0                              
  0x001EEABE  8d458c                  lea      eax, [ebp - 0x74]              
  0x001EEAC1  50                      push     eax                            
  0x001EEAC2  e8fdf9ffff              call     0x1ee4c4                       ; -> sub_001EE4C4
  0x001EEAC7  ff7524                  push     dword ptr [ebp + 0x24]         
  0x001EEACA  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x001EEACD  8b08                    mov      ecx, dword ptr [eax]           
  0x001EEACF  8d558c                  lea      edx, [ebp - 0x74]              
  0x001EEAD2  52                      push     edx                            
  0x001EEAD3  ff7510                  push     dword ptr [ebp + 0x10]         
  0x001EEAD6  ff750c                  push     dword ptr [ebp + 0xc]          
  0x001EEAD9  50                      push     eax                            
  0x001EEADA  ff5118                  call     dword ptr [ecx + 0x18]         
  0x001EEADD  5f                      pop      edi                            
  0x001EEADE  5e                      pop      esi                            
  0x001EEADF  c9                      leave                                   
  0x001EEAE0  c22000                  ret      0x20                           
  0x001EEAE3  56                      push     esi                            
  0x001EEAE4  8b742408                mov      esi, dword ptr [esp + 8]       
  0x001EEAE8  837e0c00                cmp      dword ptr [esi + 0xc], 0       
  0x001EEAEC  7507                    jne      0x1eeaf5                       
  0x001EEAEE  b805400080              mov      eax, 0x80004005                
  0x001EEAF3  eb0e                    jmp      0x1eeb03                       
                                        ; XREF: 0x001EEAEC (cond_jump)
  0x001EEAF5  ff7610                  push     dword ptr [esi + 0x10]         
  0x001EEAF8  e8a8abffff              call     0x1e96a5                       ; -> sub_001E96A5
  0x001EEAFD  83660c00                and      dword ptr [esi + 0xc], 0       
  0x001EEB01  33c0                    xor      eax, eax                       
                                        ; XREF: 0x001EEAF3 (jump)
  0x001EEB03  5e                      pop      esi                            
  0x001EEB04  c20400                  ret      4                              

; ============================================================
; Function: sub_001EEB07
; Start: 0x001EEB07  End: 0x001EEB25  Size: 30 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_001EEB07:
  0x001EEB07  8bc1                    mov      eax, ecx                       
  0x001EEB09  33c9                    xor      ecx, ecx                       
  0x001EEB0B  c70050802100            mov      dword ptr [eax], 0x218050      
  0x001EEB11  c7400401000000          mov      dword ptr [eax + 4], 1         
  0x001EEB18  894808                  mov      dword ptr [eax + 8], ecx       
  0x001EEB1B  89480c                  mov      dword ptr [eax + 0xc], ecx     
  0x001EEB1E  894810                  mov      dword ptr [eax + 0x10], ecx    
  0x001EEB21  894814                  mov      dword ptr [eax + 0x14], ecx    
  0x001EEB24  c3                      ret                                     
; end of function
  0x001EEB25  56                      push     esi                            
  0x001EEB26  8b742408                mov      esi, dword ptr [esp + 8]       
  0x001EEB2A  ff4e04                  dec      dword ptr [esi + 4]            
  0x001EEB2D  8b4604                  mov      eax, dword ptr [esi + 4]       
  0x001EEB30  7510                    jne      0x1eeb42                       
  0x001EEB32  8bce                    mov      ecx, esi                       
  0x001EEB34  e897fbffff              call     0x1ee6d0                       ; -> sub_001EE6D0
  0x001EEB39  56                      push     esi                            
  0x001EEB3A  e8b88cfcff              call     0x1b77f7                       ; -> sub_001B77F7
  0x001EEB3F  59                      pop      ecx                            
  0x001EEB40  33c0                    xor      eax, eax                       
                                        ; XREF: 0x001EEB30 (cond_jump)
  0x001EEB42  5e                      pop      esi                            
  0x001EEB43  c20400                  ret      4                              
  0x001EEB46  55                      push     ebp                            
  0x001EEB47  8d6c249c                lea      ebp, [esp - 0x64]              
  0x001EEB4B  81eca4000000            sub      esp, 0xa4                      
  0x001EEB51  53                      push     ebx                            
  0x001EEB52  33db                    xor      ebx, ebx                       
  0x001EEB54  395d70                  cmp      dword ptr [ebp + 0x70], ebx    
  0x001EEB57  750a                    jne      0x1eeb63                       
  0x001EEB59  b86c087688              mov      eax, 0x8876086c                
  0x001EEB5E  e998020000              jmp      0x1eedfb                       
                                        ; XREF: 0x001EEB57 (cond_jump)
  0x001EEB63  56                      push     esi                            
  0x001EEB64  57                      push     edi                            
  0x001EEB65  8d4530                  lea      eax, [ebp + 0x30]              
  0x001EEB68  50                      push     eax                            
  0x001EEB69  53                      push     ebx                            
  0x001EEB6A  ff7570                  push     dword ptr [ebp + 0x70]         
  0x001EEB6D  e89e19ffff              call     0x1e0510                       ; -> sub_001E0510
  0x001EEB72  8b7574                  mov      esi, dword ptr [ebp + 0x74]    
  0x001EEB75  3bf3                    cmp      esi, ebx                       
  0x001EEB77  743b                    je       0x1eebb4                       
  0x001EEB79  8d7d54                  lea      edi, [ebp + 0x54]              
  0x001EEB7C  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEB7D  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEB7E  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEB7F  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEB80  395d54                  cmp      dword ptr [ebp + 0x54], ebx    
  0x001EEB83  7c25                    jl       0x1eebaa                       
  0x001EEB85  8b4d5c                  mov      ecx, dword ptr [ebp + 0x5c]    
  0x001EEB88  394d54                  cmp      dword ptr [ebp + 0x54], ecx    
  0x001EEB8B  7f1d                    jg       0x1eebaa                       
  0x001EEB8D  8b5544                  mov      edx, dword ptr [ebp + 0x44]    
  0x001EEB90  395554                  cmp      dword ptr [ebp + 0x54], edx    
  0x001EEB93  7f15                    jg       0x1eebaa                       
  0x001EEB95  395d58                  cmp      dword ptr [ebp + 0x58], ebx    
  0x001EEB98  7c10                    jl       0x1eebaa                       
  0x001EEB9A  8b4560                  mov      eax, dword ptr [ebp + 0x60]    
  0x001EEB9D  394558                  cmp      dword ptr [ebp + 0x58], eax    
  0x001EEBA0  7f08                    jg       0x1eebaa                       
  0x001EEBA2  8b7548                  mov      esi, dword ptr [ebp + 0x48]    
  0x001EEBA5  397558                  cmp      dword ptr [ebp + 0x58], esi    
  0x001EEBA8  7e20                    jle      0x1eebca                       
                                        ; XREF: 0x001EEB83 (cond_jump), 0x001EEB8B (cond_jump), 0x001EEB93 (cond_jump), 0x001EEB98 (cond_jump), 0x001EEBA0 (cond_jump)
  0x001EEBAA  b86c087688              mov      eax, 0x8876086c                
  0x001EEBAF  e945020000              jmp      0x1eedf9                       
                                        ; XREF: 0x001EEB77 (cond_jump)
  0x001EEBB4  8b5544                  mov      edx, dword ptr [ebp + 0x44]    
  0x001EEBB7  8b7548                  mov      esi, dword ptr [ebp + 0x48]    
  0x001EEBBA  8bca                    mov      ecx, edx                       
  0x001EEBBC  8bc6                    mov      eax, esi                       
  0x001EEBBE  895d54                  mov      dword ptr [ebp + 0x54], ebx    
  0x001EEBC1  895d58                  mov      dword ptr [ebp + 0x58], ebx    
  0x001EEBC4  894d5c                  mov      dword ptr [ebp + 0x5c], ecx    
  0x001EEBC7  894560                  mov      dword ptr [ebp + 0x60], eax    
                                        ; XREF: 0x001EEBA8 (cond_jump)
  0x001EEBCA  2b4d54                  sub      ecx, dword ptr [ebp + 0x54]    
  0x001EEBCD  2b4558                  sub      eax, dword ptr [ebp + 0x58]    
  0x001EEBD0  85d2                    test     edx, edx                       
  0x001EEBD2  894d74                  mov      dword ptr [ebp + 0x74], ecx    
  0x001EEBD5  db4574                  fild     dword ptr [ebp + 0x74]         
  0x001EEBD8  894574                  mov      dword ptr [ebp + 0x74], eax    
  0x001EEBDB  db4574                  fild     dword ptr [ebp + 0x74]         
  0x001EEBDE  895574                  mov      dword ptr [ebp + 0x74], edx    
  0x001EEBE1  d905f40e2500            fld      dword ptr [0x250ef4]           
  0x001EEBE7  db4574                  fild     dword ptr [ebp + 0x74]         
  0x001EEBEA  7d06                    jge      0x1eebf2                       
  0x001EEBEC  d805fc0e2500            fadd     dword ptr [0x250efc]           
                                        ; XREF: 0x001EEBEA (cond_jump)
  0x001EEBF2  85f6                    test     esi, esi                       
  0x001EEBF4  d8f9                    fdivr    st(1)                          
  0x001EEBF6  897550                  mov      dword ptr [ebp + 0x50], esi    
  0x001EEBF9  d95d74                  fstp     dword ptr [ebp + 0x74]         
  0x001EEBFC  db4550                  fild     dword ptr [ebp + 0x50]         
  0x001EEBFF  7d06                    jge      0x1eec07                       
  0x001EEC01  d805fc0e2500            fadd     dword ptr [0x250efc]           
                                        ; XREF: 0x001EEBFF (cond_jump)
  0x001EEC07  def9                    fdivp    st(1)                          
  0x001EEC09  8d7554                  lea      esi, [ebp + 0x54]              
  0x001EEC0C  8d7dc0                  lea      edi, [ebp - 0x40]              
  0x001EEC0F  8b5d7c                  mov      ebx, dword ptr [ebp + 0x7c]    
  0x001EEC12  db4554                  fild     dword ptr [ebp + 0x54]         
  0x001EEC15  d84d74                  fmul     dword ptr [ebp + 0x74]         
  0x001EEC18  db455c                  fild     dword ptr [ebp + 0x5c]         
  0x001EEC1B  d84d74                  fmul     dword ptr [ebp + 0x74]         
  0x001EEC1E  d95d4c                  fstp     dword ptr [ebp + 0x4c]         
  0x001EEC21  db4558                  fild     dword ptr [ebp + 0x58]         
  0x001EEC24  d8ca                    fmul     st(2)                          
  0x001EEC26  d95550                  fst      dword ptr [ebp + 0x50]         
  0x001EEC29  db4560                  fild     dword ptr [ebp + 0x60]         
  0x001EEC2C  d8cb                    fmul     st(3)                          
  0x001EEC2E  d95d74                  fstp     dword ptr [ebp + 0x74]         
  0x001EEC31  d9ee                    fldz                                    
  0x001EEC33  d95d54                  fstp     dword ptr [ebp + 0x54]         
  0x001EEC36  d9ee                    fldz                                    
  0x001EEC38  d95d58                  fstp     dword ptr [ebp + 0x58]         
  0x001EEC3B  d9ee                    fldz                                    
  0x001EEC3D  d95d5c                  fstp     dword ptr [ebp + 0x5c]         
  0x001EEC40  d9e8                    fld1                                    
  0x001EEC42  d95d60                  fstp     dword ptr [ebp + 0x60]         
  0x001EEC45  d9c1                    fld      st(1)                          
  0x001EEC47  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC48  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC49  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC4A  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC4B  d95d5c                  fstp     dword ptr [ebp + 0x5c]         
  0x001EEC4E  8b455c                  mov      eax, dword ptr [ebp + 0x5c]    
  0x001EEC51  d95d60                  fstp     dword ptr [ebp + 0x60]         
  0x001EEC54  8945d4                  mov      dword ptr [ebp - 0x2c], eax    
  0x001EEC57  d9ee                    fldz                                    
  0x001EEC59  8b4560                  mov      eax, dword ptr [ebp + 0x60]    
  0x001EEC5C  d95d54                  fstp     dword ptr [ebp + 0x54]         
  0x001EEC5F  8945d8                  mov      dword ptr [ebp - 0x28], eax    
  0x001EEC62  d9c2                    fld      st(2)                          
  0x001EEC64  d95d58                  fstp     dword ptr [ebp + 0x58]         
  0x001EEC67  895dd0                  mov      dword ptr [ebp - 0x30], ebx    
  0x001EEC6A  d9ee                    fldz                                    
  0x001EEC6C  8b4574                  mov      eax, dword ptr [ebp + 0x74]    
  0x001EEC6F  d95d5c                  fstp     dword ptr [ebp + 0x5c]         
  0x001EEC72  8d7554                  lea      esi, [ebp + 0x54]              
  0x001EEC75  d9e8                    fld1                                    
  0x001EEC77  8d7ddc                  lea      edi, [ebp - 0x24]              
  0x001EEC7A  d95d60                  fstp     dword ptr [ebp + 0x60]         
  0x001EEC7D  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC7E  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC7F  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC80  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EEC81  d9555c                  fst      dword ptr [ebp + 0x5c]         
  0x001EEC84  ddd8                    fstp     st(0)                          
  0x001EEC86  ddd8                    fstp     st(0)                          
  0x001EEC88  894560                  mov      dword ptr [ebp + 0x60], eax    
  0x001EEC8B  8b455c                  mov      eax, dword ptr [ebp + 0x5c]    
  0x001EEC8E  d9c1                    fld      st(1)                          
  0x001EEC90  d95d54                  fstp     dword ptr [ebp + 0x54]         
  0x001EEC93  8945f0                  mov      dword ptr [ebp - 0x10], eax    
  0x001EEC96  8b4560                  mov      eax, dword ptr [ebp + 0x60]    
  0x001EEC99  d95d58                  fstp     dword ptr [ebp + 0x58]         
  0x001EEC9C  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x001EEC9F  8b4574                  mov      eax, dword ptr [ebp + 0x74]    
  0x001EECA2  d9ee                    fldz                                    
  0x001EECA4  d95d5c                  fstp     dword ptr [ebp + 0x5c]         
  0x001EECA7  895dec                  mov      dword ptr [ebp - 0x14], ebx    
  0x001EECAA  d9e8                    fld1                                    
  0x001EECAC  8d7554                  lea      esi, [ebp + 0x54]              
  0x001EECAF  d95d60                  fstp     dword ptr [ebp + 0x60]         
  0x001EECB2  8d7df8                  lea      edi, [ebp - 8]                 
  0x001EECB5  d9454c                  fld      dword ptr [ebp + 0x4c]         
  0x001EECB8  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECB9  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECBA  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECBB  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECBC  d9555c                  fst      dword ptr [ebp + 0x5c]         
  0x001EECBF  894560                  mov      dword ptr [ebp + 0x60], eax    
  0x001EECC2  8b455c                  mov      eax, dword ptr [ebp + 0x5c]    
  0x001EECC5  89450c                  mov      dword ptr [ebp + 0xc], eax     
  0x001EECC8  8b4560                  mov      eax, dword ptr [ebp + 0x60]    
  0x001EECCB  895d08                  mov      dword ptr [ebp + 8], ebx       
  0x001EECCE  894510                  mov      dword ptr [ebp + 0x10], eax    
  0x001EECD1  837d7800                cmp      dword ptr [ebp + 0x78], 0      
  0x001EECD5  d9c1                    fld      st(1)                          
  0x001EECD7  d95d54                  fstp     dword ptr [ebp + 0x54]         
  0x001EECDA  8b4550                  mov      eax, dword ptr [ebp + 0x50]    
  0x001EECDD  d9ee                    fldz                                    
  0x001EECDF  8d7554                  lea      esi, [ebp + 0x54]              
  0x001EECE2  d95d58                  fstp     dword ptr [ebp + 0x58]         
  0x001EECE5  8d7d14                  lea      edi, [ebp + 0x14]              
  0x001EECE8  d9ee                    fldz                                    
  0x001EECEA  d95d5c                  fstp     dword ptr [ebp + 0x5c]         
  0x001EECED  d9e8                    fld1                                    
  0x001EECEF  d95d60                  fstp     dword ptr [ebp + 0x60]         
  0x001EECF2  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECF3  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECF4  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECF5  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001EECF6  d9555c                  fst      dword ptr [ebp + 0x5c]         
  0x001EECF9  ddd8                    fstp     st(0)                          
  0x001EECFB  894560                  mov      dword ptr [ebp + 0x60], eax    
  0x001EECFE  ddd8                    fstp     st(0)                          
  0x001EED00  8b455c                  mov      eax, dword ptr [ebp + 0x5c]    
  0x001EED03  894528                  mov      dword ptr [ebp + 0x28], eax    
  0x001EED06  8b4560                  mov      eax, dword ptr [ebp + 0x60]    
  0x001EED09  895d24                  mov      dword ptr [ebp + 0x24], ebx    
  0x001EED0C  89452c                  mov      dword ptr [ebp + 0x2c], eax    
  0x001EED0F  7416                    je       0x1eed27                       
  0x001EED11  6a04                    push     4                              
  0x001EED13  8d75c0                  lea      esi, [ebp - 0x40]              
  0x001EED16  5f                      pop      edi                            
                                        ; XREF: 0x001EED25 (cond_jump)
  0x001EED17  ff7578                  push     dword ptr [ebp + 0x78]         
  0x001EED1A  56                      push     esi                            
  0x001EED1B  56                      push     esi                            
  0x001EED1C  e8f0f3ffff              call     0x1ee111                       ; -> sub_001EE111
  0x001EED21  83c61c                  add      esi, 0x1c                      
  0x001EED24  4f                      dec      edi                            
  0x001EED25  75f0                    jne      0x1eed17                       
                                        ; XREF: 0x001EED0F (cond_jump)
  0x001EED27  6a04                    push     4                              
  0x001EED29  8d45c4                  lea      eax, [ebp - 0x3c]              
  0x001EED2C  59                      pop      ecx                            
                                        ; XREF: 0x001EED52 (cond_jump)
  0x001EED2D  d94008                  fld      dword ptr [eax + 8]            
  0x001EED30  d80dd00e2500            fmul     dword ptr [0x250ed0]           
  0x001EED36  d940fc                  fld      dword ptr [eax - 4]            
  0x001EED39  d8e1                    fsub     st(1)                          
  0x001EED3B  d958fc                  fstp     dword ptr [eax - 4]            
  0x001EED3E  d900                    fld      dword ptr [eax]                
  0x001EED40  d8e1                    fsub     st(1)                          
  0x001EED42  d918                    fstp     dword ptr [eax]                
  0x001EED44  83c01c                  add      eax, 0x1c                      
  0x001EED47  49                      dec      ecx                            
  0x001EED48  ddd8                    fstp     st(0)                          
  0x001EED4A  d9e8                    fld1                                    
  0x001EED4C  d870ec                  fdiv     dword ptr [eax - 0x14]         
  0x001EED4F  d958ec                  fstp     dword ptr [eax - 0x14]         
  0x001EED52  75d9                    jne      0x1eed2d                       
  0x001EED54  8b7d6c                  mov      edi, dword ptr [ebp + 0x6c]    
  0x001EED57  8b4f0c                  mov      ecx, dword ptr [edi + 0xc]     
  0x001EED5A  b8000000ff              mov      eax, 0xff000000                
  0x001EED5F  33d2                    xor      edx, edx                       
  0x001EED61  23d8                    and      ebx, eax                       
  0x001EED63  3bd8                    cmp      ebx, eax                       
  0x001EED65  0f95c2                  setne    dl                             
  0x001EED68  894d78                  mov      dword ptr [ebp + 0x78], ecx    
  0x001EED6B  8bf2                    mov      esi, edx                       
  0x001EED6D  85f6                    test     esi, esi                       
  0x001EED6F  752f                    jne      0x1eeda0                       
  0x001EED71  8b4530                  mov      eax, dword ptr [ebp + 0x30]    
  0x001EED74  83f80b                  cmp      eax, 0xb                       
  0x001EED77  7f10                    jg       0x1eed89                       
  0x001EED79  7422                    je       0x1eed9d                       
  0x001EED7B  48                      dec      eax                            
  0x001EED7C  48                      dec      eax                            
  0x001EED7D  741e                    je       0x1eed9d                       
  0x001EED7F  48                      dec      eax                            
  0x001EED80  48                      dec      eax                            
  0x001EED81  741a                    je       0x1eed9d                       
  0x001EED83  48                      dec      eax                            
  0x001EED84  48                      dec      eax                            
  0x001EED85  7416                    je       0x1eed9d                       
  0x001EED87  eb17                    jmp      0x1eeda0                       
                                        ; XREF: 0x001EED77 (cond_jump)
  0x001EED89  83f80e                  cmp      eax, 0xe                       
  0x001EED8C  7c12                    jl       0x1eeda0                       
  0x001EED8E  83f80f                  cmp      eax, 0xf                       
  0x001EED91  7e0a                    jle      0x1eed9d                       
  0x001EED93  83f818                  cmp      eax, 0x18                      
  0x001EED96  7e08                    jle      0x1eeda0                       
  0x001EED98  83f81a                  cmp      eax, 0x1a                      
  0x001EED9B  7f03                    jg       0x1eeda0                       
                                        ; XREF: 0x001EED79 (cond_jump), 0x001EED7D (cond_jump), 0x001EED81 (cond_jump), 0x001EED85 (cond_jump), 0x001EED91 (cond_jump)
  0x001EED9D  33f6                    xor      esi, esi                       
  0x001EED9F  46                      inc      esi                            
                                        ; XREF: 0x001EED6F (cond_jump), 0x001EED87 (jump), 0x001EED8C (cond_jump), 0x001EED96 (cond_jump), 0x001EED9B (cond_jump)
  0x001EEDA0  85c9                    test     ecx, ecx                       
  0x001EEDA2  750a                    jne      0x1eedae                       
  0x001EEDA4  8b07                    mov      eax, dword ptr [edi]           
  0x001EEDA6  57                      push     edi                            
  0x001EEDA7  ff5010                  call     dword ptr [eax + 0x10]         
  0x001EEDAA  85c0                    test     eax, eax                       
  0x001EEDAC  7c4b                    jl       0x1eedf9                       
                                        ; XREF: 0x001EEDA2 (cond_jump)
  0x001EEDAE  ff7570                  push     dword ptr [ebp + 0x70]         
  0x001EEDB1  c60598de1e0001          mov      byte ptr [0x1ede98], 1         
  0x001EEDB8  6a00                    push     0                              
  0x001EEDBA  e8b1e8feff              call     0x1dd670                       ; -> sub_001DD670
  0x001EEDBF  8bd6                    mov      edx, esi                       
  0x001EEDC1  b904030400              mov      ecx, 0x40304                   
  0x001EEDC6  c605badf1e0001          mov      byte ptr [0x1edfba], 1         
  0x001EEDCD  e8def0feff              call     0x1ddeb0                       ; -> sub_001DDEB0
  0x001EEDD2  6a1c                    push     0x1c                           
  0x001EEDD4  8d45c0                  lea      eax, [ebp - 0x40]              
  0x001EEDD7  50                      push     eax                            
  0x001EEDD8  6a04                    push     4                              
  0x001EEDDA  6a07                    push     7                              
  0x001EEDDC  89355cb01e00            mov      dword ptr [0x1eb05c], esi      
  0x001EEDE2  e8a906ffff              call     0x1df490                       ; -> sub_001DF490
  0x001EEDE7  837d7800                cmp      dword ptr [ebp + 0x78], 0      
  0x001EEDEB  750a                    jne      0x1eedf7                       
  0x001EEDED  8b07                    mov      eax, dword ptr [edi]           
  0x001EEDEF  57                      push     edi                            
  0x001EEDF0  ff501c                  call     dword ptr [eax + 0x1c]         
  0x001EEDF3  85c0                    test     eax, eax                       
  0x001EEDF5  7c02                    jl       0x1eedf9                       
                                        ; XREF: 0x001EEDEB (cond_jump)
  0x001EEDF7  33c0                    xor      eax, eax                       
                                        ; XREF: 0x001EEBAF (jump), 0x001EEDAC (cond_jump), 0x001EEDF5 (cond_jump)
  0x001EEDF9  5f                      pop      edi                            
  0x001EEDFA  5e                      pop      esi                            
                                        ; XREF: 0x001EEB5E (jump)
  0x001EEDFB  5b                      pop      ebx                            
  0x001EEDFC  83c564                  add      ebp, 0x64                      
  0x001EEDFF  c9                      leave                                   
  0x001EEE00  c21400                  ret      0x14                           
  0x001EEE03  cc                      int3                                    
  0x001EEE04  cc                      int3                                    
  0x001EEE05  cc                      int3                                    
  0x001EEE06  cc                      int3                                    
  0x001EEE07  cc                      int3                                    

; ============================================================
; Function: sub_001EEE08
; Start: 0x001EEE08  End: 0x001EEE4D  Size: 69 bytes
; Detection: cc_boundary (confidence: 0.85)
; ============================================================
sub_001EEE08:
  0x001EEE08  68011f002c              push     0x2c001f01                     
  0x001EEE0D  011f                    add      dword ptr [edi], ebx           
  0x001EEE0F  000c01                  add      byte ptr [ecx + eax], cl       
  0x001EEE12  1f                      pop      ds                             
  0x001EEE13  00e4                    add      ah, ah                         
  0x001EEE15  001f                    add      byte ptr [edi], bl             
  0x001EEE17  00c8                    add      al, cl                         
  0x001EEE19  001f                    add      byte ptr [edi], bl             
  0x001EEE1B  00a8001f0080            add      byte ptr [eax - 0x7fffe100], ch 
  0x001EEE21  001f                    add      byte ptr [edi], bl             
  0x001EEE23  006800                  add      byte ptr [eax], ch             
  0x001EEE26  1f                      pop      ds                             
  0x001EEE27  005000                  add      byte ptr [eax], dl             
  0x001EEE2A  1f                      pop      ds                             
  0x001EEE2B  003c00                  add      byte ptr [eax + eax], bh       
  0x001EEE2E  1f                      pop      ds                             
  0x001EEE2F  00fc                    add      ah, bh                         
  0x001EEE31  ff1e                    call     ptr [esi]                      
  0x001EEE33  00cc                    add      ah, cl                         
  0x001EEE35  ff1e                    call     ptr [esi]                      
  0x001EEE37  00b0ff1e008c            add      byte ptr [eax - 0x73ffe101], dh 
  0x001EEE3D  ff1e                    call     ptr [esi]                      
  0x001EEE3F  0054ff1e                add      byte ptr [edi + edi*8 + 0x1e], dl 
  0x001EEE43  001cff                  add      byte ptr [edi + edi*8], bl     
  0x001EEE46  1e                      push     ds                             
  0x001EEE47  0004ff                  add      byte ptr [edi + edi*8], al     
  0x001EEE4A  1e                      push     ds                             
  0x001EEE4B  00e4                    add      ah, ah                         
; end of function
  0x001EEE4E  1e                      push     ds                             
  0x001EEE4F  00b8fe1e0068            add      byte ptr [eax + 0x68001efe], bh 
  0x001EEE56  1e                      push     ds                             
  0x001EEE57  004cfe1e                add      byte ptr [esi + edi*8 + 0x1e], cl 
  0x001EEE5B  0020                    add      byte ptr [eax], ah             
  0x001EEE5E  1e                      push     ds                             
  0x001EEE5F  0004fe                  add      byte ptr [esi + edi*8], al     
  0x001EEE62  1e                      push     ds                             
  0x001EEE63  00dc                    add      ah, bl                         
  0x001EEE65  fd                      std                                     
  0x001EEE66  1e                      push     ds                             
  0x001EEE67  00b4fd1e008cfd          add      byte ptr [ebp + edi*8 - 0x273ffe2], dh 
  0x001EEE6E  1e                      push     ds                             
  0x001EEE6F  0078fd                  add      byte ptr [eax - 3], bh         
  0x001EEE72  1e                      push     ds                             
  0x001EEE73  0060fd                  add      byte ptr [eax - 3], ah         
  0x001EEE76  1e                      push     ds                             
  0x001EEE77  004cfd1e                add      byte ptr [ebp + edi*8 + 0x1e], cl 
  0x001EEE7B  0038                    add      byte ptr [eax], bh             
  0x001EEE7D  fd                      std                                     
  0x001EEE7E  1e                      push     ds                             
  0x001EEE7F  0024fd1e00fcfc          add      byte ptr [edi*8 - 0x303ffe2], ah 
  0x001EEE86  1e                      push     ds                             
  0x001EEE87  00e4                    add      ah, ah                         
  0x001EEE89  fc                      cld                                     
  0x001EEE8A  1e                      push     ds                             
  0x001EEE8B  00d0                    add      al, dl                         
  0x001EEE8D  fc                      cld                                     
  0x001EEE8E  1e                      push     ds                             
  0x001EEE8F  00acfc1e0094fc          add      byte ptr [esp + edi*8 - 0x36bffe2], ch 
  0x001EEE96  1e                      push     ds                             
  0x001EEE97  0064fc1e                add      byte ptr [esp + edi*8 + 0x1e], ah 
  0x001EEE9B  003cfc                  add      byte ptr [esp + edi*8], bh     
  0x001EEE9E  1e                      push     ds                             
  0x001EEE9F  0018                    add      byte ptr [eax], bl             
  0x001EEEA1  fc                      cld                                     
  0x001EEEA2  1e                      push     ds                             
  0x001EEEA3  00f4                    add      ah, dh                         
  0x001EEEA5  fb                      sti                                     
  0x001EEEA6  1e                      push     ds                             
  0x001EEEA7  00c4                    add      ah, al                         
  0x001EEEA9  fb                      sti                                     
  0x001EEEAA  1e                      push     ds                             
  0x001EEEAB  00b0fb1e0094            add      byte ptr [eax - 0x6bffe105], dh 
  0x001EEEB1  fb                      sti                                     
  0x001EEEB2  1e                      push     ds                             
  0x001EEEB3  0054fb1e                add      byte ptr [ebx + edi*8 + 0x1e], dl 
  0x001EEEB7  002cfb                  add      byte ptr [ebx + edi*8], ch     
  0x001EEEBA  1e                      push     ds                             
  0x001EEEBB  0004fb                  add      byte ptr [ebx + edi*8], al     
  0x001EEEBE  1e                      push     ds                             
  0x001EEEBF  00f0                    add      al, dh                         
  0x001EEEC1  fa                      cli                                     
  0x001EEEC2  1e                      push     ds                             
  0x001EEEC3  00c0                    add      al, al                         
  0x001EEEC5  fa                      cli                                     
  0x001EEEC6  1e                      push     ds                             
  0x001EEEC7  00a4fa1e007cfa          add      byte ptr [edx + edi*8 - 0x583ffe2], ah 
  0x001EEECE  1e                      push     ds                             
  0x001EEECF  0058fa                  add      byte ptr [eax - 6], bl         
  0x001EEED2  1e                      push     ds                             
  0x001EEED3  002cfa                  add      byte ptr [edx + edi*8], ch     
  0x001EEED6  1e                      push     ds                             
  0x001EEED7  0000                    add      byte ptr [eax], al             
  0x001EEED9  fa                      cli                                     
  0x001EEEDA  1e                      push     ds                             
  0x001EEEDB  00e0                    add      al, ah                         
  0x001EEEDD  f9                      stc                                     
  0x001EEEDE  1e                      push     ds                             
  0x001EEEDF  00b0f91e0088            add      byte ptr [eax - 0x77ffe107], dh 
  0x001EEEE5  f9                      stc                                     
  0x001EEEE6  1e                      push     ds                             
  0x001EEEE7  0060f9                  add      byte ptr [eax - 7], ah         
  0x001EEEEA  1e                      push     ds                             
  0x001EEEEB  0030                    add      byte ptr [eax], dh             
  0x001EEEED  f9                      stc                                     
  0x001EEEEE  1e                      push     ds                             
  0x001EEEEF  0000                    add      byte ptr [eax], al             
  0x001EEEF1  f9                      stc                                     
  0x001EEEF2  1e                      push     ds                             
  0x001EEEF3  00d4                    add      ah, dl                         
  0x001EEEF5  f8                      clc                                     
  0x001EEEF6  1e                      push     ds                             
  0x001EEEF7  00a4f81e0078f8          add      byte ptr [eax + edi*8 - 0x787ffe2], ah 
  0x001EEEFE  1e                      push     ds                             
  0x001EEEFF  0054f81e                add      byte ptr [eax + edi*8 + 0x1e], dl 
  0x001EEF03  0034f8                  add      byte ptr [eax + edi*8], dh     
  0x001EEF06  1e                      push     ds                             
  0x001EEF07  0014f8                  add      byte ptr [eax + edi*8], dl     
  0x001EEF0A  1e                      push     ds                             
  0x001EEF0B  00dc                    add      ah, bl                         
  0x001EEF0D  f71e                    neg      dword ptr [esi]                
  0x001EEF0F  00b0f71e0090            add      byte ptr [eax - 0x6fffe109], dh 
  0x001EEF15  f71e                    neg      dword ptr [esi]                
  0x001EEF17  006cf71e                add      byte ptr [edi + esi*8 + 0x1e], ch 
  0x001EEF1B  0044f71e                add      byte ptr [edi + esi*8 + 0x1e], al 
  0x001EEF1F  002cf7                  add      byte ptr [edi + esi*8], ch     
  0x001EEF22  1e                      push     ds                             
  0x001EEF23  0018                    add      byte ptr [eax], bl             
  0x001EEF25  f71e                    neg      dword ptr [esi]                
  0x001EEF27  00f4                    add      ah, dh                         
  0x001EEF29  f61e                    neg      byte ptr [esi]                 
  0x001EEF2B  00e4                    add      ah, ah                         
  0x001EEF2D  f61e                    neg      byte ptr [esi]                 
  0x001EEF2F  00a4f61e0060f6          add      byte ptr [esi + esi*8 - 0x99fffe2], ah 
  0x001EEF36  1e                      push     ds                             
  0x001EEF37  0030                    add      byte ptr [eax], dh             
  0x001EEF39  f61e                    neg      byte ptr [esi]                 
  0x001EEF3B  0004f6                  add      byte ptr [esi + esi*8], al     
  0x001EEF3E  1e                      push     ds                             
  0x001EEF3F  00dc                    add      ah, bl                         
  0x001EEF41  f5                      cmc                                     
  0x001EEF42  1e                      push     ds                             
  0x001EEF43  00c0                    add      al, al                         
  0x001EEF45  f5                      cmc                                     
  0x001EEF46  1e                      push     ds                             
  0x001EEF47  0094f51e0078f5          add      byte ptr [ebp + esi*8 - 0xa87ffe2], dl 
  0x001EEF4E  1e                      push     ds                             
  0x001EEF4F  0064f51e                add      byte ptr [ebp + esi*8 + 0x1e], ah 
  0x001EEF53  004cf51e                add      byte ptr [ebp + esi*8 + 0x1e], cl 
  0x001EEF57  003cf51e0014f5          add      byte ptr [esi*8 - 0xaebffe2], bh 
  0x001EEF5E  1e                      push     ds                             
  0x001EEF5F  00f0                    add      al, dh                         
  0x001EEF61  f4                      hlt                                     
  0x001EEF62  1e                      push     ds                             
  0x001EEF63  00b4f41e0088f4          add      byte ptr [esp + esi*8 - 0xb77ffe2], dh 
  0x001EEF6A  1e                      push     ds                             
  0x001EEF6B  0064f41e                add      byte ptr [esp + esi*8 + 0x1e], ah 
  0x001EEF6F  0040f4                  add      byte ptr [eax - 0xc], al       
  0x001EEF72  1e                      push     ds                             
  0x001EEF73  0024f4                  add      byte ptr [esp + esi*8], ah     
  0x001EEF76  1e                      push     ds                             
  0x001EEF77  00fc                    add      ah, bh                         
  0x001EEF79  f31e                    push     ds                             
  0x001EEF7B  00d8                    add      al, bl                         
  0x001EEF7D  f31e                    push     ds                             
  0x001EEF7F  00c0                    add      al, al                         
  0x001EEF81  f31e                    push     ds                             
  0x001EEF83  009cf31e0074f3          add      byte ptr [ebx + esi*8 - 0xc8bffe2], bl 
  0x001EEF8A  1e                      push     ds                             
  0x001EEF8B  006cf31e                add      byte ptr [ebx + esi*8 + 0x1e], ch 
  0x001EEF8F  0030                    add      byte ptr [eax], dh             
  0x001EEF91  f31e                    push     ds                             
  0x001EEF93  00f4                    add      ah, dh                         
  0x001EEF95  f21e                    push     ds                             
  0x001EEF97  00d4                    add      ah, dl                         
  0x001EEF99  f21e                    push     ds                             
  0x001EEF9B  00c4                    add      ah, al                         
  0x001EEF9D  f21e                    push     ds                             
  0x001EEF9F  00a4f21e0084f2          add      byte ptr [edx + esi*8 - 0xd7bffe2], ah 
  0x001EEFA6  1e                      push     ds                             
  0x001EEFA7  0064f21e                add      byte ptr [edx + esi*8 + 0x1e], ah 
  0x001EEFAB  0048f2                  add      byte ptr [eax - 0xe], cl       
  0x001EEFAE  1e                      push     ds                             
  0x001EEFAF  002cf2                  add      byte ptr [edx + esi*8], ch     
  0x001EEFB2  1e                      push     ds                             
  0x001EEFB3  00f8                    add      al, bh                         
  0x001EEFB5  f1                      int1                                    
  0x001EEFB6  1e                      push     ds                             
  0x001EEFB7  00e4                    add      ah, ah                         
  0x001EEFB9  f1                      int1                                    
  0x001EEFBA  1e                      push     ds                             
  0x001EEFBB  00cc                    add      ah, cl                         
  0x001EEFBD  f1                      int1                                    
  0x001EEFBE  1e                      push     ds                             
  0x001EEFBF  00a4f11e0060f1          add      byte ptr [ecx + esi*8 - 0xe9fffe2], ah 
  0x001EEFC6  1e                      push     ds                             
  0x001EEFC7  0020                    add      byte ptr [eax], ah             
  0x001EEFC9  f1                      int1                                    
  0x001EEFCA  1e                      push     ds                             
  0x001EEFCB  00ec                    add      ah, ch                         
  0x001EEFCE  1e                      push     ds                             
  0x001EEFCF  00c8                    add      al, cl                         
  0x001EEFD2  1e                      push     ds                             
  0x001EEFD3  0098f01e007c            add      byte ptr [eax + 0x7c001ef0], bl 
  0x001EEFDA  1e                      push     ds                             
  0x001EEFDB  0044f01e                add      byte ptr [eax + esi*8 + 0x1e], al 
  0x001EEFDF  0018                    add      byte ptr [eax], bl             
  0x001EEFE2  1e                      push     ds                             
  0x001EEFE3  00ec                    add      ah, ch                         
  0x001EEFE5  ef                      out      dx, eax                        
  0x001EEFE6  1e                      push     ds                             
  0x001EEFE7  0000                    add      byte ptr [eax], al             
  0x001EEFE9  0000                    add      byte ptr [eax], al             
  0x001EEFEB  004170                  add      byte ptr [ecx + 0x70], al      
  0x001EEFEE  706c                    jo       0x1ef05c                       
  0x001EEFF0  69636174696f6e          imul     esp, dword ptr [ebx + 0x61], 0x6e6f6974 
  0x001EEFF7  20747261                and      byte ptr [edx + esi*2 + 0x61], dh 
  0x001EEFFB  6e                      outsb    dx, byte ptr [esi]             
  0x001EEFFC  7366                    jae      0x1ef064                       
  0x001EEFFE  657272                  jb       0x1ef073                       
  0x001EF001  656420746f6f            and      byte ptr fs:[edi + ebp*2 + 0x6f], dh 
  0x001EF007  206d61                  and      byte ptr [ebp + 0x61], ch      
  0x001EF00A  6e                      outsb    dx, byte ptr [esi]             
  0x001EF00B  7920                    jns      0x1ef02d                       
  0x001EF00D  7363                    jae      0x1ef072                       
  0x001EF00F  61                      popal                                   
  0x001EF010  6e                      outsb    dx, byte ptr [esi]             
  0x001EF011  6c                      insb     byte ptr es:[edi], dx          
  0x001EF012  696e6573000049          imul     ebp, dword ptr [esi + 0x65], 0x49000073 
  0x001EF019  6e                      outsb    dx, byte ptr [esi]             
  0x001EF01A  7661                    jbe      0x1ef07d                       
  0x001EF01C  6c                      insb     byte ptr es:[edi], dx          
  0x001EF01D  696420534f532070        imul     esp, dword ptr [eax + 0x53], 0x7020534f 
  0x001EF025  61                      popal                                   
  0x001EF026  7261                    jb       0x1ef089                       
  0x001EF028  6d                      insd     dword ptr es:[edi], dx         
  0x001EF029  657465                  je       0x1ef091                       
  0x001EF02C  7273                    jb       0x1ef0a1                       
  0x001EF02E  20666f                  and      byte ptr [esi + 0x6f], ah      
  0x001EF031  7220                    jb       0x1ef053                       
  0x001EF033  7365                    jae      0x1ef09a                       
  0x001EF035  7175                    jno      0x1ef0ac                       
  0x001EF037  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF039  7469                    je       0x1ef0a4                       
  0x001EF03B  61                      popal                                   
  0x001EF03C  6c                      insb     byte ptr es:[edi], dx          
  0x001EF03D  204a50                  and      byte ptr [edx + 0x50], cl      
  0x001EF040  45                      inc      ebp                            
  0x001EF041  47                      inc      edi                            
  0x001EF042  0000                    add      byte ptr [eax], al             
  0x001EF044  43                      inc      ebx                            
  0x001EF045  6f                      outsd    dx, dword ptr [esi]            
  0x001EF046  7272                    jb       0x1ef0ba                       
  0x001EF048  7570                    jne      0x1ef0ba                       
  0x001EF04A  7420                    je       0x1ef06c                       
  0x001EF04C  4a                      dec      edx                            
  0x001EF04D  50                      push     eax                            
  0x001EF04E  45                      inc      ebp                            
  0x001EF04F  47                      inc      edi                            
  0x001EF050  20646174                and      byte ptr [ecx + 0x74], ah      
  0x001EF054  61                      popal                                   
  0x001EF055  3a20                    cmp      ah, byte ptr [eax]             
  0x001EF057  666f                    outsw    dx, word ptr [esi]             
  0x001EF059  756e                    jne      0x1ef0c9                       
  0x001EF05B  64206d61                and      byte ptr fs:[ebp + 0x61], ch   
  0x001EF05F  726b                    jb       0x1ef0cc                       
  0x001EF061  657220                  jb       0x1ef084                       
                                        ; XREF: 0x001EEFFC (cond_jump)
  0x001EF064  307825                  xor      byte ptr [eax + 0x25], bh      
  0x001EF067  3032                    xor      byte ptr [edx], dh             
  0x001EF069  7820                    js       0x1ef08b                       
  0x001EF06B  696e7374656164          imul     ebp, dword ptr [esi + 0x73], 0x64616574 
                                        ; XREF: 0x001EF00D (cond_jump)
  0x001EF072  206f66                  and      byte ptr [edi + 0x66], ch      
  0x001EF075  205253                  and      byte ptr [edx + 0x53], dl      
  0x001EF078  54                      push     esp                            
  0x001EF079  2564005072              and      eax, 0x72500064                
  0x001EF07E  656d                    insd     dword ptr es:[edi], dx         
  0x001EF080  61                      popal                                   
  0x001EF081  7475                    je       0x1ef0f8                       
  0x001EF083  7265                    jb       0x1ef0ea                       
  0x001EF085  20656e                  and      byte ptr [ebp + 0x6e], ah      
  0x001EF088  64206f66                and      byte ptr fs:[edi + 0x66], ch   
  0x001EF08C  204a50                  and      byte ptr [edx + 0x50], cl      
  0x001EF08F  45                      inc      ebp                            
  0x001EF090  47                      inc      edi                            
                                        ; XREF: 0x001EF029 (cond_jump)
  0x001EF091  206669                  and      byte ptr [esi + 0x69], ah      
  0x001EF094  6c                      insb     byte ptr es:[edi], dx          
  0x001EF095  650000                  add      byte ptr gs:[eax], al          
  0x001EF098  57                      push     edi                            
  0x001EF099  61                      popal                                   
                                        ; XREF: 0x001EF033 (cond_jump)
  0x001EF09A  726e                    jb       0x1ef10a                       
  0x001EF09C  696e673a20756e          imul     ebp, dword ptr [esi + 0x67], 0x6e75203a 
  0x001EF0A3  6b6e6f77                imul     ebp, dword ptr [esi + 0x6f], 0x77 
  0x001EF0A7  6e                      outsb    dx, byte ptr [esi]             
  0x001EF0A8  204a46                  and      byte ptr [edx + 0x46], cl      
  0x001EF0AB  49                      dec      ecx                            
                                        ; XREF: 0x001EF035 (cond_jump)
  0x001EF0AC  46                      inc      esi                            
  0x001EF0AD  207265                  and      byte ptr [edx + 0x65], dh      
  0x001EF0B0  7669                    jbe      0x1ef11b                       
  0x001EF0B2  7369                    jae      0x1ef11d                       
  0x001EF0B4  6f                      outsd    dx, dword ptr [esi]            
  0x001EF0B5  6e                      outsb    dx, byte ptr [esi]             
  0x001EF0B6  206e75                  and      byte ptr [esi + 0x75], ch      
  0x001EF0B9  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EF046 (cond_jump), 0x001EF048 (cond_jump)
  0x001EF0BA  626572                  bound    esp, qword ptr [ebp + 0x72]    
  0x001EF0BD  2025642e2530            and      byte ptr [0x30252e64], ah      
  0x001EF0C3  32640000                xor      ah, byte ptr [eax + eax]       
  0x001EF0C7  00436f                  add      byte ptr [ebx + 0x6f], al      
  0x001EF0CA  7272                    jb       0x1ef13e                       
                                        ; XREF: 0x001EF05F (cond_jump)
  0x001EF0CC  7570                    jne      0x1ef13e                       
  0x001EF0CE  7420                    je       0x1ef0f0                       
  0x001EF0D0  4a                      dec      edx                            
  0x001EF0D1  50                      push     eax                            
  0x001EF0D2  45                      inc      ebp                            
  0x001EF0D3  47                      inc      edi                            
  0x001EF0D4  20646174                and      byte ptr [ecx + 0x74], ah      
  0x001EF0D8  61                      popal                                   
  0x001EF0D9  3a20                    cmp      ah, byte ptr [eax]             
  0x001EF0DB  626164                  bound    esp, qword ptr [ecx + 0x64]    
  0x001EF0DE  204875                  and      byte ptr [eax + 0x75], cl      
  0x001EF0E1  66666d                  insw     word ptr es:[edi], dx          
  0x001EF0E4  61                      popal                                   
  0x001EF0E5  6e                      outsb    dx, byte ptr [esi]             
  0x001EF0E6  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EF0E9  646500436f              add      byte ptr gs:[ebx + 0x6f], al   
  0x001EF0EE  7272                    jb       0x1ef162                       
                                        ; XREF: 0x001EF0CE (cond_jump)
  0x001EF0F0  7570                    jne      0x1ef162                       
  0x001EF0F2  7420                    je       0x1ef114                       
  0x001EF0F4  4a                      dec      edx                            
  0x001EF0F5  50                      push     eax                            
  0x001EF0F6  45                      inc      ebp                            
  0x001EF0F7  47                      inc      edi                            
                                        ; XREF: 0x001EF081 (cond_jump)
  0x001EF0F8  20646174                and      byte ptr [ecx + 0x74], ah      
  0x001EF0FC  61                      popal                                   
  0x001EF0FD  3a20                    cmp      ah, byte ptr [eax]             
  0x001EF0FF  7072                    jo       0x1ef173                       
  0x001EF101  656d                    insd     dword ptr es:[edi], dx         
  0x001EF103  61                      popal                                   
  0x001EF104  7475                    je       0x1ef17b                       
  0x001EF106  7265                    jb       0x1ef16d                       
  0x001EF108  20656e                  and      byte ptr [ebp + 0x6e], ah      
  0x001EF10B  64206f66                and      byte ptr fs:[edi + 0x66], ch   
  0x001EF10F  20646174                and      byte ptr [ecx + 0x74], ah      
  0x001EF113  61                      popal                                   
                                        ; XREF: 0x001EF0F2 (cond_jump)
  0x001EF114  207365                  and      byte ptr [ebx + 0x65], dh      
  0x001EF117  676d                    insd     dword ptr es:[di], dx          
  0x001EF119  656e                    outsb    dx, byte ptr gs:[esi]          
                                        ; XREF: 0x001EF0B0 (cond_jump)
  0x001EF11B  7400                    je       0x1ef11d                       
                                        ; XREF: 0x001EF0B2 (cond_jump), 0x001EF11B (cond_jump)
  0x001EF11D  0000                    add      byte ptr [eax], al             
  0x001EF11F  00436f                  add      byte ptr [ebx + 0x6f], al      
  0x001EF122  7272                    jb       0x1ef196                       
  0x001EF124  7570                    jne      0x1ef196                       
  0x001EF126  7420                    je       0x1ef148                       
  0x001EF128  4a                      dec      edx                            
  0x001EF129  50                      push     eax                            
  0x001EF12A  45                      inc      ebp                            
  0x001EF12B  47                      inc      edi                            
  0x001EF12C  20646174                and      byte ptr [ecx + 0x74], ah      
  0x001EF130  61                      popal                                   
  0x001EF131  3a20                    cmp      ah, byte ptr [eax]             
  0x001EF133  2575206578              and      eax, 0x78652075                
  0x001EF138  7472                    je       0x1ef1ac                       
  0x001EF13A  61                      popal                                   
  0x001EF13B  6e                      outsb    dx, byte ptr [esi]             
  0x001EF13C  656f                    outsd    dx, dword ptr gs:[esi]         
                                        ; XREF: 0x001EF0CA (cond_jump), 0x001EF0CC (cond_jump)
  0x001EF13E  7573                    jne      0x1ef1b3                       
  0x001EF140  206279                  and      byte ptr [edx + 0x79], ah      
  0x001EF143  7465                    je       0x1ef1aa                       
  0x001EF145  7320                    jae      0x1ef167                       
  0x001EF147  626566                  bound    esp, qword ptr [ebp + 0x66]    
  0x001EF14A  6f                      outsd    dx, dword ptr [esi]            
  0x001EF14B  7265                    jb       0x1ef1b2                       
  0x001EF14D  206d61                  and      byte ptr [ebp + 0x61], ch      
  0x001EF150  726b                    jb       0x1ef1bd                       
  0x001EF152  657220                  jb       0x1ef175                       
  0x001EF155  307825                  xor      byte ptr [eax + 0x25], bh      
  0x001EF158  3032                    xor      byte ptr [edx], dh             
  0x001EF15A  7800                    js       0x1ef15c                       
                                        ; XREF: 0x001EF15A (cond_jump)
  0x001EF15C  0000                    add      byte ptr [eax], al             
  0x001EF15E  0000                    add      byte ptr [eax], al             
  0x001EF160  49                      dec      ecx                            
  0x001EF161  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EF0EE (cond_jump), 0x001EF0F0 (cond_jump)
  0x001EF162  636f6e                  arpl     word ptr [edi + 0x6e], bp      
  0x001EF165  7369                    jae      0x1ef1d0                       
                                        ; XREF: 0x001EF145 (cond_jump)
  0x001EF167  7374                    jae      0x1ef1dd                       
  0x001EF169  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF16B  7420                    je       0x1ef18d                       
                                        ; XREF: 0x001EF106 (cond_jump)
  0x001EF16D  7072                    jo       0x1ef1e1                       
  0x001EF16F  6f                      outsd    dx, dword ptr [esi]            
  0x001EF170  677265                  jb       0x1ef1d8                       
                                        ; XREF: 0x001EF0FF (cond_jump)
  0x001EF173  7373                    jae      0x1ef1e8                       
                                        ; XREF: 0x001EF152 (cond_jump)
  0x001EF175  696f6e20736571          imul     ebp, dword ptr [edi + 0x6e], 0x71657320 
  0x001EF17C  7565                    jne      0x1ef1e3                       
  0x001EF17E  6e                      outsb    dx, byte ptr [esi]             
  0x001EF17F  636520                  arpl     word ptr [ebp + 0x20], sp      
  0x001EF182  666f                    outsw    dx, word ptr [esi]             
  0x001EF184  7220                    jb       0x1ef1a6                       
  0x001EF186  636f6d                  arpl     word ptr [edi + 0x6d], bp      
  0x001EF189  706f                    jo       0x1ef1fa                       
  0x001EF18B  6e                      outsb    dx, byte ptr [esi]             
  0x001EF18C  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF18E  7420                    je       0x1ef1b0                       
  0x001EF190  256420636f              and      eax, 0x6f632064                
  0x001EF195  656666696369656e        imul     sp, word ptr gs:[ebx + 0x69], 0x6e65 
  0x001EF19D  7420                    je       0x1ef1bf                       
  0x001EF19F  2564000000              and      eax, 0x64                      
  0x001EF1A4  55                      push     ebp                            
  0x001EF1A5  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EF184 (cond_jump)
  0x001EF1A6  6b6e6f77                imul     ebp, dword ptr [esi + 0x6f], 0x77 
                                        ; XREF: 0x001EF143 (cond_jump)
  0x001EF1AA  6e                      outsb    dx, byte ptr [esi]             
  0x001EF1AB  204164                  and      byte ptr [ecx + 0x64], al      
  0x001EF1AE  6f                      outsd    dx, dword ptr [esi]            
  0x001EF1AF  626520                  bound    esp, qword ptr [ebp + 0x20]    
                                        ; XREF: 0x001EF14B (cond_jump)
  0x001EF1B2  636f6c                  arpl     word ptr [edi + 0x6c], bp      
  0x001EF1B5  6f                      outsd    dx, dword ptr [esi]            
  0x001EF1B6  7220                    jb       0x1ef1d8                       
  0x001EF1B8  7472                    je       0x1ef22c                       
  0x001EF1BA  61                      popal                                   
  0x001EF1BB  6e                      outsb    dx, byte ptr [esi]             
  0x001EF1BC  7366                    jae      0x1ef224                       
  0x001EF1BE  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EF19D (cond_jump)
  0x001EF1BF  726d                    jb       0x1ef22e                       
  0x001EF1C1  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EF1C4  6465202564000000        and      byte ptr gs:[0x64], ah         
  0x001EF1CC  4f                      dec      edi                            
  0x001EF1CD  62746169                bound    esi, qword ptr [ecx + 0x69]    
  0x001EF1D1  6e                      outsb    dx, byte ptr [esi]             
  0x001EF1D2  656420584d              and      byte ptr fs:[eax + 0x4d], bl   
  0x001EF1D7  53                      push     ebx                            
                                        ; XREF: 0x001EF170 (cond_jump), 0x001EF1B6 (cond_jump)
  0x001EF1D8  206861                  and      byte ptr [eax + 0x61], ch      
  0x001EF1DB  6e                      outsb    dx, byte ptr [esi]             
  0x001EF1DC  646c                    insb     byte ptr es:[edi], dx          
  0x001EF1DE  65202575000046          and      byte ptr gs:[0x46000075], ah   
  0x001EF1E5  7265                    jb       0x1ef24c                       
  0x001EF1E7  656420584d              and      byte ptr fs:[eax + 0x4d], bl   
  0x001EF1EC  53                      push     ebx                            
  0x001EF1ED  206861                  and      byte ptr [eax + 0x61], ch      
  0x001EF1F0  6e                      outsb    dx, byte ptr [esi]             
  0x001EF1F1  646c                    insb     byte ptr es:[edi], dx          
  0x001EF1F3  6520257500556e          and      byte ptr gs:[0x6e550075], ah   
                                        ; XREF: 0x001EF189 (cond_jump)
  0x001EF1FA  7265                    jb       0x1ef261                       
  0x001EF1FC  636f67                  arpl     word ptr [edi + 0x67], bp      
  0x001EF1FF  6e                      outsb    dx, byte ptr [esi]             
  0x001EF200  697a656420636f          imul     edi, dword ptr [edx + 0x65], 0x6f632064 
  0x001EF207  6d                      insd     dword ptr es:[edi], dx         
  0x001EF208  706f                    jo       0x1ef279                       
  0x001EF20A  6e                      outsb    dx, byte ptr [esi]             
  0x001EF20B  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF20D  7420                    je       0x1ef22f                       
  0x001EF20F  49                      dec      ecx                            
  0x001EF210  44                      inc      esp                            
  0x001EF211  7320                    jae      0x1ef233                       
  0x001EF213  2564202564              and      eax, 0x64252064                
  0x001EF218  2025642c2061            and      byte ptr [0x61202c64], ah      
  0x001EF21E  7373                    jae      0x1ef293                       
  0x001EF220  756d                    jne      0x1ef28f                       
  0x001EF222  696e6720594362          imul     ebp, dword ptr [esi + 0x67], 0x62435920 
  0x001EF229  43                      inc      ebx                            
  0x001EF22A  7200                    jb       0x1ef22c                       
                                        ; XREF: 0x001EF1B8 (cond_jump), 0x001EF22A (cond_jump)
  0x001EF22C  4f                      dec      edi                            
  0x001EF22D  7065                    jo       0x1ef294                       
                                        ; XREF: 0x001EF20D (cond_jump)
  0x001EF22F  6e                      outsb    dx, byte ptr [esi]             
  0x001EF230  65642074656d            and      byte ptr fs:[ebp + 0x6d], dh   
  0x001EF236  706f                    jo       0x1ef2a7                       
  0x001EF238  7261                    jb       0x1ef29b                       
  0x001EF23A  7279                    jb       0x1ef2b5                       
  0x001EF23C  206669                  and      byte ptr [esi + 0x69], ah      
  0x001EF23F  6c                      insb     byte ptr es:[edi], dx          
  0x001EF240  65202573000000          and      byte ptr gs:[0x73], ah         
  0x001EF247  00436c                  add      byte ptr [ebx + 0x6c], al      
  0x001EF24A  6f                      outsd    dx, dword ptr [esi]            
  0x001EF24B  7365                    jae      0x1ef2b2                       
  0x001EF24D  642074656d              and      byte ptr fs:[ebp + 0x6d], dh   
  0x001EF252  706f                    jo       0x1ef2c3                       
  0x001EF254  7261                    jb       0x1ef2b7                       
  0x001EF256  7279                    jb       0x1ef2d1                       
  0x001EF258  206669                  and      byte ptr [esi + 0x69], ah      
  0x001EF25B  6c                      insb     byte ptr es:[edi], dx          
  0x001EF25C  65202573000000          and      byte ptr gs:[0x73], ah         
  0x001EF263  0020                    add      byte ptr [eax], ah             
  0x001EF265  205373                  and      byte ptr [ebx + 0x73], dl      
  0x001EF268  3d25642c20              cmp      eax, 0x202c6425                
  0x001EF26D  53                      push     ebx                            
  0x001EF26E  653d25642c20            cmp      eax, 0x202c6425                
  0x001EF274  41                      inc      ecx                            
  0x001EF275  683d25642c              push     0x2c64253d                     
  0x001EF27A  20416c                  and      byte ptr [ecx + 0x6c], al      
  0x001EF27D  3d25640000              cmp      eax, 0x6425                    
  0x001EF282  0000                    add      byte ptr [eax], al             
  0x001EF284  2020                    and      byte ptr [eax], ah             
  0x001EF286  2020                    and      byte ptr [eax], ah             
  0x001EF288  43                      inc      ebx                            
  0x001EF289  6f                      outsd    dx, dword ptr [esi]            
  0x001EF28A  6d                      insd     dword ptr es:[edi], dx         
  0x001EF28B  706f                    jo       0x1ef2fc                       
  0x001EF28D  6e                      outsb    dx, byte ptr [esi]             
  0x001EF28E  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF290  7420                    je       0x1ef2b2                       
  0x001EF292  25643a2064              and      eax, 0x64203a64                
  0x001EF297  633d25642061            arpl     word ptr [0x61206425], di      
  0x001EF29D  633d25640000            arpl     word ptr [0x6425], di          
  0x001EF2A3  005374                  add      byte ptr [ebx + 0x74], dl      
  0x001EF2A6  61                      popal                                   
                                        ; XREF: 0x001EF236 (cond_jump)
  0x001EF2A7  7274                    jb       0x1ef31d                       
  0x001EF2A9  204f66                  and      byte ptr [edi + 0x66], cl      
  0x001EF2AC  205363                  and      byte ptr [ebx + 0x63], dl      
  0x001EF2AF  61                      popal                                   
  0x001EF2B0  6e                      outsb    dx, byte ptr [esi]             
  0x001EF2B1  3a20                    cmp      ah, byte ptr [eax]             
  0x001EF2B3  256420636f              and      eax, 0x6f632064                
  0x001EF2B8  6d                      insd     dword ptr es:[edi], dx         
  0x001EF2B9  706f                    jo       0x1ef32a                       
  0x001EF2BB  6e                      outsb    dx, byte ptr [esi]             
  0x001EF2BC  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF2BE  7473                    je       0x1ef333                       
  0x001EF2C0  0000                    add      byte ptr [eax], al             
  0x001EF2C2  0000                    add      byte ptr [eax], al             
  0x001EF2C4  53                      push     ebx                            
  0x001EF2C5  7461                    je       0x1ef328                       
  0x001EF2C7  7274                    jb       0x1ef33d                       
  0x001EF2C9  206f66                  and      byte ptr [edi + 0x66], ch      
  0x001EF2CC  20496d                  and      byte ptr [ecx + 0x6d], cl      
  0x001EF2CF  61                      popal                                   
  0x001EF2D0  67650000                add      byte ptr gs:[bx + si], al      
  0x001EF2D4  2020                    and      byte ptr [eax], ah             
  0x001EF2D6  2020                    and      byte ptr [eax], ah             
  0x001EF2D8  43                      inc      ebx                            
  0x001EF2D9  6f                      outsd    dx, dword ptr [esi]            
  0x001EF2DA  6d                      insd     dword ptr es:[edi], dx         
  0x001EF2DB  706f                    jo       0x1ef34c                       
  0x001EF2DD  6e                      outsb    dx, byte ptr [esi]             
  0x001EF2DE  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF2E0  7420                    je       0x1ef302                       
  0x001EF2E2  25643a2025              and      eax, 0x25203a64                
  0x001EF2E7  646878256476            push     0x76642578                     
  0x001EF2ED  20713d                  and      byte ptr [ecx + 0x3d], dh      
  0x001EF2F0  2564000053              and      eax, 0x53000064                
  0x001EF2F5  7461                    je       0x1ef358                       
  0x001EF2F7  7274                    jb       0x1ef36d                       
  0x001EF2F9  204f66                  and      byte ptr [edi + 0x66], cl      
                                        ; XREF: 0x001EF28B (cond_jump)
  0x001EF2FC  204672                  and      byte ptr [esi + 0x72], al      
  0x001EF2FF  61                      popal                                   
  0x001EF300  6d                      insd     dword ptr es:[edi], dx         
  0x001EF301  652030                  and      byte ptr gs:[eax], dh          
  0x001EF304  7825                    js       0x1ef32b                       
  0x001EF306  3032                    xor      byte ptr [edx], dh             
  0x001EF308  783a                    js       0x1ef344                       
  0x001EF30A  207769                  and      byte ptr [edi + 0x69], dh      
  0x001EF30D  647468                  je       0x1ef378                       
  0x001EF310  3d25752c20              cmp      eax, 0x202c7525                
  0x001EF315  6865696768              push     0x68676965                     
  0x001EF31A  743d                    je       0x1ef359                       
  0x001EF31C  25752c2063              and      eax, 0x63202c75                
  0x001EF321  6f                      outsd    dx, dword ptr [esi]            
  0x001EF322  6d                      insd     dword ptr es:[edi], dx         
  0x001EF323  706f                    jo       0x1ef394                       
  0x001EF325  6e                      outsb    dx, byte ptr [esi]             
  0x001EF326  656e                    outsb    dx, byte ptr gs:[esi]          
                                        ; XREF: 0x001EF2C5 (cond_jump)
  0x001EF328  7473                    je       0x1ef39d                       
                                        ; XREF: 0x001EF2B9 (cond_jump)
  0x001EF32A  3d25640000              cmp      eax, 0x6425                    
  0x001EF32F  00536d                  add      byte ptr [ebx + 0x6d], dl      
  0x001EF332  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EF2BE (cond_jump)
  0x001EF333  6f                      outsd    dx, dword ptr [esi]            
  0x001EF334  7468                    je       0x1ef39e                       
  0x001EF336  696e67206e6f74          imul     ebp, dword ptr [esi + 0x67], 0x746f6e20 
                                        ; XREF: 0x001EF2C7 (cond_jump)
  0x001EF33D  207375                  and      byte ptr [ebx + 0x75], dh      
  0x001EF340  7070                    jo       0x1ef3b2                       
  0x001EF342  6f                      outsd    dx, dword ptr [esi]            
  0x001EF343  7274                    jb       0x1ef3b9                       
  0x001EF345  6564207769              and      byte ptr fs:[edi + 0x69], dh   
  0x001EF34A  7468                    je       0x1ef3b4                       
                                        ; XREF: 0x001EF2DB (cond_jump)
  0x001EF34C  206e6f                  and      byte ptr [esi + 0x6f], ch      
  0x001EF34F  6e                      outsb    dx, byte ptr [esi]             
  0x001EF350  7374                    jae      0x1ef3c6                       
  0x001EF352  61                      popal                                   
  0x001EF353  6e                      outsb    dx, byte ptr [esi]             
  0x001EF354  6461                    popal                                   
  0x001EF356  7264                    jb       0x1ef3bc                       
                                        ; XREF: 0x001EF2F5 (cond_jump)
  0x001EF358  207361                  and      byte ptr [ebx + 0x61], dh      
  0x001EF35B  6d                      insd     dword ptr es:[edi], dx         
  0x001EF35C  706c                    jo       0x1ef3ca                       
  0x001EF35E  696e6720726174          imul     ebp, dword ptr [esi + 0x67], 0x74617220 
  0x001EF365  696f7300000000          imul     ebp, dword ptr [edi + 0x73], 0 
  0x001EF36C  52                      push     edx                            
                                        ; XREF: 0x001EF2F7 (cond_jump)
  0x001EF36D  53                      push     ebx                            
  0x001EF36E  54                      push     esp                            
  0x001EF36F  2564000000              and      eax, 0x64                      
  0x001EF374  41                      inc      ecx                            
  0x001EF375  7420                    je       0x1ef397                       
  0x001EF377  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EF30D (cond_jump)
  0x001EF378  61                      popal                                   
  0x001EF379  726b                    jb       0x1ef3e6                       
  0x001EF37B  657220                  jb       0x1ef39e                       
  0x001EF37E  307825                  xor      byte ptr [eax + 0x25], bh      
  0x001EF381  3032                    xor      byte ptr [edx], dh             
  0x001EF383  782c                    js       0x1ef3b1                       
  0x001EF385  207265                  and      byte ptr [edx + 0x65], dh      
  0x001EF388  636f76                  arpl     word ptr [edi + 0x76], bp      
  0x001EF38B  657279                  jb       0x1ef407                       
  0x001EF38E  206163                  and      byte ptr [ecx + 0x63], ah      
  0x001EF391  7469                    je       0x1ef3fc                       
  0x001EF393  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EF323 (cond_jump)
  0x001EF394  6e                      outsb    dx, byte ptr [esi]             
  0x001EF395  202564000000            and      byte ptr [0x64], ah            
  0x001EF39B  005365                  add      byte ptr [ebx + 0x65], dl      
                                        ; XREF: 0x001EF334 (cond_jump), 0x001EF37B (cond_jump)
  0x001EF39E  6c                      insb     byte ptr es:[edi], dx          
  0x001EF39F  6563746564              arpl     word ptr gs:[ebp + 0x64], si   
  0x001EF3A4  20256420636f            and      byte ptr [0x6f632064], ah      
  0x001EF3AA  6c                      insb     byte ptr es:[edi], dx          
  0x001EF3AB  6f                      outsd    dx, dword ptr [esi]            
  0x001EF3AC  7273                    jb       0x1ef421                       
  0x001EF3AE  20666f                  and      byte ptr [esi + 0x6f], ah      
                                        ; XREF: 0x001EF383 (cond_jump)
  0x001EF3B1  7220                    jb       0x1ef3d3                       
  0x001EF3B3  7175                    jno      0x1ef42a                       
  0x001EF3B5  61                      popal                                   
  0x001EF3B6  6e                      outsb    dx, byte ptr [esi]             
  0x001EF3B7  7469                    je       0x1ef422                       
                                        ; XREF: 0x001EF343 (cond_jump)
  0x001EF3B9  7a61                    jp       0x1ef41c                       
  0x001EF3BB  7469                    je       0x1ef426                       
  0x001EF3BD  6f                      outsd    dx, dword ptr [esi]            
  0x001EF3BE  6e                      outsb    dx, byte ptr [esi]             
  0x001EF3BF  005175                  add      byte ptr [ecx + 0x75], dl      
  0x001EF3C2  61                      popal                                   
  0x001EF3C3  6e                      outsb    dx, byte ptr [esi]             
  0x001EF3C4  7469                    je       0x1ef42f                       
                                        ; XREF: 0x001EF350 (cond_jump)
  0x001EF3C6  7a69                    jp       0x1ef431                       
  0x001EF3C8  6e                      outsb    dx, byte ptr [esi]             
  0x001EF3C9  6720746f                and      byte ptr [si + 0x6f], dh       
  0x001EF3CD  20256420636f            and      byte ptr [0x6f632064], ah      
                                        ; XREF: 0x001EF3B1 (cond_jump)
  0x001EF3D3  6c                      insb     byte ptr es:[edi], dx          
  0x001EF3D4  6f                      outsd    dx, dword ptr [esi]            
  0x001EF3D5  7273                    jb       0x1ef44a                       
  0x001EF3D7  005175                  add      byte ptr [ecx + 0x75], dl      
  0x001EF3DA  61                      popal                                   
  0x001EF3DB  6e                      outsb    dx, byte ptr [esi]             
  0x001EF3DC  7469                    je       0x1ef447                       
  0x001EF3DE  7a69                    jp       0x1ef449                       
  0x001EF3E0  6e                      outsb    dx, byte ptr [esi]             
  0x001EF3E1  6720746f                and      byte ptr [si + 0x6f], dh       
  0x001EF3E5  202564203d20            and      byte ptr [0x203d2064], ah      
  0x001EF3EB  25642a2564              and      eax, 0x64252a64                
  0x001EF3F0  2a256420636f            sub      ah, byte ptr [0x6f632064]      
  0x001EF3F6  6c                      insb     byte ptr es:[edi], dx          
  0x001EF3F7  6f                      outsd    dx, dword ptr [esi]            
  0x001EF3F8  7273                    jb       0x1ef46d                       
  0x001EF3FA  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001EF391 (cond_jump)
  0x001EF3FC  2020                    and      byte ptr [eax], ah             
  0x001EF3FE  2020                    and      byte ptr [eax], ah             
  0x001EF400  2020                    and      byte ptr [eax], ah             
  0x001EF402  2020                    and      byte ptr [eax], ah             
  0x001EF404  2534752025              and      eax, 0x25207534                
  0x001EF409  3475                    xor      al, 0x75                       
  0x001EF40B  202534752025            and      byte ptr [0x25207534], ah      
  0x001EF411  3475                    xor      al, 0x75                       
  0x001EF413  202534752025            and      byte ptr [0x25207534], ah      
  0x001EF419  3475                    xor      al, 0x75                       
  0x001EF41B  202534752025            and      byte ptr [0x25207534], ah      
                                        ; XREF: 0x001EF3AC (cond_jump)
  0x001EF421  3475                    xor      al, 0x75                       
  0x001EF423  00556e                  add      byte ptr [ebp + 0x6e], dl      
                                        ; XREF: 0x001EF3BB (cond_jump)
  0x001EF426  657870                  js       0x1ef499                       
  0x001EF429  6563746564              arpl     word ptr gs:[ebp + 0x64], si   
  0x001EF42E  206d61                  and      byte ptr [ebp + 0x61], ch      
                                        ; XREF: 0x001EF3C6 (cond_jump)
  0x001EF431  726b                    jb       0x1ef49e                       
  0x001EF433  657220                  jb       0x1ef456                       
  0x001EF436  307825                  xor      byte ptr [eax + 0x25], bh      
  0x001EF439  3032                    xor      byte ptr [edx], dh             
  0x001EF43B  7800                    js       0x1ef43d                       
                                        ; XREF: 0x001EF43B (cond_jump)
  0x001EF43D  0000                    add      byte ptr [eax], al             
  0x001EF43F  00536b                  add      byte ptr [ebx + 0x6b], dl      
  0x001EF442  697070696e6720          imul     esi, dword ptr [eax + 0x70], 0x20676e69 
                                        ; XREF: 0x001EF3DE (cond_jump)
  0x001EF449  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EF3D5 (cond_jump)
  0x001EF44A  61                      popal                                   
  0x001EF44B  726b                    jb       0x1ef4b8                       
  0x001EF44D  657220                  jb       0x1ef470                       
  0x001EF450  307825                  xor      byte ptr [eax + 0x25], bh      
  0x001EF453  3032                    xor      byte ptr [edx], dh             
  0x001EF455  782c                    js       0x1ef483                       
  0x001EF457  206c656e                and      byte ptr [ebp + 0x6e], ch      
  0x001EF45B  677468                  je       0x1ef4c6                       
  0x001EF45E  202575000000            and      byte ptr [0x75], ah            
  0x001EF464  2020                    and      byte ptr [eax], ah             
  0x001EF466  2020                    and      byte ptr [eax], ah             
  0x001EF468  7769                    ja       0x1ef4d3                       
  0x001EF46A  7468                    je       0x1ef4d4                       
  0x001EF46C  202564207820            and      byte ptr [0x20782064], ah      
  0x001EF472  2564207468              and      eax, 0x68742064                
  0x001EF477  756d                    jne      0x1ef4e6                       
  0x001EF479  626e61                  bound    ebp, qword ptr [esi + 0x61]    
  0x001EF47C  696c20696d616765        imul     ebp, dword ptr [eax + 0x69], 0x6567616d 
  0x001EF484  0000                    add      byte ptr [eax], al             
  0x001EF486  0000                    add      byte ptr [eax], al             
  0x001EF488  55                      push     ebp                            
  0x001EF489  6e                      outsb    dx, byte ptr [esi]             
  0x001EF48A  6b6e6f77                imul     ebp, dword ptr [esi + 0x6f], 0x77 
  0x001EF48E  6e                      outsb    dx, byte ptr [esi]             
  0x001EF48F  204a46                  and      byte ptr [edx + 0x46], cl      
  0x001EF492  49                      dec      ecx                            
  0x001EF493  46                      inc      esi                            
  0x001EF494  206d69                  and      byte ptr [ebp + 0x69], ch      
  0x001EF497  6e                      outsb    dx, byte ptr [esi]             
  0x001EF498  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EF426 (cond_jump)
  0x001EF499  7220                    jb       0x1ef4bb                       
  0x001EF49B  7265                    jb       0x1ef502                       
  0x001EF49D  7669                    jbe      0x1ef508                       
  0x001EF49F  7369                    jae      0x1ef50a                       
  0x001EF4A1  6f                      outsd    dx, dword ptr [esi]            
  0x001EF4A2  6e                      outsb    dx, byte ptr [esi]             
  0x001EF4A3  206e75                  and      byte ptr [esi + 0x75], ch      
  0x001EF4A6  6d                      insd     dword ptr es:[edi], dx         
  0x001EF4A7  626572                  bound    esp, qword ptr [ebp + 0x72]    
  0x001EF4AA  2025642e2530            and      byte ptr [0x30252e64], ah      
  0x001EF4B0  32640000                xor      ah, byte ptr [eax + eax]       
  0x001EF4B4  57                      push     edi                            
  0x001EF4B5  61                      popal                                   
  0x001EF4B6  726e                    jb       0x1ef526                       
                                        ; XREF: 0x001EF44B (cond_jump)
  0x001EF4B8  696e673a207468          imul     ebp, dword ptr [esi + 0x67], 0x6874203a 
  0x001EF4BF  756d                    jne      0x1ef52e                       
  0x001EF4C1  626e61                  bound    ebp, qword ptr [esi + 0x61]    
  0x001EF4C4  696c20696d616765        imul     ebp, dword ptr [eax + 0x69], 0x6567616d 
  0x001EF4CC  207369                  and      byte ptr [ebx + 0x69], dh      
  0x001EF4CF  7a65                    jp       0x1ef536                       
  0x001EF4D1  20646f65                and      byte ptr [edi + ebp*2 + 0x65], ah 
  0x001EF4D5  7320                    jae      0x1ef4f7                       
  0x001EF4D7  6e                      outsb    dx, byte ptr [esi]             
  0x001EF4D8  6f                      outsd    dx, dword ptr [esi]            
  0x001EF4D9  7420                    je       0x1ef4fb                       
  0x001EF4DB  6d                      insd     dword ptr es:[edi], dx         
  0x001EF4DC  61                      popal                                   
  0x001EF4DD  7463                    je       0x1ef542                       
  0x001EF4DF  6820646174              push     0x74616420                     
  0x001EF4E4  61                      popal                                   
  0x001EF4E5  206c656e                and      byte ptr [ebp + 0x6e], ch      
  0x001EF4E9  677468                  je       0x1ef554                       
  0x001EF4EC  202575004a46            and      byte ptr [0x464a0075], ah      
  0x001EF4F2  49                      dec      ecx                            
  0x001EF4F3  46                      inc      esi                            
  0x001EF4F4  204150                  and      byte ptr [ecx + 0x50], al      
                                        ; XREF: 0x001EF4D5 (cond_jump)
  0x001EF4F7  50                      push     eax                            
  0x001EF4F8  3020                    xor      byte ptr [eax], ah             
  0x001EF4FA  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EF4D9 (cond_jump)
  0x001EF4FB  61                      popal                                   
  0x001EF4FC  726b                    jb       0x1ef569                       
  0x001EF4FE  65722c                  jb       0x1ef52d                       
  0x001EF501  2064656e                and      byte ptr [ebp + 0x6e], ah      
  0x001EF505  7369                    jae      0x1ef570                       
  0x001EF507  7479                    je       0x1ef582                       
  0x001EF509  202564782564            and      byte ptr [0x64257864], ah      
  0x001EF50F  2020                    and      byte ptr [eax], ah             
  0x001EF511  2564002020              and      eax, 0x20200064                
  0x001EF516  2020                    and      byte ptr [eax], ah             
  0x001EF518  2020                    and      byte ptr [eax], ah             
  0x001EF51A  2020                    and      byte ptr [eax], ah             
  0x001EF51C  2533642025              and      eax, 0x25206433                
  0x001EF521  33642025                xor      esp, dword ptr [eax + 0x25]    
  0x001EF525  33642025                xor      esp, dword ptr [eax + 0x25]    
  0x001EF529  33642025                xor      esp, dword ptr [eax + 0x25]    
                                        ; XREF: 0x001EF4FE (cond_jump)
  0x001EF52D  33642025                xor      esp, dword ptr [eax + 0x25]    
  0x001EF531  33642025                xor      esp, dword ptr [eax + 0x25]    
  0x001EF535  33642025                xor      esp, dword ptr [eax + 0x25]    
  0x001EF539  33640045                xor      esp, dword ptr [eax + eax + 0x45] 
  0x001EF53D  6e                      outsb    dx, byte ptr [esi]             
  0x001EF53E  64204f66                and      byte ptr fs:[edi + 0x66], cl   
                                        ; XREF: 0x001EF4DD (cond_jump)
  0x001EF542  20496d                  and      byte ptr [ecx + 0x6d], cl      
  0x001EF545  61                      popal                                   
  0x001EF546  67650000                add      byte ptr gs:[bx + si], al      
  0x001EF54A  0000                    add      byte ptr [eax], al             
  0x001EF54C  4f                      dec      edi                            
  0x001EF54D  62746169                bound    esi, qword ptr [ecx + 0x69]    
  0x001EF551  6e                      outsb    dx, byte ptr [esi]             
  0x001EF552  656420454d              and      byte ptr fs:[ebp + 0x4d], al   
  0x001EF557  53                      push     ebx                            
  0x001EF558  206861                  and      byte ptr [eax + 0x61], ch      
  0x001EF55B  6e                      outsb    dx, byte ptr [esi]             
  0x001EF55C  646c                    insb     byte ptr es:[edi], dx          
  0x001EF55E  65202575000046          and      byte ptr gs:[0x46000075], ah   
  0x001EF565  7265                    jb       0x1ef5cc                       
  0x001EF567  656420454d              and      byte ptr fs:[ebp + 0x4d], al   
  0x001EF56C  53                      push     ebx                            
  0x001EF56D  206861                  and      byte ptr [eax + 0x61], ch      
                                        ; XREF: 0x001EF505 (cond_jump)
  0x001EF570  6e                      outsb    dx, byte ptr [esi]             
  0x001EF571  646c                    insb     byte ptr es:[edi], dx          
  0x001EF573  65202575004465          and      byte ptr gs:[0x65440075], ah   
  0x001EF57A  66696e652052            imul     bp, word ptr [esi + 0x65], 0x5220 
  0x001EF580  657374                  jae      0x1ef5f7                       
  0x001EF583  61                      popal                                   
  0x001EF584  7274                    jb       0x1ef5fa                       
  0x001EF586  20496e                  and      byte ptr [ecx + 0x6e], cl      
  0x001EF589  7465                    je       0x1ef5f0                       
  0x001EF58B  7276                    jb       0x1ef603                       
  0x001EF58D  61                      popal                                   
  0x001EF58E  6c                      insb     byte ptr es:[edi], dx          
  0x001EF58F  202575000044            and      byte ptr [0x44000075], ah      
  0x001EF595  6566696e652051          imul     bp, word ptr gs:[esi + 0x65], 0x5120 
  0x001EF59C  7561                    jne      0x1ef5ff                       
  0x001EF59E  6e                      outsb    dx, byte ptr [esi]             
  0x001EF59F  7469                    je       0x1ef60a                       
  0x001EF5A1  7a61                    jp       0x1ef604                       
  0x001EF5A3  7469                    je       0x1ef60e                       
  0x001EF5A5  6f                      outsd    dx, dword ptr [esi]            
  0x001EF5A6  6e                      outsb    dx, byte ptr [esi]             
  0x001EF5A7  20546162                and      byte ptr [ecx + 0x62], dl      
  0x001EF5AB  6c                      insb     byte ptr es:[edi], dx          
  0x001EF5AC  65202564202070          and      byte ptr gs:[0x70202064], ah   
  0x001EF5B3  7265                    jb       0x1ef61a                       
  0x001EF5B5  636973                  arpl     word ptr [ecx + 0x73], bp      
  0x001EF5B8  696f6e20256400          imul     ebp, dword ptr [edi + 0x6e], 0x642520 
  0x001EF5BF  00446566                add      byte ptr [ebp + 0x66], al      
  0x001EF5C3  696e6520487566          imul     ebp, dword ptr [esi + 0x65], 0x66754820 
  0x001EF5CA  666d                    insw     word ptr es:[edi], dx          
                                        ; XREF: 0x001EF565 (cond_jump)
  0x001EF5CC  61                      popal                                   
  0x001EF5CD  6e                      outsb    dx, byte ptr [esi]             
  0x001EF5CE  20546162                and      byte ptr [ecx + 0x62], dl      
  0x001EF5D2  6c                      insb     byte ptr es:[edi], dx          
  0x001EF5D3  652030                  and      byte ptr gs:[eax], dh          
  0x001EF5D6  7825                    js       0x1ef5fd                       
  0x001EF5D8  3032                    xor      byte ptr [edx], dh             
  0x001EF5DA  7800                    js       0x1ef5dc                       
                                        ; XREF: 0x001EF5DA (cond_jump)
  0x001EF5DC  44                      inc      esp                            
  0x001EF5DD  6566696e652041          imul     bp, word ptr gs:[esi + 0x65], 0x4120 
  0x001EF5E4  7269                    jb       0x1ef64f                       
  0x001EF5E6  7468                    je       0x1ef650                       
  0x001EF5E8  6d                      insd     dword ptr es:[edi], dx         
  0x001EF5E9  657469                  je       0x1ef655                       
  0x001EF5EC  6320                    arpl     word ptr [eax], sp             
  0x001EF5EE  54                      push     esp                            
  0x001EF5EF  61                      popal                                   
                                        ; XREF: 0x001EF589 (cond_jump)
  0x001EF5F0  626c6520                bound    ebp, qword ptr [ebp + 0x20]    
  0x001EF5F4  307825                  xor      byte ptr [eax + 0x25], bh      
                                        ; XREF: 0x001EF580 (cond_jump)
  0x001EF5F7  3032                    xor      byte ptr [edx], dh             
  0x001EF5F9  783a                    js       0x1ef635                       
  0x001EF5FB  2030                    and      byte ptr [eax], dh             
                                        ; XREF: 0x001EF5D6 (cond_jump)
  0x001EF5FD  7825                    js       0x1ef624                       
                                        ; XREF: 0x001EF59C (cond_jump)
  0x001EF5FF  3032                    xor      byte ptr [edx], dh             
  0x001EF601  7800                    js       0x1ef603                       
                                        ; XREF: 0x001EF58B (cond_jump), 0x001EF601 (cond_jump)
  0x001EF603  00556e                  add      byte ptr [ebp + 0x6e], dl      
  0x001EF606  6b6e6f77                imul     ebp, dword ptr [esi + 0x6f], 0x77 
                                        ; XREF: 0x001EF59F (cond_jump)
  0x001EF60A  6e                      outsb    dx, byte ptr [esi]             
  0x001EF60B  204150                  and      byte ptr [ecx + 0x50], al      
                                        ; XREF: 0x001EF5A3 (cond_jump)
  0x001EF60E  50                      push     eax                            
  0x001EF60F  313420                  xor      dword ptr [eax], esi           
  0x001EF612  6d                      insd     dword ptr es:[edi], dx         
  0x001EF613  61                      popal                                   
  0x001EF614  726b                    jb       0x1ef681                       
  0x001EF616  657220                  jb       0x1ef639                       
  0x001EF619  286e6f                  sub      byte ptr [esi + 0x6f], ch      
  0x001EF61C  7420                    je       0x1ef63e                       
  0x001EF61E  41                      inc      ecx                            
  0x001EF61F  646f                    outsd    dx, dword ptr fs:[esi]         
  0x001EF621  626529                  bound    esp, qword ptr [ebp + 0x29]    
                                        ; XREF: 0x001EF5FD (cond_jump)
  0x001EF624  2c20                    sub      al, 0x20                       
  0x001EF626  6c                      insb     byte ptr es:[edi], dx          
  0x001EF627  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF629  677468                  je       0x1ef694                       
  0x001EF62C  20257500556e            and      byte ptr [0x6e550075], ah      
  0x001EF632  6b6e6f77                imul     ebp, dword ptr [esi + 0x6f], 0x77 
  0x001EF636  6e                      outsb    dx, byte ptr [esi]             
  0x001EF637  204150                  and      byte ptr [ecx + 0x50], al      
  0x001EF63A  50                      push     eax                            
  0x001EF63B  3020                    xor      byte ptr [eax], ah             
  0x001EF63D  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EF61C (cond_jump)
  0x001EF63E  61                      popal                                   
  0x001EF63F  726b                    jb       0x1ef6ac                       
  0x001EF641  657220                  jb       0x1ef664                       
  0x001EF644  286e6f                  sub      byte ptr [esi + 0x6f], ch      
  0x001EF647  7420                    je       0x1ef669                       
  0x001EF649  4a                      dec      edx                            
  0x001EF64A  46                      inc      esi                            
  0x001EF64B  49                      dec      ecx                            
  0x001EF64C  46                      inc      esi                            
  0x001EF64D  292c20                  sub      dword ptr [eax], ebp           
                                        ; XREF: 0x001EF5E6 (cond_jump)
  0x001EF650  6c                      insb     byte ptr es:[edi], dx          
  0x001EF651  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF653  677468                  je       0x1ef6be                       
  0x001EF656  202575000000            and      byte ptr [0x75], ah            
  0x001EF65C  0000                    add      byte ptr [eax], al             
  0x001EF65E  0000                    add      byte ptr [eax], al             
  0x001EF660  41                      inc      ecx                            
  0x001EF661  646f                    outsd    dx, dword ptr fs:[esi]         
  0x001EF663  626520                  bound    esp, qword ptr [ebp + 0x20]    
  0x001EF666  41                      inc      ecx                            
  0x001EF667  50                      push     eax                            
  0x001EF668  50                      push     eax                            
                                        ; XREF: 0x001EF647 (cond_jump)
  0x001EF669  313420                  xor      dword ptr [eax], esi           
  0x001EF66C  6d                      insd     dword ptr es:[edi], dx         
  0x001EF66D  61                      popal                                   
  0x001EF66E  726b                    jb       0x1ef6db                       
  0x001EF670  65723a                  jb       0x1ef6ad                       
  0x001EF673  207665                  and      byte ptr [esi + 0x65], dh      
  0x001EF676  7273                    jb       0x1ef6eb                       
  0x001EF678  696f6e2025642c          imul     ebp, dword ptr [edi + 0x6e], 0x2c642520 
  0x001EF67F  20666c                  and      byte ptr [esi + 0x6c], ah      
  0x001EF682  61                      popal                                   
  0x001EF683  677320                  jae      0x1ef6a6                       
  0x001EF686  307825                  xor      byte ptr [eax + 0x25], bh      
  0x001EF689  303478                  xor      byte ptr [eax + edi*2], dh     
  0x001EF68C  2030                    and      byte ptr [eax], dh             
  0x001EF68E  7825                    js       0x1ef6b5                       
  0x001EF690  303478                  xor      byte ptr [eax + edi*2], dh     
  0x001EF693  2c20                    sub      al, 0x20                       
  0x001EF695  7472                    je       0x1ef709                       
  0x001EF697  61                      popal                                   
  0x001EF698  6e                      outsb    dx, byte ptr [esi]             
  0x001EF699  7366                    jae      0x1ef701                       
  0x001EF69B  6f                      outsd    dx, dword ptr [esi]            
  0x001EF69C  726d                    jb       0x1ef70b                       
  0x001EF69E  202564000000            and      byte ptr [0x64], ah            
  0x001EF6A4  43                      inc      ebx                            
  0x001EF6A5  61                      popal                                   
                                        ; XREF: 0x001EF683 (cond_jump)
  0x001EF6A6  7574                    jne      0x1ef71c                       
  0x001EF6A8  696f6e3a207175          imul     ebp, dword ptr [edi + 0x6e], 0x7571203a 
  0x001EF6AF  61                      popal                                   
  0x001EF6B0  6e                      outsb    dx, byte ptr [esi]             
  0x001EF6B1  7469                    je       0x1ef71c                       
  0x001EF6B3  7a61                    jp       0x1ef716                       
                                        ; XREF: 0x001EF68E (cond_jump)
  0x001EF6B5  7469                    je       0x1ef720                       
  0x001EF6B7  6f                      outsd    dx, dword ptr [esi]            
  0x001EF6B8  6e                      outsb    dx, byte ptr [esi]             
  0x001EF6B9  20746162                and      byte ptr [ecx + 0x62], dh      
  0x001EF6BD  6c                      insb     byte ptr es:[edi], dx          
                                        ; XREF: 0x001EF653 (cond_jump)
  0x001EF6BE  657320                  jae      0x1ef6e1                       
  0x001EF6C1  61                      popal                                   
  0x001EF6C2  7265                    jb       0x1ef729                       
  0x001EF6C4  20746f6f                and      byte ptr [edi + ebp*2 + 0x6f], dh 
  0x001EF6C8  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EF6CB  61                      popal                                   
  0x001EF6CC  7273                    jb       0x1ef741                       
  0x001EF6CE  6520666f                and      byte ptr gs:[esi + 0x6f], ah   
  0x001EF6D2  7220                    jb       0x1ef6f4                       
  0x001EF6D4  626173                  bound    esp, qword ptr [ecx + 0x73]    
  0x001EF6D7  656c                    insb     byte ptr es:[edi], dx          
  0x001EF6D9  696e65204a5045          imul     ebp, dword ptr [esi + 0x65], 0x45504a20 
  0x001EF6E0  47                      inc      edi                            
                                        ; XREF: 0x001EF6BE (cond_jump)
  0x001EF6E1  0000                    add      byte ptr [eax], al             
  0x001EF6E3  0036                    add      byte ptr [esi], dh             
  0x001EF6E5  61                      popal                                   
  0x001EF6E6  2020                    and      byte ptr [eax], ah             
  0x001EF6E8  37                      aaa                                     
  0x001EF6E9  2d4665622d              sub      eax, 0x2d626546                
  0x001EF6EE  3936                    cmp      dword ptr [esi], esi           
  0x001EF6F0  0000                    add      byte ptr [eax], al             
  0x001EF6F2  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001EF6D2 (cond_jump)
  0x001EF6F4  43                      inc      ebx                            
  0x001EF6F5  6f                      outsd    dx, dword ptr [esi]            
  0x001EF6F6  7079                    jo       0x1ef771                       
  0x001EF6F8  7269                    jb       0x1ef763                       
  0x001EF6FA  676874202843            push     0x43282074                     
  0x001EF700  2920                    sub      dword ptr [eax], esp           
  0x001EF702  3139                    xor      dword ptr [ecx], edi           
  0x001EF704  3936                    cmp      dword ptr [esi], esi           
  0x001EF706  2c20                    sub      al, 0x20                       
  0x001EF708  54                      push     esp                            
                                        ; XREF: 0x001EF695 (cond_jump)
  0x001EF709  686f6d6173              push     0x73616d6f                     
  0x001EF70E  20472e                  and      byte ptr [edi + 0x2e], al      
  0x001EF711  204c616e                and      byte ptr [ecx + 0x6e], cl      
  0x001EF715  650000                  add      byte ptr gs:[eax], al          
  0x001EF718  57                      push     edi                            
  0x001EF719  7269                    jb       0x1ef784                       
  0x001EF71B  7465                    je       0x1ef782                       
  0x001EF71D  20746f20                and      byte ptr [edi + ebp*2 + 0x20], dh 
  0x001EF721  58                      pop      eax                            
  0x001EF722  4d                      dec      ebp                            
  0x001EF723  53                      push     ebx                            
  0x001EF724  206661                  and      byte ptr [esi + 0x61], ah      
  0x001EF727  696c656400526561        imul     ebp, dword ptr [ebp + 0x64], 0x61655200 
  0x001EF72F  64206672                and      byte ptr fs:[esi + 0x72], ah   
  0x001EF733  6f                      outsd    dx, dword ptr [esi]            
  0x001EF734  6d                      insd     dword ptr es:[edi], dx         
  0x001EF735  20584d                  and      byte ptr [eax + 0x4d], bl      
  0x001EF738  53                      push     ebx                            
  0x001EF739  206661                  and      byte ptr [esi + 0x61], ah      
  0x001EF73C  696c656400000000        imul     ebp, dword ptr [ebp + 0x64], 0 
  0x001EF744  49                      dec      ecx                            
  0x001EF745  6d                      insd     dword ptr es:[edi], dx         
  0x001EF746  61                      popal                                   
  0x001EF747  676520746f              and      byte ptr gs:[si + 0x6f], dh    
  0x001EF74C  6f                      outsd    dx, dword ptr [esi]            
  0x001EF74D  207769                  and      byte ptr [edi + 0x69], dh      
  0x001EF750  646520666f              and      byte ptr gs:[esi + 0x6f], ah   
  0x001EF755  7220                    jb       0x1ef777                       
  0x001EF757  7468                    je       0x1ef7c1                       
  0x001EF759  697320696d706c          imul     esi, dword ptr [ebx + 0x20], 0x6c706d69 
  0x001EF760  656d                    insd     dword ptr es:[edi], dx         
  0x001EF762  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF764  7461                    je       0x1ef7c7                       
  0x001EF766  7469                    je       0x1ef7d1                       
  0x001EF768  6f                      outsd    dx, dword ptr [esi]            
  0x001EF769  6e                      outsb    dx, byte ptr [esi]             
  0x001EF76A  0000                    add      byte ptr [eax], al             
  0x001EF76C  56                      push     esi                            
  0x001EF76D  69727475616c20          imul     esi, dword ptr [edx + 0x74], 0x206c6175 
  0x001EF774  61                      popal                                   
  0x001EF775  7272                    jb       0x1ef7e9                       
                                        ; XREF: 0x001EF755 (cond_jump)
  0x001EF777  61                      popal                                   
  0x001EF778  7920                    jns      0x1ef79a                       
  0x001EF77A  636f6e                  arpl     word ptr [edi + 0x6e], bp      
  0x001EF77D  7472                    je       0x1ef7f1                       
  0x001EF77F  6f                      outsd    dx, dword ptr [esi]            
  0x001EF780  6c                      insb     byte ptr es:[edi], dx          
  0x001EF781  6c                      insb     byte ptr es:[edi], dx          
                                        ; XREF: 0x001EF71B (cond_jump)
  0x001EF782  657220                  jb       0x1ef7a5                       
  0x001EF785  6d                      insd     dword ptr es:[edi], dx         
  0x001EF786  657373                  jae      0x1ef7fc                       
  0x001EF789  6564207570              and      byte ptr fs:[ebp + 0x70], dh   
  0x001EF78E  0000                    add      byte ptr [eax], al             
  0x001EF790  55                      push     ebp                            
  0x001EF791  6e                      outsb    dx, byte ptr [esi]             
  0x001EF792  7375                    jae      0x1ef809                       
  0x001EF794  7070                    jo       0x1ef806                       
  0x001EF796  6f                      outsd    dx, dword ptr [esi]            
  0x001EF797  7274                    jb       0x1ef80d                       
  0x001EF799  6564206d61              and      byte ptr fs:[ebp + 0x61], ch   
  0x001EF79E  726b                    jb       0x1ef80b                       
  0x001EF7A0  657220                  jb       0x1ef7c3                       
  0x001EF7A3  7479                    je       0x1ef81e                       
                                        ; XREF: 0x001EF782 (cond_jump)
  0x001EF7A5  7065                    jo       0x1ef80c                       
  0x001EF7A7  2030                    and      byte ptr [eax], dh             
  0x001EF7A9  7825                    js       0x1ef7d0                       
  0x001EF7AB  3032                    xor      byte ptr [edx], dh             
  0x001EF7AD  7800                    js       0x1ef7af                       
                                        ; XREF: 0x001EF7AD (cond_jump)
  0x001EF7AF  004170                  add      byte ptr [ecx + 0x70], al      
  0x001EF7B2  706c                    jo       0x1ef820                       
  0x001EF7B4  69636174696f6e          imul     esp, dword ptr [ebx + 0x61], 0x6e6f6974 
  0x001EF7BB  20747261                and      byte ptr [edx + esi*2 + 0x61], dh 
  0x001EF7BF  6e                      outsb    dx, byte ptr [esi]             
  0x001EF7C0  7366                    jae      0x1ef828                       
  0x001EF7C2  657272                  jb       0x1ef837                       
  0x001EF7C5  656420746f6f            and      byte ptr fs:[edi + ebp*2 + 0x6f], dh 
  0x001EF7CB  206665                  and      byte ptr [esi + 0x65], ah      
  0x001EF7CE  7720                    ja       0x1ef7f0                       
                                        ; XREF: 0x001EF7A9 (cond_jump)
  0x001EF7D0  7363                    jae      0x1ef835                       
  0x001EF7D2  61                      popal                                   
  0x001EF7D3  6e                      outsb    dx, byte ptr [esi]             
  0x001EF7D4  6c                      insb     byte ptr es:[edi], dx          
  0x001EF7D5  696e6573000000          imul     ebp, dword ptr [esi + 0x65], 0x73 
  0x001EF7DC  57                      push     edi                            
  0x001EF7DD  7269                    jb       0x1ef848                       
  0x001EF7DF  7465                    je       0x1ef846                       
  0x001EF7E1  206661                  and      byte ptr [esi + 0x61], ah      
  0x001EF7E4  696c6564206f6e20        imul     ebp, dword ptr [ebp + 0x64], 0x206e6f20 
  0x001EF7EC  7465                    je       0x1ef853                       
  0x001EF7EE  6d                      insd     dword ptr es:[edi], dx         
  0x001EF7EF  706f                    jo       0x1ef860                       
                                        ; XREF: 0x001EF77D (cond_jump)
  0x001EF7F1  7261                    jb       0x1ef854                       
  0x001EF7F3  7279                    jb       0x1ef86e                       
  0x001EF7F5  206669                  and      byte ptr [esi + 0x69], ah      
  0x001EF7F8  6c                      insb     byte ptr es:[edi], dx          
  0x001EF7F9  65202d2d2d206f          and      byte ptr gs:[0x6f202d2d], ch   
  0x001EF800  7574                    jne      0x1ef876                       
  0x001EF802  206f66                  and      byte ptr [edi + 0x66], ch      
  0x001EF805  20646973                and      byte ptr [ecx + ebp*2 + 0x73], ah 
                                        ; XREF: 0x001EF792 (cond_jump)
  0x001EF809  6b2073                  imul     esp, dword ptr [eax], 0x73     
                                        ; XREF: 0x001EF7A5 (cond_jump)
  0x001EF80C  7061                    jo       0x1ef86f                       
  0x001EF80E  63653f                  arpl     word ptr [ebp + 0x3f], sp      
  0x001EF811  0000                    add      byte ptr [eax], al             
  0x001EF813  005365                  add      byte ptr [ebx + 0x65], dl      
  0x001EF816  656b2066                imul     esp, dword ptr gs:[eax], 0x66  
  0x001EF81A  61                      popal                                   
  0x001EF81B  696c6564206f6e20        imul     ebp, dword ptr [ebp + 0x64], 0x206e6f20 
  0x001EF823  7465                    je       0x1ef88a                       
  0x001EF825  6d                      insd     dword ptr es:[edi], dx         
  0x001EF826  706f                    jo       0x1ef897                       
                                        ; XREF: 0x001EF7C0 (cond_jump)
  0x001EF828  7261                    jb       0x1ef88b                       
  0x001EF82A  7279                    jb       0x1ef8a5                       
  0x001EF82C  206669                  and      byte ptr [esi + 0x69], ah      
  0x001EF82F  6c                      insb     byte ptr es:[edi], dx          
  0x001EF830  650000                  add      byte ptr gs:[eax], al          
  0x001EF833  005265                  add      byte ptr [edx + 0x65], dl      
  0x001EF836  61                      popal                                   
                                        ; XREF: 0x001EF7C2 (cond_jump)
  0x001EF837  64206661                and      byte ptr fs:[esi + 0x61], ah   
  0x001EF83B  696c6564206f6e20        imul     ebp, dword ptr [ebp + 0x64], 0x206e6f20 
  0x001EF843  7465                    je       0x1ef8aa                       
  0x001EF845  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EF7DF (cond_jump)
  0x001EF846  706f                    jo       0x1ef8b7                       
                                        ; XREF: 0x001EF7DD (cond_jump)
  0x001EF848  7261                    jb       0x1ef8ab                       
  0x001EF84A  7279                    jb       0x1ef8c5                       
  0x001EF84C  206669                  and      byte ptr [esi + 0x69], ah      
  0x001EF84F  6c                      insb     byte ptr es:[edi], dx          
  0x001EF850  650000                  add      byte ptr gs:[eax], al          
                                        ; XREF: 0x001EF7EC (cond_jump)
  0x001EF853  004661                  add      byte ptr [esi + 0x61], al      
  0x001EF856  696c656420746f20        imul     ebp, dword ptr [ebp + 0x64], 0x206f7420 
  0x001EF85E  637265                  arpl     word ptr [edx + 0x65], si      
  0x001EF861  61                      popal                                   
  0x001EF862  7465                    je       0x1ef8c9                       
  0x001EF864  2074656d                and      byte ptr [ebp + 0x6d], dh      
  0x001EF868  706f                    jo       0x1ef8d9                       
  0x001EF86A  7261                    jb       0x1ef8cd                       
  0x001EF86C  7279                    jb       0x1ef8e7                       
                                        ; XREF: 0x001EF7F3 (cond_jump)
  0x001EF86E  206669                  and      byte ptr [esi + 0x69], ah      
  0x001EF871  6c                      insb     byte ptr es:[edi], dx          
  0x001EF872  65202573000049          and      byte ptr gs:[0x49000073], ah   
  0x001EF879  6e                      outsb    dx, byte ptr [esi]             
  0x001EF87A  7661                    jbe      0x1ef8dd                       
  0x001EF87C  6c                      insb     byte ptr es:[edi], dx          
  0x001EF87D  6964204a50454720        imul     esp, dword ptr [eax + 0x4a], 0x20474550 
  0x001EF885  66696c65207374          imul     bp, word ptr [ebp + 0x20], 0x7473 
  0x001EF88C  7275                    jb       0x1ef903                       
  0x001EF88E  63747572                arpl     word ptr [ebp + esi*2 + 0x72], si 
  0x001EF892  653a20                  cmp      ah, byte ptr gs:[eax]          
  0x001EF895  53                      push     ebx                            
  0x001EF896  4f                      dec      edi                            
                                        ; XREF: 0x001EF826 (cond_jump)
  0x001EF897  53                      push     ebx                            
  0x001EF898  206265                  and      byte ptr [edx + 0x65], ah      
  0x001EF89B  666f                    outsw    dx, word ptr [esi]             
  0x001EF89D  7265                    jb       0x1ef904                       
  0x001EF89F  20534f                  and      byte ptr [ebx + 0x4f], dl      
  0x001EF8A2  46                      inc      esi                            
  0x001EF8A3  00496e                  add      byte ptr [ecx + 0x6e], cl      
  0x001EF8A6  7661                    jbe      0x1ef909                       
  0x001EF8A8  6c                      insb     byte ptr es:[edi], dx          
  0x001EF8A9  6964204a50454720        imul     esp, dword ptr [eax + 0x4a], 0x20474550 
  0x001EF8B1  66696c65207374          imul     bp, word ptr [ebp + 0x20], 0x7473 
  0x001EF8B8  7275                    jb       0x1ef92f                       
  0x001EF8BA  63747572                arpl     word ptr [ebp + esi*2 + 0x72], si 
  0x001EF8BE  653a20                  cmp      ah, byte ptr gs:[eax]          
  0x001EF8C1  7477                    je       0x1ef93a                       
  0x001EF8C3  6f                      outsd    dx, dword ptr [esi]            
  0x001EF8C4  20534f                  and      byte ptr [ebx + 0x4f], dl      
  0x001EF8C7  49                      dec      ecx                            
  0x001EF8C8  206d61                  and      byte ptr [ebp + 0x61], ch      
  0x001EF8CB  726b                    jb       0x1ef938                       
                                        ; XREF: 0x001EF86A (cond_jump)
  0x001EF8CD  657273                  jb       0x1ef943                       
  0x001EF8D0  0000                    add      byte ptr [eax], al             
  0x001EF8D2  0000                    add      byte ptr [eax], al             
  0x001EF8D4  55                      push     ebp                            
  0x001EF8D5  6e                      outsb    dx, byte ptr [esi]             
  0x001EF8D6  7375                    jae      0x1ef94d                       
  0x001EF8D8  7070                    jo       0x1ef94a                       
  0x001EF8DA  6f                      outsd    dx, dword ptr [esi]            
  0x001EF8DB  7274                    jb       0x1ef951                       
                                        ; XREF: 0x001EF87A (cond_jump)
  0x001EF8DD  6564204a50              and      byte ptr fs:[edx + 0x50], cl   
  0x001EF8E2  45                      inc      ebp                            
  0x001EF8E3  47                      inc      edi                            
  0x001EF8E4  207072                  and      byte ptr [eax + 0x72], dh      
                                        ; XREF: 0x001EF86C (cond_jump)
  0x001EF8E7  6f                      outsd    dx, dword ptr [esi]            
  0x001EF8E8  636573                  arpl     word ptr [ebp + 0x73], sp      
  0x001EF8EB  733a                    jae      0x1ef927                       
  0x001EF8ED  20534f                  and      byte ptr [ebx + 0x4f], dl      
  0x001EF8F0  46                      inc      esi                            
  0x001EF8F1  20747970                and      byte ptr [ecx + edi*2 + 0x70], dh 
  0x001EF8F5  652030                  and      byte ptr gs:[eax], dh          
  0x001EF8F8  7825                    js       0x1ef91f                       
  0x001EF8FA  3032                    xor      byte ptr [edx], dh             
  0x001EF8FC  7800                    js       0x1ef8fe                       
                                        ; XREF: 0x001EF8FC (cond_jump)
  0x001EF8FE  0000                    add      byte ptr [eax], al             
  0x001EF900  49                      dec      ecx                            
  0x001EF901  6e                      outsb    dx, byte ptr [esi]             
  0x001EF902  7661                    jbe      0x1ef965                       
                                        ; XREF: 0x001EF89D (cond_jump)
  0x001EF904  6c                      insb     byte ptr es:[edi], dx          
  0x001EF905  6964204a50454720        imul     esp, dword ptr [eax + 0x4a], 0x20474550 
  0x001EF90D  66696c65207374          imul     bp, word ptr [ebp + 0x20], 0x7473 
  0x001EF914  7275                    jb       0x1ef98b                       
  0x001EF916  63747572                arpl     word ptr [ebp + esi*2 + 0x72], si 
  0x001EF91A  653a20                  cmp      ah, byte ptr gs:[eax]          
  0x001EF91D  6d                      insd     dword ptr es:[edi], dx         
  0x001EF91E  697373696e6720          imul     esi, dword ptr [ebx + 0x73], 0x20676e69 
  0x001EF925  53                      push     ebx                            
  0x001EF926  4f                      dec      edi                            
                                        ; XREF: 0x001EF8EB (cond_jump)
  0x001EF927  53                      push     ebx                            
  0x001EF928  206d61                  and      byte ptr [ebp + 0x61], ch      
  0x001EF92B  726b                    jb       0x1ef998                       
  0x001EF92D  657200                  jb       0x1ef930                       
                                        ; XREF: 0x001EF92D (cond_jump)
  0x001EF930  49                      dec      ecx                            
  0x001EF931  6e                      outsb    dx, byte ptr [esi]             
  0x001EF932  7661                    jbe      0x1ef995                       
  0x001EF934  6c                      insb     byte ptr es:[edi], dx          
  0x001EF935  6964204a50454720        imul     esp, dword ptr [eax + 0x4a], 0x20474550 
  0x001EF93D  66696c65207374          imul     bp, word ptr [ebp + 0x20], 0x7473 
  0x001EF944  7275                    jb       0x1ef9bb                       
  0x001EF946  63747572                arpl     word ptr [ebp + esi*2 + 0x72], si 
                                        ; XREF: 0x001EF8D8 (cond_jump)
  0x001EF94A  653a20                  cmp      ah, byte ptr gs:[eax]          
                                        ; XREF: 0x001EF8D6 (cond_jump)
  0x001EF94D  7477                    je       0x1ef9c6                       
  0x001EF94F  6f                      outsd    dx, dword ptr [esi]            
  0x001EF950  20534f                  and      byte ptr [ebx + 0x4f], dl      
  0x001EF953  46                      inc      esi                            
  0x001EF954  206d61                  and      byte ptr [ebp + 0x61], ch      
  0x001EF957  726b                    jb       0x1ef9c4                       
  0x001EF959  657273                  jb       0x1ef9cf                       
  0x001EF95C  0000                    add      byte ptr [eax], al             
  0x001EF95E  0000                    add      byte ptr [eax], al             
  0x001EF960  43                      inc      ebx                            
  0x001EF961  61                      popal                                   
  0x001EF962  6e                      outsb    dx, byte ptr [esi]             
  0x001EF963  6e                      outsb    dx, byte ptr [esi]             
  0x001EF964  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EF902 (cond_jump)
  0x001EF965  7420                    je       0x1ef987                       
  0x001EF967  7175                    jno      0x1ef9de                       
  0x001EF969  61                      popal                                   
  0x001EF96A  6e                      outsb    dx, byte ptr [esi]             
  0x001EF96B  7469                    je       0x1ef9d6                       
  0x001EF96D  7a65                    jp       0x1ef9d4                       
  0x001EF96F  20746f20                and      byte ptr [edi + ebp*2 + 0x20], dh 
  0x001EF973  6d                      insd     dword ptr es:[edi], dx         
  0x001EF974  6f                      outsd    dx, dword ptr [esi]            
  0x001EF975  7265                    jb       0x1ef9dc                       
  0x001EF977  20746861                and      byte ptr [eax + ebp*2 + 0x61], dh 
  0x001EF97B  6e                      outsb    dx, byte ptr [esi]             
  0x001EF97C  20256420636f            and      byte ptr [0x6f632064], ah      
  0x001EF982  6c                      insb     byte ptr es:[edi], dx          
  0x001EF983  6f                      outsd    dx, dword ptr [esi]            
  0x001EF984  7273                    jb       0x1ef9f9                       
  0x001EF986  0000                    add      byte ptr [eax], al             
  0x001EF988  43                      inc      ebx                            
  0x001EF989  61                      popal                                   
  0x001EF98A  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EF914 (cond_jump)
  0x001EF98B  6e                      outsb    dx, byte ptr [esi]             
  0x001EF98C  6f                      outsd    dx, dword ptr [esi]            
  0x001EF98D  7420                    je       0x1ef9af                       
  0x001EF98F  7175                    jno      0x1efa06                       
  0x001EF991  61                      popal                                   
  0x001EF992  6e                      outsb    dx, byte ptr [esi]             
  0x001EF993  7469                    je       0x1ef9fe                       
                                        ; XREF: 0x001EF932 (cond_jump)
  0x001EF995  7a65                    jp       0x1ef9fc                       
  0x001EF997  20746f20                and      byte ptr [edi + ebp*2 + 0x20], dh 
  0x001EF99B  66657765                ja       0x1efa04                       
  0x001EF99F  7220                    jb       0x1ef9c1                       
  0x001EF9A1  7468                    je       0x1efa0b                       
  0x001EF9A3  61                      popal                                   
  0x001EF9A4  6e                      outsb    dx, byte ptr [esi]             
  0x001EF9A5  20256420636f            and      byte ptr [0x6f632064], ah      
  0x001EF9AB  6c                      insb     byte ptr es:[edi], dx          
  0x001EF9AC  6f                      outsd    dx, dword ptr [esi]            
  0x001EF9AD  7273                    jb       0x1efa22                       
                                        ; XREF: 0x001EF98D (cond_jump)
  0x001EF9AF  004361                  add      byte ptr [ebx + 0x61], al      
  0x001EF9B2  6e                      outsb    dx, byte ptr [esi]             
  0x001EF9B3  6e                      outsb    dx, byte ptr [esi]             
  0x001EF9B4  6f                      outsd    dx, dword ptr [esi]            
  0x001EF9B5  7420                    je       0x1ef9d7                       
  0x001EF9B7  7175                    jno      0x1efa2e                       
  0x001EF9B9  61                      popal                                   
  0x001EF9BA  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EF944 (cond_jump)
  0x001EF9BB  7469                    je       0x1efa26                       
  0x001EF9BD  7a65                    jp       0x1efa24                       
  0x001EF9BF  206d6f                  and      byte ptr [ebp + 0x6f], ch      
  0x001EF9C2  7265                    jb       0x1efa29                       
                                        ; XREF: 0x001EF957 (cond_jump)
  0x001EF9C4  20746861                and      byte ptr [eax + ebp*2 + 0x61], dh 
  0x001EF9C8  6e                      outsb    dx, byte ptr [esi]             
  0x001EF9C9  20256420636f            and      byte ptr [0x6f632064], ah      
                                        ; XREF: 0x001EF959 (cond_jump)
  0x001EF9CF  6c                      insb     byte ptr es:[edi], dx          
  0x001EF9D0  6f                      outsd    dx, dword ptr [esi]            
  0x001EF9D1  7220                    jb       0x1ef9f3                       
  0x001EF9D3  636f6d                  arpl     word ptr [edi + 0x6d], bp      
                                        ; XREF: 0x001EF96B (cond_jump)
  0x001EF9D6  706f                    jo       0x1efa47                       
  0x001EF9D8  6e                      outsb    dx, byte ptr [esi]             
  0x001EF9D9  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EF9DB  7473                    je       0x1efa50                       
  0x001EF9DD  0000                    add      byte ptr [eax], al             
  0x001EF9DF  00496e                  add      byte ptr [ecx + 0x6e], cl      
  0x001EF9E2  7375                    jae      0x1efa59                       
  0x001EF9E4  6666696369656e          imul     sp, word ptr [ebx + 0x69], 0x6e65 
  0x001EF9EB  7420                    je       0x1efa0d                       
  0x001EF9ED  6d                      insd     dword ptr es:[edi], dx         
  0x001EF9EE  656d                    insd     dword ptr es:[edi], dx         
  0x001EF9F0  6f                      outsd    dx, dword ptr [esi]            
  0x001EF9F1  7279                    jb       0x1efa6c                       
                                        ; XREF: 0x001EF9D1 (cond_jump)
  0x001EF9F3  2028                    and      byte ptr [eax], ch             
  0x001EF9F5  636173                  arpl     word ptr [ecx + 0x73], sp      
  0x001EF9F8  65202564290000          and      byte ptr gs:[0x2964], ah       
  0x001EF9FF  004e6f                  add      byte ptr [esi + 0x6f], cl      
  0x001EFA02  7420                    je       0x1efa24                       
                                        ; XREF: 0x001EF99B (cond_jump)
  0x001EFA04  61                      popal                                   
  0x001EFA05  204a50                  and      byte ptr [edx + 0x50], cl      
  0x001EFA08  45                      inc      ebp                            
  0x001EFA09  47                      inc      edi                            
  0x001EFA0A  206669                  and      byte ptr [esi + 0x69], ah      
                                        ; XREF: 0x001EF9EB (cond_jump)
  0x001EFA0D  6c                      insb     byte ptr es:[edi], dx          
  0x001EFA0E  653a20                  cmp      ah, byte ptr gs:[eax]          
  0x001EFA11  7374                    jae      0x1efa87                       
  0x001EFA13  61                      popal                                   
  0x001EFA14  7274                    jb       0x1efa8a                       
  0x001EFA16  7320                    jae      0x1efa38                       
  0x001EFA18  7769                    ja       0x1efa83                       
  0x001EFA1A  7468                    je       0x1efa84                       
  0x001EFA1C  2030                    and      byte ptr [eax], dh             
  0x001EFA1E  7825                    js       0x1efa45                       
  0x001EFA20  3032                    xor      byte ptr [edx], dh             
                                        ; XREF: 0x001EF9AD (cond_jump)
  0x001EFA22  7820                    js       0x1efa44                       
                                        ; XREF: 0x001EF9BD (cond_jump), 0x001EFA02 (cond_jump)
  0x001EFA24  307825                  xor      byte ptr [eax + 0x25], bh      
  0x001EFA27  3032                    xor      byte ptr [edx], dh             
                                        ; XREF: 0x001EF9C2 (cond_jump)
  0x001EFA29  7800                    js       0x1efa2b                       
                                        ; XREF: 0x001EFA29 (cond_jump)
  0x001EFA2B  005175                  add      byte ptr [ecx + 0x75], dl      
                                        ; XREF: 0x001EF9B7 (cond_jump)
  0x001EFA2E  61                      popal                                   
  0x001EFA2F  6e                      outsb    dx, byte ptr [esi]             
  0x001EFA30  7469                    je       0x1efa9b                       
  0x001EFA32  7a61                    jp       0x1efa95                       
  0x001EFA34  7469                    je       0x1efa9f                       
  0x001EFA36  6f                      outsd    dx, dword ptr [esi]            
  0x001EFA37  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EFA16 (cond_jump)
  0x001EFA38  20746162                and      byte ptr [ecx + 0x62], dh      
  0x001EFA3C  6c                      insb     byte ptr es:[edi], dx          
  0x001EFA3D  652030                  and      byte ptr gs:[eax], dh          
  0x001EFA40  7825                    js       0x1efa67                       
  0x001EFA42  3032                    xor      byte ptr [edx], dh             
                                        ; XREF: 0x001EFA22 (cond_jump)
  0x001EFA44  7820                    js       0x1efa66                       
  0x001EFA46  7761                    ja       0x1efaa9                       
  0x001EFA48  7320                    jae      0x1efa6a                       
  0x001EFA4A  6e                      outsb    dx, byte ptr [esi]             
  0x001EFA4B  6f                      outsd    dx, dword ptr [esi]            
  0x001EFA4C  7420                    je       0x1efa6e                       
  0x001EFA4E  646566696e656400        imul     bp, word ptr gs:[esi + 0x65], 0x64 
  0x001EFA56  0000                    add      byte ptr [eax], al             
  0x001EFA58  4a                      dec      edx                            
                                        ; XREF: 0x001EF9E2 (cond_jump)
  0x001EFA59  50                      push     eax                            
  0x001EFA5A  45                      inc      ebp                            
  0x001EFA5B  47                      inc      edi                            
  0x001EFA5C  20646174                and      byte ptr [ecx + 0x74], ah      
  0x001EFA60  61                      popal                                   
  0x001EFA61  7374                    jae      0x1efad7                       
  0x001EFA63  7265                    jb       0x1efaca                       
  0x001EFA65  61                      popal                                   
                                        ; XREF: 0x001EFA44 (cond_jump)
  0x001EFA66  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EFA40 (cond_jump)
  0x001EFA67  20636f                  and      byte ptr [ebx + 0x6f], ah      
                                        ; XREF: 0x001EFA48 (cond_jump)
  0x001EFA6A  6e                      outsb    dx, byte ptr [esi]             
  0x001EFA6B  7461                    je       0x1eface                       
  0x001EFA6D  696e73206e6f20          imul     ebp, dword ptr [esi + 0x73], 0x206f6e20 
  0x001EFA74  696d6167650000          imul     ebp, dword ptr [ebp + 0x61], 0x6567 
  0x001EFA7B  004875                  add      byte ptr [eax + 0x75], cl      
  0x001EFA7E  66666d                  insw     word ptr es:[edi], dx          
  0x001EFA81  61                      popal                                   
  0x001EFA82  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EFA18 (cond_jump)
  0x001EFA83  20746162                and      byte ptr [ecx + 0x62], dh      
                                        ; XREF: 0x001EFA11 (cond_jump)
  0x001EFA87  6c                      insb     byte ptr es:[edi], dx          
  0x001EFA88  652030                  and      byte ptr gs:[eax], dh          
  0x001EFA8B  7825                    js       0x1efab2                       
  0x001EFA8D  3032                    xor      byte ptr [edx], dh             
  0x001EFA8F  7820                    js       0x1efab1                       
  0x001EFA91  7761                    ja       0x1efaf4                       
  0x001EFA93  7320                    jae      0x1efab5                       
                                        ; XREF: 0x001EFA32 (cond_jump)
  0x001EFA95  6e                      outsb    dx, byte ptr [esi]             
  0x001EFA96  6f                      outsd    dx, dword ptr [esi]            
  0x001EFA97  7420                    je       0x1efab9                       
  0x001EFA99  646566696e656400        imul     bp, word ptr gs:[esi + 0x65], 0x64 
  0x001EFAA1  0000                    add      byte ptr [eax], al             
  0x001EFAA3  004261                  add      byte ptr [edx + 0x61], al      
  0x001EFAA6  636b69                  arpl     word ptr [ebx + 0x69], bp      
                                        ; XREF: 0x001EFA46 (cond_jump)
  0x001EFAA9  6e                      outsb    dx, byte ptr [esi]             
  0x001EFAAA  67207374                and      byte ptr [bp + di + 0x74], dh  
  0x001EFAAE  6f                      outsd    dx, dword ptr [esi]            
  0x001EFAAF  7265                    jb       0x1efb16                       
                                        ; XREF: 0x001EFA8F (cond_jump)
  0x001EFAB1  206e6f                  and      byte ptr [esi + 0x6f], ch      
  0x001EFAB4  7420                    je       0x1efad6                       
  0x001EFAB6  7375                    jae      0x1efb2d                       
  0x001EFAB8  7070                    jo       0x1efb2a                       
  0x001EFABA  6f                      outsd    dx, dword ptr [esi]            
  0x001EFABB  7274                    jb       0x1efb31                       
  0x001EFABD  6564005265              add      byte ptr fs:[edx + 0x65], dl   
  0x001EFAC2  7175                    jno      0x1efb39                       
  0x001EFAC4  657374                  jae      0x1efb3b                       
  0x001EFAC7  6564206665              and      byte ptr fs:[esi + 0x65], ah   
  0x001EFACC  61                      popal                                   
  0x001EFACD  7475                    je       0x1efb44                       
  0x001EFACF  7265                    jb       0x1efb36                       
  0x001EFAD1  207761                  and      byte ptr [edi + 0x61], dh      
  0x001EFAD4  7320                    jae      0x1efaf6                       
                                        ; XREF: 0x001EFAB4 (cond_jump)
  0x001EFAD6  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EFA61 (cond_jump)
  0x001EFAD7  6d                      insd     dword ptr es:[edi], dx         
  0x001EFAD8  6974746564206174        imul     esi, dword ptr [esp + esi*2 + 0x65], 0x74612064 
  0x001EFAE0  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EFAE3  6d                      insd     dword ptr es:[edi], dx         
  0x001EFAE4  7069                    jo       0x1efb4f                       
  0x001EFAE6  6c                      insb     byte ptr es:[edi], dx          
  0x001EFAE7  652074696d              and      byte ptr gs:[ecx + ebp*2 + 0x6d], dh 
  0x001EFAEC  650000                  add      byte ptr gs:[eax], al          
  0x001EFAEF  004e6f                  add      byte ptr [esi + 0x6f], cl      
  0x001EFAF2  7420                    je       0x1efb14                       
                                        ; XREF: 0x001EFA91 (cond_jump)
  0x001EFAF4  696d706c656d65          imul     ebp, dword ptr [ebp + 0x70], 0x656d656c 
  0x001EFAFB  6e                      outsb    dx, byte ptr [esi]             
  0x001EFAFC  7465                    je       0x1efb63                       
  0x001EFAFE  64207965                and      byte ptr fs:[ecx + 0x65], bh   
  0x001EFB02  7400                    je       0x1efb04                       
                                        ; XREF: 0x001EFB02 (cond_jump)
  0x001EFB04  49                      dec      ecx                            
  0x001EFB05  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB06  7661                    jbe      0x1efb69                       
  0x001EFB08  6c                      insb     byte ptr es:[edi], dx          
  0x001EFB09  696420636f6c6f72        imul     esp, dword ptr [eax + 0x63], 0x726f6c6f 
  0x001EFB11  207175                  and      byte ptr [ecx + 0x75], dh      
                                        ; XREF: 0x001EFAF2 (cond_jump)
  0x001EFB14  61                      popal                                   
  0x001EFB15  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EFAAF (cond_jump)
  0x001EFB16  7469                    je       0x1efb81                       
  0x001EFB18  7a61                    jp       0x1efb7b                       
  0x001EFB1A  7469                    je       0x1efb85                       
  0x001EFB1C  6f                      outsd    dx, dword ptr [esi]            
  0x001EFB1D  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB1E  206d6f                  and      byte ptr [ebp + 0x6f], ch      
  0x001EFB21  6465206368              and      byte ptr gs:[ebx + 0x68], ah   
  0x001EFB26  61                      popal                                   
  0x001EFB27  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB28  67650000                add      byte ptr gs:[bx + si], al      
  0x001EFB2C  53                      push     ebx                            
                                        ; XREF: 0x001EFAB6 (cond_jump)
  0x001EFB2D  63616e                  arpl     word ptr [ecx + 0x6e], sp      
  0x001EFB30  207363                  and      byte ptr [ebx + 0x63], dh      
  0x001EFB33  7269                    jb       0x1efb9e                       
  0x001EFB35  7074                    jo       0x1efbab                       
  0x001EFB37  20646f65                and      byte ptr [edi + ebp*2 + 0x65], ah 
                                        ; XREF: 0x001EFAC4 (cond_jump)
  0x001EFB3B  7320                    jae      0x1efb5d                       
  0x001EFB3D  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB3E  6f                      outsd    dx, dword ptr [esi]            
  0x001EFB3F  7420                    je       0x1efb61                       
  0x001EFB41  7472                    je       0x1efbb5                       
  0x001EFB43  61                      popal                                   
                                        ; XREF: 0x001EFACD (cond_jump)
  0x001EFB44  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB45  736d                    jae      0x1efbb4                       
  0x001EFB47  697420616c6c2064        imul     esi, dword ptr [eax + 0x61], 0x64206c6c 
                                        ; XREF: 0x001EFAE4 (cond_jump)
  0x001EFB4F  61                      popal                                   
  0x001EFB50  7461                    je       0x1efbb3                       
  0x001EFB52  0000                    add      byte ptr [eax], al             
  0x001EFB54  43                      inc      ebx                            
  0x001EFB55  61                      popal                                   
  0x001EFB56  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB57  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB58  6f                      outsd    dx, dword ptr [esi]            
  0x001EFB59  7420                    je       0x1efb7b                       
  0x001EFB5B  7472                    je       0x1efbcf                       
                                        ; XREF: 0x001EFB3B (cond_jump)
  0x001EFB5D  61                      popal                                   
  0x001EFB5E  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB5F  7363                    jae      0x1efbc4                       
                                        ; XREF: 0x001EFB3F (cond_jump)
  0x001EFB61  6f                      outsd    dx, dword ptr [esi]            
  0x001EFB62  646520647565            and      byte ptr gs:[ebp + esi*2 + 0x65], ah 
  0x001EFB68  20746f20                and      byte ptr [edi + ebp*2 + 0x20], dh 
  0x001EFB6C  6d                      insd     dword ptr es:[edi], dx         
  0x001EFB6D  756c                    jne      0x1efbdb                       
  0x001EFB6F  7469                    je       0x1efbda                       
  0x001EFB71  706c                    jo       0x1efbdf                       
  0x001EFB73  65207573                and      byte ptr gs:[ebp + 0x73], dh   
  0x001EFB77  65206f66                and      byte ptr gs:[edi + 0x66], ch   
                                        ; XREF: 0x001EFB18 (cond_jump), 0x001EFB59 (cond_jump)
  0x001EFB7B  207175                  and      byte ptr [ecx + 0x75], dh      
  0x001EFB7E  61                      popal                                   
  0x001EFB7F  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB80  7469                    je       0x1efbeb                       
  0x001EFB82  7a61                    jp       0x1efbe5                       
  0x001EFB84  7469                    je       0x1efbef                       
  0x001EFB86  6f                      outsd    dx, dword ptr [esi]            
  0x001EFB87  6e                      outsb    dx, byte ptr [esi]             
  0x001EFB88  20746162                and      byte ptr [ecx + 0x62], dh      
  0x001EFB8C  6c                      insb     byte ptr es:[edi], dx          
  0x001EFB8D  65202564000000          and      byte ptr gs:[0x64], ah         
  0x001EFB94  50                      push     eax                            
  0x001EFB95  7265                    jb       0x1efbfc                       
  0x001EFB97  6d                      insd     dword ptr es:[edi], dx         
  0x001EFB98  61                      popal                                   
  0x001EFB99  7475                    je       0x1efc10                       
  0x001EFB9B  7265                    jb       0x1efc02                       
  0x001EFB9D  20656e                  and      byte ptr [ebp + 0x6e], ah      
  0x001EFBA0  64206f66                and      byte ptr fs:[edi + 0x66], ch   
  0x001EFBA4  20696e                  and      byte ptr [ecx + 0x6e], ch      
  0x001EFBA7  7075                    jo       0x1efc1e                       
  0x001EFBA9  7420                    je       0x1efbcb                       
                                        ; XREF: 0x001EFB35 (cond_jump)
  0x001EFBAB  66696c6500456d          imul     bp, word ptr [ebp], 0x6d45     
  0x001EFBB2  7074                    jo       0x1efc28                       
                                        ; XREF: 0x001EFB45 (cond_jump)
  0x001EFBB4  7920                    jns      0x1efbd6                       
  0x001EFBB6  696e7075742066          imul     ebp, dword ptr [esi + 0x70], 0x66207475 
  0x001EFBBD  696c65000000004d        imul     ebp, dword ptr [ebp], 0x4d000000 
  0x001EFBC5  61                      popal                                   
  0x001EFBC6  7869                    js       0x1efc31                       
  0x001EFBC8  6d                      insd     dword ptr es:[edi], dx         
  0x001EFBC9  756d                    jne      0x1efc38                       
                                        ; XREF: 0x001EFBA9 (cond_jump)
  0x001EFBCB  207375                  and      byte ptr [ebx + 0x75], dh      
  0x001EFBCE  7070                    jo       0x1efc40                       
  0x001EFBD0  6f                      outsd    dx, dword ptr [esi]            
  0x001EFBD1  7274                    jb       0x1efc47                       
  0x001EFBD3  656420696d              and      byte ptr fs:[ecx + 0x6d], ch   
  0x001EFBD8  61                      popal                                   
  0x001EFBD9  6765206469              and      byte ptr gs:[si + 0x69], ah    
  0x001EFBDE  6d                      insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EFB71 (cond_jump)
  0x001EFBDF  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EFBE1  7369                    jae      0x1efc4c                       
  0x001EFBE3  6f                      outsd    dx, dword ptr [esi]            
  0x001EFBE4  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EFB82 (cond_jump)
  0x001EFBE5  206973                  and      byte ptr [ecx + 0x73], ch      
  0x001EFBE8  202575207069            and      byte ptr [0x69702075], ah      
  0x001EFBEE  7865                    js       0x1efc55                       
  0x001EFBF0  6c                      insb     byte ptr es:[edi], dx          
  0x001EFBF1  7300                    jae      0x1efbf3                       
                                        ; XREF: 0x001EFBF1 (cond_jump)
  0x001EFBF3  004d69                  add      byte ptr [ebp + 0x69], cl      
  0x001EFBF6  7373                    jae      0x1efc6b                       
  0x001EFBF8  696e6720487566          imul     ebp, dword ptr [esi + 0x67], 0x66754820 
  0x001EFBFF  666d                    insw     word ptr es:[edi], dx          
  0x001EFC01  61                      popal                                   
                                        ; XREF: 0x001EFB9B (cond_jump)
  0x001EFC02  6e                      outsb    dx, byte ptr [esi]             
  0x001EFC03  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EFC06  646520746162            and      byte ptr gs:[ecx + 0x62], dh   
  0x001EFC0C  6c                      insb     byte ptr es:[edi], dx          
  0x001EFC0D  6520656e                and      byte ptr gs:[ebp + 0x6e], ah   
  0x001EFC11  7472                    je       0x1efc85                       
  0x001EFC13  7900                    jns      0x1efc15                       
                                        ; XREF: 0x001EFC13 (cond_jump)
  0x001EFC15  0000                    add      byte ptr [eax], al             
  0x001EFC17  004875                  add      byte ptr [eax + 0x75], cl      
  0x001EFC1A  66666d                  insw     word ptr es:[edi], dx          
  0x001EFC1D  61                      popal                                   
                                        ; XREF: 0x001EFBA7 (cond_jump)
  0x001EFC1E  6e                      outsb    dx, byte ptr [esi]             
  0x001EFC1F  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EFC22  6465207369              and      byte ptr gs:[ebx + 0x69], dh   
  0x001EFC27  7a65                    jp       0x1efc8e                       
  0x001EFC29  20746162                and      byte ptr [ecx + 0x62], dh      
  0x001EFC2D  6c                      insb     byte ptr es:[edi], dx          
  0x001EFC2E  65206f76                and      byte ptr gs:[edi + 0x76], ch   
  0x001EFC32  657266                  jb       0x1efc9b                       
  0x001EFC35  6c                      insb     byte ptr es:[edi], dx          
  0x001EFC36  6f                      outsd    dx, dword ptr [esi]            
  0x001EFC37  7700                    ja       0x1efc39                       
                                        ; XREF: 0x001EFC37 (cond_jump)
  0x001EFC39  0000                    add      byte ptr [eax], al             
  0x001EFC3B  004672                  add      byte ptr [esi + 0x72], al      
  0x001EFC3E  61                      popal                                   
  0x001EFC3F  6374696f                arpl     word ptr [ecx + ebp*2 + 0x6f], si 
  0x001EFC43  6e                      outsb    dx, byte ptr [esi]             
  0x001EFC44  61                      popal                                   
  0x001EFC45  6c                      insb     byte ptr es:[edi], dx          
  0x001EFC46  207361                  and      byte ptr [ebx + 0x61], dh      
  0x001EFC49  6d                      insd     dword ptr es:[edi], dx         
  0x001EFC4A  706c                    jo       0x1efcb8                       
                                        ; XREF: 0x001EFBE1 (cond_jump)
  0x001EFC4C  696e67206e6f74          imul     ebp, dword ptr [esi + 0x67], 0x746f6e20 
  0x001EFC53  20696d                  and      byte ptr [ecx + 0x6d], ch      
  0x001EFC56  706c                    jo       0x1efcc4                       
  0x001EFC58  656d                    insd     dword ptr es:[edi], dx         
  0x001EFC5A  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EFC5C  7465                    je       0x1efcc3                       
  0x001EFC5E  64207965                and      byte ptr fs:[ecx + 0x65], bh   
  0x001EFC62  7400                    je       0x1efc64                       
                                        ; XREF: 0x001EFC62 (cond_jump)
  0x001EFC64  4f                      dec      edi                            
  0x001EFC65  7574                    jne      0x1efcdb                       
  0x001EFC67  7075                    jo       0x1efcde                       
  0x001EFC69  7420                    je       0x1efc8b                       
                                        ; XREF: 0x001EFBF6 (cond_jump)
  0x001EFC6B  66696c65207772          imul     bp, word ptr [ebp + 0x20], 0x7277 
  0x001EFC72  697465206572726f        imul     esi, dword ptr [ebp + 0x20], 0x6f727265 
  0x001EFC7A  7220                    jb       0x1efc9c                       
  0x001EFC7C  2d2d2d206f              sub      eax, 0x6f202d2d                
  0x001EFC81  7574                    jne      0x1efcf7                       
  0x001EFC83  206f66                  and      byte ptr [edi + 0x66], ch      
  0x001EFC86  20646973                and      byte ptr [ecx + ebp*2 + 0x73], ah 
  0x001EFC8A  6b2073                  imul     esp, dword ptr [eax], 0x73     
  0x001EFC8D  7061                    jo       0x1efcf0                       
  0x001EFC8F  63653f                  arpl     word ptr [ebp + 0x3f], sp      
  0x001EFC92  0000                    add      byte ptr [eax], al             
  0x001EFC94  49                      dec      ecx                            
  0x001EFC95  6e                      outsb    dx, byte ptr [esi]             
  0x001EFC96  7075                    jo       0x1efd0d                       
  0x001EFC98  7420                    je       0x1efcba                       
  0x001EFC9A  66696c65207265          imul     bp, word ptr [ebp + 0x20], 0x6572 
  0x001EFCA1  61                      popal                                   
  0x001EFCA2  64206572                and      byte ptr fs:[ebp + 0x72], ah   
  0x001EFCA6  726f                    jb       0x1efd17                       
  0x001EFCA8  7200                    jb       0x1efcaa                       
                                        ; XREF: 0x001EFCA8 (cond_jump)
  0x001EFCAA  0000                    add      byte ptr [eax], al             
  0x001EFCAC  44                      inc      esp                            
  0x001EFCAD  69646e2774206578        imul     esp, dword ptr [esi + ebp*2 + 0x27], 0x78652074 
  0x001EFCB5  7065                    jo       0x1efd1c                       
  0x001EFCB7  6374206d                arpl     word ptr [eax + 0x6d], si      
  0x001EFCBB  6f                      outsd    dx, dword ptr [esi]            
  0x001EFCBC  7265                    jb       0x1efd23                       
  0x001EFCBE  20746861                and      byte ptr [eax + ebp*2 + 0x61], dh 
  0x001EFCC2  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EFC5C (cond_jump)
  0x001EFCC3  206f6e                  and      byte ptr [edi + 0x6e], ch      
  0x001EFCC6  65207363                and      byte ptr gs:[ebx + 0x63], dh   
  0x001EFCCA  61                      popal                                   
  0x001EFCCB  6e                      outsb    dx, byte ptr [esi]             
  0x001EFCCC  0000                    add      byte ptr [eax], al             
  0x001EFCCE  0000                    add      byte ptr [eax], al             
  0x001EFCD0  57                      push     edi                            
  0x001EFCD1  7269                    jb       0x1efd3c                       
  0x001EFCD3  7465                    je       0x1efd3a                       
  0x001EFCD5  20746f20                and      byte ptr [edi + ebp*2 + 0x20], dh 
  0x001EFCD9  45                      inc      ebp                            
  0x001EFCDA  4d                      dec      ebp                            
                                        ; XREF: 0x001EFC65 (cond_jump)
  0x001EFCDB  53                      push     ebx                            
  0x001EFCDC  206661                  and      byte ptr [esi + 0x61], ah      
  0x001EFCDF  696c656400526561        imul     ebp, dword ptr [ebp + 0x64], 0x61655200 
  0x001EFCE7  64206672                and      byte ptr fs:[esi + 0x72], ah   
  0x001EFCEB  6f                      outsd    dx, dword ptr [esi]            
  0x001EFCEC  6d                      insd     dword ptr es:[edi], dx         
  0x001EFCED  20454d                  and      byte ptr [ebp + 0x4d], al      
                                        ; XREF: 0x001EFC8D (cond_jump)
  0x001EFCF0  53                      push     ebx                            
  0x001EFCF1  206661                  and      byte ptr [esi + 0x61], ah      
  0x001EFCF4  696c656400000000        imul     ebp, dword ptr [ebp + 0x64], 0 
  0x001EFCFC  45                      inc      ebp                            
  0x001EFCFD  6d                      insd     dword ptr es:[edi], dx         
  0x001EFCFE  7074                    jo       0x1efd74                       
  0x001EFD00  7920                    jns      0x1efd22                       
  0x001EFD02  4a                      dec      edx                            
  0x001EFD03  50                      push     eax                            
  0x001EFD04  45                      inc      ebp                            
  0x001EFD05  47                      inc      edi                            
  0x001EFD06  20696d                  and      byte ptr [ecx + 0x6d], ch      
  0x001EFD09  61                      popal                                   
  0x001EFD0A  67652028                and      byte ptr gs:[bx + si], ch      
  0x001EFD0E  44                      inc      esp                            
  0x001EFD0F  4e                      dec      esi                            
  0x001EFD10  4c                      dec      esp                            
  0x001EFD11  206e6f                  and      byte ptr [esi + 0x6f], ch      
  0x001EFD14  7420                    je       0x1efd36                       
  0x001EFD16  7375                    jae      0x1efd8d                       
  0x001EFD18  7070                    jo       0x1efd8a                       
  0x001EFD1A  6f                      outsd    dx, dword ptr [esi]            
  0x001EFD1B  7274                    jb       0x1efd91                       
  0x001EFD1D  65642900                sub      dword ptr fs:[eax], eax        
  0x001EFD21  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001EFCBC (cond_jump)
  0x001EFD23  00426f                  add      byte ptr [edx + 0x6f], al      
  0x001EFD26  677573                  jne      0x1efd9c                       
  0x001EFD29  20445154                and      byte ptr [ecx + edx*2 + 0x54], al 
  0x001EFD2D  20696e                  and      byte ptr [ecx + 0x6e], ch      
  0x001EFD30  64657820                js       0x1efd54                       
  0x001EFD34  2564000042              and      eax, 0x42000064                
  0x001EFD39  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EFCD3 (cond_jump)
  0x001EFD3A  677573                  jne      0x1efdb0                       
  0x001EFD3D  20444854                and      byte ptr [eax + ecx*2 + 0x54], al 
  0x001EFD41  20696e                  and      byte ptr [ecx + 0x6e], ch      
  0x001EFD44  64657820                js       0x1efd68                       
  0x001EFD48  2564000042              and      eax, 0x42000064                
  0x001EFD4D  6f                      outsd    dx, dword ptr [esi]            
  0x001EFD4E  677573                  jne      0x1efdc4                       
  0x001EFD51  20444854                and      byte ptr [eax + ecx*2 + 0x54], al 
  0x001EFD55  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EFD58  756e                    jne      0x1efdc8                       
  0x001EFD5A  7473                    je       0x1efdcf                       
  0x001EFD5C  0000                    add      byte ptr [eax], al             
  0x001EFD5E  0000                    add      byte ptr [eax], al             
  0x001EFD60  42                      inc      edx                            
  0x001EFD61  6f                      outsd    dx, dword ptr [esi]            
  0x001EFD62  677573                  jne      0x1efdd8                       
  0x001EFD65  20444143                and      byte ptr [ecx + eax*2 + 0x43], al 
  0x001EFD69  207661                  and      byte ptr [esi + 0x61], dh      
  0x001EFD6C  6c                      insb     byte ptr es:[edi], dx          
  0x001EFD6D  7565                    jne      0x1efdd4                       
  0x001EFD6F  2030                    and      byte ptr [eax], dh             
  0x001EFD71  7825                    js       0x1efd98                       
  0x001EFD73  7800                    js       0x1efd75                       
                                        ; XREF: 0x001EFD73 (cond_jump)
  0x001EFD75  0000                    add      byte ptr [eax], al             
  0x001EFD77  00426f                  add      byte ptr [edx + 0x6f], al      
  0x001EFD7A  677573                  jne      0x1efdf0                       
  0x001EFD7D  20444143                and      byte ptr [ecx + eax*2 + 0x43], al 
  0x001EFD81  20696e                  and      byte ptr [ecx + 0x6e], ch      
  0x001EFD84  64657820                js       0x1efda8                       
  0x001EFD88  2564000055              and      eax, 0x55000064                
                                        ; XREF: 0x001EFD16 (cond_jump)
  0x001EFD8D  6e                      outsb    dx, byte ptr [esi]             
  0x001EFD8E  7375                    jae      0x1efe05                       
  0x001EFD90  7070                    jo       0x1efe02                       
  0x001EFD92  6f                      outsd    dx, dword ptr [esi]            
  0x001EFD93  7274                    jb       0x1efe09                       
  0x001EFD95  656420636f              and      byte ptr fs:[ebx + 0x6f], ah   
  0x001EFD9A  6c                      insb     byte ptr es:[edi], dx          
  0x001EFD9B  6f                      outsd    dx, dword ptr [esi]            
                                        ; XREF: 0x001EFD26 (cond_jump)
  0x001EFD9C  7220                    jb       0x1efdbe                       
  0x001EFD9E  636f6e                  arpl     word ptr [edi + 0x6e], bp      
  0x001EFDA1  7665                    jbe      0x1efe08                       
  0x001EFDA3  7273                    jb       0x1efe18                       
  0x001EFDA5  696f6e20726571          imul     ebp, dword ptr [edi + 0x6e], 0x71657220 
  0x001EFDAC  7565                    jne      0x1efe13                       
  0x001EFDAE  7374                    jae      0x1efe24                       
                                        ; XREF: 0x001EFD3A (cond_jump)
  0x001EFDB0  0000                    add      byte ptr [eax], al             
  0x001EFDB2  0000                    add      byte ptr [eax], al             
  0x001EFDB4  54                      push     esp                            
  0x001EFDB5  6f                      outsd    dx, dword ptr [esi]            
  0x001EFDB6  6f                      outsd    dx, dword ptr [esi]            
  0x001EFDB7  206d61                  and      byte ptr [ebp + 0x61], ch      
  0x001EFDBA  6e                      outsb    dx, byte ptr [esi]             
  0x001EFDBB  7920                    jns      0x1efddd                       
  0x001EFDBD  636f6c                  arpl     word ptr [edi + 0x6c], bp      
  0x001EFDC0  6f                      outsd    dx, dword ptr [esi]            
  0x001EFDC1  7220                    jb       0x1efde3                       
  0x001EFDC3  636f6d                  arpl     word ptr [edi + 0x6d], bp      
  0x001EFDC6  706f                    jo       0x1efe37                       
                                        ; XREF: 0x001EFD58 (cond_jump)
  0x001EFDC8  6e                      outsb    dx, byte ptr [esi]             
  0x001EFDC9  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EFDCB  7473                    je       0x1efe40                       
  0x001EFDCD  3a20                    cmp      ah, byte ptr [eax]             
                                        ; XREF: 0x001EFD5A (cond_jump)
  0x001EFDCF  25642c206d              and      eax, 0x6d202c64                
                                        ; XREF: 0x001EFD6D (cond_jump)
  0x001EFDD4  61                      popal                                   
  0x001EFDD5  7820                    js       0x1efdf7                       
  0x001EFDD7  2564000000              and      eax, 0x64                      
  0x001EFDDC  43                      inc      ebx                            
                                        ; XREF: 0x001EFDBB (cond_jump)
  0x001EFDDD  43                      inc      ebx                            
  0x001EFDDE  49                      dec      ecx                            
  0x001EFDDF  52                      push     edx                            
  0x001EFDE0  363031                  xor      byte ptr ss:[ecx], dh          
                                        ; XREF: 0x001EFDC1 (cond_jump)
  0x001EFDE3  207361                  and      byte ptr [ebx + 0x61], dh      
  0x001EFDE6  6d                      insd     dword ptr es:[edi], dx         
  0x001EFDE7  706c                    jo       0x1efe55                       
  0x001EFDE9  696e67206e6f74          imul     ebp, dword ptr [esi + 0x67], 0x746f6e20 
                                        ; XREF: 0x001EFD7A (cond_jump)
  0x001EFDF0  20696d                  and      byte ptr [ecx + 0x6d], ch      
  0x001EFDF3  706c                    jo       0x1efe61                       
  0x001EFDF5  656d                    insd     dword ptr es:[edi], dx         
                                        ; XREF: 0x001EFDD5 (cond_jump)
  0x001EFDF7  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EFDF9  7465                    je       0x1efe60                       
  0x001EFDFB  64207965                and      byte ptr fs:[ecx + 0x65], bh   
  0x001EFDFF  7400                    je       0x1efe01                       
                                        ; XREF: 0x001EFDFF (cond_jump)
  0x001EFE01  0000                    add      byte ptr [eax], al             
  0x001EFE03  005375                  add      byte ptr [ebx + 0x75], dl      
  0x001EFE06  7370                    jae      0x1efe78                       
                                        ; XREF: 0x001EFDA1 (cond_jump)
  0x001EFE08  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001EFE0A  7369                    jae      0x1efe75                       
  0x001EFE0C  6f                      outsd    dx, dword ptr [esi]            
  0x001EFE0D  6e                      outsb    dx, byte ptr [esi]             
  0x001EFE0E  206e6f                  and      byte ptr [esi + 0x6f], ch      
  0x001EFE11  7420                    je       0x1efe33                       
                                        ; XREF: 0x001EFDAC (cond_jump)
  0x001EFE13  61                      popal                                   
  0x001EFE14  6c                      insb     byte ptr es:[edi], dx          
  0x001EFE15  6c                      insb     byte ptr es:[edi], dx          
  0x001EFE16  6f                      outsd    dx, dword ptr [esi]            
  0x001EFE17  7765                    ja       0x1efe7e                       
  0x001EFE19  64206865                and      byte ptr fs:[eax + 0x65], ch   
  0x001EFE1D  7265                    jb       0x1efe84                       
  0x001EFE1F  004275                  add      byte ptr [edx + 0x75], al      
  0x001EFE22  6666657220              jb       0x1efe47                       
  0x001EFE27  7061                    jo       0x1efe8a                       
  0x001EFE29  7373                    jae      0x1efe9e                       
  0x001EFE2B  656420746f20            and      byte ptr fs:[edi + ebp*2 + 0x20], dh 
  0x001EFE31  4a                      dec      edx                            
  0x001EFE32  50                      push     eax                            
                                        ; XREF: 0x001EFE11 (cond_jump)
  0x001EFE33  45                      inc      ebp                            
  0x001EFE34  47                      inc      edi                            
  0x001EFE35  206c6962                and      byte ptr [ecx + ebp*2 + 0x62], ch 
  0x001EFE39  7261                    jb       0x1efe9c                       
  0x001EFE3B  7279                    jb       0x1efeb6                       
  0x001EFE3D  206973                  and      byte ptr [ecx + 0x73], ch      
                                        ; XREF: 0x001EFDCB (cond_jump)
  0x001EFE40  20746f6f                and      byte ptr [edi + ebp*2 + 0x6f], dh 
  0x001EFE44  20736d                  and      byte ptr [ebx + 0x6d], dh      
                                        ; XREF: 0x001EFE22 (cond_jump)
  0x001EFE47  61                      popal                                   
  0x001EFE48  6c                      insb     byte ptr es:[edi], dx          
  0x001EFE49  6c                      insb     byte ptr es:[edi], dx          
  0x001EFE4A  0000                    add      byte ptr [eax], al             
  0x001EFE4C  42                      inc      edx                            
  0x001EFE4D  6f                      outsd    dx, dword ptr [esi]            
  0x001EFE4E  677573                  jne      0x1efec4                       
  0x001EFE51  207669                  and      byte ptr [esi + 0x69], dh      
  0x001EFE54  7274                    jb       0x1efeca                       
  0x001EFE56  7561                    jne      0x1efeb9                       
  0x001EFE58  6c                      insb     byte ptr es:[edi], dx          
  0x001EFE59  206172                  and      byte ptr [ecx + 0x72], ah      
  0x001EFE5C  7261                    jb       0x1efebf                       
  0x001EFE5E  7920                    jns      0x1efe80                       
                                        ; XREF: 0x001EFDF9 (cond_jump)
  0x001EFE60  61                      popal                                   
                                        ; XREF: 0x001EFDF3 (cond_jump)
  0x001EFE61  636365                  arpl     word ptr [ebx + 0x65], sp      
  0x001EFE64  7373                    jae      0x1efed9                       
  0x001EFE66  0000                    add      byte ptr [eax], al             
  0x001EFE68  4a                      dec      edx                            
  0x001EFE69  50                      push     eax                            
  0x001EFE6A  45                      inc      ebp                            
  0x001EFE6B  47                      inc      edi                            
  0x001EFE6C  207061                  and      byte ptr [eax + 0x61], dh      
  0x001EFE6F  7261                    jb       0x1efed2                       
  0x001EFE71  6d                      insd     dword ptr es:[edi], dx         
  0x001EFE72  657465                  je       0x1efeda                       
                                        ; XREF: 0x001EFE0A (cond_jump)
  0x001EFE75  7220                    jb       0x1efe97                       
  0x001EFE77  7374                    jae      0x1efeed                       
  0x001EFE79  7275                    jb       0x1efef0                       
  0x001EFE7B  6374206d                arpl     word ptr [eax + 0x6d], si      
  0x001EFE7F  69736d61746368          imul     esi, dword ptr [ebx + 0x6d], 0x68637461 
  0x001EFE86  3a20                    cmp      ah, byte ptr [eax]             
  0x001EFE88  6c                      insb     byte ptr es:[edi], dx          
  0x001EFE89  69627261727920          imul     esp, dword ptr [edx + 0x72], 0x20797261 
  0x001EFE90  7468                    je       0x1efefa                       
  0x001EFE92  696e6b73207369          imul     ebp, dword ptr [esi + 0x6b], 0x69732073 
  0x001EFE99  7a65                    jp       0x1eff00                       
  0x001EFE9B  206973                  and      byte ptr [ecx + 0x73], ch      
                                        ; XREF: 0x001EFE29 (cond_jump)
  0x001EFE9E  2025752c2063            and      byte ptr [0x63202c75], ah      
  0x001EFEA4  61                      popal                                   
  0x001EFEA5  6c                      insb     byte ptr es:[edi], dx          
  0x001EFEA6  6c                      insb     byte ptr es:[edi], dx          
  0x001EFEA7  657220                  jb       0x1efeca                       
  0x001EFEAA  657870                  js       0x1eff1d                       
  0x001EFEAD  6563747320              arpl     word ptr gs:[ebx + esi*2 + 0x20], si 
  0x001EFEB2  2575000000              and      eax, 0x75                      
  0x001EFEB7  00496d                  add      byte ptr [ecx + 0x6d], cl      
  0x001EFEBA  7072                    jo       0x1eff2e                       
  0x001EFEBC  6f                      outsd    dx, dword ptr [esi]            
  0x001EFEBD  7065                    jo       0x1eff24                       
                                        ; XREF: 0x001EFE5C (cond_jump)
  0x001EFEBF  7220                    jb       0x1efee1                       
  0x001EFEC1  63616c                  arpl     word ptr [ecx + 0x6c], sp      
                                        ; XREF: 0x001EFE4E (cond_jump)
  0x001EFEC4  6c                      insb     byte ptr es:[edi], dx          
  0x001EFEC5  20746f20                and      byte ptr [edi + ebp*2 + 0x20], dh 
  0x001EFEC9  4a                      dec      edx                            
                                        ; XREF: 0x001EFE54 (cond_jump), 0x001EFEA7 (cond_jump)
  0x001EFECA  50                      push     eax                            
  0x001EFECB  45                      inc      ebp                            
  0x001EFECC  47                      inc      edi                            
  0x001EFECD  206c6962                and      byte ptr [ecx + ebp*2 + 0x62], ch 
  0x001EFED1  7261                    jb       0x1eff34                       
  0x001EFED3  7279                    jb       0x1eff4e                       
  0x001EFED5  20696e                  and      byte ptr [ecx + 0x6e], ch      
  0x001EFED8  207374                  and      byte ptr [ebx + 0x74], dh      
  0x001EFEDB  61                      popal                                   
  0x001EFEDC  7465                    je       0x1eff43                       
  0x001EFEDE  202564000000            and      byte ptr [0x64], ah            
  0x001EFEE4  49                      dec      ecx                            
  0x001EFEE5  6e                      outsb    dx, byte ptr [esi]             
  0x001EFEE6  7661                    jbe      0x1eff49                       
  0x001EFEE8  6c                      insb     byte ptr es:[edi], dx          
  0x001EFEE9  6964207363616e20        imul     esp, dword ptr [eax + 0x73], 0x206e6163 
  0x001EFEF1  7363                    jae      0x1eff56                       
  0x001EFEF3  7269                    jb       0x1eff5e                       
  0x001EFEF5  7074                    jo       0x1eff6b                       
  0x001EFEF7  206174                  and      byte ptr [ecx + 0x74], ah      
                                        ; XREF: 0x001EFE90 (cond_jump)
  0x001EFEFA  20656e                  and      byte ptr [ebp + 0x6e], ah      
  0x001EFEFD  7472                    je       0x1eff71                       
  0x001EFEFF  7920                    jns      0x1eff21                       
  0x001EFF01  256400426f              and      eax, 0x6f420064                
  0x001EFF06  677573                  jne      0x1eff7c                       
  0x001EFF09  207361                  and      byte ptr [ebx + 0x61], dh      
  0x001EFF0C  6d                      insd     dword ptr es:[edi], dx         
  0x001EFF0D  706c                    jo       0x1eff7b                       
  0x001EFF0F  696e6720666163          imul     ebp, dword ptr [esi + 0x67], 0x63616620 
  0x001EFF16  746f                    je       0x1eff87                       
  0x001EFF18  7273                    jb       0x1eff8d                       
  0x001EFF1A  0000                    add      byte ptr [eax], al             
  0x001EFF1C  49                      dec      ecx                            
                                        ; XREF: 0x001EFEAA (cond_jump)
  0x001EFF1D  6e                      outsb    dx, byte ptr [esi]             
  0x001EFF1E  7661                    jbe      0x1eff81                       
  0x001EFF20  6c                      insb     byte ptr es:[edi], dx          
                                        ; XREF: 0x001EFEFF (cond_jump)
  0x001EFF21  69642070726f6772        imul     esp, dword ptr [eax + 0x70], 0x72676f72 
  0x001EFF29  657373                  jae      0x1eff9f                       
  0x001EFF2C  69766520706172          imul     esi, dword ptr [esi + 0x65], 0x72617020 
  0x001EFF33  61                      popal                                   
                                        ; XREF: 0x001EFED1 (cond_jump)
  0x001EFF34  6d                      insd     dword ptr es:[edi], dx         
  0x001EFF35  657465                  je       0x1eff9d                       
  0x001EFF38  7273                    jb       0x1effad                       
  0x001EFF3A  206174                  and      byte ptr [ecx + 0x74], ah      
  0x001EFF3D  207363                  and      byte ptr [ebx + 0x63], dh      
  0x001EFF40  61                      popal                                   
  0x001EFF41  6e                      outsb    dx, byte ptr [esi]             
  0x001EFF42  207363                  and      byte ptr [ebx + 0x63], dh      
  0x001EFF45  7269                    jb       0x1effb0                       
  0x001EFF47  7074                    jo       0x1effbd                       
                                        ; XREF: 0x001EFEE6 (cond_jump)
  0x001EFF49  20656e                  and      byte ptr [ebp + 0x6e], ah      
  0x001EFF4C  7472                    je       0x1effc0                       
                                        ; XREF: 0x001EFED3 (cond_jump)
  0x001EFF4E  7920                    jns      0x1eff70                       
  0x001EFF50  2564000049              and      eax, 0x49000064                
  0x001EFF55  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001EFEF1 (cond_jump)
  0x001EFF56  7661                    jbe      0x1effb9                       
  0x001EFF58  6c                      insb     byte ptr es:[edi], dx          
  0x001EFF59  69642070726f6772        imul     esp, dword ptr [eax + 0x70], 0x72676f72 
  0x001EFF61  657373                  jae      0x1effd7                       
  0x001EFF64  69766520706172          imul     esi, dword ptr [esi + 0x65], 0x72617020 
                                        ; XREF: 0x001EFEF5 (cond_jump)
  0x001EFF6B  61                      popal                                   
  0x001EFF6C  6d                      insd     dword ptr es:[edi], dx         
  0x001EFF6D  657465                  je       0x1effd5                       
                                        ; XREF: 0x001EFF4E (cond_jump)
  0x001EFF70  7273                    jb       0x1effe5                       
  0x001EFF72  205373                  and      byte ptr [ebx + 0x73], dl      
  0x001EFF75  3d25642053              cmp      eax, 0x53206425                
  0x001EFF7A  653d25642041            cmp      eax, 0x41206425                
  0x001EFF80  683d256420              push     0x2064253d                     
  0x001EFF85  41                      inc      ecx                            
  0x001EFF86  6c                      insb     byte ptr es:[edi], dx          
                                        ; XREF: 0x001EFF16 (cond_jump)
  0x001EFF87  3d25640000              cmp      eax, 0x6425                    
  0x001EFF8C  55                      push     ebp                            
                                        ; XREF: 0x001EFF18 (cond_jump)
  0x001EFF8D  6e                      outsb    dx, byte ptr [esi]             
  0x001EFF8E  7375                    jae      0x1f0005                       
  0x001EFF90  7070                    jo       0x1f0002                       
  0x001EFF92  6f                      outsd    dx, dword ptr [esi]            
  0x001EFF93  7274                    jb       0x1f0009                       
  0x001EFF95  6564204a50              and      byte ptr fs:[edx + 0x50], cl   
  0x001EFF9A  45                      inc      ebp                            
  0x001EFF9B  47                      inc      edi                            
  0x001EFF9C  20646174                and      byte ptr [ecx + 0x74], ah      
  0x001EFFA0  61                      popal                                   
  0x001EFFA1  207072                  and      byte ptr [eax + 0x72], dh      
  0x001EFFA4  65636973                arpl     word ptr gs:[ecx + 0x73], bp   
  0x001EFFA8  696f6e20256400          imul     ebp, dword ptr [edi + 0x6e], 0x642520 
  0x001EFFAF  00496e                  add      byte ptr [ecx + 0x6e], cl      
  0x001EFFB2  7661                    jbe      0x1f0015                       
  0x001EFFB4  6c                      insb     byte ptr es:[edi], dx          
  0x001EFFB5  6964206d656d6f72        imul     esp, dword ptr [eax + 0x6d], 0x726f6d65 
                                        ; XREF: 0x001EFF47 (cond_jump)
  0x001EFFBD  7920                    jns      0x1effdf                       
  0x001EFFBF  706f                    jo       0x1f0030                       
  0x001EFFC1  6f                      outsd    dx, dword ptr [esi]            
  0x001EFFC2  6c                      insb     byte ptr es:[edi], dx          
  0x001EFFC3  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001EFFC6  6465202564005361        and      byte ptr gs:[0x61530064], ah   
  0x001EFFCE  6d                      insd     dword ptr es:[edi], dx         
  0x001EFFCF  706c                    jo       0x1f003d                       
  0x001EFFD1  696e6720666163          imul     ebp, dword ptr [esi + 0x67], 0x63616620 
  0x001EFFD8  746f                    je       0x1f0049                       
  0x001EFFDA  7273                    jb       0x1f004f                       
  0x001EFFDC  20746f6f                and      byte ptr [edi + ebp*2 + 0x6f], dh 
  0x001EFFE0  206c6172                and      byte ptr [ecx + 0x72], ch      
  0x001EFFE4  676520666f              and      byte ptr gs:[bp + 0x6f], ah    
  0x001EFFE9  7220                    jb       0x1f000b                       
  0x001EFFEB  696e7465726c65          imul     ebp, dword ptr [esi + 0x74], 0x656c7265 
  0x001EFFF2  61                      popal                                   
  0x001EFFF3  7665                    jbe      0x1f005a                       
  0x001EFFF5  64207363                and      byte ptr fs:[ebx + 0x63], dh   
  0x001EFFF9  61                      popal                                   
  0x001EFFFA  6e                      outsb    dx, byte ptr [esi]             
  0x001EFFFB  005772                  add      byte ptr [edi + 0x72], dl      
  0x001EFFFE  6f                      outsd    dx, dword ptr [esi]            
  0x001EFFFF  6e                      outsb    dx, byte ptr [esi]             
  0x001F0000  67204a50                and      byte ptr [bp + si + 0x50], cl  
  0x001F0004  45                      inc      ebp                            
                                        ; XREF: 0x001EFF8E (cond_jump)
  0x001F0005  47                      inc      edi                            
  0x001F0006  206c6962                and      byte ptr [ecx + ebp*2 + 0x62], ch 
  0x001F000A  7261                    jb       0x1f006d                       
  0x001F000C  7279                    jb       0x1f0087                       
  0x001F000E  207665                  and      byte ptr [esi + 0x65], dh      
  0x001F0011  7273                    jb       0x1f0086                       
  0x001F0013  696f6e3a206c69          imul     ebp, dword ptr [edi + 0x6e], 0x696c203a 
  0x001F001A  627261                  bound    esi, qword ptr [edx + 0x61]    
  0x001F001D  7279                    jb       0x1f0098                       
  0x001F001F  206973                  and      byte ptr [ecx + 0x73], ch      
  0x001F0022  2025642c2063            and      byte ptr [0x63202c64], ah      
  0x001F0028  61                      popal                                   
  0x001F0029  6c                      insb     byte ptr es:[edi], dx          
  0x001F002A  6c                      insb     byte ptr es:[edi], dx          
  0x001F002B  657220                  jb       0x1f004e                       
  0x001F002E  657870                  js       0x1f00a1                       
  0x001F0031  6563747320              arpl     word ptr gs:[ebx + esi*2 + 0x20], si 
  0x001F0036  2564000000              and      eax, 0x64                      
  0x001F003B  00426f                  add      byte ptr [edx + 0x6f], al      
  0x001F003E  677573                  jne      0x1f00b4                       
  0x001F0041  206d61                  and      byte ptr [ebp + 0x61], ch      
  0x001F0044  726b                    jb       0x1f00b1                       
  0x001F0046  657220                  jb       0x1f0069                       
                                        ; XREF: 0x001EFFD8 (cond_jump)
  0x001F0049  6c                      insb     byte ptr es:[edi], dx          
  0x001F004A  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001F004C  677468                  je       0x1f00b7                       
                                        ; XREF: 0x001EFFDA (cond_jump)
  0x001F004F  00426f                  add      byte ptr [edx + 0x6f], al      
  0x001F0052  677573                  jne      0x1f00c8                       
  0x001F0055  204a50                  and      byte ptr [edx + 0x50], cl      
  0x001F0058  45                      inc      ebp                            
  0x001F0059  47                      inc      edi                            
                                        ; XREF: 0x001EFFF3 (cond_jump)
  0x001F005A  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001F005D  6c                      insb     byte ptr es:[edi], dx          
  0x001F005E  6f                      outsd    dx, dword ptr [esi]            
  0x001F005F  7273                    jb       0x1f00d4                       
  0x001F0061  7061                    jo       0x1f00c4                       
  0x001F0063  636500                  arpl     word ptr [ebp], sp             
  0x001F0066  0000                    add      byte ptr [eax], al             
  0x001F0068  42                      inc      edx                            
                                        ; XREF: 0x001F0046 (cond_jump)
  0x001F0069  6f                      outsd    dx, dword ptr [esi]            
  0x001F006A  677573                  jne      0x1f00e0                       
                                        ; XREF: 0x001F000A (cond_jump)
  0x001F006D  20696e                  and      byte ptr [ecx + 0x6e], ch      
  0x001F0070  7075                    jo       0x1f00e7                       
  0x001F0072  7420                    je       0x1f0094                       
  0x001F0074  636f6c                  arpl     word ptr [edi + 0x6c], bp      
  0x001F0077  6f                      outsd    dx, dword ptr [esi]            
  0x001F0078  7273                    jb       0x1f00ed                       
  0x001F007A  7061                    jo       0x1f00dd                       
  0x001F007C  636500                  arpl     word ptr [ebp], sp             
  0x001F007F  004944                  add      byte ptr [ecx + 0x44], cl      
  0x001F0082  43                      inc      ebx                            
  0x001F0083  54                      push     esp                            
  0x001F0084  206f75                  and      byte ptr [edi + 0x75], ch      
                                        ; XREF: 0x001F000C (cond_jump)
  0x001F0087  7470                    je       0x1f00f9                       
  0x001F0089  7574                    jne      0x1f00ff                       
  0x001F008B  20626c                  and      byte ptr [edx + 0x6c], ah      
  0x001F008E  6f                      outsd    dx, dword ptr [esi]            
  0x001F008F  636b20                  arpl     word ptr [ebx + 0x20], bp      
  0x001F0092  7369                    jae      0x1f00fd                       
                                        ; XREF: 0x001F0072 (cond_jump)
  0x001F0094  7a65                    jp       0x1f00fb                       
  0x001F0096  202564206e6f            and      byte ptr [0x6f6e2064], ah      
  0x001F009C  7420                    je       0x1f00be                       
  0x001F009E  7375                    jae      0x1f0115                       
  0x001F00A0  7070                    jo       0x1f0112                       
  0x001F00A2  6f                      outsd    dx, dword ptr [esi]            
  0x001F00A3  7274                    jb       0x1f0119                       
  0x001F00A5  656400496e              add      byte ptr fs:[ecx + 0x6e], cl   
  0x001F00AA  7661                    jbe      0x1f010d                       
  0x001F00AC  6c                      insb     byte ptr es:[edi], dx          
  0x001F00AD  696420636f6d706f        imul     esp, dword ptr [eax + 0x63], 0x6f706d6f 
  0x001F00B5  6e                      outsb    dx, byte ptr [esi]             
  0x001F00B6  656e                    outsb    dx, byte ptr gs:[esi]          
  0x001F00B8  7420                    je       0x1f00da                       
  0x001F00BA  49                      dec      ecx                            
  0x001F00BB  44                      inc      esp                            
  0x001F00BC  20256420696e            and      byte ptr [0x6e692064], ah      
  0x001F00C2  20534f                  and      byte ptr [ebx + 0x4f], dl      
  0x001F00C5  53                      push     ebx                            
  0x001F00C6  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001F0052 (cond_jump)
  0x001F00C8  42                      inc      edx                            
  0x001F00C9  6f                      outsd    dx, dword ptr [esi]            
  0x001F00CA  677573                  jne      0x1f0140                       
  0x001F00CD  206275                  and      byte ptr [edx + 0x75], ah      
  0x001F00D0  6666657220              jb       0x1f00f5                       
  0x001F00D5  636f6e                  arpl     word ptr [edi + 0x6e], bp      
  0x001F00D8  7472                    je       0x1f014c                       
                                        ; XREF: 0x001F00B8 (cond_jump)
  0x001F00DA  6f                      outsd    dx, dword ptr [esi]            
  0x001F00DB  6c                      insb     byte ptr es:[edi], dx          
  0x001F00DC  206d6f                  and      byte ptr [ebp + 0x6f], ch      
  0x001F00DF  64650000                add      byte ptr gs:[eax], al          
  0x001F00E3  004d41                  add      byte ptr [ebp + 0x41], cl      
  0x001F00E6  58                      pop      eax                            
                                        ; XREF: 0x001F0070 (cond_jump)
  0x001F00E7  5f                      pop      edi                            
  0x001F00E8  41                      inc      ecx                            
  0x001F00E9  4c                      dec      esp                            
  0x001F00EA  4c                      dec      esp                            
  0x001F00EB  4f                      dec      edi                            
  0x001F00EC  43                      inc      ebx                            
                                        ; XREF: 0x001F0078 (cond_jump)
  0x001F00ED  5f                      pop      edi                            
  0x001F00EE  43                      inc      ebx                            
  0x001F00EF  48                      dec      eax                            
  0x001F00F0  55                      push     ebp                            
  0x001F00F1  4e                      dec      esi                            
  0x001F00F2  4b                      dec      ebx                            
  0x001F00F3  206973                  and      byte ptr [ecx + 0x73], ch      
  0x001F00F6  207772                  and      byte ptr [edi + 0x72], dh      
                                        ; XREF: 0x001F0087 (cond_jump)
  0x001F00F9  6f                      outsd    dx, dword ptr [esi]            
  0x001F00FA  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x001F0094 (cond_jump)
  0x001F00FB  672c20                  sub      al, 0x20                       
  0x001F00FE  706c                    jo       0x1f016c                       
  0x001F0100  6561                    popal                                   
  0x001F0102  7365                    jae      0x1f0169                       
  0x001F0104  206669                  and      byte ptr [esi + 0x69], ah      
  0x001F0107  7800                    js       0x1f0109                       
                                        ; XREF: 0x001F0107 (cond_jump)
  0x001F0109  0000                    add      byte ptr [eax], al             
  0x001F010B  00414c                  add      byte ptr [ecx + 0x4c], al      
  0x001F010E  49                      dec      ecx                            
  0x001F010F  47                      inc      edi                            
  0x001F0110  4e                      dec      esi                            
  0x001F0111  5f                      pop      edi                            
                                        ; XREF: 0x001F00A0 (cond_jump)
  0x001F0112  54                      push     esp                            
  0x001F0113  59                      pop      ecx                            
  0x001F0114  50                      push     eax                            
                                        ; XREF: 0x001F009E (cond_jump)
  0x001F0115  45                      inc      ebp                            
  0x001F0116  206973                  and      byte ptr [ecx + 0x73], ch      
                                        ; XREF: 0x001F00A3 (cond_jump)
  0x001F0119  207772                  and      byte ptr [edi + 0x72], dh      
  0x001F011C  6f                      outsd    dx, dword ptr [esi]            
  0x001F011D  6e                      outsb    dx, byte ptr [esi]             
  0x001F011E  672c20                  sub      al, 0x20                       
  0x001F0121  706c                    jo       0x1f018f                       
  0x001F0123  6561                    popal                                   
  0x001F0125  7365                    jae      0x1f018c                       
  0x001F0127  206669                  and      byte ptr [esi + 0x69], ah      
  0x001F012A  7800                    js       0x1f012c                       
                                        ; XREF: 0x001F012A (cond_jump)
  0x001F012C  53                      push     ebx                            
  0x001F012D  6f                      outsd    dx, dword ptr [esi]            
  0x001F012E  7272                    jb       0x1f01a2                       
  0x001F0130  792c                    jns      0x1f015e                       
  0x001F0132  20746865                and      byte ptr [eax + ebp*2 + 0x65], dh 
  0x001F0136  7265                    jb       0x1f019d                       
  0x001F0138  206172                  and      byte ptr [ecx + 0x72], ah      
  0x001F013B  65206c6567              and      byte ptr gs:[ebp + 0x67], ch   
                                        ; XREF: 0x001F00CA (cond_jump)
  0x001F0140  61                      popal                                   
  0x001F0141  6c                      insb     byte ptr es:[edi], dx          
  0x001F0142  207265                  and      byte ptr [edx + 0x65], dh      
  0x001F0145  7374                    jae      0x1f01bb                       
  0x001F0147  7269                    jb       0x1f01b2                       
  0x001F0149  6374696f                arpl     word ptr [ecx + ebp*2 + 0x6f], si 
  0x001F014D  6e                      outsb    dx, byte ptr [esi]             
  0x001F014E  7320                    jae      0x1f0170                       
  0x001F0150  6f                      outsd    dx, dword ptr [esi]            
  0x001F0151  6e                      outsb    dx, byte ptr [esi]             
  0x001F0152  206172                  and      byte ptr [ecx + 0x72], ah      
  0x001F0155  6974686d65746963        imul     esi, dword ptr [eax + ebp*2 + 0x6d], 0x63697465 
  0x001F015D  20636f                  and      byte ptr [ebx + 0x6f], ah      
  0x001F0160  64696e6700000000        imul     ebp, dword ptr fs:[esi + 0x67], 0 
  0x001F0168  42                      inc      edx                            
                                        ; XREF: 0x001F0102 (cond_jump)
  0x001F0169  6f                      outsd    dx, dword ptr [esi]            
  0x001F016A  677573                  jne      0x1f01e0                       
  0x001F016D  206d65                  and      byte ptr [ebp + 0x65], ch      
                                        ; XREF: 0x001F014E (cond_jump)
  0x001F0170  7373                    jae      0x1f01e5                       
  0x001F0172  61                      popal                                   
  0x001F0173  676520636f              and      byte ptr gs:[bp + di + 0x6f], ah 
  0x001F0178  6465202564000000        and      byte ptr gs:[0x64], ah         
  0x001F0180  3031                    xor      byte ptr [ecx], dh             
  0x001F0182  3233                    xor      dh, byte ptr [ebx]             
  0x001F0184  3435                    xor      al, 0x35                       
  0x001F0186  3637                    aaa                                     
  0x001F0188  3839                    cmp      byte ptr [ecx], bh             
  0x001F018A  41                      inc      ecx                            
  0x001F018B  42                      inc      edx                            
                                        ; XREF: 0x001F0125 (cond_jump)
  0x001F018C  43                      inc      ebx                            
  0x001F018D  44                      inc      esp                            
  0x001F018E  45                      inc      ebp                            
                                        ; XREF: 0x001F0121 (cond_jump)
  0x001F018F  46                      inc      esi                            
  0x001F0190  312e                    xor      dword ptr [esi], ebp           
  0x001F0192  302e                    xor      byte ptr [esi], ch             
  0x001F0194  3500000000              xor      eax, 0                         
  0x001F0199  0000                    add      byte ptr [eax], al             
  0x001F019B  008000000008            add      byte ptr [eax + 0x8000000], al 
  0x001F01A1  0000                    add      byte ptr [eax], al             
  0x001F01A3  008800000022            add      byte ptr [eax + 0x22000000], cl 
  0x001F01A9  0000                    add      byte ptr [eax], al             
  0x001F01AB  00aa00000055            add      byte ptr [edx + 0x55000000], ch 
  0x001F01B1  0000                    add      byte ptr [eax], al             
  0x001F01B3  00ff                    add      bh, bh                         
  0x001F01B5  0000                    add      byte ptr [eax], al             
  0x001F01B7  00ff                    add      bh, bh                         
  0x001F01B9  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001F0145 (cond_jump)
  0x001F01BB  000f                    add      byte ptr [edi], cl             
  0x001F01BD  0000                    add      byte ptr [eax], al             
  0x001F01BF  00ff                    add      bh, bh                         
  0x001F01C1  0000                    add      byte ptr [eax], al             
  0x001F01C3  0033                    add      byte ptr [ebx], dh             
  0x001F01C5  0000                    add      byte ptr [eax], al             
  0x001F01C7  00ff                    add      bh, bh                         
  0x001F01C9  0000                    add      byte ptr [eax], al             
  0x001F01CB  005500                  add      byte ptr [ebp], dl             
  0x001F01CE  0000                    add      byte ptr [eax], al             
  0x001F01D0  ff00                    inc      dword ptr [eax]                
  0x001F01D2  0000                    add      byte ptr [eax], al             
  0x001F01D4  49                      dec      ecx                            
  0x001F01D5  48                      dec      eax                            
  0x001F01D6  44                      inc      esp                            
  0x001F01D7  52                      push     edx                            
  0x001F01D8  0000                    add      byte ptr [eax], al             
  0x001F01DA  0000                    add      byte ptr [eax], al             
  0x001F01DC  49                      dec      ecx                            
  0x001F01DD  44                      inc      esp                            
  0x001F01DE  41                      inc      ecx                            
  0x001F01DF  54                      push     esp                            
                                        ; XREF: 0x001F016A (cond_jump)
  0x001F01E0  0000                    add      byte ptr [eax], al             
  0x001F01E2  0000                    add      byte ptr [eax], al             
  0x001F01E4  49                      dec      ecx                            
                                        ; XREF: 0x001F0170 (cond_jump)
  0x001F01E5  45                      inc      ebp                            
  0x001F01E6  4e                      dec      esi                            
  0x001F01E7  44                      inc      esp                            
  0x001F01E8  0000                    add      byte ptr [eax], al             
  0x001F01EA  0000                    add      byte ptr [eax], al             
  0x001F01EC  50                      push     eax                            
  0x001F01ED  4c                      dec      esp                            
  0x001F01EE  54                      push     esp                            
  0x001F01EF  45                      inc      ebp                            
  0x001F01F0  0000                    add      byte ptr [eax], al             
  0x001F01F2  0000                    add      byte ptr [eax], al             
  0x001F01F4  6741                    inc      ecx                            
  0x001F01F6  4d                      dec      ebp                            
  0x001F01F7  41                      inc      ecx                            
  0x001F01F8  0000                    add      byte ptr [eax], al             
  0x001F01FA  0000                    add      byte ptr [eax], al             
  0x001F01FC  7352                    jae      0x1f0250                       
  0x001F01FE  47                      inc      edi                            
  0x001F01FF  42                      inc      edx                            
  0x001F0200  0000                    add      byte ptr [eax], al             
  0x001F0202  0000                    add      byte ptr [eax], al             
  0x001F0204  7452                    je       0x1f0258                       
  0x001F0206  4e                      dec      esi                            
  0x001F0207  53                      push     ebx                            
  0x001F0208  0000                    add      byte ptr [eax], al             
  0x001F020A  0000                    add      byte ptr [eax], al             
  0x001F020C  89504e                  mov      dword ptr [eax + 0x4e], edx    
  0x001F020F  47                      inc      edi                            
  0x001F0210  0d0a1a0a00              or       eax, 0xa1a0a                   
  0x001F0215  0000                    add      byte ptr [eax], al             
  0x001F0217  0000                    add      byte ptr [eax], al             
  0x001F0219  0000                    add      byte ptr [eax], al             
  0x001F021B  0001                    add      byte ptr [ecx], al             
  0x001F021D  0000                    add      byte ptr [eax], al             
  0x001F021F  000500000006            add      byte ptr [0x6000000], al       
  0x001F0225  0000                    add      byte ptr [eax], al             
  0x001F0227  000e                    add      byte ptr [esi], cl             
  0x001F0229  0000                    add      byte ptr [eax], al             
  0x001F022B  000f                    add      byte ptr [edi], cl             
  0x001F022D  0000                    add      byte ptr [eax], al             
  0x001F022F  001b                    add      byte ptr [ebx], bl             
  0x001F0231  0000                    add      byte ptr [eax], al             
  0x001F0233  001c00                  add      byte ptr [eax + eax], bl       
  0x001F0236  0000                    add      byte ptr [eax], al             
  0x001F0238  0200                    add      al, byte ptr [eax]             
  0x001F023A  0000                    add      byte ptr [eax], al             
  0x001F023C  0400                    add      al, 0                          
  0x001F023E  0000                    add      byte ptr [eax], al             
  0x001F0240  07                      pop      es                             
  0x001F0241  0000                    add      byte ptr [eax], al             
  0x001F0243  000d00000010            add      byte ptr [0x10000000], cl      
  0x001F0249  0000                    add      byte ptr [eax], al             
  0x001F024B  001a                    add      byte ptr [edx], bl             
  0x001F024D  0000                    add      byte ptr [eax], al             
  0x001F024F  001d0000002a            add      byte ptr [0x2a000000], bl      
  0x001F0255  0000                    add      byte ptr [eax], al             
  0x001F0257  0003                    add      byte ptr [ebx], al             
  0x001F0259  0000                    add      byte ptr [eax], al             
  0x001F025B  0008                    add      byte ptr [eax], cl             
  0x001F025D  0000                    add      byte ptr [eax], al             
  0x001F025F  000c00                  add      byte ptr [eax + eax], cl       
  0x001F0262  0000                    add      byte ptr [eax], al             
  0x001F0264  1100                    adc      dword ptr [eax], eax           
  0x001F0266  0000                    add      byte ptr [eax], al             
  0x001F0268  1900                    sbb      dword ptr [eax], eax           
  0x001F026A  0000                    add      byte ptr [eax], al             
  0x001F026C  1e                      push     ds                             
  0x001F026D  0000                    add      byte ptr [eax], al             
  0x001F026F  0029                    add      byte ptr [ecx], ch             
  0x001F0271  0000                    add      byte ptr [eax], al             
  0x001F0273  002b                    add      byte ptr [ebx], ch             
  0x001F0275  0000                    add      byte ptr [eax], al             
  0x001F0277  0009                    add      byte ptr [ecx], cl             
  0x001F0279  0000                    add      byte ptr [eax], al             
  0x001F027B  000b                    add      byte ptr [ebx], cl             
  0x001F027D  0000                    add      byte ptr [eax], al             
  0x001F027F  0012                    add      byte ptr [edx], dl             
  0x001F0281  0000                    add      byte ptr [eax], al             
  0x001F0283  0018                    add      byte ptr [eax], bl             
  0x001F0285  0000                    add      byte ptr [eax], al             
  0x001F0287  001f                    add      byte ptr [edi], bl             
  0x001F0289  0000                    add      byte ptr [eax], al             
  0x001F028B  0028                    add      byte ptr [eax], ch             
  0x001F028D  0000                    add      byte ptr [eax], al             
  0x001F028F  002c00                  add      byte ptr [eax + eax], ch       
  0x001F0292  0000                    add      byte ptr [eax], al             
  0x001F0294  350000000a              xor      eax, 0xa000000                 
  0x001F0299  0000                    add      byte ptr [eax], al             
  0x001F029B  0013                    add      byte ptr [ebx], dl             
  0x001F029D  0000                    add      byte ptr [eax], al             
  0x001F029F  0017                    add      byte ptr [edi], dl             
  0x001F02A1  0000                    add      byte ptr [eax], al             
  0x001F02A3  0020                    add      byte ptr [eax], ah             
  0x001F02A5  0000                    add      byte ptr [eax], al             
  0x001F02A7  0027                    add      byte ptr [edi], ah             
  0x001F02A9  0000                    add      byte ptr [eax], al             
  0x001F02AB  002d00000034            add      byte ptr [0x34000000], ch      
  0x001F02B1  0000                    add      byte ptr [eax], al             
  0x001F02B3  0036                    add      byte ptr [esi], dh             
  0x001F02B5  0000                    add      byte ptr [eax], al             
  0x001F02B7  001400                  add      byte ptr [eax + eax], dl       
  0x001F02BA  0000                    add      byte ptr [eax], al             
  0x001F02BC  16                      push     ss                             
  0x001F02BD  0000                    add      byte ptr [eax], al             
  0x001F02BF  0021                    add      byte ptr [ecx], ah             
  0x001F02C1  0000                    add      byte ptr [eax], al             
  0x001F02C3  0026                    add      byte ptr [esi], ah             
  0x001F02C5  0000                    add      byte ptr [eax], al             
  0x001F02C7  002e                    add      byte ptr [esi], ch             
  0x001F02C9  0000                    add      byte ptr [eax], al             
  0x001F02CB  0033                    add      byte ptr [ebx], dh             
  0x001F02CD  0000                    add      byte ptr [eax], al             
  0x001F02CF  0037                    add      byte ptr [edi], dh             
  0x001F02D1  0000                    add      byte ptr [eax], al             
  0x001F02D3  003c00                  add      byte ptr [eax + eax], bh       
  0x001F02D6  0000                    add      byte ptr [eax], al             
  0x001F02D8  1500000022              adc      eax, 0x22000000                
  0x001F02DD  0000                    add      byte ptr [eax], al             
  0x001F02DF  00250000002f            add      byte ptr [0x2f000000], ah      
  0x001F02E5  0000                    add      byte ptr [eax], al             
  0x001F02E7  0032                    add      byte ptr [edx], dh             
  0x001F02E9  0000                    add      byte ptr [eax], al             
  0x001F02EB  0038                    add      byte ptr [eax], bh             
  0x001F02ED  0000                    add      byte ptr [eax], al             
  0x001F02EF  003b                    add      byte ptr [ebx], bh             
  0x001F02F1  0000                    add      byte ptr [eax], al             
  0x001F02F3  003d00000023            add      byte ptr [0x23000000], bh      
  0x001F02F9  0000                    add      byte ptr [eax], al             
  0x001F02FB  002400                  add      byte ptr [eax + eax], ah       
  0x001F02FE  0000                    add      byte ptr [eax], al             
  0x001F0300  3000                    xor      byte ptr [eax], al             
  0x001F0302  0000                    add      byte ptr [eax], al             
  0x001F0304  3100                    xor      dword ptr [eax], eax           
  0x001F0306  0000                    add      byte ptr [eax], al             
  0x001F0308  3900                    cmp      dword ptr [eax], eax           
  0x001F030A  0000                    add      byte ptr [eax], al             
  0x001F030C  3a00                    cmp      al, byte ptr [eax]             
  0x001F030E  0000                    add      byte ptr [eax], al             
  0x001F0310  3e0000                  add      byte ptr ds:[eax], al          
  0x001F0313  003f                    add      byte ptr [edi], bh             
  0x001F0315  0000                    add      byte ptr [eax], al             
  0x001F0317  0000                    add      byte ptr [eax], al             
  0x001F0319  0000                    add      byte ptr [eax], al             
  0x001F031B  0001                    add      byte ptr [ecx], al             
  0x001F031D  0000                    add      byte ptr [eax], al             
  0x001F031F  0008                    add      byte ptr [eax], cl             
  0x001F0321  0000                    add      byte ptr [eax], al             
  0x001F0323  0010                    add      byte ptr [eax], dl             
  0x001F0325  0000                    add      byte ptr [eax], al             
  0x001F0327  0009                    add      byte ptr [ecx], cl             
  0x001F0329  0000                    add      byte ptr [eax], al             
  0x001F032B  0002                    add      byte ptr [edx], al             
  0x001F032D  0000                    add      byte ptr [eax], al             
  0x001F032F  0003                    add      byte ptr [ebx], al             
  0x001F0331  0000                    add      byte ptr [eax], al             
  0x001F0333  000a                    add      byte ptr [edx], cl             
  0x001F0335  0000                    add      byte ptr [eax], al             
  0x001F0337  0011                    add      byte ptr [ecx], dl             
  0x001F0339  0000                    add      byte ptr [eax], al             
  0x001F033B  0018                    add      byte ptr [eax], bl             
  0x001F033D  0000                    add      byte ptr [eax], al             
  0x001F033F  0020                    add      byte ptr [eax], ah             
  0x001F0341  0000                    add      byte ptr [eax], al             
  0x001F0343  0019                    add      byte ptr [ecx], bl             
  0x001F0345  0000                    add      byte ptr [eax], al             
  0x001F0347  0012                    add      byte ptr [edx], dl             
  0x001F0349  0000                    add      byte ptr [eax], al             
  0x001F034B  000b                    add      byte ptr [ebx], cl             
  0x001F034D  0000                    add      byte ptr [eax], al             
  0x001F034F  000400                  add      byte ptr [eax + eax], al       
  0x001F0352  0000                    add      byte ptr [eax], al             
  0x001F0354  050000000c              add      eax, 0xc000000                 
  0x001F0359  0000                    add      byte ptr [eax], al             
  0x001F035B  0013                    add      byte ptr [ebx], dl             
  0x001F035D  0000                    add      byte ptr [eax], al             
  0x001F035F  001a                    add      byte ptr [edx], bl             
  0x001F0361  0000                    add      byte ptr [eax], al             
  0x001F0363  0021                    add      byte ptr [ecx], ah             
  0x001F0365  0000                    add      byte ptr [eax], al             
  0x001F0367  0028                    add      byte ptr [eax], ch             
  0x001F0369  0000                    add      byte ptr [eax], al             
  0x001F036B  0030                    add      byte ptr [eax], dh             
  0x001F036D  0000                    add      byte ptr [eax], al             
  0x001F036F  0029                    add      byte ptr [ecx], ch             
  0x001F0371  0000                    add      byte ptr [eax], al             
  0x001F0373  0022                    add      byte ptr [edx], ah             
  0x001F0375  0000                    add      byte ptr [eax], al             
  0x001F0377  001b                    add      byte ptr [ebx], bl             
  0x001F0379  0000                    add      byte ptr [eax], al             
  0x001F037B  001400                  add      byte ptr [eax + eax], dl       
  0x001F037E  0000                    add      byte ptr [eax], al             
  0x001F0380  0d00000006              or       eax, 0x6000000                 
  0x001F0385  0000                    add      byte ptr [eax], al             
  0x001F0387  0007                    add      byte ptr [edi], al             
  0x001F0389  0000                    add      byte ptr [eax], al             
  0x001F038B  000e                    add      byte ptr [esi], cl             
  0x001F038D  0000                    add      byte ptr [eax], al             
  0x001F038F  00150000001c            add      byte ptr [0x1c000000], dl      
  0x001F0395  0000                    add      byte ptr [eax], al             
  0x001F0397  0023                    add      byte ptr [ebx], ah             
  0x001F0399  0000                    add      byte ptr [eax], al             
  0x001F039B  002a                    add      byte ptr [edx], ch             
  0x001F039D  0000                    add      byte ptr [eax], al             
  0x001F039F  0031                    add      byte ptr [ecx], dh             
  0x001F03A1  0000                    add      byte ptr [eax], al             
  0x001F03A3  0038                    add      byte ptr [eax], bh             
  0x001F03A5  0000                    add      byte ptr [eax], al             
  0x001F03A7  0039                    add      byte ptr [ecx], bh             
  0x001F03A9  0000                    add      byte ptr [eax], al             
  0x001F03AB  0032                    add      byte ptr [edx], dh             
  0x001F03AD  0000                    add      byte ptr [eax], al             
  0x001F03AF  002b                    add      byte ptr [ebx], ch             
  0x001F03B1  0000                    add      byte ptr [eax], al             
  0x001F03B3  002400                  add      byte ptr [eax + eax], ah       
  0x001F03B6  0000                    add      byte ptr [eax], al             
  0x001F03B8  1d00000016              sbb      eax, 0x16000000                
  0x001F03BD  0000                    add      byte ptr [eax], al             
  0x001F03BF  000f                    add      byte ptr [edi], cl             
  0x001F03C1  0000                    add      byte ptr [eax], al             
  0x001F03C3  0017                    add      byte ptr [edi], dl             
  0x001F03C5  0000                    add      byte ptr [eax], al             
  0x001F03C7  001e                    add      byte ptr [esi], bl             
  0x001F03C9  0000                    add      byte ptr [eax], al             
  0x001F03CB  00250000002c            add      byte ptr [0x2c000000], ah      
  0x001F03D1  0000                    add      byte ptr [eax], al             
  0x001F03D3  0033                    add      byte ptr [ebx], dh             
  0x001F03D5  0000                    add      byte ptr [eax], al             
  0x001F03D7  003a                    add      byte ptr [edx], bh             
  0x001F03D9  0000                    add      byte ptr [eax], al             
  0x001F03DB  003b                    add      byte ptr [ebx], bh             
  0x001F03DD  0000                    add      byte ptr [eax], al             
  0x001F03DF  003400                  add      byte ptr [eax + eax], dh       
  0x001F03E2  0000                    add      byte ptr [eax], al             
  0x001F03E4  2d00000026              sub      eax, 0x26000000                
  0x001F03E9  0000                    add      byte ptr [eax], al             
  0x001F03EB  001f                    add      byte ptr [edi], bl             
  0x001F03ED  0000                    add      byte ptr [eax], al             
  0x001F03EF  0027                    add      byte ptr [edi], ah             
  0x001F03F1  0000                    add      byte ptr [eax], al             
  0x001F03F3  002e                    add      byte ptr [esi], ch             
  0x001F03F5  0000                    add      byte ptr [eax], al             
  0x001F03F7  00350000003c            add      byte ptr [0x3c000000], dh      
  0x001F03FD  0000                    add      byte ptr [eax], al             
  0x001F03FF  003d00000036            add      byte ptr [0x36000000], bh      
  0x001F0405  0000                    add      byte ptr [eax], al             
  0x001F0407  002f                    add      byte ptr [edi], ch             
  0x001F0409  0000                    add      byte ptr [eax], al             
  0x001F040B  0037                    add      byte ptr [edi], dh             
  0x001F040D  0000                    add      byte ptr [eax], al             
  0x001F040F  003e                    add      byte ptr [esi], bh             
  0x001F0411  0000                    add      byte ptr [eax], al             
  0x001F0413  003f                    add      byte ptr [edi], bh             
  0x001F0415  0000                    add      byte ptr [eax], al             
  0x001F0417  003f                    add      byte ptr [edi], bh             
  0x001F0419  0000                    add      byte ptr [eax], al             
  0x001F041B  003f                    add      byte ptr [edi], bh             
  0x001F041D  0000                    add      byte ptr [eax], al             
  0x001F041F  003f                    add      byte ptr [edi], bh             
  0x001F0421  0000                    add      byte ptr [eax], al             
  0x001F0423  003f                    add      byte ptr [edi], bh             
  0x001F0425  0000                    add      byte ptr [eax], al             
  0x001F0427  003f                    add      byte ptr [edi], bh             
  0x001F0429  0000                    add      byte ptr [eax], al             
  0x001F042B  003f                    add      byte ptr [edi], bh             
  0x001F042D  0000                    add      byte ptr [eax], al             
  0x001F042F  003f                    add      byte ptr [edi], bh             
  0x001F0431  0000                    add      byte ptr [eax], al             
  0x001F0433  003f                    add      byte ptr [edi], bh             
  0x001F0435  0000                    add      byte ptr [eax], al             
  0x001F0437  003f                    add      byte ptr [edi], bh             
  0x001F0439  0000                    add      byte ptr [eax], al             
  0x001F043B  003f                    add      byte ptr [edi], bh             
  0x001F043D  0000                    add      byte ptr [eax], al             
  0x001F043F  003f                    add      byte ptr [edi], bh             
  0x001F0441  0000                    add      byte ptr [eax], al             
  0x001F0443  003f                    add      byte ptr [edi], bh             
  0x001F0445  0000                    add      byte ptr [eax], al             
  0x001F0447  003f                    add      byte ptr [edi], bh             
  0x001F0449  0000                    add      byte ptr [eax], al             
  0x001F044B  003f                    add      byte ptr [edi], bh             
  0x001F044D  0000                    add      byte ptr [eax], al             
  0x001F044F  003f                    add      byte ptr [edi], bh             
  0x001F0451  0000                    add      byte ptr [eax], al             
  0x001F0453  003f                    add      byte ptr [edi], bh             
  0x001F0455  0000                    add      byte ptr [eax], al             
  0x001F0457  004006                  add      byte ptr [eax + 6], al         
  0x001F045A  0000                    add      byte ptr [eax], al             
  0x001F045C  803e00                  cmp      byte ptr [esi], 0              
  0x001F045F  0000                    add      byte ptr [eax], al             
  0x001F0461  0000                    add      byte ptr [eax], al             
  0x001F0463  00881300006e            add      byte ptr [eax + 0x6e000013], cl 
  0x001F0469  65656420646963          and      byte ptr fs:[ecx + ebp*2 + 0x63], ah 
  0x001F0470  7469                    je       0x1f04db                       
  0x001F0472  6f                      outsd    dx, dword ptr [esi]            
  0x001F0473  6e                      outsb    dx, byte ptr [esi]             
  0x001F0474  61                      popal                                   
  0x001F0475  7279                    jb       0x1f04f0                       
  0x001F0477  0000                    add      byte ptr [eax], al             
  0x001F0479  0000                    add      byte ptr [eax], al             
  0x001F047B  000400                  add      byte ptr [eax + eax], al       
  0x001F047E  0000                    add      byte ptr [eax], al             
  0x001F0480  0000                    add      byte ptr [eax], al             
  0x001F0482  0000                    add      byte ptr [eax], al             
  0x001F0484  0200                    add      al, byte ptr [eax]             
  0x001F0486  0000                    add      byte ptr [eax], al             
  0x001F0488  0000                    add      byte ptr [eax], al             
  0x001F048A  0000                    add      byte ptr [eax], al             
  0x001F048C  0100                    add      dword ptr [eax], eax           
  0x001F048E  0000                    add      byte ptr [eax], al             
  0x001F0490  0000                    add      byte ptr [eax], al             
  0x001F0492  0000                    add      byte ptr [eax], al             
  0x001F0494  0800                    or       byte ptr [eax], al             
  0x001F0496  0000                    add      byte ptr [eax], al             
  0x001F0498  0800                    or       byte ptr [eax], al             
  0x001F049A  0000                    add      byte ptr [eax], al             
  0x001F049C  0400                    add      al, 0                          
  0x001F049E  0000                    add      byte ptr [eax], al             
  0x001F04A0  0400                    add      al, 0                          
  0x001F04A2  0000                    add      byte ptr [eax], al             
  0x001F04A4  0200                    add      al, byte ptr [eax]             
  0x001F04A6  0000                    add      byte ptr [eax], al             
  0x001F04A8  0200                    add      al, byte ptr [eax]             
  0x001F04AA  0000                    add      byte ptr [eax], al             
  0x001F04AC  0100                    add      dword ptr [eax], eax           
  0x001F04AE  0000                    add      byte ptr [eax], al             
  0x001F04B0  0000                    add      byte ptr [eax], al             
  0x001F04B2  0000                    add      byte ptr [eax], al             
  0x001F04B4  0000                    add      byte ptr [eax], al             
  0x001F04B6  0000                    add      byte ptr [eax], al             
  0x001F04B8  0400                    add      al, 0                          
  0x001F04BA  0000                    add      byte ptr [eax], al             
  0x001F04BC  0000                    add      byte ptr [eax], al             
  0x001F04BE  0000                    add      byte ptr [eax], al             
  0x001F04C0  0200                    add      al, byte ptr [eax]             
  0x001F04C2  0000                    add      byte ptr [eax], al             
  0x001F04C4  0000                    add      byte ptr [eax], al             
  0x001F04C6  0000                    add      byte ptr [eax], al             
  0x001F04C8  0100                    add      dword ptr [eax], eax           
  0x001F04CA  0000                    add      byte ptr [eax], al             
  0x001F04CC  0800                    or       byte ptr [eax], al             
  0x001F04CE  0000                    add      byte ptr [eax], al             
  0x001F04D0  0800                    or       byte ptr [eax], al             
  0x001F04D2  0000                    add      byte ptr [eax], al             
  0x001F04D4  0800                    or       byte ptr [eax], al             
  0x001F04D6  0000                    add      byte ptr [eax], al             
  0x001F04D8  0400                    add      al, 0                          
  0x001F04DA  0000                    add      byte ptr [eax], al             
  0x001F04DC  0400                    add      al, 0                          
  0x001F04DE  0000                    add      byte ptr [eax], al             
  0x001F04E0  0200                    add      al, byte ptr [eax]             
  0x001F04E2  0000                    add      byte ptr [eax], al             
  0x001F04E4  0200                    add      al, byte ptr [eax]             
  0x001F04E6  0000                    add      byte ptr [eax], al             
  0x001F04E8  49                      dec      ecx                            
  0x001F04E9  44                      inc      esp                            
  0x001F04EA  41                      inc      ecx                            
  0x001F04EB  54                      push     esp                            
  0x001F04EC  0000                    add      byte ptr [eax], al             
  0x001F04EE  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001F0475 (cond_jump)
  0x001F04F0  0000                    add      byte ptr [eax], al             
  0x001F04F2  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001F0539 (cond_jump)
  0x001F04F4  96                      xchg     esi, eax                       
  0x001F04F5  3007                    xor      byte ptr [edi], al             
  0x001F04F7  772c                    ja       0x1f0525                       
  0x001F04F9  61                      popal                                   
  0x001F04FA  0e                      push     cs                             
  0x001F04FB  ee                      out      dx, al                         
  0x001F04FC  ba51099919              mov      edx, 0x19990951                
  0x001F0501  c46d07                  les      ebp, ptr [ebp + 7]             
  0x001F0505  f4                      hlt                                     
  0x001F0506  6a70                    push     0x70                           
  0x001F0508  35a563e9a3              xor      eax, 0xa3e963a5                
  0x001F050D  95                      xchg     ebp, eax                       
  0x001F050E  649e                    sahf                                    
  0x001F0510  3288db0ea4b8            xor      cl, byte ptr [eax - 0x475bf125] 
  0x001F0516  dc791e                  fdivr    qword ptr [ecx + 0x1e]         
  0x001F0519  e9d5e088d9              jmp      0xd9a7e5f3                     
  0x001F051E  d2972b4cb609            rcl      byte ptr [edi + 0x9b64c2b], cl 
  0x001F0524  bd7cb17e07              mov      ebp, 0x77eb17c                 
  0x001F0529  2db8e7911d              sub      eax, 0x1d91e7b8                
  0x001F052E  bf906410b7              mov      edi, 0xb7106490                
  0x001F0533  1df220b06a              sbb      eax, 0x6ab020f2                
  0x001F0538  48                      dec      eax                            
  0x001F0539  71b9                    jno      0x1f04f4                       
  0x001F053B  f3de41be                fiadd    word ptr [ecx - 0x42]          
  0x001F053F  847dd4                  test     byte ptr [ebp - 0x2c], bh      
  0x001F0542  da1a                    ficomp   dword ptr [edx]                
  0x001F0544  ebe4                    jmp      0x1f052a                       
  0x001F0547  6d                      insd     dword ptr es:[edi], dx         
  0x001F0548  51                      push     ecx                            
  0x001F0549  b5d4                    mov      ch, 0xd4                       
  0x001F054B  f4                      hlt                                     
  0x001F054C  c785d38356986c13c0a8    mov      dword ptr [ebp - 0x67a97c2d], 0xa8c0136c 
  0x001F0556  6b647af962              imul     esp, dword ptr [edx + edi*2 - 7], 0x62 
  0x001F055B  fd                      std                                     
  0x001F055C  ec                      in       al, dx                         
  0x001F055D  c9                      leave                                   
  0x001F055E  658a4f5c                mov      cl, byte ptr gs:[edi + 0x5c]   
  0x001F0562  0114d9                  add      dword ptr [ecx + ebx*8], edx   
  0x001F0565  6c                      insb     byte ptr es:[edi], dx          
  0x001F0566  06                      push     es                             
  0x001F0567  63633d                  arpl     word ptr [ebx + 0x3d], sp      
  0x001F056A  0ffaf5                  psubd    mm6, mm5                       
  0x001F056D  0d088dc820              or       eax, 0x20c88d08                
  0x001F0572  6e                      outsb    dx, byte ptr [esi]             
  0x001F0573  3b5e10                  cmp      ebx, dword ptr [esi + 0x10]    
  0x001F0576  694ce44160d57271        imul     ecx, dword ptr [esp + 0x41], 0x7172d560 
  0x001F057E  67a2d1e4                mov      byte ptr [0xe4d1], al          
  0x001F0582  033c47                  add      edi, dword ptr [edi + eax*2]   
  0x001F0585  d404                    aam      4                              
  0x001F0587  4b                      dec      ebx                            
  0x001F0588  fd                      std                                     
  0x001F0589  850dd26bb50a            test     dword ptr [0xab56bd2], ecx     
  0x001F058F  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001F0590  fa                      cli                                     
  0x001F0591  a8b5                    test     al, 0xb5                       
  0x001F0593  356c98b242              xor      eax, 0x42b2986c                
  0x001F0598  d6                      salc                                    
  0x001F0599  c9                      leave                                   
  0x001F059A  bbdb40f9bc              mov      ebx, 0xbcf940db                
  0x001F059F  ac                      lodsb    al, byte ptr [esi]             
  0x001F05A0  e36c                    jecxz    0x1f060e                       
  0x001F05A2  d832                    fdiv     dword ptr [edx]                
  0x001F05A4  755c                    jne      0x1f0602                       
  0x001F05A6  df45cf                  fild     word ptr [ebp - 0x31]          
  0x001F05A9  0dd6dc593d              or       eax, 0x3d59dcd6                
  0x001F05AE  d1abac30d926            shr      dword ptr [ebx + 0x26d930ac], 1 
  0x001F05B4  3a00                    cmp      al, byte ptr [eax]             
  0x001F05B6  de5180                  ficom    word ptr [ecx - 0x80]          
  0x001F05B9  51                      push     ecx                            
  0x001F05BA  d7                      xlatb                                   
  0x001F05BB  c81661d0                enter    0x6116, -0x30                  
  0x001F05BF  bfb5f4b421              mov      edi, 0x21b4f4b5                
  0x001F05C4  23c4                    and      eax, esp                       
  0x001F05C6  b356                    mov      bl, 0x56                       
  0x001F05C8  99                      cdq                                     
  0x001F05C9  95                      xchg     ebp, eax                       
  0x001F05CA  bacf0fa5bd              mov      edx, 0xbda50fcf                
  0x001F05CF  b89eb80228              mov      eax, 0x2802b89e                
  0x001F05D4  0888055fb2d9            or       byte ptr [eax - 0x264da0fb], cl 
  0x001F05DA  0cc6                    or       al, 0xc6                       
                                        ; XREF: 0x001F061F (cond_jump)
  0x001F05DC  24e9                    and      al, 0xe9                       
  0x001F05DE  0bb1877c6f2f            or       esi, dword ptr [ecx + 0x2f6f7c87] 
  0x001F05E4  114c6858                adc      dword ptr [eax + ebp*2 + 0x58], ecx 
  0x001F05E8  ab                      stosd    dword ptr es:[edi], eax        
  0x001F05E9  1d61c13d2d              sbb      eax, 0x2d3dc161                
  0x001F05EE  66b690                  mov      dh, 0x90                       
  0x001F05F1  41                      inc      ecx                            
  0x001F05F2  dc7606                  fdiv     qword ptr [esi + 6]            
  0x001F05F5  71db                    jno      0x1f05d2                       
  0x001F05F7  01bc20d2982a10          add      dword ptr [eax + 0x102a98d2], edi 
  0x001F05FE  d5ef                    aad      0xef                           
  0x001F0600  8985b1711fb5            mov      dword ptr [ebp - 0x4ae08e4f], eax 
  0x001F0606  b606                    mov      dh, 6                          
  0x001F0608  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001F0609  e4bf                    in       al, 0xbf                       
  0x001F060B  9f                      lahf                                    
  0x001F060C  33d4                    xor      edx, esp                       
                                        ; XREF: 0x001F05A0 (cond_jump)
  0x001F060E  b8e8a2c907              mov      eax, 0x7c9a2e8                 
  0x001F0613  7834                    js       0x1f0649                       
  0x001F0615  f9                      stc                                     
  0x001F0616  000f                    add      byte ptr [edi], cl             
  0x001F0618  8ea809961898            mov      gs, word ptr [eax - 0x67e769f7] 
  0x001F061E  0e                      push     cs                             
  0x001F061F  e1bb                    loope    0x1f05dc                       
  0x001F0621  0d6a7f2d3d              or       eax, 0x3d2d7f6a                
  0x001F0626  6d                      insd     dword ptr es:[edi], dx         
  0x001F0627  08976c649101            or       byte ptr [edi + 0x191646c], dl 
  0x001F062D  5c                      pop      esp                            
  0x001F062E  63e6                    arpl     si, sp                         
  0x001F0630  f4                      hlt                                     
  0x001F0631  51                      push     ecx                            
  0x001F0632  6b6b6261                imul     ebp, dword ptr [ebx + 0x62], 0x61 
  0x001F0636  6c                      insb     byte ptr es:[edi], dx          
  0x001F0637  1cd8                    sbb      al, 0xd8                       
  0x001F0639  306585                  xor      byte ptr [ebp - 0x7b], ah      
  0x001F063C  4e                      dec      esi                            
  0x001F063D  0062f2                  add      byte ptr [edx - 0xe], ah       
  0x001F0640  ed                      in       eax, dx                        
  0x001F0641  95                      xchg     ebp, eax                       
  0x001F0642  06                      push     es                             
  0x001F0643  6c                      insb     byte ptr es:[edi], dx          
  0x001F0644  7ba5                    jnp      0x1f05eb                       
  0x001F0646  011b                    add      dword ptr [ebx], ebx           
  0x001F0648  c1f408                  sal      esp, 8                         
  0x001F064B  8257c40f                adc      byte ptr [edi - 0x3c], 0xf     
  0x001F064F  f5                      cmc                                     
  0x001F0651  d9b06550e9b7            fnstenv  [eax - 0x4816af9b]             
  0x001F0657  12ea                    adc      ch, dl                         
  0x001F0659  b8be8b7c88              mov      eax, 0x887c8bbe                
  0x001F065E  b9fcdf1ddd              mov      ecx, 0xdd1ddffc                
  0x001F0663  62492d                  bound    ecx, qword ptr [ecx + 0x2d]    
  0x001F0666  da15f37cd38c            ficom    dword ptr [0x8cd37cf3]         
  0x001F066C  654c                    dec      esp                            
  0x001F066E  d4fb                    aam      0xfb                           
  0x001F0670  58                      pop      eax                            
  0x001F0671  61                      popal                                   
  0x001F0672  b24d                    mov      dl, 0x4d                       
  0x001F0674  ce                      into                                    
  0x001F0675  51                      push     ecx                            
  0x001F0676  b53a                    mov      ch, 0x3a                       
  0x001F0678  7400                    je       0x1f067a                       
                                        ; XREF: 0x001F0678 (cond_jump)
  0x001F067A  bca3e230bb              mov      esp, 0xbb30e2a3                
  0x001F067F  d441                    aam      0x41                           
  0x001F0681  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x001F0682  df4ad7                  fisttp   word ptr [edx - 0x29]          
  0x001F0685  95                      xchg     ebp, eax                       
  0x001F0686  d83d6dc4d1a4            fdivr    dword ptr [0xa4d1c46d]         
  0x001F068C  fb                      sti                                     
  0x001F068D  f4                      hlt                                     
  0x001F068E  d6                      salc                                    
  0x001F068F  d36ae9                  shr      dword ptr [edx - 0x17], cl     
  0x001F0692  6943fcd96e3446          imul     eax, dword ptr [ebx - 4], 0x46346ed9 
  0x001F0699  8867ad                  mov      byte ptr [edi - 0x53], ah      
  0x001F069C  d0b860da732d            sar      byte ptr [eax + 0x2d73da60], 1 
  0x001F06A2  0444                    add      al, 0x44                       
  0x001F06A4  e51d                    in       eax, 0x1d                      
  0x001F06A6  0333                    add      esi, dword ptr [ebx]           
  0x001F06A8  5f                      pop      edi                            
  0x001F06A9  4c                      dec      esp                            
  0x001F06AA  0aaac97c0ddd            or       ch, byte ptr [edx - 0x22f28337] 
  0x001F06B0  3c71                    cmp      al, 0x71                       
  0x001F06B2  0550aa4102              add      eax, 0x241aa50                 
  0x001F06B7  27                      daa                                     
  0x001F06B8  1010                    adc      byte ptr [eax], dl             
  0x001F06BA  0bbe86200cc9            or       edi, dword ptr [esi - 0x36f3df7a] 
  0x001F06C0  25b56857b3              and      eax, 0xb35768b5                
  0x001F06C5  856f20                  test     dword ptr [edi + 0x20], ebp    
  0x001F06C8  09d4                    or       esp, edx                       
  0x001F06CA  66b99fe4                mov      cx, 0xe49f                     
  0x001F06CE  61                      popal                                   
  0x001F06CF  ce                      into                                    
  0x001F06D0  0e                      push     cs                             
  0x001F06D1  f9                      stc                                     
  0x001F06D2  de5e98                  ficomp   word ptr [esi - 0x68]          
  0x001F06D5  c9                      leave                                   
  0x001F06D6  d929                    fldcw    word ptr [ecx]                 
  0x001F06D8  2298d0b0b4a8            and      bl, byte ptr [eax - 0x574b4f30] 
  0x001F06DE  d7                      xlatb                                   
  0x001F06E0  17                      pop      ss                             
  0x001F06E1  3db359810d              cmp      eax, 0xd8159b3                 
  0x001F06E6  b42e                    mov      ah, 0x2e                       
  0x001F06E8  3b5cbdb7                cmp      ebx, dword ptr [ebp + edi*4 - 0x49] 
  0x001F06EC  ad                      lodsd    eax, dword ptr [esi]           
  0x001F06ED  6c                      insb     byte ptr es:[edi], dx          
  0x001F06EE  bac02083b8              mov      edx, 0xb88320c0                
  0x001F06F3  ed                      in       eax, dx                        
  0x001F06F4  b6b3                    mov      dh, 0xb3                       
  0x001F06F6  bf9a0ce2b6              mov      edi, 0xb6e20c9a                
  0x001F06FB  039ad2b17439            add      ebx, dword ptr [edx + 0x3974b1d2] 
  0x001F0701  47                      inc      edi                            
  0x001F0702  d5ea                    aad      0xea                           
  0x001F0704  af                      scasd    eax, dword ptr es:[edi]        
  0x001F0705  77d2                    ja       0x1f06d9                       
  0x001F0707  9d                      popfd                                   
  0x001F0708  1526db0483              adc      eax, 0x8304db26                
  0x001F070D  16                      push     ss                             
  0x001F070E  dc7312                  fdiv     qword ptr [ebx + 0x12]         
  0x001F0711  0b63e3                  or       esp, dword ptr [ebx - 0x1d]    
  0x001F0714  843b                    test     byte ptr [ebx], bh             
  0x001F0716  6494                    xchg     esp, eax                       
  0x001F0718  3e6a6d                  push     0x6d                           
  0x001F071B  0da85a6a7a              or       eax, 0x7a6a5aa8                
  0x001F0720  0bcf                    or       ecx, edi                       
  0x001F0722  0e                      push     cs                             
  0x001F0723  e49d                    in       al, 0x9d                       
  0x001F0725  ff09                    dec      dword ptr [ecx]                
  0x001F0727  93                      xchg     ebx, eax                       
  0x001F0728  27                      daa                                     
  0x001F0729  ae                      scasb    al, byte ptr es:[edi]          
  0x001F072A  000a                    add      byte ptr [edx], cl             
  0x001F072C  b19e                    mov      cl, 0x9e                       
  0x001F072E  07                      pop      es                             
  0x001F072F  7d44                    jge      0x1f0775                       
  0x001F0731  93                      xchg     ebx, eax                       
  0x001F0734  d2a3088768f2            shl      byte ptr [ebx - 0xd9778f8], cl 
  0x001F073A  011e                    add      dword ptr [esi], ebx           
  0x001F073C  fec2                    inc      dl                             
  0x001F073E  06                      push     es                             
  0x001F073F  695d5762f7cb67          imul     ebx, dword ptr [ebp + 0x57], 0x67cbf762 
  0x001F0746  658071366c              xor      byte ptr gs:[ecx + 0x36], 0x6c 
  0x001F074B  19e7                    sbb      edi, esp                       
  0x001F074D  06                      push     es                             
  0x001F074E  6b6e761b                imul     ebp, dword ptr [esi + 0x76], 0x1b 
  0x001F0752  d4fe                    aam      0xfe                           
  0x001F0754  e02b                    loopne   0x1f0781                       
  0x001F0756  d3895a7ada10            ror      dword ptr [ecx + 0x10da7a5a], cl 
  0x001F075C  cc                      int3                                    
  0x001F075D  4a                      dec      edx                            
  0x001F075E  dd676f                  frstor   dword ptr [edi + 0x6f]         
  0x001F0761  dfb9f9f9efbe            fistp    qword ptr [ecx - 0x41100607]   
  0x001F0767  8e43be                  mov      es, word ptr [ebx - 0x42]      
  0x001F076A  b717                    mov      bh, 0x17                       
                                        ; XREF: 0x001F07AC (cond_jump)
  0x001F076C  d58e                    aad      0x8e                           
  0x001F076E  b060                    mov      al, 0x60                       
  0x001F0770  e8a3d6d67e              call     0x7ef5de18                     
                                        ; XREF: 0x001F072F (cond_jump)
  0x001F0775  93                      xchg     ebx, eax                       
  0x001F0776  d1a1c4c2d838            shl      dword ptr [ecx + 0x38d8c2c4], 1 
  0x001F077C  52                      push     edx                            
  0x001F077D  f2df4ff1                fisttp   word ptr [edi - 0xf]           
                                        ; XREF: 0x001F0754 (cond_jump)
  0x001F0781  67bbd16757bc            mov      ebx, 0xbc5767d1                
  0x001F0787  a6                      cmpsb    byte ptr [esi], byte ptr es:[edi] 
  0x001F0788  dd06                    fld      qword ptr [esi]                
  0x001F078A  b53f                    mov      ch, 0x3f                       
  0x001F078C  4b                      dec      ebx                            
  0x001F078D  36b248                  mov      dl, 0x48                       
  0x001F0790  da2b                    fisubr   dword ptr [ebx]                
  0x001F0792  0dd84c1b0a              or       eax, 0xa1b4cd8                 
  0x001F0797  af                      scasd    eax, dword ptr es:[edi]        
  0x001F0798  f64a0336                test     byte ptr [edx + 3], 0x36       
  0x001F079C  60                      pushal                                  
  0x001F079D  7a04                    jp       0x1f07a3                       
  0x001F079F  41                      inc      ecx                            
  0x001F07A0  c3                      ret                                     
  0x001F07A1  ef                      out      dx, eax                        
  0x001F07A2  60                      pushal                                  
                                        ; XREF: 0x001F079D (cond_jump)
  0x001F07A3  df55df                  fist     word ptr [ebp - 0x21]          
  0x001F07A6  67a8ef                  test     al, 0xef                       
  0x001F07A9  8e6e31                  mov      gs, word ptr [esi + 0x31]      
  0x001F07AC  79be                    jns      0x1f076c                       
  0x001F07AE  69468cb361cb1a          imul     eax, dword ptr [esi - 0x74], 0x1acb61b3 
  0x001F07B5  8366bca0                and      dword ptr [esi - 0x44], 0xffffffa0 
  0x001F07B9  d26f25                  shr      byte ptr [edi + 0x25], cl      
  0x001F07BC  36e268                  loop     0x1f0827                       
  0x001F07BF  52                      push     edx                            
  0x001F07C0  95                      xchg     ebp, eax                       
  0x001F07C1  770c                    ja       0x1f07cf                       
  0x001F07C3  cc                      int3                                    
  0x001F07C4  03470b                  add      eax, dword ptr [edi + 0xb]     
  0x001F07C7  bbb9160222              mov      ebx, 0x220216b9                
  0x001F07CC  2f                      das                                     
  0x001F07CD  260555be3bba            add      eax, 0xba3bbe55                
  0x001F07D3  c528                    lds      ebp, ptr [eax]                 
  0x001F07D5  0bbdb2925ab4            or       edi, dword ptr [ebp - 0x4ba56d4e] 
  0x001F07DB  2b046a                  sub      eax, dword ptr [edx + ebp*2]   
  0x001F07DE  b35c                    mov      bl, 0x5c                       
  0x001F07E0  a7                      cmpsd    dword ptr [esi], dword ptr es:[edi] 
  0x001F07E1  ffd7                    call     edi                            
  0x001F07E3  c231cf                  ret      0xcf31                         
  0x001F07E6  d0b58b9ed92c            sal      byte ptr [ebp + 0x2cd99e8b], 1 
  0x001F07EC  1daede5bb0              sbb      eax, 0xb05bdeae                
  0x001F07F1  c2649b                  ret      0x9b64                         
  0x001F07F4  26f263ec                arpl     sp, bp                         
  0x001F07F8  9c                      pushfd                                  
  0x001F07F9  a36a750a93              mov      dword ptr [0x930a756a], eax    
  0x001F07FE  6d                      insd     dword ptr es:[edi], dx         
  0x001F07FF  02a906099c3f            add      ch, byte ptr [ecx + 0x3f9c0906] 
  0x001F0805  360e                    push     cs                             
  0x001F0807  eb85                    jmp      0x1f078e                       
  0x001F0809  6707                    pop      es                             
  0x001F080B  7213                    jb       0x1f0820                       
  0x001F080D  57                      push     edi                            
  0x001F080E  0005824abf95            add      byte ptr [0x95bf4a82], al      
  0x001F0814  147a                    adc      al, 0x7a                       
  0x001F0816  b8e2ae2bb1              mov      eax, 0xb12baee2                
  0x001F081B  7b38                    jnp      0x1f0855                       
  0x001F081D  1bb60c9b8ed2            sbb      esi, dword ptr [esi - 0x2d7164f4] 
  0x001F0823  92                      xchg     edx, eax                       
  0x001F0824  0dbed5e5b7              or       eax, 0xb7e5d5be                
  0x001F0829  ef                      out      dx, eax                        
  0x001F082A  dc7c21df                fdivr    qword ptr [ecx - 0x21]         
  0x001F082E  db0b                    fisttp   dword ptr [ebx]                
  0x001F0830  d4d2                    aam      0xd2                           
  0x001F0832  d38642e2d4f1            rol      dword ptr [esi - 0xe2b1dbe], cl 
  0x001F0838  f8                      clc                                     
  0x001F0839  b3dd                    mov      bl, 0xdd                       
  0x001F083B  686e83da1f              push     0x1fda836e                     
  0x001F0840  cd16                    int      0x16                           
  0x001F0842  be815b26b9              mov      esi, 0xb9265b81                
  0x001F0847  f6e1                    mul      cl                             
  0x001F0849  77b0                    ja       0x1f07fb                       
  0x001F084B  6f                      outsd    dx, dword ptr [esi]            
  0x001F084C  7747                    ja       0x1f0895                       
  0x001F084E  b718                    mov      bh, 0x18                       
  0x001F0850  e65a                    out      0x5a, al                       
  0x001F0852  0888706a0fff            or       byte ptr [eax - 0xf09590], cl  
  0x001F0858  ca3b06                  retf     0x63b                          
  0x001F085B  665c                    pop      sp                             
  0x001F085D  0b01                    or       eax, dword ptr [ecx]           
  0x001F085F  11ff                    adc      edi, edi                       
  0x001F0861  9e                      sahf                                    
  0x001F0864  69ae62f8d3ff6b6145cf    imul     ebp, dword ptr [esi - 0x2c079e], 0xcf45616b 
  0x001F086E  6c                      insb     byte ptr es:[edi], dx          
  0x001F086F  16                      push     ss                             
  0x001F0870  78e2                    js       0x1f0854                       
  0x001F0872  0aa0eed20dd7            or       ah, byte ptr [eax - 0x28f22d12] 
  0x001F0878  54                      push     esp                            
  0x001F0879  83044ec2                add      dword ptr [esi + ecx*2], -0x3e 
  0x001F087D  b303                    mov      bl, 3                          
  0x001F087F  396126                  cmp      dword ptr [ecx + 0x26], esp    
  0x001F0882  67a7                    cmpsd    dword ptr [si], dword ptr es:[di] 
  0x001F0884  f716                    not      dword ptr [esi]                
  0x001F0886  60                      pushal                                  
  0x001F0887  d04d47                  ror      byte ptr [ebp + 0x47], 1       
  0x001F088A  6949db776e3e4a          imul     ecx, dword ptr [ecx - 0x25], 0x4a3e6e77 
  0x001F0891  6ad1                    push     -0x2f                          
  0x001F0893  ae                      scasb    al, byte ptr es:[edi]          
  0x001F0894  dc5ad6                  fcomp    qword ptr [edx - 0x2a]         
  0x001F0897  d9660b                  fldenv   [esi + 0xb]                    
  0x001F089A  df40f0                  fild     word ptr [eax - 0x10]          
  0x001F089D  3bd8                    cmp      ebx, eax                       
  0x001F089F  37                      aaa                                     
  0x001F08A0  53                      push     ebx                            
  0x001F08A1  ae                      scasb    al, byte ptr es:[edi]          
  0x001F08A2  bca9c59ebb              mov      esp, 0xbb9ec5a9                
  0x001F08A7  de7fcf                  fidivr   word ptr [edi - 0x31]          
  0x001F08AA  b247                    mov      dl, 0x47                       
  0x001F08AC  e9ffb5301c              jmp      0x1c4fbeb0                     
  0x001F08B1  f2bdbd8ac2ba            mov      ebp, 0xbac28abd                
  0x001F08B7  ca3093                  retf     0x9330                         
  0x001F08BA  b353                    mov      bl, 0x53                       
  0x001F08BC  a6                      cmpsb    byte ptr [esi], byte ptr es:[edi] 
  0x001F08BD  a3b4240536              mov      dword ptr [0x360524b4], eax    
  0x001F08C2  d0ba9306d7cd            sar      byte ptr [edx - 0x3228f96d], 1 
  0x001F08C8  2957de                  sub      dword ptr [edi - 0x22], edx    
  0x001F08CB  54                      push     esp                            
  0x001F08CC  bf67d9232e              mov      edi, 0x2e23d967                
  0x001F08D1  7a66                    jp       0x1f0939                       
  0x001F08D3  b3b8                    mov      bl, 0xb8                       
  0x001F08D5  4a                      dec      edx                            
  0x001F08D6  61                      popal                                   
  0x001F08D7  c402                    les      eax, ptr [edx]                 
  0x001F08D9  1b685d                  sbb      ebp, dword ptr [eax + 0x5d]    
  0x001F08DC  94                      xchg     esp, eax                       
  0x001F08DD  2b6f2a                  sub      ebp, dword ptr [edi + 0x2a]    
  0x001F08E0  37                      aaa                                     
  0x001F08E1  be0bb4a18e              mov      esi, 0x8ea1b40b                
  0x001F08E6  0cc3                    or       al, 0xc3                       
  0x001F08E8  1bdf                    sbb      ebx, edi                       
  0x001F08EA  055a8def02              add      eax, 0x2ef8d5a                 
  0x001F08EF  2d00000000              sub      eax, 0                         
  0x001F08F4  0100                    add      dword ptr [eax], eax           
  0x001F08F6  0000                    add      byte ptr [eax], al             
  0x001F08F8  0200                    add      al, byte ptr [eax]             
  0x001F08FA  0000                    add      byte ptr [eax], al             
  0x001F08FC  0400                    add      al, 0                          
  0x001F08FE  0000                    add      byte ptr [eax], al             
  0x001F0900  0800                    or       byte ptr [eax], al             
  0x001F0902  0000                    add      byte ptr [eax], al             
  0x001F0904  1000                    adc      byte ptr [eax], al             
  0x001F0906  0000                    add      byte ptr [eax], al             
  0x001F0908  2000                    and      byte ptr [eax], al             
  0x001F090A  0000                    add      byte ptr [eax], al             
  0x001F090C  40                      inc      eax                            
  0x001F090D  0000                    add      byte ptr [eax], al             
  0x001F090F  008000000000            add      byte ptr [eax], al             
  0x001F0915  0100                    add      dword ptr [eax], eax           
  0x001F0917  0000                    add      byte ptr [eax], al             
  0x001F0919  0200                    add      al, byte ptr [eax]             
  0x001F091B  0000                    add      byte ptr [eax], al             
  0x001F091D  0400                    add      al, 0                          
  0x001F091F  0000                    add      byte ptr [eax], al             
  0x001F0921  0800                    or       byte ptr [eax], al             
  0x001F0923  0000                    add      byte ptr [eax], al             
  0x001F0925  1000                    adc      byte ptr [eax], al             
  0x001F0927  0000                    add      byte ptr [eax], al             
  0x001F0929  2000                    and      byte ptr [eax], al             
  0x001F092B  0000                    add      byte ptr [eax], al             
  0x001F092D  40                      inc      eax                            
  0x001F092E  0000                    add      byte ptr [eax], al             
  0x001F0930  0000                    add      byte ptr [eax], al             
  0x001F0932  0000                    add      byte ptr [eax], al             
  0x001F0938  fd                      std                                     
  0x001F093C  f9                      stc                                     
  0x001F093F  fff1                    push     ecx                            
  0x001F0943  ffe1                    jmp      ecx                            
  0x001F0947  ffc1                    inc      ecx                            
  0x001F094B  ff81ffffff01            inc      dword ptr [ecx + 0x1ffffff]    
  0x001F0953  ff01                    inc      dword ptr [ecx]                
  0x001F0957  ff01                    inc      dword ptr [ecx]                
  0x001F0959  fc                      cld                                     
  0x001F095B  ff01                    inc      dword ptr [ecx]                
  0x001F095D  f8                      clc                                     
  0x001F095F  ff01                    inc      dword ptr [ecx]                
  0x001F0963  ff01                    inc      dword ptr [ecx]                
  0x001F0965  e0ff                    loopne   0x1f0966                       
  0x001F0967  ff01                    inc      dword ptr [ecx]                
  0x001F0969  c0ffff                  sar      bh, 0xff                       
  0x001F096C  0180ffff0000            add      dword ptr [eax + 0xffff], eax  
  0x001F0972  0000                    add      byte ptr [eax], al             
  0x001F0974  0100                    add      dword ptr [eax], eax           
  0x001F0976  0000                    add      byte ptr [eax], al             
  0x001F0978  0200                    add      al, byte ptr [eax]             
  0x001F097A  0000                    add      byte ptr [eax], al             
  0x001F097C  0400                    add      al, 0                          
  0x001F097E  0000                    add      byte ptr [eax], al             
  0x001F0980  0800                    or       byte ptr [eax], al             
  0x001F0982  0000                    add      byte ptr [eax], al             
  0x001F0984  1000                    adc      byte ptr [eax], al             
  0x001F0986  0000                    add      byte ptr [eax], al             
  0x001F0988  2000                    and      byte ptr [eax], al             
  0x001F098A  0000                    add      byte ptr [eax], al             
  0x001F098C  40                      inc      eax                            
  0x001F098D  0000                    add      byte ptr [eax], al             
  0x001F098F  008000000000            add      byte ptr [eax], al             
  0x001F0995  0100                    add      dword ptr [eax], eax           
  0x001F0997  0000                    add      byte ptr [eax], al             
  0x001F0999  0200                    add      al, byte ptr [eax]             
  0x001F099B  0000                    add      byte ptr [eax], al             
  0x001F099D  0400                    add      al, 0                          
  0x001F099F  0000                    add      byte ptr [eax], al             
  0x001F09A1  0800                    or       byte ptr [eax], al             
  0x001F09A3  0000                    add      byte ptr [eax], al             
  0x001F09A5  0000                    add      byte ptr [eax], al             
  0x001F09A7  00ff                    add      bh, bh                         
  0x001F09AC  fd                      std                                     
  0x001F09B0  f9                      stc                                     
  0x001F09B3  fff1                    push     ecx                            
  0x001F09B7  ffe1                    jmp      ecx                            
  0x001F09BB  ffc1                    inc      ecx                            
  0x001F09BF  ff81ffffff01            inc      dword ptr [ecx + 0x1ffffff]    
  0x001F09C7  ff01                    inc      dword ptr [ecx]                
  0x001F09CB  ff01                    inc      dword ptr [ecx]                
  0x001F09CD  fc                      cld                                     
  0x001F09CF  ff01                    inc      dword ptr [ecx]                
  0x001F09D1  f8                      clc                                     
  0x001F09D3  ff00                    inc      dword ptr [eax]                
  0x001F09D5  0000                    add      byte ptr [eax], al             
  0x001F09D7  0000                    add      byte ptr [eax], al             
  0x001F09D9  0000                    add      byte ptr [eax], al             
  0x001F09DB  0001                    add      byte ptr [ecx], al             
  0x001F09DD  0000                    add      byte ptr [eax], al             
  0x001F09DF  0002                    add      byte ptr [edx], al             
  0x001F09E1  0000                    add      byte ptr [eax], al             
  0x001F09E3  000400                  add      byte ptr [eax + eax], al       
  0x001F09E6  0000                    add      byte ptr [eax], al             
  0x001F09E8  0800                    or       byte ptr [eax], al             
  0x001F09EA  0000                    add      byte ptr [eax], al             
  0x001F09EC  1000                    adc      byte ptr [eax], al             
  0x001F09EE  0000                    add      byte ptr [eax], al             
  0x001F09F0  2000                    and      byte ptr [eax], al             
  0x001F09F2  0000                    add      byte ptr [eax], al             
  0x001F09F4  40                      inc      eax                            
  0x001F09F5  0000                    add      byte ptr [eax], al             
  0x001F09F7  008000000000            add      byte ptr [eax], al             
  0x001F09FD  0100                    add      dword ptr [eax], eax           
  0x001F09FF  0000                    add      byte ptr [eax], al             
  0x001F0A01  0200                    add      al, byte ptr [eax]             
  0x001F0A03  0000                    add      byte ptr [eax], al             
  0x001F0A05  0400                    add      al, 0                          
  0x001F0A07  0000                    add      byte ptr [eax], al             
  0x001F0A09  0800                    or       byte ptr [eax], al             
  0x001F0A0B  0000                    add      byte ptr [eax], al             
  0x001F0A0D  1000                    adc      byte ptr [eax], al             
  0x001F0A0F  0000                    add      byte ptr [eax], al             
  0x001F0A11  2000                    and      byte ptr [eax], al             
  0x001F0A13  0000                    add      byte ptr [eax], al             
  0x001F0A15  40                      inc      eax                            
  0x001F0A16  0000                    add      byte ptr [eax], al             
  0x001F0A18  0000                    add      byte ptr [eax], al             
  0x001F0A1A  0000                    add      byte ptr [eax], al             
  0x001F0A20  fd                      std                                     
  0x001F0A24  f9                      stc                                     
  0x001F0A27  fff1                    push     ecx                            
  0x001F0A2B  ffe1                    jmp      ecx                            
  0x001F0A2F  ffc1                    inc      ecx                            
  0x001F0A33  ff81ffffff01            inc      dword ptr [ecx + 0x1ffffff]    
  0x001F0A3B  ff01                    inc      dword ptr [ecx]                
  0x001F0A3F  ff01                    inc      dword ptr [ecx]                
  0x001F0A41  fc                      cld                                     
  0x001F0A43  ff01                    inc      dword ptr [ecx]                
  0x001F0A45  f8                      clc                                     
  0x001F0A47  ff01                    inc      dword ptr [ecx]                
  0x001F0A4B  ff01                    inc      dword ptr [ecx]                
  0x001F0A4D  e0ff                    loopne   0x1f0a4e                       
  0x001F0A4F  ff01                    inc      dword ptr [ecx]                
  0x001F0A51  c0ffff                  sar      bh, 0xff                       
  0x001F0A54  0180ffff00c0            add      dword ptr [eax - 0x3fff0001], eax 
  0x001F0A5A  30f0                    xor      al, dh                         
  0x001F0A5C  0ccc                    or       al, 0xcc                       
  0x001F0A5E  3cfc                    cmp      al, 0xfc                       
  0x001F0A60  03c3                    add      eax, ebx                       
  0x001F0A62  33f3                    xor      esi, ebx                       
  0x001F0A64  0fcf                    bswap    edi                            
  0x001F0A66  3f                      aas                                     
  0x001F0A67  ff8040b0708c            inc      dword ptr [eax - 0x738f4fc0]   
  0x001F0A6D  4c                      dec      esp                            
  0x001F0A6E  bc7c8343b3              mov      esp, 0xb343837c                
  0x001F0A73  738f                    jae      0x1f0a04                       
  0x001F0A75  4f                      dec      edi                            
  0x001F0A76  bf7f20e010              mov      edi, 0x10e0207f                
  0x001F0A7B  d02cec                  shr      byte ptr [esp + ebp*8], 1      
  0x001F0A7E  1cdc                    sbb      al, 0xdc                       
  0x001F0A80  23e3                    and      esp, ebx                       
  0x001F0A82  13d3                    adc      edx, ebx                       
  0x001F0A84  2f                      das                                     
  0x001F0A85  ef                      out      dx, eax                        
  0x001F0A86  1f                      pop      ds                             
  0x001F0A87  dfa0609050ac            fbld     tbyte ptr [eax - 0x53af6fa0]   
  0x001F0A8D  6c                      insb     byte ptr es:[edi], dx          
  0x001F0A8E  9c                      pushfd                                  
  0x001F0A8F  5c                      pop      esp                            
  0x001F0A90  a3639353af              mov      dword ptr [0xaf539363], eax    
  0x001F0A95  6f                      outsd    dx, dword ptr [esi]            
  0x001F0A96  9f                      lahf                                    
  0x001F0A97  5f                      pop      edi                            
  0x001F0A98  08c8                    or       al, cl                         
  0x001F0A9A  38f8                    cmp      al, bh                         
  0x001F0A9C  04c4                    add      al, 0xc4                       
  0x001F0A9E  34f4                    xor      al, 0xf4                       
  0x001F0AA0  0bcb                    or       ecx, ebx                       
  0x001F0AA2  3bfb                    cmp      edi, ebx                       
  0x001F0AA4  07                      pop      es                             
  0x001F0AA6  37                      aaa                                     
  0x001F0AA7  f78848b8788444b4748b    test     dword ptr [eax - 0x7b8747b8], 0x8b74b444 
  0x001F0AB1  4b                      dec      ebx                            
  0x001F0AB2  bb7b8747b7              mov      ebx, 0xb747877b                
  0x001F0AB7  7728                    ja       0x1f0ae1                       
  0x001F0AB9  e818d824e4              call     0xe443e2d6                     
  0x001F0ABE  14d4                    adc      al, 0xd4                       
  0x001F0AC0  2beb                    sub      ebp, ebx                       
  0x001F0AC2  1bdb                    sbb      ebx, ebx                       
  0x001F0AC4  27                      daa                                     
  0x001F0AC5  e717                    out      0x17, eax                      
  0x001F0AC7  d7                      xlatb                                   
  0x001F0AC8  a868                    test     al, 0x68                       
  0x001F0ACA  98                      cwde                                    
  0x001F0ACB  58                      pop      eax                            
  0x001F0ACC  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x001F0ACD  6494                    xchg     esp, eax                       
  0x001F0ACF  54                      push     esp                            
  0x001F0AD0  ab                      stosd    dword ptr es:[edi], eax        
  0x001F0AD1  6b9b5ba7679757          imul     ebx, dword ptr [ebx - 0x689858a5], 0x57 
  0x001F0AD8  02c2                    add      al, dl                         
  0x001F0ADA  32f2                    xor      dh, dl                         
  0x001F0ADC  0e                      push     cs                             
  0x001F0ADD  ce                      into                                    
  0x001F0ADE  3efe01                  inc      byte ptr ds:[ecx]              
                                        ; XREF: 0x001F0AB7 (cond_jump)
  0x001F0AE1  c131f1                  sal      dword ptr [ecx], 0xf1          
  0x001F0AE4  0dcd3dfd82              or       eax, 0x82fd3dcd                
  0x001F0AE9  42                      inc      edx                            
  0x001F0AEA  b272                    mov      dl, 0x72                       
  0x001F0AEC  8e4ebe                  mov      cs, word ptr [esi - 0x42]      
  0x001F0AEF  7e81                    jle      0x1f0a72                       
  0x001F0AF1  41                      inc      ecx                            
  0x001F0AF2  b171                    mov      cl, 0x71                       
  0x001F0AF4  8d4dbd                  lea      ecx, [ebp - 0x43]              
  0x001F0AF7  7d22                    jge      0x1f0b1b                       
  0x001F0AF9  e212                    loop     0x1f0b0d                       
  0x001F0AFB  d22e                    shr      byte ptr [esi], cl             
  0x001F0AFD  ee                      out      dx, al                         
  0x001F0AFE  1e                      push     ds                             
  0x001F0AFF  de21                    fisub    word ptr [ecx]                 
  0x001F0B01  e111                    loope    0x1f0b14                       
  0x001F0B03  d12ded1ddda2            shr      dword ptr [0xa2dd1ded], 1      
  0x001F0B09  629252ae6e9e            bound    edx, qword ptr [edx - 0x619151ae] 
  0x001F0B0F  5e                      pop      esi                            
  0x001F0B10  a1619151ad              mov      eax, dword ptr [0xad519161]    
  0x001F0B15  6d                      insd     dword ptr es:[edi], dx         
  0x001F0B16  9d                      popfd                                   
  0x001F0B17  5d                      pop      ebp                            
  0x001F0B18  0aca                    or       cl, dl                         
  0x001F0B1A  3afa                    cmp      bh, dl                         
  0x001F0B1C  06                      push     es                             
  0x001F0B1E  36f609c9                test     byte ptr ss:[ecx], -0x37       
  0x001F0B22  39f9                    cmp      ecx, edi                       
  0x001F0B24  05c535f58a              add      eax, 0x8af535c5                
  0x001F0B29  4a                      dec      edx                            
  0x001F0B2A  ba7a8646b6              mov      edx, 0xb646867a                
  0x001F0B2F  7689                    jbe      0x1f0aba                       
  0x001F0B31  49                      dec      ecx                            
  0x001F0B32  b9798545b5              mov      ecx, 0xb5458579                
  0x001F0B37  752a                    jne      0x1f0b63                       
  0x001F0B39  ea1ada26e616d6          ljmp     0xd616:0xe626da1a              
  0x001F0B40  29e9                    sub      ecx, ebp                       
  0x001F0B42  19d9                    sbb      ecx, ebx                       
  0x001F0B44  25e515d5aa              and      eax, 0xaad515e5                
  0x001F0B49  6a9a                    push     -0x66                          
  0x001F0B4B  5a                      pop      edx                            
  0x001F0B4C  a6                      cmpsb    byte ptr [esi], byte ptr es:[edi] 
  0x001F0B4D  6696                    xchg     si, ax                         
  0x001F0B4F  56                      push     esi                            
  0x001F0B50  a9699959a5              test     eax, 0xa5599969                
  0x001F0B55  6595                    xchg     ebp, eax                       
  0x001F0B57  55                      push     ebp                            
  0x001F0B58  1000                    adc      byte ptr [eax], al             
  0x001F0B5A  0000                    add      byte ptr [eax], al             
  0x001F0B5C  1100                    adc      dword ptr [eax], eax           
  0x001F0B5E  0000                    add      byte ptr [eax], al             
  0x001F0B60  1200                    adc      al, byte ptr [eax]             
  0x001F0B62  0000                    add      byte ptr [eax], al             
  0x001F0B64  0000                    add      byte ptr [eax], al             
  0x001F0B66  0000                    add      byte ptr [eax], al             
  0x001F0B68  0800                    or       byte ptr [eax], al             
  0x001F0B6A  0000                    add      byte ptr [eax], al             
  0x001F0B6C  07                      pop      es                             
  0x001F0B6D  0000                    add      byte ptr [eax], al             
  0x001F0B6F  0009                    add      byte ptr [ecx], cl             
  0x001F0B71  0000                    add      byte ptr [eax], al             
  0x001F0B73  0006                    add      byte ptr [esi], al             
  0x001F0B75  0000                    add      byte ptr [eax], al             
  0x001F0B77  000a                    add      byte ptr [edx], cl             
  0x001F0B79  0000                    add      byte ptr [eax], al             
  0x001F0B7B  00050000000b            add      byte ptr [0xb000000], al       
  0x001F0B81  0000                    add      byte ptr [eax], al             
  0x001F0B83  000400                  add      byte ptr [eax + eax], al       
  0x001F0B86  0000                    add      byte ptr [eax], al             
  0x001F0B88  0c00                    or       al, 0                          
  0x001F0B8A  0000                    add      byte ptr [eax], al             
  0x001F0B8C  0300                    add      eax, dword ptr [eax]           
  0x001F0B8E  0000                    add      byte ptr [eax], al             
  0x001F0B90  0d00000002              or       eax, 0x2000000                 
  0x001F0B95  0000                    add      byte ptr [eax], al             
  0x001F0B97  000e                    add      byte ptr [esi], cl             
  0x001F0B99  0000                    add      byte ptr [eax], al             
  0x001F0B9B  0001                    add      byte ptr [ecx], al             
  0x001F0B9D  0000                    add      byte ptr [eax], al             
  0x001F0B9F  000f                    add      byte ptr [edi], cl             
  0x001F0BA1  0000                    add      byte ptr [eax], al             
  0x001F0BA3  00696e                  add      byte ptr [ecx + 0x6e], ch      
  0x001F0BA6  636f6d                  arpl     word ptr [edi + 0x6d], bp      
  0x001F0BA9  7061                    jo       0x1f0c0c                       
  0x001F0BAB  7469                    je       0x1f0c16                       
  0x001F0BAD  626c6520                bound    ebp, qword ptr [ebp + 0x20]    
  0x001F0BB1  7665                    jbe      0x1f0c18                       
  0x001F0BB3  7273                    jb       0x1f0c28                       
  0x001F0BB5  696f6e00000000          imul     ebp, dword ptr [edi + 0x6e], 0 
  0x001F0BBC  627566                  bound    esi, qword ptr [ebp + 0x66]    
  0x001F0BBF  66657220                jb       0x1f0be3                       
  0x001F0BC3  657272                  jb       0x1f0c38                       
  0x001F0BC6  6f                      outsd    dx, dword ptr [esi]            
  0x001F0BC7  7200                    jb       0x1f0bc9                       
                                        ; XREF: 0x001F0BC7 (cond_jump)
  0x001F0BC9  0000                    add      byte ptr [eax], al             
  0x001F0BCB  00696e                  add      byte ptr [ecx + 0x6e], ch      
  0x001F0BCE  7375                    jae      0x1f0c45                       
  0x001F0BD0  6666696369656e          imul     sp, word ptr [ebx + 0x69], 0x6e65 
  0x001F0BD7  7420                    je       0x1f0bf9                       
  0x001F0BD9  6d                      insd     dword ptr es:[edi], dx         
  0x001F0BDA  656d                    insd     dword ptr es:[edi], dx         
  0x001F0BDC  6f                      outsd    dx, dword ptr [esi]            
  0x001F0BDD  7279                    jb       0x1f0c58                       
  0x001F0BDF  00646174                add      byte ptr [ecx + 0x74], ah      
                                        ; XREF: 0x001F0BBF (cond_jump)
  0x001F0BE3  61                      popal                                   
  0x001F0BE4  206572                  and      byte ptr [ebp + 0x72], ah      
  0x001F0BE7  726f                    jb       0x1f0c58                       
  0x001F0BE9  7200                    jb       0x1f0beb                       
                                        ; XREF: 0x001F0BE9 (cond_jump)
  0x001F0BEB  007374                  add      byte ptr [ebx + 0x74], dh      
  0x001F0BEE  7265                    jb       0x1f0c55                       
  0x001F0BF0  61                      popal                                   
  0x001F0BF1  6d                      insd     dword ptr es:[edi], dx         
  0x001F0BF2  206572                  and      byte ptr [ebp + 0x72], ah      
  0x001F0BF5  726f                    jb       0x1f0c66                       
  0x001F0BF7  7200                    jb       0x1f0bf9                       
                                        ; XREF: 0x001F0BD7 (cond_jump), 0x001F0BF7 (cond_jump)
  0x001F0BF9  0000                    add      byte ptr [eax], al             
  0x001F0BFB  006669                  add      byte ptr [esi + 0x69], ah      
  0x001F0BFE  6c                      insb     byte ptr es:[edi], dx          
  0x001F0BFF  65206572                and      byte ptr gs:[ebp + 0x72], ah   
  0x001F0C03  726f                    jb       0x1f0c74                       
  0x001F0C05  7200                    jb       0x1f0c07                       
                                        ; XREF: 0x001F0C05 (cond_jump)
  0x001F0C07  007374                  add      byte ptr [ebx + 0x74], dh      
  0x001F0C0A  7265                    jb       0x1f0c71                       
                                        ; XREF: 0x001F0BA9 (cond_jump)
  0x001F0C0C  61                      popal                                   
  0x001F0C0D  6d                      insd     dword ptr es:[edi], dx         
  0x001F0C0E  20656e                  and      byte ptr [ebp + 0x6e], ah      
  0x001F0C11  640000                  add      byte ptr fs:[eax], al          
  0x001F0C14  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001F0BAB (cond_jump)
  0x001F0C16  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001F0BB1 (cond_jump)
  0x001F0C18  0300                    add      eax, dword ptr [eax]           
  0x001F0C1A  0000                    add      byte ptr [eax], al             
  0x001F0C1C  0400                    add      al, 0                          
  0x001F0C1E  0000                    add      byte ptr [eax], al             
  0x001F0C20  0500000006              add      eax, 0x6000000                 
  0x001F0C25  0000                    add      byte ptr [eax], al             
  0x001F0C27  0007                    add      byte ptr [edi], al             
  0x001F0C29  0000                    add      byte ptr [eax], al             
  0x001F0C2B  0008                    add      byte ptr [eax], cl             
  0x001F0C2D  0000                    add      byte ptr [eax], al             
  0x001F0C2F  0009                    add      byte ptr [ecx], cl             
  0x001F0C31  0000                    add      byte ptr [eax], al             
  0x001F0C33  000a                    add      byte ptr [edx], cl             
  0x001F0C35  0000                    add      byte ptr [eax], al             
  0x001F0C37  000b                    add      byte ptr [ebx], cl             
  0x001F0C39  0000                    add      byte ptr [eax], al             
  0x001F0C3B  000d0000000f            add      byte ptr [0xf000000], cl       
  0x001F0C41  0000                    add      byte ptr [eax], al             
  0x001F0C43  0011                    add      byte ptr [ecx], dl             
                                        ; XREF: 0x001F0BCE (cond_jump)
  0x001F0C45  0000                    add      byte ptr [eax], al             
  0x001F0C47  0013                    add      byte ptr [ebx], dl             
  0x001F0C49  0000                    add      byte ptr [eax], al             
  0x001F0C4B  0017                    add      byte ptr [edi], dl             
  0x001F0C4D  0000                    add      byte ptr [eax], al             
  0x001F0C4F  001b                    add      byte ptr [ebx], bl             
  0x001F0C51  0000                    add      byte ptr [eax], al             
  0x001F0C53  001f                    add      byte ptr [edi], bl             
                                        ; XREF: 0x001F0BEE (cond_jump)
  0x001F0C55  0000                    add      byte ptr [eax], al             
  0x001F0C57  0023                    add      byte ptr [ebx], ah             
  0x001F0C59  0000                    add      byte ptr [eax], al             
  0x001F0C5B  002b                    add      byte ptr [ebx], ch             
  0x001F0C5D  0000                    add      byte ptr [eax], al             
  0x001F0C5F  0033                    add      byte ptr [ebx], dh             
  0x001F0C61  0000                    add      byte ptr [eax], al             
  0x001F0C63  003b                    add      byte ptr [ebx], bh             
  0x001F0C65  0000                    add      byte ptr [eax], al             
  0x001F0C67  004300                  add      byte ptr [ebx], al             
  0x001F0C6A  0000                    add      byte ptr [eax], al             
  0x001F0C6C  53                      push     ebx                            
  0x001F0C6D  0000                    add      byte ptr [eax], al             
  0x001F0C6F  006300                  add      byte ptr [ebx], ah             
  0x001F0C72  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x001F0C03 (cond_jump)
  0x001F0C74  7300                    jae      0x1f0c76                       
                                        ; XREF: 0x001F0C74 (cond_jump)
  0x001F0C76  0000                    add      byte ptr [eax], al             
  0x001F0C78  830000                  add      dword ptr [eax], 0             
  0x001F0C7B  00a3000000c3            add      byte ptr [ebx - 0x3d000000], ah 
  0x001F0C81  0000                    add      byte ptr [eax], al             
  0x001F0C83  00e3                    add      bl, ah                         
  0x001F0C85  0000                    add      byte ptr [eax], al             
  0x001F0C87  0002                    add      byte ptr [edx], al             
  0x001F0C89  0100                    add      dword ptr [eax], eax           
  0x001F0C8B  0000                    add      byte ptr [eax], al             
  0x001F0C8D  0000                    add      byte ptr [eax], al             
  0x001F0C8F  0000                    add      byte ptr [eax], al             
  0x001F0C91  0000                    add      byte ptr [eax], al             
  0x001F0C93  0000                    add      byte ptr [eax], al             
  0x001F0C95  0000                    add      byte ptr [eax], al             
  0x001F0C97  0000                    add      byte ptr [eax], al             
  0x001F0C99  0000                    add      byte ptr [eax], al             
  0x001F0C9B  0000                    add      byte ptr [eax], al             
  0x001F0C9D  0000                    add      byte ptr [eax], al             
  0x001F0C9F  0000                    add      byte ptr [eax], al             
  0x001F0CA1  0000                    add      byte ptr [eax], al             
  0x001F0CA3  0000                    add      byte ptr [eax], al             
  0x001F0CA5  0000                    add      byte ptr [eax], al             
  0x001F0CA7  0000                    add      byte ptr [eax], al             
  0x001F0CA9  0000                    add      byte ptr [eax], al             
  0x001F0CAB  0000                    add      byte ptr [eax], al             
  0x001F0CAD  0000                    add      byte ptr [eax], al             
  0x001F0CAF  0000                    add      byte ptr [eax], al             
  0x001F0CB1  0000                    add      byte ptr [eax], al             
  0x001F0CB3  0000                    add      byte ptr [eax], al             
  0x001F0CB5  0000                    add      byte ptr [eax], al             
  0x001F0CB7  0001                    add      byte ptr [ecx], al             
  0x001F0CB9  0000                    add      byte ptr [eax], al             
  0x001F0CBB  0001                    add      byte ptr [ecx], al             
  0x001F0CBD  0000                    add      byte ptr [eax], al             
  0x001F0CBF  0001                    add      byte ptr [ecx], al             
  0x001F0CC1  0000                    add      byte ptr [eax], al             
  0x001F0CC3  0001                    add      byte ptr [ecx], al             
  0x001F0CC5  0000                    add      byte ptr [eax], al             
  0x001F0CC7  0002                    add      byte ptr [edx], al             
  0x001F0CC9  0000                    add      byte ptr [eax], al             
  0x001F0CCB  0002                    add      byte ptr [edx], al             
  0x001F0CCD  0000                    add      byte ptr [eax], al             
  0x001F0CCF  0002                    add      byte ptr [edx], al             
  0x001F0CD1  0000                    add      byte ptr [eax], al             
  0x001F0CD3  0002                    add      byte ptr [edx], al             
  0x001F0CD5  0000                    add      byte ptr [eax], al             
  0x001F0CD7  0003                    add      byte ptr [ebx], al             
  0x001F0CD9  0000                    add      byte ptr [eax], al             
  0x001F0CDB  0003                    add      byte ptr [ebx], al             
  0x001F0CDD  0000                    add      byte ptr [eax], al             
  0x001F0CDF  0003                    add      byte ptr [ebx], al             
  0x001F0CE1  0000                    add      byte ptr [eax], al             
  0x001F0CE3  0003                    add      byte ptr [ebx], al             
  0x001F0CE5  0000                    add      byte ptr [eax], al             
  0x001F0CE7  000400                  add      byte ptr [eax + eax], al       
  0x001F0CEA  0000                    add      byte ptr [eax], al             
  0x001F0CEC  0400                    add      al, 0                          
  0x001F0CEE  0000                    add      byte ptr [eax], al             
  0x001F0CF0  0400                    add      al, 0                          
  0x001F0CF2  0000                    add      byte ptr [eax], al             
  0x001F0CF4  0400                    add      al, 0                          
  0x001F0CF6  0000                    add      byte ptr [eax], al             
  0x001F0CF8  0500000005              add      eax, 0x5000000                 
  0x001F0CFD  0000                    add      byte ptr [eax], al             
  0x001F0CFF  000500000005            add      byte ptr [0x5000000], al       
  0x001F0D05  0000                    add      byte ptr [eax], al             
  0x001F0D07  0000                    add      byte ptr [eax], al             
  0x001F0D09  0000                    add      byte ptr [eax], al             
  0x001F0D0B  007000                  add      byte ptr [eax], dh             
  0x001F0D0E  0000                    add      byte ptr [eax], al             
  0x001F0D10  7000                    jo       0x1f0d12                       
                                        ; XREF: 0x001F0D10 (cond_jump)
  0x001F0D12  0000                    add      byte ptr [eax], al             
  0x001F0D14  0000                    add      byte ptr [eax], al             
  0x001F0D16  0000                    add      byte ptr [eax], al             
  0x001F0D18  0100                    add      dword ptr [eax], eax           
  0x001F0D1A  0000                    add      byte ptr [eax], al             
  0x001F0D1C  0200                    add      al, byte ptr [eax]             
  0x001F0D1E  0000                    add      byte ptr [eax], al             
  0x001F0D20  0300                    add      eax, dword ptr [eax]           
  0x001F0D22  0000                    add      byte ptr [eax], al             
  0x001F0D24  0400                    add      al, 0                          
  0x001F0D26  0000                    add      byte ptr [eax], al             
  0x001F0D28  0500000007              add      eax, 0x7000000                 
  0x001F0D2D  0000                    add      byte ptr [eax], al             
  0x001F0D2F  0009                    add      byte ptr [ecx], cl             
  0x001F0D31  0000                    add      byte ptr [eax], al             
  0x001F0D33  000d00000011            add      byte ptr [0x11000000], cl      
  0x001F0D39  0000                    add      byte ptr [eax], al             
  0x001F0D3B  0019                    add      byte ptr [ecx], bl             
  0x001F0D3D  0000                    add      byte ptr [eax], al             
  0x001F0D3F  0021                    add      byte ptr [ecx], ah             
  0x001F0D41  0000                    add      byte ptr [eax], al             
  0x001F0D43  0031                    add      byte ptr [ecx], dh             
  0x001F0D45  0000                    add      byte ptr [eax], al             
  0x001F0D47  004100                  add      byte ptr [ecx], al             
  0x001F0D4A  0000                    add      byte ptr [eax], al             
  0x001F0D4C  61                      popal                                   
  0x001F0D4D  0000                    add      byte ptr [eax], al             
  0x001F0D4F  0081000000c1            add      byte ptr [ecx - 0x3f000000], al 
  0x001F0D55  0000                    add      byte ptr [eax], al             
  0x001F0D57  0001                    add      byte ptr [ecx], al             
  0x001F0D59  0100                    add      dword ptr [eax], eax           
  0x001F0D5B  008101000001            add      byte ptr [ecx + 0x1000001], al 
  0x001F0D61  0200                    add      al, byte ptr [eax]             
  0x001F0D63  0001                    add      byte ptr [ecx], al             
  0x001F0D65  0300                    add      eax, dword ptr [eax]           
  0x001F0D67  0001                    add      byte ptr [ecx], al             
  0x001F0D69  0400                    add      al, 0                          
  0x001F0D6B  0001                    add      byte ptr [ecx], al             
  0x001F0D6D  06                      push     es                             
  0x001F0D6E  0000                    add      byte ptr [eax], al             
  0x001F0D70  0108                    add      dword ptr [eax], ecx           
  0x001F0D72  0000                    add      byte ptr [eax], al             
  0x001F0D74  010c00                  add      dword ptr [eax + eax], ecx     
  0x001F0D77  0001                    add      byte ptr [ecx], al             
  0x001F0D79  1000                    adc      byte ptr [eax], al             
  0x001F0D7B  0001                    add      byte ptr [ecx], al             
  0x001F0D7D  1800                    sbb      byte ptr [eax], al             
  0x001F0D7F  0001                    add      byte ptr [ecx], al             
  0x001F0D81  2000                    and      byte ptr [eax], al             
  0x001F0D83  0001                    add      byte ptr [ecx], al             
  0x001F0D85  3000                    xor      byte ptr [eax], al             
  0x001F0D87  0001                    add      byte ptr [ecx], al             
  0x001F0D89  40                      inc      eax                            
  0x001F0D8A  0000                    add      byte ptr [eax], al             
  0x001F0D8C  016000                  add      dword ptr [eax], esp           
  0x001F0D8F  0000                    add      byte ptr [eax], al             
  0x001F0D91  0000                    add      byte ptr [eax], al             
  0x001F0D93  0000                    add      byte ptr [eax], al             
  0x001F0D95  0000                    add      byte ptr [eax], al             
  0x001F0D97  0000                    add      byte ptr [eax], al             
  0x001F0D99  0000                    add      byte ptr [eax], al             
  0x001F0D9B  0000                    add      byte ptr [eax], al             
  0x001F0D9D  0000                    add      byte ptr [eax], al             
  0x001F0D9F  0001                    add      byte ptr [ecx], al             
  0x001F0DA1  0000                    add      byte ptr [eax], al             
  0x001F0DA3  0001                    add      byte ptr [ecx], al             
  0x001F0DA5  0000                    add      byte ptr [eax], al             
  0x001F0DA7  0002                    add      byte ptr [edx], al             
  0x001F0DA9  0000                    add      byte ptr [eax], al             
  0x001F0DAB  0002                    add      byte ptr [edx], al             
  0x001F0DAD  0000                    add      byte ptr [eax], al             
  0x001F0DAF  0003                    add      byte ptr [ebx], al             
  0x001F0DB1  0000                    add      byte ptr [eax], al             
  0x001F0DB3  0003                    add      byte ptr [ebx], al             
  0x001F0DB5  0000                    add      byte ptr [eax], al             
  0x001F0DB7  000400                  add      byte ptr [eax + eax], al       
  0x001F0DBA  0000                    add      byte ptr [eax], al             
  0x001F0DBC  0400                    add      al, 0                          
  0x001F0DBE  0000                    add      byte ptr [eax], al             
  0x001F0DC0  0500000005              add      eax, 0x5000000                 
  0x001F0DC5  0000                    add      byte ptr [eax], al             
  0x001F0DC7  0006                    add      byte ptr [esi], al             
  0x001F0DC9  0000                    add      byte ptr [eax], al             
  0x001F0DCB  0006                    add      byte ptr [esi], al             
  0x001F0DCD  0000                    add      byte ptr [eax], al             
  0x001F0DCF  0007                    add      byte ptr [edi], al             
  0x001F0DD1  0000                    add      byte ptr [eax], al             
  0x001F0DD3  0007                    add      byte ptr [edi], al             
  0x001F0DD5  0000                    add      byte ptr [eax], al             
  0x001F0DD7  0008                    add      byte ptr [eax], cl             
  0x001F0DD9  0000                    add      byte ptr [eax], al             
  0x001F0DDB  0008                    add      byte ptr [eax], cl             
  0x001F0DDD  0000                    add      byte ptr [eax], al             
  0x001F0DDF  0009                    add      byte ptr [ecx], cl             
  0x001F0DE1  0000                    add      byte ptr [eax], al             
  0x001F0DE3  0009                    add      byte ptr [ecx], cl             
  0x001F0DE5  0000                    add      byte ptr [eax], al             
  0x001F0DE7  000a                    add      byte ptr [edx], cl             
  0x001F0DE9  0000                    add      byte ptr [eax], al             
  0x001F0DEB  000a                    add      byte ptr [edx], cl             
  0x001F0DED  0000                    add      byte ptr [eax], al             
  0x001F0DEF  000b                    add      byte ptr [ebx], cl             
  0x001F0DF1  0000                    add      byte ptr [eax], al             
  0x001F0DF3  000b                    add      byte ptr [ebx], cl             
  0x001F0DF5  0000                    add      byte ptr [eax], al             
  0x001F0DF7  000c00                  add      byte ptr [eax + eax], cl       
  0x001F0DFA  0000                    add      byte ptr [eax], al             
  0x001F0DFC  0c00                    or       al, 0                          
  0x001F0DFE  0000                    add      byte ptr [eax], al             
  0x001F0E00  0d0000000d              or       eax, 0xd000000                 
  0x001F0E05  0000                    add      byte ptr [eax], al             
  0x001F0E07  008e09000084            add      byte ptr [esi - 0x7bfffff7], cl 
  0x001F0E0F  ff5111                  call     dword ptr [ecx + 0x11]         
  0x001F0E12  0000                    add      byte ptr [eax], al             
  0x001F0E14  7e18                    jle      0x1f0e2e                       
  0x001F0E16  0000                    add      byte ptr [eax], al             
  0x001F0E18  33e3                    xor      esp, ebx                       
  0x001F0E1B  ffa12500000b            jmp      dword ptr [ecx + 0xb000025]    
  0x001F0E21  3000                    xor      byte ptr [eax], al             
  0x001F0E23  00df                    add      bh, bl                         
  0x001F0E28  3bc1                    cmp      eax, ecx                       
  0x001F0E2B  ffb3410000fd            push     dword ptr [ebx - 0x2ffffbf]    
  0x001F0E31  ad                      lodsd    eax, dword ptr [esi]           
  0x001F0E33  ff546200                call     dword ptr [edx]                
  0x001F0E37  0000                    add      byte ptr [eax], al             
  0x001F0E39  0400                    add      al, 0                          
  0x001F0E3B  0000                    add      byte ptr [eax], al             
  0x001F0E3D  0002                    add      byte ptr [edx], al             
  0x001F0E3F  00ff                    add      bh, bh                         
  0x001F0E41  0300                    add      eax, dword ptr [eax]           
  0x001F0E43  0000                    add      byte ptr [eax], al             
  0x001F0E45  0000                    add      byte ptr [eax], al             
  0x001F0E47  008e0900008e            add      byte ptr [esi - 0x71fffff7], cl 
  0x001F0E4D  0900                    or       dword ptr [eax], eax           
  0x001F0E4F  007c0c00                add      byte ptr [esp + ecx], bh       
  0x001F0E53  007c0c00                add      byte ptr [esp + ecx], bh       
  0x001F0E57  005111                  add      byte ptr [ecx + 0x11], dl      
  0x001F0E5A  0000                    add      byte ptr [eax], al             
  0x001F0E5C  51                      push     ecx                            
  0x001F0E5D  1100                    adc      dword ptr [eax], eax           
  0x001F0E5F  007e18                  add      byte ptr [esi + 0x18], bh      
  0x001F0E62  0000                    add      byte ptr [eax], al             
  0x001F0E64  7e18                    jle      0x1f0e7e                       
  0x001F0E66  0000                    add      byte ptr [eax], al             
  0x001F0E68  cd1c                    int      0x1c                           
  0x001F0E6A  0000                    add      byte ptr [eax], al             
  0x001F0E6C  cd1c                    int      0x1c                           
  0x001F0E6E  0000                    add      byte ptr [eax], al             
  0x001F0E70  a1250000a1              mov      eax, dword ptr [0xa1000025]    
  0x001F0E75  2500000b30              and      eax, 0x300b0000                
  0x001F0E7A  0000                    add      byte ptr [eax], al             
  0x001F0E7C  0b30                    or       esi, dword ptr [eax]           
                                        ; XREF: 0x001F0E64 (cond_jump)
  0x001F0E7E  0000                    add      byte ptr [eax], al             
  0x001F0E80  213b                    and      dword ptr [ebx], edi           
  0x001F0E82  0000                    add      byte ptr [eax], al             
  0x001F0E84  213b                    and      dword ptr [ebx], edi           
  0x001F0E86  0000                    add      byte ptr [eax], al             
  0x001F0E88  c53e                    lds      edi, ptr [esi]                 
  0x001F0E8A  0000                    add      byte ptr [eax], al             
  0x001F0E8C  c53e                    lds      edi, ptr [esi]                 
  0x001F0E8E  0000                    add      byte ptr [eax], al             
  0x001F0E90  b341                    mov      bl, 0x41                       
  0x001F0E92  0000                    add      byte ptr [eax], al             
  0x001F0E94  b341                    mov      bl, 0x41                       
  0x001F0E96  0000                    add      byte ptr [eax], al             
  0x001F0E98  035200                  add      edx, dword ptr [edx]           
  0x001F0E9B  0003                    add      byte ptr [ebx], al             
  0x001F0E9D  52                      push     edx                            
  0x001F0E9E  0000                    add      byte ptr [eax], al             
  0x001F0EA0  54                      push     esp                            
  0x001F0EA1  6200                    bound    eax, qword ptr [eax]           
  0x001F0EA3  00546200                add      byte ptr [edx], dl             
  0x001F0EA7  00ff                    add      bh, bh                         
  0x001F0EAF  ff01                    inc      dword ptr [ecx]                
  0x001F0EB1  0000                    add      byte ptr [eax], al             
  0x001F0EB3  0001                    add      byte ptr [ecx], al             
  0x001F0EB5  0000                    add      byte ptr [eax], al             
  0x001F0EB7  0001                    add      byte ptr [ecx], al             
  0x001F0EB9  0000                    add      byte ptr [eax], al             
  0x001F0EBB  0001                    add      byte ptr [ecx], al             
  0x001F0EBD  0000                    add      byte ptr [eax], al             
  0x001F0EBF  0000                    add      byte ptr [eax], al             
  0x001F0EC1  0400                    add      al, 0                          
  0x001F0EC3  0000                    add      byte ptr [eax], al             
  0x001F0EC5  0400                    add      al, 0                          
  0x001F0EC7  0000                    add      byte ptr [eax], al             
  0x001F0EC9  0002                    add      byte ptr [edx], al             
  0x001F0ECB  0000                    add      byte ptr [eax], al             
  0x001F0ECD  0002                    add      byte ptr [edx], al             
  0x001F0ECF  00ff                    add      bh, bh                         
  0x001F0ED1  0300                    add      eax, dword ptr [eax]           
  0x001F0ED3  00ff                    add      bh, bh                         
  0x001F0ED5  0300                    add      eax, dword ptr [eax]           
  0x001F0ED7  007e18                  add      byte ptr [esi + 0x18], bh      
  0x001F0EDA  0000                    add      byte ptr [eax], al             
  0x001F0EDC  213b                    and      dword ptr [ebx], edi           
  0x001F0EDE  0000                    add      byte ptr [eax], al             
  0x001F0EE0  0000                    add      byte ptr [eax], al             
  0x001F0EE2  0000                    add      byte ptr [eax], al             
  0x001F0EE4  51                      push     ecx                            
  0x001F0EE5  1100                    adc      dword ptr [eax], eax           
  0x001F0EE7  0000                    add      byte ptr [eax], al             
  0x001F0EE9  000b                    add      byte ptr [ebx], cl             
  0x001F0EEB  3000                    xor      byte ptr [eax], al             
  0x001F0EED  00546200                add      byte ptr [edx], dl             
  0x001F0EF1  00b34100008e            add      byte ptr [ebx - 0x71ffffbf], dh 
  0x001F0EF7  0900                    or       dword ptr [eax], eax           
  0x001F0EF9  0003                    add      byte ptr [ebx], al             
  0x001F0EFB  52                      push     edx                            
  0x001F0EFC  0000                    add      byte ptr [eax], al             
  0x001F0EFE  cd1c                    int      0x1c                           
  0x001F0F00  0000                    add      byte ptr [eax], al             
  0x001F0F02  7c0c                    jl       0x1f0f10                       
  0x001F0F04  0000                    add      byte ptr [eax], al             
  0x001F0F06  c53e                    lds      edi, ptr [esi]                 
  0x001F0F08  0000                    add      byte ptr [eax], al             
  0x001F0F0A  0000                    add      byte ptr [eax], al             
  0x001F0F0C  0000                    add      byte ptr [eax], al             
  0x001F0F0E  a1256a0100              mov      eax, dword ptr [0x16a25]       
  0x001F0F13  0063fd                  add      byte ptr [ebx - 3], ah         
  0x001F0F17  ff15010000d9            call     dword ptr [0xd9000001]         
  0x001F0F1D  0100                    add      dword ptr [eax], eax           
  0x001F0F1F  006a01                  add      byte ptr [edx + 1], ch         
  0x001F0F22  0000                    add      byte ptr [eax], al             
  0x001F0F24  6a01                    push     1                              
  0x001F0F26  0000                    add      byte ptr [eax], al             
  0x001F0F28  63fd                    arpl     bp, di                         
  0x001F0F2A  0000                    add      byte ptr [eax], al             
  0x001F0F2C  63fd                    arpl     bp, di                         
  0x001F0F2E  0000                    add      byte ptr [eax], al             
  0x001F0F30  1501000015              adc      eax, 0x15000001                
  0x001F0F35  0100                    add      dword ptr [eax], eax           
  0x001F0F37  00d9                    add      cl, bl                         
  0x001F0F39  0100                    add      dword ptr [eax], eax           
  0x001F0F3B  00d9                    add      cl, bl                         
  0x001F0F3D  0100                    add      dword ptr [eax], eax           
