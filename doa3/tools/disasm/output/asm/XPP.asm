; ============================================================
; Section: XPP
; VA: 0x00210760 - 0x002179FC
; Size: 29340 bytes (28.7 KB)
; Functions: 177
; Instructions: 9748
; ============================================================

  0x00210760  0000                    add      byte ptr [eax], al             
  0x00210762  0000                    add      byte ptr [eax], al             
  0x00210764  1409                    adc      al, 9                          
  0x00210766  2100                    and      dword ptr [eax], eax           
  0x00210768  90                      nop                                     
  0x00210769  07                      pop      es                             
  0x0021076A  2100                    and      dword ptr [eax], eax           
  0x0021076C  3008                    xor      byte ptr [eax], cl             
  0x0021076E  2100                    and      dword ptr [eax], eax           
  0x00210770  48                      dec      eax                            
  0x00210771  0821                    or       byte ptr [ecx], ah             
  0x00210773  0000                    add      byte ptr [eax], al             
  0x00210775  0000                    add      byte ptr [eax], al             
  0x00210777  0000                    add      byte ptr [eax], al             
  0x00210779  0000                    add      byte ptr [eax], al             
  0x0021077B  0000                    add      byte ptr [eax], al             
  0x0021077D  0000                    add      byte ptr [eax], al             
  0x0021077F  0000                    add      byte ptr [eax], al             
  0x00210781  0000                    add      byte ptr [eax], al             
  0x00210783  0000                    add      byte ptr [eax], al             
  0x00210785  0000                    add      byte ptr [eax], al             
  0x00210787  0000                    add      byte ptr [eax], al             
  0x00210789  0000                    add      byte ptr [eax], al             
  0x0021078B  008007210082            add      byte ptr [eax - 0x7dffdef9], al 
  0x00210791  08ff                    or       bh, bh                         
  0x00210793  ff2c09                  jmp      ptr [ecx + ecx]                
  0x00210796  2100                    and      dword ptr [eax], eax           
  0x00210798  3416                    xor      al, 0x16                       
  0x0021079A  2100                    and      dword ptr [eax], eax           
  0x0021079C  b617                    mov      dh, 0x17                       
  0x0021079E  2100                    and      dword ptr [eax], eax           
  0x002107A0  0100                    add      dword ptr [eax], eax           
  0x002107A2  0000                    add      byte ptr [eax], al             
  0x002107A4  8c07                    mov      word ptr [edi], es             
  0x002107A6  2100                    and      dword ptr [eax], eax           
  0x002107A8  0200                    add      al, byte ptr [eax]             
  0x002107AA  0001                    add      byte ptr [ecx], al             
  0x002107AC  0200                    add      al, byte ptr [eax]             
  0x002107AE  0020                    add      byte ptr [eax], ah             
  0x002107B0  0000                    add      byte ptr [eax], al             
  0x002107B2  0000                    add      byte ptr [eax], al             
  0x002107B4  0000                    add      byte ptr [eax], al             
  0x002107B6  0000                    add      byte ptr [eax], al             
  0x002107B8  843421                  test     byte ptr [ecx], dh             
  0x002107BB  0000                    add      byte ptr [eax], al             
  0x002107BD  0000                    add      byte ptr [eax], al             
  0x002107BF  0000                    add      byte ptr [eax], al             
  0x002107C1  0000                    add      byte ptr [eax], al             
  0x002107C3  0000                    add      byte ptr [eax], al             
  0x002107C5  0000                    add      byte ptr [eax], al             
  0x002107C7  0000                    add      byte ptr [eax], al             
  0x002107C9  0000                    add      byte ptr [eax], al             
  0x002107CB  008729210087            add      byte ptr [edi - 0x78ffded7], al 
  0x002107D1  2921                    sub      dword ptr [ecx], esp           
  0x002107D3  0000                    add      byte ptr [eax], al             
  0x002107D5  0000                    add      byte ptr [eax], al             
  0x002107D7  0000                    add      byte ptr [eax], al             
  0x002107D9  0000                    add      byte ptr [eax], al             
  0x002107DB  0000                    add      byte ptr [eax], al             
  0x002107DD  0000                    add      byte ptr [eax], al             
  0x002107DF  0000                    add      byte ptr [eax], al             
  0x002107E1  0000                    add      byte ptr [eax], al             
  0x002107E3  0000                    add      byte ptr [eax], al             
  0x002107E5  0000                    add      byte ptr [eax], al             
  0x002107E7  0000                    add      byte ptr [eax], al             
  0x002107E9  0000                    add      byte ptr [eax], al             
  0x002107EB  008729210087            add      byte ptr [edi - 0x78ffded7], al 
  0x002107F1  2921                    sub      dword ptr [ecx], esp           
  0x002107F3  0000                    add      byte ptr [eax], al             
  0x002107F5  0000                    add      byte ptr [eax], al             
  0x002107F7  0000                    add      byte ptr [eax], al             
  0x002107F9  0000                    add      byte ptr [eax], al             
  0x002107FB  0000                    add      byte ptr [eax], al             
  0x002107FD  0000                    add      byte ptr [eax], al             
  0x002107FF  0000                    add      byte ptr [eax], al             
  0x00210801  0000                    add      byte ptr [eax], al             
  0x00210803  0000                    add      byte ptr [eax], al             
  0x00210805  0000                    add      byte ptr [eax], al             
  0x00210807  0000                    add      byte ptr [eax], al             
  0x00210809  0000                    add      byte ptr [eax], al             
  0x0021080B  0000                    add      byte ptr [eax], al             
  0x0021080D  0000                    add      byte ptr [eax], al             
  0x0021080F  0000                    add      byte ptr [eax], al             
  0x00210811  0000                    add      byte ptr [eax], al             
  0x00210813  0000                    add      byte ptr [eax], al             
  0x00210815  0000                    add      byte ptr [eax], al             
  0x00210817  0000                    add      byte ptr [eax], al             
  0x00210819  0000                    add      byte ptr [eax], al             
  0x0021081B  0000                    add      byte ptr [eax], al             
  0x0021081D  0000                    add      byte ptr [eax], al             
  0x0021081F  0000                    add      byte ptr [eax], al             
  0x00210821  0000                    add      byte ptr [eax], al             
  0x00210823  0000                    add      byte ptr [eax], al             
  0x00210825  0821                    or       byte ptr [ecx], ah             
  0x00210827  000c08                  add      byte ptr [eax + ecx], cl       
  0x0021082A  2100                    and      dword ptr [eax], eax           
  0x0021082C  1808                    sbb      byte ptr [eax], cl             
  0x0021082E  2100                    and      dword ptr [eax], eax           
  0x00210830  8203ff                  add      byte ptr [ebx], 0xff           
  0x00210833  ff430a                  inc      dword ptr [ebx + 0xa]          
  0x00210836  2100                    and      dword ptr [eax], eax           
  0x00210838  d6                      salc                                    
  0x00210839  262100                  and      dword ptr es:[eax], eax        
  0x0021083C  0121                    add      dword ptr [ecx], esp           
  0x0021083E  2100                    and      dword ptr [eax], eax           
  0x00210840  0300                    add      eax, dword ptr [eax]           
  0x00210842  0000                    add      byte ptr [eax], al             
  0x00210844  2408                    and      al, 8                          
  0x00210846  2100                    and      dword ptr [eax], eax           
  0x00210848  8258ffff                sbb      byte ptr [eax - 1], 0xff       
  0x0021084C  43                      inc      ebx                            
  0x0021084D  0a21                    or       ah, byte ptr [ecx]             
  0x0021084F  00d6                    add      dh, dl                         
  0x00210851  262100                  and      dword ptr es:[eax], eax        
  0x00210854  0121                    add      dword ptr [ecx], esp           
  0x00210856  2100                    and      dword ptr [eax], eax           
  0x00210858  0300                    add      eax, dword ptr [eax]           
  0x0021085A  0000                    add      byte ptr [eax], al             
  0x0021085C  2408                    and      al, 8                          
  0x0021085E  2100                    and      dword ptr [eax], eax           
  0x00210860  0000                    add      byte ptr [eax], al             
  0x00210862  0001                    add      byte ptr [ecx], al             
  0x00210864  0002                    add      byte ptr [edx], al             
  0x00210866  0a00                    or       al, byte ptr [eax]             
  0x00210868  0000                    add      byte ptr [eax], al             
  0x0021086A  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00210A43 (data_read), 0x00210A58 (data_read)
  0x0021086C  0000                    add      byte ptr [eax], al             
  0x0021086E  0000                    add      byte ptr [eax], al             
  0x00210870  0000                    add      byte ptr [eax], al             
  0x00210872  0000                    add      byte ptr [eax], al             
  0x00210874  0000                    add      byte ptr [eax], al             
  0x00210876  0000                    add      byte ptr [eax], al             
  0x00210878  0000                    add      byte ptr [eax], al             
  0x0021087A  0000                    add      byte ptr [eax], al             
  0x0021087C  0000                    add      byte ptr [eax], al             
  0x0021087E  0000                    add      byte ptr [eax], al             
  0x00210880  0000                    add      byte ptr [eax], al             
  0x00210882  0000                    add      byte ptr [eax], al             
  0x00210884  127008                  adc      dh, byte ptr [eax + 8]         
  0x00210887  2100                    and      dword ptr [eax], eax           
  0x00210889  0000                    add      byte ptr [eax], al             
  0x0021088B  0000                    add      byte ptr [eax], al             
  0x0021088D  0000                    add      byte ptr [eax], al             
  0x0021088F  00048c                  add      byte ptr [esp + ecx*4], al     
  0x00210892  0821                    or       byte ptr [ecx], ah             
  0x00210894  0000                    add      byte ptr [eax], al             
  0x00210896  0000                    add      byte ptr [eax], al             
  0x00210898  0108                    add      dword ptr [eax], ecx           
  0x0021089A  0000                    add      byte ptr [eax], al             
  0x0021089C  0000                    add      byte ptr [eax], al             
  0x0021089E  0000                    add      byte ptr [eax], al             
  0x002108A0  0000                    add      byte ptr [eax], al             
  0x002108A2  0000                    add      byte ptr [eax], al             
  0x002108A4  089c0821000000          or       byte ptr [eax + ecx + 0x21], bl 
  0x002108AB  0001                    add      byte ptr [ecx], al             
  0x002108AD  a908210000              test     eax, 0x2108                    
  0x002108B2  0000                    add      byte ptr [eax], al             
  0x002108B4  0110                    add      dword ptr [eax], edx           
  0x002108B6  0000                    add      byte ptr [eax], al             
  0x002108B8  0000                    add      byte ptr [eax], al             
  0x002108BA  0000                    add      byte ptr [eax], al             
  0x002108BC  04b8                    add      al, 0xb8                       
  0x002108BE  0821                    or       byte ptr [ecx], ah             
  0x002108C0  0000                    add      byte ptr [eax], al             
  0x002108C2  0000                    add      byte ptr [eax], al             
  0x002108C4  0110                    add      dword ptr [eax], edx           
  0x002108C6  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00210A87 (data_read), 0x00210AC4 (data_read)
  0x002108C8  0401                    add      al, 1                          
  0x002108CA  0000                    add      byte ptr [eax], al             
  0x002108CC  8408                    test     byte ptr [eax], cl             
  0x002108CE  2100                    and      dword ptr [eax], eax           
  0x002108D0  90                      nop                                     
  0x002108D1  0821                    or       byte ptr [ecx], ah             
  0x002108D3  00980821007f            add      byte ptr [eax + 0x7f002108], bl 
  0x002108D9  1e                      push     ds                             
  0x002108DA  2100                    and      dword ptr [eax], eax           
  0x002108DC  0000                    add      byte ptr [eax], al             
  0x002108DE  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00210A94 (data_read), 0x00210AB4 (data_read)
  0x002108E0  0101                    add      dword ptr [ecx], eax           
  0x002108E2  0000                    add      byte ptr [eax], al             
  0x002108E4  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x002108E5  0821                    or       byte ptr [ecx], ah             
  0x002108E7  00ac082100b408          add      byte ptr [eax + ecx + 0x8b40021], ch 
  0x002108EE  2100                    and      dword ptr [eax], eax           
  0x002108F0  e41e                    in       al, 0x1e                       
  0x002108F2  2100                    and      dword ptr [eax], eax           
  0x002108F4  0300                    add      eax, dword ptr [eax]           
  0x002108F6  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00210AAA (data_read), 0x00210ABB (data_read)
  0x002108F8  0101                    add      dword ptr [ecx], eax           
  0x002108FA  0000                    add      byte ptr [eax], al             
  0x002108FC  bc08210000              mov      esp, 0x2108                    
  0x00210901  0000                    add      byte ptr [eax], al             
  0x00210903  00c4                    add      ah, al                         
  0x00210905  0821                    or       byte ptr [ecx], ah             
  0x00210907  00c0                    add      al, al                         
  0x00210909  1e                      push     ds                             
  0x0021090A  2100                    and      dword ptr [eax], eax           
  0x0021090C  0000                    add      byte ptr [eax], al             
  0x0021090E  0000                    add      byte ptr [eax], al             
  0x00210910  0000                    add      byte ptr [eax], al             
  0x00210912  0000                    add      byte ptr [eax], al             
  0x00210914  81090000f211            or       dword ptr [ecx], 0x11f20000    
  0x0021091A  2100                    and      dword ptr [eax], eax           
  0x0021091C  625f21                  bound    ebx, qword ptr [edi + 0x21]    
  0x0021091F  00c9                    add      cl, cl                         
  0x00210921  59                      pop      ecx                            
  0x00210922  2100                    and      dword ptr [eax], eax           
  0x00210924  0100                    add      dword ptr [eax], eax           
  0x00210926  0000                    add      byte ptr [eax], al             
  0x00210928  1009                    adc      byte ptr [ecx], cl             
  0x0021092A  2100                    and      dword ptr [eax], eax           

; ============================================================
; Function: sub_0021092C
; Start: 0x0021092C  End: 0x0021097D  Size: 81 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00210CAA
; ============================================================
sub_0021092C:
  0x0021092C  55                      push     ebp                            
  0x0021092D  8bec                    mov      ebp, esp                       
  0x0021092F  83ec10                  sub      esp, 0x10                      
  0x00210932  56                      push     esi                            
  0x00210933  57                      push     edi                            
  0x00210934  33c0                    xor      eax, eax                       
  0x00210936  b9040c0000              mov      ecx, 0xc04                     
  0x0021093B  bfc8704000              mov      edi, 0x4070c8                  
  0x00210940  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00210942  b8cc704000              mov      eax, 0x4070cc                  
  0x00210947  6a60                    push     0x60                           
  0x00210949  a3d0704000              mov      dword ptr [0x4070d0], eax      
  0x0021094E  a3cc704000              mov      dword ptr [0x4070cc], eax      
  0x00210953  ff15087b2100            call     dword ptr [0x217b08]           ; -> xbox_DbgPrint
  0x00210959  8bf8                    mov      edi, eax                       
  0x0021095B  6a18                    push     0x18                           
  0x0021095D  59                      pop      ecx                            
  0x0021095E  33c0                    xor      eax, eax                       
  0x00210960  893dd4a04000            mov      dword ptr [0x40a0d4], edi      
  0x00210966  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00210968  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x0021096B  e83a030000              call     0x210caa                       ; -> sub_00210CAA
  0x00210970  85c0                    test     eax, eax                       
  0x00210972  7409                    je       0x21097d                       
  0x00210974  c745f808000000          mov      dword ptr [ebp - 8], 8         
  0x0021097B  eb1a                    jmp      0x210997                       
; end of function
                                        ; XREF: 0x00210972 (cond_jump)
  0x0021097D  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00210980  6880072100              push     0x210780                       
  0x00210985  e8e9020000              call     0x210c73                       ; -> sub_00210C73
  0x0021098A  6a08                    push     8                              
  0x0021098C  59                      pop      ecx                            
  0x0021098D  3bc1                    cmp      eax, ecx                       
  0x0021098F  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x00210992  7603                    jbe      0x210997                       
  0x00210994  894df8                  mov      dword ptr [ebp - 8], ecx       
                                        ; XREF: 0x0021097B (jump), 0x00210992 (cond_jump)
  0x00210997  68cc792100              push     0x2179cc                       
  0x0021099C  8d45f0                  lea      eax, [ebp - 0x10]              
  0x0021099F  50                      push     eax                            
  0x002109A0  ff15047a2100            call     dword ptr [0x217a04]           ; -> xbox_RtlInitAnsiString
  0x002109A6  33f6                    xor      esi, esi                       
  0x002109A8  3975f8                  cmp      dword ptr [ebp - 8], esi       
  0x002109AB  7670                    jbe      0x210a1d                       
                                        ; XREF: 0x00210A1B (cond_jump)
  0x002109AD  83fe09                  cmp      esi, 9                         
  0x002109B0  8d4641                  lea      eax, [esi + 0x41]              
  0x002109B3  7703                    ja       0x2109b8                       
  0x002109B5  8d4630                  lea      eax, [esi + 0x30]              
                                        ; XREF: 0x002109B3 (cond_jump)
  0x002109B8  8b4df4                  mov      ecx, dword ptr [ebp - 0xc]     
  0x002109BB  88410b                  mov      byte ptr [ecx + 0xb], al       
  0x002109BE  8d45fc                  lea      eax, [ebp - 4]                 
  0x002109C1  50                      push     eax                            
  0x002109C2  6a00                    push     0                              
  0x002109C4  6a3a                    push     0x3a                           
  0x002109C6  8d45f0                  lea      eax, [ebp - 0x10]              
  0x002109C9  50                      push     eax                            
  0x002109CA  6870010000              push     0x170                          
  0x002109CF  68b8072100              push     0x2107b8                       
  0x002109D4  ff15047b2100            call     dword ptr [0x217b04]           ; -> xbox_IoCompletionObjectType
  0x002109DA  85c0                    test     eax, eax                       
  0x002109DC  7c3f                    jl       0x210a1d                       
  0x002109DE  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x002109E1  8b5018                  mov      edx, dword ptr [eax + 0x18]    
  0x002109E4  33c0                    xor      eax, eax                       
  0x002109E6  6a5c                    push     0x5c                           
  0x002109E8  59                      pop      ecx                            
  0x002109E9  8bfa                    mov      edi, edx                       
  0x002109EB  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x002109ED  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x002109F0  8902                    mov      dword ptr [edx], eax           
  0x002109F2  897204                  mov      dword ptr [edx + 4], esi       
  0x002109F5  c7420c04000000          mov      dword ptr [edx + 0xc], 4       
  0x002109FC  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x002109FF  c6401e01                mov      byte ptr [eax + 0x1e], 1       
  0x00210A03  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00210A06  83481404                or       dword ptr [eax + 0x14], 4      
  0x00210A0A  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00210A0D  836014ef                and      dword ptr [eax + 0x14], 0xffffffef 
  0x00210A11  52                      push     edx                            
  0x00210A12  e8f50b0000              call     0x21160c                       ; -> sub_0021160C
  0x00210A17  46                      inc      esi                            
  0x00210A18  3b75f8                  cmp      esi, dword ptr [ebp - 8]       
  0x00210A1B  7290                    jb       0x2109ad                       
                                        ; XREF: 0x002109AB (cond_jump), 0x002109DC (cond_jump)
  0x00210A1D  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00210A20  83f801                  cmp      eax, 1                         
  0x00210A23  5f                      pop      edi                            
  0x00210A24  a2a9072100              mov      byte ptr [0x2107a9], al        
  0x00210A29  5e                      pop      esi                            
  0x00210A2A  7606                    jbe      0x210a32                       
  0x00210A2C  d025af072100            shl      byte ptr [0x2107af], 1         
                                        ; XREF: 0x00210A2A (cond_jump)
  0x00210A32  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00210A35  68a8072100              push     0x2107a8                       
  0x00210A3A  e877020000              call     0x210cb6                       ; -> sub_00210CB6
  0x00210A3F  c9                      leave                                   
  0x00210A40  c20400                  ret      4                              
  0x00210A43  833d6c08210000          cmp      dword ptr [0x21086c], 0        
  0x00210A4A  0f8519010000            jne      0x210b69                       
  0x00210A50  53                      push     ebx                            
  0x00210A51  56                      push     esi                            
  0x00210A52  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00210A56  8bce                    mov      ecx, esi                       
  0x00210A58  c7056c08210001000000    mov      dword ptr [0x21086c], 1        
  0x00210A62  66c705d8a040000400      mov      word ptr [0x40a0d8], 4         
  0x00210A6B  e83a020000              call     0x210caa                       ; -> sub_00210CAA
  0x00210A70  85c0                    test     eax, eax                       
  0x00210A72  7540                    jne      0x210ab4                       
  0x00210A74  6800082100              push     0x210800                       
  0x00210A79  8bce                    mov      ecx, esi                       
  0x00210A7B  e8f3010000              call     0x210c73                       ; -> sub_00210C73
  0x00210A80  680c082100              push     0x21080c                       
  0x00210A85  8bce                    mov      ecx, esi                       
  0x00210A87  a2c8082100              mov      byte ptr [0x2108c8], al        
  0x00210A8C  0fb6d8                  movzx    ebx, al                        
  0x00210A8F  e8df010000              call     0x210c73                       ; -> sub_00210C73
  0x00210A94  a2e0082100              mov      byte ptr [0x2108e0], al        
  0x00210A99  0fb6c0                  movzx    eax, al                        
  0x00210A9C  6818082100              push     0x210818                       
  0x00210AA1  8bce                    mov      ecx, esi                       
  0x00210AA3  03d8                    add      ebx, eax                       
  0x00210AA5  e8c9010000              call     0x210c73                       ; -> sub_00210C73
  0x00210AAA  a2f8082100              mov      byte ptr [0x2108f8], al        
  0x00210AAF  0fb6c0                  movzx    eax, al                        
  0x00210AB2  eb17                    jmp      0x210acb                       
                                        ; XREF: 0x00210A72 (cond_jump)
  0x00210AB4  0fb605e0082100          movzx    eax, byte ptr [0x2108e0]       
  0x00210ABB  0fb61df8082100          movzx    ebx, byte ptr [0x2108f8]       
  0x00210AC2  03d8                    add      ebx, eax                       
  0x00210AC4  0fb605c8082100          movzx    eax, byte ptr [0x2108c8]       
                                        ; XREF: 0x00210AB2 (jump)
  0x00210ACB  03d8                    add      ebx, eax                       
  0x00210ACD  0fb705d8a04000          movzx    eax, word ptr [0x40a0d8]       
  0x00210AD4  3bd8                    cmp      ebx, eax                       
  0x00210AD6  7602                    jbe      0x210ada                       
  0x00210AD8  8bd8                    mov      ebx, eax                       
                                        ; XREF: 0x00210AD6 (cond_jump)
  0x00210ADA  8bcb                    mov      ecx, ebx                       
  0x00210ADC  69c9a8000000            imul     ecx, ecx, 0xa8                 
  0x00210AE2  8d04c0                  lea      eax, [eax + eax*8]             
  0x00210AE5  8d0441                  lea      eax, [ecx + eax*2]             
  0x00210AE8  50                      push     eax                            
  0x00210AE9  ff15087b2100            call     dword ptr [0x217b08]           ; -> xbox_DbgPrint
  0x00210AEF  33c9                    xor      ecx, ecx                       
  0x00210AF1  85db                    test     ebx, ebx                       
  0x00210AF3  890de0a04000            mov      dword ptr [0x40a0e0], ecx      
  0x00210AF9  7618                    jbe      0x210b13                       
  0x00210AFB  8bd3                    mov      edx, ebx                       
                                        ; XREF: 0x00210B0B (cond_jump)
  0x00210AFD  8988a4000000            mov      dword ptr [eax + 0xa4], ecx    
  0x00210B03  8bc8                    mov      ecx, eax                       
  0x00210B05  05a8000000              add      eax, 0xa8                      
  0x00210B0A  4a                      dec      edx                            
  0x00210B0B  75f0                    jne      0x210afd                       
  0x00210B0D  890de0a04000            mov      dword ptr [0x40a0e0], ecx      
                                        ; XREF: 0x00210AF9 (cond_jump)
  0x00210B13  668325daa0400000        and      word ptr [0x40a0da], 0         
  0x00210B1B  33d2                    xor      edx, edx                       
  0x00210B1D  663915d8a04000          cmp      word ptr [0x40a0d8], dx        
  0x00210B24  a3dca04000              mov      dword ptr [0x40a0dc], eax      
  0x00210B29  761d                    jbe      0x210b48                       
  0x00210B2B  33c9                    xor      ecx, ecx                       
                                        ; XREF: 0x00210B46 (cond_jump)
  0x00210B2D  a1dca04000              mov      eax, dword ptr [0x40a0dc]      
  0x00210B32  8d440104                lea      eax, [ecx + eax + 4]           
  0x00210B36  8020fe                  and      byte ptr [eax], 0xfe           
  0x00210B39  0fb705d8a04000          movzx    eax, word ptr [0x40a0d8]       
  0x00210B40  42                      inc      edx                            
  0x00210B41  83c112                  add      ecx, 0x12                      
  0x00210B44  3bd0                    cmp      edx, eax                       
  0x00210B46  72e5                    jb       0x210b2d                       
                                        ; XREF: 0x00210B29 (cond_jump)
  0x00210B48  6860082100              push     0x210860                       
  0x00210B4D  8bce                    mov      ecx, esi                       
  0x00210B4F  881d61082100            mov      byte ptr [0x210861], bl        
  0x00210B55  e85c010000              call     0x210cb6                       ; -> sub_00210CB6
  0x00210B5A  6a00                    push     0                              
  0x00210B5C  6830a14000              push     0x40a130                       
  0x00210B61  ff15147b2100            call     dword ptr [0x217b14]           ; -> xbox_KeInitializeTimerEx
  0x00210B67  5e                      pop      esi                            
  0x00210B68  5b                      pop      ebx                            
                                        ; XREF: 0x00210A4A (cond_jump)
  0x00210B69  c20400                  ret      4                              
                                        ; XREF: 0x00211578 (jump)
  0x00210B6C  53                      push     ebx                            
  0x00210B6D  56                      push     esi                            
  0x00210B6E  57                      push     edi                            
  0x00210B6F  68b4000000              push     0xb4                           
  0x00210B74  ff15087b2100            call     dword ptr [0x217b08]           ; -> xbox_DbgPrint
  0x00210B7A  85c0                    test     eax, eax                       
  0x00210B7C  7413                    je       0x210b91                       
  0x00210B7E  ff742414                push     dword ptr [esp + 0x14]         
  0x00210B82  8bc8                    mov      ecx, eax                       
  0x00210B84  ff742414                push     dword ptr [esp + 0x14]         
  0x00210B88  e89e1c0000              call     0x21282b                       ; -> sub_0021282B
  0x00210B8D  8bf0                    mov      esi, eax                       
  0x00210B8F  eb02                    jmp      0x210b93                       
                                        ; XREF: 0x00210B7C (cond_jump)
  0x00210B91  33f6                    xor      esi, esi                       
                                        ; XREF: 0x00210B8F (jump)
  0x00210B93  b864072100              mov      eax, 0x210764                  
  0x00210B98  bf74072100              mov      edi, 0x210774                  
  0x00210B9D  3bc7                    cmp      eax, edi                       
  0x00210B9F  8bd8                    mov      ebx, eax                       
  0x00210BA1  7311                    jae      0x210bb4                       
                                        ; XREF: 0x00210BB2 (cond_jump)
  0x00210BA3  8b03                    mov      eax, dword ptr [ebx]           
  0x00210BA5  85c0                    test     eax, eax                       
  0x00210BA7  7404                    je       0x210bad                       
  0x00210BA9  56                      push     esi                            
  0x00210BAA  ff5004                  call     dword ptr [eax + 4]            
                                        ; XREF: 0x00210BA7 (cond_jump)
  0x00210BAD  83c304                  add      ebx, 4                         
  0x00210BB0  3bdf                    cmp      ebx, edi                       
  0x00210BB2  72ef                    jb       0x210ba3                       
                                        ; XREF: 0x00210BA1 (cond_jump)
  0x00210BB4  8bce                    mov      ecx, esi                       
  0x00210BB6  e8fd020000              call     0x210eb8                       ; -> sub_00210EB8
  0x00210BBB  8d86a4000000            lea      eax, [esi + 0xa4]              
  0x00210BC1  50                      push     eax                            
  0x00210BC2  e8b3040000              call     0x21107a                       ; -> sub_0021107A
  0x00210BC7  0fb686a1000000          movzx    eax, byte ptr [esi + 0xa1]     
  0x00210BCE  50                      push     eax                            
  0x00210BCF  0fb686a0000000          movzx    eax, byte ptr [esi + 0xa0]     
  0x00210BD6  50                      push     eax                            
  0x00210BD7  b980a14000              mov      ecx, 0x40a180                  
  0x00210BDC  e8fa030000              call     0x210fdb                       ; -> sub_00210FDB
  0x00210BE1  56                      push     esi                            
  0x00210BE2  ff153c7b2100            call     dword ptr [0x217b3c]           ; -> xbox_ExEventObjectType
  0x00210BE8  80257ca1400000          and      byte ptr [0x40a17c], 0         
  0x00210BEF  e892040000              call     0x211086                       ; -> sub_00211086
  0x00210BF4  5f                      pop      edi                            
  0x00210BF5  5e                      pop      esi                            
  0x00210BF6  5b                      pop      ebx                            
  0x00210BF7  c20800                  ret      8                              

; ============================================================
; Function: sub_00210BFA
; Start: 0x00210BFA  End: 0x00210C73  Size: 121 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002110C8, sub_00212885
; Called by: sub_00211086
; ============================================================
sub_00210BFA:
  0x00210BFA  56                      push     esi                            
  0x00210BFB  57                      push     edi                            
  0x00210BFC  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00210C00  83c718                  add      edi, 0x18                      
  0x00210C03  57                      push     edi                            
  0x00210C04  ff15087b2100            call     dword ptr [0x217b08]           ; -> xbox_DbgPrint
  0x00210C0A  8bf0                    mov      esi, eax                       
  0x00210C0C  85f6                    test     esi, esi                       
  0x00210C0E  745e                    je       0x210c6e                       
  0x00210C10  8bcf                    mov      ecx, edi                       
  0x00210C12  8bd1                    mov      edx, ecx                       
  0x00210C14  c1e902                  shr      ecx, 2                         
  0x00210C17  33c0                    xor      eax, eax                       
  0x00210C19  8bfe                    mov      edi, esi                       
  0x00210C1B  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00210C1D  8bca                    mov      ecx, edx                       
  0x00210C1F  83e103                  and      ecx, 3                         
  0x00210C22  f3aa                    rep stosb byte ptr es:[edi], al          
  0x00210C24  fe057ca14000            inc      byte ptr [0x40a17c]            
  0x00210C2A  0fb6057ca14000          movzx    eax, byte ptr [0x40a17c]       
  0x00210C31  b980a14000              mov      ecx, 0x40a180                  
  0x00210C36  8906                    mov      dword ptr [esi], eax           
  0x00210C38  e8481c0000              call     0x212885                       ; -> sub_00212885
  0x00210C3D  ff74240c                push     dword ptr [esp + 0xc]          
  0x00210C41  8d4e04                  lea      ecx, [esi + 4]                 
  0x00210C44  8901                    mov      dword ptr [ecx], eax           
  0x00210C46  802000                  and      byte ptr [eax], 0              
  0x00210C49  8b01                    mov      eax, dword ptr [ecx]           
  0x00210C4B  c6400280                mov      byte ptr [eax + 2], 0x80       
  0x00210C4F  8b01                    mov      eax, dword ptr [ecx]           
  0x00210C51  c6400180                mov      byte ptr [eax + 1], 0x80       
  0x00210C55  8b01                    mov      eax, dword ptr [ecx]           
  0x00210C57  c6400380                mov      byte ptr [eax + 3], 0x80       
  0x00210C5B  8b01                    mov      eax, dword ptr [ecx]           
  0x00210C5D  89700c                  mov      dword ptr [eax + 0xc], esi     
  0x00210C60  33c0                    xor      eax, eax                       
  0x00210C62  8a06                    mov      al, byte ptr [esi]             
  0x00210C64  83c618                  add      esi, 0x18                      
  0x00210C67  50                      push     eax                            
  0x00210C68  56                      push     esi                            
  0x00210C69  e85a040000              call     0x2110c8                       ; -> sub_002110C8
                                        ; XREF: 0x00210C0E (cond_jump)
  0x00210C6E  5f                      pop      edi                            
  0x00210C6F  5e                      pop      esi                            
  0x00210C70  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00210C73
; Start: 0x00210C73  End: 0x00210CA4  Size: 49 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00210C73:
  0x00210C73  8b919c000000            mov      edx, dword ptr [ecx + 0x9c]    
  0x00210C79  33c0                    xor      eax, eax                       
  0x00210C7B  85d2                    test     edx, edx                       
  0x00210C7D  7422                    je       0x210ca1                       
  0x00210C7F  8b8998000000            mov      ecx, dword ptr [ecx + 0x98]    
  0x00210C85  85c9                    test     ecx, ecx                       
  0x00210C87  56                      push     esi                            
  0x00210C88  57                      push     edi                            
  0x00210C89  7612                    jbe      0x210c9d                       
  0x00210C8B  8bf2                    mov      esi, edx                       
                                        ; XREF: 0x00210C9B (cond_jump)
  0x00210C8D  8b3e                    mov      edi, dword ptr [esi]           
  0x00210C8F  3b7c240c                cmp      edi, dword ptr [esp + 0xc]     
  0x00210C93  740f                    je       0x210ca4                       
  0x00210C95  40                      inc      eax                            
  0x00210C96  83c608                  add      esi, 8                         
  0x00210C99  3bc1                    cmp      eax, ecx                       
  0x00210C9B  72f0                    jb       0x210c8d                       
                                        ; XREF: 0x00210C89 (cond_jump)
  0x00210C9D  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00210CA8 (jump)
  0x00210C9F  5f                      pop      edi                            
  0x00210CA0  5e                      pop      esi                            
                                        ; XREF: 0x00210C7D (cond_jump)
  0x00210CA1  c20400                  ret      4                              
; end of function
                                        ; XREF: 0x00210C93 (cond_jump)
  0x00210CA4  8b44c204                mov      eax, dword ptr [edx + eax*8 + 4] 
  0x00210CA8  ebf5                    jmp      0x210c9f                       

; ============================================================
; Function: sub_00210CAA
; Start: 0x00210CAA  End: 0x00210CB6  Size: 12 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_0021092C
; ============================================================
sub_00210CAA:
  0x00210CAA  33c0                    xor      eax, eax                       
  0x00210CAC  39819c000000            cmp      dword ptr [ecx + 0x9c], eax    
  0x00210CB2  0f94c0                  sete     al                             
  0x00210CB5  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00210CB6
; Start: 0x00210CB6  End: 0x00210EB8  Size: 514 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00210CB6
; Called by: sub_00210CB6
; ============================================================
sub_00210CB6:
  0x00210CB6  55                      push     ebp                            
  0x00210CB7  8bec                    mov      ebp, esp                       
  0x00210CB9  83ec0c                  sub      esp, 0xc                       
  0x00210CBC  53                      push     ebx                            
  0x00210CBD  56                      push     esi                            
  0x00210CBE  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x00210CC1  0fb64601                movzx    eax, byte ptr [esi + 1]        
  0x00210CC5  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x00210CC8  0fb606                  movzx    eax, byte ptr [esi]            
  0x00210CCB  83e800                  sub      eax, 0                         
  0x00210CCE  8bd1                    mov      edx, ecx                       
  0x00210CD0  57                      push     edi                            
  0x00210CD1  8955f4                  mov      dword ptr [ebp - 0xc], edx     
  0x00210CD4  7440                    je       0x210d16                       
  0x00210CD6  48                      dec      eax                            
  0x00210CD7  7438                    je       0x210d11                       
  0x00210CD9  48                      dec      eax                            
  0x00210CDA  753e                    jne      0x210d1a                       
  0x00210CDC  8a4602                  mov      al, byte ptr [esi + 2]         
  0x00210CDF  3a4234                  cmp      al, byte ptr [edx + 0x34]      
  0x00210CE2  8d5a64                  lea      ebx, [edx + 0x64]              
  0x00210CE5  7603                    jbe      0x210cea                       
  0x00210CE7  884234                  mov      byte ptr [edx + 0x34], al      
                                        ; XREF: 0x00210CE5 (cond_jump)
  0x00210CEA  8a4601                  mov      al, byte ptr [esi + 1]         
  0x00210CED  3c04                    cmp      al, 4                          
  0x00210CEF  762c                    jbe      0x210d1d                       
  0x00210CF1  2c04                    sub      al, 4                          
  0x00210CF3  56                      push     esi                            
  0x00210CF4  8bca                    mov      ecx, edx                       
  0x00210CF6  884601                  mov      byte ptr [esi + 1], al         
  0x00210CF9  c60601                  mov      byte ptr [esi], 1              
  0x00210CFC  e8b5ffffff              call     0x210cb6                       ; -> sub_00210CB6
  0x00210D01  80460104                add      byte ptr [esi + 1], 4          
  0x00210D05  836df804                sub      dword ptr [ebp - 8], 4         
  0x00210D09  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
  0x00210D0C  c60602                  mov      byte ptr [esi], 2              
  0x00210D0F  eb0c                    jmp      0x210d1d                       
                                        ; XREF: 0x00210CD7 (cond_jump)
  0x00210D11  8d5a32                  lea      ebx, [edx + 0x32]              
  0x00210D14  eb07                    jmp      0x210d1d                       
                                        ; XREF: 0x00210CD4 (cond_jump)
  0x00210D16  8bda                    mov      ebx, edx                       
  0x00210D18  eb03                    jmp      0x210d1d                       
                                        ; XREF: 0x00210CDA (cond_jump)
  0x00210D1A  8b5d08                  mov      ebx, dword ptr [ebp + 8]       
                                        ; XREF: 0x00210CEF (cond_jump), 0x00210D0F (jump), 0x00210D14 (jump), 0x00210D18 (jump)
  0x00210D1D  8a4602                  mov      al, byte ptr [esi + 2]         
  0x00210D20  3a4302                  cmp      al, byte ptr [ebx + 2]         
  0x00210D23  7603                    jbe      0x210d28                       
  0x00210D25  884302                  mov      byte ptr [ebx + 2], al         
                                        ; XREF: 0x00210D23 (cond_jump)
  0x00210D28  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00210D2B  83650800                and      dword ptr [ebp + 8], 0         
  0x00210D2F  85c0                    test     eax, eax                       
  0x00210D31  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00210D34  7449                    je       0x210d7f                       
  0x00210D36  8d7b03                  lea      edi, [ebx + 3]                 
                                        ; XREF: 0x00210D7D (cond_jump)
  0x00210D39  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210D3D  7340                    jae      0x210d7f                       
  0x00210D3F  8a07                    mov      al, byte ptr [edi]             
  0x00210D41  3a4603                  cmp      al, byte ptr [esi + 3]         
  0x00210D44  7608                    jbe      0x210d4e                       
  0x00210D46  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210D49  83c70a                  add      edi, 0xa                       
  0x00210D4C  eb2b                    jmp      0x210d79                       
                                        ; XREF: 0x00210D44 (cond_jump)
  0x00210D4E  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210D52  7317                    jae      0x210d6b                       
  0x00210D54  6a04                    push     4                              
  0x00210D56  58                      pop      eax                            
  0x00210D57  2b4508                  sub      eax, dword ptr [ebp + 8]       
  0x00210D5A  8d4b2b                  lea      ecx, [ebx + 0x2b]              
                                        ; XREF: 0x00210D66 (cond_jump)
  0x00210D5D  8a51f6                  mov      dl, byte ptr [ecx - 0xa]       
  0x00210D60  8811                    mov      byte ptr [ecx], dl             
  0x00210D62  83e90a                  sub      ecx, 0xa                       
  0x00210D65  48                      dec      eax                            
  0x00210D66  75f5                    jne      0x210d5d                       
  0x00210D68  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
                                        ; XREF: 0x00210D52 (cond_jump)
  0x00210D6B  8a4603                  mov      al, byte ptr [esi + 3]         
  0x00210D6E  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210D71  8807                    mov      byte ptr [edi], al             
  0x00210D73  83c70a                  add      edi, 0xa                       
  0x00210D76  ff4dfc                  dec      dword ptr [ebp - 4]            
                                        ; XREF: 0x00210D4C (jump)
  0x00210D79  837dfc00                cmp      dword ptr [ebp - 4], 0         
  0x00210D7D  75ba                    jne      0x210d39                       
                                        ; XREF: 0x00210D34 (cond_jump), 0x00210D3D (cond_jump)
  0x00210D7F  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00210D82  83650800                and      dword ptr [ebp + 8], 0         
  0x00210D86  85c0                    test     eax, eax                       
  0x00210D88  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00210D8B  7449                    je       0x210dd6                       
  0x00210D8D  8d7b04                  lea      edi, [ebx + 4]                 
                                        ; XREF: 0x00210DD4 (cond_jump)
  0x00210D90  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210D94  7340                    jae      0x210dd6                       
  0x00210D96  8a07                    mov      al, byte ptr [edi]             
  0x00210D98  3a4604                  cmp      al, byte ptr [esi + 4]         
  0x00210D9B  7608                    jbe      0x210da5                       
  0x00210D9D  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210DA0  83c70a                  add      edi, 0xa                       
  0x00210DA3  eb2b                    jmp      0x210dd0                       
                                        ; XREF: 0x00210D9B (cond_jump)
  0x00210DA5  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210DA9  7317                    jae      0x210dc2                       
  0x00210DAB  6a04                    push     4                              
  0x00210DAD  58                      pop      eax                            
  0x00210DAE  2b4508                  sub      eax, dword ptr [ebp + 8]       
  0x00210DB1  8d4b2c                  lea      ecx, [ebx + 0x2c]              
                                        ; XREF: 0x00210DBD (cond_jump)
  0x00210DB4  8a51f6                  mov      dl, byte ptr [ecx - 0xa]       
  0x00210DB7  8811                    mov      byte ptr [ecx], dl             
  0x00210DB9  83e90a                  sub      ecx, 0xa                       
  0x00210DBC  48                      dec      eax                            
  0x00210DBD  75f5                    jne      0x210db4                       
  0x00210DBF  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
                                        ; XREF: 0x00210DA9 (cond_jump)
  0x00210DC2  8a4604                  mov      al, byte ptr [esi + 4]         
  0x00210DC5  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210DC8  8807                    mov      byte ptr [edi], al             
  0x00210DCA  83c70a                  add      edi, 0xa                       
  0x00210DCD  ff4dfc                  dec      dword ptr [ebp - 4]            
                                        ; XREF: 0x00210DA3 (jump)
  0x00210DD0  837dfc00                cmp      dword ptr [ebp - 4], 0         
  0x00210DD4  75ba                    jne      0x210d90                       
                                        ; XREF: 0x00210D8B (cond_jump), 0x00210D94 (cond_jump)
  0x00210DD6  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00210DD9  83650800                and      dword ptr [ebp + 8], 0         
  0x00210DDD  85c0                    test     eax, eax                       
  0x00210DDF  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00210DE2  7449                    je       0x210e2d                       
  0x00210DE4  8d7b05                  lea      edi, [ebx + 5]                 
                                        ; XREF: 0x00210E2B (cond_jump)
  0x00210DE7  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210DEB  7340                    jae      0x210e2d                       
  0x00210DED  8a07                    mov      al, byte ptr [edi]             
  0x00210DEF  3a4605                  cmp      al, byte ptr [esi + 5]         
  0x00210DF2  7608                    jbe      0x210dfc                       
  0x00210DF4  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210DF7  83c70a                  add      edi, 0xa                       
  0x00210DFA  eb2b                    jmp      0x210e27                       
                                        ; XREF: 0x00210DF2 (cond_jump)
  0x00210DFC  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210E00  7317                    jae      0x210e19                       
  0x00210E02  6a04                    push     4                              
  0x00210E04  58                      pop      eax                            
  0x00210E05  2b4508                  sub      eax, dword ptr [ebp + 8]       
  0x00210E08  8d4b2d                  lea      ecx, [ebx + 0x2d]              
                                        ; XREF: 0x00210E14 (cond_jump)
  0x00210E0B  8a51f6                  mov      dl, byte ptr [ecx - 0xa]       
  0x00210E0E  8811                    mov      byte ptr [ecx], dl             
  0x00210E10  83e90a                  sub      ecx, 0xa                       
  0x00210E13  48                      dec      eax                            
  0x00210E14  75f5                    jne      0x210e0b                       
  0x00210E16  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
                                        ; XREF: 0x00210E00 (cond_jump)
  0x00210E19  8a4605                  mov      al, byte ptr [esi + 5]         
  0x00210E1C  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210E1F  8807                    mov      byte ptr [edi], al             
  0x00210E21  83c70a                  add      edi, 0xa                       
  0x00210E24  ff4dfc                  dec      dword ptr [ebp - 4]            
                                        ; XREF: 0x00210DFA (jump)
  0x00210E27  837dfc00                cmp      dword ptr [ebp - 4], 0         
  0x00210E2B  75ba                    jne      0x210de7                       
                                        ; XREF: 0x00210DE2 (cond_jump), 0x00210DEB (cond_jump)
  0x00210E2D  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00210E30  83650800                and      dword ptr [ebp + 8], 0         
  0x00210E34  85c0                    test     eax, eax                       
  0x00210E36  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00210E39  7449                    je       0x210e84                       
  0x00210E3B  8d7b08                  lea      edi, [ebx + 8]                 
                                        ; XREF: 0x00210E82 (cond_jump)
  0x00210E3E  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210E42  7340                    jae      0x210e84                       
  0x00210E44  8a07                    mov      al, byte ptr [edi]             
  0x00210E46  3a4608                  cmp      al, byte ptr [esi + 8]         
  0x00210E49  7608                    jbe      0x210e53                       
  0x00210E4B  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210E4E  83c70a                  add      edi, 0xa                       
  0x00210E51  eb2b                    jmp      0x210e7e                       
                                        ; XREF: 0x00210E49 (cond_jump)
  0x00210E53  837d0804                cmp      dword ptr [ebp + 8], 4         
  0x00210E57  7317                    jae      0x210e70                       
  0x00210E59  6a04                    push     4                              
  0x00210E5B  58                      pop      eax                            
  0x00210E5C  2b4508                  sub      eax, dword ptr [ebp + 8]       
  0x00210E5F  8d4b30                  lea      ecx, [ebx + 0x30]              
                                        ; XREF: 0x00210E6B (cond_jump)
  0x00210E62  8a51f6                  mov      dl, byte ptr [ecx - 0xa]       
  0x00210E65  8811                    mov      byte ptr [ecx], dl             
  0x00210E67  83e90a                  sub      ecx, 0xa                       
  0x00210E6A  48                      dec      eax                            
  0x00210E6B  75f5                    jne      0x210e62                       
  0x00210E6D  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
                                        ; XREF: 0x00210E57 (cond_jump)
  0x00210E70  8a4608                  mov      al, byte ptr [esi + 8]         
  0x00210E73  ff4508                  inc      dword ptr [ebp + 8]            
  0x00210E76  8807                    mov      byte ptr [edi], al             
  0x00210E78  83c70a                  add      edi, 0xa                       
  0x00210E7B  ff4dfc                  dec      dword ptr [ebp - 4]            
                                        ; XREF: 0x00210E51 (jump)
  0x00210E7E  837dfc00                cmp      dword ptr [ebp - 4], 0         
  0x00210E82  75ba                    jne      0x210e3e                       
                                        ; XREF: 0x00210E39 (cond_jump), 0x00210E42 (cond_jump)
  0x00210E84  8a4603                  mov      al, byte ptr [esi + 3]         
  0x00210E87  8d8ab0000000            lea      ecx, [edx + 0xb0]              
  0x00210E8D  3a01                    cmp      al, byte ptr [ecx]             
  0x00210E8F  7602                    jbe      0x210e93                       
  0x00210E91  8801                    mov      byte ptr [ecx], al             
                                        ; XREF: 0x00210E8F (cond_jump)
  0x00210E93  8a4607                  mov      al, byte ptr [esi + 7]         
  0x00210E96  8d8ab1000000            lea      ecx, [edx + 0xb1]              
  0x00210E9C  3a01                    cmp      al, byte ptr [ecx]             
  0x00210E9E  7602                    jbe      0x210ea2                       
  0x00210EA0  8801                    mov      byte ptr [ecx], al             
                                        ; XREF: 0x00210E9E (cond_jump)
  0x00210EA2  8a4609                  mov      al, byte ptr [esi + 9]         
  0x00210EA5  5f                      pop      edi                            
  0x00210EA6  8d8ab2000000            lea      ecx, [edx + 0xb2]              
  0x00210EAC  3a01                    cmp      al, byte ptr [ecx]             
  0x00210EAE  5e                      pop      esi                            
  0x00210EAF  5b                      pop      ebx                            
  0x00210EB0  7602                    jbe      0x210eb4                       
  0x00210EB2  8801                    mov      byte ptr [ecx], al             
                                        ; XREF: 0x00210EB0 (cond_jump)
  0x00210EB4  c9                      leave                                   
  0x00210EB5  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00210EB8
; Start: 0x00210EB8  End: 0x00210FDB  Size: 291 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00210EB8:
  0x00210EB8  55                      push     ebp                            
  0x00210EB9  8bec                    mov      ebp, esp                       
  0x00210EBB  83ec0c                  sub      esp, 0xc                       
  0x00210EBE  8a4102                  mov      al, byte ptr [ecx + 2]         
  0x00210EC1  8a5134                  mov      dl, byte ptr [ecx + 0x34]      
  0x00210EC4  02d0                    add      dl, al                         
  0x00210EC6  025166                  add      dl, byte ptr [ecx + 0x66]      
  0x00210EC9  53                      push     ebx                            
  0x00210ECA  c0e202                  shl      dl, 2                          
  0x00210ECD  80c213                  add      dl, 0x13                       
  0x00210ED0  56                      push     esi                            
  0x00210ED1  33f6                    xor      esi, esi                       
  0x00210ED3  8891a0000000            mov      byte ptr [ecx + 0xa0], dl      
  0x00210ED9  3a81a1000000            cmp      al, byte ptr [ecx + 0xa1]      
  0x00210EDF  57                      push     edi                            
  0x00210EE0  8975fc                  mov      dword ptr [ebp - 4], esi       
  0x00210EE3  8975f8                  mov      dword ptr [ebp - 8], esi       
  0x00210EE6  7606                    jbe      0x210eee                       
  0x00210EE8  8881a1000000            mov      byte ptr [ecx + 0xa1], al      
                                        ; XREF: 0x00210EE6 (cond_jump)
  0x00210EEE  8a4166                  mov      al, byte ptr [ecx + 0x66]      
  0x00210EF1  3a81a1000000            cmp      al, byte ptr [ecx + 0xa1]      
  0x00210EF7  7606                    jbe      0x210eff                       
  0x00210EF9  8881a1000000            mov      byte ptr [ecx + 0xa1], al      
                                        ; XREF: 0x00210EF7 (cond_jump)
  0x00210EFF  8a4134                  mov      al, byte ptr [ecx + 0x34]      
  0x00210F02  3a81a1000000            cmp      al, byte ptr [ecx + 0xa1]      
  0x00210F08  7606                    jbe      0x210f10                       
  0x00210F0A  8881a1000000            mov      byte ptr [ecx + 0xa1], al      
                                        ; XREF: 0x00210F08 (cond_jump)
  0x00210F10  8a81a1000000            mov      al, byte ptr [ecx + 0xa1]      
  0x00210F16  0081a0000000            add      byte ptr [ecx + 0xa0], al      
  0x00210F1C  8d4137                  lea      eax, [ecx + 0x37]              
  0x00210F1F  c745f404000000          mov      dword ptr [ebp - 0xc], 4       
                                        ; XREF: 0x00210F92 (cond_jump)
  0x00210F26  0fb67832                movzx    edi, byte ptr [eax + 0x32]     
  0x00210F2A  0fb650ce                movzx    edx, byte ptr [eax - 0x32]     
  0x00210F2E  03d7                    add      edx, edi                       
  0x00210F30  0fb638                  movzx    edi, byte ptr [eax]            
  0x00210F33  03fe                    add      edi, esi                       
  0x00210F35  8d3417                  lea      esi, [edi + edx]               
  0x00210F38  0fb67830                movzx    edi, byte ptr [eax + 0x30]     
  0x00210F3C  0fb650cc                movzx    edx, byte ptr [eax - 0x34]     
  0x00210F40  03d7                    add      edx, edi                       
  0x00210F42  0fb678fe                movzx    edi, byte ptr [eax - 2]        
  0x00210F46  037dfc                  add      edi, dword ptr [ebp - 4]       
  0x00210F49  83c00a                  add      eax, 0xa                       
  0x00210F4C  03fa                    add      edi, edx                       
  0x00210F4E  0fb650c3                movzx    edx, byte ptr [eax - 0x3d]     
  0x00210F52  897dfc                  mov      dword ptr [ebp - 4], edi       
  0x00210F55  0fb67827                movzx    edi, byte ptr [eax + 0x27]     
  0x00210F59  03d7                    add      edx, edi                       
  0x00210F5B  0fb678f5                movzx    edi, byte ptr [eax - 0xb]      
  0x00210F5F  037df8                  add      edi, dword ptr [ebp - 8]       
  0x00210F62  03fa                    add      edi, edx                       
  0x00210F64  0fb650c7                movzx    edx, byte ptr [eax - 0x39]     
  0x00210F68  0191a8000000            add      dword ptr [ecx + 0xa8], edx    
  0x00210F6E  0fb6582b                movzx    ebx, byte ptr [eax + 0x2b]     
  0x00210F72  8b91a8000000            mov      edx, dword ptr [ecx + 0xa8]    
  0x00210F78  03d3                    add      edx, ebx                       
  0x00210F7A  8991a8000000            mov      dword ptr [ecx + 0xa8], edx    
  0x00210F80  0fb658f9                movzx    ebx, byte ptr [eax - 7]        
  0x00210F84  03da                    add      ebx, edx                       
  0x00210F86  ff4df4                  dec      dword ptr [ebp - 0xc]          
  0x00210F89  897df8                  mov      dword ptr [ebp - 8], edi       
  0x00210F8C  8999a8000000            mov      dword ptr [ecx + 0xa8], ebx    
  0x00210F92  7592                    jne      0x210f26                       
  0x00210F94  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00210F97  83c605                  add      esi, 5                         
  0x00210F9A  83c006                  add      eax, 6                         
  0x00210F9D  80b9b00000000d          cmp      byte ptr [ecx + 0xb0], 0xd     
  0x00210FA4  7307                    jae      0x210fad                       
  0x00210FA6  c681b00000000d          mov      byte ptr [ecx + 0xb0], 0xd     
                                        ; XREF: 0x00210FA4 (cond_jump)
  0x00210FAD  0fb691b1000000          movzx    edx, byte ptr [ecx + 0xb1]     
  0x00210FB4  03c7                    add      eax, edi                       
  0x00210FB6  8d3c70                  lea      edi, [eax + esi*2]             
  0x00210FB9  8bde                    mov      ebx, esi                       
  0x00210FBB  03df                    add      ebx, edi                       
  0x00210FBD  0fb6b9b0000000          movzx    edi, byte ptr [ecx + 0xb0]     
  0x00210FC4  03da                    add      ebx, edx                       
  0x00210FC6  03fb                    add      edi, ebx                       
  0x00210FC8  89b9ac000000            mov      dword ptr [ecx + 0xac], edi    
  0x00210FCE  5f                      pop      edi                            
  0x00210FCF  03c6                    add      eax, esi                       
  0x00210FD1  5e                      pop      esi                            
  0x00210FD2  8981a4000000            mov      dword ptr [ecx + 0xa4], eax    
  0x00210FD8  5b                      pop      ebx                            
  0x00210FD9  c9                      leave                                   
  0x00210FDA  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00210FDB
; Start: 0x00210FDB  End: 0x00211022  Size: 71 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00020480
; ============================================================
sub_00210FDB:
  0x00210FDB  55                      push     ebp                            
  0x00210FDC  8bec                    mov      ebp, esp                       
  0x00210FDE  8a4508                  mov      al, byte ptr [ebp + 8]         
  0x00210FE1  53                      push     ebx                            
  0x00210FE2  56                      push     esi                            
  0x00210FE3  8bf1                    mov      esi, ecx                       
  0x00210FE5  57                      push     edi                            
  0x00210FE6  0fb6f8                  movzx    edi, al                        
  0x00210FE9  88467a                  mov      byte ptr [esi + 0x7a], al      
  0x00210FEC  33db                    xor      ebx, ebx                       
  0x00210FEE  8bc7                    mov      eax, edi                       
  0x00210FF0  c1e005                  shl      eax, 5                         
  0x00210FF3  50                      push     eax                            
  0x00210FF4  881e                    mov      byte ptr [esi], bl             
  0x00210FF6  885e79                  mov      byte ptr [esi + 0x79], bl      
  0x00210FF9  895e7c                  mov      dword ptr [esi + 0x7c], ebx    
  0x00210FFC  899e80000000            mov      dword ptr [esi + 0x80], ebx    
  0x00211002  ff15087b2100            call     dword ptr [0x217b08]           ; -> xbox_DbgPrint
  0x00211008  3bc3                    cmp      eax, ebx                       
  0x0021100A  894508                  mov      dword ptr [ebp + 8], eax       
  0x0021100D  7413                    je       0x211022                       
  0x0021100F  68f5342100              push     0x2134f5                       
  0x00211014  57                      push     edi                            
  0x00211015  6a20                    push     0x20                           
  0x00211017  50                      push     eax                            
  0x00211018  e863f4e0ff              call     0x20480                        ; -> sub_00020480
  0x0021101D  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x00211020  eb02                    jmp      0x211024                       
; end of function
                                        ; XREF: 0x0021100D (cond_jump)
  0x00211022  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00211020 (jump)
  0x00211024  0fb64e7a                movzx    ecx, byte ptr [esi + 0x7a]     
  0x00211028  8986e0000000            mov      dword ptr [esi + 0xe0], eax    
  0x0021102E  8a450c                  mov      al, byte ptr [ebp + 0xc]       
  0x00211031  88467b                  mov      byte ptr [esi + 0x7b], al      
  0x00211034  32c0                    xor      al, al                         
  0x00211036  49                      dec      ecx                            
  0x00211037  85c9                    test     ecx, ecx                       
  0x00211039  7e1d                    jle      0x211058                       
  0x0021103B  33c9                    xor      ecx, ecx                       
                                        ; XREF: 0x00211056 (cond_jump)
  0x0021103D  8b96e0000000            mov      edx, dword ptr [esi + 0xe0]    
  0x00211043  fec0                    inc      al                             
  0x00211045  c1e105                  shl      ecx, 5                         
  0x00211048  88441101                mov      byte ptr [ecx + edx + 1], al   
  0x0021104C  0fb6567a                movzx    edx, byte ptr [esi + 0x7a]     
  0x00211050  0fbec8                  movsx    ecx, al                        
  0x00211053  4a                      dec      edx                            
  0x00211054  3bca                    cmp      ecx, edx                       
  0x00211056  7ce5                    jl       0x21103d                       
                                        ; XREF: 0x00211039 (cond_jump)
  0x00211058  53                      push     ebx                            
  0x00211059  6802422100              push     0x214202                       
  0x0021105E  8d4634                  lea      eax, [esi + 0x34]              
  0x00211061  50                      push     eax                            
  0x00211062  ff15187b2100            call     dword ptr [0x217b18]           ; -> xbox_KeInitializeDpc
  0x00211068  53                      push     ebx                            
  0x00211069  83c650                  add      esi, 0x50                      
  0x0021106C  56                      push     esi                            
  0x0021106D  ff15147b2100            call     dword ptr [0x217b14]           ; -> xbox_KeInitializeTimerEx
  0x00211073  5f                      pop      edi                            
  0x00211074  5e                      pop      esi                            
  0x00211075  5b                      pop      ebx                            
  0x00211076  5d                      pop      ebp                            
  0x00211077  c20800                  ret      8                              

; ============================================================
; Function: sub_0021107A
; Start: 0x0021107A  End: 0x00211086  Size: 12 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002112C3
; ============================================================
sub_0021107A:
  0x0021107A  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x0021107E  e840020000              call     0x2112c3                       ; -> sub_002112C3
  0x00211083  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00211086
; Start: 0x00211086  End: 0x002110C8  Size: 66 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00210BFA
; ============================================================
sub_00211086:
  0x00211086  55                      push     ebp                            
  0x00211087  8bec                    mov      ebp, esp                       
  0x00211089  83ec28                  sub      esp, 0x28                      
  0x0021108C  a1587b2100              mov      eax, dword ptr [0x217b58]      
  0x00211091  807805a1                cmp      byte ptr [eax + 5], 0xa1       
  0x00211095  742f                    je       0x2110c6                       
  0x00211097  8d45fc                  lea      eax, [ebp - 4]                 
  0x0021109A  50                      push     eax                            
  0x0021109B  6a01                    push     1                              
  0x0021109D  c645e803                mov      byte ptr [ebp - 0x18], 3       
  0x002110A1  c745f000100000          mov      dword ptr [ebp - 0x10], 0x1000 
  0x002110A8  c745ec0000d0fe          mov      dword ptr [ebp - 0x14], 0xfed00000 
  0x002110AF  ff156c7b2100            call     dword ptr [0x217b6c]           ; -> xbox_HalGetInterruptVector
  0x002110B5  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x002110B8  68d0040000              push     0x4d0                          
  0x002110BD  8d45d8                  lea      eax, [ebp - 0x28]              
  0x002110C0  50                      push     eax                            
  0x002110C1  e834fbffff              call     0x210bfa                       ; -> sub_00210BFA
                                        ; XREF: 0x00211095 (cond_jump)
  0x002110C6  c9                      leave                                   
  0x002110C7  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002110C8
; Start: 0x002110C8  End: 0x002111F2  Size: 298 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00211213, sub_00211488, sub_002114E9, sub_002114F4
; Called by: sub_00210BFA
; ============================================================
sub_002110C8:
  0x002110C8  55                      push     ebp                            
  0x002110C9  8bec                    mov      ebp, esp                       
  0x002110CB  fe4d0c                  dec      byte ptr [ebp + 0xc]           
  0x002110CE  8b4510                  mov      eax, dword ptr [ebp + 0x10]    
  0x002110D1  53                      push     ebx                            
  0x002110D2  0fb65d0c                movzx    ebx, byte ptr [ebp + 0xc]      
  0x002110D6  56                      push     esi                            
  0x002110D7  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x002110DA  899e5c040000            mov      dword ptr [esi + 0x45c], ebx   
  0x002110E0  8b4818                  mov      ecx, dword ptr [eax + 0x18]    
  0x002110E3  57                      push     edi                            
  0x002110E4  894e04                  mov      dword ptr [esi + 4], ecx       
  0x002110E7  8b4014                  mov      eax, dword ptr [eax + 0x14]    
  0x002110EA  56                      push     esi                            
  0x002110EB  8906                    mov      dword ptr [esi], eax           
  0x002110ED  e8f7030000              call     0x2114e9                       ; -> sub_002114E9
  0x002110F2  56                      push     esi                            
  0x002110F3  e8fc030000              call     0x2114f4                       ; -> sub_002114F4
  0x002110F8  8b3e                    mov      edi, dword ptr [esi]           
  0x002110FA  56                      push     esi                            
  0x002110FB  68786c2100              push     0x216c78                       
  0x00211100  8d8640040000            lea      eax, [esi + 0x440]             
  0x00211106  c7474800120000          mov      dword ptr [edi + 0x48], 0x1200 
  0x0021110D  c7474c00000000          mov      dword ptr [edi + 0x4c], 0      
  0x00211114  c7475000000080          mov      dword ptr [edi + 0x50], 0x80000000 
  0x0021111B  50                      push     eax                            
  0x0021111C  ff15187b2100            call     dword ptr [0x217b18]           ; -> xbox_KeInitializeDpc
  0x00211122  8b049da4b9c600          mov      eax, dword ptr [ebx*4 + 0xc6b9a4] 
  0x00211129  894608                  mov      dword ptr [esi + 8], eax       
  0x0021112C  8b4708                  mov      eax, dword ptr [edi + 8]       
  0x0021112F  894508                  mov      dword ptr [ebp + 8], eax       
  0x00211132  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00211138  88450f                  mov      byte ptr [ebp + 0xf], al       
  0x0021113B  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x0021113E  83c801                  or       eax, 1                         
  0x00211141  894708                  mov      dword ptr [edi + 8], eax       
  0x00211144  6a0a                    push     0xa                            
  0x00211146  ff15807b2100            call     dword ptr [0x217b80]           ; -> xbox_KeStallExecutionProcessor
  0x0021114C  8bce                    mov      ecx, esi                       
  0x0021114E  e835030000              call     0x211488                       ; -> sub_00211488
  0x00211153  c74704be000000          mov      dword ptr [edi + 4], 0xbe      
  0x0021115A  8b4734                  mov      eax, dword ptr [edi + 0x34]    
  0x0021115D  25dfee78a7              and      eax, 0xa778eedf                
  0x00211162  0ddf2e7827              or       eax, 0x27782edf                
  0x00211167  8bc8                    mov      ecx, eax                       
  0x00211169  f7d1                    not      ecx                            
  0x0021116B  33c8                    xor      ecx, eax                       
  0x0021116D  81e1ffffff7f            and      ecx, 0x7fffffff                
  0x00211173  f7d0                    not      eax                            
  0x00211175  33c8                    xor      ecx, eax                       
  0x00211177  894f34                  mov      dword ptr [edi + 0x34], ecx    
  0x0021117A  8a4d0f                  mov      cl, byte ptr [ebp + 0xf]       
  0x0021117D  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00211183  6bdb70                  imul     ebx, ebx, 0x70                 
  0x00211186  8b4510                  mov      eax, dword ptr [ebp + 0x10]    
  0x00211189  33c9                    xor      ecx, ecx                       
  0x0021118B  8a4824                  mov      cl, byte ptr [eax + 0x24]      
  0x0021118E  6a01                    push     1                              
  0x00211190  6a00                    push     0                              
  0x00211192  8d9be0b9c600            lea      ebx, [ebx + 0xc6b9e0]          
  0x00211198  51                      push     ecx                            
  0x00211199  ff701c                  push     dword ptr [eax + 0x1c]         
  0x0021119C  56                      push     esi                            
  0x0021119D  68b7662100              push     0x2166b7                       
  0x002111A2  53                      push     ebx                            
  0x002111A3  ff157c7b2100            call     dword ptr [0x217b7c]           ; -> xbox_KeInitializeInterrupt
  0x002111A9  53                      push     ebx                            
  0x002111AA  ff15787b2100            call     dword ptr [0x217b78]           ; -> xbox_KeBugCheckEx
  0x002111B0  33d2                    xor      edx, edx                       
  0x002111B2  42                      inc      edx                            
  0x002111B3  8d8ec0040000            lea      ecx, [esi + 0x4c0]             
  0x002111B9  8d86c8040000            lea      eax, [esi + 0x4c8]             
  0x002111BF  52                      push     edx                            
  0x002111C0  51                      push     ecx                            
  0x002111C1  c701c5512100            mov      dword ptr [ecx], 0x2151c5      
  0x002111C7  8996c4040000            mov      dword ptr [esi + 0x4c4], edx   
  0x002111CD  8986cc040000            mov      dword ptr [esi + 0x4cc], eax   
  0x002111D3  8900                    mov      dword ptr [eax], eax           
  0x002111D5  ff15747b2100            call     dword ptr [0x217b74]           ; -> xbox_HalReadWritePCISpace
  0x002111DB  8bce                    mov      ecx, esi                       
  0x002111DD  c7471033000080          mov      dword ptr [edi + 0x10], 0x80000033 
  0x002111E4  e82a000000              call     0x211213                       ; -> sub_00211213
  0x002111E9  33c0                    xor      eax, eax                       
  0x002111EB  5f                      pop      edi                            
  0x002111EC  5e                      pop      esi                            
  0x002111ED  5b                      pop      ebx                            
  0x002111EE  5d                      pop      ebp                            
  0x002111EF  c20c00                  ret      0xc                            
; end of function
  0x002111F2  6683251aa3400000        and      word ptr [0x40a31a], 0         
  0x002111FA  6a00                    push     0                              
  0x002111FC  68d0a24000              push     0x40a2d0                       
  0x00211201  66c70518a340000600      mov      word ptr [0x40a318], 6         
  0x0021120A  ff15147b2100            call     dword ptr [0x217b14]           ; -> xbox_KeInitializeTimerEx
  0x00211210  c20400                  ret      4                              

; ============================================================
; Function: sub_00211213
; Start: 0x00211213  End: 0x002112C3  Size: 176 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00216048
; Called by: sub_002110C8
; ============================================================
sub_00211213:
  0x00211213  55                      push     ebp                            
  0x00211214  8bec                    mov      ebp, esp                       
  0x00211216  51                      push     ecx                            
  0x00211217  51                      push     ecx                            
  0x00211218  53                      push     ebx                            
  0x00211219  56                      push     esi                            
  0x0021121A  57                      push     edi                            
  0x0021121B  8bf1                    mov      esi, ecx                       
  0x0021121D  8b1e                    mov      ebx, dword ptr [esi]           
  0x0021121F  6a00                    push     0                              
  0x00211221  8d8678040000            lea      eax, [esi + 0x478]             
  0x00211227  50                      push     eax                            
  0x00211228  ff15147b2100            call     dword ptr [0x217b14]           ; -> xbox_KeInitializeTimerEx
  0x0021122E  56                      push     esi                            
  0x0021122F  6841612100              push     0x216141                       
  0x00211234  8d86a0040000            lea      eax, [esi + 0x4a0]             
  0x0021123A  50                      push     eax                            
  0x0021123B  ff15187b2100            call     dword ptr [0x217b18]           ; -> xbox_KeInitializeDpc
  0x00211241  c6866004000004          mov      byte ptr [esi + 0x460], 4      
  0x00211248  8b4350                  mov      eax, dword ptr [ebx + 0x50]    
  0x0021124B  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x0021124E  668365f800              and      word ptr [ebp - 8], 0          
  0x00211253  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00211256  33c9                    xor      ecx, ecx                       
  0x00211258  894350                  mov      dword ptr [ebx + 0x50], eax    
  0x0021125B  33c0                    xor      eax, eax                       
  0x0021125D  33d2                    xor      edx, edx                       
  0x0021125F  8d7dfc                  lea      edi, [ebp - 4]                 
  0x00211262  41                      inc      ecx                            
  0x00211263  389660040000            cmp      byte ptr [esi + 0x460], dl     
  0x00211269  ab                      stosd    dword ptr es:[edi], eax        
  0x0021126A  7631                    jbe      0x21129d                       
  0x0021126C  8d4354                  lea      eax, [ebx + 0x54]              
                                        ; XREF: 0x0021129B (cond_jump)
  0x0021126F  8b38                    mov      edi, dword ptr [eax]           
  0x00211271  897df8                  mov      dword ptr [ebp - 8], edi       
  0x00211274  f645f801                test     byte ptr [ebp - 8], 1          
  0x00211278  7408                    je       0x211282                       
  0x0021127A  66094dfe                or       word ptr [ebp - 2], cx         
  0x0021127E  66094dfc                or       word ptr [ebp - 4], cx         
                                        ; XREF: 0x00211278 (cond_jump)
  0x00211282  668365f800              and      word ptr [ebp - 8], 0          
  0x00211287  8b7df8                  mov      edi, dword ptr [ebp - 8]       
  0x0021128A  8938                    mov      dword ptr [eax], edi           
  0x0021128C  0fb6be60040000          movzx    edi, byte ptr [esi + 0x460]    
  0x00211293  42                      inc      edx                            
  0x00211294  83c004                  add      eax, 4                         
  0x00211297  d1e1                    shl      ecx, 1                         
  0x00211299  3bd7                    cmp      edx, edi                       
  0x0021129B  72d2                    jb       0x21126f                       
                                        ; XREF: 0x0021126A (cond_jump)
  0x0021129D  c7431040000000          mov      dword ptr [ebx + 0x10], 0x40   
  0x002112A4  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x002112AA  8d55fc                  lea      edx, [ebp - 4]                 
  0x002112AD  8bce                    mov      ecx, esi                       
  0x002112AF  8ad8                    mov      bl, al                         
  0x002112B1  e8924d0000              call     0x216048                       ; -> sub_00216048
  0x002112B6  8acb                    mov      cl, bl                         
  0x002112B8  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002112BE  5f                      pop      edi                            
  0x002112BF  5e                      pop      esi                            
  0x002112C0  5b                      pop      ebx                            
  0x002112C1  c9                      leave                                   
  0x002112C2  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002112C3
; Start: 0x002112C3  End: 0x00211488  Size: 453 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021622B
; Called by: sub_0021107A
; ============================================================
sub_002112C3:
  0x002112C3  55                      push     ebp                            
  0x002112C4  8bec                    mov      ebp, esp                       
  0x002112C6  83ec0c                  sub      esp, 0xc                       
  0x002112C9  53                      push     ebx                            
  0x002112CA  8bd9                    mov      ebx, ecx                       
  0x002112CC  0fb64b0e                movzx    ecx, byte ptr [ebx + 0xe]      
  0x002112D0  8b4304                  mov      eax, dword ptr [ebx + 4]       
  0x002112D3  c1e106                  shl      ecx, 6                         
  0x002112D6  83c130                  add      ecx, 0x30                      
  0x002112D9  3903                    cmp      dword ptr [ebx], eax           
  0x002112DB  56                      push     esi                            
  0x002112DC  894df8                  mov      dword ptr [ebp - 8], ecx       
  0x002112DF  7302                    jae      0x2112e3                       
  0x002112E1  8903                    mov      dword ptr [ebx], eax           
                                        ; XREF: 0x002112DF (cond_jump)
  0x002112E3  0fafc1                  imul     eax, ecx                       
  0x002112E6  8b4b08                  mov      ecx, dword ptr [ebx + 8]       
  0x002112E9  83c108                  add      ecx, 8                         
  0x002112EC  c1e105                  shl      ecx, 5                         
  0x002112EF  03c1                    add      eax, ecx                       
  0x002112F1  8b0b                    mov      ecx, dword ptr [ebx]           
  0x002112F3  8d0c49                  lea      ecx, [ecx + ecx*2]             
  0x002112F6  c1e104                  shl      ecx, 4                         
  0x002112F9  03c1                    add      eax, ecx                       
  0x002112FB  8bc8                    mov      ecx, eax                       
  0x002112FD  8d81ff0f0000            lea      eax, [ecx + 0xfff]             
  0x00211303  c1e80c                  shr      eax, 0xc                       
  0x00211306  8bd0                    mov      edx, eax                       
  0x00211308  c1e204                  shl      edx, 4                         
  0x0021130B  03d1                    add      edx, ecx                       
  0x0021130D  8bc8                    mov      ecx, eax                       
  0x0021130F  c1e10c                  shl      ecx, 0xc                       
  0x00211312  3bca                    cmp      ecx, edx                       
  0x00211314  7301                    jae      0x211317                       
  0x00211316  40                      inc      eax                            
                                        ; XREF: 0x00211314 (cond_jump)
  0x00211317  57                      push     edi                            
  0x00211318  c1e00c                  shl      eax, 0xc                       
  0x0021131B  8bf8                    mov      edi, eax                       
  0x0021131D  57                      push     edi                            
  0x0021131E  897dfc                  mov      dword ptr [ebp - 4], edi       
  0x00211321  ff15487a2100            call     dword ptr [0x217a48]           ; -> xbox_MmAllocateContiguousMemory
  0x00211327  6a00                    push     0                              
  0x00211329  8bf0                    mov      esi, eax                       
  0x0021132B  57                      push     edi                            
  0x0021132C  56                      push     esi                            
  0x0021132D  ff15887b2100            call     dword ptr [0x217b88]           ; -> xbox_MmLockUnlockBufferPages
  0x00211333  56                      push     esi                            
  0x00211334  ff15847b2100            call     dword ptr [0x217b84]           ; -> xbox_MmGetPhysicalAddress
  0x0021133A  8bce                    mov      ecx, esi                       
  0x0021133C  2bc8                    sub      ecx, eax                       
  0x0021133E  890da0b9c600            mov      dword ptr [0xc6b9a0], ecx      
  0x00211344  8bcf                    mov      ecx, edi                       
  0x00211346  8bd1                    mov      edx, ecx                       
  0x00211348  c1e902                  shr      ecx, 2                         
  0x0021134B  33c0                    xor      eax, eax                       
  0x0021134D  8bfe                    mov      edi, esi                       
  0x0021134F  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00211351  8bca                    mov      ecx, edx                       
  0x00211353  83e103                  and      ecx, 3                         
  0x00211356  f3aa                    rep stosb byte ptr es:[edi], al          
  0x00211358  8bc2                    mov      eax, edx                       
  0x0021135A  33d2                    xor      edx, edx                       
  0x0021135C  8935a4b9c600            mov      dword ptr [0xc6b9a4], esi      
  0x00211362  8915a8b9c600            mov      dword ptr [0xc6b9a8], edx      
  0x00211368  0fb67b0e                movzx    edi, byte ptr [ebx + 0xe]      
  0x0021136C  8d0c06                  lea      ecx, [esi + eax]               
  0x0021136F  81c600010000            add      esi, 0x100                     
  0x00211375  33c0                    xor      eax, eax                       
  0x00211377  893dccb9c600            mov      dword ptr [0xc6b9cc], edi      
  0x0021137D  8915c8b9c600            mov      dword ptr [0xc6b9c8], edx      
  0x00211383  395304                  cmp      dword ptr [ebx + 4], edx       
  0x00211386  894df4                  mov      dword ptr [ebp - 0xc], ecx     
  0x00211389  763e                    jbe      0x2113c9                       
                                        ; XREF: 0x002113B2 (cond_jump)
  0x0021138B  8b3dc8b9c600            mov      edi, dword ptr [0xc6b9c8]      
  0x00211391  893e                    mov      dword ptr [esi], edi           
  0x00211393  8b3da8b9c600            mov      edi, dword ptr [0xc6b9a8]      
  0x00211399  8935c8b9c600            mov      dword ptr [0xc6b9c8], esi      
  0x0021139F  0375f8                  add      esi, dword ptr [ebp - 8]       
  0x002113A2  897e18                  mov      dword ptr [esi + 0x18], edi    
  0x002113A5  8935a8b9c600            mov      dword ptr [0xc6b9a8], esi      
  0x002113AB  83c630                  add      esi, 0x30                      
  0x002113AE  40                      inc      eax                            
  0x002113AF  3b4304                  cmp      eax, dword ptr [ebx + 4]       
  0x002113B2  72d7                    jb       0x21138b                       
  0x002113B4  eb13                    jmp      0x2113c9                       
                                        ; XREF: 0x002113CB (cond_jump)
  0x002113B6  8b3da8b9c600            mov      edi, dword ptr [0xc6b9a8]      
  0x002113BC  897e18                  mov      dword ptr [esi + 0x18], edi    
  0x002113BF  8935a8b9c600            mov      dword ptr [0xc6b9a8], esi      
  0x002113C5  83c630                  add      esi, 0x30                      
  0x002113C8  40                      inc      eax                            
                                        ; XREF: 0x00211389 (cond_jump), 0x002113B4 (jump)
  0x002113C9  3b03                    cmp      eax, dword ptr [ebx]           
  0x002113CB  72e9                    jb       0x2113b6                       
  0x002113CD  8d7e20                  lea      edi, [esi + 0x20]              
  0x002113D0  3bf9                    cmp      edi, ecx                       
  0x002113D2  8955fc                  mov      dword ptr [ebp - 4], edx       
  0x002113D5  8915acb9c600            mov      dword ptr [0xc6b9ac], edx      
  0x002113DB  8935b0b9c600            mov      dword ptr [0xc6b9b0], esi      
  0x002113E1  7724                    ja       0x211407                       
                                        ; XREF: 0x00211403 (cond_jump)
  0x002113E3  8bc6                    mov      eax, esi                       
  0x002113E5  2b05a0b9c600            sub      eax, dword ptr [0xc6b9a0]      
  0x002113EB  56                      push     esi                            
  0x002113EC  8975f8                  mov      dword ptr [ebp - 8], esi       
  0x002113EF  8947f0                  mov      dword ptr [edi - 0x10], eax    
  0x002113F2  e8344e0000              call     0x21622b                       ; -> sub_0021622B
  0x002113F7  83c620                  add      esi, 0x20                      
  0x002113FA  83c720                  add      edi, 0x20                      
  0x002113FD  ff45fc                  inc      dword ptr [ebp - 4]            
  0x00211400  3b7df4                  cmp      edi, dword ptr [ebp - 0xc]     
  0x00211403  76de                    jbe      0x2113e3                       
  0x00211405  33d2                    xor      edx, edx                       
                                        ; XREF: 0x002113E1 (cond_jump)
  0x00211407  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x0021140A  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x0021140D  a3b4b9c600              mov      dword ptr [0xc6b9b4], eax      
  0x00211412  8915b8b9c600            mov      dword ptr [0xc6b9b8], edx      
  0x00211418  c705bcb9c600e8030000    mov      dword ptr [0xc6b9bc], 0x3e8    
  0x00211422  660fb6430c              movzx    ax, byte ptr [ebx + 0xc]       
  0x00211427  66a3c0b9c600            mov      word ptr [0xc6b9c0], ax        
  0x0021142D  660fb6430d              movzx    ax, byte ptr [ebx + 0xd]       
  0x00211432  66a3c4b9c600            mov      word ptr [0xc6b9c4], ax        
  0x00211438  3b4b08                  cmp      ecx, dword ptr [ebx + 8]       
  0x0021143B  5f                      pop      edi                            
  0x0021143C  7632                    jbe      0x211470                       
  0x0021143E  2a4b08                  sub      cl, byte ptr [ebx + 8]         
  0x00211441  663bc2                  cmp      ax, dx                         
  0x00211444  7413                    je       0x211459                       
  0x00211446  8ad1                    mov      dl, cl                         
  0x00211448  d0ea                    shr      dl, 1                          
  0x0021144A  660fb6f2                movzx    si, dl                         
  0x0021144E  6603c6                  add      ax, si                         
  0x00211451  66a3c4b9c600            mov      word ptr [0xc6b9c4], ax        
  0x00211457  2aca                    sub      cl, dl                         
                                        ; XREF: 0x00211444 (cond_jump)
  0x00211459  660fb6c1                movzx    ax, cl                         
  0x0021145D  660105c0b9c600          add      word ptr [0xc6b9c0], ax        
  0x00211464  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00211467  894308                  mov      dword ptr [ebx + 8], eax       
  0x0021146A  66a1c4b9c600            mov      ax, word ptr [0xc6b9c4]        
                                        ; XREF: 0x0021143C (cond_jump)
  0x00211470  668b0dc0b9c600          mov      cx, word ptr [0xc6b9c0]        
  0x00211477  5e                      pop      esi                            
  0x00211478  66890dc2b9c600          mov      word ptr [0xc6b9c2], cx        
  0x0021147F  66a3c6b9c600            mov      word ptr [0xc6b9c6], ax        
  0x00211485  5b                      pop      ebx                            
  0x00211486  c9                      leave                                   
  0x00211487  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00211488
; Start: 0x00211488  End: 0x002114E9  Size: 97 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002128E9
; Called by: sub_002110C8
; ============================================================
sub_00211488:
  0x00211488  56                      push     esi                            
  0x00211489  8bf1                    mov      esi, ecx                       
  0x0021148B  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x0021148E  2b05a0b9c600            sub      eax, dword ptr [0xc6b9a0]      
  0x00211494  8b0e                    mov      ecx, dword ptr [esi]           
  0x00211496  894118                  mov      dword ptr [ecx + 0x18], eax    
  0x00211499  8b0e                    mov      ecx, dword ptr [esi]           
  0x0021149B  33c0                    xor      eax, eax                       
  0x0021149D  89411c                  mov      dword ptr [ecx + 0x1c], eax    
  0x002114A0  8b0e                    mov      ecx, dword ptr [esi]           
  0x002114A2  894120                  mov      dword ptr [ecx + 0x20], eax    
  0x002114A5  8b0e                    mov      ecx, dword ptr [esi]           
  0x002114A7  894124                  mov      dword ptr [ecx + 0x24], eax    
  0x002114AA  8b0e                    mov      ecx, dword ptr [esi]           
  0x002114AC  894128                  mov      dword ptr [ecx + 0x28], eax    
  0x002114AF  8b0e                    mov      ecx, dword ptr [esi]           
  0x002114B1  89412c                  mov      dword ptr [ecx + 0x2c], eax    
  0x002114B4  8b0e                    mov      ecx, dword ptr [esi]           
  0x002114B6  894130                  mov      dword ptr [ecx + 0x30], eax    
  0x002114B9  6a01                    push     1                              
  0x002114BB  66c786160400007827      mov      word ptr [esi + 0x416], 0x2778 
  0x002114C4  8b06                    mov      eax, dword ptr [esi]           
  0x002114C6  6a03                    push     3                              
  0x002114C8  c740402f2a0000          mov      dword ptr [eax + 0x40], 0x2a2f 
  0x002114CF  6a08                    push     8                              
  0x002114D1  66c786140400007423      mov      word ptr [esi + 0x414], 0x2374 
  0x002114DA  e80a140000              call     0x2128e9                       ; -> sub_002128E9
  0x002114DF  8b0e                    mov      ecx, dword ptr [esi]           
  0x002114E1  0fb7c0                  movzx    eax, ax                        
  0x002114E4  894144                  mov      dword ptr [ecx + 0x44], eax    
  0x002114E7  5e                      pop      esi                            
  0x002114E8  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002114E9
; Start: 0x002114E9  End: 0x002114F4  Size: 11 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002110C8
; ============================================================
sub_002114E9:
  0x002114E9  8b442404                mov      eax, dword ptr [esp + 4]       
  0x002114ED  8b00                    mov      eax, dword ptr [eax]           
  0x002114EF  8b00                    mov      eax, dword ptr [eax]           
  0x002114F1  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002114F4
; Start: 0x002114F4  End: 0x0021151D  Size: 41 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_002110C8
; ============================================================
sub_002114F4:
  0x002114F4  55                      push     ebp                            
  0x002114F5  8bec                    mov      ebp, esp                       
  0x002114F7  51                      push     ecx                            
  0x002114F8  51                      push     ecx                            
  0x002114F9  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x002114FC  8b02                    mov      eax, dword ptr [edx]           
  0x002114FE  8b4804                  mov      ecx, dword ptr [eax + 4]       
  0x00211501  56                      push     esi                            
  0x00211502  be00010000              mov      esi, 0x100                     
  0x00211507  85ce                    test     esi, ecx                       
  0x00211509  7412                    je       0x21151d                       
  0x0021150B  8b4808                  mov      ecx, dword ptr [eax + 8]       
  0x0021150E  83c908                  or       ecx, 8                         
  0x00211511  894808                  mov      dword ptr [eax + 8], ecx       
  0x00211514  8b12                    mov      edx, dword ptr [edx]           
                                        ; XREF: 0x00211519 (cond_jump)
  0x00211516  857204                  test     dword ptr [edx + 4], esi       
  0x00211519  75fb                    jne      0x211516                       
  0x0021151B  eb34                    jmp      0x211551                       
; end of function
                                        ; XREF: 0x00211509 (cond_jump)
  0x0021151D  8bd1                    mov      edx, ecx                       
  0x0021151F  c1ea06                  shr      edx, 6                         
  0x00211522  83e203                  and      edx, 3                         
  0x00211525  742a                    je       0x211551                       
  0x00211527  83fa02                  cmp      edx, 2                         
  0x0021152A  7425                    je       0x211551                       
  0x0021152C  81e17fffffff            and      ecx, 0xffffff7f                
  0x00211532  83c940                  or       ecx, 0x40                      
  0x00211535  894804                  mov      dword ptr [eax + 4], ecx       
  0x00211538  834dfcff                or       dword ptr [ebp - 4], 0xffffffff 
  0x0021153C  8d45f8                  lea      eax, [ebp - 8]                 
  0x0021153F  50                      push     eax                            
  0x00211540  6a00                    push     0                              
  0x00211542  6a00                    push     0                              
  0x00211544  c745f8c0f2fcff          mov      dword ptr [ebp - 8], 0xfffcf2c0 
  0x0021154B  ff15c07a2100            call     dword ptr [0x217ac0]           ; -> xbox_KeCancelTimer
                                        ; XREF: 0x0021151B (jump), 0x00211525 (cond_jump), 0x0021152A (cond_jump)
  0x00211551  5e                      pop      esi                            
  0x00211552  c9                      leave                                   
  0x00211553  c20400                  ret      4                              

; ============================================================
; Function: sub_00211556
; Start: 0x00211556  End: 0x00211571  Size: 27 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002137C4
; ============================================================
sub_00211556:
  0x00211556  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x0021155A  33c0                    xor      eax, eax                       
  0x0021155C  40                      inc      eax                            
  0x0021155D  d3e0                    shl      eax, cl                        
  0x0021155F  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x00211563  094104                  or       dword ptr [ecx + 4], eax       
  0x00211566  807c240c00              cmp      byte ptr [esp + 0xc], 0        
  0x0021156B  7404                    je       0x211571                       
  0x0021156D  0901                    or       dword ptr [ecx], eax           
  0x0021156F  eb04                    jmp      0x211575                       
; end of function
                                        ; XREF: 0x0021156B (cond_jump)
  0x00211571  f7d0                    not      eax                            
  0x00211573  2101                    and      dword ptr [ecx], eax           
                                        ; XREF: 0x0021156F (jump)
  0x00211575  c20c00                  ret      0xc                            

; ============================================================
; Function: sub_00211578
; Start: 0x00211578  End: 0x0021157D  Size: 5 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_000A6400
; ============================================================
sub_00211578:
  0x00211578  e9eff5ffff              jmp      0x210b6c                       
; end of function

; ============================================================
; Function: sub_0021157D
; Start: 0x0021157D  End: 0x0021159F  Size: 34 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_000A4D40
; ============================================================
sub_0021157D:
  0x0021157D  56                      push     esi                            
  0x0021157E  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00211584  8b542408                mov      edx, dword ptr [esp + 8]       
  0x00211588  8b32                    mov      esi, dword ptr [edx]           
  0x0021158A  83620400                and      dword ptr [edx + 4], 0         
  0x0021158E  8ac8                    mov      cl, al                         
  0x00211590  897208                  mov      dword ptr [edx + 8], esi       
  0x00211593  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00211599  8bc6                    mov      eax, esi                       
  0x0021159B  5e                      pop      esi                            
  0x0021159C  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_0021159F
; Start: 0x0021159F  End: 0x002115B9  Size: 26 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_000A4DD0
; ============================================================
sub_0021159F:
  0x0021159F  55                      push     ebp                            
  0x002115A0  8bec                    mov      ebp, esp                       
  0x002115A2  56                      push     esi                            
  0x002115A3  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x002115A6  33c0                    xor      eax, eax                       
  0x002115A8  394604                  cmp      dword ptr [esi + 4], eax       
  0x002115AB  750c                    jne      0x2115b9                       
  0x002115AD  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x002115B0  8901                    mov      dword ptr [ecx], eax           
  0x002115B2  8b4d10                  mov      ecx, dword ptr [ebp + 0x10]    
  0x002115B5  8901                    mov      dword ptr [ecx], eax           
  0x002115B7  eb4e                    jmp      0x211607                       
; end of function
                                        ; XREF: 0x002115AB (cond_jump)
  0x002115B9  53                      push     ebx                            
  0x002115BA  57                      push     edi                            
  0x002115BB  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x002115C1  8b4e08                  mov      ecx, dword ptr [esi + 8]       
  0x002115C4  8b5d0c                  mov      ebx, dword ptr [ebp + 0xc]     
  0x002115C7  f7d1                    not      ecx                            
  0x002115C9  230e                    and      ecx, dword ptr [esi]           
  0x002115CB  890b                    mov      dword ptr [ebx], ecx           
  0x002115CD  8b16                    mov      edx, dword ptr [esi]           
  0x002115CF  8b4d10                  mov      ecx, dword ptr [ebp + 0x10]    
  0x002115D2  f7d2                    not      edx                            
  0x002115D4  235608                  and      edx, dword ptr [esi + 8]       
  0x002115D7  8911                    mov      dword ptr [ecx], edx           
  0x002115D9  8b7e04                  mov      edi, dword ptr [esi + 4]       
  0x002115DC  237e08                  and      edi, dword ptr [esi + 8]       
  0x002115DF  233e                    and      edi, dword ptr [esi]           
  0x002115E1  0bd7                    or       edx, edi                       
  0x002115E3  8911                    mov      dword ptr [ecx], edx           
  0x002115E5  093b                    or       dword ptr [ebx], edi           
  0x002115E7  8b0e                    mov      ecx, dword ptr [esi]           
  0x002115E9  83660400                and      dword ptr [esi + 4], 0         
  0x002115ED  894e08                  mov      dword ptr [esi + 8], ecx       
  0x002115F0  8ac8                    mov      cl, al                         
  0x002115F2  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002115F8  8b03                    mov      eax, dword ptr [ebx]           
  0x002115FA  8b4d10                  mov      ecx, dword ptr [ebp + 0x10]    
  0x002115FD  0b01                    or       eax, dword ptr [ecx]           
  0x002115FF  5f                      pop      edi                            
  0x00211600  f7d8                    neg      eax                            
  0x00211602  1bc0                    sbb      eax, eax                       
  0x00211604  f7d8                    neg      eax                            
  0x00211606  5b                      pop      ebx                            
                                        ; XREF: 0x002115B7 (jump)
  0x00211607  5e                      pop      esi                            
  0x00211608  5d                      pop      ebp                            
  0x00211609  c20c00                  ret      0xc                            

; ============================================================
; Function: sub_0021160C
; Start: 0x0021160C  End: 0x00211634  Size: 40 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0021160C:
  0x0021160C  8b542404                mov      edx, dword ptr [esp + 4]       
  0x00211610  57                      push     edi                            
  0x00211611  6a58                    push     0x58                           
  0x00211613  59                      pop      ecx                            
  0x00211614  33c0                    xor      eax, eax                       
  0x00211616  8d7a10                  lea      edi, [edx + 0x10]              
  0x00211619  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x0021161B  c7420c04000000          mov      dword ptr [edx + 0xc], 4       
  0x00211622  a1c8704000              mov      eax, dword ptr [0x4070c8]      
  0x00211627  894208                  mov      dword ptr [edx + 8], eax       
  0x0021162A  8915c8704000            mov      dword ptr [0x4070c8], edx      
  0x00211630  5f                      pop      edi                            
  0x00211631  c20400                  ret      4                              
; end of function
  0x00211634  53                      push     ebx                            
  0x00211635  56                      push     esi                            
  0x00211636  57                      push     edi                            
  0x00211637  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x0021163B  8bcf                    mov      ecx, edi                       
  0x0021163D  e8ad2d0000              call     0x2143ef                       ; -> sub_002143EF
  0x00211642  33db                    xor      ebx, ebx                       
  0x00211644  33c9                    xor      ecx, ecx                       
  0x00211646  83f810                  cmp      eax, 0x10                      
  0x00211649  7204                    jb       0x21164f                       
  0x0021164B  83e810                  sub      eax, 0x10                      
  0x0021164E  41                      inc      ecx                            
                                        ; XREF: 0x00211649 (cond_jump)
  0x0021164F  8d0441                  lea      eax, [ecx + eax*2]             
  0x00211652  8b0dd4a04000            mov      ecx, dword ptr [0x40a0d4]      
  0x00211658  53                      push     ebx                            
  0x00211659  8d0440                  lea      eax, [eax + eax*2]             
  0x0021165C  6a01                    push     1                              
  0x0021165E  8d3481                  lea      esi, [ecx + eax*4]             
  0x00211661  6a02                    push     2                              
  0x00211663  8bcf                    mov      ecx, edi                       
  0x00211665  e80f2d0000              call     0x214379                       ; -> sub_00214379
  0x0021166A  3bc3                    cmp      eax, ebx                       
  0x0021166C  744a                    je       0x2116b8                       
  0x0021166E  8a4802                  mov      cl, byte ptr [eax + 2]         
  0x00211671  884e05                  mov      byte ptr [esi + 5], cl         
  0x00211674  6683780440              cmp      word ptr [eax + 4], 0x40       
  0x00211679  8bcf                    mov      ecx, edi                       
  0x0021167B  753d                    jne      0x2116ba                       
  0x0021167D  53                      push     ebx                            
  0x0021167E  53                      push     ebx                            
  0x0021167F  6a02                    push     2                              
  0x00211681  e8f32c0000              call     0x214379                       ; -> sub_00214379
  0x00211686  3bc3                    cmp      eax, ebx                       
  0x00211688  742e                    je       0x2116b8                       
  0x0021168A  8a4802                  mov      cl, byte ptr [eax + 2]         
  0x0021168D  884e06                  mov      byte ptr [esi + 6], cl         
  0x00211690  6683780440              cmp      word ptr [eax + 4], 0x40       
  0x00211695  8bcf                    mov      ecx, edi                       
  0x00211697  7521                    jne      0x2116ba                       
  0x00211699  56                      push     esi                            
  0x0021169A  e8df2b0000              call     0x21427e                       ; -> sub_0021427E
  0x0021169F  8bcf                    mov      ecx, edi                       
  0x002116A1  893e                    mov      dword ptr [esi], edi           
  0x002116A3  e8e32b0000              call     0x21428b                       ; -> sub_0021428B
  0x002116A8  53                      push     ebx                            
  0x002116A9  8bcf                    mov      ecx, edi                       
  0x002116AB  884604                  mov      byte ptr [esi + 4], al         
  0x002116AE  e8dc2b0000              call     0x21428f                       ; -> sub_0021428F
  0x002116B3  53                      push     ebx                            
  0x002116B4  8bcf                    mov      ecx, edi                       
  0x002116B6  eb07                    jmp      0x2116bf                       
                                        ; XREF: 0x0021166C (cond_jump), 0x00211688 (cond_jump)
  0x002116B8  8bcf                    mov      ecx, edi                       
                                        ; XREF: 0x0021167B (cond_jump), 0x00211697 (cond_jump)
  0x002116BA  6800040080              push     0x80000400                     
                                        ; XREF: 0x002116B6 (jump)
  0x002116BF  e8be240000              call     0x213b82                       ; -> sub_00213B82
  0x002116C4  5f                      pop      edi                            
  0x002116C5  5e                      pop      esi                            
  0x002116C6  5b                      pop      ebx                            
  0x002116C7  c20400                  ret      4                              

; ============================================================
; Function: sub_002116CA
; Start: 0x002116CA  End: 0x002116F8  Size: 46 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_0021179C
; ============================================================
sub_002116CA:
  0x002116CA  56                      push     esi                            
  0x002116CB  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x002116CF  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x002116D2  a900000400              test     eax, 0x40000                   
  0x002116D7  57                      push     edi                            
  0x002116D8  8b7e08                  mov      edi, dword ptr [esi + 8]       
  0x002116DB  741b                    je       0x2116f8                       
  0x002116DD  8b5624                  mov      edx, dword ptr [esi + 0x24]    
  0x002116E0  83662400                and      dword ptr [esi + 0x24], 0      
  0x002116E4  c6861101000043          mov      byte ptr [esi + 0x111], 0x43   
  0x002116EB  899620010000            mov      dword ptr [esi + 0x120], edx   
  0x002116F1  25fffffbff              and      eax, 0xfffbffff                
  0x002116F6  eb35                    jmp      0x21172d                       
; end of function
                                        ; XREF: 0x002116DB (cond_jump)
  0x002116F8  a900000200              test     eax, 0x20000                   
  0x002116FD  741b                    je       0x21171a                       
  0x002116FF  8b5620                  mov      edx, dword ptr [esi + 0x20]    
  0x00211702  83662000                and      dword ptr [esi + 0x20], 0      
  0x00211706  c6861101000043          mov      byte ptr [esi + 0x111], 0x43   
  0x0021170D  899620010000            mov      dword ptr [esi + 0x120], edx   
  0x00211713  25fffffdff              and      eax, 0xfffdffff                
  0x00211718  eb13                    jmp      0x21172d                       
                                        ; XREF: 0x002116FD (cond_jump)
  0x0021171A  a900000100              test     eax, 0x10000                   
  0x0021171F  7432                    je       0x211753                       
  0x00211721  c68611010000c3          mov      byte ptr [esi + 0x111], 0xc3   
  0x00211728  25fffffeff              and      eax, 0xfffeffff                
                                        ; XREF: 0x002116F6 (jump), 0x00211718 (jump)
  0x0021172D  8d8e10010000            lea      ecx, [esi + 0x110]             
  0x00211733  c6011c                  mov      byte ptr [ecx], 0x1c           
  0x00211736  c78618010000ca162100    mov      dword ptr [esi + 0x118], 0x2116ca 
  0x00211740  89b61c010000            mov      dword ptr [esi + 0x11c], esi   
  0x00211746  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00211749  51                      push     ecx                            
  0x0021174A  8b0f                    mov      ecx, dword ptr [edi]           
  0x0021174C  e8d02e0000              call     0x214621                       ; -> sub_00214621
  0x00211751  eb44                    jmp      0x211797                       
                                        ; XREF: 0x0021171F (cond_jump)
  0x00211753  25fffff7ff              and      eax, 0xfff7ffff                
  0x00211758  53                      push     ebx                            
  0x00211759  33db                    xor      ebx, ebx                       
  0x0021175B  a801                    test     al, 1                          
  0x0021175D  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00211760  895e08                  mov      dword ptr [esi + 8], ebx       
  0x00211763  7416                    je       0x21177b                       
  0x00211765  53                      push     ebx                            
  0x00211766  53                      push     ebx                            
  0x00211767  8d862c010000            lea      eax, [esi + 0x12c]             
  0x0021176D  50                      push     eax                            
  0x0021176E  895f08                  mov      dword ptr [edi + 8], ebx       
  0x00211771  ff150c7b2100            call     dword ptr [0x217b0c]           ; -> xbox_KeSetEvent
  0x00211777  83660cfe                and      dword ptr [esi + 0xc], 0xfffffffe 
                                        ; XREF: 0x00211763 (cond_jump)
  0x0021177B  f6460c02                test     byte ptr [esi + 0xc], 2        
  0x0021177F  7415                    je       0x211796                       
  0x00211781  8b0f                    mov      ecx, dword ptr [edi]           
  0x00211783  e83c200000              call     0x2137c4                       ; -> sub_002137C4
  0x00211788  891f                    mov      dword ptr [edi], ebx           
  0x0021178A  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x0021178D  83e0fd                  and      eax, 0xfffffffd                
  0x00211790  83c804                  or       eax, 4                         
  0x00211793  89460c                  mov      dword ptr [esi + 0xc], eax     
                                        ; XREF: 0x0021177F (cond_jump)
  0x00211796  5b                      pop      ebx                            
                                        ; XREF: 0x00211751 (jump)
  0x00211797  5f                      pop      edi                            
  0x00211798  5e                      pop      esi                            
  0x00211799  c20800                  ret      8                              

; ============================================================
; Function: sub_0021179C
; Start: 0x0021179C  End: 0x002117B6  Size: 26 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002116CA
; ============================================================
sub_0021179C:
  0x0021179C  8b410c                  mov      eax, dword ptr [ecx + 0xc]     
  0x0021179F  ba00000800              mov      edx, 0x80000                   
  0x002117A4  85c2                    test     edx, eax                       
  0x002117A6  750d                    jne      0x2117b5                       
  0x002117A8  51                      push     ecx                            
  0x002117A9  0bc2                    or       eax, edx                       
  0x002117AB  6a00                    push     0                              
  0x002117AD  89410c                  mov      dword ptr [ecx + 0xc], eax     
  0x002117B0  e815ffffff              call     0x2116ca                       ; -> sub_002116CA
                                        ; XREF: 0x002117A6 (cond_jump)
  0x002117B5  c3                      ret                                     
; end of function
  0x002117B6  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x002117BA  56                      push     esi                            
  0x002117BB  e8ba2a0000              call     0x21427a                       ; -> sub_0021427A
  0x002117C0  8bf0                    mov      esi, eax                       
  0x002117C2  8b4e08                  mov      ecx, dword ptr [esi + 8]       
  0x002117C5  85c9                    test     ecx, ecx                       
  0x002117C7  7414                    je       0x2117dd                       
  0x002117C9  8b410c                  mov      eax, dword ptr [ecx + 0xc]     
  0x002117CC  a804                    test     al, 4                          
  0x002117CE  750d                    jne      0x2117dd                       
  0x002117D0  83c802                  or       eax, 2                         
  0x002117D3  89410c                  mov      dword ptr [ecx + 0xc], eax     
  0x002117D6  e8c1ffffff              call     0x21179c                       ; -> sub_0021179C
  0x002117DB  eb0a                    jmp      0x2117e7                       
                                        ; XREF: 0x002117C7 (cond_jump), 0x002117CE (cond_jump)
  0x002117DD  8b0e                    mov      ecx, dword ptr [esi]           
  0x002117DF  e8e01f0000              call     0x2137c4                       ; -> sub_002137C4
  0x002117E4  832600                  and      dword ptr [esi], 0             
                                        ; XREF: 0x002117DB (jump)
  0x002117E7  5e                      pop      esi                            
  0x002117E8  c20400                  ret      4                              
  0x002117EB  a11c7b2100              mov      eax, dword ptr [0x217b1c]      
  0x002117F0  a3c4072100              mov      dword ptr [0x2107c4], eax      
  0x002117F5  a3c8072100              mov      dword ptr [0x2107c8], eax      
  0x002117FA  a3d4072100              mov      dword ptr [0x2107d4], eax      
  0x002117FF  a3d8072100              mov      dword ptr [0x2107d8], eax      
  0x00211804  a3dc072100              mov      dword ptr [0x2107dc], eax      
  0x00211809  a3e0072100              mov      dword ptr [0x2107e0], eax      
  0x0021180E  a3e4072100              mov      dword ptr [0x2107e4], eax      
  0x00211813  a3e8072100              mov      dword ptr [0x2107e8], eax      
  0x00211818  a3f4072100              mov      dword ptr [0x2107f4], eax      
  0x0021181D  a3f8072100              mov      dword ptr [0x2107f8], eax      
  0x00211822  c3                      ret                                     

; ============================================================
; Function: sub_00211823
; Start: 0x00211823  End: 0x00211838  Size: 21 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_000A4D40, sub_000A4DD0
; ============================================================
sub_00211823:
  0x00211823  55                      push     ebp                            
  0x00211824  8bec                    mov      ebp, esp                       
  0x00211826  51                      push     ecx                            
  0x00211827  8365fc00                and      dword ptr [ebp - 4], 0         
  0x0021182B  817d0800082100          cmp      dword ptr [ebp + 8], 0x210800  
  0x00211832  7504                    jne      0x211838                       
  0x00211834  32c9                    xor      cl, cl                         
  0x00211836  eb18                    jmp      0x211850                       
; end of function
                                        ; XREF: 0x00211832 (cond_jump)
  0x00211838  817d080c082100          cmp      dword ptr [ebp + 8], 0x21080c  
  0x0021183F  7504                    jne      0x211845                       
  0x00211841  b101                    mov      cl, 1                          
  0x00211843  eb0b                    jmp      0x211850                       
                                        ; XREF: 0x0021183F (cond_jump)
  0x00211845  817d0818082100          cmp      dword ptr [ebp + 8], 0x210818  
  0x0021184C  753d                    jne      0x21188b                       
  0x0021184E  b102                    mov      cl, 2                          
                                        ; XREF: 0x00211836 (jump), 0x00211843 (jump)
  0x00211850  8b4514                  mov      eax, dword ptr [ebp + 0x14]    
  0x00211853  85c0                    test     eax, eax                       
  0x00211855  750d                    jne      0x211864                       
  0x00211857  0fb6c1                  movzx    eax, cl                        
  0x0021185A  8d0440                  lea      eax, [eax + eax*2]             
  0x0021185D  8b04c5d4082100          mov      eax, dword ptr [eax*8 + 0x2108d4] 
                                        ; XREF: 0x00211855 (cond_jump)
  0x00211864  837d1001                cmp      dword ptr [ebp + 0x10], 1      
  0x00211868  8b550c                  mov      edx, dword ptr [ebp + 0xc]     
  0x0021186B  7503                    jne      0x211870                       
  0x0021186D  83c210                  add      edx, 0x10                      
                                        ; XREF: 0x0021186B (cond_jump)
  0x00211870  50                      push     eax                            
  0x00211871  8d45fc                  lea      eax, [ebp - 4]                 
  0x00211874  50                      push     eax                            
  0x00211875  e80e0c0000              call     0x212488                       ; -> sub_00212488
  0x0021187A  837dfc00                cmp      dword ptr [ebp - 4], 0         
  0x0021187E  7506                    jne      0x211886                       
  0x00211880  50                      push     eax                            
  0x00211881  e8eab1f7ff              call     0x18ca70                       ; -> sub_0018CA70
                                        ; XREF: 0x0021187E (cond_jump)
  0x00211886  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00211889  eb09                    jmp      0x211894                       
                                        ; XREF: 0x0021184C (cond_jump)
  0x0021188B  6a57                    push     0x57                           
  0x0021188D  e8deb1f7ff              call     0x18ca70                       ; -> sub_0018CA70
  0x00211892  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00211889 (jump)
  0x00211894  c9                      leave                                   
  0x00211895  c21000                  ret      0x10                           

; ============================================================
; Function: sub_00211898
; Start: 0x00211898  End: 0x002118A4  Size: 12 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00212140
; Called by: sub_000A4DD0
; ============================================================
sub_00211898:
  0x00211898  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x0021189C  e89f080000              call     0x212140                       ; -> sub_00212140
  0x002118A1  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002118A4
; Start: 0x002118A4  End: 0x00211A96  Size: 498 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00214299, sub_00214621
; Called by: sub_000A4D40, sub_000A4DD0
; ============================================================
sub_002118A4:
  0x002118A4  55                      push     ebp                            
  0x002118A5  8bec                    mov      ebp, esp                       
  0x002118A7  83ec48                  sub      esp, 0x48                      
  0x002118AA  53                      push     ebx                            
  0x002118AB  56                      push     esi                            
  0x002118AC  33db                    xor      ebx, ebx                       
  0x002118AE  57                      push     edi                            
  0x002118AF  895df8                  mov      dword ptr [ebp - 8], ebx       
  0x002118B2  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x002118B8  8845ff                  mov      byte ptr [ebp - 1], al         
  0x002118BB  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x002118BE  8b30                    mov      esi, dword ptr [eax]           
  0x002118C0  3bf3                    cmp      esi, ebx                       
  0x002118C2  0f84ad010000            je       0x211a75                       
  0x002118C8  f6460402                test     byte ptr [esi + 4], 2          
  0x002118CC  0f85a3010000            jne      0x211a75                       
  0x002118D2  8b550c                  mov      edx, dword ptr [ebp + 0xc]     
  0x002118D5  33c0                    xor      eax, eax                       
  0x002118D7  6a06                    push     6                              
  0x002118D9  59                      pop      ecx                            
  0x002118DA  8bfa                    mov      edi, edx                       
  0x002118DC  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x002118DE  aa                      stosb    byte ptr es:[edi], al          
  0x002118DF  8a460b                  mov      al, byte ptr [esi + 0xb]       
  0x002118E2  8802                    mov      byte ptr [edx], al             
  0x002118E4  0fb6460a                movzx    eax, byte ptr [esi + 0xa]      
  0x002118E8  8d0440                  lea      eax, [eax + eax*2]             
  0x002118EB  c1e003                  shl      eax, 3                         
  0x002118EE  f680dc08210001          test     byte ptr [eax + 0x2108dc], 1   
  0x002118F5  740c                    je       0x211903                       
  0x002118F7  c745f805000000          mov      dword ptr [ebp - 8], 5         
  0x002118FE  e979010000              jmp      0x211a7c                       
                                        ; XREF: 0x002118F5 (cond_jump)
  0x00211903  8b80d0082100            mov      eax, dword ptr [eax + 0x2108d0] 
  0x00211909  0fb600                  movzx    eax, byte ptr [eax]            
  0x0021190C  8d4df0                  lea      ecx, [ebp - 0x10]              
  0x0021190F  894df4                  mov      dword ptr [ebp - 0xc], ecx     
  0x00211912  8d4df0                  lea      ecx, [ebp - 0x10]              
  0x00211915  894df0                  mov      dword ptr [ebp - 0x10], ecx    
  0x00211918  8d4de8                  lea      ecx, [ebp - 0x18]              
  0x0021191B  894dc4                  mov      dword ptr [ebp - 0x3c], ecx    
  0x0021191E  8d4802                  lea      ecx, [eax + 2]                 
  0x00211921  83c213                  add      edx, 0x13                      
  0x00211924  885de8                  mov      byte ptr [ebp - 0x18], bl      
  0x00211927  c645ea04                mov      byte ptr [ebp - 0x16], 4       
  0x0021192B  895dec                  mov      dword ptr [ebp - 0x14], ebx    
  0x0021192E  c645b830                mov      byte ptr [ebp - 0x48], 0x30    
  0x00211932  c645b940                mov      byte ptr [ebp - 0x47], 0x40    
  0x00211936  c745c056422100          mov      dword ptr [ebp - 0x40], 0x214256 
  0x0021193D  895dc8                  mov      dword ptr [ebp - 0x38], ebx    
  0x00211940  8955d0                  mov      dword ptr [ebp - 0x30], edx    
  0x00211943  894dcc                  mov      dword ptr [ebp - 0x34], ecx    
  0x00211946  c645d402                mov      byte ptr [ebp - 0x2c], 2       
  0x0021194A  c645d501                mov      byte ptr [ebp - 0x2b], 1       
  0x0021194E  885dd6                  mov      byte ptr [ebp - 0x2a], bl      
  0x00211951  c645e0c1                mov      byte ptr [ebp - 0x20], 0xc1    
  0x00211955  c645e101                mov      byte ptr [ebp - 0x1f], 1       
  0x00211959  66c745e20002            mov      word ptr [ebp - 0x1e], 0x200   
  0x0021195F  660fb64e05              movzx    cx, byte ptr [esi + 5]         
  0x00211964  83c002                  add      eax, 2                         
  0x00211967  668945e6                mov      word ptr [ebp - 0x1a], ax      
  0x0021196B  8d45b8                  lea      eax, [ebp - 0x48]              
  0x0021196E  66894de4                mov      word ptr [ebp - 0x1c], cx      
  0x00211972  8b0e                    mov      ecx, dword ptr [esi]           
  0x00211974  50                      push     eax                            
  0x00211975  e8a72c0000              call     0x214621                       ; -> sub_00214621
  0x0021197A  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x0021197D  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00211983  8b3d107b2100            mov      edi, dword ptr [0x217b10]      
  0x00211989  53                      push     ebx                            
  0x0021198A  53                      push     ebx                            
  0x0021198B  53                      push     ebx                            
  0x0021198C  53                      push     ebx                            
  0x0021198D  8d45e8                  lea      eax, [ebp - 0x18]              
  0x00211990  50                      push     eax                            
  0x00211991  ffd7                    call     edi                            
  0x00211993  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00211999  8845ff                  mov      byte ptr [ebp - 1], al         
  0x0021199C  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x0021199F  3918                    cmp      dword ptr [eax], ebx           
  0x002119A1  0f84ce000000            je       0x211a75                       
  0x002119A7  f6460402                test     byte ptr [esi + 4], 2          
  0x002119AB  0f85c4000000            jne      0x211a75                       
  0x002119B1  395dbc                  cmp      dword ptr [ebp - 0x44], ebx    
  0x002119B4  0f8cae000000            jl       0x211a68                       
  0x002119BA  0fb6460a                movzx    eax, byte ptr [esi + 0xa]      
  0x002119BE  8d0440                  lea      eax, [eax + eax*2]             
  0x002119C1  8b04c5cc082100          mov      eax, dword ptr [eax*8 + 0x2108cc] 
  0x002119C8  0fb600                  movzx    eax, byte ptr [eax]            
  0x002119CB  8d4de8                  lea      ecx, [ebp - 0x18]              
  0x002119CE  894dc4                  mov      dword ptr [ebp - 0x3c], ecx    
  0x002119D1  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x002119D4  41                      inc      ecx                            
  0x002119D5  894dd0                  mov      dword ptr [ebp - 0x30], ecx    
  0x002119D8  8d4802                  lea      ecx, [eax + 2]                 
  0x002119DB  83c002                  add      eax, 2                         
  0x002119DE  c645b830                mov      byte ptr [ebp - 0x48], 0x30    
  0x002119E2  c645b940                mov      byte ptr [ebp - 0x47], 0x40    
  0x002119E6  c745c056422100          mov      dword ptr [ebp - 0x40], 0x214256 
  0x002119ED  895dc8                  mov      dword ptr [ebp - 0x38], ebx    
  0x002119F0  894dcc                  mov      dword ptr [ebp - 0x34], ecx    
  0x002119F3  c645d402                mov      byte ptr [ebp - 0x2c], 2       
  0x002119F7  c645d501                mov      byte ptr [ebp - 0x2b], 1       
  0x002119FB  885dd6                  mov      byte ptr [ebp - 0x2a], bl      
  0x002119FE  c645e0c1                mov      byte ptr [ebp - 0x20], 0xc1    
  0x00211A02  c645e101                mov      byte ptr [ebp - 0x1f], 1       
  0x00211A06  66c745e20001            mov      word ptr [ebp - 0x1e], 0x100   
  0x00211A0C  660fb64e05              movzx    cx, byte ptr [esi + 5]         
  0x00211A11  668945e6                mov      word ptr [ebp - 0x1a], ax      
  0x00211A15  8d45f0                  lea      eax, [ebp - 0x10]              
  0x00211A18  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x00211A1B  8d45f0                  lea      eax, [ebp - 0x10]              
  0x00211A1E  8945f0                  mov      dword ptr [ebp - 0x10], eax    
  0x00211A21  8d45b8                  lea      eax, [ebp - 0x48]              
  0x00211A24  66894de4                mov      word ptr [ebp - 0x1c], cx      
  0x00211A28  885de8                  mov      byte ptr [ebp - 0x18], bl      
  0x00211A2B  c645ea04                mov      byte ptr [ebp - 0x16], 4       
  0x00211A2F  895dec                  mov      dword ptr [ebp - 0x14], ebx    
  0x00211A32  8b0e                    mov      ecx, dword ptr [esi]           
  0x00211A34  50                      push     eax                            
  0x00211A35  e8e72b0000              call     0x214621                       ; -> sub_00214621
  0x00211A3A  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x00211A3D  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00211A43  53                      push     ebx                            
  0x00211A44  53                      push     ebx                            
  0x00211A45  53                      push     ebx                            
  0x00211A46  53                      push     ebx                            
  0x00211A47  8d45e8                  lea      eax, [ebp - 0x18]              
  0x00211A4A  50                      push     eax                            
  0x00211A4B  ffd7                    call     edi                            
  0x00211A4D  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00211A53  8845ff                  mov      byte ptr [ebp - 1], al         
  0x00211A56  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x00211A59  3918                    cmp      dword ptr [eax], ebx           
  0x00211A5B  7418                    je       0x211a75                       
  0x00211A5D  f6460402                test     byte ptr [esi + 4], 2          
  0x00211A61  7512                    jne      0x211a75                       
  0x00211A63  395dbc                  cmp      dword ptr [ebp - 0x44], ebx    
  0x00211A66  7d14                    jge      0x211a7c                       
                                        ; XREF: 0x002119B4 (cond_jump)
  0x00211A68  ff75bc                  push     dword ptr [ebp - 0x44]         
  0x00211A6B  e829280000              call     0x214299                       ; -> sub_00214299
  0x00211A70  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x00211A73  eb07                    jmp      0x211a7c                       
                                        ; XREF: 0x002118C2 (cond_jump), 0x002118CC (cond_jump), 0x002119A1 (cond_jump), 0x002119AB (cond_jump), 0x00211A5B (cond_jump), ... (+1 more)
  0x00211A75  c745f88f040000          mov      dword ptr [ebp - 8], 0x48f     
                                        ; XREF: 0x002118FE (jump), 0x00211A66 (cond_jump), 0x00211A73 (jump)
  0x00211A7C  8b450c                  mov      eax, dword ptr [ebp + 0xc]     
  0x00211A7F  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x00211A82  66895801                mov      word ptr [eax + 1], bx         
  0x00211A86  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00211A8C  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00211A8F  5f                      pop      edi                            
  0x00211A90  5e                      pop      esi                            
  0x00211A91  5b                      pop      ebx                            
  0x00211A92  c9                      leave                                   
  0x00211A93  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00211A96
; Start: 0x00211A96  End: 0x00211AB2  Size: 28 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_000A4DD0
; ============================================================
sub_00211A96:
  0x00211A96  53                      push     ebx                            
  0x00211A97  56                      push     esi                            
  0x00211A98  33db                    xor      ebx, ebx                       
  0x00211A9A  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00211AA0  8b54240c                mov      edx, dword ptr [esp + 0xc]     
  0x00211AA4  80baa300000001          cmp      byte ptr [edx + 0xa3], 1       
  0x00211AAB  7505                    jne      0x211ab2                       
  0x00211AAD  6a57                    push     0x57                           
  0x00211AAF  5e                      pop      esi                            
  0x00211AB0  eb46                    jmp      0x211af8                       
; end of function
                                        ; XREF: 0x00211AAB (cond_jump)
  0x00211AB2  8b0a                    mov      ecx, dword ptr [edx]           
  0x00211AB4  85c9                    test     ecx, ecx                       
  0x00211AB6  7406                    je       0x211abe                       
  0x00211AB8  f6410402                test     byte ptr [ecx + 4], 2          
  0x00211ABC  7405                    je       0x211ac3                       
                                        ; XREF: 0x00211AB6 (cond_jump)
  0x00211ABE  bb8f040000              mov      ebx, 0x48f                     
                                        ; XREF: 0x00211ABC (cond_jump)
  0x00211AC3  8b4a08                  mov      ecx, dword ptr [edx + 8]       
  0x00211AC6  57                      push     edi                            
  0x00211AC7  8b7c2414                mov      edi, dword ptr [esp + 0x14]    
  0x00211ACB  890f                    mov      dword ptr [edi], ecx           
  0x00211ACD  0fb68aa3000000          movzx    ecx, byte ptr [edx + 0xa3]     
  0x00211AD4  8d0c49                  lea      ecx, [ecx + ecx*2]             
  0x00211AD7  8b0ccdcc082100          mov      ecx, dword ptr [ecx*8 + 0x2108cc] 
  0x00211ADE  0fb609                  movzx    ecx, byte ptr [ecx]            
  0x00211AE1  8d7214                  lea      esi, [edx + 0x14]              
  0x00211AE4  8bd1                    mov      edx, ecx                       
  0x00211AE6  83c704                  add      edi, 4                         
  0x00211AE9  c1e902                  shr      ecx, 2                         
  0x00211AEC  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x00211AEE  8bca                    mov      ecx, edx                       
  0x00211AF0  83e103                  and      ecx, 3                         
  0x00211AF3  f3a4                    rep movsb byte ptr es:[edi], byte ptr [esi] 
  0x00211AF5  8bf3                    mov      esi, ebx                       
  0x00211AF7  5f                      pop      edi                            
                                        ; XREF: 0x00211AB0 (jump)
  0x00211AF8  8ac8                    mov      cl, al                         
  0x00211AFA  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00211B00  8bc6                    mov      eax, esi                       
  0x00211B02  5e                      pop      esi                            
  0x00211B03  5b                      pop      ebx                            
  0x00211B04  c20800                  ret      8                              

; ============================================================
; Function: sub_00211B07
; Start: 0x00211B07  End: 0x00211B1B  Size: 20 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_000A50A0
; ============================================================
sub_00211B07:
  0x00211B07  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x00211B0B  8d81a3000000            lea      eax, [ecx + 0xa3]              
  0x00211B11  803801                  cmp      byte ptr [eax], 1              
  0x00211B14  7505                    jne      0x211b1b                       
  0x00211B16  6a57                    push     0x57                           
  0x00211B18  58                      pop      eax                            
  0x00211B19  eb21                    jmp      0x211b3c                       
; end of function
                                        ; XREF: 0x00211B14 (cond_jump)
  0x00211B1B  8b542408                mov      edx, dword ptr [esp + 8]       
  0x00211B1F  80624000                and      byte ptr [edx + 0x40], 0       
  0x00211B23  0fb600                  movzx    eax, byte ptr [eax]            
  0x00211B26  8d0440                  lea      eax, [eax + eax*2]             
  0x00211B29  8b04c5d0082100          mov      eax, dword ptr [eax*8 + 0x2108d0] 
  0x00211B30  8a00                    mov      al, byte ptr [eax]             
  0x00211B32  0402                    add      al, 2                          
  0x00211B34  884241                  mov      byte ptr [edx + 0x41], al      
  0x00211B37  e8a9060000              call     0x2121e5                       ; -> sub_002121E5
                                        ; XREF: 0x00211B19 (jump)
  0x00211B3C  c20800                  ret      8                              

; ============================================================
; Function: sub_00211B3F
; Start: 0x00211B3F  End: 0x00211B5A  Size: 27 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00212311
; ============================================================
sub_00211B3F:
  0x00211B3F  6858a14000              push     0x40a158                       
  0x00211B44  83c9ff                  or       ecx, 0xffffffff                
  0x00211B47  51                      push     ecx                            
  0x00211B48  b8800f05fd              mov      eax, 0xfd050f80                
  0x00211B4D  50                      push     eax                            
  0x00211B4E  6830a14000              push     0x40a130                       
  0x00211B53  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x00211B59  c3                      ret                                     
; end of function
  0x00211B5A  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x00211B5E  68f8a04000              push     0x40a0f8                       
  0x00211B63  e8ff260000              call     0x214267                       ; -> sub_00214267
  0x00211B68  c21000                  ret      0x10                           

; ============================================================
; Function: sub_00211B6B
; Start: 0x00211B6B  End: 0x00211B8E  Size: 35 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002137C4, sub_0021427E
; ============================================================
sub_00211B6B:
  0x00211B6B  56                      push     esi                            
  0x00211B6C  8bf1                    mov      esi, ecx                       
  0x00211B6E  8b0e                    mov      ecx, dword ptr [esi]           
  0x00211B70  6a00                    push     0                              
  0x00211B72  e807270000              call     0x21427e                       ; -> sub_0021427E
  0x00211B77  8b0e                    mov      ecx, dword ptr [esi]           
  0x00211B79  e8461c0000              call     0x2137c4                       ; -> sub_002137C4
  0x00211B7E  832600                  and      dword ptr [esi], 0             
  0x00211B81  806604fe                and      byte ptr [esi + 4], 0xfe       
  0x00211B85  66ff0ddaa04000          dec      word ptr [0x40a0da]            
  0x00211B8C  5e                      pop      esi                            
  0x00211B8D  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00211B8E
; Start: 0x00211B8E  End: 0x00211BFC  Size: 110 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002143EF
; Called by: sub_00212488
; ============================================================
sub_00211B8E:
  0x00211B8E  51                      push     ecx                            
  0x00211B8F  53                      push     ebx                            
  0x00211B90  55                      push     ebp                            
  0x00211B91  57                      push     edi                            
  0x00211B92  33db                    xor      ebx, ebx                       
  0x00211B94  33ff                    xor      edi, edi                       
  0x00211B96  66391dd8a04000          cmp      word ptr [0x40a0d8], bx        
  0x00211B9D  8bea                    mov      ebp, edx                       
  0x00211B9F  884c240c                mov      byte ptr [esp + 0xc], cl       
  0x00211BA3  7650                    jbe      0x211bf5                       
  0x00211BA5  56                      push     esi                            
                                        ; XREF: 0x00211BF2 (cond_jump)
  0x00211BA6  0fb6c3                  movzx    eax, bl                        
  0x00211BA9  8d34c0                  lea      esi, [eax + eax*8]             
  0x00211BAC  a1dca04000              mov      eax, dword ptr [0x40a0dc]      
  0x00211BB1  d1e6                    shl      esi, 1                         
  0x00211BB3  03c6                    add      eax, esi                       
  0x00211BB5  f6400401                test     byte ptr [eax + 4], 1          
  0x00211BB9  742a                    je       0x211be5                       
  0x00211BBB  8b08                    mov      ecx, dword ptr [eax]           
  0x00211BBD  e82d280000              call     0x2143ef                       ; -> sub_002143EF
  0x00211BC2  3bc5                    cmp      eax, ebp                       
  0x00211BC4  751f                    jne      0x211be5                       
  0x00211BC6  a1dca04000              mov      eax, dword ptr [0x40a0dc]      
  0x00211BCB  8a4c2410                mov      cl, byte ptr [esp + 0x10]      
  0x00211BCF  03c6                    add      eax, esi                       
  0x00211BD1  38480a                  cmp      byte ptr [eax + 0xa], cl       
  0x00211BD4  750f                    jne      0x211be5                       
  0x00211BD6  8a4804                  mov      cl, byte ptr [eax + 4]         
  0x00211BD9  f6c108                  test     cl, 8                          
  0x00211BDC  7407                    je       0x211be5                       
  0x00211BDE  f6c102                  test     cl, 2                          
  0x00211BE1  7502                    jne      0x211be5                       
  0x00211BE3  8bf8                    mov      edi, eax                       
                                        ; XREF: 0x00211BB9 (cond_jump), 0x00211BC4 (cond_jump), 0x00211BD4 (cond_jump), 0x00211BDC (cond_jump), 0x00211BE1 (cond_jump)
  0x00211BE5  fec3                    inc      bl                             
  0x00211BE7  660fb6c3                movzx    ax, bl                         
  0x00211BEB  663b05d8a04000          cmp      ax, word ptr [0x40a0d8]        
  0x00211BF2  72b2                    jb       0x211ba6                       
  0x00211BF4  5e                      pop      esi                            
                                        ; XREF: 0x00211BA3 (cond_jump)
  0x00211BF5  8bc7                    mov      eax, edi                       
  0x00211BF7  5f                      pop      edi                            
  0x00211BF8  5d                      pop      ebp                            
  0x00211BF9  5b                      pop      ebx                            
  0x00211BFA  59                      pop      ecx                            
  0x00211BFB  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00211BFC
; Start: 0x00211BFC  End: 0x00211CA7  Size: 171 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00214621
; ============================================================
sub_00211BFC:
  0x00211BFC  55                      push     ebp                            
  0x00211BFD  8bec                    mov      ebp, esp                       
  0x00211BFF  51                      push     ecx                            
  0x00211C00  53                      push     ebx                            
  0x00211C01  56                      push     esi                            
  0x00211C02  8bf1                    mov      esi, ecx                       
  0x00211C04  83665a00                and      dword ptr [esi + 0x5a], 0      
  0x00211C08  57                      push     edi                            
  0x00211C09  8b3e                    mov      edi, dword ptr [esi]           
  0x00211C0B  8d5e52                  lea      ebx, [esi + 0x52]              
  0x00211C0E  c60320                  mov      byte ptr [ebx], 0x20           
  0x00211C11  c6465382                mov      byte ptr [esi + 0x53], 0x82    
  0x00211C15  8b0f                    mov      ecx, dword ptr [edi]           
  0x00211C17  53                      push     ebx                            
  0x00211C18  8955fc                  mov      dword ptr [ebp - 4], edx       
  0x00211C1B  e8012a0000              call     0x214621                       ; -> sub_00214621
  0x00211C20  85c0                    test     eax, eax                       
  0x00211C22  7c7e                    jl       0x211ca2                       
  0x00211C24  808ea200000002          or       byte ptr [esi + 0xa2], 2       
  0x00211C2B  83665a00                and      dword ptr [esi + 0x5a], 0      
  0x00211C2F  c60320                  mov      byte ptr [ebx], 0x20           
  0x00211C32  c6465302                mov      byte ptr [esi + 0x53], 2       
  0x00211C36  8a4708                  mov      al, byte ptr [edi + 8]         
  0x00211C39  884667                  mov      byte ptr [esi + 0x67], al      
  0x00211C3C  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00211C3F  c6466803                mov      byte ptr [esi + 0x68], 3       
  0x00211C43  8a4001                  mov      al, byte ptr [eax + 1]         
  0x00211C46  884669                  mov      byte ptr [esi + 0x69], al      
  0x00211C49  66c7466e2000            mov      word ptr [esi + 0x6e], 0x20    
  0x00211C4F  8b0f                    mov      ecx, dword ptr [edi]           
  0x00211C51  53                      push     ebx                            
  0x00211C52  e8ca290000              call     0x214621                       ; -> sub_00214621
  0x00211C57  85c0                    test     eax, eax                       
  0x00211C59  7c47                    jl       0x211ca2                       
  0x00211C5B  8b4e62                  mov      ecx, dword ptr [esi + 0x62]    
  0x00211C5E  894e0c                  mov      dword ptr [esi + 0xc], ecx     
  0x00211C61  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x00211C64  f60102                  test     byte ptr [ecx], 2              
  0x00211C67  7439                    je       0x211ca2                       
  0x00211C69  807f0900                cmp      byte ptr [edi + 9], 0          
  0x00211C6D  7433                    je       0x211ca2                       
  0x00211C6F  83665a00                and      dword ptr [esi + 0x5a], 0      
  0x00211C73  c60320                  mov      byte ptr [ebx], 0x20           
  0x00211C76  c6465302                mov      byte ptr [esi + 0x53], 2       
  0x00211C7A  8a4709                  mov      al, byte ptr [edi + 9]         
  0x00211C7D  884667                  mov      byte ptr [esi + 0x67], al      
  0x00211C80  c6466803                mov      byte ptr [esi + 0x68], 3       
  0x00211C84  8a4102                  mov      al, byte ptr [ecx + 2]         
  0x00211C87  884669                  mov      byte ptr [esi + 0x69], al      
  0x00211C8A  66c7466e2000            mov      word ptr [esi + 0x6e], 0x20    
  0x00211C90  8b0f                    mov      ecx, dword ptr [edi]           
  0x00211C92  53                      push     ebx                            
  0x00211C93  e889290000              call     0x214621                       ; -> sub_00214621
  0x00211C98  85c0                    test     eax, eax                       
  0x00211C9A  7c06                    jl       0x211ca2                       
  0x00211C9C  8b4e62                  mov      ecx, dword ptr [esi + 0x62]    
  0x00211C9F  894e10                  mov      dword ptr [esi + 0x10], ecx    
                                        ; XREF: 0x00211C22 (cond_jump), 0x00211C59 (cond_jump), 0x00211C67 (cond_jump), 0x00211C6D (cond_jump), 0x00211C9A (cond_jump)
  0x00211CA2  5f                      pop      edi                            
  0x00211CA3  5e                      pop      esi                            
  0x00211CA4  5b                      pop      ebx                            
  0x00211CA5  c9                      leave                                   
  0x00211CA6  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00211CA7
; Start: 0x00211CA7  End: 0x00211CD8  Size: 49 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00211FD4
; ============================================================
sub_00211CA7:
  0x00211CA7  56                      push     esi                            
  0x00211CA8  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00211CAC  f686a200000002          test     byte ptr [esi + 0xa2], 2       
  0x00211CB3  8b06                    mov      eax, dword ptr [esi]           
  0x00211CB5  8b08                    mov      ecx, dword ptr [eax]           
  0x00211CB7  57                      push     edi                            
  0x00211CB8  741e                    je       0x211cd8                       
  0x00211CBA  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x00211CBE  c6001c                  mov      byte ptr [eax], 0x1c           
  0x00211CC1  c64001c3                mov      byte ptr [eax + 1], 0xc3       
  0x00211CC5  c74008a71c2100          mov      dword ptr [eax + 8], 0x211ca7  
  0x00211CCC  89700c                  mov      dword ptr [eax + 0xc], esi     
  0x00211CCF  80a6a2000000fd          and      byte ptr [esi + 0xa2], 0xfd    
  0x00211CD6  eb4a                    jmp      0x211d22                       
; end of function
                                        ; XREF: 0x00211CB8 (cond_jump)
  0x00211CD8  33ff                    xor      edi, edi                       
  0x00211CDA  397e0c                  cmp      dword ptr [esi + 0xc], edi     
  0x00211CDD  7420                    je       0x211cff                       
  0x00211CDF  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x00211CE3  c6001c                  mov      byte ptr [eax], 0x1c           
  0x00211CE6  c6400143                mov      byte ptr [eax + 1], 0x43       
  0x00211CEA  c74008a71c2100          mov      dword ptr [eax + 8], 0x211ca7  
  0x00211CF1  89700c                  mov      dword ptr [eax + 0xc], esi     
  0x00211CF4  8b560c                  mov      edx, dword ptr [esi + 0xc]     
  0x00211CF7  895010                  mov      dword ptr [eax + 0x10], edx    
  0x00211CFA  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x00211CFD  eb23                    jmp      0x211d22                       
                                        ; XREF: 0x00211CDD (cond_jump)
  0x00211CFF  397e10                  cmp      dword ptr [esi + 0x10], edi    
  0x00211D02  7426                    je       0x211d2a                       
  0x00211D04  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x00211D08  c6001c                  mov      byte ptr [eax], 0x1c           
  0x00211D0B  c6400143                mov      byte ptr [eax + 1], 0x43       
  0x00211D0F  c74008a71c2100          mov      dword ptr [eax + 8], 0x211ca7  
  0x00211D16  89700c                  mov      dword ptr [eax + 0xc], esi     
  0x00211D19  8b5610                  mov      edx, dword ptr [esi + 0x10]    
  0x00211D1C  895010                  mov      dword ptr [eax + 0x10], edx    
  0x00211D1F  897e10                  mov      dword ptr [esi + 0x10], edi    
                                        ; XREF: 0x00211CD6 (jump), 0x00211CFD (jump)
  0x00211D22  50                      push     eax                            
  0x00211D23  e8f9280000              call     0x214621                       ; -> sub_00214621
  0x00211D28  eb29                    jmp      0x211d53                       
                                        ; XREF: 0x00211D02 (cond_jump)
  0x00211D2A  89780e                  mov      dword ptr [eax + 0xe], edi     
  0x00211D2D  893e                    mov      dword ptr [esi], edi           
  0x00211D2F  f6400402                test     byte ptr [eax + 4], 2          
  0x00211D33  7407                    je       0x211d3c                       
  0x00211D35  8bc8                    mov      ecx, eax                       
  0x00211D37  e82ffeffff              call     0x211b6b                       ; -> sub_00211B6B
                                        ; XREF: 0x00211D33 (cond_jump)
  0x00211D3C  f686a200000001          test     byte ptr [esi + 0xa2], 1       
  0x00211D43  740e                    je       0x211d53                       
  0x00211D45  57                      push     edi                            
  0x00211D46  57                      push     edi                            
  0x00211D47  ffb69e000000            push     dword ptr [esi + 0x9e]         
  0x00211D4D  ff150c7b2100            call     dword ptr [0x217b0c]           ; -> xbox_KeSetEvent
                                        ; XREF: 0x00211D28 (jump), 0x00211D43 (cond_jump)
  0x00211D53  5f                      pop      edi                            
  0x00211D54  5e                      pop      esi                            
  0x00211D55  c20800                  ret      8                              
  0x00211D58  53                      push     ebx                            
  0x00211D59  57                      push     edi                            
  0x00211D5A  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00211D5E  f687a200000001          test     byte ptr [edi + 0xa2], 1       
  0x00211D65  8b1f                    mov      ebx, dword ptr [edi]           
  0x00211D67  0f8583000000            jne      0x211df0                       
  0x00211D6D  f6430402                test     byte ptr [ebx + 4], 2          
  0x00211D71  757d                    jne      0x211df0                       
  0x00211D73  56                      push     esi                            
  0x00211D74  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00211D78  33c0                    xor      eax, eax                       
  0x00211D7A  394604                  cmp      dword ptr [esi + 4], eax       
  0x00211D7D  7c29                    jl       0x211da8                       
  0x00211D7F  0fb6430a                movzx    eax, byte ptr [ebx + 0xa]      
  0x00211D83  8d0440                  lea      eax, [eax + eax*2]             
  0x00211D86  8bcf                    mov      ecx, edi                       
  0x00211D88  ff14c5d8082100          call     dword ptr [eax*8 + 0x2108d8]   
  0x00211D8F  ff4708                  inc      dword ptr [edi + 8]            
  0x00211D92  83670400                and      dword ptr [edi + 4], 0         
  0x00211D96  0fb6430c                movzx    eax, byte ptr [ebx + 0xc]      
  0x00211D9A  894614                  mov      dword ptr [esi + 0x14], eax    
  0x00211D9D  f687a200000008          test     byte ptr [edi + 0xa2], 8       
  0x00211DA4  7449                    je       0x211def                       
  0x00211DA6  eb3f                    jmp      0x211de7                       
                                        ; XREF: 0x00211D7D (cond_jump)
  0x00211DA8  80661c00                and      byte ptr [esi + 0x1c], 0       
  0x00211DAC  80661d00                and      byte ptr [esi + 0x1d], 0       
  0x00211DB0  80661e00                and      byte ptr [esi + 0x1e], 0       
  0x00211DB4  c60630                  mov      byte ptr [esi], 0x30           
  0x00211DB7  c6460140                mov      byte ptr [esi + 1], 0x40       
  0x00211DBB  c74608f51d2100          mov      dword ptr [esi + 8], 0x211df5  
  0x00211DC2  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x00211DC5  894610                  mov      dword ptr [esi + 0x10], eax    
  0x00211DC8  894618                  mov      dword ptr [esi + 0x18], eax    
  0x00211DCB  894614                  mov      dword ptr [esi + 0x14], eax    
  0x00211DCE  c6462802                mov      byte ptr [esi + 0x28], 2       
  0x00211DD2  c6462901                mov      byte ptr [esi + 0x29], 1       
  0x00211DD6  6689462a                mov      word ptr [esi + 0x2a], ax      
  0x00211DDA  660fb64b08              movzx    cx, byte ptr [ebx + 8]         
  0x00211DDF  66894e2c                mov      word ptr [esi + 0x2c], cx      
  0x00211DE3  6689462e                mov      word ptr [esi + 0x2e], ax      
                                        ; XREF: 0x00211DA6 (jump)
  0x00211DE7  8b0b                    mov      ecx, dword ptr [ebx]           
  0x00211DE9  56                      push     esi                            
  0x00211DEA  e832280000              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x00211DA4 (cond_jump)
  0x00211DEF  5e                      pop      esi                            
                                        ; XREF: 0x00211D67 (cond_jump), 0x00211D71 (cond_jump)
  0x00211DF0  5f                      pop      edi                            
  0x00211DF1  5b                      pop      ebx                            
  0x00211DF2  c20800                  ret      8                              
  0x00211DF5  53                      push     ebx                            
  0x00211DF6  56                      push     esi                            
  0x00211DF7  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00211DFB  8b1e                    mov      ebx, dword ptr [esi]           
  0x00211DFD  f6430402                test     byte ptr [ebx + 4], 2          
  0x00211E01  7577                    jne      0x211e7a                       
  0x00211E03  f686a200000001          test     byte ptr [esi + 0xa2], 1       
  0x00211E0A  756e                    jne      0x211e7a                       
  0x00211E0C  57                      push     edi                            
  0x00211E0D  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00211E11  837f0400                cmp      dword ptr [edi + 4], 0         
  0x00211E15  7c5b                    jl       0x211e72                       
  0x00211E17  83670800                and      dword ptr [edi + 8], 0         
  0x00211E1B  c60718                  mov      byte ptr [edi], 0x18           
  0x00211E1E  c6470105                mov      byte ptr [edi + 1], 5          
  0x00211E22  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00211E25  894710                  mov      dword ptr [edi + 0x10], eax    
  0x00211E28  c7471404000000          mov      dword ptr [edi + 0x14], 4      
  0x00211E2F  8b0b                    mov      ecx, dword ptr [ebx]           
  0x00211E31  57                      push     edi                            
  0x00211E32  e8ea270000              call     0x214621                       ; -> sub_00214621
  0x00211E37  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00211E3A  894662                  mov      dword ptr [esi + 0x62], eax    
  0x00211E3D  8d4632                  lea      eax, [esi + 0x32]              
  0x00211E40  c6465228                mov      byte ptr [esi + 0x52], 0x28    
  0x00211E44  c6465341                mov      byte ptr [esi + 0x53], 0x41    
  0x00211E48  c7465a581d2100          mov      dword ptr [esi + 0x5a], 0x211d58 
  0x00211E4F  89765e                  mov      dword ptr [esi + 0x5e], esi    
  0x00211E52  89466a                  mov      dword ptr [esi + 0x6a], eax    
  0x00211E55  0fb6430c                movzx    eax, byte ptr [ebx + 0xc]      
  0x00211E59  80667000                and      byte ptr [esi + 0x70], 0       
  0x00211E5D  894666                  mov      dword ptr [esi + 0x66], eax    
  0x00211E60  c6466e02                mov      byte ptr [esi + 0x6e], 2       
  0x00211E64  c6466f01                mov      byte ptr [esi + 0x6f], 1       
  0x00211E68  8b0b                    mov      ecx, dword ptr [ebx]           
  0x00211E6A  57                      push     edi                            
  0x00211E6B  e8b1270000              call     0x214621                       ; -> sub_00214621
  0x00211E70  eb07                    jmp      0x211e79                       
                                        ; XREF: 0x00211E15 (cond_jump)
  0x00211E72  8b0b                    mov      ecx, dword ptr [ebx]           
  0x00211E74  e82b1b0000              call     0x2139a4                       ; -> sub_002139A4
                                        ; XREF: 0x00211E70 (jump)
  0x00211E79  5f                      pop      edi                            
                                        ; XREF: 0x00211E01 (cond_jump), 0x00211E0A (cond_jump)
  0x00211E7A  5e                      pop      esi                            
  0x00211E7B  5b                      pop      ebx                            
  0x00211E7C  c20800                  ret      8                              
  0x00211E7F  8bc1                    mov      eax, ecx                       
  0x00211E81  8b4866                  mov      ecx, dword ptr [eax + 0x66]    
  0x00211E84  83f902                  cmp      ecx, 2                         
  0x00211E87  8d5014                  lea      edx, [eax + 0x14]              
  0x00211E8A  722e                    jb       0x211eba                       
  0x00211E8C  83c1fe                  add      ecx, -2                        
  0x00211E8F  56                      push     esi                            
  0x00211E90  57                      push     edi                            
  0x00211E91  8d7034                  lea      esi, [eax + 0x34]              
  0x00211E94  8bc1                    mov      eax, ecx                       
  0x00211E96  c1e902                  shr      ecx, 2                         
  0x00211E99  8bfa                    mov      edi, edx                       
  0x00211E9B  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x00211E9D  8bc8                    mov      ecx, eax                       
  0x00211E9F  83e103                  and      ecx, 3                         
  0x00211EA2  f3a4                    rep movsb byte ptr es:[edi], byte ptr [esi] 
  0x00211EA4  f6023f                  test     byte ptr [edx], 0x3f           
  0x00211EA7  5f                      pop      edi                            
  0x00211EA8  5e                      pop      esi                            
  0x00211EA9  7510                    jne      0x211ebb                       
  0x00211EAB  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00211EB8 (cond_jump)
  0x00211EAD  807c020200              cmp      byte ptr [edx + eax + 2], 0    
  0x00211EB2  7507                    jne      0x211ebb                       
  0x00211EB4  40                      inc      eax                            
  0x00211EB5  83f807                  cmp      eax, 7                         
  0x00211EB8  7ef3                    jle      0x211ead                       
                                        ; XREF: 0x00211E8A (cond_jump)
  0x00211EBA  c3                      ret                                     
                                        ; XREF: 0x00211EA9 (cond_jump), 0x00211EB2 (cond_jump)
  0x00211EBB  e946def7ff              jmp      0x18fd06                       ; -> sub_0018FD06
  0x00211EC0  8bc1                    mov      eax, ecx                       
  0x00211EC2  8b4866                  mov      ecx, dword ptr [eax + 0x66]    
  0x00211EC5  49                      dec      ecx                            
  0x00211EC6  56                      push     esi                            
  0x00211EC7  49                      dec      ecx                            
  0x00211EC8  57                      push     edi                            
  0x00211EC9  8d7034                  lea      esi, [eax + 0x34]              
  0x00211ECC  8d7814                  lea      edi, [eax + 0x14]              
  0x00211ECF  8bc1                    mov      eax, ecx                       
  0x00211ED1  c1e902                  shr      ecx, 2                         
  0x00211ED4  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x00211ED6  8bc8                    mov      ecx, eax                       
  0x00211ED8  83e103                  and      ecx, 3                         
  0x00211EDB  f3a4                    rep movsb byte ptr es:[edi], byte ptr [esi] 
  0x00211EDD  5f                      pop      edi                            
  0x00211EDE  5e                      pop      esi                            
  0x00211EDF  e922def7ff              jmp      0x18fd06                       ; -> sub_0018FD06
  0x00211EE4  8b5132                  mov      edx, dword ptr [ecx + 0x32]    
  0x00211EE7  8d4114                  lea      eax, [ecx + 0x14]              
  0x00211EEA  8910                    mov      dword ptr [eax], edx           
  0x00211EEC  8b5136                  mov      edx, dword ptr [ecx + 0x36]    
  0x00211EEF  895004                  mov      dword ptr [eax + 4], edx       
  0x00211EF2  8b15fc072100            mov      edx, dword ptr [0x2107fc]      
  0x00211EF8  85d2                    test     edx, edx                       
  0x00211EFA  740a                    je       0x211f06                       
  0x00211EFC  50                      push     eax                            
  0x00211EFD  51                      push     ecx                            
  0x00211EFE  ff520c                  call     dword ptr [edx + 0xc]          
  0x00211F01  e900def7ff              jmp      0x18fd06                       ; -> sub_0018FD06
                                        ; XREF: 0x00211EFA (cond_jump)
  0x00211F06  c3                      ret                                     

; ============================================================
; Function: sub_00211F07
; Start: 0x00211F07  End: 0x00211F31  Size: 42 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00214299
; ============================================================
sub_00211F07:
  0x00211F07  56                      push     esi                            
  0x00211F08  57                      push     edi                            
  0x00211F09  ff7104                  push     dword ptr [ecx + 4]            
  0x00211F0C  8bf2                    mov      esi, edx                       
  0x00211F0E  8b7e0c                  mov      edi, dword ptr [esi + 0xc]     
  0x00211F11  e883230000              call     0x214299                       ; -> sub_00214299
  0x00211F16  85ff                    test     edi, edi                       
  0x00211F18  8906                    mov      dword ptr [esi], eax           
  0x00211F1A  7415                    je       0x211f31                       
  0x00211F1C  6a00                    push     0                              
  0x00211F1E  6a00                    push     0                              
  0x00211F20  57                      push     edi                            
  0x00211F21  ff150c7b2100            call     dword ptr [0x217b0c]           ; -> xbox_KeSetEvent
  0x00211F27  8bcf                    mov      ecx, edi                       
  0x00211F29  5f                      pop      edi                            
  0x00211F2A  5e                      pop      esi                            
  0x00211F2B  ff25cc7a2100            jmp      dword ptr [0x217acc]           
; end of function
                                        ; XREF: 0x00211F1A (cond_jump)
  0x00211F31  5f                      pop      edi                            
  0x00211F32  5e                      pop      esi                            
  0x00211F33  c3                      ret                                     
  0x00211F34  56                      push     esi                            
  0x00211F35  57                      push     edi                            
  0x00211F36  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00211F3A  8b4708                  mov      eax, dword ptr [edi + 8]       
  0x00211F3D  f680a200000001          test     byte ptr [eax + 0xa2], 1       
  0x00211F44  8b08                    mov      ecx, dword ptr [eax]           
  0x00211F46  7539                    jne      0x211f81                       
  0x00211F48  f6410402                test     byte ptr [ecx + 4], 2          
  0x00211F4C  7533                    jne      0x211f81                       
  0x00211F4E  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00211F52  837e0400                cmp      dword ptr [esi + 4], 0         
  0x00211F56  7c20                    jl       0x211f78                       
  0x00211F58  83660800                and      dword ptr [esi + 8], 0         
  0x00211F5C  c60618                  mov      byte ptr [esi], 0x18           
  0x00211F5F  c6460105                mov      byte ptr [esi + 1], 5          
  0x00211F63  8b4010                  mov      eax, dword ptr [eax + 0x10]    
  0x00211F66  894610                  mov      dword ptr [esi + 0x10], eax    
  0x00211F69  c7461404000000          mov      dword ptr [esi + 0x14], 4      
  0x00211F70  8b09                    mov      ecx, dword ptr [ecx]           
  0x00211F72  56                      push     esi                            
  0x00211F73  e8a9260000              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x00211F56 (cond_jump)
  0x00211F78  c74604040000c0          mov      dword ptr [esi + 4], 0xc0000004 
  0x00211F7F  eb0b                    jmp      0x211f8c                       
                                        ; XREF: 0x00211F46 (cond_jump), 0x00211F4C (cond_jump)
  0x00211F81  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00211F85  c7460400070080          mov      dword ptr [esi + 4], 0x80000700 
                                        ; XREF: 0x00211F7F (jump)
  0x00211F8C  8bd7                    mov      edx, edi                       
  0x00211F8E  8bce                    mov      ecx, esi                       
  0x00211F90  e872ffffff              call     0x211f07                       ; -> sub_00211F07
  0x00211F95  5f                      pop      edi                            
  0x00211F96  5e                      pop      esi                            
  0x00211F97  c20800                  ret      8                              
  0x00211F9A  56                      push     esi                            
  0x00211F9B  6830a14000              push     0x40a130                       
  0x00211FA0  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00211FA6  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00211FAA  8b0e                    mov      ecx, dword ptr [esi]           
  0x00211FAC  80660b00                and      byte ptr [esi + 0xb], 0        
  0x00211FB0  6a01                    push     1                              
  0x00211FB2  c6460a01                mov      byte ptr [esi + 0xa], 1        
  0x00211FB6  c6460c08                mov      byte ptr [esi + 0xc], 8        
  0x00211FBA  c6460d01                mov      byte ptr [esi + 0xd], 1        
  0x00211FBE  e8cc220000              call     0x21428f                       ; -> sub_0021428F
  0x00211FC3  8b0e                    mov      ecx, dword ptr [esi]           
  0x00211FC5  6a00                    push     0                              
  0x00211FC7  e8b61b0000              call     0x213b82                       ; -> sub_00213B82
  0x00211FCC  804e0408                or       byte ptr [esi + 4], 8          
  0x00211FD0  5e                      pop      esi                            
  0x00211FD1  c20800                  ret      8                              

; ============================================================
; Function: sub_00211FD4
; Start: 0x00211FD4  End: 0x00211FF2  Size: 30 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00211CA7
; Called by: sub_00212140
; ============================================================
sub_00211FD4:
  0x00211FD4  8d91a2000000            lea      edx, [ecx + 0xa2]              
  0x00211FDA  8a02                    mov      al, byte ptr [edx]             
  0x00211FDC  a804                    test     al, 4                          
  0x00211FDE  7511                    jne      0x211ff1                       
  0x00211FE0  51                      push     ecx                            
  0x00211FE1  81c182000000            add      ecx, 0x82                      
  0x00211FE7  0c04                    or       al, 4                          
  0x00211FE9  51                      push     ecx                            
  0x00211FEA  8802                    mov      byte ptr [edx], al             
  0x00211FEC  e8b6fcffff              call     0x211ca7                       ; -> sub_00211CA7
                                        ; XREF: 0x00211FDE (cond_jump)
  0x00211FF1  c3                      ret                                     
; end of function
  0x00211FF2  8b542408                mov      edx, dword ptr [esp + 8]       
  0x00211FF6  8b4a08                  mov      ecx, dword ptr [edx + 8]       
  0x00211FF9  8b01                    mov      eax, dword ptr [ecx]           
  0x00211FFB  f681a200000001          test     byte ptr [ecx + 0xa2], 1       
  0x00212002  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x00212006  7506                    jne      0x21200e                       
  0x00212008  f6400402                test     byte ptr [eax + 4], 2          
  0x0021200C  7407                    je       0x212015                       
                                        ; XREF: 0x00212006 (cond_jump)
  0x0021200E  c7410400070080          mov      dword ptr [ecx + 4], 0x80000700 
                                        ; XREF: 0x0021200C (cond_jump)
  0x00212015  817904040000c0          cmp      dword ptr [ecx + 4], 0xc0000004 
  0x0021201C  7550                    jne      0x21206e                       
  0x0021201E  80790141                cmp      byte ptr [ecx + 1], 0x41       
  0x00212022  754a                    jne      0x21206e                       
  0x00212024  89510c                  mov      dword ptr [ecx + 0xc], edx     
  0x00212027  33d2                    xor      edx, edx                       
  0x00212029  56                      push     esi                            
  0x0021202A  c60130                  mov      byte ptr [ecx], 0x30           
  0x0021202D  c6410140                mov      byte ptr [ecx + 1], 0x40       
  0x00212031  c74108341f2100          mov      dword ptr [ecx + 8], 0x211f34  
  0x00212038  895110                  mov      dword ptr [ecx + 0x10], edx    
  0x0021203B  895118                  mov      dword ptr [ecx + 0x18], edx    
  0x0021203E  895114                  mov      dword ptr [ecx + 0x14], edx    
  0x00212041  88511c                  mov      byte ptr [ecx + 0x1c], dl      
  0x00212044  88511d                  mov      byte ptr [ecx + 0x1d], dl      
  0x00212047  88511e                  mov      byte ptr [ecx + 0x1e], dl      
  0x0021204A  c6412802                mov      byte ptr [ecx + 0x28], 2       
  0x0021204E  c6412901                mov      byte ptr [ecx + 0x29], 1       
  0x00212052  6689512a                mov      word ptr [ecx + 0x2a], dx      
  0x00212056  660fb67009              movzx    si, byte ptr [eax + 9]         
  0x0021205B  6689712c                mov      word ptr [ecx + 0x2c], si      
  0x0021205F  6689512e                mov      word ptr [ecx + 0x2e], dx      
  0x00212063  51                      push     ecx                            
  0x00212064  8b08                    mov      ecx, dword ptr [eax]           
  0x00212066  e8b6250000              call     0x214621                       ; -> sub_00214621
  0x0021206B  5e                      pop      esi                            
  0x0021206C  eb05                    jmp      0x212073                       
                                        ; XREF: 0x0021201C (cond_jump), 0x00212022 (cond_jump)
  0x0021206E  e894feffff              call     0x211f07                       ; -> sub_00211F07
                                        ; XREF: 0x0021206C (jump)
  0x00212073  c20800                  ret      8                              
  0x00212076  56                      push     esi                            
  0x00212077  6830a14000              push     0x40a130                       
  0x0021207C  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00212082  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00212086  33c0                    xor      eax, eax                       
  0x00212088  c605f8a0400030          mov      byte ptr [0x40a0f8], 0x30      
  0x0021208F  c605f9a0400040          mov      byte ptr [0x40a0f9], 0x40      
  0x00212096  c70500a140009a1f2100    mov      dword ptr [0x40a100], 0x211f9a 
  0x002120A0  893504a14000            mov      dword ptr [0x40a104], esi      
  0x002120A6  a308a14000              mov      dword ptr [0x40a108], eax      
  0x002120AB  a310a14000              mov      dword ptr [0x40a110], eax      
  0x002120B0  a30ca14000              mov      dword ptr [0x40a10c], eax      
  0x002120B5  a214a14000              mov      byte ptr [0x40a114], al        
  0x002120BA  c60515a1400001          mov      byte ptr [0x40a115], 1         
  0x002120C1  a216a14000              mov      byte ptr [0x40a116], al        
  0x002120C6  c60520a1400021          mov      byte ptr [0x40a120], 0x21      
  0x002120CD  c60521a140000a          mov      byte ptr [0x40a121], 0xa       
  0x002120D4  66a322a14000            mov      word ptr [0x40a122], ax        
  0x002120DA  660fb64e05              movzx    cx, byte ptr [esi + 5]         
  0x002120DF  66890d24a14000          mov      word ptr [0x40a124], cx        
  0x002120E6  66a326a14000            mov      word ptr [0x40a126], ax        
  0x002120EC  e84efaffff              call     0x211b3f                       ; -> sub_00211B3F
  0x002120F1  8b0e                    mov      ecx, dword ptr [esi]           
  0x002120F3  68f8a04000              push     0x40a0f8                       
  0x002120F8  e824250000              call     0x214621                       ; -> sub_00214621
  0x002120FD  5e                      pop      esi                            
  0x002120FE  c20800                  ret      8                              
  0x00212101  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x00212105  56                      push     esi                            
  0x00212106  e86f210000              call     0x21427a                       ; -> sub_0021427A
  0x0021210B  8bf0                    mov      esi, eax                       
  0x0021210D  8b4e0e                  mov      ecx, dword ptr [esi + 0xe]     
  0x00212110  804e0402                or       byte ptr [esi + 4], 2          
  0x00212114  85c9                    test     ecx, ecx                       
  0x00212116  741d                    je       0x212135                       
  0x00212118  807e0a01                cmp      byte ptr [esi + 0xa], 1        
  0x0021211C  750d                    jne      0x21212b                       
  0x0021211E  a1fc072100              mov      eax, dword ptr [0x2107fc]      
  0x00212123  85c0                    test     eax, eax                       
  0x00212125  7404                    je       0x21212b                       
  0x00212127  51                      push     ecx                            
  0x00212128  ff5008                  call     dword ptr [eax + 8]            
                                        ; XREF: 0x0021211C (cond_jump), 0x00212125 (cond_jump)
  0x0021212B  8b4e0e                  mov      ecx, dword ptr [esi + 0xe]     
  0x0021212E  e8a1feffff              call     0x211fd4                       ; -> sub_00211FD4
  0x00212133  eb07                    jmp      0x21213c                       
                                        ; XREF: 0x00212116 (cond_jump)
  0x00212135  8bce                    mov      ecx, esi                       
  0x00212137  e82ffaffff              call     0x211b6b                       ; -> sub_00211B6B
                                        ; XREF: 0x00212133 (jump)
  0x0021213C  5e                      pop      esi                            
  0x0021213D  c20400                  ret      4                              

; ============================================================
; Function: sub_00212140
; Start: 0x00212140  End: 0x002121B7  Size: 119 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00211FD4
; Called by: sub_00211898
; ============================================================
sub_00212140:
  0x00212140  55                      push     ebp                            
  0x00212141  8bec                    mov      ebp, esp                       
  0x00212143  83ec14                  sub      esp, 0x14                      
  0x00212146  53                      push     ebx                            
  0x00212147  56                      push     esi                            
  0x00212148  8bf1                    mov      esi, ecx                       
  0x0021214A  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00212150  33db                    xor      ebx, ebx                       
  0x00212152  391e                    cmp      dword ptr [esi], ebx           
  0x00212154  8ac8                    mov      cl, al                         
  0x00212156  884dff                  mov      byte ptr [ebp - 1], cl         
  0x00212159  745c                    je       0x2121b7                       
  0x0021215B  80bea300000001          cmp      byte ptr [esi + 0xa3], 1       
  0x00212162  750d                    jne      0x212171                       
  0x00212164  a1fc072100              mov      eax, dword ptr [0x2107fc]      
  0x00212169  3bc3                    cmp      eax, ebx                       
  0x0021216B  7404                    je       0x212171                       
  0x0021216D  56                      push     esi                            
  0x0021216E  ff5004                  call     dword ptr [eax + 4]            
                                        ; XREF: 0x00212162 (cond_jump), 0x0021216B (cond_jump)
  0x00212171  808ea200000001          or       byte ptr [esi + 0xa2], 1       
  0x00212178  8d45f4                  lea      eax, [ebp - 0xc]               
  0x0021217B  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x0021217E  8d45f4                  lea      eax, [ebp - 0xc]               
  0x00212181  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x00212184  8d45ec                  lea      eax, [ebp - 0x14]              
  0x00212187  8bce                    mov      ecx, esi                       
  0x00212189  885dec                  mov      byte ptr [ebp - 0x14], bl      
  0x0021218C  c645ee04                mov      byte ptr [ebp - 0x12], 4       
  0x00212190  895df0                  mov      dword ptr [ebp - 0x10], ebx    
  0x00212193  89869e000000            mov      dword ptr [esi + 0x9e], eax    
  0x00212199  e836feffff              call     0x211fd4                       ; -> sub_00211FD4
  0x0021219E  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x002121A1  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002121A7  53                      push     ebx                            
  0x002121A8  53                      push     ebx                            
  0x002121A9  53                      push     ebx                            
  0x002121AA  53                      push     ebx                            
  0x002121AB  8d45ec                  lea      eax, [ebp - 0x14]              
  0x002121AE  50                      push     eax                            
  0x002121AF  ff15107b2100            call     dword ptr [0x217b10]           ; -> xbox_KeWaitForSingleObject
  0x002121B5  eb06                    jmp      0x2121bd                       
; end of function
                                        ; XREF: 0x00212159 (cond_jump)
  0x002121B7  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
                                        ; XREF: 0x002121B5 (jump)
  0x002121BD  0fb686a3000000          movzx    eax, byte ptr [esi + 0xa3]     
  0x002121C4  8d0440                  lea      eax, [eax + eax*2]             
  0x002121C7  8d04c5c8082100          lea      eax, [eax*8 + 0x2108c8]        
  0x002121CE  fe00                    inc      byte ptr [eax]                 
  0x002121D0  a1e0a04000              mov      eax, dword ptr [0x40a0e0]      
  0x002121D5  8986a4000000            mov      dword ptr [esi + 0xa4], eax    
  0x002121DB  8935e0a04000            mov      dword ptr [0x40a0e0], esi      
  0x002121E1  5e                      pop      esi                            
  0x002121E2  5b                      pop      ebx                            
  0x002121E3  c9                      leave                                   
  0x002121E4  c3                      ret                                     

; ============================================================
; Function: sub_002121E5
; Start: 0x002121E5  End: 0x002122FB  Size: 278 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00214299, sub_00214621
; ============================================================
sub_002121E5:
  0x002121E5  55                      push     ebp                            
  0x002121E6  8bec                    mov      ebp, esp                       
  0x002121E8  51                      push     ecx                            
  0x002121E9  53                      push     ebx                            
  0x002121EA  56                      push     esi                            
  0x002121EB  8bd9                    mov      ebx, ecx                       
  0x002121ED  57                      push     edi                            
  0x002121EE  8b3b                    mov      edi, dword ptr [ebx]           
  0x002121F0  8bf2                    mov      esi, edx                       
  0x002121F2  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x002121F8  85ff                    test     edi, edi                       
  0x002121FA  8845ff                  mov      byte ptr [ebp - 1], al         
  0x002121FD  0f84f8000000            je       0x2122fb                       
  0x00212203  f6470402                test     byte ptr [edi + 4], 2          
  0x00212207  0f85ee000000            jne      0x2122fb                       
  0x0021220D  807f0d00                cmp      byte ptr [edi + 0xd], 0        
  0x00212211  750b                    jne      0x21221e                       
  0x00212213  c70632000000            mov      dword ptr [esi], 0x32          
  0x00212219  e9e3000000              jmp      0x212301                       
                                        ; XREF: 0x00212211 (cond_jump)
  0x0021221E  8b4604                  mov      eax, dword ptr [esi + 4]       
  0x00212221  85c0                    test     eax, eax                       
  0x00212223  7419                    je       0x21223e                       
  0x00212225  8d4e0c                  lea      ecx, [esi + 0xc]               
  0x00212228  51                      push     ecx                            
  0x00212229  ff35b47a2100            push     dword ptr [0x217ab4]           
  0x0021222F  50                      push     eax                            
  0x00212230  ff15d47a2100            call     dword ptr [0x217ad4]           ; -> xbox_ObReferenceObjectByHandle
  0x00212236  85c0                    test     eax, eax                       
  0x00212238  7d08                    jge      0x212242                       
  0x0021223A  83660400                and      dword ptr [esi + 4], 0         
                                        ; XREF: 0x00212223 (cond_jump)
  0x0021223E  83660c00                and      dword ptr [esi + 0xc], 0       
                                        ; XREF: 0x00212238 (cond_jump)
  0x00212242  8a470d                  mov      al, byte ptr [edi + 0xd]       
  0x00212245  3a4641                  cmp      al, byte ptr [esi + 0x41]      
  0x00212248  7303                    jae      0x21224d                       
  0x0021224A  884641                  mov      byte ptr [esi + 0x41], al      
                                        ; XREF: 0x00212248 (cond_jump)
  0x0021224D  0fb6470a                movzx    eax, byte ptr [edi + 0xa]      
  0x00212251  8d0440                  lea      eax, [eax + eax*2]             
  0x00212254  f604c5dc08210002        test     byte ptr [eax*8 + 0x2108dc], 2 
  0x0021225C  8d4e42                  lea      ecx, [esi + 0x42]              
  0x0021225F  7503                    jne      0x212264                       
  0x00212261  8d4e40                  lea      ecx, [esi + 0x40]              
                                        ; XREF: 0x0021225F (cond_jump)
  0x00212264  837b1000                cmp      dword ptr [ebx + 0x10], 0      
  0x00212268  8d4610                  lea      eax, [esi + 0x10]              
  0x0021226B  89761c                  mov      dword ptr [esi + 0x1c], esi    
  0x0021226E  c74618f21f2100          mov      dword ptr [esi + 0x18], 0x211ff2 
  0x00212275  7425                    je       0x21229c                       
  0x00212277  c60028                  mov      byte ptr [eax], 0x28           
  0x0021227A  c6461141                mov      byte ptr [esi + 0x11], 0x41    
  0x0021227E  8b5310                  mov      edx, dword ptr [ebx + 0x10]    
  0x00212281  80662d00                and      byte ptr [esi + 0x2d], 0       
  0x00212285  80662e00                and      byte ptr [esi + 0x2e], 0       
  0x00212289  894e28                  mov      dword ptr [esi + 0x28], ecx    
  0x0021228C  0fb64e41                movzx    ecx, byte ptr [esi + 0x41]     
  0x00212290  895620                  mov      dword ptr [esi + 0x20], edx    
  0x00212293  894e24                  mov      dword ptr [esi + 0x24], ecx    
  0x00212296  c6462c01                mov      byte ptr [esi + 0x2c], 1       
  0x0021229A  eb4a                    jmp      0x2122e6                       
                                        ; XREF: 0x00212275 (cond_jump)
  0x0021229C  83662000                and      dword ptr [esi + 0x20], 0      
  0x002122A0  80662d00                and      byte ptr [esi + 0x2d], 0       
  0x002122A4  80662e00                and      byte ptr [esi + 0x2e], 0       
  0x002122A8  894e28                  mov      dword ptr [esi + 0x28], ecx    
  0x002122AB  8a4e41                  mov      cl, byte ptr [esi + 0x41]      
  0x002122AE  0fb6d1                  movzx    edx, cl                        
  0x002122B1  895624                  mov      dword ptr [esi + 0x24], edx    
  0x002122B4  660fb65640              movzx    dx, byte ptr [esi + 0x40]      
  0x002122B9  6681ca0002              or       dx, 0x200                      
  0x002122BE  c60030                  mov      byte ptr [eax], 0x30           
  0x002122C1  c6461140                mov      byte ptr [esi + 0x11], 0x40    
  0x002122C5  c6462c01                mov      byte ptr [esi + 0x2c], 1       
  0x002122C9  c6463821                mov      byte ptr [esi + 0x38], 0x21    
  0x002122CD  c6463909                mov      byte ptr [esi + 0x39], 9       
  0x002122D1  6689563a                mov      word ptr [esi + 0x3a], dx      
  0x002122D5  660fb65705              movzx    dx, byte ptr [edi + 5]         
  0x002122DA  660fb6c9                movzx    cx, cl                         
  0x002122DE  6689563c                mov      word ptr [esi + 0x3c], dx      
  0x002122E2  66894e3e                mov      word ptr [esi + 0x3e], cx      
                                        ; XREF: 0x0021229A (jump)
  0x002122E6  895e08                  mov      dword ptr [esi + 8], ebx       
  0x002122E9  8b0f                    mov      ecx, dword ptr [edi]           
  0x002122EB  50                      push     eax                            
  0x002122EC  e830230000              call     0x214621                       ; -> sub_00214621
  0x002122F1  50                      push     eax                            
  0x002122F2  e8a21f0000              call     0x214299                       ; -> sub_00214299
  0x002122F7  8906                    mov      dword ptr [esi], eax           
  0x002122F9  eb06                    jmp      0x212301                       
; end of function
                                        ; XREF: 0x002121FD (cond_jump), 0x00212207 (cond_jump)
  0x002122FB  c7068f040000            mov      dword ptr [esi], 0x48f         
                                        ; XREF: 0x00212219 (jump), 0x002122F9 (jump)
  0x00212301  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x00212304  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x0021230A  8b06                    mov      eax, dword ptr [esi]           
  0x0021230C  5f                      pop      edi                            
  0x0021230D  5e                      pop      esi                            
  0x0021230E  5b                      pop      ebx                            
  0x0021230F  c9                      leave                                   
  0x00212310  c3                      ret                                     

; ============================================================
; Function: sub_00212311
; Start: 0x00212311  End: 0x002123AD  Size: 156 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00211B3F, sub_00214373, sub_00214621
; ============================================================
sub_00212311:
  0x00212311  53                      push     ebx                            
  0x00212312  56                      push     esi                            
  0x00212313  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00212317  8b0e                    mov      ecx, dword ptr [esi]           
  0x00212319  e855200000              call     0x214373                       ; -> sub_00214373
  0x0021231E  80780503                cmp      byte ptr [eax + 5], 3          
  0x00212322  0f8585000000            jne      0x2123ad                       
  0x00212328  80780701                cmp      byte ptr [eax + 7], 1          
  0x0021232C  757f                    jne      0x2123ad                       
  0x0021232E  33db                    xor      ebx, ebx                       
  0x00212330  c605f8a0400030          mov      byte ptr [0x40a0f8], 0x30      
  0x00212337  c605f9a0400040          mov      byte ptr [0x40a0f9], 0x40      
  0x0021233E  c70500a1400076202100    mov      dword ptr [0x40a100], 0x212076 
  0x00212348  893504a14000            mov      dword ptr [0x40a104], esi      
  0x0021234E  891d08a14000            mov      dword ptr [0x40a108], ebx      
  0x00212354  891d10a14000            mov      dword ptr [0x40a110], ebx      
  0x0021235A  891d0ca14000            mov      dword ptr [0x40a10c], ebx      
  0x00212360  881d14a14000            mov      byte ptr [0x40a114], bl        
  0x00212366  c60515a1400001          mov      byte ptr [0x40a115], 1         
  0x0021236D  881d16a14000            mov      byte ptr [0x40a116], bl        
  0x00212373  c60520a1400021          mov      byte ptr [0x40a120], 0x21      
  0x0021237A  c60521a140000b          mov      byte ptr [0x40a121], 0xb       
  0x00212381  66891d22a14000          mov      word ptr [0x40a122], bx        
  0x00212388  660fb64605              movzx    ax, byte ptr [esi + 5]         
  0x0021238D  66a324a14000            mov      word ptr [0x40a124], ax        
  0x00212393  66891d26a14000          mov      word ptr [0x40a126], bx        
  0x0021239A  e8a0f7ffff              call     0x211b3f                       ; -> sub_00211B3F
  0x0021239F  8b0e                    mov      ecx, dword ptr [esi]           
  0x002123A1  68f8a04000              push     0x40a0f8                       
  0x002123A6  e876220000              call     0x214621                       ; -> sub_00214621
  0x002123AB  eb23                    jmp      0x2123d0                       
; end of function
                                        ; XREF: 0x00212322 (cond_jump), 0x0021232C (cond_jump)
  0x002123AD  8b0e                    mov      ecx, dword ptr [esi]           
  0x002123AF  33db                    xor      ebx, ebx                       
  0x002123B1  53                      push     ebx                            
  0x002123B2  e8c71e0000              call     0x21427e                       ; -> sub_0021427E
  0x002123B7  8b0e                    mov      ecx, dword ptr [esi]           
  0x002123B9  6800040080              push     0x80000400                     
  0x002123BE  e8bf170000              call     0x213b82                       ; -> sub_00213B82
  0x002123C3  806604fe                and      byte ptr [esi + 4], 0xfe       
  0x002123C7  891e                    mov      dword ptr [esi], ebx           
  0x002123C9  66ff0ddaa04000          dec      word ptr [0x40a0da]            
                                        ; XREF: 0x002123AB (jump)
  0x002123D0  5e                      pop      esi                            
  0x002123D1  5b                      pop      ebx                            
  0x002123D2  c20800                  ret      8                              
  0x002123D5  53                      push     ebx                            
  0x002123D6  6830a14000              push     0x40a130                       
  0x002123DB  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x002123E1  8b442408                mov      eax, dword ptr [esp + 8]       
  0x002123E5  33db                    xor      ebx, ebx                       
  0x002123E7  395804                  cmp      dword ptr [eax + 4], ebx       
  0x002123EA  7d0f                    jge      0x2123fb                       
  0x002123EC  ff74240c                push     dword ptr [esp + 0xc]          
  0x002123F0  50                      push     eax                            
  0x002123F1  e81bffffff              call     0x212311                       ; -> sub_00212311
  0x002123F6  e989000000              jmp      0x212484                       
                                        ; XREF: 0x002123EA (cond_jump)
  0x002123FB  a0e8a04000              mov      al, byte ptr [0x40a0e8]        
  0x00212400  fec8                    dec      al                             
  0x00212402  56                      push     esi                            
  0x00212403  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00212407  88460a                  mov      byte ptr [esi + 0xa], al       
  0x0021240A  a0e9a04000              mov      al, byte ptr [0x40a0e9]        
  0x0021240F  8a4e0a                  mov      cl, byte ptr [esi + 0xa]       
  0x00212412  80f903                  cmp      cl, 3                          
  0x00212415  88460b                  mov      byte ptr [esi + 0xb], al       
  0x00212418  a0eaa04000              mov      al, byte ptr [0x40a0ea]        
  0x0021241D  88460c                  mov      byte ptr [esi + 0xc], al       
  0x00212420  a0eba04000              mov      al, byte ptr [0x40a0eb]        
  0x00212425  88460d                  mov      byte ptr [esi + 0xd], al       
  0x00212428  7338                    jae      0x212462                       
  0x0021242A  a0eaa04000              mov      al, byte ptr [0x40a0ea]        
  0x0021242F  3c02                    cmp      al, 2                          
  0x00212431  722f                    jb       0x212462                       
  0x00212433  3c20                    cmp      al, 0x20                       
  0x00212435  772b                    ja       0x212462                       
  0x00212437  8a460c                  mov      al, byte ptr [esi + 0xc]       
  0x0021243A  3a4606                  cmp      al, byte ptr [esi + 6]         
  0x0021243D  7723                    ja       0x212462                       
  0x0021243F  385e09                  cmp      byte ptr [esi + 9], bl         
  0x00212442  7408                    je       0x21244c                       
  0x00212444  8a460d                  mov      al, byte ptr [esi + 0xd]       
  0x00212447  3a4607                  cmp      al, byte ptr [esi + 7]         
  0x0021244A  7716                    ja       0x212462                       
                                        ; XREF: 0x00212442 (cond_jump)
  0x0021244C  51                      push     ecx                            
  0x0021244D  8b0e                    mov      ecx, dword ptr [esi]           
  0x0021244F  e83b1e0000              call     0x21428f                       ; -> sub_0021428F
  0x00212454  8b0e                    mov      ecx, dword ptr [esi]           
  0x00212456  53                      push     ebx                            
  0x00212457  e826170000              call     0x213b82                       ; -> sub_00213B82
  0x0021245C  804e0408                or       byte ptr [esi + 4], 8          
  0x00212460  eb21                    jmp      0x212483                       
                                        ; XREF: 0x00212428 (cond_jump), 0x00212431 (cond_jump), 0x00212435 (cond_jump), 0x0021243D (cond_jump), 0x0021244A (cond_jump)
  0x00212462  8b0e                    mov      ecx, dword ptr [esi]           
  0x00212464  53                      push     ebx                            
  0x00212465  e8141e0000              call     0x21427e                       ; -> sub_0021427E
  0x0021246A  8b0e                    mov      ecx, dword ptr [esi]           
  0x0021246C  6800040080              push     0x80000400                     
  0x00212471  e80c170000              call     0x213b82                       ; -> sub_00213B82
  0x00212476  806604fe                and      byte ptr [esi + 4], 0xfe       
  0x0021247A  891e                    mov      dword ptr [esi], ebx           
  0x0021247C  66ff0ddaa04000          dec      word ptr [0x40a0da]            
                                        ; XREF: 0x00212460 (jump)
  0x00212483  5e                      pop      esi                            
                                        ; XREF: 0x002123F6 (jump)
  0x00212484  5b                      pop      ebx                            
  0x00212485  c20800                  ret      8                              

; ============================================================
; Function: sub_00212488
; Start: 0x00212488  End: 0x002124CA  Size: 66 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00211B8E
; ============================================================
sub_00212488:
  0x00212488  55                      push     ebp                            
  0x00212489  8bec                    mov      ebp, esp                       
  0x0021248B  83ec28                  sub      esp, 0x28                      
  0x0021248E  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x00212491  56                      push     esi                            
  0x00212492  57                      push     edi                            
  0x00212493  33ff                    xor      edi, edi                       
  0x00212495  8bf2                    mov      esi, edx                       
  0x00212497  884dfc                  mov      byte ptr [ebp - 4], cl         
  0x0021249A  897df4                  mov      dword ptr [ebp - 0xc], edi     
  0x0021249D  897df0                  mov      dword ptr [ebp - 0x10], edi    
  0x002124A0  8938                    mov      dword ptr [eax], edi           
  0x002124A2  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x002124A8  8a4dfc                  mov      cl, byte ptr [ebp - 4]         
  0x002124AB  8bd6                    mov      edx, esi                       
  0x002124AD  8845fb                  mov      byte ptr [ebp - 5], al         
  0x002124B0  e8d9f6ffff              call     0x211b8e                       ; -> sub_00211B8E
  0x002124B5  8bf0                    mov      esi, eax                       
  0x002124B7  3bf7                    cmp      esi, edi                       
  0x002124B9  8975ec                  mov      dword ptr [ebp - 0x14], esi    
  0x002124BC  750c                    jne      0x2124ca                       
  0x002124BE  c745f48f040000          mov      dword ptr [ebp - 0xc], 0x48f   
  0x002124C5  e9ec010000              jmp      0x2126b6                       
; end of function
                                        ; XREF: 0x002124BC (cond_jump)
  0x002124CA  397e0e                  cmp      dword ptr [esi + 0xe], edi     
  0x002124CD  740c                    je       0x2124db                       
  0x002124CF  c745f420000000          mov      dword ptr [ebp - 0xc], 0x20    
  0x002124D6  e9db010000              jmp      0x2126b6                       
                                        ; XREF: 0x002124CD (cond_jump)
  0x002124DB  0fb645fc                movzx    eax, byte ptr [ebp - 4]        
  0x002124DF  8d0c40                  lea      ecx, [eax + eax*2]             
  0x002124E2  8d0ccdc8082100          lea      ecx, [ecx*8 + 0x2108c8]        
  0x002124E9  8a01                    mov      al, byte ptr [ecx]             
  0x002124EB  84c0                    test     al, al                         
  0x002124ED  750c                    jne      0x2124fb                       
  0x002124EF  c745f40e000000          mov      dword ptr [ebp - 0xc], 0xe     
  0x002124F6  e9bb010000              jmp      0x2126b6                       
                                        ; XREF: 0x002124ED (cond_jump)
  0x002124FB  8b550c                  mov      edx, dword ptr [ebp + 0xc]     
  0x002124FE  fec8                    dec      al                             
  0x00212500  8801                    mov      byte ptr [ecx], al             
  0x00212502  53                      push     ebx                            
  0x00212503  8b1de0a04000            mov      ebx, dword ptr [0x40a0e0]      
  0x00212509  8b83a4000000            mov      eax, dword ptr [ebx + 0xa4]    
  0x0021250F  a3e0a04000              mov      dword ptr [0x40a0e0], eax      
  0x00212514  33c0                    xor      eax, eax                       
  0x00212516  6a2a                    push     0x2a                           
  0x00212518  59                      pop      ecx                            
  0x00212519  8bfb                    mov      edi, ebx                       
  0x0021251B  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x0021251D  8a45fc                  mov      al, byte ptr [ebp - 4]         
  0x00212520  8883a3000000            mov      byte ptr [ebx + 0xa3], al      
  0x00212526  8933                    mov      dword ptr [ebx], esi           
  0x00212528  8a02                    mov      al, byte ptr [edx]             
  0x0021252A  c0e003                  shl      al, 3                          
  0x0021252D  3283a2000000            xor      al, byte ptr [ebx + 0xa2]      
  0x00212533  8bcb                    mov      ecx, ebx                       
  0x00212535  2408                    and      al, 8                          
  0x00212537  3083a2000000            xor      byte ptr [ebx + 0xa2], al      
  0x0021253D  895de8                  mov      dword ptr [ebp - 0x18], ebx    
  0x00212540  895e0e                  mov      dword ptr [esi + 0xe], ebx     
  0x00212543  c745f001000000          mov      dword ptr [ebp - 0x10], 1      
  0x0021254A  e8adf6ffff              call     0x211bfc                       ; -> sub_00211BFC
  0x0021254F  85c0                    test     eax, eax                       
  0x00212551  0f8c55010000            jl       0x2126ac                       
  0x00212557  0fb6460a                movzx    eax, byte ptr [esi + 0xa]      
  0x0021255B  8d0440                  lea      eax, [eax + eax*2]             
  0x0021255E  8b04c5cc082100          mov      eax, dword ptr [eax*8 + 0x2108cc] 
  0x00212565  0fb608                  movzx    ecx, byte ptr [eax]            
  0x00212568  8b7001                  mov      esi, dword ptr [eax + 1]       
  0x0021256B  8bc1                    mov      eax, ecx                       
  0x0021256D  c1e902                  shr      ecx, 2                         
  0x00212570  8d5334                  lea      edx, [ebx + 0x34]              
  0x00212573  8bfa                    mov      edi, edx                       
  0x00212575  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x00212577  8bc8                    mov      ecx, eax                       
  0x00212579  83e103                  and      ecx, 3                         
  0x0021257C  f3a4                    rep movsb byte ptr es:[edi], byte ptr [esi] 
  0x0021257E  6a07                    push     7                              
  0x00212580  59                      pop      ecx                            
  0x00212581  8365dc00                and      dword ptr [ebp - 0x24], 0      
  0x00212585  8bf2                    mov      esi, edx                       
  0x00212587  8d7b14                  lea      edi, [ebx + 0x14]              
  0x0021258A  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x0021258C  66a5                    movsw    word ptr es:[edi], word ptr [esi] 
  0x0021258E  8b75ec                  mov      esi, dword ptr [ebp - 0x14]    
  0x00212591  8d45e0                  lea      eax, [ebp - 0x20]              
  0x00212594  8945e4                  mov      dword ptr [ebp - 0x1c], eax    
  0x00212597  8d45e0                  lea      eax, [ebp - 0x20]              
  0x0021259A  8945e0                  mov      dword ptr [ebp - 0x20], eax    
  0x0021259D  0fb6460c                movzx    eax, byte ptr [esi + 0xc]      
  0x002125A1  83636200                and      dword ptr [ebx + 0x62], 0      
  0x002125A5  80637000                and      byte ptr [ebx + 0x70], 0       
  0x002125A9  8d4dd8                  lea      ecx, [ebp - 0x28]              
  0x002125AC  894b5e                  mov      dword ptr [ebx + 0x5e], ecx    
  0x002125AF  8d4b32                  lea      ecx, [ebx + 0x32]              
  0x002125B2  8d7b52                  lea      edi, [ebx + 0x52]              
  0x002125B5  c60730                  mov      byte ptr [edi], 0x30           
  0x002125B8  c6435340                mov      byte ptr [ebx + 0x53], 0x40    
  0x002125BC  c7435a56422100          mov      dword ptr [ebx + 0x5a], 0x214256 
  0x002125C3  894b6a                  mov      dword ptr [ebx + 0x6a], ecx    
  0x002125C6  894366                  mov      dword ptr [ebx + 0x66], eax    
  0x002125C9  c6436e02                mov      byte ptr [ebx + 0x6e], 2       
  0x002125CD  c6436f01                mov      byte ptr [ebx + 0x6f], 1       
  0x002125D1  c6437aa1                mov      byte ptr [ebx + 0x7a], 0xa1    
  0x002125D5  c6437b01                mov      byte ptr [ebx + 0x7b], 1       
  0x002125D9  66c7437c0001            mov      word ptr [ebx + 0x7c], 0x100   
  0x002125DF  660fb64e05              movzx    cx, byte ptr [esi + 5]         
  0x002125E4  66894b7e                mov      word ptr [ebx + 0x7e], cx      
  0x002125E8  66898380000000          mov      word ptr [ebx + 0x80], ax      
  0x002125EF  8b0e                    mov      ecx, dword ptr [esi]           
  0x002125F1  57                      push     edi                            
  0x002125F2  c645d801                mov      byte ptr [ebp - 0x28], 1       
  0x002125F6  c645da04                mov      byte ptr [ebp - 0x26], 4       
  0x002125FA  e822200000              call     0x214621                       ; -> sub_00214621
  0x002125FF  8a4dfb                  mov      cl, byte ptr [ebp - 5]         
  0x00212602  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00212608  33c0                    xor      eax, eax                       
  0x0021260A  50                      push     eax                            
  0x0021260B  50                      push     eax                            
  0x0021260C  50                      push     eax                            
  0x0021260D  50                      push     eax                            
  0x0021260E  8d45d8                  lea      eax, [ebp - 0x28]              
  0x00212611  50                      push     eax                            
  0x00212612  ff15107b2100            call     dword ptr [0x217b10]           ; -> xbox_KeWaitForSingleObject
  0x00212618  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x0021261E  833b00                  cmp      dword ptr [ebx], 0             
  0x00212621  8845fb                  mov      byte ptr [ebp - 5], al         
  0x00212624  747d                    je       0x2126a3                       
  0x00212626  f6460402                test     byte ptr [esi + 4], 2          
  0x0021262A  7577                    jne      0x2126a3                       
  0x0021262C  837b5600                cmp      dword ptr [ebx + 0x56], 0      
  0x00212630  7c10                    jl       0x212642                       
  0x00212632  0fb6460a                movzx    eax, byte ptr [esi + 0xa]      
  0x00212636  8d0440                  lea      eax, [eax + eax*2]             
  0x00212639  8bcb                    mov      ecx, ebx                       
  0x0021263B  ff14c5d8082100          call     dword ptr [eax*8 + 0x2108d8]   
                                        ; XREF: 0x00212630 (cond_jump)
  0x00212642  80bba300000001          cmp      byte ptr [ebx + 0xa3], 1       
  0x00212649  750c                    jne      0x212657                       
  0x0021264B  a1fc072100              mov      eax, dword ptr [0x2107fc]      
  0x00212650  85c0                    test     eax, eax                       
  0x00212652  7403                    je       0x212657                       
  0x00212654  53                      push     ebx                            
  0x00212655  ff10                    call     dword ptr [eax]                
                                        ; XREF: 0x00212649 (cond_jump), 0x00212652 (cond_jump)
  0x00212657  8b430c                  mov      eax, dword ptr [ebx + 0xc]     
  0x0021265A  894362                  mov      dword ptr [ebx + 0x62], eax    
  0x0021265D  8d4332                  lea      eax, [ebx + 0x32]              
  0x00212660  c60728                  mov      byte ptr [edi], 0x28           
  0x00212663  c6435341                mov      byte ptr [ebx + 0x53], 0x41    
  0x00212667  c7435a581d2100          mov      dword ptr [ebx + 0x5a], 0x211d58 
  0x0021266E  895b5e                  mov      dword ptr [ebx + 0x5e], ebx    
  0x00212671  89436a                  mov      dword ptr [ebx + 0x6a], eax    
  0x00212674  0fb6460c                movzx    eax, byte ptr [esi + 0xc]      
  0x00212678  80637000                and      byte ptr [ebx + 0x70], 0       
  0x0021267C  f683a200000008          test     byte ptr [ebx + 0xa2], 8       
  0x00212683  894366                  mov      dword ptr [ebx + 0x66], eax    
  0x00212686  c6436e02                mov      byte ptr [ebx + 0x6e], 2       
  0x0021268A  c6436f01                mov      byte ptr [ebx + 0x6f], 1       
  0x0021268E  7408                    je       0x212698                       
  0x00212690  8b0e                    mov      ecx, dword ptr [esi]           
  0x00212692  57                      push     edi                            
  0x00212693  e8891f0000              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x0021268E (cond_jump)
  0x00212698  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x0021269B  8365f000                and      dword ptr [ebp - 0x10], 0      
  0x0021269F  8918                    mov      dword ptr [eax], ebx           
  0x002126A1  eb12                    jmp      0x2126b5                       
                                        ; XREF: 0x00212624 (cond_jump), 0x0021262A (cond_jump)
  0x002126A3  c745f48f040000          mov      dword ptr [ebp - 0xc], 0x48f   
  0x002126AA  eb09                    jmp      0x2126b5                       
                                        ; XREF: 0x00212551 (cond_jump)
  0x002126AC  50                      push     eax                            
  0x002126AD  e8e71b0000              call     0x214299                       ; -> sub_00214299
  0x002126B2  8945f4                  mov      dword ptr [ebp - 0xc], eax     
                                        ; XREF: 0x002126A1 (jump), 0x002126AA (jump)
  0x002126B5  5b                      pop      ebx                            
                                        ; XREF: 0x002124C5 (jump), 0x002124D6 (jump), 0x002124F6 (jump)
  0x002126B6  8a4dfb                  mov      cl, byte ptr [ebp - 5]         
  0x002126B9  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002126BF  837df000                cmp      dword ptr [ebp - 0x10], 0      
  0x002126C3  5f                      pop      edi                            
  0x002126C4  5e                      pop      esi                            
  0x002126C5  7408                    je       0x2126cf                       
  0x002126C7  8b4de8                  mov      ecx, dword ptr [ebp - 0x18]    
  0x002126CA  e871faffff              call     0x212140                       ; -> sub_00212140
                                        ; XREF: 0x002126C5 (cond_jump)
  0x002126CF  8b45f4                  mov      eax, dword ptr [ebp - 0xc]     
  0x002126D2  c9                      leave                                   
  0x002126D3  c20800                  ret      8                              
  0x002126D6  66a1daa04000            mov      ax, word ptr [0x40a0da]        
  0x002126DC  53                      push     ebx                            
  0x002126DD  33db                    xor      ebx, ebx                       
  0x002126DF  32c9                    xor      cl, cl                         
  0x002126E1  663b05d8a04000          cmp      ax, word ptr [0x40a0d8]        
  0x002126E8  0f832b010000            jae      0x212819                       
  0x002126EE  a1dca04000              mov      eax, dword ptr [0x40a0dc]      
  0x002126F3  f6400401                test     byte ptr [eax + 4], 1          
  0x002126F7  740f                    je       0x212708                       
                                        ; XREF: 0x00212706 (cond_jump)
  0x002126F9  fec1                    inc      cl                             
  0x002126FB  0fb6d1                  movzx    edx, cl                        
  0x002126FE  8d14d2                  lea      edx, [edx + edx*8]             
  0x00212701  f644500401              test     byte ptr [eax + edx*2 + 4], 1  
  0x00212706  75f1                    jne      0x2126f9                       
                                        ; XREF: 0x002126F7 (cond_jump)
  0x00212708  66ff05daa04000          inc      word ptr [0x40a0da]            
  0x0021270F  880d28a14000            mov      byte ptr [0x40a128], cl        
  0x00212715  0fb6c9                  movzx    ecx, cl                        
  0x00212718  56                      push     esi                            
  0x00212719  8d0cc9                  lea      ecx, [ecx + ecx*8]             
  0x0021271C  57                      push     edi                            
  0x0021271D  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00212721  8d3448                  lea      esi, [eax + ecx*2]             
  0x00212724  56                      push     esi                            
  0x00212725  8bcf                    mov      ecx, edi                       
  0x00212727  e8521b0000              call     0x21427e                       ; -> sub_0021427E
  0x0021272C  8a4604                  mov      al, byte ptr [esi + 4]         
  0x0021272F  24f1                    and      al, 0xf1                       
  0x00212731  0c01                    or       al, 1                          
  0x00212733  8bcf                    mov      ecx, edi                       
  0x00212735  893e                    mov      dword ptr [esi], edi           
  0x00212737  884604                  mov      byte ptr [esi + 4], al         
  0x0021273A  e84c1b0000              call     0x21428b                       ; -> sub_0021428B
  0x0021273F  53                      push     ebx                            
  0x00212740  6a01                    push     1                              
  0x00212742  6a03                    push     3                              
  0x00212744  8bcf                    mov      ecx, edi                       
  0x00212746  884605                  mov      byte ptr [esi + 5], al         
  0x00212749  895e0e                  mov      dword ptr [esi + 0xe], ebx     
  0x0021274C  e8281c0000              call     0x214379                       ; -> sub_00214379
  0x00212751  8a4802                  mov      cl, byte ptr [eax + 2]         
  0x00212754  53                      push     ebx                            
  0x00212755  884e08                  mov      byte ptr [esi + 8], cl         
  0x00212758  8a4004                  mov      al, byte ptr [eax + 4]         
  0x0021275B  53                      push     ebx                            
  0x0021275C  6a03                    push     3                              
  0x0021275E  8bcf                    mov      ecx, edi                       
  0x00212760  884606                  mov      byte ptr [esi + 6], al         
  0x00212763  e8111c0000              call     0x214379                       ; -> sub_00214379
  0x00212768  3bc3                    cmp      eax, ebx                       
  0x0021276A  740e                    je       0x21277a                       
  0x0021276C  8a4802                  mov      cl, byte ptr [eax + 2]         
  0x0021276F  884e09                  mov      byte ptr [esi + 9], cl         
  0x00212772  8a4004                  mov      al, byte ptr [eax + 4]         
  0x00212775  884607                  mov      byte ptr [esi + 7], al         
  0x00212778  eb06                    jmp      0x212780                       
                                        ; XREF: 0x0021276A (cond_jump)
  0x0021277A  885e09                  mov      byte ptr [esi + 9], bl         
  0x0021277D  885e07                  mov      byte ptr [esi + 7], bl         
                                        ; XREF: 0x00212778 (jump)
  0x00212780  6a10                    push     0x10                           
  0x00212782  58                      pop      eax                            
  0x00212783  57                      push     edi                            
  0x00212784  c605f8a0400030          mov      byte ptr [0x40a0f8], 0x30      
  0x0021278B  c605f9a0400040          mov      byte ptr [0x40a0f9], 0x40      
  0x00212792  c70500a14000d5232100    mov      dword ptr [0x40a100], 0x2123d5 
  0x0021279C  893504a14000            mov      dword ptr [0x40a104], esi      
  0x002127A2  891d08a14000            mov      dword ptr [0x40a108], ebx      
  0x002127A8  c70510a14000e4a04000    mov      dword ptr [0x40a110], 0x40a0e4 
  0x002127B2  a30ca14000              mov      dword ptr [0x40a10c], eax      
  0x002127B7  c60514a1400002          mov      byte ptr [0x40a114], 2         
  0x002127BE  c60515a1400001          mov      byte ptr [0x40a115], 1         
  0x002127C5  881d16a14000            mov      byte ptr [0x40a116], bl        
  0x002127CB  c60520a14000c1          mov      byte ptr [0x40a120], 0xc1      
  0x002127D2  c60521a1400006          mov      byte ptr [0x40a121], 6         
  0x002127D9  66c70522a140000042      mov      word ptr [0x40a122], 0x4200    
  0x002127E2  660fb64e05              movzx    cx, byte ptr [esi + 5]         
  0x002127E7  685a1b2100              push     0x211b5a                       
  0x002127EC  6858a14000              push     0x40a158                       
  0x002127F1  66890d24a14000          mov      word ptr [0x40a124], cx        
  0x002127F8  66a326a14000            mov      word ptr [0x40a126], ax        
  0x002127FE  ff15187b2100            call     dword ptr [0x217b18]           ; -> xbox_KeInitializeDpc
  0x00212804  e836f3ffff              call     0x211b3f                       ; -> sub_00211B3F
  0x00212809  68f8a04000              push     0x40a0f8                       
  0x0021280E  8bcf                    mov      ecx, edi                       
  0x00212810  e80c1e0000              call     0x214621                       ; -> sub_00214621
  0x00212815  5f                      pop      edi                            
  0x00212816  5e                      pop      esi                            
  0x00212817  eb0e                    jmp      0x212827                       
                                        ; XREF: 0x002126E8 (cond_jump)
  0x00212819  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x0021281D  6800010080              push     0x80000100                     
  0x00212822  e85b130000              call     0x213b82                       ; -> sub_00213B82
                                        ; XREF: 0x00212817 (jump)
  0x00212827  5b                      pop      ebx                            
  0x00212828  c20400                  ret      4                              

; ============================================================
; Function: sub_0021282B
; Start: 0x0021282B  End: 0x00212885  Size: 90 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0021282B:
  0x0021282B  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021282F  57                      push     edi                            
  0x00212830  8bd1                    mov      edx, ecx                       
  0x00212832  80a2a000000000          and      byte ptr [edx + 0xa0], 0       
  0x00212839  80a2a100000000          and      byte ptr [edx + 0xa1], 0       
  0x00212840  898298000000            mov      dword ptr [edx + 0x98], eax    
  0x00212846  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x0021284A  89829c000000            mov      dword ptr [edx + 0x9c], eax    
  0x00212850  33c0                    xor      eax, eax                       
  0x00212852  6a0c                    push     0xc                            
  0x00212854  59                      pop      ecx                            
  0x00212855  8bfa                    mov      edi, edx                       
  0x00212857  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00212859  66ab                    stosw    word ptr es:[edi], ax          
  0x0021285B  33c0                    xor      eax, eax                       
  0x0021285D  6a0c                    push     0xc                            
  0x0021285F  59                      pop      ecx                            
  0x00212860  8d7a32                  lea      edi, [edx + 0x32]              
  0x00212863  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00212865  66ab                    stosw    word ptr es:[edi], ax          
  0x00212867  33c0                    xor      eax, eax                       
  0x00212869  6a0c                    push     0xc                            
  0x0021286B  59                      pop      ecx                            
  0x0021286C  8d7a64                  lea      edi, [edx + 0x64]              
  0x0021286F  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00212871  66ab                    stosw    word ptr es:[edi], ax          
  0x00212873  33c0                    xor      eax, eax                       
  0x00212875  8dbaa4000000            lea      edi, [edx + 0xa4]              
  0x0021287B  ab                      stosd    dword ptr es:[edi], eax        
  0x0021287C  ab                      stosd    dword ptr es:[edi], eax        
  0x0021287D  ab                      stosd    dword ptr es:[edi], eax        
  0x0021287E  ab                      stosd    dword ptr es:[edi], eax        
  0x0021287F  8bc2                    mov      eax, edx                       
  0x00212881  5f                      pop      edi                            
  0x00212882  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00212885
; Start: 0x00212885  End: 0x002128D5  Size: 80 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00210BFA, sub_0021357E, sub_00213DC4
; ============================================================
sub_00212885:
  0x00212885  0fb65179                movzx    edx, byte ptr [ecx + 0x79]     
  0x00212889  8b81e0000000            mov      eax, dword ptr [ecx + 0xe0]    
  0x0021288F  c1e205                  shl      edx, 5                         
  0x00212892  56                      push     esi                            
  0x00212893  8d741001                lea      esi, [eax + edx + 1]           
  0x00212897  8a06                    mov      al, byte ptr [esi]             
  0x00212899  884179                  mov      byte ptr [ecx + 0x79], al      
  0x0021289C  c60680                  mov      byte ptr [esi], 0x80           
  0x0021289F  8b81e0000000            mov      eax, dword ptr [ecx + 0xe0]    
  0x002128A5  c644100280              mov      byte ptr [eax + edx + 2], 0x80 
  0x002128AA  8b81e0000000            mov      eax, dword ptr [ecx + 0xe0]    
  0x002128B0  c644100380              mov      byte ptr [eax + edx + 3], 0x80 
  0x002128B5  8b81e0000000            mov      eax, dword ptr [ecx + 0xe0]    
  0x002128BB  8364101c00              and      dword ptr [eax + edx + 0x1c], 0 
  0x002128C0  8b81e0000000            mov      eax, dword ptr [ecx + 0xe0]    
  0x002128C6  804c1007ff              or       byte ptr [eax + edx + 7], 0xff 
  0x002128CB  8b81e0000000            mov      eax, dword ptr [ecx + 0xe0]    
  0x002128D1  03c2                    add      eax, edx                       
  0x002128D3  5e                      pop      esi                            
  0x002128D4  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002128D5
; Start: 0x002128D5  End: 0x002128E9  Size: 20 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002143F3, sub_00215112, sub_00216771, sub_00216B17, sub_00216B9E, sub_00216F89
; ============================================================
sub_002128D5:
  0x002128D5  8b442404                mov      eax, dword ptr [esp + 4]       
  0x002128D9  8b4808                  mov      ecx, dword ptr [eax + 8]       
  0x002128DC  85c9                    test     ecx, ecx                       
  0x002128DE  7406                    je       0x2128e6                       
  0x002128E0  ff700c                  push     dword ptr [eax + 0xc]          
  0x002128E3  50                      push     eax                            
  0x002128E4  ffd1                    call     ecx                            
                                        ; XREF: 0x002128DE (cond_jump)
  0x002128E6  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002128E9
; Start: 0x002128E9  End: 0x00212932  Size: 73 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_00211488
; ============================================================
sub_002128E9:
  0x002128E9  55                      push     ebp                            
  0x002128EA  8bec                    mov      ebp, esp                       
  0x002128EC  83ec10                  sub      esp, 0x10                      
  0x002128EF  0fb6450c                movzx    eax, byte ptr [ebp + 0xc]      
  0x002128F3  8365f000                and      dword ptr [ebp - 0x10], 0      
  0x002128F7  8365f800                and      dword ptr [ebp - 8], 0         
  0x002128FB  c745f409000000          mov      dword ptr [ebp - 0xc], 9       
  0x00212902  c745fc0d000000          mov      dword ptr [ebp - 4], 0xd       
  0x00212909  8b4c85f0                mov      ecx, dword ptr [ebp + eax*4 - 0x10] 
  0x0021290D  0fb74508                movzx    eax, word ptr [ebp + 8]        
  0x00212911  03c1                    add      eax, ecx                       
  0x00212913  6bc038                  imul     eax, eax, 0x38                 
  0x00212916  56                      push     esi                            
  0x00212917  6a06                    push     6                              
  0x00212919  33d2                    xor      edx, edx                       
  0x0021291B  5e                      pop      esi                            
  0x0021291C  f7f6                    div      esi                            
  0x0021291E  85c9                    test     ecx, ecx                       
  0x00212920  5e                      pop      esi                            
  0x00212921  7502                    jne      0x212925                       
  0x00212923  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00212921 (cond_jump)
  0x00212925  807d1000                cmp      byte ptr [ebp + 0x10], 0       
  0x00212929  7403                    je       0x21292e                       
  0x0021292B  c1e003                  shl      eax, 3                         
                                        ; XREF: 0x00212929 (cond_jump)
  0x0021292E  c9                      leave                                   
  0x0021292F  c20c00                  ret      0xc                            
; end of function

; ============================================================
; Function: sub_00212932
; Start: 0x00212932  End: 0x00212963  Size: 49 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00213CB5
; ============================================================
sub_00212932:
  0x00212932  56                      push     esi                            
  0x00212933  b864072100              mov      eax, 0x210764                  
  0x00212938  be74072100              mov      esi, 0x210774                  
  0x0021293D  3bc6                    cmp      eax, esi                       
  0x0021293F  8bc8                    mov      ecx, eax                       
  0x00212941  731a                    jae      0x21295d                       
  0x00212943  8b542408                mov      edx, dword ptr [esp + 8]       
                                        ; XREF: 0x0021295B (cond_jump)
  0x00212947  8b01                    mov      eax, dword ptr [ecx]           
  0x00212949  85c0                    test     eax, eax                       
  0x0021294B  7409                    je       0x212956                       
  0x0021294D  3a7001                  cmp      dh, byte ptr [eax + 1]         
  0x00212950  7504                    jne      0x212956                       
  0x00212952  3a10                    cmp      dl, byte ptr [eax]             
  0x00212954  740d                    je       0x212963                       
                                        ; XREF: 0x0021294B (cond_jump), 0x00212950 (cond_jump)
  0x00212956  83c104                  add      ecx, 4                         
  0x00212959  3bce                    cmp      ecx, esi                       
  0x0021295B  72ea                    jb       0x212947                       
                                        ; XREF: 0x00212941 (cond_jump)
  0x0021295D  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00212965 (jump)
  0x0021295F  5e                      pop      esi                            
  0x00212960  c20400                  ret      4                              
; end of function
                                        ; XREF: 0x00212954 (cond_jump)
  0x00212963  8b01                    mov      eax, dword ptr [ecx]           
  0x00212965  ebf8                    jmp      0x21295f                       

; ============================================================
; Function: sub_00212967
; Start: 0x00212967  End: 0x00212987  Size: 32 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_00212C7A, sub_00212DC9
; ============================================================
sub_00212967:
  0x00212967  55                      push     ebp                            
  0x00212968  8bec                    mov      ebp, esp                       
  0x0021296A  51                      push     ecx                            
  0x0021296B  8a450b                  mov      al, byte ptr [ebp + 0xb]       
  0x0021296E  8845fc                  mov      byte ptr [ebp - 4], al         
  0x00212971  8a450a                  mov      al, byte ptr [ebp + 0xa]       
  0x00212974  8845fd                  mov      byte ptr [ebp - 3], al         
  0x00212977  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x0021297A  8865fe                  mov      byte ptr [ebp - 2], ah         
  0x0021297D  8845ff                  mov      byte ptr [ebp - 1], al         
  0x00212980  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00212983  c9                      leave                                   
  0x00212984  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00212987
; Start: 0x00212987  End: 0x002129BE  Size: 55 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00212987:
  0x00212987  55                      push     ebp                            
  0x00212988  8bec                    mov      ebp, esp                       
  0x0021298A  53                      push     ebx                            
  0x0021298B  56                      push     esi                            
  0x0021298C  57                      push     edi                            
  0x0021298D  8b7d08                  mov      edi, dword ptr [ebp + 8]       
  0x00212990  8b7718                  mov      esi, dword ptr [edi + 0x18]    
  0x00212993  bb03010000              mov      ebx, 0x103                     
  0x00212998  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x0021299E  f6460c06                test     byte ptr [esi + 0xc], 6        
  0x002129A2  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x002129A5  88450b                  mov      byte ptr [ebp + 0xb], al       
  0x002129A8  7414                    je       0x2129be                       
  0x002129AA  b89d0000c0              mov      eax, 0xc000009d                
  0x002129AF  32d2                    xor      dl, dl                         
  0x002129B1  8bd8                    mov      ebx, eax                       
  0x002129B3  894110                  mov      dword ptr [ecx + 0x10], eax    
  0x002129B6  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x002129BC  eb11                    jmp      0x2129cf                       
; end of function
                                        ; XREF: 0x002129A8 (cond_jump)
  0x002129BE  8b415c                  mov      eax, dword ptr [ecx + 0x5c]    
  0x002129C1  80480301                or       byte ptr [eax + 3], 1          
  0x002129C5  6a00                    push     0                              
  0x002129C7  51                      push     ecx                            
  0x002129C8  57                      push     edi                            
  0x002129C9  ff15407b2100            call     dword ptr [0x217b40]           ; -> xbox_IoStartNextPacket
                                        ; XREF: 0x002129BC (jump)
  0x002129CF  8a4d0b                  mov      cl, byte ptr [ebp + 0xb]       
  0x002129D2  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002129D8  5f                      pop      edi                            
  0x002129D9  5e                      pop      esi                            
  0x002129DA  8bc3                    mov      eax, ebx                       
  0x002129DC  5b                      pop      ebx                            
  0x002129DD  5d                      pop      ebp                            
  0x002129DE  c20800                  ret      8                              

; ============================================================
; Function: sub_002129E1
; Start: 0x002129E1  End: 0x00212A04  Size: 35 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00212CFB
; ============================================================
sub_002129E1:
  0x002129E1  8b442408                mov      eax, dword ptr [esp + 8]       
  0x002129E5  56                      push     esi                            
  0x002129E6  8b742408                mov      esi, dword ptr [esp + 8]       
  0x002129EA  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x002129ED  894110                  mov      dword ptr [ecx + 0x10], eax    
  0x002129F0  b9000000c0              mov      ecx, 0xc0000000                
  0x002129F5  23c1                    and      eax, ecx                       
  0x002129F7  3bc1                    cmp      eax, ecx                       
  0x002129F9  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x002129FC  7506                    jne      0x212a04                       
  0x002129FE  83601400                and      dword ptr [eax + 0x14], 0      
  0x00212A02  eb06                    jmp      0x212a0a                       
; end of function
                                        ; XREF: 0x002129FC (cond_jump)
  0x00212A04  8b4e2c                  mov      ecx, dword ptr [esi + 0x2c]    
  0x00212A07  894814                  mov      dword ptr [eax + 0x14], ecx    
                                        ; XREF: 0x00212A02 (jump)
  0x00212A0A  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x00212A0D  32d2                    xor      dl, dl                         
  0x00212A0F  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x00212A15  ff36                    push     dword ptr [esi]                
  0x00212A17  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
  0x00212A1D  5e                      pop      esi                            
  0x00212A1E  c20800                  ret      8                              

; ============================================================
; Function: sub_00212A21
; Start: 0x00212A21  End: 0x00212A3D  Size: 28 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00212DC9
; ============================================================
sub_00212A21:
  0x00212A21  a1cc704000              mov      eax, dword ptr [0x4070cc]      
  0x00212A26  3dcc704000              cmp      eax, 0x4070cc                  
  0x00212A2B  740f                    je       0x212a3c                       
  0x00212A2D  8b4008                  mov      eax, dword ptr [eax + 8]       
  0x00212A30  8b4014                  mov      eax, dword ptr [eax + 0x14]    
  0x00212A33  8b4018                  mov      eax, dword ptr [eax + 0x18]    
  0x00212A36  6a00                    push     0                              
  0x00212A38  50                      push     eax                            
  0x00212A39  ff5030                  call     dword ptr [eax + 0x30]         
                                        ; XREF: 0x00212A2B (cond_jump)
  0x00212A3C  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00212A3D
; Start: 0x00212A3D  End: 0x00212A64  Size: 39 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00212DC9
; ============================================================
sub_00212A3D:
  0x00212A3D  8bc1                    mov      eax, ecx                       
  0x00212A3F  33d2                    xor      edx, edx                       
  0x00212A41  395004                  cmp      dword ptr [eax + 4], edx       
  0x00212A44  761d                    jbe      0x212a63                       
  0x00212A46  56                      push     esi                            
  0x00212A47  57                      push     edi                            
                                        ; XREF: 0x00212A5F (cond_jump)
  0x00212A48  8b38                    mov      edi, dword ptr [eax]           
  0x00212A4A  03fa                    add      edi, edx                       
  0x00212A4C  6a08                    push     8                              
  0x00212A4E  59                      pop      ecx                            
  0x00212A4F  bedc792100              mov      esi, 0x2179dc                  
  0x00212A54  81c200100000            add      edx, 0x1000                    
  0x00212A5A  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x00212A5C  3b5004                  cmp      edx, dword ptr [eax + 4]       
  0x00212A5F  72e7                    jb       0x212a48                       
  0x00212A61  5f                      pop      edi                            
  0x00212A62  5e                      pop      esi                            
                                        ; XREF: 0x00212A44 (cond_jump)
  0x00212A63  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00212A64
; Start: 0x00212A64  End: 0x00212A79  Size: 21 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00212A64:
  0x00212A64  8b442404                mov      eax, dword ptr [esp + 4]       
  0x00212A68  83f801                  cmp      eax, 1                         
  0x00212A6B  760c                    jbe      0x212a79                       
  0x00212A6D  8d48ff                  lea      ecx, [eax - 1]                 
  0x00212A70  85c8                    test     eax, ecx                       
  0x00212A72  7505                    jne      0x212a79                       
  0x00212A74  33c0                    xor      eax, eax                       
  0x00212A76  40                      inc      eax                            
  0x00212A77  eb02                    jmp      0x212a7b                       
; end of function
                                        ; XREF: 0x00212A6B (cond_jump), 0x00212A72 (cond_jump)
  0x00212A79  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00212A77 (jump)
  0x00212A7B  c20400                  ret      4                              

; ============================================================
; Function: sub_00212A7E
; Start: 0x00212A7E  End: 0x00212A94  Size: 22 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00212A7E:
  0x00212A7E  33c9                    xor      ecx, ecx                       
  0x00212A80  32c0                    xor      al, al                         
  0x00212A82  41                      inc      ecx                            
                                        ; XREF: 0x00212A8F (cond_jump)
  0x00212A83  854c2404                test     dword ptr [esp + 4], ecx       
  0x00212A87  7508                    jne      0x212a91                       
  0x00212A89  fec0                    inc      al                             
  0x00212A8B  d1e1                    shl      ecx, 1                         
  0x00212A8D  3c20                    cmp      al, 0x20                       
  0x00212A8F  72f2                    jb       0x212a83                       
                                        ; XREF: 0x00212A87 (cond_jump)
  0x00212A91  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00212A94
; Start: 0x00212A94  End: 0x00212AD1  Size: 61 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00212A94:
  0x00212A94  55                      push     ebp                            
  0x00212A95  8bec                    mov      ebp, esp                       
  0x00212A97  83ec14                  sub      esp, 0x14                      
  0x00212A9A  8b550c                  mov      edx, dword ptr [ebp + 0xc]     
  0x00212A9D  53                      push     ebx                            
  0x00212A9E  8b5d08                  mov      ebx, dword ptr [ebp + 8]       
  0x00212AA1  8b4b10                  mov      ecx, dword ptr [ebx + 0x10]    
  0x00212AA4  8b415c                  mov      eax, dword ptr [ecx + 0x5c]    
  0x00212AA7  56                      push     esi                            
  0x00212AA8  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x00212AAB  b8000000c0              mov      eax, 0xc0000000                
  0x00212AB0  8bf2                    mov      esi, edx                       
  0x00212AB2  57                      push     edi                            
  0x00212AB3  8b7b28                  mov      edi, dword ptr [ebx + 0x28]    
  0x00212AB6  23f0                    and      esi, eax                       
  0x00212AB8  3bf0                    cmp      esi, eax                       
  0x00212ABA  897df8                  mov      dword ptr [ebp - 8], edi       
  0x00212ABD  894dfc                  mov      dword ptr [ebp - 4], ecx       
  0x00212AC0  750f                    jne      0x212ad1                       
  0x00212AC2  83611400                and      dword ptr [ecx + 0x14], 0      
  0x00212AC6  8b4310                  mov      eax, dword ptr [ebx + 0x10]    
  0x00212AC9  895010                  mov      dword ptr [eax + 0x10], edx    
  0x00212ACC  e975010000              jmp      0x212c46                       
; end of function
                                        ; XREF: 0x00212AC0 (cond_jump)
  0x00212AD1  668b4706                mov      ax, word ptr [edi + 6]         
  0x00212AD5  ff37                    push     dword ptr [edi]                
  0x00212AD7  88650a                  mov      byte ptr [ebp + 0xa], ah       
  0x00212ADA  88450b                  mov      byte ptr [ebp + 0xb], al       
  0x00212ADD  668b4704                mov      ax, word ptr [edi + 4]         
  0x00212AE1  0fb74d0a                movzx    ecx, word ptr [ebp + 0xa]      
  0x00212AE5  88650e                  mov      byte ptr [ebp + 0xe], ah       
  0x00212AE8  88450f                  mov      byte ptr [ebp + 0xf], al       
  0x00212AEB  0fb7750e                movzx    esi, word ptr [ebp + 0xe]      
  0x00212AEF  0faff1                  imul     esi, ecx                       
  0x00212AF2  e870feffff              call     0x212967                       ; -> sub_00212967
  0x00212AF7  0fb74d0a                movzx    ecx, word ptr [ebp + 0xa]      
  0x00212AFB  40                      inc      eax                            
  0x00212AFC  89450c                  mov      dword ptr [ebp + 0xc], eax     
  0x00212AFF  f7e1                    mul      ecx                            
  0x00212B01  85f6                    test     esi, esi                       
  0x00212B03  8945ec                  mov      dword ptr [ebp - 0x14], eax    
  0x00212B06  8955f0                  mov      dword ptr [ebp - 0x10], edx    
  0x00212B09  7505                    jne      0x212b10                       
  0x00212B0B  be00200000              mov      esi, 0x2000                    
                                        ; XREF: 0x00212B09 (cond_jump)
  0x00212B10  51                      push     ecx                            
  0x00212B11  e84effffff              call     0x212a64                       ; -> sub_00212A64
  0x00212B16  85c0                    test     eax, eax                       
  0x00212B18  0f8449010000            je       0x212c67                       
  0x00212B1E  0fb7550a                movzx    edx, word ptr [ebp + 0xa]      
  0x00212B22  81fa00100000            cmp      edx, 0x1000                    
  0x00212B28  0f8739010000            ja       0x212c67                       
  0x00212B2E  81fe00400000            cmp      esi, 0x4000                    
  0x00212B34  0f872d010000            ja       0x212c67                       
  0x00212B3A  66f7c6ff0f              test     si, 0xfff                      
  0x00212B3F  0f8522010000            jne      0x212c67                       
  0x00212B45  33c9                    xor      ecx, ecx                       
  0x00212B47  394d0c                  cmp      dword ptr [ebp + 0xc], ecx     
  0x00212B4A  0f8417010000            je       0x212c67                       
  0x00212B50  8b45f0                  mov      eax, dword ptr [ebp - 0x10]    
  0x00212B53  3bc1                    cmp      eax, ecx                       
  0x00212B55  0f820c010000            jb       0x212c67                       
  0x00212B5B  7709                    ja       0x212b66                       
  0x00212B5D  3975ec                  cmp      dword ptr [ebp - 0x14], esi    
  0x00212B60  0f8201010000            jb       0x212c67                       
                                        ; XREF: 0x00212B5B (cond_jump)
  0x00212B66  83f801                  cmp      eax, 1                         
  0x00212B69  0f87f8000000            ja       0x212c67                       
  0x00212B6F  720a                    jb       0x212b7b                       
  0x00212B71  837dec00                cmp      dword ptr [ebp - 0x14], 0      
  0x00212B75  0f87ec000000            ja       0x212c67                       
                                        ; XREF: 0x00212B6F (cond_jump)
  0x00212B7B  8b4dec                  mov      ecx, dword ptr [ebp - 0x14]    
  0x00212B7E  52                      push     edx                            
  0x00212B7F  898b60010000            mov      dword ptr [ebx + 0x160], ecx   
  0x00212B85  898364010000            mov      dword ptr [ebx + 0x164], eax   
  0x00212B8B  89b368010000            mov      dword ptr [ebx + 0x168], esi   
  0x00212B91  e8e8feffff              call     0x212a7e                       ; -> sub_00212A7E
  0x00212B96  0fb6c0                  movzx    eax, al                        
  0x00212B99  898358010000            mov      dword ptr [ebx + 0x158], eax   
  0x00212B9F  33c0                    xor      eax, eax                       
  0x00212BA1  50                      push     eax                            
  0x00212BA2  56                      push     esi                            
  0x00212BA3  ff75f0                  push     dword ptr [ebp - 0x10]         
  0x00212BA6  c783480100000c000000    mov      dword ptr [ebx + 0x148], 0xc   
  0x00212BB0  ff75ec                  push     dword ptr [ebp - 0x14]         
  0x00212BB3  8dbb40010000            lea      edi, [ebx + 0x140]             
  0x00212BB9  e8825dfaff              call     0x1b8940                       ; -> sub_001B8940
  0x00212BBE  8907                    mov      dword ptr [edi], eax           
  0x00212BC0  8b45f4                  mov      eax, dword ptr [ebp - 0xc]     
  0x00212BC3  c1ee0c                  shr      esi, 0xc                       
  0x00212BC6  895704                  mov      dword ptr [edi + 4], edx       
  0x00212BC9  c7834c01000001000000    mov      dword ptr [ebx + 0x14c], 1     
  0x00212BD3  89b350010000            mov      dword ptr [ebx + 0x150], esi   
  0x00212BD9  c7835401000000100000    mov      dword ptr [ebx + 0x154], 0x1000 
  0x00212BE3  8b4010                  mov      eax, dword ptr [eax + 0x10]    
  0x00212BE6  3d00000700              cmp      eax, 0x70000                   
  0x00212BEB  7438                    je       0x212c25                       
  0x00212BED  3d04400700              cmp      eax, 0x74004                   
  0x00212BF2  754f                    jne      0x212c43                       
  0x00212BF4  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00212BF7  8b5030                  mov      edx, dword ptr [eax + 0x30]    
  0x00212BFA  33c0                    xor      eax, eax                       
  0x00212BFC  6a08                    push     8                              
  0x00212BFE  59                      pop      ecx                            
  0x00212BFF  8bfa                    mov      edi, edx                       
  0x00212C01  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00212C03  8b8360010000            mov      eax, dword ptr [ebx + 0x160]   
  0x00212C09  894208                  mov      dword ptr [edx + 8], eax       
  0x00212C0C  8b8364010000            mov      eax, dword ptr [ebx + 0x164]   
  0x00212C12  89420c                  mov      dword ptr [edx + 0xc], eax     
  0x00212C15  c6421a01                mov      byte ptr [edx + 0x1a], 1       
  0x00212C19  8b4310                  mov      eax, dword ptr [ebx + 0x10]    
  0x00212C1C  c7401420000000          mov      dword ptr [eax + 0x14], 0x20   
  0x00212C23  eb17                    jmp      0x212c3c                       
                                        ; XREF: 0x00212BEB (cond_jump)
  0x00212C25  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00212C28  6a06                    push     6                              
  0x00212C2A  8bf7                    mov      esi, edi                       
  0x00212C2C  8b7830                  mov      edi, dword ptr [eax + 0x30]    
  0x00212C2F  59                      pop      ecx                            
  0x00212C30  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x00212C32  8b4310                  mov      eax, dword ptr [ebx + 0x10]    
  0x00212C35  c7401418000000          mov      dword ptr [eax + 0x14], 0x18   
                                        ; XREF: 0x00212C23 (jump)
  0x00212C3C  8b4310                  mov      eax, dword ptr [ebx + 0x10]    
  0x00212C3F  83601000                and      dword ptr [eax + 0x10], 0      
                                        ; XREF: 0x00212BF2 (cond_jump)
  0x00212C43  8b7df8                  mov      edi, dword ptr [ebp - 8]       
                                        ; XREF: 0x00212ACC (jump), 0x00212C78 (jump)
  0x00212C46  57                      push     edi                            
  0x00212C47  ff153c7b2100            call     dword ptr [0x217b3c]           ; -> xbox_ExEventObjectType
  0x00212C4D  8b4b10                  mov      ecx, dword ptr [ebx + 0x10]    
  0x00212C50  32d2                    xor      dl, dl                         
  0x00212C52  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x00212C58  ff33                    push     dword ptr [ebx]                
  0x00212C5A  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
  0x00212C60  5f                      pop      edi                            
  0x00212C61  5e                      pop      esi                            
  0x00212C62  5b                      pop      ebx                            
  0x00212C63  c9                      leave                                   
  0x00212C64  c20800                  ret      8                              
                                        ; XREF: 0x00212B18 (cond_jump), 0x00212B28 (cond_jump), 0x00212B34 (cond_jump), 0x00212B3F (cond_jump), 0x00212B4A (cond_jump), ... (+4 more)
  0x00212C67  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00212C6A  83601400                and      dword ptr [eax + 0x14], 0      
  0x00212C6E  8b4310                  mov      eax, dword ptr [ebx + 0x10]    
  0x00212C71  c740104f0100c0          mov      dword ptr [eax + 0x10], 0xc000014f 
  0x00212C78  ebcc                    jmp      0x212c46                       

; ============================================================
; Function: sub_00212C7A
; Start: 0x00212C7A  End: 0x00212CFB  Size: 129 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_001B89B0, sub_00212967, sub_00214E82
; Called by: sub_00213224
; ============================================================
sub_00212C7A:
  0x00212C7A  55                      push     ebp                            
  0x00212C7B  8bec                    mov      ebp, esp                       
  0x00212C7D  51                      push     ecx                            
  0x00212C7E  8b425c                  mov      eax, dword ptr [edx + 0x5c]    
  0x00212C81  53                      push     ebx                            
  0x00212C82  56                      push     esi                            
  0x00212C83  8bf1                    mov      esi, ecx                       
  0x00212C85  57                      push     edi                            
  0x00212C86  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x00212C89  c6464f2f                mov      byte ptr [esi + 0x4f], 0x2f    
  0x00212C8D  8b8e58010000            mov      ecx, dword ptr [esi + 0x158]   
  0x00212C93  8b07                    mov      eax, dword ptr [edi]           
  0x00212C95  8b5704                  mov      edx, dword ptr [edi + 4]       
  0x00212C98  e8135dfaff              call     0x1b89b0                       ; -> sub_001B89B0
  0x00212C9D  8b8e58010000            mov      ecx, dword ptr [esi + 0x158]   
  0x00212CA3  8b5f08                  mov      ebx, dword ptr [edi + 8]       
  0x00212CA6  8bd0                    mov      edx, eax                       
  0x00212CA8  33c0                    xor      eax, eax                       
  0x00212CAA  894628                  mov      dword ptr [esi + 0x28], eax    
  0x00212CAD  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x00212CB0  c74630e1292100          mov      dword ptr [esi + 0x30], 0x2129e1 
  0x00212CB7  d3eb                    shr      ebx, cl                        
  0x00212CB9  668b4f0a                mov      cx, word ptr [edi + 0xa]       
  0x00212CBD  666bc964                imul     cx, cx, 0x64                   
  0x00212CC1  66894e34                mov      word ptr [esi + 0x34], cx      
  0x00212CC5  8d4e4f                  lea      ecx, [esi + 0x4f]              
  0x00212CC8  884637                  mov      byte ptr [esi + 0x37], al      
  0x00212CCB  c6463602                mov      byte ptr [esi + 0x36], 2       
  0x00212CCF  8bf9                    mov      edi, ecx                       
  0x00212CD1  ab                      stosd    dword ptr es:[edi], eax        
  0x00212CD2  ab                      stosd    dword ptr es:[edi], eax        
  0x00212CD3  ab                      stosd    dword ptr es:[edi], eax        
  0x00212CD4  ab                      stosd    dword ptr es:[edi], eax        
  0x00212CD5  52                      push     edx                            
  0x00212CD6  c6012f                  mov      byte ptr [ecx], 0x2f           
  0x00212CD9  e889fcffff              call     0x212967                       ; -> sub_00212967
  0x00212CDE  894651                  mov      dword ptr [esi + 0x51], eax    
  0x00212CE1  887dfc                  mov      byte ptr [ebp - 4], bh         
  0x00212CE4  885dfd                  mov      byte ptr [ebp - 3], bl         
  0x00212CE7  668b45fc                mov      ax, word ptr [ebp - 4]         
  0x00212CEB  8bce                    mov      ecx, esi                       
  0x00212CED  66894656                mov      word ptr [esi + 0x56], ax      
  0x00212CF1  e88c210000              call     0x214e82                       ; -> sub_00214E82
  0x00212CF6  5f                      pop      edi                            
  0x00212CF7  5e                      pop      esi                            
  0x00212CF8  5b                      pop      ebx                            
  0x00212CF9  c9                      leave                                   
  0x00212CFA  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00212CFB
; Start: 0x00212CFB  End: 0x00212DC9  Size: 206 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_002129E1
; ============================================================
sub_00212CFB:
  0x00212CFB  55                      push     ebp                            
  0x00212CFC  8bec                    mov      ebp, esp                       
  0x00212CFE  83ec0c                  sub      esp, 0xc                       
  0x00212D01  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x00212D04  8b482c                  mov      ecx, dword ptr [eax + 0x2c]    
  0x00212D07  57                      push     edi                            
  0x00212D08  8b7828                  mov      edi, dword ptr [eax + 0x28]    
  0x00212D0B  03cf                    add      ecx, edi                       
  0x00212D0D  3bf9                    cmp      edi, ecx                       
  0x00212D0F  894df8                  mov      dword ptr [ebp - 8], ecx       
  0x00212D12  0f83a3000000            jae      0x212dbb                       
  0x00212D18  8bd7                    mov      edx, edi                       
  0x00212D1A  81eadc792100            sub      edx, 0x2179dc                  
  0x00212D20  53                      push     ebx                            
  0x00212D21  8955f4                  mov      dword ptr [ebp - 0xc], edx     
  0x00212D24  56                      push     esi                            
                                        ; XREF: 0x00212DB0 (cond_jump)
  0x00212D25  33c0                    xor      eax, eax                       
  0x00212D27  813f39353146            cmp      dword ptr [edi], 0x46313539    
  0x00212D2D  7570                    jne      0x212d9f                       
                                        ; XREF: 0x00212D46 (cond_jump)
  0x00212D2F  40                      inc      eax                            
  0x00212D30  83f808                  cmp      eax, 8                         
  0x00212D33  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00212D36  7412                    je       0x212d4a                       
  0x00212D38  8bb482dc792100          mov      esi, dword ptr [edx + eax*4 + 0x2179dc] 
  0x00212D3F  3b3485dc792100          cmp      esi, dword ptr [eax*4 + 0x2179dc] 
  0x00212D46  74e7                    je       0x212d2f                       
  0x00212D48  eb55                    jmp      0x212d9f                       
                                        ; XREF: 0x00212D36 (cond_jump)
  0x00212D4A  57                      push     edi                            
  0x00212D4B  ff159c7a2100            call     dword ptr [0x217a9c]           ; -> xbox_MmQueryAddressProtect
  0x00212D51  83ceff                  or       esi, 0xffffffff                
  0x00212D54  837d0c00                cmp      dword ptr [ebp + 0xc], 0       
  0x00212D58  8bd8                    mov      ebx, eax                       
  0x00212D5A  7c07                    jl       0x212d63                       
  0x00212D5C  c7450c3e0000c0          mov      dword ptr [ebp + 0xc], 0xc000003e 
                                        ; XREF: 0x00212D5A (cond_jump)
  0x00212D63  8bc3                    mov      eax, ebx                       
  0x00212D65  83e006                  and      eax, 6                         
  0x00212D68  3c02                    cmp      al, 2                          
  0x00212D6A  7512                    jne      0x212d7e                       
  0x00212D6C  8bf3                    mov      esi, ebx                       
  0x00212D6E  83e6fd                  and      esi, 0xfffffffd                
  0x00212D71  83ce04                  or       esi, 4                         
  0x00212D74  56                      push     esi                            
  0x00212D75  6a20                    push     0x20                           
  0x00212D77  57                      push     edi                            
  0x00212D78  ff15987a2100            call     dword ptr [0x217a98]           ; -> xbox_MmSetAddressProtect
                                        ; XREF: 0x00212D6A (cond_jump), 0x00212D8B (cond_jump)
  0x00212D7E  ff4dfc                  dec      dword ptr [ebp - 4]            
  0x00212D81  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00212D84  c704874641494c          mov      dword ptr [edi + eax*4], 0x4c494146 
  0x00212D8B  75f1                    jne      0x212d7e                       
  0x00212D8D  83feff                  cmp      esi, -1                        
  0x00212D90  740a                    je       0x212d9c                       
  0x00212D92  53                      push     ebx                            
  0x00212D93  6a20                    push     0x20                           
  0x00212D95  57                      push     edi                            
  0x00212D96  ff15987a2100            call     dword ptr [0x217a98]           ; -> xbox_MmSetAddressProtect
                                        ; XREF: 0x00212D90 (cond_jump)
  0x00212D9C  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
                                        ; XREF: 0x00212D2D (cond_jump), 0x00212D48 (jump)
  0x00212D9F  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
  0x00212DA2  b800100000              mov      eax, 0x1000                    
  0x00212DA7  03f8                    add      edi, eax                       
  0x00212DA9  03d0                    add      edx, eax                       
  0x00212DAB  3bf9                    cmp      edi, ecx                       
  0x00212DAD  8955f4                  mov      dword ptr [ebp - 0xc], edx     
  0x00212DB0  0f826fffffff            jb       0x212d25                       
  0x00212DB6  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x00212DB9  5e                      pop      esi                            
  0x00212DBA  5b                      pop      ebx                            
                                        ; XREF: 0x00212D12 (cond_jump)
  0x00212DBB  ff750c                  push     dword ptr [ebp + 0xc]          
  0x00212DBE  50                      push     eax                            
  0x00212DBF  e81dfcffff              call     0x2129e1                       ; -> sub_002129E1
  0x00212DC4  5f                      pop      edi                            
  0x00212DC5  c9                      leave                                   
  0x00212DC6  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00212DC9
; Start: 0x00212DC9  End: 0x00212FAF  Size: 486 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00212967, sub_00212A21, sub_00212A3D, sub_00214E82
; ============================================================
sub_00212DC9:
  0x00212DC9  55                      push     ebp                            
  0x00212DCA  8bec                    mov      ebp, esp                       
  0x00212DCC  53                      push     ebx                            
  0x00212DCD  56                      push     esi                            
  0x00212DCE  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x00212DD1  8b5e0c                  mov      ebx, dword ptr [esi + 0xc]     
  0x00212DD4  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x00212DD7  81e3000000f0            and      ebx, 0xf0000000                
  0x00212DDD  837d0c00                cmp      dword ptr [ebp + 0xc], 0       
  0x00212DE1  57                      push     edi                            
  0x00212DE2  8b795c                  mov      edi, dword ptr [ecx + 0x5c]    
  0x00212DE5  894d08                  mov      dword ptr [ebp + 8], ecx       
  0x00212DE8  0f8cb1000000            jl       0x212e9f                       
  0x00212DEE  81c300000010            add      ebx, 0x10000000                
  0x00212DF4  81fb00000020            cmp      ebx, 0x20000000                
  0x00212DFA  750b                    jne      0x212e07                       
  0x00212DFC  f6460f02                test     byte ptr [esi + 0xf], 2        
  0x00212E00  7518                    jne      0x212e1a                       
  0x00212E02  bb00000040              mov      ebx, 0x40000000                
                                        ; XREF: 0x00212DFA (cond_jump)
  0x00212E07  81fb00000040            cmp      ebx, 0x40000000                
  0x00212E0D  750b                    jne      0x212e1a                       
  0x00212E0F  f6460f04                test     byte ptr [esi + 0xf], 4        
  0x00212E13  7505                    jne      0x212e1a                       
                                        ; XREF: 0x00212EAD (cond_jump), 0x00212EBA (cond_jump)
  0x00212E15  bb00000060              mov      ebx, 0x60000000                
                                        ; XREF: 0x00212E00 (cond_jump), 0x00212E0D (cond_jump), 0x00212E13 (cond_jump), 0x00212ED5 (jump)
  0x00212E1A  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00212E1D  baffffff0f              mov      edx, 0xfffffff                 
  0x00212E22  23c2                    and      eax, edx                       
  0x00212E24  0bc3                    or       eax, ebx                       
  0x00212E26  81fb00000020            cmp      ebx, 0x20000000                
  0x00212E2C  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00212E2F  0f8423010000            je       0x212f58                       
  0x00212E35  81fb00000030            cmp      ebx, 0x30000000                
  0x00212E3B  0f84eb000000            je       0x212f2c                       
  0x00212E41  81fb00000040            cmp      ebx, 0x40000000                
  0x00212E47  0f84bf000000            je       0x212f0c                       
  0x00212E4D  81fb00000050            cmp      ebx, 0x50000000                
  0x00212E53  0f8481000000            je       0x212eda                       
  0x00212E59  81fb00000060            cmp      ebx, 0x60000000                
  0x00212E5F  0f8543010000            jne      0x212fa8                       
  0x00212E65  23c2                    and      eax, edx                       
  0x00212E67  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00212E6A  a1cc704000              mov      eax, dword ptr [0x4070cc]      
  0x00212E6F  8b10                    mov      edx, dword ptr [eax]           
  0x00212E71  8b4004                  mov      eax, dword ptr [eax + 4]       
  0x00212E74  8910                    mov      dword ptr [eax], edx           
  0x00212E76  894204                  mov      dword ptr [edx + 4], eax       
  0x00212E79  8b4704                  mov      eax, dword ptr [edi + 4]       
  0x00212E7C  894114                  mov      dword ptr [ecx + 0x14], eax    
  0x00212E7F  8b450c                  mov      eax, dword ptr [ebp + 0xc]     
  0x00212E82  32d2                    xor      dl, dl                         
  0x00212E84  894110                  mov      dword ptr [ecx + 0x10], eax    
  0x00212E87  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x00212E8D  e88ffbffff              call     0x212a21                       ; -> sub_00212A21
  0x00212E92  ff36                    push     dword ptr [esi]                
  0x00212E94  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
  0x00212E9A  e909010000              jmp      0x212fa8                       
                                        ; XREF: 0x00212DE8 (cond_jump)
  0x00212E9F  81fb00000020            cmp      ebx, 0x20000000                
  0x00212EA5  740c                    je       0x212eb3                       
  0x00212EA7  81fb00000040            cmp      ebx, 0x40000000                
  0x00212EAD  0f8562ffffff            jne      0x212e15                       
                                        ; XREF: 0x00212EA5 (cond_jump)
  0x00212EB3  817d0c3e0000c0          cmp      dword ptr [ebp + 0xc], 0xc000003e 
  0x00212EBA  0f8555ffffff            jne      0x212e15                       
  0x00212EC0  8d4e28                  lea      ecx, [esi + 0x28]              
  0x00212EC3  e875fbffff              call     0x212a3d                       ; -> sub_00212A3D
  0x00212EC8  83650c00                and      dword ptr [ebp + 0xc], 0       
  0x00212ECC  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00212ECF  81c300000010            add      ebx, 0x10000000                
  0x00212ED5  e940ffffff              jmp      0x212e1a                       
                                        ; XREF: 0x00212E53 (cond_jump)
  0x00212EDA  83663800                and      dword ptr [esi + 0x38], 0      
  0x00212EDE  c6463706                mov      byte ptr [esi + 0x37], 6       
  0x00212EE2  8b461c                  mov      eax, dword ptr [esi + 0x1c]    
  0x00212EE5  89463c                  mov      dword ptr [esi + 0x3c], eax    
  0x00212EE8  c6464f2a                mov      byte ptr [esi + 0x4f], 0x2a    
  0x00212EEC  8b4708                  mov      eax, dword ptr [edi + 8]       
  0x00212EEF  2b461c                  sub      eax, dword ptr [esi + 0x1c]    
  0x00212EF2  034704                  add      eax, dword ptr [edi + 4]       
  0x00212EF5  894628                  mov      dword ptr [esi + 0x28], eax    
  0x00212EF8  8b8668010000            mov      eax, dword ptr [esi + 0x168]   
  0x00212EFE  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x00212F01  8b470c                  mov      eax, dword ptr [edi + 0xc]     
  0x00212F04  2b461c                  sub      eax, dword ptr [esi + 0x1c]    
                                        ; XREF: 0x00212F2A (jump)
  0x00212F07  034704                  add      eax, dword ptr [edi + 4]       
  0x00212F0A  eb6d                    jmp      0x212f79                       
                                        ; XREF: 0x00212E47 (cond_jump)
  0x00212F0C  c74628d4704000          mov      dword ptr [esi + 0x28], 0x4070d4 
  0x00212F13  8b8668010000            mov      eax, dword ptr [esi + 0x168]   
  0x00212F19  2b461c                  sub      eax, dword ptr [esi + 0x1c]    
  0x00212F1C  c6463701                mov      byte ptr [esi + 0x37], 1       
  0x00212F20  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x00212F23  c6464f28                mov      byte ptr [esi + 0x4f], 0x28    
  0x00212F27  8b470c                  mov      eax, dword ptr [edi + 0xc]     
  0x00212F2A  ebdb                    jmp      0x212f07                       
                                        ; XREF: 0x00212E3B (cond_jump)
  0x00212F2C  8b462c                  mov      eax, dword ptr [esi + 0x2c]    
  0x00212F2F  894638                  mov      dword ptr [esi + 0x38], eax    
  0x00212F32  c6463706                mov      byte ptr [esi + 0x37], 6       
  0x00212F36  8d8668010000            lea      eax, [esi + 0x168]             
  0x00212F3C  8b08                    mov      ecx, dword ptr [eax]           
  0x00212F3E  894e3c                  mov      dword ptr [esi + 0x3c], ecx    
  0x00212F41  8b4f08                  mov      ecx, dword ptr [edi + 8]       
  0x00212F44  894e28                  mov      dword ptr [esi + 0x28], ecx    
  0x00212F47  8b00                    mov      eax, dword ptr [eax]           
  0x00212F49  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x00212F4C  c6464f2a                mov      byte ptr [esi + 0x4f], 0x2a    
  0x00212F50  8b470c                  mov      eax, dword ptr [edi + 0xc]     
  0x00212F53  2b4638                  sub      eax, dword ptr [esi + 0x38]    
  0x00212F56  eb21                    jmp      0x212f79                       
                                        ; XREF: 0x00212E2F (cond_jump)
  0x00212F58  c74628d4704000          mov      dword ptr [esi + 0x28], 0x4070d4 
  0x00212F5F  8b8668010000            mov      eax, dword ptr [esi + 0x168]   
  0x00212F65  2b4618                  sub      eax, dword ptr [esi + 0x18]    
  0x00212F68  c6463701                mov      byte ptr [esi + 0x37], 1       
  0x00212F6C  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x00212F6F  c6464f28                mov      byte ptr [esi + 0x4f], 0x28    
  0x00212F73  8b470c                  mov      eax, dword ptr [edi + 0xc]     
  0x00212F76  2b462c                  sub      eax, dword ptr [esi + 0x2c]    
                                        ; XREF: 0x00212F0A (jump), 0x00212F56 (jump)
  0x00212F79  8dbe58010000            lea      edi, [esi + 0x158]             
  0x00212F7F  8b0f                    mov      ecx, dword ptr [edi]           
  0x00212F81  d3e8                    shr      eax, cl                        
  0x00212F83  50                      push     eax                            
  0x00212F84  e8def9ffff              call     0x212967                       ; -> sub_00212967
  0x00212F89  894651                  mov      dword ptr [esi + 0x51], eax    
  0x00212F8C  8b0f                    mov      ecx, dword ptr [edi]           
  0x00212F8E  8b462c                  mov      eax, dword ptr [esi + 0x2c]    
  0x00212F91  d3e8                    shr      eax, cl                        
  0x00212F93  8bce                    mov      ecx, esi                       
  0x00212F95  88650e                  mov      byte ptr [ebp + 0xe], ah       
  0x00212F98  88450f                  mov      byte ptr [ebp + 0xf], al       
  0x00212F9B  668b450e                mov      ax, word ptr [ebp + 0xe]       
  0x00212F9F  66894656                mov      word ptr [esi + 0x56], ax      
  0x00212FA3  e8da1e0000              call     0x214e82                       ; -> sub_00214E82
                                        ; XREF: 0x00212E5F (cond_jump), 0x00212E9A (jump)
  0x00212FA8  5f                      pop      edi                            
  0x00212FA9  5e                      pop      esi                            
  0x00212FAA  5b                      pop      ebx                            
  0x00212FAB  5d                      pop      ebp                            
  0x00212FAC  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00212FAF
; Start: 0x00212FAF  End: 0x00212FD8  Size: 41 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00212FAF:
  0x00212FAF  55                      push     ebp                            
  0x00212FB0  8bec                    mov      ebp, esp                       
  0x00212FB2  53                      push     ebx                            
  0x00212FB3  56                      push     esi                            
  0x00212FB4  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x00212FB7  8b5e10                  mov      ebx, dword ptr [esi + 0x10]    
  0x00212FBA  8b535c                  mov      edx, dword ptr [ebx + 0x5c]    
  0x00212FBD  57                      push     edi                            
  0x00212FBE  8b7e0c                  mov      edi, dword ptr [esi + 0xc]     
  0x00212FC1  81e7000000f0            and      edi, 0xf0000000                
  0x00212FC7  837d0c00                cmp      dword ptr [ebp + 0xc], 0       
  0x00212FCB  895508                  mov      dword ptr [ebp + 8], edx       
  0x00212FCE  7c08                    jl       0x212fd8                       
  0x00212FD0  81c700000010            add      edi, 0x10000000                
  0x00212FD6  eb35                    jmp      0x21300d                       
; end of function
                                        ; XREF: 0x00212FCE (cond_jump)
  0x00212FD8  81ff00000020            cmp      edi, 0x20000000                
  0x00212FDE  7408                    je       0x212fe8                       
  0x00212FE0  81ff00000030            cmp      edi, 0x30000000                
  0x00212FE6  7520                    jne      0x213008                       
                                        ; XREF: 0x00212FDE (cond_jump)
  0x00212FE8  817d0c3e0000c0          cmp      dword ptr [ebp + 0xc], 0xc000003e 
  0x00212FEF  7517                    jne      0x213008                       
  0x00212FF1  8d4e28                  lea      ecx, [esi + 0x28]              
  0x00212FF4  e844faffff              call     0x212a3d                       ; -> sub_00212A3D
  0x00212FF9  83650c00                and      dword ptr [ebp + 0xc], 0       
  0x00212FFD  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x00213000  81c700000010            add      edi, 0x10000000                
  0x00213006  eb05                    jmp      0x21300d                       
                                        ; XREF: 0x00212FE6 (cond_jump), 0x00212FEF (cond_jump)
  0x00213008  bf00000050              mov      edi, 0x50000000                
                                        ; XREF: 0x00212FD6 (jump), 0x00213006 (jump)
  0x0021300D  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00213010  b9ffffff0f              mov      ecx, 0xfffffff                 
  0x00213015  23c1                    and      eax, ecx                       
  0x00213017  0bc7                    or       eax, edi                       
  0x00213019  81ff00000020            cmp      edi, 0x20000000                
  0x0021301F  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00213022  0f84ac000000            je       0x2130d4                       
  0x00213028  81ff00000030            cmp      edi, 0x30000000                
  0x0021302E  0f8487000000            je       0x2130bb                       
  0x00213034  81ff00000040            cmp      edi, 0x40000000                
  0x0021303A  7448                    je       0x213084                       
  0x0021303C  81ff00000050            cmp      edi, 0x50000000                
  0x00213042  0f85db000000            jne      0x213123                       
  0x00213048  23c1                    and      eax, ecx                       
  0x0021304A  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x0021304D  a1cc704000              mov      eax, dword ptr [0x4070cc]      
  0x00213052  8b08                    mov      ecx, dword ptr [eax]           
  0x00213054  8b4004                  mov      eax, dword ptr [eax + 4]       
  0x00213057  8908                    mov      dword ptr [eax], ecx           
  0x00213059  894104                  mov      dword ptr [ecx + 4], eax       
  0x0021305C  8b4204                  mov      eax, dword ptr [edx + 4]       
  0x0021305F  894314                  mov      dword ptr [ebx + 0x14], eax    
  0x00213062  8b450c                  mov      eax, dword ptr [ebp + 0xc]     
  0x00213065  32d2                    xor      dl, dl                         
  0x00213067  8bcb                    mov      ecx, ebx                       
  0x00213069  894310                  mov      dword ptr [ebx + 0x10], eax    
  0x0021306C  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x00213072  e8aaf9ffff              call     0x212a21                       ; -> sub_00212A21
  0x00213077  ff36                    push     dword ptr [esi]                
  0x00213079  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
  0x0021307F  e99f000000              jmp      0x213123                       
                                        ; XREF: 0x0021303A (cond_jump)
  0x00213084  c6464f2a                mov      byte ptr [esi + 0x4f], 0x2a    
  0x00213088  c6463706                mov      byte ptr [esi + 0x37], 6       
  0x0021308C  8b8668010000            mov      eax, dword ptr [esi + 0x168]   
  0x00213092  2b4618                  sub      eax, dword ptr [esi + 0x18]    
  0x00213095  894638                  mov      dword ptr [esi + 0x38], eax    
  0x00213098  8b8668010000            mov      eax, dword ptr [esi + 0x168]   
  0x0021309E  2b461c                  sub      eax, dword ptr [esi + 0x1c]    
  0x002130A1  89463c                  mov      dword ptr [esi + 0x3c], eax    
  0x002130A4  8b4208                  mov      eax, dword ptr [edx + 8]       
  0x002130A7  894628                  mov      dword ptr [esi + 0x28], eax    
  0x002130AA  8b8668010000            mov      eax, dword ptr [esi + 0x168]   
  0x002130B0  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x002130B3  8b420c                  mov      eax, dword ptr [edx + 0xc]     
  0x002130B6  2b4638                  sub      eax, dword ptr [esi + 0x38]    
  0x002130B9  eb39                    jmp      0x2130f4                       
                                        ; XREF: 0x0021302E (cond_jump)
  0x002130BB  8b462c                  mov      eax, dword ptr [esi + 0x2c]    
  0x002130BE  05d4704000              add      eax, 0x4070d4                  
  0x002130C3  894628                  mov      dword ptr [esi + 0x28], eax    
  0x002130C6  8b461c                  mov      eax, dword ptr [esi + 0x1c]    
  0x002130C9  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x002130CC  8b420c                  mov      eax, dword ptr [edx + 0xc]     
  0x002130CF  034204                  add      eax, dword ptr [edx + 4]       
  0x002130D2  eb20                    jmp      0x2130f4                       
                                        ; XREF: 0x00213022 (cond_jump)
  0x002130D4  c6463701                mov      byte ptr [esi + 0x37], 1       
  0x002130D8  c6464f28                mov      byte ptr [esi + 0x4f], 0x28    
  0x002130DC  c74628d4704000          mov      dword ptr [esi + 0x28], 0x4070d4 
  0x002130E3  8b8e68010000            mov      ecx, dword ptr [esi + 0x168]   
  0x002130E9  2b4e18                  sub      ecx, dword ptr [esi + 0x18]    
  0x002130EC  894e2c                  mov      dword ptr [esi + 0x2c], ecx    
  0x002130EF  8b420c                  mov      eax, dword ptr [edx + 0xc]     
  0x002130F2  2bc1                    sub      eax, ecx                       
                                        ; XREF: 0x002130B9 (jump), 0x002130D2 (jump)
  0x002130F4  8dbe58010000            lea      edi, [esi + 0x158]             
  0x002130FA  8b0f                    mov      ecx, dword ptr [edi]           
  0x002130FC  d3e8                    shr      eax, cl                        
  0x002130FE  50                      push     eax                            
  0x002130FF  e863f8ffff              call     0x212967                       ; -> sub_00212967
  0x00213104  894651                  mov      dword ptr [esi + 0x51], eax    
  0x00213107  8b0f                    mov      ecx, dword ptr [edi]           
  0x00213109  8b462c                  mov      eax, dword ptr [esi + 0x2c]    
  0x0021310C  d3e8                    shr      eax, cl                        
  0x0021310E  8bce                    mov      ecx, esi                       
  0x00213110  88650e                  mov      byte ptr [ebp + 0xe], ah       
  0x00213113  88450f                  mov      byte ptr [ebp + 0xf], al       
  0x00213116  668b450e                mov      ax, word ptr [ebp + 0xe]       
  0x0021311A  66894656                mov      word ptr [esi + 0x56], ax      
  0x0021311E  e85f1d0000              call     0x214e82                       ; -> sub_00214E82
                                        ; XREF: 0x00213042 (cond_jump), 0x0021307F (jump)
  0x00213123  5f                      pop      edi                            
  0x00213124  5e                      pop      esi                            
  0x00213125  5b                      pop      ebx                            
  0x00213126  5d                      pop      ebp                            
  0x00213127  c20800                  ret      8                              

; ============================================================
; Function: sub_0021312A
; Start: 0x0021312A  End: 0x0021314A  Size: 32 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00213224
; ============================================================
sub_0021312A:
  0x0021312A  56                      push     esi                            
  0x0021312B  57                      push     edi                            
  0x0021312C  6a08                    push     8                              
  0x0021312E  5f                      pop      edi                            
  0x0021312F  57                      push     edi                            
  0x00213130  8bf1                    mov      esi, ecx                       
  0x00213132  ff15087b2100            call     dword ptr [0x217b08]           ; -> xbox_DbgPrint
  0x00213138  85c0                    test     eax, eax                       
  0x0021313A  750e                    jne      0x21314a                       
  0x0021313C  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x0021313F  83601400                and      dword ptr [eax + 0x14], 0      
  0x00213143  b89a0000c0              mov      eax, 0xc000009a                
  0x00213148  eb35                    jmp      0x21317f                       
; end of function
                                        ; XREF: 0x0021313A (cond_jump)
  0x0021314A  894628                  mov      dword ptr [esi + 0x28], eax    
  0x0021314D  897e2c                  mov      dword ptr [esi + 0x2c], edi    
  0x00213150  8d4e4f                  lea      ecx, [esi + 0x4f]              
  0x00213153  c74630942a2100          mov      dword ptr [esi + 0x30], 0x212a94 
  0x0021315A  66c746340a00            mov      word ptr [esi + 0x34], 0xa     
  0x00213160  c6463602                mov      byte ptr [esi + 0x36], 2       
  0x00213164  c6463701                mov      byte ptr [esi + 0x37], 1       
  0x00213168  33c0                    xor      eax, eax                       
  0x0021316A  8bf9                    mov      edi, ecx                       
  0x0021316C  ab                      stosd    dword ptr es:[edi], eax        
  0x0021316D  ab                      stosd    dword ptr es:[edi], eax        
  0x0021316E  ab                      stosd    dword ptr es:[edi], eax        
  0x0021316F  ab                      stosd    dword ptr es:[edi], eax        
  0x00213170  c60125                  mov      byte ptr [ecx], 0x25           
  0x00213173  8bce                    mov      ecx, esi                       
  0x00213175  e8081d0000              call     0x214e82                       ; -> sub_00214E82
  0x0021317A  b803010000              mov      eax, 0x103                     
                                        ; XREF: 0x00213148 (jump)
  0x0021317F  5f                      pop      edi                            
  0x00213180  5e                      pop      esi                            
  0x00213181  c3                      ret                                     

; ============================================================
; Function: sub_00213182
; Start: 0x00213182  End: 0x002131B9  Size: 55 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00213182:
  0x00213182  8b542408                mov      edx, dword ptr [esp + 8]       
  0x00213186  56                      push     esi                            
  0x00213187  8b742408                mov      esi, dword ptr [esp + 8]       
  0x0021318B  80660f0f                and      byte ptr [esi + 0xf], 0xf      
  0x0021318F  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x00213192  57                      push     edi                            
  0x00213193  b8000000c0              mov      eax, 0xc0000000                
  0x00213198  8bfa                    mov      edi, edx                       
  0x0021319A  23f8                    and      edi, eax                       
  0x0021319C  3bf8                    cmp      edi, eax                       
  0x0021319E  7519                    jne      0x2131b9                       
  0x002131A0  83611400                and      dword ptr [ecx + 0x14], 0      
  0x002131A4  895110                  mov      dword ptr [ecx + 0x10], edx    
  0x002131A7  32d2                    xor      dl, dl                         
  0x002131A9  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x002131AF  ff36                    push     dword ptr [esi]                
  0x002131B1  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
  0x002131B7  eb66                    jmp      0x21321f                       
; end of function
                                        ; XREF: 0x0021319E (cond_jump)
  0x002131B9  33c0                    xor      eax, eax                       
  0x002131BB  8d7e4f                  lea      edi, [esi + 0x4f]              
  0x002131BE  ab                      stosd    dword ptr es:[edi], eax        
  0x002131BF  ab                      stosd    dword ptr es:[edi], eax        
  0x002131C0  ab                      stosd    dword ptr es:[edi], eax        
  0x002131C1  ab                      stosd    dword ptr es:[edi], eax        
  0x002131C2  66c746340800            mov      word ptr [esi + 0x34], 8       
  0x002131C8  c6463602                mov      byte ptr [esi + 0x36], 2       
  0x002131CC  f6460f08                test     byte ptr [esi + 0xf], 8        
  0x002131D0  7409                    je       0x2131db                       
  0x002131D2  c74630af2f2100          mov      dword ptr [esi + 0x30], 0x212faf 
  0x002131D9  eb07                    jmp      0x2131e2                       
                                        ; XREF: 0x002131D0 (cond_jump)
  0x002131DB  c74630c92d2100          mov      dword ptr [esi + 0x30], 0x212dc9 
                                        ; XREF: 0x002131D9 (jump)
  0x002131E2  804e0f10                or       byte ptr [esi + 0xf], 0x10     
  0x002131E6  8b3dcc704000            mov      edi, dword ptr [0x4070cc]      
  0x002131EC  bacc704000              mov      edx, 0x4070cc                  
  0x002131F1  3bfa                    cmp      edi, edx                       
  0x002131F3  8d4154                  lea      eax, [ecx + 0x54]              
  0x002131F6  7515                    jne      0x21320d                       
  0x002131F8  8938                    mov      dword ptr [eax], edi           
  0x002131FA  895158                  mov      dword ptr [ecx + 0x58], edx    
  0x002131FD  6a00                    push     0                              
  0x002131FF  894704                  mov      dword ptr [edi + 4], eax       
  0x00213202  56                      push     esi                            
  0x00213203  a3cc704000              mov      dword ptr [0x4070cc], eax      
  0x00213208  ff5630                  call     dword ptr [esi + 0x30]         
  0x0021320B  eb12                    jmp      0x21321f                       
                                        ; XREF: 0x002131F6 (cond_jump)
  0x0021320D  8b35d0704000            mov      esi, dword ptr [0x4070d0]      
  0x00213213  8910                    mov      dword ptr [eax], edx           
  0x00213215  897158                  mov      dword ptr [ecx + 0x58], esi    
  0x00213218  8906                    mov      dword ptr [esi], eax           
  0x0021321A  a3d0704000              mov      dword ptr [0x4070d0], eax      
                                        ; XREF: 0x002131B7 (jump), 0x0021320B (jump)
  0x0021321F  5f                      pop      edi                            
  0x00213220  5e                      pop      esi                            
  0x00213221  c20800                  ret      8                              

; ============================================================
; Function: sub_00213224
; Start: 0x00213224  End: 0x002132A7  Size: 131 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00212C7A, sub_0021312A
; ============================================================
sub_00213224:
  0x00213224  55                      push     ebp                            
  0x00213225  8bec                    mov      ebp, esp                       
  0x00213227  53                      push     ebx                            
  0x00213228  8b5d08                  mov      ebx, dword ptr [ebp + 8]       
  0x0021322B  56                      push     esi                            
  0x0021322C  8b750c                  mov      esi, dword ptr [ebp + 0xc]     
  0x0021322F  8b465c                  mov      eax, dword ptr [esi + 0x5c]    
  0x00213232  8b4010                  mov      eax, dword ptr [eax + 0x10]    
  0x00213235  2d00000700              sub      eax, 0x70000                   
  0x0021323A  57                      push     edi                            
  0x0021323B  746a                    je       0x2132a7                       
  0x0021323D  83e814                  sub      eax, 0x14                      
  0x00213240  7459                    je       0x21329b                       
  0x00213242  2df03f0000              sub      eax, 0x3ff0                    
  0x00213247  740e                    je       0x213257                       
  0x00213249  83661400                and      dword ptr [esi + 0x14], 0      
  0x0021324D  b8100000c0              mov      eax, 0xc0000010                
  0x00213252  e981000000              jmp      0x2132d8                       
                                        ; XREF: 0x00213247 (cond_jump)
  0x00213257  8b5630                  mov      edx, dword ptr [esi + 0x30]    
  0x0021325A  6a08                    push     8                              
  0x0021325C  33c0                    xor      eax, eax                       
  0x0021325E  59                      pop      ecx                            
  0x0021325F  8bfa                    mov      edi, edx                       
  0x00213261  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00213263  8b8360010000            mov      eax, dword ptr [ebx + 0x160]   
  0x00213269  0b8364010000            or       eax, dword ptr [ebx + 0x164]   
  0x0021326F  750b                    jne      0x21327c                       
                                        ; XREF: 0x002132B6 (cond_jump)
  0x00213271  8bd6                    mov      edx, esi                       
  0x00213273  8bcb                    mov      ecx, ebx                       
  0x00213275  e8b0feffff              call     0x21312a                       ; -> sub_0021312A
  0x0021327A  eb55                    jmp      0x2132d1                       
                                        ; XREF: 0x0021326F (cond_jump)
  0x0021327C  8b8360010000            mov      eax, dword ptr [ebx + 0x160]   
  0x00213282  894208                  mov      dword ptr [edx + 8], eax       
  0x00213285  8b8364010000            mov      eax, dword ptr [ebx + 0x164]   
  0x0021328B  89420c                  mov      dword ptr [edx + 0xc], eax     
  0x0021328E  c6421a01                mov      byte ptr [edx + 0x1a], 1       
  0x00213292  c7461420000000          mov      dword ptr [esi + 0x14], 0x20   
  0x00213299  eb34                    jmp      0x2132cf                       
                                        ; XREF: 0x00213240 (cond_jump)
  0x0021329B  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x0021329E  8bd6                    mov      edx, esi                       
  0x002132A0  e8d5f9ffff              call     0x212c7a                       ; -> sub_00212C7A
  0x002132A5  eb46                    jmp      0x2132ed                       
; end of function
                                        ; XREF: 0x0021323B (cond_jump)
  0x002132A7  8b8360010000            mov      eax, dword ptr [ebx + 0x160]   
  0x002132AD  0b8364010000            or       eax, dword ptr [ebx + 0x164]   
  0x002132B3  8b7e30                  mov      edi, dword ptr [esi + 0x30]    
  0x002132B6  74b9                    je       0x213271                       
  0x002132B8  6a06                    push     6                              
  0x002132BA  8db340010000            lea      esi, [ebx + 0x140]             
  0x002132C0  59                      pop      ecx                            
  0x002132C1  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x002132C3  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x002132C6  c7411418000000          mov      dword ptr [ecx + 0x14], 0x18   
  0x002132CD  8bf1                    mov      esi, ecx                       
                                        ; XREF: 0x00213299 (jump)
  0x002132CF  33c0                    xor      eax, eax                       
                                        ; XREF: 0x0021327A (jump)
  0x002132D1  3d03010000              cmp      eax, 0x103                     
  0x002132D6  7415                    je       0x2132ed                       
                                        ; XREF: 0x00213252 (jump)
  0x002132D8  32d2                    xor      dl, dl                         
  0x002132DA  8bce                    mov      ecx, esi                       
  0x002132DC  894610                  mov      dword ptr [esi + 0x10], eax    
  0x002132DF  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x002132E5  ff33                    push     dword ptr [ebx]                
  0x002132E7  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
                                        ; XREF: 0x002132A5 (jump), 0x002132D6 (cond_jump)
  0x002132ED  5f                      pop      edi                            
  0x002132EE  5e                      pop      esi                            
  0x002132EF  5b                      pop      ebx                            
  0x002132F0  5d                      pop      ebp                            
  0x002132F1  c20800                  ret      8                              

; ============================================================
; Function: sub_002132F4
; Start: 0x002132F4  End: 0x00213398  Size: 164 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_002132F4:
  0x002132F4  55                      push     ebp                            
  0x002132F5  8bec                    mov      ebp, esp                       
  0x002132F7  83ec0c                  sub      esp, 0xc                       
  0x002132FA  8365fc00                and      dword ptr [ebp - 4], 0         
  0x002132FE  53                      push     ebx                            
  0x002132FF  8bc2                    mov      eax, edx                       
  0x00213301  56                      push     esi                            
  0x00213302  8bf1                    mov      esi, ecx                       
  0x00213304  8b485c                  mov      ecx, dword ptr [eax + 0x5c]    
  0x00213307  8b510c                  mov      edx, dword ptr [ecx + 0xc]     
  0x0021330A  8b5904                  mov      ebx, dword ptr [ecx + 4]       
  0x0021330D  57                      push     edi                            
  0x0021330E  bfff0f0000              mov      edi, 0xfff                     
  0x00213313  85d7                    test     edi, edx                       
  0x00213315  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x00213318  8955f4                  mov      dword ptr [ebp - 0xc], edx     
  0x0021331B  7553                    jne      0x213370                       
  0x0021331D  85df                    test     edi, ebx                       
  0x0021331F  754f                    jne      0x213370                       
  0x00213321  33c0                    xor      eax, eax                       
  0x00213323  8bfb                    mov      edi, ebx                       
  0x00213325  03fa                    add      edi, edx                       
  0x00213327  134110                  adc      eax, dword ptr [ecx + 0x10]    
  0x0021332A  3b8664010000            cmp      eax, dword ptr [esi + 0x164]   
  0x00213330  7f3b                    jg       0x21336d                       
  0x00213332  7c08                    jl       0x21333c                       
  0x00213334  3bbe60010000            cmp      edi, dword ptr [esi + 0x160]   
  0x0021333A  7731                    ja       0x21336d                       
                                        ; XREF: 0x00213332 (cond_jump)
  0x0021333C  85db                    test     ebx, ebx                       
  0x0021333E  7508                    jne      0x213348                       
  0x00213340  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00213343  215810                  and      dword ptr [eax + 0x10], ebx    
  0x00213346  eb35                    jmp      0x21337d                       
                                        ; XREF: 0x0021333E (cond_jump)
  0x00213348  f6410280                test     byte ptr [ecx + 2], 0x80       
  0x0021334C  740b                    je       0x213359                       
  0x0021334E  8b4108                  mov      eax, dword ptr [ecx + 8]       
  0x00213351  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00213354  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00213357  eb0c                    jmp      0x213365                       
                                        ; XREF: 0x0021334C (cond_jump)
  0x00213359  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x0021335C  8b7830                  mov      edi, dword ptr [eax + 0x30]    
  0x0021335F  037908                  add      edi, dword ptr [ecx + 8]       
  0x00213362  897dfc                  mov      dword ptr [ebp - 4], edi       
                                        ; XREF: 0x00213357 (jump)
  0x00213365  8b7dfc                  mov      edi, dword ptr [ebp - 4]       
  0x00213368  897908                  mov      dword ptr [ecx + 8], edi       
  0x0021336B  eb0a                    jmp      0x213377                       
                                        ; XREF: 0x00213330 (cond_jump), 0x0021333A (cond_jump)
  0x0021336D  8b45f8                  mov      eax, dword ptr [ebp - 8]       
                                        ; XREF: 0x0021331B (cond_jump), 0x0021331F (cond_jump)
  0x00213370  c740100d0000c0          mov      dword ptr [eax + 0x10], 0xc000000d 
                                        ; XREF: 0x0021336B (jump)
  0x00213377  837dfc00                cmp      dword ptr [ebp - 4], 0         
  0x0021337B  751b                    jne      0x213398                       
                                        ; XREF: 0x00213346 (jump)
  0x0021337D  83601400                and      dword ptr [eax + 0x14], 0      
  0x00213381  32d2                    xor      dl, dl                         
  0x00213383  8bc8                    mov      ecx, eax                       
  0x00213385  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x0021338B  ff36                    push     dword ptr [esi]                
  0x0021338D  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
  0x00213393  e9e7000000              jmp      0x21347f                       
; end of function
                                        ; XREF: 0x0021337B (cond_jump)
  0x00213398  33c0                    xor      eax, eax                       
  0x0021339A  8d7e4f                  lea      edi, [esi + 0x4f]              
  0x0021339D  ab                      stosd    dword ptr es:[edi], eax        
  0x0021339E  ab                      stosd    dword ptr es:[edi], eax        
  0x0021339F  ab                      stosd    dword ptr es:[edi], eax        
  0x002133A0  ab                      stosd    dword ptr es:[edi], eax        
  0x002133A1  803903                  cmp      byte ptr [ecx], 3              
  0x002133A4  0f8587000000            jne      0x213431                       
  0x002133AA  8bbe68010000            mov      edi, dword ptr [esi + 0x168]   
  0x002133B0  8bc2                    mov      eax, edx                       
  0x002133B2  33d2                    xor      edx, edx                       
  0x002133B4  f7f7                    div      edi                            
  0x002133B6  80660ff1                and      byte ptr [esi + 0xf], 0xf1     
  0x002133BA  8b4e0c                  mov      ecx, dword ptr [esi + 0xc]     
  0x002133BD  85d2                    test     edx, edx                       
  0x002133BF  742c                    je       0x2133ed                       
  0x002133C1  8bc7                    mov      eax, edi                       
  0x002133C3  2bc2                    sub      eax, edx                       
  0x002133C5  3bc3                    cmp      eax, ebx                       
  0x002133C7  894618                  mov      dword ptr [esi + 0x18], eax    
  0x002133CA  7610                    jbe      0x2133dc                       
  0x002133CC  81c900000008            or       ecx, 0x8000000                 
  0x002133D2  2bc3                    sub      eax, ebx                       
  0x002133D4  894e0c                  mov      dword ptr [esi + 0xc], ecx     
  0x002133D7  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x002133DA  eb28                    jmp      0x213404                       
                                        ; XREF: 0x002133CA (cond_jump)
  0x002133DC  0145f4                  add      dword ptr [ebp - 0xc], eax     
  0x002133DF  0145fc                  add      dword ptr [ebp - 4], eax       
  0x002133E2  2bd8                    sub      ebx, eax                       
  0x002133E4  81c900000002            or       ecx, 0x2000000                 
  0x002133EA  894e0c                  mov      dword ptr [esi + 0xc], ecx     
                                        ; XREF: 0x002133BF (cond_jump)
  0x002133ED  33d2                    xor      edx, edx                       
  0x002133EF  8bc3                    mov      eax, ebx                       
  0x002133F1  f7f7                    div      edi                            
  0x002133F3  85d2                    test     edx, edx                       
  0x002133F5  7409                    je       0x213400                       
  0x002133F7  2bda                    sub      ebx, edx                       
  0x002133F9  804e0f04                or       byte ptr [esi + 0xf], 4        
  0x002133FD  89561c                  mov      dword ptr [esi + 0x1c], edx    
                                        ; XREF: 0x002133F5 (cond_jump)
  0x00213400  85db                    test     ebx, ebx                       
  0x00213402  750a                    jne      0x21340e                       
                                        ; XREF: 0x002133DA (jump)
  0x00213404  6a00                    push     0                              
  0x00213406  56                      push     esi                            
  0x00213407  e876fdffff              call     0x213182                       ; -> sub_00213182
  0x0021340C  eb71                    jmp      0x21347f                       
                                        ; XREF: 0x00213402 (cond_jump)
  0x0021340E  f6460f0e                test     byte ptr [esi + 0xf], 0xe      
  0x00213412  7409                    je       0x21341d                       
  0x00213414  c7463082312100          mov      dword ptr [esi + 0x30], 0x213182 
  0x0021341B  eb07                    jmp      0x213424                       
                                        ; XREF: 0x00213412 (cond_jump)
  0x0021341D  c74630e1292100          mov      dword ptr [esi + 0x30], 0x2129e1 
                                        ; XREF: 0x0021341B (jump)
  0x00213424  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
  0x00213427  c6463702                mov      byte ptr [esi + 0x37], 2       
  0x0021342B  c6464f2a                mov      byte ptr [esi + 0x4f], 0x2a    
  0x0021342F  eb0f                    jmp      0x213440                       
                                        ; XREF: 0x002133A4 (cond_jump)
  0x00213431  c74630fb2c2100          mov      dword ptr [esi + 0x30], 0x212cfb 
  0x00213438  c6463701                mov      byte ptr [esi + 0x37], 1       
  0x0021343C  c6464f28                mov      byte ptr [esi + 0x4f], 0x28    
                                        ; XREF: 0x0021342F (jump)
  0x00213440  8bbe58010000            mov      edi, dword ptr [esi + 0x158]   
  0x00213446  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00213449  8bcf                    mov      ecx, edi                       
  0x0021344B  d3ea                    shr      edx, cl                        
  0x0021344D  894628                  mov      dword ptr [esi + 0x28], eax    
  0x00213450  895e2c                  mov      dword ptr [esi + 0x2c], ebx    
  0x00213453  66c746340800            mov      word ptr [esi + 0x34], 8       
  0x00213459  52                      push     edx                            
  0x0021345A  c6463602                mov      byte ptr [esi + 0x36], 2       
  0x0021345E  e804f5ffff              call     0x212967                       ; -> sub_00212967
  0x00213463  8bcf                    mov      ecx, edi                       
  0x00213465  d3eb                    shr      ebx, cl                        
  0x00213467  894651                  mov      dword ptr [esi + 0x51], eax    
  0x0021346A  8bce                    mov      ecx, esi                       
  0x0021346C  887dfe                  mov      byte ptr [ebp - 2], bh         
  0x0021346F  885dff                  mov      byte ptr [ebp - 1], bl         
  0x00213472  668b45fe                mov      ax, word ptr [ebp - 2]         
  0x00213476  66894656                mov      word ptr [esi + 0x56], ax      
  0x0021347A  e8031a0000              call     0x214e82                       ; -> sub_00214E82
                                        ; XREF: 0x00213393 (jump), 0x0021340C (jump)
  0x0021347F  5f                      pop      edi                            
  0x00213480  5e                      pop      esi                            
  0x00213481  5b                      pop      ebx                            
  0x00213482  c9                      leave                                   
  0x00213483  c3                      ret                                     

; ============================================================
; Function: sub_00213484
; Start: 0x00213484  End: 0x002134BD  Size: 57 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00213484:
  0x00213484  55                      push     ebp                            
  0x00213485  8bec                    mov      ebp, esp                       
  0x00213487  53                      push     ebx                            
  0x00213488  8b5d08                  mov      ebx, dword ptr [ebp + 8]       
  0x0021348B  8b4318                  mov      eax, dword ptr [ebx + 0x18]    
  0x0021348E  56                      push     esi                            
  0x0021348F  57                      push     edi                            
  0x00213490  8b7d0c                  mov      edi, dword ptr [ebp + 0xc]     
  0x00213493  8b775c                  mov      esi, dword ptr [edi + 0x5c]    
  0x00213496  894508                  mov      dword ptr [ebp + 8], eax       
  0x00213499  ff15507b2100            call     dword ptr [0x217b50]           ; -> xbox_HalIsResetOrShutdownPending
  0x0021349F  84c0                    test     al, al                         
  0x002134A1  741a                    je       0x2134bd                       
  0x002134A3  32d2                    xor      dl, dl                         
  0x002134A5  8bcf                    mov      ecx, edi                       
  0x002134A7  c74710400200c0          mov      dword ptr [edi + 0x10], 0xc0000240 
  0x002134AE  ff15447b2100            call     dword ptr [0x217b44]           ; -> xbox_IoSynchronousFsdRequest
  0x002134B4  53                      push     ebx                            
  0x002134B5  ff15487b2100            call     dword ptr [0x217b48]           ; -> xbox_IoSetIoCompletion
  0x002134BB  eb31                    jmp      0x2134ee                       
; end of function
                                        ; XREF: 0x002134A1 (cond_jump)
  0x002134BD  895e14                  mov      dword ptr [esi + 0x14], ebx    
  0x002134C0  8b5d08                  mov      ebx, dword ptr [ebp + 8]       
  0x002134C3  897b10                  mov      dword ptr [ebx + 0x10], edi    
  0x002134C6  0fb606                  movzx    eax, byte ptr [esi]            
  0x002134C9  48                      dec      eax                            
  0x002134CA  48                      dec      eax                            
  0x002134CB  7418                    je       0x2134e5                       
  0x002134CD  48                      dec      eax                            
  0x002134CE  740e                    je       0x2134de                       
  0x002134D0  83e807                  sub      eax, 7                         
  0x002134D3  7519                    jne      0x2134ee                       
  0x002134D5  57                      push     edi                            
  0x002134D6  53                      push     ebx                            
  0x002134D7  e848fdffff              call     0x213224                       ; -> sub_00213224
  0x002134DC  eb10                    jmp      0x2134ee                       
                                        ; XREF: 0x002134CE (cond_jump)
  0x002134DE  57                      push     edi                            
  0x002134DF  ff154c7b2100            call     dword ptr [0x217b4c]           ; -> xbox_IoMarkIrpMustComplete
                                        ; XREF: 0x002134CB (cond_jump)
  0x002134E5  8bd7                    mov      edx, edi                       
  0x002134E7  8bcb                    mov      ecx, ebx                       
  0x002134E9  e806feffff              call     0x2132f4                       ; -> sub_002132F4
                                        ; XREF: 0x002134BB (jump), 0x002134D3 (cond_jump), 0x002134DC (jump)
  0x002134EE  5f                      pop      edi                            
  0x002134EF  5e                      pop      esi                            
  0x002134F0  5b                      pop      ebx                            
  0x002134F1  5d                      pop      ebp                            
  0x002134F2  c20800                  ret      8                              
  0x002134F5  8bc1                    mov      eax, ecx                       
  0x002134F7  8008ff                  or       byte ptr [eax], 0xff           
  0x002134FA  c6400180                mov      byte ptr [eax + 1], 0x80       
  0x002134FE  c6400280                mov      byte ptr [eax + 2], 0x80       
  0x00213502  c6400380                mov      byte ptr [eax + 3], 0x80       
  0x00213506  c3                      ret                                     

; ============================================================
; Function: sub_00213507
; Start: 0x00213507  End: 0x00213539  Size: 50 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002137C4, sub_0021392D, sub_002139E9
; ============================================================
sub_00213507:
  0x00213507  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021350B  53                      push     ebx                            
  0x0021350C  56                      push     esi                            
  0x0021350D  8bf1                    mov      esi, ecx                       
  0x0021350F  8b8ee0000000            mov      ecx, dword ptr [esi + 0xe0]    
  0x00213515  2bc1                    sub      eax, ecx                       
  0x00213517  c1f805                  sar      eax, 5                         
  0x0021351A  0fb6d0                  movzx    edx, al                        
  0x0021351D  c1e205                  shl      edx, 5                         
  0x00213520  800c0aff                or       byte ptr [edx + ecx], 0xff     
  0x00213524  8a5e79                  mov      bl, byte ptr [esi + 0x79]      
  0x00213527  8b8ee0000000            mov      ecx, dword ptr [esi + 0xe0]    
  0x0021352D  885c0a01                mov      byte ptr [edx + ecx + 1], bl   
  0x00213531  884679                  mov      byte ptr [esi + 0x79], al      
  0x00213534  5e                      pop      esi                            
  0x00213535  5b                      pop      ebx                            
  0x00213536  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00213539
; Start: 0x00213539  End: 0x0021354D  Size: 20 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002137C4, sub_002139A4, sub_002139E9, sub_00213AB0, sub_002143F3, sub_00214479, sub_00214750
; ============================================================
sub_00213539:
  0x00213539  8a4101                  mov      al, byte ptr [ecx + 1]         
  0x0021353C  3c80                    cmp      al, 0x80                       
  0x0021353E  740d                    je       0x21354d                       
  0x00213540  0fb6c0                  movzx    eax, al                        
  0x00213543  c1e005                  shl      eax, 5                         
  0x00213546  030560a24000            add      eax, dword ptr [0x40a260]      
  0x0021354C  c3                      ret                                     
; end of function
                                        ; XREF: 0x0021353E (cond_jump)
  0x0021354D  33c0                    xor      eax, eax                       
  0x0021354F  c3                      ret                                     

; ============================================================
; Function: sub_00213550
; Start: 0x00213550  End: 0x00213564  Size: 20 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_0021383A, sub_00213DC4, sub_00214479, sub_002144E5, sub_00214516, sub_0021455E
; ============================================================
sub_00213550:
  0x00213550  8a4102                  mov      al, byte ptr [ecx + 2]         
  0x00213553  3c80                    cmp      al, 0x80                       
  0x00213555  740d                    je       0x213564                       
  0x00213557  0fb6c0                  movzx    eax, al                        
  0x0021355A  c1e005                  shl      eax, 5                         
  0x0021355D  030560a24000            add      eax, dword ptr [0x40a260]      
  0x00213563  c3                      ret                                     
; end of function
                                        ; XREF: 0x00213555 (cond_jump)
  0x00213564  33c0                    xor      eax, eax                       
  0x00213566  c3                      ret                                     

; ============================================================
; Function: sub_00213567
; Start: 0x00213567  End: 0x0021357B  Size: 20 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_0021383A, sub_00214479, sub_002144E5, sub_00214516, sub_0021455E
; ============================================================
sub_00213567:
  0x00213567  8a4103                  mov      al, byte ptr [ecx + 3]         
  0x0021356A  3c80                    cmp      al, 0x80                       
  0x0021356C  740d                    je       0x21357b                       
  0x0021356E  0fb6c0                  movzx    eax, al                        
  0x00213571  c1e005                  shl      eax, 5                         
  0x00213574  030560a24000            add      eax, dword ptr [0x40a260]      
  0x0021357A  c3                      ret                                     
; end of function
                                        ; XREF: 0x0021356C (cond_jump)
  0x0021357B  33c0                    xor      eax, eax                       
  0x0021357D  c3                      ret                                     

; ============================================================
; Function: sub_0021357E
; Start: 0x0021357E  End: 0x00213645  Size: 199 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00212885, sub_00214516
; Called by: sub_00213748, sub_002139E9
; ============================================================
sub_0021357E:
  0x0021357E  53                      push     ebx                            
  0x0021357F  56                      push     esi                            
  0x00213580  57                      push     edi                            
  0x00213581  8bf9                    mov      edi, ecx                       
  0x00213583  b980a14000              mov      ecx, 0x40a180                  
  0x00213588  e8f8f2ffff              call     0x212885                       ; -> sub_00212885
  0x0021358D  8bf0                    mov      esi, eax                       
  0x0021358F  33db                    xor      ebx, ebx                       
  0x00213591  3bf3                    cmp      esi, ebx                       
  0x00213593  0f84a6000000            je       0x21363f                       
  0x00213599  8a442410                mov      al, byte ptr [esp + 0x10]      
  0x0021359D  c606fe                  mov      byte ptr [esi], 0xfe           
  0x002135A0  884604                  mov      byte ptr [esi + 4], al         
  0x002135A3  895e10                  mov      dword ptr [esi + 0x10], ebx    
  0x002135A6  8b470c                  mov      eax, dword ptr [edi + 0xc]     
  0x002135A9  56                      push     esi                            
  0x002135AA  8bcf                    mov      ecx, edi                       
  0x002135AC  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x002135AF  e8620f0000              call     0x214516                       ; -> sub_00214516
  0x002135B4  381d80a14000            cmp      byte ptr [0x40a180], bl        
  0x002135BA  8a442414                mov      al, byte ptr [esp + 0x14]      
  0x002135BE  743a                    je       0x2135fa                       
  0x002135C0  884605                  mov      byte ptr [esi + 5], al         
  0x002135C3  ff15547b2100            call     dword ptr [0x217b54]           ; -> xbox_KeQueryInterruptTime
  0x002135C9  894618                  mov      dword ptr [esi + 0x18], eax    
  0x002135CC  814618a0860100          add      dword ptr [esi + 0x18], 0x186a0 
  0x002135D3  89561c                  mov      dword ptr [esi + 0x1c], edx    
  0x002135D6  895e10                  mov      dword ptr [esi + 0x10], ebx    
  0x002135D9  115e1c                  adc      dword ptr [esi + 0x1c], ebx    
  0x002135DC  a1fca14000              mov      eax, dword ptr [0x40a1fc]      
  0x002135E1  3bc3                    cmp      eax, ebx                       
  0x002135E3  750b                    jne      0x2135f0                       
  0x002135E5  8935fca14000            mov      dword ptr [0x40a1fc], esi      
  0x002135EB  eb52                    jmp      0x21363f                       
                                        ; XREF: 0x002135F3 (cond_jump)
  0x002135ED  8b4010                  mov      eax, dword ptr [eax + 0x10]    
                                        ; XREF: 0x002135E3 (cond_jump)
  0x002135F0  395810                  cmp      dword ptr [eax + 0x10], ebx    
  0x002135F3  75f8                    jne      0x2135ed                       
  0x002135F5  897010                  mov      dword ptr [eax + 0x10], esi    
  0x002135F8  eb45                    jmp      0x21363f                       
                                        ; XREF: 0x002135BE (cond_jump)
  0x002135FA  c606fd                  mov      byte ptr [esi], 0xfd           
  0x002135FD  68b4a14000              push     0x40a1b4                       
  0x00213602  a282a14000              mov      byte ptr [0x40a182], al        
  0x00213607  83c9ff                  or       ecx, 0xffffffff                
  0x0021360A  51                      push     ecx                            
  0x0021360B  b8c0bdf0ff              mov      eax, 0xfff0bdc0                
  0x00213610  50                      push     eax                            
  0x00213611  c60580a1400001          mov      byte ptr [0x40a180], 1         
  0x00213618  881d81a14000            mov      byte ptr [0x40a181], bl        
  0x0021361E  893500a24000            mov      dword ptr [0x40a200], esi      
  0x00213624  c60583a1400080          mov      byte ptr [0x40a183], 0x80      
  0x0021362B  885e05                  mov      byte ptr [esi + 5], bl         
  0x0021362E  68d0a14000              push     0x40a1d0                       
  0x00213633  881df8a14000            mov      byte ptr [0x40a1f8], bl        
  0x00213639  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
                                        ; XREF: 0x00213593 (cond_jump), 0x002135EB (jump), 0x002135F8 (jump)
  0x0021363F  5f                      pop      edi                            
  0x00213640  5e                      pop      esi                            
  0x00213641  5b                      pop      ebx                            
  0x00213642  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00213645
; Start: 0x00213645  End: 0x00213667  Size: 34 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00213645:
  0x00213645  68b4a14000              push     0x40a1b4                       
  0x0021364A  83c9ff                  or       ecx, 0xffffffff                
  0x0021364D  51                      push     ecx                            
  0x0021364E  b8800f05fd              mov      eax, 0xfd050f80                
  0x00213653  50                      push     eax                            
  0x00213654  68d0a14000              push     0x40a1d0                       
  0x00213659  c605f8a1400001          mov      byte ptr [0x40a1f8], 1         
  0x00213660  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x00213666  c3                      ret                                     
; end of function
                                        ; XREF: 0x002137BF (jump)
  0x00213667  837c240400              cmp      dword ptr [esp + 4], 0         
  0x0021366C  c60583a1400081          mov      byte ptr [0x40a183], 0x81      
  0x00213673  7d0d                    jge      0x213682                       
  0x00213675  ff742408                push     dword ptr [esp + 8]            
  0x00213679  6a00                    push     0                              
  0x0021367B  e830040000              call     0x213ab0                       ; -> sub_00213AB0
  0x00213680  eb33                    jmp      0x2136b5                       
                                        ; XREF: 0x00213673 (cond_jump)
  0x00213682  817c240400000001        cmp      dword ptr [esp + 4], 0x1000000 
  0x0021368A  7508                    jne      0x213694                       
  0x0021368C  8b442408                mov      eax, dword ptr [esp + 8]       
  0x00213690  80480480                or       byte ptr [eax + 4], 0x80       
                                        ; XREF: 0x0021368A (cond_jump)
  0x00213694  68b4a14000              push     0x40a1b4                       
  0x00213699  83c9ff                  or       ecx, 0xffffffff                
  0x0021369C  51                      push     ecx                            
  0x0021369D  b86079feff              mov      eax, 0xfffe7960                
  0x002136A2  50                      push     eax                            
  0x002136A3  68d0a14000              push     0x40a1d0                       
  0x002136A8  c605f8a1400002          mov      byte ptr [0x40a1f8], 2         
  0x002136AF  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
                                        ; XREF: 0x00213680 (jump)
  0x002136B5  c20800                  ret      8                              
  0x002136B8  68b4a14000              push     0x40a1b4                       
  0x002136BD  83c9ff                  or       ecx, 0xffffffff                
  0x002136C0  51                      push     ecx                            
  0x002136C1  b8e0b1ffff              mov      eax, 0xffffb1e0                
  0x002136C6  50                      push     eax                            
  0x002136C7  68d0a14000              push     0x40a1d0                       
  0x002136CC  c60583a1400084          mov      byte ptr [0x40a183], 0x84      
  0x002136D3  c605f8a1400003          mov      byte ptr [0x40a1f8], 3         
  0x002136DA  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x002136E0  c20800                  ret      8                              

; ============================================================
; Function: sub_002136E3
; Start: 0x002136E3  End: 0x00213706  Size: 35 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_002136E3:
  0x002136E3  53                      push     ebx                            
  0x002136E4  33d2                    xor      edx, edx                       
  0x002136E6  32db                    xor      bl, bl                         
  0x002136E8  42                      inc      edx                            
  0x002136E9  56                      push     esi                            
  0x002136EA  8ac2                    mov      al, dl                         
                                        ; XREF: 0x00213702 (cond_jump)
  0x002136EC  0fb6f3                  movzx    esi, bl                        
  0x002136EF  8554b108                test     dword ptr [ecx + esi*4 + 8], edx 
  0x002136F3  7411                    je       0x213706                       
  0x002136F5  d1e2                    shl      edx, 1                         
  0x002136F7  7505                    jne      0x2136fe                       
  0x002136F9  33d2                    xor      edx, edx                       
  0x002136FB  fec3                    inc      bl                             
  0x002136FD  42                      inc      edx                            
                                        ; XREF: 0x002136F7 (cond_jump)
  0x002136FE  fec0                    inc      al                             
  0x00213700  3c80                    cmp      al, 0x80                       
  0x00213702  72e8                    jb       0x2136ec                       
  0x00213704  eb09                    jmp      0x21370f                       
; end of function
                                        ; XREF: 0x002136F3 (cond_jump)
  0x00213706  0fb6f3                  movzx    esi, bl                        
  0x00213709  8d4cb108                lea      ecx, [ecx + esi*4 + 8]         
  0x0021370D  0911                    or       dword ptr [ecx], edx           
                                        ; XREF: 0x00213704 (jump)
  0x0021370F  5e                      pop      esi                            
  0x00213710  247f                    and      al, 0x7f                       
  0x00213712  5b                      pop      ebx                            
  0x00213713  c3                      ret                                     

; ============================================================
; Function: sub_00213714
; Start: 0x00213714  End: 0x00213748  Size: 52 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002137C4, sub_002139E9
; ============================================================
sub_00213714:
  0x00213714  53                      push     ebx                            
  0x00213715  32db                    xor      bl, bl                         
  0x00213717  feca                    dec      dl                             
  0x00213719  80fa1f                  cmp      dl, 0x1f                       
  0x0021371C  56                      push     esi                            
  0x0021371D  7614                    jbe      0x213733                       
  0x0021371F  8ac2                    mov      al, dl                         
  0x00213721  2c20                    sub      al, 0x20                       
  0x00213723  c0e805                  shr      al, 5                          
  0x00213726  fec0                    inc      al                             
  0x00213728  0fb6c0                  movzx    eax, al                        
  0x0021372B  8ad8                    mov      bl, al                         
                                        ; XREF: 0x00213731 (cond_jump)
  0x0021372D  80c2e0                  add      dl, 0xe0                       
  0x00213730  48                      dec      eax                            
  0x00213731  75fa                    jne      0x21372d                       
                                        ; XREF: 0x0021371D (cond_jump)
  0x00213733  0fb6c3                  movzx    eax, bl                        
  0x00213736  33f6                    xor      esi, esi                       
  0x00213738  8d448108                lea      eax, [ecx + eax*4 + 8]         
  0x0021373C  46                      inc      esi                            
  0x0021373D  8aca                    mov      cl, dl                         
  0x0021373F  d3e6                    shl      esi, cl                        
  0x00213741  f7d6                    not      esi                            
  0x00213743  2130                    and      dword ptr [eax], esi           
  0x00213745  5e                      pop      esi                            
  0x00213746  5b                      pop      ebx                            
  0x00213747  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00213748
; Start: 0x00213748  End: 0x0021375D  Size: 21 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021357E
; Called by: sub_00216048
; ============================================================
sub_00213748:
  0x00213748  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021374C  8b48ec                  mov      ecx, dword ptr [eax - 0x14]    
  0x0021374F  6a05                    push     5                              
  0x00213751  ff74240c                push     dword ptr [esp + 0xc]          
  0x00213755  e824feffff              call     0x21357e                       ; -> sub_0021357E
  0x0021375A  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_0021375D
; Start: 0x0021375D  End: 0x0021377F  Size: 34 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213AB0
; ============================================================
sub_0021375D:
  0x0021375D  53                      push     ebx                            
  0x0021375E  56                      push     esi                            
  0x0021375F  8b3500a24000            mov      esi, dword ptr [0x40a200]      
  0x00213765  57                      push     edi                            
  0x00213766  8b7e0c                  mov      edi, dword ptr [esi + 0xc]     
  0x00213769  33db                    xor      ebx, ebx                       
  0x0021376B  83c718                  add      edi, 0x18                      
  0x0021376E  381d81a14000            cmp      byte ptr [0x40a181], bl        
  0x00213774  7409                    je       0x21377f                       
  0x00213776  56                      push     esi                            
  0x00213777  53                      push     ebx                            
  0x00213778  e833030000              call     0x213ab0                       ; -> sub_00213AB0
  0x0021377D  eb3c                    jmp      0x2137bb                       
; end of function
                                        ; XREF: 0x00213774 (cond_jump)
  0x0021377F  8bce                    mov      ecx, esi                       
  0x00213781  881d83a14000            mov      byte ptr [0x40a183], bl        
  0x00213787  e8adfdffff              call     0x213539                       ; -> sub_00213539
  0x0021378C  3818                    cmp      byte ptr [eax], bl             
  0x0021378E  7517                    jne      0x2137a7                       
  0x00213790  33c0                    xor      eax, eax                       
  0x00213792  8a4604                  mov      al, byte ptr [esi + 4]         
  0x00213795  56                      push     esi                            
  0x00213796  6867362100              push     0x213667                       
  0x0021379B  83e07f                  and      eax, 0x7f                      
  0x0021379E  50                      push     eax                            
  0x0021379F  57                      push     edi                            
  0x002137A0  e832290000              call     0x2160d7                       ; -> sub_002160D7
  0x002137A5  eb14                    jmp      0x2137bb                       
                                        ; XREF: 0x0021378E (cond_jump)
  0x002137A7  33c9                    xor      ecx, ecx                       
  0x002137A9  8a4e04                  mov      cl, byte ptr [esi + 4]         
  0x002137AC  53                      push     ebx                            
  0x002137AD  56                      push     esi                            
  0x002137AE  81e17fffffff            and      ecx, 0xffffff7f                
  0x002137B4  51                      push     ecx                            
  0x002137B5  50                      push     eax                            
  0x002137B6  e8ae1f0000              call     0x215769                       ; -> sub_00215769
                                        ; XREF: 0x0021377D (jump), 0x002137A5 (jump)
  0x002137BB  5f                      pop      edi                            
  0x002137BC  5e                      pop      esi                            
  0x002137BD  5b                      pop      ebx                            
  0x002137BE  c3                      ret                                     

; ============================================================
; Function: sub_002137BF
; Start: 0x002137BF  End: 0x002137C4  Size: 5 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002155F9
; ============================================================
sub_002137BF:
  0x002137BF  e9a3feffff              jmp      0x213667                       
; end of function

; ============================================================
; Function: sub_002137C4
; Start: 0x002137C4  End: 0x0021383A  Size: 118 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00211556, sub_00213507, sub_00213539, sub_00213714, sub_0021455E
; Called by: sub_00211B6B, sub_0021383A
; ============================================================
sub_002137C4:
  0x002137C4  53                      push     ebx                            
  0x002137C5  56                      push     esi                            
  0x002137C6  8bf1                    mov      esi, ecx                       
  0x002137C8  8a4607                  mov      al, byte ptr [esi + 7]         
  0x002137CB  3cff                    cmp      al, 0xff                       
  0x002137CD  57                      push     edi                            
  0x002137CE  741b                    je       0x2137eb                       
  0x002137D0  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x002137D3  8b4914                  mov      ecx, dword ptr [ecx + 0x14]    
  0x002137D6  0fb6c0                  movzx    eax, al                        
  0x002137D9  8b0481                  mov      eax, dword ptr [ecx + eax*4]   
  0x002137DC  85c0                    test     eax, eax                       
  0x002137DE  740b                    je       0x2137eb                       
  0x002137E0  6a00                    push     0                              
  0x002137E2  ff7614                  push     dword ptr [esi + 0x14]         
  0x002137E5  50                      push     eax                            
  0x002137E6  e86bddffff              call     0x211556                       ; -> sub_00211556
                                        ; XREF: 0x002137CE (cond_jump), 0x002137DE (cond_jump)
  0x002137EB  803e05                  cmp      byte ptr [esi], 5              
  0x002137EE  bb80a14000              mov      ebx, 0x40a180                  
  0x002137F3  752a                    jne      0x21381f                       
  0x002137F5  8bce                    mov      ecx, esi                       
  0x002137F7  e83dfdffff              call     0x213539                       ; -> sub_00213539
  0x002137FC  8bf8                    mov      edi, eax                       
  0x002137FE  56                      push     esi                            
  0x002137FF  8bcf                    mov      ecx, edi                       
  0x00213801  e8580d0000              call     0x21455e                       ; -> sub_0021455E
  0x00213806  84c0                    test     al, al                         
  0x00213808  7524                    jne      0x21382e                       
  0x0021380A  8a5705                  mov      dl, byte ptr [edi + 5]         
  0x0021380D  8b4f0c                  mov      ecx, dword ptr [edi + 0xc]     
  0x00213810  e8fffeffff              call     0x213714                       ; -> sub_00213714
  0x00213815  57                      push     edi                            
  0x00213816  8bcb                    mov      ecx, ebx                       
  0x00213818  e8eafcffff              call     0x213507                       ; -> sub_00213507
  0x0021381D  eb0f                    jmp      0x21382e                       
                                        ; XREF: 0x002137F3 (cond_jump)
  0x0021381F  8a5605                  mov      dl, byte ptr [esi + 5]         
  0x00213822  84d2                    test     dl, dl                         
  0x00213824  7408                    je       0x21382e                       
  0x00213826  8b4e0c                  mov      ecx, dword ptr [esi + 0xc]     
  0x00213829  e8e6feffff              call     0x213714                       ; -> sub_00213714
                                        ; XREF: 0x00213808 (cond_jump), 0x0021381D (jump), 0x00213824 (cond_jump)
  0x0021382E  56                      push     esi                            
  0x0021382F  8bcb                    mov      ecx, ebx                       
  0x00213831  e8d1fcffff              call     0x213507                       ; -> sub_00213507
  0x00213836  5f                      pop      edi                            
  0x00213837  5e                      pop      esi                            
  0x00213838  5b                      pop      ebx                            
  0x00213839  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_0021383A
; Start: 0x0021383A  End: 0x0021388C  Size: 82 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213550, sub_00213567, sub_002137C4
; Called by: sub_0021388C, sub_0021392D
; ============================================================
sub_0021383A:
  0x0021383A  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x0021383E  803904                  cmp      byte ptr [ecx], 4              
  0x00213841  56                      push     esi                            
  0x00213842  7532                    jne      0x213876                       
  0x00213844  e807fdffff              call     0x213550                       ; -> sub_00213550
  0x00213849  8bf0                    mov      esi, eax                       
  0x0021384B  85f6                    test     esi, esi                       
  0x0021384D  7439                    je       0x213888                       
  0x0021384F  57                      push     edi                            
                                        ; XREF: 0x00213871 (cond_jump)
  0x00213850  8bce                    mov      ecx, esi                       
  0x00213852  e810fdffff              call     0x213567                       ; -> sub_00213567
  0x00213857  8bf8                    mov      edi, eax                       
  0x00213859  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x0021385C  85c0                    test     eax, eax                       
  0x0021385E  7406                    je       0x213866                       
  0x00213860  56                      push     esi                            
  0x00213861  ff500c                  call     dword ptr [eax + 0xc]          
  0x00213864  eb07                    jmp      0x21386d                       
                                        ; XREF: 0x0021385E (cond_jump)
  0x00213866  8bce                    mov      ecx, esi                       
  0x00213868  e857ffffff              call     0x2137c4                       ; -> sub_002137C4
                                        ; XREF: 0x00213864 (jump)
  0x0021386D  85ff                    test     edi, edi                       
  0x0021386F  8bf7                    mov      esi, edi                       
  0x00213871  75dd                    jne      0x213850                       
  0x00213873  5f                      pop      edi                            
  0x00213874  eb12                    jmp      0x213888                       
                                        ; XREF: 0x00213842 (cond_jump)
  0x00213876  8b4110                  mov      eax, dword ptr [ecx + 0x10]    
  0x00213879  85c0                    test     eax, eax                       
  0x0021387B  7406                    je       0x213883                       
  0x0021387D  51                      push     ecx                            
  0x0021387E  ff500c                  call     dword ptr [eax + 0xc]          
  0x00213881  eb05                    jmp      0x213888                       
                                        ; XREF: 0x0021387B (cond_jump)
  0x00213883  e83cffffff              call     0x2137c4                       ; -> sub_002137C4
                                        ; XREF: 0x0021384D (cond_jump), 0x00213874 (jump), 0x00213881 (jump)
  0x00213888  5e                      pop      esi                            
  0x00213889  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_0021388C
; Start: 0x0021388C  End: 0x002138BD  Size: 49 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021383A
; Called by: sub_002139E9
; ============================================================
sub_0021388C:
  0x0021388C  53                      push     ebx                            
  0x0021388D  33db                    xor      ebx, ebx                       
  0x0021388F  381d81a14000            cmp      byte ptr [0x40a181], bl        
  0x00213895  7409                    je       0x2138a0                       
  0x00213897  ff74240c                push     dword ptr [esp + 0xc]          
  0x0021389B  e89affffff              call     0x21383a                       ; -> sub_0021383A
                                        ; XREF: 0x00213895 (cond_jump)
  0x002138A0  a1fca14000              mov      eax, dword ptr [0x40a1fc]      
  0x002138A5  3bc3                    cmp      eax, ebx                       
  0x002138A7  881d81a14000            mov      byte ptr [0x40a181], bl        
  0x002138AD  750e                    jne      0x2138bd                       
  0x002138AF  891d00a24000            mov      dword ptr [0x40a200], ebx      
  0x002138B5  881d80a14000            mov      byte ptr [0x40a180], bl        
  0x002138BB  eb6c                    jmp      0x213929                       
; end of function
                                        ; XREF: 0x002138AD (cond_jump)
  0x002138BD  a300a24000              mov      dword ptr [0x40a200], eax      
  0x002138C2  8b4810                  mov      ecx, dword ptr [eax + 0x10]    
  0x002138C5  890dfca14000            mov      dword ptr [0x40a1fc], ecx      
  0x002138CB  8a4805                  mov      cl, byte ptr [eax + 5]         
  0x002138CE  880d82a14000            mov      byte ptr [0x40a182], cl        
  0x002138D4  c60583a1400080          mov      byte ptr [0x40a183], 0x80      
  0x002138DB  c600fd                  mov      byte ptr [eax], 0xfd           
  0x002138DE  a100a24000              mov      eax, dword ptr [0x40a200]      
  0x002138E3  895810                  mov      dword ptr [eax + 0x10], ebx    
  0x002138E6  a100a24000              mov      eax, dword ptr [0x40a200]      
  0x002138EB  885805                  mov      byte ptr [eax + 5], bl         
  0x002138EE  ff15547b2100            call     dword ptr [0x217b54]           ; -> xbox_KeQueryInterruptTime
  0x002138F4  8b0d00a24000            mov      ecx, dword ptr [0x40a200]      
  0x002138FA  3b511c                  cmp      edx, dword ptr [ecx + 0x1c]    
  0x002138FD  7c0e                    jl       0x21390d                       
  0x002138FF  7f05                    jg       0x213906                       
  0x00213901  3b4118                  cmp      eax, dword ptr [ecx + 0x18]    
  0x00213904  7607                    jbe      0x21390d                       
                                        ; XREF: 0x002138FF (cond_jump)
  0x00213906  e852feffff              call     0x21375d                       ; -> sub_0021375D
  0x0021390B  eb1c                    jmp      0x213929                       
                                        ; XREF: 0x002138FD (cond_jump), 0x00213904 (cond_jump)
  0x0021390D  68b4a14000              push     0x40a1b4                       
  0x00213912  881df8a14000            mov      byte ptr [0x40a1f8], bl        
  0x00213918  ff711c                  push     dword ptr [ecx + 0x1c]         
  0x0021391B  ff7118                  push     dword ptr [ecx + 0x18]         
  0x0021391E  68d0a14000              push     0x40a1d0                       
  0x00213923  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
                                        ; XREF: 0x002138BB (jump), 0x0021390B (jump)
  0x00213929  5b                      pop      ebx                            
  0x0021392A  c20800                  ret      8                              

; ============================================================
; Function: sub_0021392D
; Start: 0x0021392D  End: 0x002139A4  Size: 119 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213507, sub_0021383A, sub_002144E5, sub_0021455E
; Called by: sub_00213A6A, sub_002159C9
; ============================================================
sub_0021392D:
  0x0021392D  56                      push     esi                            
  0x0021392E  57                      push     edi                            
  0x0021392F  ff74240c                push     dword ptr [esp + 0xc]          
  0x00213933  8bf9                    mov      edi, ecx                       
  0x00213935  e8ab0b0000              call     0x2144e5                       ; -> sub_002144E5
  0x0021393A  8bf0                    mov      esi, eax                       
  0x0021393C  85f6                    test     esi, esi                       
  0x0021393E  745f                    je       0x21399f                       
  0x00213940  56                      push     esi                            
  0x00213941  8bcf                    mov      ecx, edi                       
  0x00213943  e8160c0000              call     0x21455e                       ; -> sub_0021455E
  0x00213948  803efe                  cmp      byte ptr [esi], 0xfe           
  0x0021394B  7532                    jne      0x21397f                       
  0x0021394D  a1fca14000              mov      eax, dword ptr [0x40a1fc]      
  0x00213952  3bc6                    cmp      eax, esi                       
  0x00213954  750d                    jne      0x213963                       
  0x00213956  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x00213959  a3fca14000              mov      dword ptr [0x40a1fc], eax      
  0x0021395E  eb0e                    jmp      0x21396e                       
                                        ; XREF: 0x00213966 (cond_jump)
  0x00213960  8b4010                  mov      eax, dword ptr [eax + 0x10]    
                                        ; XREF: 0x00213954 (cond_jump)
  0x00213963  3b7010                  cmp      esi, dword ptr [eax + 0x10]    
  0x00213966  75f8                    jne      0x213960                       
  0x00213968  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x0021396B  894810                  mov      dword ptr [eax + 0x10], ecx    
                                        ; XREF: 0x0021395E (jump)
  0x0021396E  83661000                and      dword ptr [esi + 0x10], 0      
  0x00213972  56                      push     esi                            
  0x00213973  b980a14000              mov      ecx, 0x40a180                  
  0x00213978  e88afbffff              call     0x213507                       ; -> sub_00213507
  0x0021397D  eb20                    jmp      0x21399f                       
                                        ; XREF: 0x0021394B (cond_jump)
  0x0021397F  803d80a1400000          cmp      byte ptr [0x40a180], 0         
  0x00213986  7411                    je       0x213999                       
  0x00213988  393500a24000            cmp      dword ptr [0x40a200], esi      
  0x0021398E  7509                    jne      0x213999                       
  0x00213990  c60581a1400001          mov      byte ptr [0x40a181], 1         
  0x00213997  eb06                    jmp      0x21399f                       
                                        ; XREF: 0x00213986 (cond_jump), 0x0021398E (cond_jump)
  0x00213999  56                      push     esi                            
  0x0021399A  e89bfeffff              call     0x21383a                       ; -> sub_0021383A
                                        ; XREF: 0x0021393E (cond_jump), 0x0021397D (jump), 0x00213997 (jump)
  0x0021399F  5f                      pop      edi                            
  0x002139A0  5e                      pop      esi                            
  0x002139A1  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002139A4
; Start: 0x002139A4  End: 0x002139B8  Size: 20 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00213539
; ============================================================
sub_002139A4:
  0x002139A4  55                      push     ebp                            
  0x002139A5  8bec                    mov      ebp, esp                       
  0x002139A7  51                      push     ecx                            
  0x002139A8  803905                  cmp      byte ptr [ecx], 5              
  0x002139AB  56                      push     esi                            
  0x002139AC  57                      push     edi                            
  0x002139AD  7509                    jne      0x2139b8                       
  0x002139AF  e885fbffff              call     0x213539                       ; -> sub_00213539
  0x002139B4  8bf0                    mov      esi, eax                       
  0x002139B6  eb02                    jmp      0x2139ba                       
; end of function
                                        ; XREF: 0x002139AD (cond_jump)
  0x002139B8  8bf1                    mov      esi, ecx                       
                                        ; XREF: 0x002139B6 (jump)
  0x002139BA  8bce                    mov      ecx, esi                       
  0x002139BC  e878fbffff              call     0x213539                       ; -> sub_00213539
  0x002139C1  8bf8                    mov      edi, eax                       
  0x002139C3  85ff                    test     edi, edi                       
  0x002139C5  741e                    je       0x2139e5                       
  0x002139C7  8a4604                  mov      al, byte ptr [esi + 4]         
  0x002139CA  247f                    and      al, 0x7f                       
  0x002139CC  8845fc                  mov      byte ptr [ebp - 4], al         
  0x002139CF  ff75fc                  push     dword ptr [ebp - 4]            
  0x002139D2  8bcf                    mov      ecx, edi                       
  0x002139D4  e854ffffff              call     0x21392d                       ; -> sub_0021392D
  0x002139D9  6a05                    push     5                              
  0x002139DB  ff75fc                  push     dword ptr [ebp - 4]            
  0x002139DE  8bcf                    mov      ecx, edi                       
  0x002139E0  e899fbffff              call     0x21357e                       ; -> sub_0021357E
                                        ; XREF: 0x002139C5 (cond_jump)
  0x002139E5  5f                      pop      edi                            
  0x002139E6  5e                      pop      esi                            
  0x002139E7  c9                      leave                                   
  0x002139E8  c3                      ret                                     

; ============================================================
; Function: sub_002139E9
; Start: 0x002139E9  End: 0x00213A6A  Size: 129 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00213507, sub_00213539, sub_0021357E, sub_00213714, sub_0021388C, sub_0021455E
; Called by: sub_00213A7D
; ============================================================
sub_002139E9:
  0x002139E9  55                      push     ebp                            
  0x002139EA  8bec                    mov      ebp, esp                       
  0x002139EC  51                      push     ecx                            
  0x002139ED  53                      push     ebx                            
  0x002139EE  56                      push     esi                            
  0x002139EF  32db                    xor      bl, bl                         
  0x002139F1  205dfc                  and      byte ptr [ebp - 4], bl         
  0x002139F4  57                      push     edi                            
  0x002139F5  33ff                    xor      edi, edi                       
  0x002139F7  803d81a1400000          cmp      byte ptr [0x40a181], 0         
  0x002139FE  8bf1                    mov      esi, ecx                       
  0x00213A00  c60583a140000a          mov      byte ptr [0x40a183], 0xa       
  0x00213A07  7522                    jne      0x213a2b                       
  0x00213A09  e82bfbffff              call     0x213539                       ; -> sub_00213539
  0x00213A0E  8bf8                    mov      edi, eax                       
  0x00213A10  56                      push     esi                            
  0x00213A11  8bcf                    mov      ecx, edi                       
  0x00213A13  e8460b0000              call     0x21455e                       ; -> sub_0021455E
  0x00213A18  a082a14000              mov      al, byte ptr [0x40a182]        
  0x00213A1D  84c0                    test     al, al                         
  0x00213A1F  740a                    je       0x213a2b                       
  0x00213A21  8ad8                    mov      bl, al                         
  0x00213A23  8a4604                  mov      al, byte ptr [esi + 4]         
  0x00213A26  247f                    and      al, 0x7f                       
  0x00213A28  8845fc                  mov      byte ptr [ebp - 4], al         
                                        ; XREF: 0x00213A07 (cond_jump), 0x00213A1F (cond_jump)
  0x00213A2B  8a5605                  mov      dl, byte ptr [esi + 5]         
  0x00213A2E  84d2                    test     dl, dl                         
  0x00213A30  7408                    je       0x213a3a                       
  0x00213A32  8b4e0c                  mov      ecx, dword ptr [esi + 0xc]     
  0x00213A35  e8dafcffff              call     0x213714                       ; -> sub_00213714
                                        ; XREF: 0x00213A30 (cond_jump)
  0x00213A3A  56                      push     esi                            
  0x00213A3B  b980a14000              mov      ecx, 0x40a180                  
  0x00213A40  e8c2faffff              call     0x213507                       ; -> sub_00213507
  0x00213A45  84db                    test     bl, bl                         
  0x00213A47  740d                    je       0x213a56                       
  0x00213A49  fecb                    dec      bl                             
  0x00213A4B  8bcf                    mov      ecx, edi                       
  0x00213A4D  53                      push     ebx                            
  0x00213A4E  ff75fc                  push     dword ptr [ebp - 4]            
  0x00213A51  e828fbffff              call     0x21357e                       ; -> sub_0021357E
                                        ; XREF: 0x00213A47 (cond_jump)
  0x00213A56  802581a1400000          and      byte ptr [0x40a181], 0         
  0x00213A5D  56                      push     esi                            
  0x00213A5E  6a00                    push     0                              
  0x00213A60  e827feffff              call     0x21388c                       ; -> sub_0021388C
  0x00213A65  5f                      pop      edi                            
  0x00213A66  5e                      pop      esi                            
  0x00213A67  5b                      pop      ebx                            
  0x00213A68  c9                      leave                                   
  0x00213A69  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00213A6A
; Start: 0x00213A6A  End: 0x00213A7D  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021392D
; Called by: sub_00216048
; ============================================================
sub_00213A6A:
  0x00213A6A  8b442404                mov      eax, dword ptr [esp + 4]       
  0x00213A6E  ff742408                push     dword ptr [esp + 8]            
  0x00213A72  8b48ec                  mov      ecx, dword ptr [eax - 0x14]    
  0x00213A75  e8b3feffff              call     0x21392d                       ; -> sub_0021392D
  0x00213A7A  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00213A7D
; Start: 0x00213A7D  End: 0x00213AB0  Size: 51 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002139E9
; Called by: sub_00213AB0
; ============================================================
sub_00213A7D:
  0x00213A7D  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x00213A81  33c0                    xor      eax, eax                       
  0x00213A83  39442404                cmp      dword ptr [esp + 4], eax       
  0x00213A87  c60583a1400009          mov      byte ptr [0x40a183], 9         
  0x00213A8E  7d0b                    jge      0x213a9b                       
  0x00213A90  380581a14000            cmp      byte ptr [0x40a181], al        
  0x00213A96  750b                    jne      0x213aa3                       
  0x00213A98  884105                  mov      byte ptr [ecx + 5], al         
                                        ; XREF: 0x00213A8E (cond_jump)
  0x00213A9B  380581a14000            cmp      byte ptr [0x40a181], al        
  0x00213AA1  7405                    je       0x213aa8                       
                                        ; XREF: 0x00213A96 (cond_jump)
  0x00213AA3  a282a14000              mov      byte ptr [0x40a182], al        
                                        ; XREF: 0x00213AA1 (cond_jump)
  0x00213AA8  e83cffffff              call     0x2139e9                       ; -> sub_002139E9
  0x00213AAD  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00213AB0
; Start: 0x00213AB0  End: 0x00213B09  Size: 89 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213539, sub_00213A7D, sub_00215769, sub_00216124
; Called by: sub_0021375D, sub_00213F01, sub_002140B7
; ============================================================
sub_00213AB0:
  0x00213AB0  803d81a1400000          cmp      byte ptr [0x40a181], 0         
  0x00213AB7  56                      push     esi                            
  0x00213AB8  c60583a1400008          mov      byte ptr [0x40a183], 8         
  0x00213ABF  7548                    jne      0x213b09                       
  0x00213AC1  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00213AC5  8bce                    mov      ecx, esi                       
  0x00213AC7  e86dfaffff              call     0x213539                       ; -> sub_00213539
  0x00213ACC  803800                  cmp      byte ptr [eax], 0              
  0x00213ACF  7521                    jne      0x213af2                       
  0x00213AD1  33c0                    xor      eax, eax                       
  0x00213AD3  8a4604                  mov      al, byte ptr [esi + 4]         
  0x00213AD6  83e07f                  and      eax, 0x7f                      
  0x00213AD9  50                      push     eax                            
  0x00213ADA  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00213ADD  83c018                  add      eax, 0x18                      
  0x00213AE0  50                      push     eax                            
  0x00213AE1  e83e260000              call     0x216124                       ; -> sub_00216124
  0x00213AE6  56                      push     esi                            
  0x00213AE7  6a00                    push     0                              
  0x00213AE9  8bce                    mov      ecx, esi                       
  0x00213AEB  e88dffffff              call     0x213a7d                       ; -> sub_00213A7D
  0x00213AF0  eb27                    jmp      0x213b19                       
                                        ; XREF: 0x00213ACF (cond_jump)
  0x00213AF2  33c9                    xor      ecx, ecx                       
  0x00213AF4  8a4e04                  mov      cl, byte ptr [esi + 4]         
  0x00213AF7  6a01                    push     1                              
  0x00213AF9  56                      push     esi                            
  0x00213AFA  81e17fffffff            and      ecx, 0xffffff7f                
  0x00213B00  51                      push     ecx                            
  0x00213B01  50                      push     eax                            
  0x00213B02  e8621c0000              call     0x215769                       ; -> sub_00215769
  0x00213B07  eb10                    jmp      0x213b19                       
; end of function
                                        ; XREF: 0x00213ABF (cond_jump)
  0x00213B09  8b4c240c                mov      ecx, dword ptr [esp + 0xc]     
  0x00213B0D  802582a1400000          and      byte ptr [0x40a182], 0         
  0x00213B14  e8d0feffff              call     0x2139e9                       ; -> sub_002139E9
                                        ; XREF: 0x00213AF0 (jump), 0x00213B07 (jump)
  0x00213B19  5e                      pop      esi                            
  0x00213B1A  c20800                  ret      8                              

; ============================================================
; Function: sub_00213B1D
; Start: 0x00213B1D  End: 0x00213B82  Size: 101 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00215371
; Called by: sub_00213FFE
; ============================================================
sub_00213B1D:
  0x00213B1D  68d0a14000              push     0x40a1d0                       
  0x00213B22  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00213B28  8b442404                mov      eax, dword ptr [esp + 4]       
  0x00213B2C  c60583a1400003          mov      byte ptr [0x40a183], 3         
  0x00213B33  83780400                cmp      dword ptr [eax + 4], 0         
  0x00213B37  7c13                    jl       0x213b4c                       
  0x00213B39  803d81a1400000          cmp      byte ptr [0x40a181], 0         
  0x00213B40  c7058ca14000b8362100    mov      dword ptr [0x40a18c], 0x2136b8 
  0x00213B4A  740a                    je       0x213b56                       
                                        ; XREF: 0x00213B37 (cond_jump)
  0x00213B4C  c7058ca14000b03a2100    mov      dword ptr [0x40a18c], 0x213ab0 
                                        ; XREF: 0x00213B4A (cond_jump)
  0x00213B56  56                      push     esi                            
  0x00213B57  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00213B5B  c60584a140001c          mov      byte ptr [0x40a184], 0x1c      
  0x00213B62  c60585a1400043          mov      byte ptr [0x40a185], 0x43      
  0x00213B69  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00213B6C  6884a14000              push     0x40a184                       
  0x00213B71  83c018                  add      eax, 0x18                      
  0x00213B74  50                      push     eax                            
  0x00213B75  e8f7170000              call     0x215371                       ; -> sub_00215371
  0x00213B7A  83660800                and      dword ptr [esi + 8], 0         
  0x00213B7E  5e                      pop      esi                            
  0x00213B7F  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00213B82
; Start: 0x00213B82  End: 0x00213BA0  Size: 30 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00213B82:
  0x00213B82  55                      push     ebp                            
  0x00213B83  8bec                    mov      ebp, esp                       
  0x00213B85  817d0800040080          cmp      dword ptr [ebp + 8], 0x80000400 
  0x00213B8C  53                      push     ebx                            
  0x00213B8D  56                      push     esi                            
  0x00213B8E  57                      push     edi                            
  0x00213B8F  8bf1                    mov      esi, ecx                       
  0x00213B91  c60583a1400007          mov      byte ptr [0x40a183], 7         
  0x00213B98  7506                    jne      0x213ba0                       
  0x00213B9A  83661000                and      dword ptr [esi + 0x10], 0      
  0x00213B9E  eb22                    jmp      0x213bc2                       
; end of function
                                        ; XREF: 0x00213B98 (cond_jump)
  0x00213BA0  8a4607                  mov      al, byte ptr [esi + 7]         
  0x00213BA3  3cff                    cmp      al, 0xff                       
  0x00213BA5  741b                    je       0x213bc2                       
  0x00213BA7  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x00213BAA  8b4914                  mov      ecx, dword ptr [ecx + 0x14]    
  0x00213BAD  0fb6c0                  movzx    eax, al                        
  0x00213BB0  8b0481                  mov      eax, dword ptr [ecx + eax*4]   
  0x00213BB3  85c0                    test     eax, eax                       
  0x00213BB5  740b                    je       0x213bc2                       
  0x00213BB7  6a01                    push     1                              
  0x00213BB9  ff7614                  push     dword ptr [esi + 0x14]         
  0x00213BBC  50                      push     eax                            
  0x00213BBD  e894d9ffff              call     0x211556                       ; -> sub_00211556
                                        ; XREF: 0x00213B9E (jump), 0x00213BA5 (cond_jump), 0x00213BB5 (cond_jump)
  0x00213BC2  8a0e                    mov      cl, byte ptr [esi]             
  0x00213BC4  80f902                  cmp      cl, 2                          
  0x00213BC7  0f84bf000000            je       0x213c8c                       
  0x00213BCD  80f901                  cmp      cl, 1                          
  0x00213BD0  0f84b6000000            je       0x213c8c                       
  0x00213BD6  8b7e08                  mov      edi, dword ptr [esi + 8]       
  0x00213BD9  83660800                and      dword ptr [esi + 8], 0         
  0x00213BDD  80f905                  cmp      cl, 5                          
  0x00213BE0  8bc6                    mov      eax, esi                       
  0x00213BE2  bb8c382100              mov      ebx, 0x21388c                  
  0x00213BE7  7559                    jne      0x213c42                       
  0x00213BE9  8bce                    mov      ecx, esi                       
  0x00213BEB  e877f9ffff              call     0x213567                       ; -> sub_00213567
  0x00213BF0  85c0                    test     eax, eax                       
  0x00213BF2  7445                    je       0x213c39                       
  0x00213BF4  8b155ca24000            mov      edx, dword ptr [0x40a25c]      
                                        ; XREF: 0x00213C03 (cond_jump)
  0x00213BFA  0fb60a                  movzx    ecx, byte ptr [edx]            
  0x00213BFD  03d1                    add      edx, ecx                       
  0x00213BFF  807a0104                cmp      byte ptr [edx + 1], 4          
  0x00213C03  75f5                    jne      0x213bfa                       
  0x00213C05  89155ca24000            mov      dword ptr [0x40a25c], edx      
  0x00213C0B  8a4a02                  mov      cl, byte ptr [edx + 2]         
  0x00213C0E  884802                  mov      byte ptr [eax + 2], cl         
  0x00213C11  8b155ca24000            mov      edx, dword ptr [0x40a25c]      
  0x00213C17  8a4a05                  mov      cl, byte ptr [edx + 5]         
  0x00213C1A  884d09                  mov      byte ptr [ebp + 9], cl         
  0x00213C1D  8a4a06                  mov      cl, byte ptr [edx + 6]         
  0x00213C20  884d0a                  mov      byte ptr [ebp + 0xa], cl       
  0x00213C23  8a4a07                  mov      cl, byte ptr [edx + 7]         
  0x00213C26  884d0b                  mov      byte ptr [ebp + 0xb], cl       
  0x00213C29  c6450882                mov      byte ptr [ebp + 8], 0x82       
  0x00213C2D  ff7508                  push     dword ptr [ebp + 8]            
  0x00213C30  8bc8                    mov      ecx, eax                       
  0x00213C32  e87e000000              call     0x213cb5                       ; -> sub_00213CB5
  0x00213C37  eb75                    jmp      0x213cae                       
                                        ; XREF: 0x00213BF2 (cond_jump)
  0x00213C39  8bce                    mov      ecx, esi                       
  0x00213C3B  e8f9f8ffff              call     0x213539                       ; -> sub_00213539
  0x00213C40  eb18                    jmp      0x213c5a                       
                                        ; XREF: 0x00213BE7 (cond_jump)
  0x00213C42  837d0800                cmp      dword ptr [ebp + 8], 0         
  0x00213C46  7d12                    jge      0x213c5a                       
  0x00213C48  837e1000                cmp      dword ptr [esi + 0x10], 0      
  0x00213C4C  7507                    jne      0x213c55                       
  0x00213C4E  802582a1400000          and      byte ptr [0x40a182], 0         
                                        ; XREF: 0x00213C4C (cond_jump)
  0x00213C55  bbb03a2100              mov      ebx, 0x213ab0                  
                                        ; XREF: 0x00213C40 (jump), 0x00213C46 (cond_jump)
  0x00213C5A  a390a14000              mov      dword ptr [0x40a190], eax      
  0x00213C5F  c60584a140001c          mov      byte ptr [0x40a184], 0x1c      
  0x00213C66  c60585a1400043          mov      byte ptr [0x40a185], 0x43      
  0x00213C6D  891d8ca14000            mov      dword ptr [0x40a18c], ebx      
  0x00213C73  893d94a14000            mov      dword ptr [0x40a194], edi      
  0x00213C79  8b400c                  mov      eax, dword ptr [eax + 0xc]     
  0x00213C7C  6884a14000              push     0x40a184                       
  0x00213C81  83c018                  add      eax, 0x18                      
  0x00213C84  50                      push     eax                            
  0x00213C85  e8e7160000              call     0x215371                       ; -> sub_00215371
  0x00213C8A  eb22                    jmp      0x213cae                       
                                        ; XREF: 0x00213BC7 (cond_jump), 0x00213BD0 (cond_jump)
  0x00213C8C  33c0                    xor      eax, eax                       
  0x00213C8E  394508                  cmp      dword ptr [ebp + 8], eax       
  0x00213C91  7d14                    jge      0x213ca7                       
  0x00213C93  394610                  cmp      dword ptr [esi + 0x10], eax    
  0x00213C96  7506                    jne      0x213c9e                       
  0x00213C98  200582a14000            and      byte ptr [0x40a182], al        
                                        ; XREF: 0x00213C96 (cond_jump)
  0x00213C9E  56                      push     esi                            
  0x00213C9F  50                      push     eax                            
  0x00213CA0  e80bfeffff              call     0x213ab0                       ; -> sub_00213AB0
  0x00213CA5  eb07                    jmp      0x213cae                       
                                        ; XREF: 0x00213C91 (cond_jump)
  0x00213CA7  56                      push     esi                            
  0x00213CA8  50                      push     eax                            
  0x00213CA9  e8defbffff              call     0x21388c                       ; -> sub_0021388C
                                        ; XREF: 0x00213C37 (jump), 0x00213C8A (jump), 0x00213CA5 (jump)
  0x00213CAE  5f                      pop      edi                            
  0x00213CAF  5e                      pop      esi                            
  0x00213CB0  5b                      pop      ebx                            
  0x00213CB1  5d                      pop      ebp                            
  0x00213CB2  c20400                  ret      4                              

; ============================================================
; Function: sub_00213CB5
; Start: 0x00213CB5  End: 0x00213CD9  Size: 36 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00212932, sub_00214750
; Called by: sub_00213DC4
; ============================================================
sub_00213CB5:
  0x00213CB5  56                      push     esi                            
  0x00213CB6  8bf1                    mov      esi, ecx                       
  0x00213CB8  e8930a0000              call     0x214750                       ; -> sub_00214750
  0x00213CBD  837e1420                cmp      dword ptr [esi + 0x14], 0x20   
  0x00213CC1  7416                    je       0x213cd9                       
  0x00213CC3  ff742408                push     dword ptr [esp + 8]            
  0x00213CC7  e866ecffff              call     0x212932                       ; -> sub_00212932
  0x00213CCC  85c0                    test     eax, eax                       
  0x00213CCE  894610                  mov      dword ptr [esi + 0x10], eax    
  0x00213CD1  7406                    je       0x213cd9                       
  0x00213CD3  56                      push     esi                            
  0x00213CD4  ff5008                  call     dword ptr [eax + 8]            
  0x00213CD7  eb0c                    jmp      0x213ce5                       
; end of function
                                        ; XREF: 0x00213CC1 (cond_jump), 0x00213CD1 (cond_jump)
  0x00213CD9  8bce                    mov      ecx, esi                       
  0x00213CDB  6800040080              push     0x80000400                     
  0x00213CE0  e89dfeffff              call     0x213b82                       ; -> sub_00213B82
                                        ; XREF: 0x00213CD7 (jump)
  0x00213CE5  5e                      pop      esi                            
  0x00213CE6  c20400                  ret      4                              
  0x00213CE9  53                      push     ebx                            
  0x00213CEA  68d0a14000              push     0x40a1d0                       
  0x00213CEF  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00213CF5  8b542408                mov      edx, dword ptr [esp + 8]       
  0x00213CF9  33db                    xor      ebx, ebx                       
  0x00213CFB  c60583a1400002          mov      byte ptr [0x40a183], 2         
  0x00213D02  395a04                  cmp      dword ptr [edx + 4], ebx       
  0x00213D05  0f8cab000000            jl       0x213db6                       
  0x00213D0B  381d81a14000            cmp      byte ptr [0x40a181], bl        
  0x00213D11  0f859f000000            jne      0x213db6                       
  0x00213D17  837a1408                cmp      dword ptr [edx + 0x14], 8      
  0x00213D1B  0f828e000000            jb       0x213daf                       
  0x00213D21  a00ba24000              mov      al, byte ptr [0x40a20b]        
  0x00213D26  3c40                    cmp      al, 0x40                       
  0x00213D28  0f8781000000            ja       0x213daf                       
  0x00213D2E  803d05a2400001          cmp      byte ptr [0x40a205], 1         
  0x00213D35  7578                    jne      0x213daf                       
  0x00213D37  8a0d04a24000            mov      cl, byte ptr [0x40a204]        
  0x00213D3D  80f908                  cmp      cl, 8                          
  0x00213D40  7405                    je       0x213d47                       
  0x00213D42  80f912                  cmp      cl, 0x12                       
  0x00213D45  7568                    jne      0x213daf                       
                                        ; XREF: 0x00213D40 (cond_jump)
  0x00213D47  56                      push     esi                            
  0x00213D48  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00213D4C  8b4e0c                  mov      ecx, dword ptr [esi + 0xc]     
  0x00213D4F  884606                  mov      byte ptr [esi + 6], al         
  0x00213D52  e88cf9ffff              call     0x2136e3                       ; -> sub_002136E3
  0x00213D57  884605                  mov      byte ptr [esi + 5], al         
  0x00213D5A  c7058ca140001d3b2100    mov      dword ptr [0x40a18c], 0x213b1d 
  0x00213D64  891d9ca14000            mov      dword ptr [0x40a19c], ebx      
  0x00213D6A  891d98a14000            mov      dword ptr [0x40a198], ebx      
  0x00213D70  881daca14000            mov      byte ptr [0x40a1ac], bl        
  0x00213D76  c605ada1400005          mov      byte ptr [0x40a1ad], 5         
  0x00213D7D  660fb64605              movzx    ax, byte ptr [esi + 5]         
  0x00213D82  66a3aea14000            mov      word ptr [0x40a1ae], ax        
  0x00213D88  66891db0a14000          mov      word ptr [0x40a1b0], bx        
  0x00213D8F  66891db2a14000          mov      word ptr [0x40a1b2], bx        
  0x00213D96  e8aaf8ffff              call     0x213645                       ; -> sub_00213645
  0x00213D9B  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00213D9E  6884a14000              push     0x40a184                       
  0x00213DA3  83c018                  add      eax, 0x18                      
  0x00213DA6  50                      push     eax                            
  0x00213DA7  e8c5150000              call     0x215371                       ; -> sub_00215371
  0x00213DAC  5e                      pop      esi                            
  0x00213DAD  eb11                    jmp      0x213dc0                       
                                        ; XREF: 0x00213D1B (cond_jump), 0x00213D28 (cond_jump), 0x00213D35 (cond_jump), 0x00213D45 (cond_jump)
  0x00213DAF  c7420400060080          mov      dword ptr [edx + 4], 0x80000600 
                                        ; XREF: 0x00213D05 (cond_jump), 0x00213D11 (cond_jump)
  0x00213DB6  ff74240c                push     dword ptr [esp + 0xc]          
  0x00213DBA  52                      push     edx                            
  0x00213DBB  e85dfdffff              call     0x213b1d                       ; -> sub_00213B1D
                                        ; XREF: 0x00213DAD (jump)
  0x00213DC0  5b                      pop      ebx                            
  0x00213DC1  c20800                  ret      8                              

; ============================================================
; Function: sub_00213DC4
; Start: 0x00213DC4  End: 0x00213EF2  Size: 302 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00212885, sub_00213550, sub_00213CB5, sub_00214516
; ============================================================
sub_00213DC4:
  0x00213DC4  55                      push     ebp                            
  0x00213DC5  8bec                    mov      ebp, esp                       
  0x00213DC7  53                      push     ebx                            
  0x00213DC8  56                      push     esi                            
  0x00213DC9  68d0a14000              push     0x40a1d0                       
  0x00213DCE  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00213DD4  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00213DD7  33db                    xor      ebx, ebx                       
  0x00213DD9  c60583a1400006          mov      byte ptr [0x40a183], 6         
  0x00213DE0  395904                  cmp      dword ptr [ecx + 4], ebx       
  0x00213DE3  0f8c09010000            jl       0x213ef2                       
  0x00213DE9  381d81a14000            cmp      byte ptr [0x40a181], bl        
  0x00213DEF  0f85fd000000            jne      0x213ef2                       
  0x00213DF5  8b750c                  mov      esi, dword ptr [ebp + 0xc]     
  0x00213DF8  895e18                  mov      dword ptr [esi + 0x18], ebx    
  0x00213DFB  b80ca24000              mov      eax, 0x40a20c                  
                                        ; XREF: 0x00213E1D (cond_jump)
  0x00213E00  0fb610                  movzx    edx, byte ptr [eax]            
  0x00213E03  03c2                    add      eax, edx                       
  0x00213E05  3d5ca24000              cmp      eax, 0x40a25c                  
  0x00213E0A  0f83d1000000            jae      0x213ee1                       
  0x00213E10  803800                  cmp      byte ptr [eax], 0              
  0x00213E13  0f84c8000000            je       0x213ee1                       
  0x00213E19  80780104                cmp      byte ptr [eax + 1], 4          
  0x00213E1D  75e1                    jne      0x213e00                       
  0x00213E1F  803d10a2400001          cmp      byte ptr [0x40a210], 1         
  0x00213E26  a35ca24000              mov      dword ptr [0x40a25c], eax      
  0x00213E2B  747f                    je       0x213eac                       
  0x00213E2D  803dfba1400000          cmp      byte ptr [0x40a1fb], 0         
  0x00213E34  7476                    je       0x213eac                       
  0x00213E36  c60604                  mov      byte ptr [esi], 4              
  0x00213E39  c6460280                mov      byte ptr [esi + 2], 0x80       
  0x00213E3D  803d10a2400000          cmp      byte ptr [0x40a210], 0         
  0x00213E44  7657                    jbe      0x213e9d                       
                                        ; XREF: 0x00213E9B (cond_jump)
  0x00213E46  0fb605fba14000          movzx    eax, byte ptr [0x40a1fb]       
  0x00213E4D  3bc3                    cmp      eax, ebx                       
  0x00213E4F  764c                    jbe      0x213e9d                       
  0x00213E51  b980a14000              mov      ecx, 0x40a180                  
  0x00213E56  e82aeaffff              call     0x212885                       ; -> sub_00212885
  0x00213E5B  85c0                    test     eax, eax                       
  0x00213E5D  743e                    je       0x213e9d                       
  0x00213E5F  c60005                  mov      byte ptr [eax], 5              
  0x00213E62  8a4e04                  mov      cl, byte ptr [esi + 4]         
  0x00213E65  80e180                  and      cl, 0x80                       
  0x00213E68  8ad3                    mov      dl, bl                         
  0x00213E6A  fec2                    inc      dl                             
  0x00213E6C  0aca                    or       cl, dl                         
  0x00213E6E  884804                  mov      byte ptr [eax + 4], cl         
  0x00213E71  8a4e05                  mov      cl, byte ptr [esi + 5]         
  0x00213E74  884805                  mov      byte ptr [eax + 5], cl         
  0x00213E77  8b4e08                  mov      ecx, dword ptr [esi + 8]       
  0x00213E7A  894808                  mov      dword ptr [eax + 8], ecx       
  0x00213E7D  8b4e0c                  mov      ecx, dword ptr [esi + 0xc]     
  0x00213E80  89480c                  mov      dword ptr [eax + 0xc], ecx     
  0x00213E83  8a4e06                  mov      cl, byte ptr [esi + 6]         
  0x00213E86  884806                  mov      byte ptr [eax + 6], cl         
  0x00213E89  50                      push     eax                            
  0x00213E8A  8bce                    mov      ecx, esi                       
  0x00213E8C  e885060000              call     0x214516                       ; -> sub_00214516
  0x00213E91  0fb60510a24000          movzx    eax, byte ptr [0x40a210]       
  0x00213E98  43                      inc      ebx                            
  0x00213E99  3bd8                    cmp      ebx, eax                       
  0x00213E9B  72a9                    jb       0x213e46                       
                                        ; XREF: 0x00213E44 (cond_jump), 0x00213E4F (cond_jump), 0x00213E5D (cond_jump)
  0x00213E9D  83660800                and      dword ptr [esi + 8], 0         
  0x00213EA1  8bce                    mov      ecx, esi                       
  0x00213EA3  e8a8f6ffff              call     0x213550                       ; -> sub_00213550
  0x00213EA8  8bf0                    mov      esi, eax                       
  0x00213EAA  eb03                    jmp      0x213eaf                       
                                        ; XREF: 0x00213E2B (cond_jump), 0x00213E34 (cond_jump)
  0x00213EAC  c60603                  mov      byte ptr [esi], 3              
                                        ; XREF: 0x00213EAA (jump)
  0x00213EAF  a15ca24000              mov      eax, dword ptr [0x40a25c]      
  0x00213EB4  8a4002                  mov      al, byte ptr [eax + 2]         
  0x00213EB7  884602                  mov      byte ptr [esi + 2], al         
  0x00213EBA  a15ca24000              mov      eax, dword ptr [0x40a25c]      
  0x00213EBF  8a4805                  mov      cl, byte ptr [eax + 5]         
  0x00213EC2  884d0d                  mov      byte ptr [ebp + 0xd], cl       
  0x00213EC5  8a4806                  mov      cl, byte ptr [eax + 6]         
  0x00213EC8  8a4007                  mov      al, byte ptr [eax + 7]         
  0x00213ECB  884d0e                  mov      byte ptr [ebp + 0xe], cl       
  0x00213ECE  88450f                  mov      byte ptr [ebp + 0xf], al       
  0x00213ED1  c6450c82                mov      byte ptr [ebp + 0xc], 0x82     
  0x00213ED5  ff750c                  push     dword ptr [ebp + 0xc]          
  0x00213ED8  8bce                    mov      ecx, esi                       
  0x00213EDA  e8d6fdffff              call     0x213cb5                       ; -> sub_00213CB5
  0x00213EDF  eb1a                    jmp      0x213efb                       
                                        ; XREF: 0x00213E0A (cond_jump), 0x00213E13 (cond_jump)
  0x00213EE1  802582a1400000          and      byte ptr [0x40a182], 0         
  0x00213EE8  c7410400040080          mov      dword ptr [ecx + 4], 0x80000400 
  0x00213EEF  56                      push     esi                            
  0x00213EF0  eb03                    jmp      0x213ef5                       
; end of function
                                        ; XREF: 0x00213DE3 (cond_jump), 0x00213DEF (cond_jump)
  0x00213EF2  ff750c                  push     dword ptr [ebp + 0xc]          
                                        ; XREF: 0x00213EF0 (jump)
  0x00213EF5  51                      push     ecx                            
  0x00213EF6  e822fcffff              call     0x213b1d                       ; -> sub_00213B1D
                                        ; XREF: 0x00213EDF (jump)
  0x00213EFB  5e                      pop      esi                            
  0x00213EFC  5b                      pop      ebx                            
  0x00213EFD  5d                      pop      ebp                            
  0x00213EFE  c20800                  ret      8                              

; ============================================================
; Function: sub_00213F01
; Start: 0x00213F01  End: 0x00213F29  Size: 40 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213AB0
; ============================================================
sub_00213F01:
  0x00213F01  53                      push     ebx                            
  0x00213F02  56                      push     esi                            
  0x00213F03  57                      push     edi                            
  0x00213F04  8bf1                    mov      esi, ecx                       
  0x00213F06  8b7e0c                  mov      edi, dword ptr [esi + 0xc]     
  0x00213F09  33db                    xor      ebx, ebx                       
  0x00213F0B  83c718                  add      edi, 0x18                      
  0x00213F0E  381d81a14000            cmp      byte ptr [0x40a181], bl        
  0x00213F14  c60583a1400001          mov      byte ptr [0x40a183], 1         
  0x00213F1B  740c                    je       0x213f29                       
  0x00213F1D  56                      push     esi                            
  0x00213F1E  53                      push     ebx                            
  0x00213F1F  e88cfbffff              call     0x213ab0                       ; -> sub_00213AB0
  0x00213F24  e9d1000000              jmp      0x213ffa                       
; end of function
                                        ; XREF: 0x00213F1B (cond_jump)
  0x00213F29  55                      push     ebp                            
  0x00213F2A  c60584a1400020          mov      byte ptr [0x40a184], 0x20      
  0x00213F31  c60585a1400002          mov      byte ptr [0x40a185], 2         
  0x00213F38  891d8ca14000            mov      dword ptr [0x40a18c], ebx      
  0x00213F3E  881d99a14000            mov      byte ptr [0x40a199], bl        
  0x00213F44  881d9aa14000            mov      byte ptr [0x40a19a], bl        
  0x00213F4A  881d9ba14000            mov      byte ptr [0x40a19b], bl        
  0x00213F50  66c705a0a140000800      mov      word ptr [0x40a1a0], 8         
  0x00213F59  8a4604                  mov      al, byte ptr [esi + 4]         
  0x00213F5C  bd84a14000              mov      ebp, 0x40a184                  
  0x00213F61  55                      push     ebp                            
  0x00213F62  c0e807                  shr      al, 7                          
  0x00213F65  57                      push     edi                            
  0x00213F66  a2a2a14000              mov      byte ptr [0x40a1a2], al        
  0x00213F6B  881d98a14000            mov      byte ptr [0x40a198], bl        
  0x00213F71  e8fb130000              call     0x215371                       ; -> sub_00215371
  0x00213F76  a194a14000              mov      eax, dword ptr [0x40a194]      
  0x00213F7B  894608                  mov      dword ptr [esi + 8], eax       
  0x00213F7E  c60584a1400030          mov      byte ptr [0x40a184], 0x30      
  0x00213F85  c60585a1400040          mov      byte ptr [0x40a185], 0x40      
  0x00213F8C  c7058ca14000e93c2100    mov      dword ptr [0x40a18c], 0x213ce9 
  0x00213F96  893590a14000            mov      dword ptr [0x40a190], esi      
  0x00213F9C  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x00213F9F  6a08                    push     8                              
  0x00213FA1  a394a14000              mov      dword ptr [0x40a194], eax      
  0x00213FA6  58                      pop      eax                            
  0x00213FA7  c7059ca1400004a24000    mov      dword ptr [0x40a19c], 0x40a204 
  0x00213FB1  a398a14000              mov      dword ptr [0x40a198], eax      
  0x00213FB6  c605a0a1400002          mov      byte ptr [0x40a1a0], 2         
  0x00213FBD  881da1a14000            mov      byte ptr [0x40a1a1], bl        
  0x00213FC3  881da2a14000            mov      byte ptr [0x40a1a2], bl        
  0x00213FC9  c605aca1400080          mov      byte ptr [0x40a1ac], 0x80      
  0x00213FD0  c605ada1400006          mov      byte ptr [0x40a1ad], 6         
  0x00213FD7  66c705aea140000001      mov      word ptr [0x40a1ae], 0x100     
  0x00213FE0  66891db0a14000          mov      word ptr [0x40a1b0], bx        
  0x00213FE7  66a3b2a14000            mov      word ptr [0x40a1b2], ax        
  0x00213FED  e853f6ffff              call     0x213645                       ; -> sub_00213645
  0x00213FF2  55                      push     ebp                            
  0x00213FF3  57                      push     edi                            
  0x00213FF4  e878130000              call     0x215371                       ; -> sub_00215371
  0x00213FF9  5d                      pop      ebp                            
                                        ; XREF: 0x00213F24 (jump)
  0x00213FFA  5f                      pop      edi                            
  0x00213FFB  5e                      pop      esi                            
  0x00213FFC  5b                      pop      ebx                            
  0x00213FFD  c3                      ret                                     

; ============================================================
; Function: sub_00213FFE
; Start: 0x00213FFE  End: 0x0021404C  Size: 78 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00213B1D
; ============================================================
sub_00213FFE:
  0x00213FFE  55                      push     ebp                            
  0x00213FFF  8bec                    mov      ebp, esp                       
  0x00214001  68d0a14000              push     0x40a1d0                       
  0x00214006  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x0021400C  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x0021400F  33d2                    xor      edx, edx                       
  0x00214011  c60583a1400005          mov      byte ptr [0x40a183], 5         
  0x00214018  395004                  cmp      dword ptr [eax + 4], edx       
  0x0021401B  7c22                    jl       0x21403f                       
  0x0021401D  381581a14000            cmp      byte ptr [0x40a181], dl        
  0x00214023  751a                    jne      0x21403f                       
  0x00214025  668b0d0ea24000          mov      cx, word ptr [0x40a20e]        
  0x0021402C  6683f950                cmp      cx, 0x50                       
  0x00214030  761a                    jbe      0x21404c                       
  0x00214032  881582a14000            mov      byte ptr [0x40a182], dl        
  0x00214038  c7400400040080          mov      dword ptr [eax + 4], 0x80000400 
                                        ; XREF: 0x0021401B (cond_jump), 0x00214023 (cond_jump), 0x0021405B (jump)
  0x0021403F  ff750c                  push     dword ptr [ebp + 0xc]          
  0x00214042  50                      push     eax                            
  0x00214043  e8d5faffff              call     0x213b1d                       ; -> sub_00213B1D
                                        ; XREF: 0x002140B5 (jump)
  0x00214048  5d                      pop      ebp                            
  0x00214049  c20800                  ret      8                              
; end of function
                                        ; XREF: 0x00214030 (cond_jump)
  0x0021404C  0fb7c9                  movzx    ecx, cx                        
  0x0021404F  3b4814                  cmp      ecx, dword ptr [eax + 0x14]    
  0x00214052  7409                    je       0x21405d                       
  0x00214054  c7400400000080          mov      dword ptr [eax + 4], 0x80000000 
  0x0021405B  ebe2                    jmp      0x21403f                       
                                        ; XREF: 0x00214052 (cond_jump)
  0x0021405D  660fb60511a24000        movzx    ax, byte ptr [0x40a211]        
  0x00214065  c7058ca14000c43d2100    mov      dword ptr [0x40a18c], 0x213dc4 
  0x0021406F  89159ca14000            mov      dword ptr [0x40a19c], edx      
  0x00214075  891598a14000            mov      dword ptr [0x40a198], edx      
  0x0021407B  8815aca14000            mov      byte ptr [0x40a1ac], dl        
  0x00214081  c605ada1400009          mov      byte ptr [0x40a1ad], 9         
  0x00214088  66a3aea14000            mov      word ptr [0x40a1ae], ax        
  0x0021408E  668915b0a14000          mov      word ptr [0x40a1b0], dx        
  0x00214095  668915b2a14000          mov      word ptr [0x40a1b2], dx        
  0x0021409C  e8a4f5ffff              call     0x213645                       ; -> sub_00213645
  0x002140A1  8b450c                  mov      eax, dword ptr [ebp + 0xc]     
  0x002140A4  8b400c                  mov      eax, dword ptr [eax + 0xc]     
  0x002140A7  6884a14000              push     0x40a184                       
  0x002140AC  83c018                  add      eax, 0x18                      
  0x002140AF  50                      push     eax                            
  0x002140B0  e8bc120000              call     0x215371                       ; -> sub_00215371
  0x002140B5  eb91                    jmp      0x214048                       

; ============================================================
; Function: sub_002140B7
; Start: 0x002140B7  End: 0x002140DC  Size: 37 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00213AB0
; ============================================================
sub_002140B7:
  0x002140B7  55                      push     ebp                            
  0x002140B8  8bec                    mov      ebp, esp                       
  0x002140BA  51                      push     ecx                            
  0x002140BB  53                      push     ebx                            
  0x002140BC  33db                    xor      ebx, ebx                       
  0x002140BE  381d81a14000            cmp      byte ptr [0x40a181], bl        
  0x002140C4  56                      push     esi                            
  0x002140C5  8bf2                    mov      esi, edx                       
  0x002140C7  c60583a1400004          mov      byte ptr [0x40a183], 4         
  0x002140CE  740c                    je       0x2140dc                       
  0x002140D0  56                      push     esi                            
  0x002140D1  53                      push     ebx                            
  0x002140D2  e8d9f9ffff              call     0x213ab0                       ; -> sub_00213AB0
  0x002140D7  e922010000              jmp      0x2141fe                       
; end of function
                                        ; XREF: 0x002140CE (cond_jump)
  0x002140DC  a008a24000              mov      al, byte ptr [0x40a208]        
  0x002140E1  3ac3                    cmp      al, bl                         
  0x002140E3  7434                    je       0x214119                       
  0x002140E5  3c09                    cmp      al, 9                          
  0x002140E7  0f95c0                  setne    al                             
  0x002140EA  fec0                    inc      al                             
  0x002140EC  8806                    mov      byte ptr [esi], al             
  0x002140EE  a008a24000              mov      al, byte ptr [0x40a208]        
  0x002140F3  8845fd                  mov      byte ptr [ebp - 3], al         
  0x002140F6  a009a24000              mov      al, byte ptr [0x40a209]        
  0x002140FB  8845fe                  mov      byte ptr [ebp - 2], al         
  0x002140FE  a00aa24000              mov      al, byte ptr [0x40a20a]        
  0x00214103  8845ff                  mov      byte ptr [ebp - 1], al         
  0x00214106  c645fc81                mov      byte ptr [ebp - 4], 0x81       
  0x0021410A  ff75fc                  push     dword ptr [ebp - 4]            
  0x0021410D  8bce                    mov      ecx, esi                       
  0x0021410F  e8a1fbffff              call     0x213cb5                       ; -> sub_00213CB5
  0x00214114  e9e5000000              jmp      0x2141fe                       
                                        ; XREF: 0x002140E3 (cond_jump)
  0x00214119  660fb6050ba24000        movzx    ax, byte ptr [0x40a20b]        
  0x00214121  66a3a0a14000            mov      word ptr [0x40a1a0], ax        
  0x00214127  c60584a1400020          mov      byte ptr [0x40a184], 0x20      
  0x0021412E  c60585a1400002          mov      byte ptr [0x40a185], 2         
  0x00214135  891d8ca14000            mov      dword ptr [0x40a18c], ebx      
  0x0021413B  881d99a14000            mov      byte ptr [0x40a199], bl        
  0x00214141  881d9aa14000            mov      byte ptr [0x40a19a], bl        
  0x00214147  881d9ba14000            mov      byte ptr [0x40a19b], bl        
  0x0021414D  8a4604                  mov      al, byte ptr [esi + 4]         
  0x00214150  c0e807                  shr      al, 7                          
  0x00214153  a2a2a14000              mov      byte ptr [0x40a1a2], al        
  0x00214158  8a4605                  mov      al, byte ptr [esi + 5]         
  0x0021415B  57                      push     edi                            
  0x0021415C  a298a14000              mov      byte ptr [0x40a198], al        
  0x00214161  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00214164  bf84a14000              mov      edi, 0x40a184                  
  0x00214169  57                      push     edi                            
  0x0021416A  83c018                  add      eax, 0x18                      
  0x0021416D  50                      push     eax                            
  0x0021416E  e8fe110000              call     0x215371                       ; -> sub_00215371
  0x00214173  a194a14000              mov      eax, dword ptr [0x40a194]      
  0x00214178  894608                  mov      dword ptr [esi + 8], eax       
  0x0021417B  c60584a1400030          mov      byte ptr [0x40a184], 0x30      
  0x00214182  c60585a1400040          mov      byte ptr [0x40a185], 0x40      
  0x00214189  c7058ca14000fe3f2100    mov      dword ptr [0x40a18c], 0x213ffe 
  0x00214193  893590a14000            mov      dword ptr [0x40a190], esi      
  0x00214199  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x0021419C  6a50                    push     0x50                           
  0x0021419E  a394a14000              mov      dword ptr [0x40a194], eax      
  0x002141A3  58                      pop      eax                            
  0x002141A4  c7059ca140000ca24000    mov      dword ptr [0x40a19c], 0x40a20c 
  0x002141AE  a398a14000              mov      dword ptr [0x40a198], eax      
  0x002141B3  c605a0a1400002          mov      byte ptr [0x40a1a0], 2         
  0x002141BA  c605a1a1400001          mov      byte ptr [0x40a1a1], 1         
  0x002141C1  881da2a14000            mov      byte ptr [0x40a1a2], bl        
  0x002141C7  c605aca1400080          mov      byte ptr [0x40a1ac], 0x80      
  0x002141CE  c605ada1400006          mov      byte ptr [0x40a1ad], 6         
  0x002141D5  66c705aea140000002      mov      word ptr [0x40a1ae], 0x200     
  0x002141DE  66891db0a14000          mov      word ptr [0x40a1b0], bx        
  0x002141E5  66a3b2a14000            mov      word ptr [0x40a1b2], ax        
  0x002141EB  e855f4ffff              call     0x213645                       ; -> sub_00213645
  0x002141F0  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x002141F3  57                      push     edi                            
  0x002141F4  83c018                  add      eax, 0x18                      
  0x002141F7  50                      push     eax                            
  0x002141F8  e874110000              call     0x215371                       ; -> sub_00215371
  0x002141FD  5f                      pop      edi                            
                                        ; XREF: 0x002140D7 (jump), 0x00214114 (jump)
  0x002141FE  5e                      pop      esi                            
  0x002141FF  5b                      pop      ebx                            
  0x00214200  c9                      leave                                   
  0x00214201  c3                      ret                                     
  0x00214202  0fb605f8a14000          movzx    eax, byte ptr [0x40a1f8]       
  0x00214209  83e800                  sub      eax, 0                         
  0x0021420C  7440                    je       0x21424e                       
  0x0021420E  48                      dec      eax                            
  0x0021420F  7425                    je       0x214236                       
  0x00214211  48                      dec      eax                            
  0x00214212  7415                    je       0x214229                       
  0x00214214  48                      dec      eax                            
  0x00214215  753c                    jne      0x214253                       
  0x00214217  8b1500a24000            mov      edx, dword ptr [0x40a200]      
  0x0021421D  b984a14000              mov      ecx, 0x40a184                  
  0x00214222  e890feffff              call     0x2140b7                       ; -> sub_002140B7
  0x00214227  eb2a                    jmp      0x214253                       
                                        ; XREF: 0x00214212 (cond_jump)
  0x00214229  8b0d00a24000            mov      ecx, dword ptr [0x40a200]      
  0x0021422F  e8cdfcffff              call     0x213f01                       ; -> sub_00213F01
  0x00214234  eb1d                    jmp      0x214253                       
                                        ; XREF: 0x0021420F (cond_jump)
  0x00214236  a100a24000              mov      eax, dword ptr [0x40a200]      
  0x0021423B  8b400c                  mov      eax, dword ptr [eax + 0xc]     
  0x0021423E  6884a14000              push     0x40a184                       
  0x00214243  83c018                  add      eax, 0x18                      
  0x00214246  50                      push     eax                            
  0x00214247  e8b40f0000              call     0x215200                       ; -> sub_00215200
  0x0021424C  eb05                    jmp      0x214253                       
                                        ; XREF: 0x0021420C (cond_jump)
  0x0021424E  e80af5ffff              call     0x21375d                       ; -> sub_0021375D
                                        ; XREF: 0x00214215 (cond_jump), 0x00214227 (jump), 0x00214234 (jump), 0x0021424C (jump)
  0x00214253  c21000                  ret      0x10                           
  0x00214256  6a00                    push     0                              
  0x00214258  6a00                    push     0                              
  0x0021425A  ff742410                push     dword ptr [esp + 0x10]         
  0x0021425E  ff150c7b2100            call     dword ptr [0x217b0c]           ; -> xbox_KeSetEvent
  0x00214264  c20800                  ret      8                              

; ============================================================
; Function: sub_00214267
; Start: 0x00214267  End: 0x0021427A  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00215200
; ============================================================
sub_00214267:
  0x00214267  8b410c                  mov      eax, dword ptr [ecx + 0xc]     
  0x0021426A  ff742404                push     dword ptr [esp + 4]            
  0x0021426E  83c018                  add      eax, 0x18                      
  0x00214271  50                      push     eax                            
  0x00214272  e8890f0000              call     0x215200                       ; -> sub_00215200
  0x00214277  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_0021427A
; Start: 0x0021427A  End: 0x0021427E  Size: 4 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_0021559C, sub_002155F9, sub_00215769, sub_00215850, sub_002159C9, sub_00215BEB, sub_00215CB4, sub_00215E76
; ============================================================
sub_0021427A:
  0x0021427A  8b411c                  mov      eax, dword ptr [ecx + 0x1c]    
  0x0021427D  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_0021427E
; Start: 0x0021427E  End: 0x0021428B  Size: 13 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00211B6B
; ============================================================
sub_0021427E:
  0x0021427E  8b542404                mov      edx, dword ptr [esp + 4]       
  0x00214282  8b411c                  mov      eax, dword ptr [ecx + 0x1c]    
  0x00214285  89511c                  mov      dword ptr [ecx + 0x1c], edx    
  0x00214288  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_0021428B
; Start: 0x0021428B  End: 0x0021428F  Size: 4 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0021428B:
  0x0021428B  8a4102                  mov      al, byte ptr [ecx + 2]         
  0x0021428E  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_0021428F
; Start: 0x0021428F  End: 0x00214299  Size: 10 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0021428F:
  0x0021428F  8a442404                mov      al, byte ptr [esp + 4]         
  0x00214293  884107                  mov      byte ptr [ecx + 7], al         
  0x00214296  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00214299
; Start: 0x00214299  End: 0x00214303  Size: 106 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002118A4, sub_00211F07, sub_002121E5
; ============================================================
sub_00214299:
  0x00214299  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021429D  b90f0000c0              mov      ecx, 0xc000000f                
  0x002142A2  3bc1                    cmp      eax, ecx                       
  0x002142A4  7f40                    jg       0x2142e6                       
  0x002142A6  7437                    je       0x2142df                       
  0x002142A8  3d00000080              cmp      eax, 0x80000000                
  0x002142AD  741c                    je       0x2142cb                       
  0x002142AF  3d00010080              cmp      eax, 0x80000100                
  0x002142B4  7424                    je       0x2142da                       
  0x002142B6  3d00080080              cmp      eax, 0x80000800                
  0x002142BB  7416                    je       0x2142d3                       
  0x002142BD  3dffffffbf              cmp      eax, 0xbfffffff                
  0x002142C2  7e34                    jle      0x2142f8                       
  0x002142C4  3d0e0000c0              cmp      eax, 0xc000000e                
  0x002142C9  7f2d                    jg       0x2142f8                       
                                        ; XREF: 0x002142AD (cond_jump), 0x002142EB (cond_jump)
  0x002142CB  b85d040000              mov      eax, 0x45d                     
                                        ; XREF: 0x002142D8 (jump), 0x002142DD (jump), 0x002142E4 (jump), 0x00214301 (jump), 0x00214305 (jump)
  0x002142D0  c20400                  ret      4                              
                                        ; XREF: 0x002142BB (cond_jump)
  0x002142D3  b8aa050000              mov      eax, 0x5aa                     
  0x002142D8  ebf6                    jmp      0x2142d0                       
                                        ; XREF: 0x002142B4 (cond_jump)
  0x002142DA  6a0e                    push     0xe                            
                                        ; XREF: 0x002142FA (jump)
  0x002142DC  58                      pop      eax                            
  0x002142DD  ebf1                    jmp      0x2142d0                       
                                        ; XREF: 0x002142A6 (cond_jump)
  0x002142DF  b8c7040000              mov      eax, 0x4c7                     
  0x002142E4  ebea                    jmp      0x2142d0                       
                                        ; XREF: 0x002142A4 (cond_jump)
  0x002142E6  3d100000c0              cmp      eax, 0xc0000010                
  0x002142EB  74de                    je       0x2142cb                       
  0x002142ED  85c0                    test     eax, eax                       
  0x002142EF  7412                    je       0x214303                       
  0x002142F1  3d00000040              cmp      eax, 0x40000000                
  0x002142F6  7404                    je       0x2142fc                       
                                        ; XREF: 0x002142C2 (cond_jump), 0x002142C9 (cond_jump)
  0x002142F8  6a1f                    push     0x1f                           
  0x002142FA  ebe0                    jmp      0x2142dc                       
                                        ; XREF: 0x002142F6 (cond_jump)
  0x002142FC  b8e5030000              mov      eax, 0x3e5                     
  0x00214301  ebcd                    jmp      0x2142d0                       
; end of function
                                        ; XREF: 0x002142EF (cond_jump)
  0x00214303  33c0                    xor      eax, eax                       
  0x00214305  ebc9                    jmp      0x2142d0                       

; ============================================================
; Function: sub_00214307
; Start: 0x00214307  End: 0x0021436F  Size: 104 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00214307:
  0x00214307  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021430B  b90f0000c0              mov      ecx, 0xc000000f                
  0x00214310  3bc1                    cmp      eax, ecx                       
  0x00214312  7f3b                    jg       0x21434f                       
  0x00214314  7432                    je       0x214348                       
  0x00214316  3d00000080              cmp      eax, 0x80000000                
  0x0021431B  741c                    je       0x214339                       
  0x0021431D  3d00010080              cmp      eax, 0x80000100                
  0x00214322  741d                    je       0x214341                       
  0x00214324  3d00080080              cmp      eax, 0x80000800                
  0x00214329  7416                    je       0x214341                       
  0x0021432B  3dffffffbf              cmp      eax, 0xbfffffff                
  0x00214330  7e2f                    jle      0x214361                       
  0x00214332  3d0e0000c0              cmp      eax, 0xc000000e                
  0x00214337  7f28                    jg       0x214361                       
                                        ; XREF: 0x0021431B (cond_jump), 0x00214354 (cond_jump)
  0x00214339  b8850100c0              mov      eax, 0xc0000185                
                                        ; XREF: 0x00214346 (jump), 0x0021434D (jump), 0x00214366 (jump), 0x0021436D (jump), 0x00214371 (jump)
  0x0021433E  c20400                  ret      4                              
                                        ; XREF: 0x00214322 (cond_jump), 0x00214329 (cond_jump)
  0x00214341  b89a0000c0              mov      eax, 0xc000009a                
  0x00214346  ebf6                    jmp      0x21433e                       
                                        ; XREF: 0x00214314 (cond_jump)
  0x00214348  b8200100c0              mov      eax, 0xc0000120                
  0x0021434D  ebef                    jmp      0x21433e                       
                                        ; XREF: 0x00214312 (cond_jump)
  0x0021434F  3d100000c0              cmp      eax, 0xc0000010                
  0x00214354  74e3                    je       0x214339                       
  0x00214356  85c0                    test     eax, eax                       
  0x00214358  7415                    je       0x21436f                       
  0x0021435A  3d00000040              cmp      eax, 0x40000000                
  0x0021435F  7407                    je       0x214368                       
                                        ; XREF: 0x00214330 (cond_jump), 0x00214337 (cond_jump)
  0x00214361  b8010000c0              mov      eax, 0xc0000001                
  0x00214366  ebd6                    jmp      0x21433e                       
                                        ; XREF: 0x0021435F (cond_jump)
  0x00214368  b803010000              mov      eax, 0x103                     
  0x0021436D  ebcf                    jmp      0x21433e                       
; end of function
                                        ; XREF: 0x00214358 (cond_jump)
  0x0021436F  33c0                    xor      eax, eax                       
  0x00214371  ebcb                    jmp      0x21433e                       

; ============================================================
; Function: sub_00214373
; Start: 0x00214373  End: 0x00214379  Size: 6 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00212311
; ============================================================
sub_00214373:
  0x00214373  a15ca24000              mov      eax, dword ptr [0x40a25c]      
  0x00214378  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00214379
; Start: 0x00214379  End: 0x002143EF  Size: 118 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00214379:
  0x00214379  55                      push     ebp                            
  0x0021437A  8bec                    mov      ebp, esp                       
  0x0021437C  8b0d5ca24000            mov      ecx, dword ptr [0x40a25c]      
  0x00214382  53                      push     ebx                            
  0x00214383  56                      push     esi                            
  0x00214384  0fb7350ea24000          movzx    esi, word ptr [0x40a20e]       
  0x0021438B  57                      push     edi                            
  0x0021438C  81c60ca24000            add      esi, 0x40a20c                  
  0x00214392  33ff                    xor      edi, edi                       
                                        ; XREF: 0x002143E0 (cond_jump)
  0x00214394  8a11                    mov      dl, byte ptr [ecx]             
  0x00214396  84d2                    test     dl, dl                         
  0x00214398  744c                    je       0x2143e6                       
  0x0021439A  0fb6c2                  movzx    eax, dl                        
  0x0021439D  03c8                    add      ecx, eax                       
  0x0021439F  3bce                    cmp      ecx, esi                       
  0x002143A1  7343                    jae      0x2143e6                       
  0x002143A3  8a4101                  mov      al, byte ptr [ecx + 1]         
  0x002143A6  3c05                    cmp      al, 5                          
  0x002143A8  7534                    jne      0x2143de                       
  0x002143AA  8a5103                  mov      dl, byte ptr [ecx + 3]         
  0x002143AD  80e203                  and      dl, 3                          
  0x002143B0  3a5508                  cmp      dl, byte ptr [ebp + 8]         
  0x002143B3  7529                    jne      0x2143de                       
  0x002143B5  807d0800                cmp      byte ptr [ebp + 8], 0          
  0x002143B9  7423                    je       0x2143de                       
  0x002143BB  33d2                    xor      edx, edx                       
  0x002143BD  8a5102                  mov      dl, byte ptr [ecx + 2]         
  0x002143C0  33db                    xor      ebx, ebx                       
  0x002143C2  c1ea07                  shr      edx, 7                         
  0x002143C5  f7d2                    not      edx                            
  0x002143C7  83e201                  and      edx, 1                         
  0x002143CA  385d0c                  cmp      byte ptr [ebp + 0xc], bl       
  0x002143CD  0f94c3                  sete     bl                             
  0x002143D0  3bd3                    cmp      edx, ebx                       
  0x002143D2  750a                    jne      0x2143de                       
  0x002143D4  8a5510                  mov      dl, byte ptr [ebp + 0x10]      
  0x002143D7  fe4d10                  dec      byte ptr [ebp + 0x10]          
  0x002143DA  84d2                    test     dl, dl                         
  0x002143DC  7406                    je       0x2143e4                       
                                        ; XREF: 0x002143A8 (cond_jump), 0x002143B3 (cond_jump), 0x002143B9 (cond_jump), 0x002143D2 (cond_jump)
  0x002143DE  3c04                    cmp      al, 4                          
  0x002143E0  75b2                    jne      0x214394                       
  0x002143E2  eb02                    jmp      0x2143e6                       
                                        ; XREF: 0x002143DC (cond_jump)
  0x002143E4  8bf9                    mov      edi, ecx                       
                                        ; XREF: 0x00214398 (cond_jump), 0x002143A1 (cond_jump), 0x002143E2 (jump)
  0x002143E6  8bc7                    mov      eax, edi                       
  0x002143E8  5f                      pop      edi                            
  0x002143E9  5e                      pop      esi                            
  0x002143EA  5b                      pop      ebx                            
  0x002143EB  5d                      pop      ebp                            
  0x002143EC  c20c00                  ret      0xc                            
; end of function

; ============================================================
; Function: sub_002143EF
; Start: 0x002143EF  End: 0x002143F3  Size: 4 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00211B8E
; ============================================================
sub_002143EF:
  0x002143EF  8b4114                  mov      eax, dword ptr [ecx + 0x14]    
  0x002143F2  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002143F3
; Start: 0x002143F3  End: 0x00214421  Size: 46 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002128D5, sub_00213539
; Called by: sub_00214621
; ============================================================
sub_002143F3:
  0x002143F3  53                      push     ebx                            
  0x002143F4  57                      push     edi                            
  0x002143F5  8bf9                    mov      edi, ecx                       
  0x002143F7  33db                    xor      ebx, ebx                       
  0x002143F9  803f05                  cmp      byte ptr [edi], 5              
  0x002143FC  7523                    jne      0x214421                       
  0x002143FE  e836f1ffff              call     0x213539                       ; -> sub_00213539
  0x00214403  8bd8                    mov      ebx, eax                       
  0x00214405  8b4308                  mov      eax, dword ptr [ebx + 8]       
  0x00214408  85c0                    test     eax, eax                       
  0x0021440A  7415                    je       0x214421                       
  0x0021440C  894708                  mov      dword ptr [edi + 8], eax       
  0x0021440F  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x00214413  83600400                and      dword ptr [eax + 4], 0         
  0x00214417  50                      push     eax                            
  0x00214418  e8b8e4ffff              call     0x2128d5                       ; -> sub_002128D5
  0x0021441D  33c0                    xor      eax, eax                       
  0x0021441F  eb53                    jmp      0x214474                       
; end of function
                                        ; XREF: 0x002143FC (cond_jump), 0x0021440A (cond_jump)
  0x00214421  8a4705                  mov      al, byte ptr [edi + 5]         
  0x00214424  56                      push     esi                            
  0x00214425  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00214429  80661500                and      byte ptr [esi + 0x15], 0       
  0x0021442D  80661600                and      byte ptr [esi + 0x16], 0       
  0x00214431  884614                  mov      byte ptr [esi + 0x14], al      
  0x00214434  660fb64706              movzx    ax, byte ptr [edi + 6]         
  0x00214439  6689461c                mov      word ptr [esi + 0x1c], ax      
  0x0021443D  8a4704                  mov      al, byte ptr [edi + 4]         
  0x00214440  83661800                and      dword ptr [esi + 0x18], 0      
  0x00214444  c0e807                  shr      al, 7                          
  0x00214447  88461e                  mov      byte ptr [esi + 0x1e], al      
  0x0021444A  c6460102                mov      byte ptr [esi + 1], 2          
  0x0021444E  8b470c                  mov      eax, dword ptr [edi + 0xc]     
  0x00214451  56                      push     esi                            
  0x00214452  83c018                  add      eax, 0x18                      
  0x00214455  50                      push     eax                            
  0x00214456  e8160f0000              call     0x215371                       ; -> sub_00215371
  0x0021445B  85c0                    test     eax, eax                       
  0x0021445D  7c10                    jl       0x21446f                       
  0x0021445F  85db                    test     ebx, ebx                       
  0x00214461  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x00214464  894f08                  mov      dword ptr [edi + 8], ecx       
  0x00214467  7406                    je       0x21446f                       
  0x00214469  8b4e10                  mov      ecx, dword ptr [esi + 0x10]    
  0x0021446C  894b08                  mov      dword ptr [ebx + 8], ecx       
                                        ; XREF: 0x0021445D (cond_jump), 0x00214467 (cond_jump)
  0x0021446F  83661000                and      dword ptr [esi + 0x10], 0      
  0x00214473  5e                      pop      esi                            
                                        ; XREF: 0x0021441F (jump)
  0x00214474  5f                      pop      edi                            
  0x00214475  5b                      pop      ebx                            
  0x00214476  c20400                  ret      4                              

; ============================================================
; Function: sub_00214479
; Start: 0x00214479  End: 0x002144D3  Size: 90 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213539, sub_00213550, sub_00213567, sub_00215371
; Called by: sub_00214621
; ============================================================
sub_00214479:
  0x00214479  56                      push     esi                            
  0x0021447A  8bf1                    mov      esi, ecx                       
  0x0021447C  57                      push     edi                            
  0x0021447D  8b7e08                  mov      edi, dword ptr [esi + 8]       
  0x00214480  83660800                and      dword ptr [esi + 8], 0         
  0x00214484  803e05                  cmp      byte ptr [esi], 5              
  0x00214487  7529                    jne      0x2144b2                       
  0x00214489  e8abf0ffff              call     0x213539                       ; -> sub_00213539
  0x0021448E  8bc8                    mov      ecx, eax                       
  0x00214490  e8bbf0ffff              call     0x213550                       ; -> sub_00213550
  0x00214495  eb0c                    jmp      0x2144a3                       
                                        ; XREF: 0x002144A5 (cond_jump)
  0x00214497  397808                  cmp      dword ptr [eax + 8], edi       
  0x0021449A  7437                    je       0x2144d3                       
  0x0021449C  8bc8                    mov      ecx, eax                       
  0x0021449E  e8c4f0ffff              call     0x213567                       ; -> sub_00213567
                                        ; XREF: 0x00214495 (jump)
  0x002144A3  85c0                    test     eax, eax                       
  0x002144A5  75f0                    jne      0x214497                       
  0x002144A7  8bce                    mov      ecx, esi                       
  0x002144A9  e88bf0ffff              call     0x213539                       ; -> sub_00213539
  0x002144AE  83600800                and      dword ptr [eax + 8], 0         
                                        ; XREF: 0x00214487 (cond_jump)
  0x002144B2  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x002144B6  83601800                and      dword ptr [eax + 0x18], 0      
  0x002144BA  50                      push     eax                            
  0x002144BB  c6400143                mov      byte ptr [eax + 1], 0x43       
  0x002144BF  897810                  mov      dword ptr [eax + 0x10], edi    
  0x002144C2  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x002144C5  83c018                  add      eax, 0x18                      
  0x002144C8  50                      push     eax                            
  0x002144C9  e8a30e0000              call     0x215371                       ; -> sub_00215371
                                        ; XREF: 0x002144E3 (jump)
  0x002144CE  5f                      pop      edi                            
  0x002144CF  5e                      pop      esi                            
  0x002144D0  c20400                  ret      4                              
; end of function
                                        ; XREF: 0x0021449A (cond_jump)
  0x002144D3  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x002144D7  83600400                and      dword ptr [eax + 4], 0         
  0x002144DB  50                      push     eax                            
  0x002144DC  e8f4e3ffff              call     0x2128d5                       ; -> sub_002128D5
  0x002144E1  33c0                    xor      eax, eax                       
  0x002144E3  ebe9                    jmp      0x2144ce                       

; ============================================================
; Function: sub_002144E5
; Start: 0x002144E5  End: 0x00214516  Size: 49 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213550, sub_00213567
; Called by: sub_0021392D
; ============================================================
sub_002144E5:
  0x002144E5  e866f0ffff              call     0x213550                       ; -> sub_00213550
  0x002144EA  85c0                    test     eax, eax                       
  0x002144EC  7425                    je       0x214513                       
  0x002144EE  56                      push     esi                            
  0x002144EF  57                      push     edi                            
  0x002144F0  0fb67c240c              movzx    edi, byte ptr [esp + 0xc]      
  0x002144F5  be7fffffff              mov      esi, 0xffffff7f                
  0x002144FA  23fe                    and      edi, esi                       
                                        ; XREF: 0x0021450F (cond_jump)
  0x002144FC  0fb64804                movzx    ecx, byte ptr [eax + 4]        
  0x00214500  23ce                    and      ecx, esi                       
  0x00214502  3bcf                    cmp      ecx, edi                       
  0x00214504  740b                    je       0x214511                       
  0x00214506  8bc8                    mov      ecx, eax                       
  0x00214508  e85af0ffff              call     0x213567                       ; -> sub_00213567
  0x0021450D  85c0                    test     eax, eax                       
  0x0021450F  75eb                    jne      0x2144fc                       
                                        ; XREF: 0x00214504 (cond_jump)
  0x00214511  5f                      pop      edi                            
  0x00214512  5e                      pop      esi                            
                                        ; XREF: 0x002144EC (cond_jump)
  0x00214513  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00214516
; Start: 0x00214516  End: 0x0021455E  Size: 72 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213550, sub_00213567
; Called by: sub_0021357E, sub_00213DC4
; ============================================================
sub_00214516:
  0x00214516  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021451A  53                      push     ebx                            
  0x0021451B  8bd8                    mov      ebx, eax                       
  0x0021451D  2b1d60a24000            sub      ebx, dword ptr [0x40a260]      
  0x00214523  56                      push     esi                            
  0x00214524  8bf1                    mov      esi, ecx                       
  0x00214526  c6400380                mov      byte ptr [eax + 3], 0x80       
  0x0021452A  2b0d60a24000            sub      ecx, dword ptr [0x40a260]      
  0x00214530  c1fb05                  sar      ebx, 5                         
  0x00214533  c1f905                  sar      ecx, 5                         
  0x00214536  884801                  mov      byte ptr [eax + 1], cl         
  0x00214539  8bce                    mov      ecx, esi                       
  0x0021453B  e810f0ffff              call     0x213550                       ; -> sub_00213550
  0x00214540  85c0                    test     eax, eax                       
  0x00214542  750c                    jne      0x214550                       
  0x00214544  885e02                  mov      byte ptr [esi + 2], bl         
  0x00214547  eb10                    jmp      0x214559                       
                                        ; XREF: 0x00214554 (cond_jump)
  0x00214549  8bc8                    mov      ecx, eax                       
  0x0021454B  e817f0ffff              call     0x213567                       ; -> sub_00213567
                                        ; XREF: 0x00214542 (cond_jump)
  0x00214550  80780380                cmp      byte ptr [eax + 3], 0x80       
  0x00214554  75f3                    jne      0x214549                       
  0x00214556  885803                  mov      byte ptr [eax + 3], bl         
                                        ; XREF: 0x00214547 (jump)
  0x00214559  5e                      pop      esi                            
  0x0021455A  5b                      pop      ebx                            
  0x0021455B  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_0021455E
; Start: 0x0021455E  End: 0x002145BA  Size: 92 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213550, sub_00213567
; Called by: sub_002137C4, sub_0021392D, sub_002139E9
; ============================================================
sub_0021455E:
  0x0021455E  53                      push     ebx                            
  0x0021455F  55                      push     ebp                            
  0x00214560  56                      push     esi                            
  0x00214561  57                      push     edi                            
  0x00214562  8be9                    mov      ebp, ecx                       
  0x00214564  e8e7efffff              call     0x213550                       ; -> sub_00213550
  0x00214569  8b742414                mov      esi, dword ptr [esp + 0x14]    
  0x0021456D  8bf8                    mov      edi, eax                       
  0x0021456F  3bfe                    cmp      edi, esi                       
  0x00214571  b301                    mov      bl, 1                          
  0x00214573  750e                    jne      0x214583                       
  0x00214575  8a4603                  mov      al, byte ptr [esi + 3]         
  0x00214578  3c80                    cmp      al, 0x80                       
  0x0021457A  884502                  mov      byte ptr [ebp + 2], al         
  0x0021457D  752a                    jne      0x2145a9                       
  0x0021457F  32db                    xor      bl, bl                         
  0x00214581  eb26                    jmp      0x2145a9                       
                                        ; XREF: 0x00214573 (cond_jump)
  0x00214583  85ff                    test     edi, edi                       
  0x00214585  7422                    je       0x2145a9                       
                                        ; XREF: 0x0021459D (cond_jump)
  0x00214587  8bcf                    mov      ecx, edi                       
  0x00214589  e8d9efffff              call     0x213567                       ; -> sub_00213567
  0x0021458E  3bc6                    cmp      eax, esi                       
  0x00214590  740d                    je       0x21459f                       
  0x00214592  8bcf                    mov      ecx, edi                       
  0x00214594  e8ceefffff              call     0x213567                       ; -> sub_00213567
  0x00214599  8bf8                    mov      edi, eax                       
  0x0021459B  85ff                    test     edi, edi                       
  0x0021459D  75e8                    jne      0x214587                       
                                        ; XREF: 0x00214590 (cond_jump)
  0x0021459F  85ff                    test     edi, edi                       
  0x002145A1  7406                    je       0x2145a9                       
  0x002145A3  8a4603                  mov      al, byte ptr [esi + 3]         
  0x002145A6  884703                  mov      byte ptr [edi + 3], al         
                                        ; XREF: 0x0021457D (cond_jump), 0x00214581 (jump), 0x00214585 (cond_jump), 0x002145A1 (cond_jump)
  0x002145A9  5f                      pop      edi                            
  0x002145AA  c6460380                mov      byte ptr [esi + 3], 0x80       
  0x002145AE  c6460180                mov      byte ptr [esi + 1], 0x80       
  0x002145B2  5e                      pop      esi                            
  0x002145B3  5d                      pop      ebp                            
  0x002145B4  8ac3                    mov      al, bl                         
  0x002145B6  5b                      pop      ebx                            
  0x002145B7  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002145BA
; Start: 0x002145BA  End: 0x002145DA  Size: 32 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_002145BA:
  0x002145BA  8b442404                mov      eax, dword ptr [esp + 4]       
  0x002145BE  8b5004                  mov      edx, dword ptr [eax + 4]       
  0x002145C1  0fb65204                movzx    edx, byte ptr [edx + 4]        
  0x002145C5  83e27f                  and      edx, 0x7f                      
  0x002145C8  4a                      dec      edx                            
  0x002145C9  83fa04                  cmp      edx, 4                         
  0x002145CC  895114                  mov      dword ptr [ecx + 0x14], edx    
  0x002145CF  7c09                    jl       0x2145da                       
  0x002145D1  c7411420000000          mov      dword ptr [ecx + 0x14], 0x20   
  0x002145D8  eb44                    jmp      0x21461e                       
; end of function
                                        ; XREF: 0x002145CF (cond_jump)
  0x002145DA  56                      push     esi                            
  0x002145DB  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x002145DF  83f202                  xor      edx, 2                         
  0x002145E2  895114                  mov      dword ptr [ecx + 0x14], edx    
  0x002145E5  57                      push     edi                            
  0x002145E6  8b3cb0                  mov      edi, dword ptr [eax + esi*4]   
  0x002145E9  803f01                  cmp      byte ptr [edi], 1              
  0x002145EC  750e                    jne      0x2145fc                       
  0x002145EE  83fe01                  cmp      esi, 1                         
  0x002145F1  7429                    je       0x21461c                       
                                        ; XREF: 0x0021460F (cond_jump)
  0x002145F3  c7411420000000          mov      dword ptr [ecx + 0x14], 0x20   
  0x002145FA  eb20                    jmp      0x21461c                       
                                        ; XREF: 0x002145EC (cond_jump)
  0x002145FC  83fe01                  cmp      esi, 1                         
  0x002145FF  761b                    jbe      0x21461c                       
  0x00214601  8b4008                  mov      eax, dword ptr [eax + 8]       
  0x00214604  0fb64004                movzx    eax, byte ptr [eax + 4]        
  0x00214608  83e07f                  and      eax, 0x7f                      
  0x0021460B  48                      dec      eax                            
  0x0021460C  83f803                  cmp      eax, 3                         
  0x0021460F  77e2                    ja       0x2145f3                       
  0x00214611  83f802                  cmp      eax, 2                         
  0x00214614  7506                    jne      0x21461c                       
  0x00214616  83c210                  add      edx, 0x10                      
  0x00214619  895114                  mov      dword ptr [ecx + 0x14], edx    
                                        ; XREF: 0x002145F1 (cond_jump), 0x002145FA (jump), 0x002145FF (cond_jump), 0x00214614 (cond_jump)
  0x0021461C  5f                      pop      edi                            
  0x0021461D  5e                      pop      esi                            
                                        ; XREF: 0x002145D8 (jump)
  0x0021461E  c20800                  ret      8                              

; ============================================================
; Function: sub_00214621
; Start: 0x00214621  End: 0x0021470E  Size: 237 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_002143F3, sub_00214479, sub_00215371
; Called by: sub_002118A4, sub_00211BFC, sub_002121E5, sub_00212311, sub_00214925, sub_00214BD9, sub_0021559C, sub_00215747, sub_00215769, sub_00215850
; ============================================================
sub_00214621:
  0x00214621  55                      push     ebp                            
  0x00214622  8bec                    mov      ebp, esp                       
  0x00214624  83ec14                  sub      esp, 0x14                      
  0x00214627  53                      push     ebx                            
  0x00214628  56                      push     esi                            
  0x00214629  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x0021462C  8a4601                  mov      al, byte ptr [esi + 1]         
  0x0021462F  33db                    xor      ebx, ebx                       
  0x00214631  a840                    test     al, 0x40                       
  0x00214633  885dff                  mov      byte ptr [ebp - 1], bl         
  0x00214636  742c                    je       0x214664                       
  0x00214638  395e08                  cmp      dword ptr [esi + 8], ebx       
  0x0021463B  7527                    jne      0x214664                       
  0x0021463D  8d55f4                  lea      edx, [ebp - 0xc]               
  0x00214640  8955f8                  mov      dword ptr [ebp - 8], edx       
  0x00214643  8d55f4                  lea      edx, [ebp - 0xc]               
  0x00214646  8955f4                  mov      dword ptr [ebp - 0xc], edx     
  0x00214649  8d55ec                  lea      edx, [ebp - 0x14]              
  0x0021464C  c645ff01                mov      byte ptr [ebp - 1], 1          
  0x00214650  885dec                  mov      byte ptr [ebp - 0x14], bl      
  0x00214653  c645ee04                mov      byte ptr [ebp - 0x12], 4       
  0x00214657  895df0                  mov      dword ptr [ebp - 0x10], ebx    
  0x0021465A  c7460856422100          mov      dword ptr [esi + 8], 0x214256  
  0x00214661  89560c                  mov      dword ptr [esi + 0xc], edx     
                                        ; XREF: 0x00214636 (cond_jump), 0x0021463B (cond_jump)
  0x00214664  0fb6c0                  movzx    eax, al                        
  0x00214667  48                      dec      eax                            
  0x00214668  48                      dec      eax                            
  0x00214669  744f                    je       0x2146ba                       
  0x0021466B  83e807                  sub      eax, 7                         
  0x0021466E  7442                    je       0x2146b2                       
  0x00214670  83e837                  sub      eax, 0x37                      
  0x00214673  7427                    je       0x21469c                       
  0x00214675  83e803                  sub      eax, 3                         
  0x00214678  741a                    je       0x214694                       
  0x0021467A  83e83f                  sub      eax, 0x3f                      
  0x0021467D  740d                    je       0x21468c                       
  0x0021467F  83e841                  sub      eax, 0x41                      
  0x00214682  754b                    jne      0x2146cf                       
  0x00214684  56                      push     esi                            
  0x00214685  e8effdffff              call     0x214479                       ; -> sub_00214479
  0x0021468A  eb50                    jmp      0x2146dc                       
                                        ; XREF: 0x0021467D (cond_jump)
  0x0021468C  56                      push     esi                            
  0x0021468D  e861fdffff              call     0x2143f3                       ; -> sub_002143F3
  0x00214692  eb48                    jmp      0x2146dc                       
                                        ; XREF: 0x00214678 (cond_jump)
  0x00214694  8d4118                  lea      eax, [ecx + 0x18]              
  0x00214697  894618                  mov      dword ptr [esi + 0x18], eax    
  0x0021469A  eb33                    jmp      0x2146cf                       
                                        ; XREF: 0x00214673 (cond_jump)
  0x0021469C  395e10                  cmp      dword ptr [esi + 0x10], ebx    
  0x0021469F  7506                    jne      0x2146a7                       
  0x002146A1  8b4108                  mov      eax, dword ptr [ecx + 8]       
  0x002146A4  894610                  mov      dword ptr [esi + 0x10], eax    
                                        ; XREF: 0x0021469F (cond_jump)
  0x002146A7  807e2909                cmp      byte ptr [esi + 0x29], 9       
  0x002146AB  7522                    jne      0x2146cf                       
  0x002146AD  895918                  mov      dword ptr [ecx + 0x18], ebx    
  0x002146B0  eb1d                    jmp      0x2146cf                       
                                        ; XREF: 0x0021466E (cond_jump)
  0x002146B2  8a4105                  mov      al, byte ptr [ecx + 5]         
  0x002146B5  884614                  mov      byte ptr [esi + 0x14], al      
  0x002146B8  eb15                    jmp      0x2146cf                       
                                        ; XREF: 0x00214669 (cond_jump)
  0x002146BA  8a4105                  mov      al, byte ptr [ecx + 5]         
  0x002146BD  884614                  mov      byte ptr [esi + 0x14], al      
  0x002146C0  8d4118                  lea      eax, [ecx + 0x18]              
  0x002146C3  894618                  mov      dword ptr [esi + 0x18], eax    
  0x002146C6  8a4104                  mov      al, byte ptr [ecx + 4]         
  0x002146C9  c0e807                  shr      al, 7                          
  0x002146CC  88461e                  mov      byte ptr [esi + 0x1e], al      
                                        ; XREF: 0x00214682 (cond_jump), 0x0021469A (jump), 0x002146AB (cond_jump), 0x002146B0 (jump), 0x002146B8 (jump)
  0x002146CF  8b410c                  mov      eax, dword ptr [ecx + 0xc]     
  0x002146D2  56                      push     esi                            
  0x002146D3  83c018                  add      eax, 0x18                      
  0x002146D6  50                      push     eax                            
  0x002146D7  e8950c0000              call     0x215371                       ; -> sub_00215371
                                        ; XREF: 0x0021468A (jump), 0x00214692 (jump)
  0x002146DC  385dff                  cmp      byte ptr [ebp - 1], bl         
  0x002146DF  7427                    je       0x214708                       
  0x002146E1  8bc8                    mov      ecx, eax                       
  0x002146E3  81e1000000c0            and      ecx, 0xc0000000                
  0x002146E9  81f900000040            cmp      ecx, 0x40000000                
  0x002146EF  7511                    jne      0x214702                       
  0x002146F1  53                      push     ebx                            
  0x002146F2  53                      push     ebx                            
  0x002146F3  53                      push     ebx                            
  0x002146F4  53                      push     ebx                            
  0x002146F5  8d45ec                  lea      eax, [ebp - 0x14]              
  0x002146F8  50                      push     eax                            
  0x002146F9  ff15107b2100            call     dword ptr [0x217b10]           ; -> xbox_KeWaitForSingleObject
  0x002146FF  8b4604                  mov      eax, dword ptr [esi + 4]       
                                        ; XREF: 0x002146EF (cond_jump)
  0x00214702  895e08                  mov      dword ptr [esi + 8], ebx       
  0x00214705  895e0c                  mov      dword ptr [esi + 0xc], ebx     
                                        ; XREF: 0x002146DF (cond_jump)
  0x00214708  5e                      pop      esi                            
  0x00214709  5b                      pop      ebx                            
  0x0021470A  c9                      leave                                   
  0x0021470B  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_0021470E
; Start: 0x0021470E  End: 0x00214731  Size: 35 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00214750
; ============================================================
sub_0021470E:
  0x0021470E  56                      push     esi                            
  0x0021470F  8b742408                mov      esi, dword ptr [esp + 8]       
  0x00214713  8d4604                  lea      eax, [esi + 4]                 
  0x00214716  8b10                    mov      edx, dword ptr [eax]           
  0x00214718  803a01                  cmp      byte ptr [edx], 1              
  0x0021471B  750b                    jne      0x214728                       
  0x0021471D  8a5204                  mov      dl, byte ptr [edx + 4]         
  0x00214720  80e27f                  and      dl, 0x7f                       
  0x00214723  80fa01                  cmp      dl, 1                          
  0x00214726  7409                    je       0x214731                       
                                        ; XREF: 0x0021471B (cond_jump)
  0x00214728  c7411420000000          mov      dword ptr [ecx + 0x14], 0x20   
  0x0021472F  eb1b                    jmp      0x21474c                       
; end of function
                                        ; XREF: 0x00214726 (cond_jump)
  0x00214731  8b54240c                mov      edx, dword ptr [esp + 0xc]     
  0x00214735  83fa01                  cmp      edx, 1                         
  0x00214738  7506                    jne      0x214740                       
  0x0021473A  83611400                and      dword ptr [ecx + 0x14], 0      
  0x0021473E  eb0c                    jmp      0x21474c                       
                                        ; XREF: 0x00214738 (cond_jump)
  0x00214740  8b36                    mov      esi, dword ptr [esi]           
  0x00214742  4a                      dec      edx                            
  0x00214743  52                      push     edx                            
  0x00214744  50                      push     eax                            
  0x00214745  8930                    mov      dword ptr [eax], esi           
  0x00214747  e86efeffff              call     0x2145ba                       ; -> sub_002145BA
                                        ; XREF: 0x0021472F (jump), 0x0021473E (jump)
  0x0021474C  5e                      pop      esi                            
  0x0021474D  c20800                  ret      8                              

; ============================================================
; Function: sub_00214750
; Start: 0x00214750  End: 0x00214792  Size: 66 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00213539, sub_0021470E
; Called by: sub_00213CB5
; ============================================================
sub_00214750:
  0x00214750  55                      push     ebp                            
  0x00214751  8bec                    mov      ebp, esp                       
  0x00214753  83ec18                  sub      esp, 0x18                      
  0x00214756  56                      push     esi                            
  0x00214757  57                      push     edi                            
  0x00214758  8bf9                    mov      edi, ecx                       
  0x0021475A  6a05                    push     5                              
  0x0021475C  5e                      pop      esi                            
  0x0021475D  897dfc                  mov      dword ptr [ebp - 4], edi       
                                        ; XREF: 0x00214771 (cond_jump)
  0x00214760  8b4cb5e8                mov      ecx, dword ptr [ebp + esi*4 - 0x18] 
  0x00214764  4e                      dec      esi                            
  0x00214765  e8cfedffff              call     0x213539                       ; -> sub_00213539
  0x0021476A  803800                  cmp      byte ptr [eax], 0              
  0x0021476D  8944b5e8                mov      dword ptr [ebp + esi*4 - 0x18], eax 
  0x00214771  75ed                    jne      0x214760                       
  0x00214773  8b15587b2100            mov      edx, dword ptr [0x217b58]      
  0x00214779  6a05                    push     5                              
  0x0021477B  58                      pop      eax                            
  0x0021477C  2bc6                    sub      eax, esi                       
  0x0021477E  f60201                  test     byte ptr [edx], 1              
  0x00214781  8d4cb5e8                lea      ecx, [ebp + esi*4 - 0x18]      
  0x00214785  50                      push     eax                            
  0x00214786  51                      push     ecx                            
  0x00214787  8bcf                    mov      ecx, edi                       
  0x00214789  7407                    je       0x214792                       
  0x0021478B  e87effffff              call     0x21470e                       ; -> sub_0021470E
  0x00214790  eb05                    jmp      0x214797                       
; end of function
                                        ; XREF: 0x00214789 (cond_jump)
  0x00214792  e823feffff              call     0x2145ba                       ; -> sub_002145BA
                                        ; XREF: 0x00214790 (jump)
  0x00214797  5f                      pop      edi                            
  0x00214798  5e                      pop      esi                            
  0x00214799  c9                      leave                                   
  0x0021479A  c3                      ret                                     

; ============================================================
; Function: sub_0021479B
; Start: 0x0021479B  End: 0x002147CE  Size: 51 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0021479B:
  0x0021479B  55                      push     ebp                            
  0x0021479C  56                      push     esi                            
  0x0021479D  8bf1                    mov      esi, ecx                       
  0x0021479F  f6460d01                test     byte ptr [esi + 0xd], 1        
  0x002147A3  8b6e08                  mov      ebp, dword ptr [esi + 8]       
  0x002147A6  57                      push     edi                            
  0x002147A7  8bfa                    mov      edi, edx                       
  0x002147A9  740e                    je       0x2147b9                       
  0x002147AB  8d4670                  lea      eax, [esi + 0x70]              
  0x002147AE  50                      push     eax                            
  0x002147AF  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x002147B5  80660dfe                and      byte ptr [esi + 0xd], 0xfe     
                                        ; XREF: 0x002147A9 (cond_jump)
  0x002147B9  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x002147BC  a806                    test     al, 6                          
  0x002147BE  740e                    je       0x2147ce                       
  0x002147C0  689d0000c0              push     0xc000009d                     
  0x002147C5  56                      push     esi                            
  0x002147C6  ff5630                  call     dword ptr [esi + 0x30]         
  0x002147C9  e9cf000000              jmp      0x21489d                       
; end of function
                                        ; XREF: 0x002147BE (cond_jump)
  0x002147CE  0d00100000              or       eax, 0x1000                    
  0x002147D3  53                      push     ebx                            
  0x002147D4  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x002147D7  8b4620                  mov      eax, dword ptr [esi + 0x20]    
  0x002147DA  897e6c                  mov      dword ptr [esi + 0x6c], edi    
  0x002147DD  8dbeb8000000            lea      edi, [esi + 0xb8]              
  0x002147E3  33db                    xor      ebx, ebx                       
  0x002147E5  c60718                  mov      byte ptr [edi], 0x18           
  0x002147E8  c686b900000005          mov      byte ptr [esi + 0xb9], 5       
  0x002147EF  899ec0000000            mov      dword ptr [esi + 0xc0], ebx    
  0x002147F5  8986c8000000            mov      dword ptr [esi + 0xc8], eax    
  0x002147FB  c786cc00000004000000    mov      dword ptr [esi + 0xcc], 4      
  0x00214805  8b4d00                  mov      ecx, dword ptr [ebp]           
  0x00214808  57                      push     edi                            
  0x00214809  e813feffff              call     0x214621                       ; -> sub_00214621
  0x0021480E  804e0d03                or       byte ptr [esi + 0xd], 3        
  0x00214812  8d9698000000            lea      edx, [esi + 0x98]              
  0x00214818  52                      push     edx                            
  0x00214819  83c9ff                  or       ecx, 0xffffffff                
  0x0021481C  51                      push     ecx                            
  0x0021481D  b8c0bdf0ff              mov      eax, 0xfff0bdc0                
  0x00214822  50                      push     eax                            
  0x00214823  8d4670                  lea      eax, [esi + 0x70]              
  0x00214826  50                      push     eax                            
  0x00214827  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x0021482D  c60730                  mov      byte ptr [edi], 0x30           
  0x00214830  c686b900000040          mov      byte ptr [esi + 0xb9], 0x40    
  0x00214837  c786c0000000824c2100    mov      dword ptr [esi + 0xc0], 0x214c82 
  0x00214841  89b6c4000000            mov      dword ptr [esi + 0xc4], esi    
  0x00214847  899ec8000000            mov      dword ptr [esi + 0xc8], ebx    
  0x0021484D  899ed0000000            mov      dword ptr [esi + 0xd0], ebx    
  0x00214853  899ecc000000            mov      dword ptr [esi + 0xcc], ebx    
  0x00214859  889ed4000000            mov      byte ptr [esi + 0xd4], bl      
  0x0021485F  889ed5000000            mov      byte ptr [esi + 0xd5], bl      
  0x00214865  889ed6000000            mov      byte ptr [esi + 0xd6], bl      
  0x0021486B  c686e000000002          mov      byte ptr [esi + 0xe0], 2       
  0x00214872  c686e100000001          mov      byte ptr [esi + 0xe1], 1       
  0x00214879  66899ee2000000          mov      word ptr [esi + 0xe2], bx      
  0x00214880  660fb64505              movzx    ax, byte ptr [ebp + 5]         
  0x00214885  668986e4000000          mov      word ptr [esi + 0xe4], ax      
  0x0021488C  66899ee6000000          mov      word ptr [esi + 0xe6], bx      
  0x00214893  8b4d00                  mov      ecx, dword ptr [ebp]           
  0x00214896  57                      push     edi                            
  0x00214897  e885fdffff              call     0x214621                       ; -> sub_00214621
  0x0021489C  5b                      pop      ebx                            
                                        ; XREF: 0x002147C9 (jump)
  0x0021489D  5f                      pop      edi                            
  0x0021489E  5e                      pop      esi                            
  0x0021489F  5d                      pop      ebp                            
  0x002148A0  c3                      ret                                     
  0x002148A1  56                      push     esi                            
  0x002148A2  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x002148A6  57                      push     edi                            
  0x002148A7  33ff                    xor      edi, edi                       
  0x002148A9  f6460d01                test     byte ptr [esi + 0xd], 1        
  0x002148AD  740e                    je       0x2148bd                       
  0x002148AF  8d4670                  lea      eax, [esi + 0x70]              
  0x002148B2  50                      push     eax                            
  0x002148B3  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x002148B9  80660dfe                and      byte ptr [esi + 0xd], 0xfe     
                                        ; XREF: 0x002148AD (cond_jump)
  0x002148BD  80660dfd                and      byte ptr [esi + 0xd], 0xfd     
  0x002148C1  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x002148C5  8b4004                  mov      eax, dword ptr [eax + 4]       
  0x002148C8  85c0                    test     eax, eax                       
  0x002148CA  7d08                    jge      0x2148d4                       
  0x002148CC  50                      push     eax                            
  0x002148CD  e835faffff              call     0x214307                       ; -> sub_00214307
  0x002148D2  8bf8                    mov      edi, eax                       
                                        ; XREF: 0x002148CA (cond_jump)
  0x002148D4  817e5f55534253          cmp      dword ptr [esi + 0x5f], 0x53425355 
  0x002148DB  b8010000c0              mov      eax, 0xc0000001                
  0x002148E0  7402                    je       0x2148e4                       
  0x002148E2  8bf8                    mov      edi, eax                       
                                        ; XREF: 0x002148E0 (cond_jump)
  0x002148E4  8b4e63                  mov      ecx, dword ptr [esi + 0x63]    
  0x002148E7  3b4e44                  cmp      ecx, dword ptr [esi + 0x44]    
  0x002148EA  7402                    je       0x2148ee                       
  0x002148EC  8bf8                    mov      edi, eax                       
                                        ; XREF: 0x002148EA (cond_jump)
  0x002148EE  8a4e6b                  mov      cl, byte ptr [esi + 0x6b]      
  0x002148F1  80f902                  cmp      cl, 2                          
  0x002148F4  7502                    jne      0x2148f8                       
  0x002148F6  8bf8                    mov      edi, eax                       
                                        ; XREF: 0x002148F4 (cond_jump)
  0x002148F8  80f901                  cmp      cl, 1                          
  0x002148FB  7505                    jne      0x214902                       
  0x002148FD  bf3e0000c0              mov      edi, 0xc000003e                
                                        ; XREF: 0x002148FB (cond_jump)
  0x00214902  b8000000c0              mov      eax, 0xc0000000                
  0x00214907  8bcf                    mov      ecx, edi                       
  0x00214909  23c8                    and      ecx, eax                       
  0x0021490B  3bc8                    cmp      ecx, eax                       
  0x0021490D  750b                    jne      0x21491a                       
  0x0021490F  8bd7                    mov      edx, edi                       
  0x00214911  8bce                    mov      ecx, esi                       
  0x00214913  e883feffff              call     0x21479b                       ; -> sub_0021479B
  0x00214918  eb06                    jmp      0x214920                       
                                        ; XREF: 0x0021490D (cond_jump)
  0x0021491A  6a00                    push     0                              
  0x0021491C  56                      push     esi                            
  0x0021491D  ff5630                  call     dword ptr [esi + 0x30]         
                                        ; XREF: 0x00214918 (jump)
  0x00214920  5f                      pop      edi                            
  0x00214921  5e                      pop      esi                            
  0x00214922  c20800                  ret      8                              

; ============================================================
; Function: sub_00214925
; Start: 0x00214925  End: 0x002149AE  Size: 137 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_001B8470, sub_00214621
; ============================================================
sub_00214925:
  0x00214925  56                      push     esi                            
  0x00214926  8bf1                    mov      esi, ecx                       
  0x00214928  8b4620                  mov      eax, dword ptr [esi + 0x20]    
  0x0021492B  80a6d500000000          and      byte ptr [esi + 0xd5], 0       
  0x00214932  80a6d600000000          and      byte ptr [esi + 0xd6], 0       
  0x00214939  8986c8000000            mov      dword ptr [esi + 0xc8], eax    
  0x0021493F  57                      push     edi                            
  0x00214940  8d465f                  lea      eax, [esi + 0x5f]              
  0x00214943  8986d0000000            mov      dword ptr [esi + 0xd0], eax    
  0x00214949  0fb74634                movzx    eax, word ptr [esi + 0x34]     
  0x0021494D  6aff                    push     -1                             
  0x0021494F  99                      cdq                                     
  0x00214950  686079feff              push     0xfffe7960                     
  0x00214955  52                      push     edx                            
  0x00214956  8dbeb8000000            lea      edi, [esi + 0xb8]              
  0x0021495C  50                      push     eax                            
  0x0021495D  c60728                  mov      byte ptr [edi], 0x28           
  0x00214960  c686b900000041          mov      byte ptr [esi + 0xb9], 0x41    
  0x00214967  c786c0000000a1482100    mov      dword ptr [esi + 0xc0], 0x2148a1 
  0x00214971  89b6c4000000            mov      dword ptr [esi + 0xc4], esi    
  0x00214977  c786cc0000000d000000    mov      dword ptr [esi + 0xcc], 0xd    
  0x00214981  c686d400000002          mov      byte ptr [esi + 0xd4], 2       
  0x00214988  e8e33afaff              call     0x1b8470                       ; -> sub_001B8470
  0x0021498D  8d8e98000000            lea      ecx, [esi + 0x98]              
  0x00214993  51                      push     ecx                            
  0x00214994  52                      push     edx                            
  0x00214995  50                      push     eax                            
  0x00214996  8d4670                  lea      eax, [esi + 0x70]              
  0x00214999  50                      push     eax                            
  0x0021499A  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x002149A0  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x002149A3  8b08                    mov      ecx, dword ptr [eax]           
  0x002149A5  57                      push     edi                            
  0x002149A6  e876fcffff              call     0x214621                       ; -> sub_00214621
  0x002149AB  5f                      pop      edi                            
  0x002149AC  5e                      pop      esi                            
  0x002149AD  c3                      ret                                     
; end of function
  0x002149AE  56                      push     esi                            
  0x002149AF  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x002149B3  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x002149B6  ba00080000              mov      edx, 0x800                     
  0x002149BB  85c2                    test     edx, eax                       
  0x002149BD  743e                    je       0x2149fd                       
  0x002149BF  25fff7ffff              and      eax, 0xfffff7ff                
  0x002149C4  8d8eb8000000            lea      ecx, [esi + 0xb8]              
  0x002149CA  394c2408                cmp      dword ptr [esp + 8], ecx       
  0x002149CE  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x002149D1  7508                    jne      0x2149db                       
  0x002149D3  8b8eec000000            mov      ecx, dword ptr [esi + 0xec]    
  0x002149D9  eb06                    jmp      0x2149e1                       
                                        ; XREF: 0x002149D1 (cond_jump)
  0x002149DB  8b8ebc000000            mov      ecx, dword ptr [esi + 0xbc]    
                                        ; XREF: 0x002149D9 (jump)
  0x002149E1  25fff9ffff              and      eax, 0xfffff9ff                
  0x002149E6  51                      push     ecx                            
  0x002149E7  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x002149EA  e818f9ffff              call     0x214307                       ; -> sub_00214307
  0x002149EF  8bd0                    mov      edx, eax                       
  0x002149F1  8bce                    mov      ecx, esi                       
  0x002149F3  e8a3fdffff              call     0x21479b                       ; -> sub_0021479B
  0x002149F8  e9aa000000              jmp      0x214aa7                       
                                        ; XREF: 0x002149BD (cond_jump)
  0x002149FD  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x00214A01  83790400                cmp      dword ptr [ecx + 4], 0         
  0x00214A05  57                      push     edi                            
  0x00214A06  7c49                    jl       0x214a51                       
  0x00214A08  a806                    test     al, 6                          
  0x00214A0A  7545                    jne      0x214a51                       
  0x00214A0C  8b566c                  mov      edx, dword ptr [esi + 0x6c]    
  0x00214A0F  3b562c                  cmp      edx, dword ptr [esi + 0x2c]    
  0x00214A12  730c                    jae      0x214a20                       
  0x00214A14  8bd6                    mov      edx, esi                       
  0x00214A16  e890000000              call     0x214aab                       ; -> sub_00214AAB
  0x00214A1B  e986000000              jmp      0x214aa6                       
                                        ; XREF: 0x00214A12 (cond_jump)
  0x00214A20  8dbeb8000000            lea      edi, [esi + 0xb8]              
  0x00214A26  3bcf                    cmp      ecx, edi                       
  0x00214A28  ba00020000              mov      edx, 0x200                     
  0x00214A2D  750a                    jne      0x214a39                       
  0x00214A2F  25fffdffff              and      eax, 0xfffffdff                
  0x00214A34  f6c404                  test     ah, 4                          
  0x00214A37  eb07                    jmp      0x214a40                       
                                        ; XREF: 0x00214A2D (cond_jump)
  0x00214A39  25fffbffff              and      eax, 0xfffffbff                
  0x00214A3E  85c2                    test     edx, eax                       
                                        ; XREF: 0x00214A37 (jump)
  0x00214A40  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214A43  7561                    jne      0x214aa6                       
  0x00214A45  09560c                  or       dword ptr [esi + 0xc], edx     
  0x00214A48  8bce                    mov      ecx, esi                       
  0x00214A4A  e8d6feffff              call     0x214925                       ; -> sub_00214925
  0x00214A4F  eb55                    jmp      0x214aa6                       
                                        ; XREF: 0x00214A06 (cond_jump), 0x00214A0A (cond_jump)
  0x00214A51  8dbeb8000000            lea      edi, [esi + 0xb8]              
  0x00214A57  3bcf                    cmp      ecx, edi                       
  0x00214A59  7525                    jne      0x214a80                       
  0x00214A5B  25fffdffff              and      eax, 0xfffffdff                
  0x00214A60  f6c404                  test     ah, 4                          
  0x00214A63  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214A66  742d                    je       0x214a95                       
  0x00214A68  0bc2                    or       eax, edx                       
  0x00214A6A  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214A6D  8d86e8000000            lea      eax, [esi + 0xe8]              
  0x00214A73  50                      push     eax                            
                                        ; XREF: 0x00214A93 (jump)
  0x00214A74  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x00214A77  8b08                    mov      ecx, dword ptr [eax]           
  0x00214A79  e8e9f7ffff              call     0x214267                       ; -> sub_00214267
  0x00214A7E  eb26                    jmp      0x214aa6                       
                                        ; XREF: 0x00214A59 (cond_jump)
  0x00214A80  25fffbffff              and      eax, 0xfffffbff                
  0x00214A85  f6c402                  test     ah, 2                          
  0x00214A88  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214A8B  7408                    je       0x214a95                       
  0x00214A8D  0bc2                    or       eax, edx                       
  0x00214A8F  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214A92  57                      push     edi                            
  0x00214A93  ebdf                    jmp      0x214a74                       
                                        ; XREF: 0x00214A66 (cond_jump), 0x00214A8B (cond_jump)
  0x00214A95  ff7104                  push     dword ptr [ecx + 4]            
  0x00214A98  e86af8ffff              call     0x214307                       ; -> sub_00214307
  0x00214A9D  8bd0                    mov      edx, eax                       
  0x00214A9F  8bce                    mov      ecx, esi                       
  0x00214AA1  e8f5fcffff              call     0x21479b                       ; -> sub_0021479B
                                        ; XREF: 0x00214A1B (jump), 0x00214A43 (cond_jump), 0x00214A4F (jump), 0x00214A7E (jump)
  0x00214AA6  5f                      pop      edi                            
                                        ; XREF: 0x002149F8 (jump)
  0x00214AA7  5e                      pop      esi                            
  0x00214AA8  c20800                  ret      8                              

; ============================================================
; Function: sub_00214AAB
; Start: 0x00214AAB  End: 0x00214ACC  Size: 33 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00214AAB:
  0x00214AAB  55                      push     ebp                            
  0x00214AAC  8bec                    mov      ebp, esp                       
  0x00214AAE  51                      push     ecx                            
  0x00214AAF  53                      push     ebx                            
  0x00214AB0  56                      push     esi                            
  0x00214AB1  57                      push     edi                            
  0x00214AB2  8bfa                    mov      edi, edx                       
  0x00214AB4  8a4737                  mov      al, byte ptr [edi + 0x37]      
  0x00214AB7  8bf1                    mov      esi, ecx                       
  0x00214AB9  8ac8                    mov      cl, al                         
  0x00214ABB  80e103                  and      cl, 3                          
  0x00214ABE  80f901                  cmp      cl, 1                          
  0x00214AC1  7509                    jne      0x214acc                       
  0x00214AC3  8b5f20                  mov      ebx, dword ptr [edi + 0x20]    
  0x00214AC6  c645ff02                mov      byte ptr [ebp - 1], 2          
  0x00214ACA  eb07                    jmp      0x214ad3                       
; end of function
                                        ; XREF: 0x00214AC1 (cond_jump)
  0x00214ACC  8b5f24                  mov      ebx, dword ptr [edi + 0x24]    
  0x00214ACF  c645ff01                mov      byte ptr [ebp - 1], 1          
                                        ; XREF: 0x00214ACA (jump)
  0x00214AD3  a804                    test     al, 4                          
  0x00214AD5  8b576c                  mov      edx, dword ptr [edi + 0x6c]    
  0x00214AD8  7428                    je       0x214b02                       
  0x00214ADA  8b4f38                  mov      ecx, dword ptr [edi + 0x38]    
  0x00214ADD  3bd1                    cmp      edx, ecx                       
  0x00214ADF  7308                    jae      0x214ae9                       
  0x00214AE1  8d82d4704000            lea      eax, [edx + 0x4070d4]          
  0x00214AE7  eb1e                    jmp      0x214b07                       
                                        ; XREF: 0x00214ADF (cond_jump)
  0x00214AE9  8b473c                  mov      eax, dword ptr [edi + 0x3c]    
  0x00214AEC  3bd0                    cmp      edx, eax                       
  0x00214AEE  7307                    jae      0x214af7                       
  0x00214AF0  8b4728                  mov      eax, dword ptr [edi + 0x28]    
  0x00214AF3  2bc1                    sub      eax, ecx                       
  0x00214AF5  eb0e                    jmp      0x214b05                       
                                        ; XREF: 0x00214AEE (cond_jump)
  0x00214AF7  2bc8                    sub      ecx, eax                       
  0x00214AF9  8d8411d4704000          lea      eax, [ecx + edx + 0x4070d4]    
  0x00214B00  eb05                    jmp      0x214b07                       
                                        ; XREF: 0x00214AD8 (cond_jump)
  0x00214B02  8b4728                  mov      eax, dword ptr [edi + 0x28]    
                                        ; XREF: 0x00214AF5 (jump)
  0x00214B05  03c2                    add      eax, edx                       
                                        ; XREF: 0x00214AE7 (jump), 0x00214B00 (jump)
  0x00214B07  8b4f2c                  mov      ecx, dword ptr [edi + 0x2c]    
  0x00214B0A  2bca                    sub      ecx, edx                       
  0x00214B0C  81f900040000            cmp      ecx, 0x400                     
  0x00214B12  7605                    jbe      0x214b19                       
  0x00214B14  b900040000              mov      ecx, 0x400                     
                                        ; XREF: 0x00214B12 (cond_jump)
  0x00214B19  03d1                    add      edx, ecx                       
  0x00214B1B  89576c                  mov      dword ptr [edi + 0x6c], edx    
  0x00214B1E  80661d00                and      byte ptr [esi + 0x1d], 0       
  0x00214B22  80661e00                and      byte ptr [esi + 0x1e], 0       
  0x00214B26  894618                  mov      dword ptr [esi + 0x18], eax    
  0x00214B29  8a45ff                  mov      al, byte ptr [ebp - 1]         
  0x00214B2C  8d9798000000            lea      edx, [edi + 0x98]              
  0x00214B32  52                      push     edx                            
  0x00214B33  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x00214B36  88461c                  mov      byte ptr [esi + 0x1c], al      
  0x00214B39  83c9ff                  or       ecx, 0xffffffff                
  0x00214B3C  51                      push     ecx                            
  0x00214B3D  b8a01ce9ff              mov      eax, 0xffe91ca0                
  0x00214B42  50                      push     eax                            
  0x00214B43  8d4770                  lea      eax, [edi + 0x70]              
  0x00214B46  50                      push     eax                            
  0x00214B47  c60628                  mov      byte ptr [esi], 0x28           
  0x00214B4A  c6460141                mov      byte ptr [esi + 1], 0x41       
  0x00214B4E  c74608ae492100          mov      dword ptr [esi + 8], 0x2149ae  
  0x00214B55  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x00214B58  895e10                  mov      dword ptr [esi + 0x10], ebx    
  0x00214B5B  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x00214B61  8b4708                  mov      eax, dword ptr [edi + 8]       
  0x00214B64  8b08                    mov      ecx, dword ptr [eax]           
  0x00214B66  56                      push     esi                            
  0x00214B67  e8b5faffff              call     0x214621                       ; -> sub_00214621
  0x00214B6C  5f                      pop      edi                            
  0x00214B6D  5e                      pop      esi                            
  0x00214B6E  5b                      pop      ebx                            
  0x00214B6F  c9                      leave                                   
  0x00214B70  c3                      ret                                     
  0x00214B71  8b442404                mov      eax, dword ptr [esp + 4]       
  0x00214B75  33c9                    xor      ecx, ecx                       
  0x00214B77  394804                  cmp      dword ptr [eax + 4], ecx       
  0x00214B7A  56                      push     esi                            
  0x00214B7B  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00214B7F  7c3f                    jl       0x214bc0                       
  0x00214B81  f6460c06                test     byte ptr [esi + 0xc], 6        
  0x00214B85  7539                    jne      0x214bc0                       
  0x00214B87  394e28                  cmp      dword ptr [esi + 0x28], ecx    
  0x00214B8A  742b                    je       0x214bb7                       
  0x00214B8C  894e6c                  mov      dword ptr [esi + 0x6c], ecx    
  0x00214B8F  8d8eb8000000            lea      ecx, [esi + 0xb8]              
  0x00214B95  8bd6                    mov      edx, esi                       
  0x00214B97  e80fffffff              call     0x214aab                       ; -> sub_00214AAB
  0x00214B9C  8b466c                  mov      eax, dword ptr [esi + 0x6c]    
  0x00214B9F  3b462c                  cmp      eax, dword ptr [esi + 0x2c]    
  0x00214BA2  7331                    jae      0x214bd5                       
  0x00214BA4  804e0d04                or       byte ptr [esi + 0xd], 4        
  0x00214BA8  8d8ee8000000            lea      ecx, [esi + 0xe8]              
  0x00214BAE  8bd6                    mov      edx, esi                       
  0x00214BB0  e8f6feffff              call     0x214aab                       ; -> sub_00214AAB
  0x00214BB5  eb1e                    jmp      0x214bd5                       
                                        ; XREF: 0x00214B8A (cond_jump)
  0x00214BB7  8bce                    mov      ecx, esi                       
  0x00214BB9  e867fdffff              call     0x214925                       ; -> sub_00214925
  0x00214BBE  eb15                    jmp      0x214bd5                       
                                        ; XREF: 0x00214B7F (cond_jump), 0x00214B85 (cond_jump)
  0x00214BC0  80660dfd                and      byte ptr [esi + 0xd], 0xfd     
  0x00214BC4  ff7004                  push     dword ptr [eax + 4]            
  0x00214BC7  e83bf7ffff              call     0x214307                       ; -> sub_00214307
  0x00214BCC  8bd0                    mov      edx, eax                       
  0x00214BCE  8bce                    mov      ecx, esi                       
  0x00214BD0  e8c6fbffff              call     0x21479b                       ; -> sub_0021479B
                                        ; XREF: 0x00214BA2 (cond_jump), 0x00214BB5 (jump), 0x00214BBE (jump)
  0x00214BD5  5e                      pop      esi                            
  0x00214BD6  c20800                  ret      8                              

; ============================================================
; Function: sub_00214BD9
; Start: 0x00214BD9  End: 0x00214C82  Size: 169 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00214621
; ============================================================
sub_00214BD9:
  0x00214BD9  56                      push     esi                            
  0x00214BDA  8bf1                    mov      esi, ecx                       
  0x00214BDC  57                      push     edi                            
  0x00214BDD  8d7e40                  lea      edi, [esi + 0x40]              
  0x00214BE0  c70755534243            mov      dword ptr [edi], 0x43425355    
  0x00214BE6  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x00214BE9  894704                  mov      dword ptr [edi + 4], eax       
  0x00214BEC  8b462c                  mov      eax, dword ptr [esi + 0x2c]    
  0x00214BEF  894708                  mov      dword ptr [edi + 8], eax       
  0x00214BF2  8a4637                  mov      al, byte ptr [esi + 0x37]      
  0x00214BF5  80670d00                and      byte ptr [edi + 0xd], 0        
  0x00214BF9  c0e007                  shl      al, 7                          
  0x00214BFC  8d9698000000            lea      edx, [esi + 0x98]              
  0x00214C02  52                      push     edx                            
  0x00214C03  88470c                  mov      byte ptr [edi + 0xc], al       
  0x00214C06  83c9ff                  or       ecx, 0xffffffff                
  0x00214C09  51                      push     ecx                            
  0x00214C0A  b800cbf3ff              mov      eax, 0xfff3cb00                
  0x00214C0F  50                      push     eax                            
  0x00214C10  8d4670                  lea      eax, [esi + 0x70]              
  0x00214C13  c6470e0a                mov      byte ptr [edi + 0xe], 0xa      
  0x00214C17  804e0d01                or       byte ptr [esi + 0xd], 1        
  0x00214C1B  50                      push     eax                            
  0x00214C1C  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x00214C22  8b4e24                  mov      ecx, dword ptr [esi + 0x24]    
  0x00214C25  804e0d02                or       byte ptr [esi + 0xd], 2        
  0x00214C29  80a6d500000000          and      byte ptr [esi + 0xd5], 0       
  0x00214C30  80a6d600000000          and      byte ptr [esi + 0xd6], 0       
  0x00214C37  8d86b8000000            lea      eax, [esi + 0xb8]              
  0x00214C3D  c60028                  mov      byte ptr [eax], 0x28           
  0x00214C40  50                      push     eax                            
  0x00214C41  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x00214C44  c686b900000041          mov      byte ptr [esi + 0xb9], 0x41    
  0x00214C4B  c786c0000000714b2100    mov      dword ptr [esi + 0xc0], 0x214b71 
  0x00214C55  89b6c4000000            mov      dword ptr [esi + 0xc4], esi    
  0x00214C5B  898ec8000000            mov      dword ptr [esi + 0xc8], ecx    
  0x00214C61  89bed0000000            mov      dword ptr [esi + 0xd0], edi    
  0x00214C67  c786cc0000001f000000    mov      dword ptr [esi + 0xcc], 0x1f   
  0x00214C71  c686d400000001          mov      byte ptr [esi + 0xd4], 1       
  0x00214C78  8b08                    mov      ecx, dword ptr [eax]           
  0x00214C7A  e8a2f9ffff              call     0x214621                       ; -> sub_00214621
  0x00214C7F  5f                      pop      edi                            
  0x00214C80  5e                      pop      esi                            
  0x00214C81  c3                      ret                                     
; end of function
  0x00214C82  53                      push     ebx                            
  0x00214C83  56                      push     esi                            
  0x00214C84  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00214C88  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00214C8B  8b5e08                  mov      ebx, dword ptr [esi + 8]       
  0x00214C8E  57                      push     edi                            
  0x00214C8F  8bf8                    mov      edi, eax                       
  0x00214C91  25fffdffff              and      eax, 0xfffffdff                
  0x00214C96  81e700700000            and      edi, 0x7000                    
  0x00214C9C  f6c401                  test     ah, 1                          
  0x00214C9F  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214CA2  740e                    je       0x214cb2                       
  0x00214CA4  8d4670                  lea      eax, [esi + 0x70]              
  0x00214CA7  50                      push     eax                            
  0x00214CA8  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00214CAE  80660dfe                and      byte ptr [esi + 0xd], 0xfe     
                                        ; XREF: 0x00214CA2 (cond_jump)
  0x00214CB2  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00214CB5  a806                    test     al, 6                          
  0x00214CB7  7407                    je       0x214cc0                       
                                        ; XREF: 0x00214CDA (jump)
  0x00214CB9  689d0000c0              push     0xc000009d                     
  0x00214CBE  eb61                    jmp      0x214d21                       
                                        ; XREF: 0x00214CB7 (cond_jump)
  0x00214CC0  8b542410                mov      edx, dword ptr [esp + 0x10]    
  0x00214CC4  33c9                    xor      ecx, ecx                       
  0x00214CC6  394a04                  cmp      dword ptr [edx + 4], ecx       
  0x00214CC9  7d11                    jge      0x214cdc                       
  0x00214CCB  25ff8fffff              and      eax, 0xffff8fff                
  0x00214CD0  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214CD3  8b0b                    mov      ecx, dword ptr [ebx]           
  0x00214CD5  e8caecffff              call     0x2139a4                       ; -> sub_002139A4
  0x00214CDA  ebdd                    jmp      0x214cb9                       
                                        ; XREF: 0x00214CC9 (cond_jump)
  0x00214CDC  81ff00100000            cmp      edi, 0x1000                    
  0x00214CE2  0f84c3000000            je       0x214dab                       
  0x00214CE8  81ff00200000            cmp      edi, 0x2000                    
  0x00214CEE  743a                    je       0x214d2a                       
  0x00214CF0  81ff00400000            cmp      edi, 0x4000                    
  0x00214CF6  0f8580010000            jne      0x214e7c                       
  0x00214CFC  25ffbfffff              and      eax, 0xffffbfff                
  0x00214D01  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214D04  8a4636                  mov      al, byte ptr [esi + 0x36]      
  0x00214D07  8ac8                    mov      cl, al                         
  0x00214D09  fec9                    dec      cl                             
  0x00214D0B  84c0                    test     al, al                         
  0x00214D0D  884e36                  mov      byte ptr [esi + 0x36], cl      
  0x00214D10  740c                    je       0x214d1e                       
  0x00214D12  8bce                    mov      ecx, esi                       
  0x00214D14  e8c0feffff              call     0x214bd9                       ; -> sub_00214BD9
  0x00214D19  e95e010000              jmp      0x214e7c                       
                                        ; XREF: 0x00214D10 (cond_jump)
  0x00214D1E  ff766c                  push     dword ptr [esi + 0x6c]         
                                        ; XREF: 0x00214CBE (jump)
  0x00214D21  56                      push     esi                            
  0x00214D22  ff5630                  call     dword ptr [esi + 0x30]         
  0x00214D25  e952010000              jmp      0x214e7c                       
                                        ; XREF: 0x00214CEE (cond_jump)
  0x00214D2A  80a6d500000000          and      byte ptr [esi + 0xd5], 0       
  0x00214D31  80a6d600000000          and      byte ptr [esi + 0xd6], 0       
  0x00214D38  25ffdfffff              and      eax, 0xffffdfff                
  0x00214D3D  0d00400000              or       eax, 0x4000                    
  0x00214D42  808ee1000000ff          or       byte ptr [esi + 0xe1], 0xff    
  0x00214D49  8dbeb8000000            lea      edi, [esi + 0xb8]              
  0x00214D4F  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214D52  c60730                  mov      byte ptr [edi], 0x30           
  0x00214D55  c686b900000040          mov      byte ptr [esi + 0xb9], 0x40    
  0x00214D5C  c786c0000000824c2100    mov      dword ptr [esi + 0xc0], 0x214c82 
  0x00214D66  89b6c4000000            mov      dword ptr [esi + 0xc4], esi    
  0x00214D6C  898ec8000000            mov      dword ptr [esi + 0xc8], ecx    
  0x00214D72  898ed0000000            mov      dword ptr [esi + 0xd0], ecx    
  0x00214D78  898ecc000000            mov      dword ptr [esi + 0xcc], ecx    
  0x00214D7E  c686d400000001          mov      byte ptr [esi + 0xd4], 1       
  0x00214D85  c686e000000021          mov      byte ptr [esi + 0xe0], 0x21    
  0x00214D8C  66898ee2000000          mov      word ptr [esi + 0xe2], cx      
  0x00214D93  660fb64304              movzx    ax, byte ptr [ebx + 4]         
  0x00214D98  668986e4000000          mov      word ptr [esi + 0xe4], ax      
  0x00214D9F  66898ee6000000          mov      word ptr [esi + 0xe6], cx      
  0x00214DA6  e9a6000000              jmp      0x214e51                       
                                        ; XREF: 0x00214CE2 (cond_jump)
  0x00214DAB  25ffefffff              and      eax, 0xffffefff                
  0x00214DB0  0d00200000              or       eax, 0x2000                    
  0x00214DB5  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214DB8  8b4624                  mov      eax, dword ptr [esi + 0x24]    
  0x00214DBB  8dbeb8000000            lea      edi, [esi + 0xb8]              
  0x00214DC1  c60718                  mov      byte ptr [edi], 0x18           
  0x00214DC4  c686b900000005          mov      byte ptr [esi + 0xb9], 5       
  0x00214DCB  898ec0000000            mov      dword ptr [esi + 0xc0], ecx    
  0x00214DD1  8986c8000000            mov      dword ptr [esi + 0xc8], eax    
  0x00214DD7  c786cc00000004000000    mov      dword ptr [esi + 0xcc], 4      
  0x00214DE1  8b0b                    mov      ecx, dword ptr [ebx]           
  0x00214DE3  57                      push     edi                            
  0x00214DE4  e838f8ffff              call     0x214621                       ; -> sub_00214621
  0x00214DE9  33c0                    xor      eax, eax                       
  0x00214DEB  2086d4000000            and      byte ptr [esi + 0xd4], al      
  0x00214DF1  2086d5000000            and      byte ptr [esi + 0xd5], al      
  0x00214DF7  2086d6000000            and      byte ptr [esi + 0xd6], al      
  0x00214DFD  c60730                  mov      byte ptr [edi], 0x30           
  0x00214E00  c686b900000040          mov      byte ptr [esi + 0xb9], 0x40    
  0x00214E07  c786c0000000824c2100    mov      dword ptr [esi + 0xc0], 0x214c82 
  0x00214E11  89b6c4000000            mov      dword ptr [esi + 0xc4], esi    
  0x00214E17  8986c8000000            mov      dword ptr [esi + 0xc8], eax    
  0x00214E1D  8986d0000000            mov      dword ptr [esi + 0xd0], eax    
  0x00214E23  8986cc000000            mov      dword ptr [esi + 0xcc], eax    
  0x00214E29  c686e000000002          mov      byte ptr [esi + 0xe0], 2       
  0x00214E30  c686e100000001          mov      byte ptr [esi + 0xe1], 1       
  0x00214E37  668986e2000000          mov      word ptr [esi + 0xe2], ax      
  0x00214E3E  660fb64b06              movzx    cx, byte ptr [ebx + 6]         
  0x00214E43  66898ee4000000          mov      word ptr [esi + 0xe4], cx      
  0x00214E4A  668986e6000000          mov      word ptr [esi + 0xe6], ax      
                                        ; XREF: 0x00214DA6 (jump)
  0x00214E51  804e0d03                or       byte ptr [esi + 0xd], 3        
  0x00214E55  8d9698000000            lea      edx, [esi + 0x98]              
  0x00214E5B  52                      push     edx                            
  0x00214E5C  83c9ff                  or       ecx, 0xffffffff                
  0x00214E5F  51                      push     ecx                            
  0x00214E60  b8c0bdf0ff              mov      eax, 0xfff0bdc0                
  0x00214E65  50                      push     eax                            
  0x00214E66  8d4670                  lea      eax, [esi + 0x70]              
  0x00214E69  50                      push     eax                            
  0x00214E6A  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x00214E70  804e0d02                or       byte ptr [esi + 0xd], 2        
  0x00214E74  8b0b                    mov      ecx, dword ptr [ebx]           
  0x00214E76  57                      push     edi                            
  0x00214E77  e8a5f7ffff              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x00214CF6 (cond_jump), 0x00214D19 (jump), 0x00214D25 (jump)
  0x00214E7C  5f                      pop      edi                            
  0x00214E7D  5e                      pop      esi                            
  0x00214E7E  5b                      pop      ebx                            
  0x00214E7F  c20800                  ret      8                              

; ============================================================
; Function: sub_00214E82
; Start: 0x00214E82  End: 0x00214E9F  Size: 29 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00212C7A, sub_00212DC9
; ============================================================
sub_00214E82:
  0x00214E82  53                      push     ebx                            
  0x00214E83  56                      push     esi                            
  0x00214E84  8bf1                    mov      esi, ecx                       
  0x00214E86  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00214E8C  f6460c06                test     byte ptr [esi + 0xc], 6        
  0x00214E90  8ad8                    mov      bl, al                         
  0x00214E92  740b                    je       0x214e9f                       
  0x00214E94  689d0000c0              push     0xc000009d                     
  0x00214E99  56                      push     esi                            
  0x00214E9A  ff5630                  call     dword ptr [esi + 0x30]         
  0x00214E9D  eb07                    jmp      0x214ea6                       
; end of function
                                        ; XREF: 0x00214E92 (cond_jump)
  0x00214E9F  8bce                    mov      ecx, esi                       
  0x00214EA1  e833fdffff              call     0x214bd9                       ; -> sub_00214BD9
                                        ; XREF: 0x00214E9D (jump)
  0x00214EA6  5e                      pop      esi                            
  0x00214EA7  8acb                    mov      cl, bl                         
  0x00214EA9  5b                      pop      ebx                            
  0x00214EAA  ff25fc7a2100            jmp      dword ptr [0x217afc]           

; ============================================================
; Function: sub_00214EB0
; Start: 0x00214EB0  End: 0x00214EC3  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00214EC3
; ============================================================
sub_00214EB0:
  0x00214EB0  a1a8b9c600              mov      eax, dword ptr [0xc6b9a8]      
  0x00214EB5  85c0                    test     eax, eax                       
  0x00214EB7  7409                    je       0x214ec2                       
  0x00214EB9  8b4818                  mov      ecx, dword ptr [eax + 0x18]    
  0x00214EBC  890da8b9c600            mov      dword ptr [0xc6b9a8], ecx      
                                        ; XREF: 0x00214EB7 (cond_jump)
  0x00214EC2  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00214EC3
; Start: 0x00214EC3  End: 0x00214EEA  Size: 39 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00214EB0
; Called by: sub_00215371
; ============================================================
sub_00214EC3:
  0x00214EC3  55                      push     ebp                            
  0x00214EC4  8bec                    mov      ebp, esp                       
  0x00214EC6  51                      push     ecx                            
  0x00214EC7  51                      push     ecx                            
  0x00214EC8  8365fc00                and      dword ptr [ebp - 4], 0         
  0x00214ECC  53                      push     ebx                            
  0x00214ECD  56                      push     esi                            
  0x00214ECE  8bda                    mov      ebx, edx                       
  0x00214ED0  894df8                  mov      dword ptr [ebp - 8], ecx       
  0x00214ED3  e8d8ffffff              call     0x214eb0                       ; -> sub_00214EB0
  0x00214ED8  8bf0                    mov      esi, eax                       
  0x00214EDA  85f6                    test     esi, esi                       
  0x00214EDC  750c                    jne      0x214eea                       
  0x00214EDE  c745fc00010080          mov      dword ptr [ebp - 4], 0x80000100 
  0x00214EE5  e92e010000              jmp      0x215018                       
; end of function
                                        ; XREF: 0x00214EDC (cond_jump)
  0x00214EEA  57                      push     edi                            
  0x00214EEB  33c0                    xor      eax, eax                       
  0x00214EED  6a0c                    push     0xc                            
  0x00214EEF  59                      pop      ecx                            
  0x00214EF0  8bfe                    mov      edi, esi                       
  0x00214EF2  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00214EF4  8bc6                    mov      eax, esi                       
  0x00214EF6  2b05a0b9c600            sub      eax, dword ptr [0xc6b9a0]      
  0x00214EFC  894614                  mov      dword ptr [esi + 0x14], eax    
  0x00214EFF  8a4316                  mov      al, byte ptr [ebx + 0x16]      
  0x00214F02  884611                  mov      byte ptr [esi + 0x11], al      
  0x00214F05  8a4317                  mov      al, byte ptr [ebx + 0x17]      
  0x00214F08  884613                  mov      byte ptr [esi + 0x13], al      
  0x00214F0B  33c0                    xor      eax, eax                       
  0x00214F0D  8a431e                  mov      al, byte ptr [ebx + 0x1e]      
  0x00214F10  50                      push     eax                            
  0x00214F11  33c0                    xor      eax, eax                       
  0x00214F13  8a4611                  mov      al, byte ptr [esi + 0x11]      
  0x00214F16  50                      push     eax                            
  0x00214F17  33c0                    xor      eax, eax                       
  0x00214F19  668b431c                mov      ax, word ptr [ebx + 0x1c]      
  0x00214F1D  50                      push     eax                            
  0x00214F1E  e8c6d9ffff              call     0x2128e9                       ; -> sub_002128E9
  0x00214F23  66894622                mov      word ptr [esi + 0x22], ax      
  0x00214F27  33c0                    xor      eax, eax                       
  0x00214F29  8a4314                  mov      al, byte ptr [ebx + 0x14]      
  0x00214F2C  3306                    xor      eax, dword ptr [esi]           
  0x00214F2E  83e07f                  and      eax, 0x7f                      
  0x00214F31  3106                    xor      dword ptr [esi], eax           
  0x00214F33  0fb64315                movzx    eax, byte ptr [ebx + 0x15]     
  0x00214F37  8b0e                    mov      ecx, dword ptr [esi]           
  0x00214F39  c1e007                  shl      eax, 7                         
  0x00214F3C  33c1                    xor      eax, ecx                       
  0x00214F3E  2580070000              and      eax, 0x780                     
  0x00214F43  33c1                    xor      eax, ecx                       
  0x00214F45  807e1100                cmp      byte ptr [esi + 0x11], 0       
  0x00214F49  8906                    mov      dword ptr [esi], eax           
  0x00214F4B  7509                    jne      0x214f56                       
  0x00214F4D  25ffe7ffff              and      eax, 0xffffe7ff                
  0x00214F52  8906                    mov      dword ptr [esi], eax           
  0x00214F54  eb1a                    jmp      0x214f70                       
                                        ; XREF: 0x00214F4B (cond_jump)
  0x00214F56  f6431580                test     byte ptr [ebx + 0x15], 0x80    
  0x00214F5A  6a00                    push     0                              
  0x00214F5C  59                      pop      ecx                            
  0x00214F5D  0f95c1                  setne    cl                             
  0x00214F60  41                      inc      ecx                            
  0x00214F61  c1e10b                  shl      ecx, 0xb                       
  0x00214F64  33c8                    xor      ecx, eax                       
  0x00214F66  81e100180000            and      ecx, 0x1800                    
  0x00214F6C  33c8                    xor      ecx, eax                       
  0x00214F6E  890e                    mov      dword ptr [esi], ecx           
                                        ; XREF: 0x00214F54 (jump)
  0x00214F70  8b0e                    mov      ecx, dword ptr [esi]           
  0x00214F72  33c0                    xor      eax, eax                       
  0x00214F74  8a431e                  mov      al, byte ptr [ebx + 0x1e]      
  0x00214F77  81e1ff5fffff            and      ecx, 0xffff5fff                
  0x00214F7D  83e001                  and      eax, 1                         
  0x00214F80  83c802                  or       eax, 2                         
  0x00214F83  c1e00d                  shl      eax, 0xd                       
  0x00214F86  0bc1                    or       eax, ecx                       
  0x00214F88  8906                    mov      dword ptr [esi], eax           
  0x00214F8A  0fb74b1c                movzx    ecx, word ptr [ebx + 0x1c]     
  0x00214F8E  c1e110                  shl      ecx, 0x10                      
  0x00214F91  33c8                    xor      ecx, eax                       
  0x00214F93  81e10000ff07            and      ecx, 0x7ff0000                 
  0x00214F99  33c8                    xor      ecx, eax                       
  0x00214F9B  33c0                    xor      eax, eax                       
  0x00214F9D  890e                    mov      dword ptr [esi], ecx           
  0x00214F9F  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00214FA2  894608                  mov      dword ptr [esi + 8], eax       
  0x00214FA5  894604                  mov      dword ptr [esi + 4], eax       
  0x00214FA8  8b5318                  mov      edx, dword ptr [ebx + 0x18]    
  0x00214FAB  3bd0                    cmp      edx, eax                       
  0x00214FAD  7427                    je       0x214fd6                       
  0x00214FAF  8bc1                    mov      eax, ecx                       
  0x00214FB1  33ff                    xor      edi, edi                       
  0x00214FB3  c1e907                  shr      ecx, 7                         
  0x00214FB6  83e10f                  and      ecx, 0xf                       
  0x00214FB9  47                      inc      edi                            
  0x00214FBA  2500180000              and      eax, 0x1800                    
  0x00214FBF  d3e7                    shl      edi, cl                        
  0x00214FC1  3d00100000              cmp      eax, 0x1000                    
  0x00214FC6  7503                    jne      0x214fcb                       
  0x00214FC8  c1e710                  shl      edi, 0x10                      
                                        ; XREF: 0x00214FC6 (cond_jump)
  0x00214FCB  853a                    test     dword ptr [edx], edi           
  0x00214FCD  7407                    je       0x214fd6                       
  0x00214FCF  c7460802000000          mov      dword ptr [esi + 8], 2         
                                        ; XREF: 0x00214FAD (cond_jump), 0x00214FCD (cond_jump)
  0x00214FD6  8a4611                  mov      al, byte ptr [esi + 0x11]      
  0x00214FD9  84c0                    test     al, al                         
  0x00214FDB  5f                      pop      edi                            
  0x00214FDC  7413                    je       0x214ff1                       
  0x00214FDE  3c02                    cmp      al, 2                          
  0x00214FE0  740f                    je       0x214ff1                       
  0x00214FE2  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x00214FE5  8bd6                    mov      edx, esi                       
  0x00214FE7  e876130000              call     0x216362                       ; -> sub_00216362
  0x00214FEC  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00214FEF  eb0a                    jmp      0x214ffb                       
                                        ; XREF: 0x00214FDC (cond_jump), 0x00214FE0 (cond_jump)
  0x00214FF1  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x00214FF4  8bd6                    mov      edx, esi                       
  0x00214FF6  e84d120000              call     0x216248                       ; -> sub_00216248
                                        ; XREF: 0x00214FEF (jump)
  0x00214FFB  837dfc00                cmp      dword ptr [ebp - 4], 0         
  0x00214FFF  7c05                    jl       0x215006                       
  0x00215001  897310                  mov      dword ptr [ebx + 0x10], esi    
  0x00215004  eb12                    jmp      0x215018                       
                                        ; XREF: 0x00214FFF (cond_jump)
  0x00215006  83631000                and      dword ptr [ebx + 0x10], 0      
  0x0021500A  a1a8b9c600              mov      eax, dword ptr [0xc6b9a8]      
  0x0021500F  894618                  mov      dword ptr [esi + 0x18], eax    
  0x00215012  8935a8b9c600            mov      dword ptr [0xc6b9a8], esi      
                                        ; XREF: 0x00214EE5 (jump), 0x00215004 (jump)
  0x00215018  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x0021501B  5e                      pop      esi                            
  0x0021501C  894304                  mov      dword ptr [ebx + 4], eax       
  0x0021501F  5b                      pop      ebx                            
  0x00215020  c9                      leave                                   
  0x00215021  c3                      ret                                     

; ============================================================
; Function: sub_00215022
; Start: 0x00215022  End: 0x00215043  Size: 33 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00215371
; ============================================================
sub_00215022:
  0x00215022  8b4a10                  mov      ecx, dword ptr [edx + 0x10]    
  0x00215025  8b4108                  mov      eax, dword ptr [ecx + 8]       
  0x00215028  83e001                  and      eax, 1                         
  0x0021502B  894214                  mov      dword ptr [edx + 0x14], eax    
  0x0021502E  80792600                cmp      byte ptr [ecx + 0x26], 0       
  0x00215032  7506                    jne      0x21503a                       
  0x00215034  80792700                cmp      byte ptr [ecx + 0x27], 0       
  0x00215038  7406                    je       0x215040                       
                                        ; XREF: 0x00215032 (cond_jump)
  0x0021503A  83c802                  or       eax, 2                         
  0x0021503D  894214                  mov      dword ptr [edx + 0x14], eax    
                                        ; XREF: 0x00215038 (cond_jump)
  0x00215040  33c0                    xor      eax, eax                       
  0x00215042  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00215043
; Start: 0x00215043  End: 0x00215071  Size: 46 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00215371
; ============================================================
sub_00215043:
  0x00215043  8b4210                  mov      eax, dword ptr [edx + 0x10]    
  0x00215046  8b5214                  mov      edx, dword ptr [edx + 0x14]    
  0x00215049  f6c204                  test     dl, 4                          
  0x0021504C  7404                    je       0x215052                       
  0x0021504E  836008fd                and      dword ptr [eax + 8], 0xfffffffd 
                                        ; XREF: 0x0021504C (cond_jump)
  0x00215052  f6c208                  test     dl, 8                          
  0x00215055  7404                    je       0x21505b                       
  0x00215057  83480802                or       dword ptr [eax + 8], 2         
                                        ; XREF: 0x00215055 (cond_jump)
  0x0021505B  f6c201                  test     dl, 1                          
  0x0021505E  750e                    jne      0x21506e                       
  0x00215060  8b4808                  mov      ecx, dword ptr [eax + 8]       
  0x00215063  f6c101                  test     cl, 1                          
  0x00215066  7406                    je       0x21506e                       
  0x00215068  83e1fe                  and      ecx, 0xfffffffe                
  0x0021506B  894808                  mov      dword ptr [eax + 8], ecx       
                                        ; XREF: 0x0021505E (cond_jump), 0x00215066 (cond_jump)
  0x0021506E  33c0                    xor      eax, eax                       
  0x00215070  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00215071
; Start: 0x00215071  End: 0x00215096  Size: 37 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00215071:
  0x00215071  8b811c040000            mov      eax, dword ptr [ecx + 0x41c]   
  0x00215077  56                      push     esi                            
  0x00215078  33f6                    xor      esi, esi                       
  0x0021507A  3bc2                    cmp      eax, edx                       
  0x0021507C  740d                    je       0x21508b                       
                                        ; XREF: 0x00215085 (cond_jump)
  0x0021507E  8bf0                    mov      esi, eax                       
  0x00215080  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x00215083  3bc2                    cmp      eax, edx                       
  0x00215085  75f7                    jne      0x21507e                       
  0x00215087  85f6                    test     esi, esi                       
  0x00215089  750b                    jne      0x215096                       
                                        ; XREF: 0x0021507C (cond_jump)
  0x0021508B  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x0021508E  89811c040000            mov      dword ptr [ecx + 0x41c], eax   
  0x00215094  eb06                    jmp      0x21509c                       
; end of function
                                        ; XREF: 0x00215089 (cond_jump)
  0x00215096  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x00215099  894624                  mov      dword ptr [esi + 0x24], eax    
                                        ; XREF: 0x00215094 (jump)
  0x0021509C  8d8120040000            lea      eax, [ecx + 0x420]             
  0x002150A2  3b10                    cmp      edx, dword ptr [eax]           
  0x002150A4  7502                    jne      0x2150a8                       
  0x002150A6  8930                    mov      dword ptr [eax], esi           
                                        ; XREF: 0x002150A4 (cond_jump)
  0x002150A8  5e                      pop      esi                            
  0x002150A9  c3                      ret                                     

; ============================================================
; Function: sub_002150AA
; Start: 0x002150AA  End: 0x002150CF  Size: 37 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_002150AA:
  0x002150AA  8b8124040000            mov      eax, dword ptr [ecx + 0x424]   
  0x002150B0  56                      push     esi                            
  0x002150B1  33f6                    xor      esi, esi                       
  0x002150B3  3bc2                    cmp      eax, edx                       
  0x002150B5  740d                    je       0x2150c4                       
                                        ; XREF: 0x002150BE (cond_jump)
  0x002150B7  8bf0                    mov      esi, eax                       
  0x002150B9  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x002150BC  3bc2                    cmp      eax, edx                       
  0x002150BE  75f7                    jne      0x2150b7                       
  0x002150C0  85f6                    test     esi, esi                       
  0x002150C2  750b                    jne      0x2150cf                       
                                        ; XREF: 0x002150B5 (cond_jump)
  0x002150C4  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x002150C7  898124040000            mov      dword ptr [ecx + 0x424], eax   
  0x002150CD  eb06                    jmp      0x2150d5                       
; end of function
                                        ; XREF: 0x002150C2 (cond_jump)
  0x002150CF  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x002150D2  894624                  mov      dword ptr [esi + 0x24], eax    
                                        ; XREF: 0x002150CD (jump)
  0x002150D5  8d8128040000            lea      eax, [ecx + 0x428]             
  0x002150DB  3b10                    cmp      edx, dword ptr [eax]           
  0x002150DD  7502                    jne      0x2150e1                       
  0x002150DF  8930                    mov      dword ptr [eax], esi           
                                        ; XREF: 0x002150DD (cond_jump)
  0x002150E1  5e                      pop      esi                            
  0x002150E2  c3                      ret                                     

; ============================================================
; Function: sub_002150E3
; Start: 0x002150E3  End: 0x00215102  Size: 31 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_002150E3:
  0x002150E3  8b4128                  mov      eax, dword ptr [ecx + 0x28]    
  0x002150E6  56                      push     esi                            
  0x002150E7  33f6                    xor      esi, esi                       
  0x002150E9  3bc2                    cmp      eax, edx                       
  0x002150EB  740d                    je       0x2150fa                       
                                        ; XREF: 0x002150F4 (cond_jump)
  0x002150ED  8bf0                    mov      esi, eax                       
  0x002150EF  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x002150F2  3bc2                    cmp      eax, edx                       
  0x002150F4  75f7                    jne      0x2150ed                       
  0x002150F6  85f6                    test     esi, esi                       
  0x002150F8  7508                    jne      0x215102                       
                                        ; XREF: 0x002150EB (cond_jump)
  0x002150FA  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x002150FD  894128                  mov      dword ptr [ecx + 0x28], eax    
  0x00215100  eb06                    jmp      0x215108                       
; end of function
                                        ; XREF: 0x002150F8 (cond_jump)
  0x00215102  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x00215105  894624                  mov      dword ptr [esi + 0x24], eax    
                                        ; XREF: 0x00215100 (jump)
  0x00215108  3b512c                  cmp      edx, dword ptr [ecx + 0x2c]    
  0x0021510B  7503                    jne      0x215110                       
  0x0021510D  89712c                  mov      dword ptr [ecx + 0x2c], esi    
                                        ; XREF: 0x0021510B (cond_jump)
  0x00215110  5e                      pop      esi                            
  0x00215111  c3                      ret                                     

; ============================================================
; Function: sub_00215112
; Start: 0x00215112  End: 0x00215188  Size: 118 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002128D5
; Called by: sub_002152B7, sub_0021531E
; ============================================================
sub_00215112:
  0x00215112  51                      push     ecx                            
  0x00215113  57                      push     edi                            
  0x00215114  8bfa                    mov      edi, edx                       
  0x00215116  807f2600                cmp      byte ptr [edi + 0x26], 0       
  0x0021511A  7469                    je       0x215185                       
  0x0021511C  0fb64711                movzx    eax, byte ptr [edi + 0x11]     
  0x00215120  83e800                  sub      eax, 0                         
  0x00215123  53                      push     ebx                            
  0x00215124  55                      push     ebp                            
  0x00215125  741d                    je       0x215144                       
  0x00215127  48                      dec      eax                            
  0x00215128  48                      dec      eax                            
  0x00215129  740b                    je       0x215136                       
  0x0021512B  48                      dec      eax                            
  0x0021512C  7555                    jne      0x215183                       
  0x0021512E  8d5f28                  lea      ebx, [edi + 0x28]              
  0x00215131  8d6f2c                  lea      ebp, [edi + 0x2c]              
  0x00215134  eb1a                    jmp      0x215150                       
                                        ; XREF: 0x00215129 (cond_jump)
  0x00215136  8d9924040000            lea      ebx, [ecx + 0x424]             
  0x0021513C  8da928040000            lea      ebp, [ecx + 0x428]             
  0x00215142  eb0c                    jmp      0x215150                       
                                        ; XREF: 0x00215125 (cond_jump)
  0x00215144  8d991c040000            lea      ebx, [ecx + 0x41c]             
  0x0021514A  8da920040000            lea      ebp, [ecx + 0x420]             
                                        ; XREF: 0x00215134 (jump), 0x00215142 (jump)
  0x00215150  56                      push     esi                            
                                        ; XREF: 0x00215179 (cond_jump)
  0x00215151  8b33                    mov      esi, dword ptr [ebx]           
  0x00215153  397e10                  cmp      dword ptr [esi + 0x10], edi    
  0x00215156  7517                    jne      0x21516f                       
  0x00215158  8b4624                  mov      eax, dword ptr [esi + 0x24]    
  0x0021515B  8903                    mov      dword ptr [ebx], eax           
  0x0021515D  c746040f0000c0          mov      dword ptr [esi + 4], 0xc000000f 
  0x00215164  fe4f26                  dec      byte ptr [edi + 0x26]          
  0x00215167  56                      push     esi                            
  0x00215168  e868d7ffff              call     0x2128d5                       ; -> sub_002128D5
  0x0021516D  eb07                    jmp      0x215176                       
                                        ; XREF: 0x00215156 (cond_jump)
  0x0021516F  89742410                mov      dword ptr [esp + 0x10], esi    
  0x00215173  8d5e24                  lea      ebx, [esi + 0x24]              
                                        ; XREF: 0x0021516D (jump)
  0x00215176  397500                  cmp      dword ptr [ebp], esi           
  0x00215179  75d6                    jne      0x215151                       
  0x0021517B  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x0021517F  894500                  mov      dword ptr [ebp], eax           
  0x00215182  5e                      pop      esi                            
                                        ; XREF: 0x0021512C (cond_jump)
  0x00215183  5d                      pop      ebp                            
  0x00215184  5b                      pop      ebx                            
                                        ; XREF: 0x0021511A (cond_jump)
  0x00215185  5f                      pop      edi                            
  0x00215186  59                      pop      ecx                            
  0x00215187  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00215188
; Start: 0x00215188  End: 0x002151C5  Size: 61 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021674D
; Called by: sub_0021531E, sub_00216F45
; ============================================================
sub_00215188:
  0x00215188  56                      push     esi                            
  0x00215189  8bf2                    mov      esi, edx                       
  0x0021518B  fe4620                  inc      byte ptr [esi + 0x20]          
  0x0021518E  f6461020                test     byte ptr [esi + 0x10], 0x20    
  0x00215192  57                      push     edi                            
  0x00215193  8bf9                    mov      edi, ecx                       
  0x00215195  752b                    jne      0x2151c2                       
  0x00215197  804e0140                or       byte ptr [esi + 1], 0x40       
  0x0021519B  e8ad150000              call     0x21674d                       ; -> sub_0021674D
  0x002151A0  40                      inc      eax                            
  0x002151A1  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x002151A4  83bf3804000000          cmp      dword ptr [edi + 0x438], 0     
  0x002151AB  7404                    je       0x2151b1                       
  0x002151AD  804e1040                or       byte ptr [esi + 0x10], 0x40    
                                        ; XREF: 0x002151AB (cond_jump)
  0x002151B1  8b0f                    mov      ecx, dword ptr [edi]           
  0x002151B3  6a04                    push     4                              
  0x002151B5  58                      pop      eax                            
  0x002151B6  89410c                  mov      dword ptr [ecx + 0xc], eax     
  0x002151B9  8b0f                    mov      ecx, dword ptr [edi]           
  0x002151BB  894110                  mov      dword ptr [ecx + 0x10], eax    
  0x002151BE  804e1020                or       byte ptr [esi + 0x10], 0x20    
                                        ; XREF: 0x00215195 (cond_jump)
  0x002151C2  5f                      pop      edi                            
  0x002151C3  5e                      pop      esi                            
  0x002151C4  c3                      ret                                     
; end of function
  0x002151C5  56                      push     esi                            
  0x002151C6  8b742408                mov      esi, dword ptr [esp + 8]       
  0x002151CA  8b469c                  mov      eax, dword ptr [esi - 0x64]    
  0x002151CD  81c640fbffff            add      esi, 0xfffffb40                
  0x002151D3  6bc070                  imul     eax, eax, 0x70                 
  0x002151D6  8a88ecb9c600            mov      cl, byte ptr [eax + 0xc6b9ec]  
  0x002151DC  ff15707b2100            call     dword ptr [0x217b70]           ; -> xbox_KfRaiseIrql
  0x002151E2  8b0e                    mov      ecx, dword ptr [esi]           
  0x002151E4  c7411433000080          mov      dword ptr [ecx + 0x14], 0x80000033 
  0x002151EB  8b0e                    mov      ecx, dword ptr [esi]           
  0x002151ED  c7410402000000          mov      dword ptr [ecx + 4], 2         
  0x002151F4  8ac8                    mov      cl, al                         
  0x002151F6  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002151FC  5e                      pop      esi                            
  0x002151FD  c20400                  ret      4                              

; ============================================================
; Function: sub_00215200
; Start: 0x00215200  End: 0x00215215  Size: 21 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00214267
; ============================================================
sub_00215200:
  0x00215200  56                      push     esi                            
  0x00215201  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00215205  f6462201                test     byte ptr [esi + 0x22], 1       
  0x00215209  740a                    je       0x215215                       
  0x0021520B  b800000240              mov      eax, 0x40020000                
  0x00215210  e99e000000              jmp      0x2152b3                       
; end of function
                                        ; XREF: 0x00215209 (cond_jump)
  0x00215215  53                      push     ebx                            
  0x00215216  57                      push     edi                            
  0x00215217  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x0021521D  8b7e10                  mov      edi, dword ptr [esi + 0x10]    
  0x00215220  f6471010                test     byte ptr [edi + 0x10], 0x10    
  0x00215224  8ad8                    mov      bl, al                         
  0x00215226  757a                    jne      0x2152a2                       
  0x00215228  668b4622                mov      ax, word ptr [esi + 0x22]      
  0x0021522C  a802                    test     al, 2                          
  0x0021522E  7452                    je       0x215282                       
  0x00215230  0fb64711                movzx    eax, byte ptr [edi + 0x11]     
  0x00215234  83e800                  sub      eax, 0                         
  0x00215237  7426                    je       0x21525f                       
  0x00215239  48                      dec      eax                            
  0x0021523A  48                      dec      eax                            
  0x0021523B  7415                    je       0x215252                       
  0x0021523D  48                      dec      eax                            
  0x0021523E  7407                    je       0x215247                       
  0x00215240  be00060080              mov      esi, 0x80000600                
  0x00215245  eb60                    jmp      0x2152a7                       
                                        ; XREF: 0x0021523E (cond_jump)
  0x00215247  8bd6                    mov      edx, esi                       
  0x00215249  8bcf                    mov      ecx, edi                       
  0x0021524B  e893feffff              call     0x2150e3                       ; -> sub_002150E3
  0x00215250  eb18                    jmp      0x21526a                       
                                        ; XREF: 0x0021523B (cond_jump)
  0x00215252  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x00215256  8bd6                    mov      edx, esi                       
  0x00215258  e84dfeffff              call     0x2150aa                       ; -> sub_002150AA
  0x0021525D  eb0b                    jmp      0x21526a                       
                                        ; XREF: 0x00215237 (cond_jump)
  0x0021525F  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x00215263  8bd6                    mov      edx, esi                       
  0x00215265  e807feffff              call     0x215071                       ; -> sub_00215071
                                        ; XREF: 0x00215250 (jump), 0x0021525D (jump)
  0x0021526A  fe4f26                  dec      byte ptr [edi + 0x26]          
  0x0021526D  804e2201                or       byte ptr [esi + 0x22], 1       
  0x00215271  56                      push     esi                            
  0x00215272  c746040f0000c0          mov      dword ptr [esi + 4], 0xc000000f 
  0x00215279  e857d6ffff              call     0x2128d5                       ; -> sub_002128D5
  0x0021527E  33f6                    xor      esi, esi                       
  0x00215280  eb25                    jmp      0x2152a7                       
                                        ; XREF: 0x0021522E (cond_jump)
  0x00215282  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x00215286  660d0100                or       ax, 1                          
  0x0021528A  66894622                mov      word ptr [esi + 0x22], ax      
  0x0021528E  8d812c040000            lea      eax, [ecx + 0x42c]             
  0x00215294  8b10                    mov      edx, dword ptr [eax]           
  0x00215296  895624                  mov      dword ptr [esi + 0x24], edx    
  0x00215299  8bd7                    mov      edx, edi                       
  0x0021529B  8930                    mov      dword ptr [eax], esi           
  0x0021529D  e8e6feffff              call     0x215188                       ; -> sub_00215188
                                        ; XREF: 0x00215226 (cond_jump)
  0x002152A2  be00000240              mov      esi, 0x40020000                
                                        ; XREF: 0x00215245 (jump), 0x00215280 (jump)
  0x002152A7  8acb                    mov      cl, bl                         
  0x002152A9  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002152AF  5f                      pop      edi                            
  0x002152B0  8bc6                    mov      eax, esi                       
  0x002152B2  5b                      pop      ebx                            
                                        ; XREF: 0x00215210 (jump)
  0x002152B3  5e                      pop      esi                            
  0x002152B4  c20800                  ret      8                              

; ============================================================
; Function: sub_002152B7
; Start: 0x002152B7  End: 0x002152ED  Size: 54 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00215112, sub_0021653E
; Called by: sub_00215371
; ============================================================
sub_002152B7:
  0x002152B7  53                      push     ebx                            
  0x002152B8  55                      push     ebp                            
  0x002152B9  56                      push     esi                            
  0x002152BA  57                      push     edi                            
  0x002152BB  8bfa                    mov      edi, edx                       
  0x002152BD  8b7710                  mov      esi, dword ptr [edi + 0x10]    
  0x002152C0  8be9                    mov      ebp, ecx                       
  0x002152C2  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x002152C8  804e1010                or       byte ptr [esi + 0x10], 0x10    
  0x002152CC  8bd6                    mov      edx, esi                       
  0x002152CE  8bcd                    mov      ecx, ebp                       
  0x002152D0  8ad8                    mov      bl, al                         
  0x002152D2  e83bfeffff              call     0x215112                       ; -> sub_00215112
  0x002152D7  8a4611                  mov      al, byte ptr [esi + 0x11]      
  0x002152DA  84c0                    test     al, al                         
  0x002152DC  740f                    je       0x2152ed                       
  0x002152DE  3c02                    cmp      al, 2                          
  0x002152E0  740b                    je       0x2152ed                       
  0x002152E2  8bd6                    mov      edx, esi                       
  0x002152E4  8bcd                    mov      ecx, ebp                       
  0x002152E6  e853120000              call     0x21653e                       ; -> sub_0021653E
  0x002152EB  eb09                    jmp      0x2152f6                       
; end of function
                                        ; XREF: 0x002152DC (cond_jump), 0x002152E0 (cond_jump)
  0x002152ED  8bd6                    mov      edx, esi                       
  0x002152EF  8bcd                    mov      ecx, ebp                       
  0x002152F1  e88f0f0000              call     0x216285                       ; -> sub_00216285
                                        ; XREF: 0x002152EB (jump)
  0x002152F6  8bd6                    mov      edx, esi                       
  0x002152F8  8bcd                    mov      ecx, ebp                       
  0x002152FA  e889feffff              call     0x215188                       ; -> sub_00215188
  0x002152FF  8d8534040000            lea      eax, [ebp + 0x434]             
  0x00215305  8b08                    mov      ecx, dword ptr [eax]           
  0x00215307  894f14                  mov      dword ptr [edi + 0x14], ecx    
  0x0021530A  8acb                    mov      cl, bl                         
  0x0021530C  8938                    mov      dword ptr [eax], edi           
  0x0021530E  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00215314  5f                      pop      edi                            
  0x00215315  5e                      pop      esi                            
  0x00215316  5d                      pop      ebp                            
  0x00215317  b800000040              mov      eax, 0x40000000                
  0x0021531C  5b                      pop      ebx                            
  0x0021531D  c3                      ret                                     

; ============================================================
; Function: sub_0021531E
; Start: 0x0021531E  End: 0x00215371  Size: 83 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00215112, sub_00215188
; Called by: sub_00215371
; ============================================================
sub_0021531E:
  0x0021531E  51                      push     ecx                            
  0x0021531F  53                      push     ebx                            
  0x00215320  55                      push     ebp                            
  0x00215321  56                      push     esi                            
  0x00215322  8bf2                    mov      esi, edx                       
  0x00215324  57                      push     edi                            
  0x00215325  8b7e10                  mov      edi, dword ptr [esi + 0x10]    
  0x00215328  8be9                    mov      ebp, ecx                       
  0x0021532A  33db                    xor      ebx, ebx                       
  0x0021532C  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00215332  8bd7                    mov      edx, edi                       
  0x00215334  8bcd                    mov      ecx, ebp                       
  0x00215336  88442413                mov      byte ptr [esp + 0x13], al      
  0x0021533A  e8d3fdffff              call     0x215112                       ; -> sub_00215112
  0x0021533F  385f27                  cmp      byte ptr [edi + 0x27], bl      
  0x00215342  741b                    je       0x21535f                       
  0x00215344  8d8530040000            lea      eax, [ebp + 0x430]             
  0x0021534A  8b08                    mov      ecx, dword ptr [eax]           
  0x0021534C  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x0021534F  8bd7                    mov      edx, edi                       
  0x00215351  8bcd                    mov      ecx, ebp                       
  0x00215353  8930                    mov      dword ptr [eax], esi           
  0x00215355  e82efeffff              call     0x215188                       ; -> sub_00215188
  0x0021535A  bb00000040              mov      ebx, 0x40000000                
                                        ; XREF: 0x00215342 (cond_jump)
  0x0021535F  8a4c2413                mov      cl, byte ptr [esp + 0x13]      
  0x00215363  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00215369  5f                      pop      edi                            
  0x0021536A  5e                      pop      esi                            
  0x0021536B  5d                      pop      ebp                            
  0x0021536C  8bc3                    mov      eax, ebx                       
  0x0021536E  5b                      pop      ebx                            
  0x0021536F  59                      pop      ecx                            
  0x00215370  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00215371
; Start: 0x00215371  End: 0x0021545E  Size: 237 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00214EC3, sub_00215022, sub_00215043, sub_002152B7, sub_0021531E, sub_0021674D, sub_00216D98, sub_00216F45, sub_00217012, sub_00217179 ... (+1 more)
; Called by: sub_00213B1D, sub_00214479, sub_00214621
; ============================================================
sub_00215371:
  0x00215371  55                      push     ebp                            
  0x00215372  8bec                    mov      ebp, esp                       
  0x00215374  56                      push     esi                            
  0x00215375  57                      push     edi                            
  0x00215376  8b7d0c                  mov      edi, dword ptr [ebp + 0xc]     
  0x00215379  0fb64701                movzx    eax, byte ptr [edi + 1]        
  0x0021537D  83f80c                  cmp      eax, 0xc                       
  0x00215380  0f8f83000000            jg       0x215409                       
  0x00215386  7475                    je       0x2153fd                       
  0x00215388  6a02                    push     2                              
  0x0021538A  59                      pop      ecx                            
  0x0021538B  2bc1                    sub      eax, ecx                       
  0x0021538D  7462                    je       0x2153f1                       
  0x0021538F  2bc1                    sub      eax, ecx                       
  0x00215391  7452                    je       0x2153e5                       
  0x00215393  48                      dec      eax                            
  0x00215394  7440                    je       0x2153d6                       
  0x00215396  2bc1                    sub      eax, ecx                       
  0x00215398  742a                    je       0x2153c4                       
  0x0021539A  2bc1                    sub      eax, ecx                       
  0x0021539C  7417                    je       0x2153b5                       
  0x0021539E  2bc1                    sub      eax, ecx                       
  0x002153A0  0f85b1000000            jne      0x215457                       
  0x002153A6  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002153A9  8bd7                    mov      edx, edi                       
  0x002153AB  e8621c0000              call     0x217012                       ; -> sub_00217012
  0x002153B0  e9b3000000              jmp      0x215468                       
                                        ; XREF: 0x0021539C (cond_jump)
  0x002153B5  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002153B8  8bd7                    mov      edx, edi                       
  0x002153BA  e8d9190000              call     0x216d98                       ; -> sub_00216D98
  0x002153BF  e9a4000000              jmp      0x215468                       
                                        ; XREF: 0x00215398 (cond_jump)
  0x002153C4  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002153C7  e881130000              call     0x21674d                       ; -> sub_0021674D
  0x002153CC  894714                  mov      dword ptr [edi + 0x14], eax    
  0x002153CF  33f6                    xor      esi, esi                       
  0x002153D1  e994000000              jmp      0x21546a                       
                                        ; XREF: 0x00215394 (cond_jump)
  0x002153D6  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002153D9  8bd7                    mov      edx, edi                       
  0x002153DB  e863fcffff              call     0x215043                       ; -> sub_00215043
  0x002153E0  e983000000              jmp      0x215468                       
                                        ; XREF: 0x00215391 (cond_jump)
  0x002153E5  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002153E8  8bd7                    mov      edx, edi                       
  0x002153EA  e833fcffff              call     0x215022                       ; -> sub_00215022
  0x002153EF  eb77                    jmp      0x215468                       
                                        ; XREF: 0x0021538D (cond_jump)
  0x002153F1  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002153F4  8bd7                    mov      edx, edi                       
  0x002153F6  e8c8faffff              call     0x214ec3                       ; -> sub_00214EC3
  0x002153FB  eb6b                    jmp      0x215468                       
                                        ; XREF: 0x00215386 (cond_jump)
  0x002153FD  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00215400  8bd7                    mov      edx, edi                       
  0x00215402  e8721d0000              call     0x217179                       ; -> sub_00217179
  0x00215407  eb5f                    jmp      0x215468                       
                                        ; XREF: 0x00215380 (cond_jump)
  0x00215409  83f80d                  cmp      eax, 0xd                       
  0x0021540C  7450                    je       0x21545e                       
  0x0021540E  83f83f                  cmp      eax, 0x3f                      
  0x00215411  7e44                    jle      0x215457                       
  0x00215413  83f841                  cmp      eax, 0x41                      
  0x00215416  7e33                    jle      0x21544b                       
  0x00215418  83f843                  cmp      eax, 0x43                      
  0x0021541B  7422                    je       0x21543f                       
  0x0021541D  83f846                  cmp      eax, 0x46                      
  0x00215420  7411                    je       0x215433                       
  0x00215422  83f84a                  cmp      eax, 0x4a                      
  0x00215425  7530                    jne      0x215457                       
  0x00215427  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x0021542A  8bd7                    mov      edx, edi                       
  0x0021542C  e8141b0000              call     0x216f45                       ; -> sub_00216F45
  0x00215431  eb35                    jmp      0x215468                       
                                        ; XREF: 0x00215420 (cond_jump)
  0x00215433  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00215436  8bd7                    mov      edx, edi                       
  0x00215438  e8e1feffff              call     0x21531e                       ; -> sub_0021531E
  0x0021543D  eb29                    jmp      0x215468                       
                                        ; XREF: 0x0021541B (cond_jump)
  0x0021543F  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00215442  8bd7                    mov      edx, edi                       
  0x00215444  e86efeffff              call     0x2152b7                       ; -> sub_002152B7
  0x00215449  eb1d                    jmp      0x215468                       
                                        ; XREF: 0x00215416 (cond_jump)
  0x0021544B  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x0021544E  8bd7                    mov      edx, edi                       
  0x00215450  e8ee240000              call     0x217943                       ; -> sub_00217943
  0x00215455  eb11                    jmp      0x215468                       
                                        ; XREF: 0x002153A0 (cond_jump), 0x00215411 (cond_jump), 0x00215425 (cond_jump)
  0x00215457  be00020080              mov      esi, 0x80000200                
  0x0021545C  eb0c                    jmp      0x21546a                       
; end of function
                                        ; XREF: 0x0021540C (cond_jump)
  0x0021545E  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00215461  8bd7                    mov      edx, edi                       
  0x00215463  e8301e0000              call     0x217298                       ; -> sub_00217298
                                        ; XREF: 0x002153B0 (jump), 0x002153BF (jump), 0x002153E0 (jump), 0x002153EF (jump), 0x002153FB (jump), ... (+5 more)
  0x00215468  8bf0                    mov      esi, eax                       
                                        ; XREF: 0x002153D1 (jump), 0x0021545C (jump)
  0x0021546A  8bc6                    mov      eax, esi                       
  0x0021546C  25000000c0              and      eax, 0xc0000000                
  0x00215471  3d00000040              cmp      eax, 0x40000000                
  0x00215476  7409                    je       0x215481                       
  0x00215478  57                      push     edi                            
  0x00215479  897704                  mov      dword ptr [edi + 4], esi       
  0x0021547C  e854d4ffff              call     0x2128d5                       ; -> sub_002128D5
                                        ; XREF: 0x00215476 (cond_jump)
  0x00215481  5f                      pop      edi                            
  0x00215482  8bc6                    mov      eax, esi                       
  0x00215484  5e                      pop      esi                            
  0x00215485  5d                      pop      ebp                            
  0x00215486  c20800                  ret      8                              
  0x00215489  56                      push     esi                            
  0x0021548A  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x0021548E  8bce                    mov      ecx, esi                       
  0x00215490  e8e5edffff              call     0x21427a                       ; -> sub_0021427A
  0x00215495  8b0d14a34000            mov      ecx, dword ptr [0x40a314]      
  0x0021549B  83e900                  sub      ecx, 0                         
  0x0021549E  742e                    je       0x2154ce                       
  0x002154A0  49                      dec      ecx                            
  0x002154A1  7417                    je       0x2154ba                       
  0x002154A3  49                      dec      ecx                            
  0x002154A4  7533                    jne      0x2154d9                       
  0x002154A6  ff35c8a24000            push     dword ptr [0x40a2c8]           
  0x002154AC  8bce                    mov      ecx, esi                       
  0x002154AE  6800060080              push     0x80000600                     
  0x002154B3  e8c5e5ffff              call     0x213a7d                       ; -> sub_00213A7D
  0x002154B8  eb1f                    jmp      0x2154d9                       
                                        ; XREF: 0x002154A1 (cond_jump)
  0x002154BA  ff35c8a24000            push     dword ptr [0x40a2c8]           
  0x002154C0  8bce                    mov      ecx, esi                       
  0x002154C2  6800060080              push     0x80000600                     
  0x002154C7  e8f3e2ffff              call     0x2137bf                       ; -> sub_002137BF
  0x002154CC  eb0b                    jmp      0x2154d9                       
                                        ; XREF: 0x0021549E (cond_jump)
  0x002154CE  83c008                  add      eax, 8                         
  0x002154D1  50                      push     eax                            
  0x002154D2  8bce                    mov      ecx, esi                       
  0x002154D4  e88eedffff              call     0x214267                       ; -> sub_00214267
                                        ; XREF: 0x002154A4 (cond_jump), 0x002154B8 (jump), 0x002154CC (jump)
  0x002154D9  5e                      pop      esi                            
  0x002154DA  c21000                  ret      0x10                           

; ============================================================
; Function: sub_002154DD
; Start: 0x002154DD  End: 0x0021550D  Size: 48 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00215769
; ============================================================
sub_002154DD:
  0x002154DD  8b542404                mov      edx, dword ptr [esp + 4]       
  0x002154E1  83c9ff                  or       ecx, 0xffffffff                
  0x002154E4  85d2                    test     edx, edx                       
  0x002154E6  b8800f05fd              mov      eax, 0xfd050f80                
  0x002154EB  7405                    je       0x2154f2                       
  0x002154ED  b8c0b4b3ff              mov      eax, 0xffb3b4c0                
                                        ; XREF: 0x002154EB (cond_jump)
  0x002154F2  68f8a24000              push     0x40a2f8                       
  0x002154F7  51                      push     ecx                            
  0x002154F8  50                      push     eax                            
  0x002154F9  68d0a24000              push     0x40a2d0                       
  0x002154FE  891514a34000            mov      dword ptr [0x40a314], edx      
  0x00215504  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x0021550A  c20400                  ret      4                              
; end of function
  0x0021550D  56                      push     esi                            
  0x0021550E  57                      push     edi                            
  0x0021550F  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00215513  8bcf                    mov      ecx, edi                       
  0x00215515  e860edffff              call     0x21427a                       ; -> sub_0021427A
  0x0021551A  6a00                    push     0                              
  0x0021551C  8bcf                    mov      ecx, edi                       
  0x0021551E  8bf0                    mov      esi, eax                       
  0x00215520  e859edffff              call     0x21427e                       ; -> sub_0021427E
  0x00215525  8026fe                  and      byte ptr [esi], 0xfe           
  0x00215528  66ff0d1aa34000          dec      word ptr [0x40a31a]            
  0x0021552F  f60602                  test     byte ptr [esi], 2              
  0x00215532  8bcf                    mov      ecx, edi                       
  0x00215534  7407                    je       0x21553d                       
  0x00215536  e889e2ffff              call     0x2137c4                       ; -> sub_002137C4
  0x0021553B  eb0a                    jmp      0x215547                       
                                        ; XREF: 0x00215534 (cond_jump)
  0x0021553D  6800010080              push     0x80000100                     
  0x00215542  e83be6ffff              call     0x213b82                       ; -> sub_00213B82
                                        ; XREF: 0x0021553B (jump)
  0x00215547  5f                      pop      edi                            
  0x00215548  5e                      pop      esi                            
  0x00215549  c20800                  ret      8                              
  0x0021554C  56                      push     esi                            
  0x0021554D  8b742408                mov      esi, dword ptr [esp + 8]       
  0x00215551  837e0400                cmp      dword ptr [esi + 4], 0         
  0x00215555  7d1d                    jge      0x215574                       
  0x00215557  68d0a24000              push     0x40a2d0                       
  0x0021555C  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00215562  ff35c8a24000            push     dword ptr [0x40a2c8]           
  0x00215568  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x0021556C  ff7604                  push     dword ptr [esi + 4]            
  0x0021556F  e84be2ffff              call     0x2137bf                       ; -> sub_002137BF
                                        ; XREF: 0x00215555 (cond_jump)
  0x00215574  5e                      pop      esi                            
  0x00215575  c20800                  ret      8                              
  0x00215578  68d0a24000              push     0x40a2d0                       
  0x0021557D  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00215583  ff35c8a24000            push     dword ptr [0x40a2c8]           
  0x00215589  8b442408                mov      eax, dword ptr [esp + 8]       
  0x0021558D  ff7004                  push     dword ptr [eax + 4]            
  0x00215590  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x00215594  e8e4e4ffff              call     0x213a7d                       ; -> sub_00213A7D
  0x00215599  c20800                  ret      8                              

; ============================================================
; Function: sub_0021559C
; Start: 0x0021559C  End: 0x002155F9  Size: 93 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021427A, sub_00214621
; ============================================================
sub_0021559C:
  0x0021559C  56                      push     esi                            
  0x0021559D  8b742408                mov      esi, dword ptr [esp + 8]       
  0x002155A1  57                      push     edi                            
  0x002155A2  8bce                    mov      ecx, esi                       
  0x002155A4  e8d1ecffff              call     0x21427a                       ; -> sub_0021427A
  0x002155A9  660fb67803              movzx    di, byte ptr [eax + 3]         
  0x002155AE  33d2                    xor      edx, edx                       
  0x002155B0  8d4808                  lea      ecx, [eax + 8]                 
  0x002155B3  c60130                  mov      byte ptr [ecx], 0x30           
  0x002155B6  51                      push     ecx                            
  0x002155B7  8bce                    mov      ecx, esi                       
  0x002155B9  c6400940                mov      byte ptr [eax + 9], 0x40       
  0x002155BD  c74010e45c2100          mov      dword ptr [eax + 0x10], 0x215ce4 
  0x002155C4  897014                  mov      dword ptr [eax + 0x14], esi    
  0x002155C7  895018                  mov      dword ptr [eax + 0x18], edx    
  0x002155CA  895020                  mov      dword ptr [eax + 0x20], edx    
  0x002155CD  89501c                  mov      dword ptr [eax + 0x1c], edx    
  0x002155D0  885024                  mov      byte ptr [eax + 0x24], dl      
  0x002155D3  885025                  mov      byte ptr [eax + 0x25], dl      
  0x002155D6  885026                  mov      byte ptr [eax + 0x26], dl      
  0x002155D9  c6403023                mov      byte ptr [eax + 0x30], 0x23    
  0x002155DD  c6403101                mov      byte ptr [eax + 0x31], 1       
  0x002155E1  66c740320100            mov      word ptr [eax + 0x32], 1       
  0x002155E7  66897834                mov      word ptr [eax + 0x34], di      
  0x002155EB  66895036                mov      word ptr [eax + 0x36], dx      
  0x002155EF  e82df0ffff              call     0x214621                       ; -> sub_00214621
  0x002155F4  5f                      pop      edi                            
  0x002155F5  5e                      pop      esi                            
  0x002155F6  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002155F9
; Start: 0x002155F9  End: 0x00215670  Size: 119 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_002137BF, sub_0021427A
; ============================================================
sub_002155F9:
  0x002155F9  55                      push     ebp                            
  0x002155FA  8bec                    mov      ebp, esp                       
  0x002155FC  53                      push     ebx                            
  0x002155FD  56                      push     esi                            
  0x002155FE  57                      push     edi                            
  0x002155FF  8b7d08                  mov      edi, dword ptr [ebp + 8]       
  0x00215602  8bcf                    mov      ecx, edi                       
  0x00215604  e871ecffff              call     0x21427a                       ; -> sub_0021427A
  0x00215609  8bf0                    mov      esi, eax                       
  0x0021560B  668b463a                mov      ax, word ptr [esi + 0x3a]      
  0x0021560F  33db                    xor      ebx, ebx                       
  0x00215611  a810                    test     al, 0x10                       
  0x00215613  745b                    je       0x215670                       
  0x00215615  833d14a3400001          cmp      dword ptr [0x40a314], 1        
  0x0021561C  754a                    jne      0x215668                       
  0x0021561E  391dc8a24000            cmp      dword ptr [0x40a2c8], ebx      
  0x00215624  7442                    je       0x215668                       
  0x00215626  68d0a24000              push     0x40a2d0                       
  0x0021562B  33ff                    xor      edi, edi                       
  0x0021562D  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00215633  668b4638                mov      ax, word ptr [esi + 0x38]      
  0x00215637  a802                    test     al, 2                          
  0x00215639  7410                    je       0x21564b                       
  0x0021563B  a810                    test     al, 0x10                       
  0x0021563D  750c                    jne      0x21564b                       
  0x0021563F  f6c402                  test     ah, 2                          
  0x00215642  740c                    je       0x215650                       
  0x00215644  bf00000001              mov      edi, 0x1000000                 
  0x00215649  eb05                    jmp      0x215650                       
                                        ; XREF: 0x00215639 (cond_jump), 0x0021563D (cond_jump)
  0x0021564B  bf00060080              mov      edi, 0x80000600                
                                        ; XREF: 0x00215642 (cond_jump), 0x00215649 (jump)
  0x00215650  a1c8a24000              mov      eax, dword ptr [0x40a2c8]      
  0x00215655  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00215658  50                      push     eax                            
  0x00215659  57                      push     edi                            
  0x0021565A  891dc8a24000            mov      dword ptr [0x40a2c8], ebx      
  0x00215660  e85ae1ffff              call     0x2137bf                       ; -> sub_002137BF
  0x00215665  8b7d08                  mov      edi, dword ptr [ebp + 8]       
                                        ; XREF: 0x0021561C (cond_jump), 0x00215624 (cond_jump)
  0x00215668  80663aef                and      byte ptr [esi + 0x3a], 0xef    
  0x0021566C  6a14                    push     0x14                           
  0x0021566E  eb77                    jmp      0x2156e7                       
; end of function
                                        ; XREF: 0x00215613 (cond_jump)
  0x00215670  a801                    test     al, 1                          
  0x00215672  744d                    je       0x2156c1                       
  0x00215674  33c9                    xor      ecx, ecx                       
  0x00215676  8a4e03                  mov      cl, byte ptr [esi + 3]         
  0x00215679  884d08                  mov      byte ptr [ebp + 8], cl         
  0x0021567C  b001                    mov      al, 1                          
  0x0021567E  49                      dec      ecx                            
  0x0021567F  d2e0                    shl      al, cl                         
  0x00215681  f6463801                test     byte ptr [esi + 0x38], 1       
  0x00215685  741a                    je       0x2156a1                       
  0x00215687  844605                  test     byte ptr [esi + 5], al         
  0x0021568A  740a                    je       0x215696                       
  0x0021568C  ff7508                  push     dword ptr [ebp + 8]            
  0x0021568F  8bcf                    mov      ecx, edi                       
  0x00215691  e897e2ffff              call     0x21392d                       ; -> sub_0021392D
                                        ; XREF: 0x0021568A (cond_jump)
  0x00215696  57                      push     edi                            
  0x00215697  e800ffffff              call     0x21559c                       ; -> sub_0021559C
  0x0021569C  e99f000000              jmp      0x215740                       
                                        ; XREF: 0x00215685 (cond_jump)
  0x002156A1  8a4e05                  mov      cl, byte ptr [esi + 5]         
  0x002156A4  84c8                    test     al, cl                         
  0x002156A6  7411                    je       0x2156b9                       
  0x002156A8  ff7508                  push     dword ptr [ebp + 8]            
  0x002156AB  f6d0                    not      al                             
  0x002156AD  22c1                    and      al, cl                         
  0x002156AF  8bcf                    mov      ecx, edi                       
  0x002156B1  884605                  mov      byte ptr [esi + 5], al         
  0x002156B4  e874e2ffff              call     0x21392d                       ; -> sub_0021392D
                                        ; XREF: 0x002156A6 (cond_jump)
  0x002156B9  80663afe                and      byte ptr [esi + 0x3a], 0xfe    
  0x002156BD  6a10                    push     0x10                           
  0x002156BF  eb26                    jmp      0x2156e7                       
                                        ; XREF: 0x00215672 (cond_jump)
  0x002156C1  a802                    test     al, 2                          
  0x002156C3  7408                    je       0x2156cd                       
  0x002156C5  6625fdff                and      ax, 0xfffd                     
  0x002156C9  6a11                    push     0x11                           
  0x002156CB  eb16                    jmp      0x2156e3                       
                                        ; XREF: 0x002156C3 (cond_jump)
  0x002156CD  a804                    test     al, 4                          
  0x002156CF  7408                    je       0x2156d9                       
  0x002156D1  6625fbff                and      ax, 0xfffb                     
  0x002156D5  6a12                    push     0x12                           
  0x002156D7  eb0a                    jmp      0x2156e3                       
                                        ; XREF: 0x002156CF (cond_jump)
  0x002156D9  a808                    test     al, 8                          
  0x002156DB  7454                    je       0x215731                       
  0x002156DD  6625f7ff                and      ax, 0xfff7                     
  0x002156E1  6a13                    push     0x13                           
                                        ; XREF: 0x002156CB (jump), 0x002156D7 (jump)
  0x002156E3  6689463a                mov      word ptr [esi + 0x3a], ax      
                                        ; XREF: 0x0021566E (jump), 0x002156BF (jump)
  0x002156E7  59                      pop      ecx                            
  0x002156E8  66894e32                mov      word ptr [esi + 0x32], cx      
  0x002156EC  660fb64e03              movzx    cx, byte ptr [esi + 3]         
  0x002156F1  8d4608                  lea      eax, [esi + 8]                 
  0x002156F4  66894e34                mov      word ptr [esi + 0x34], cx      
  0x002156F8  50                      push     eax                            
  0x002156F9  8bcf                    mov      ecx, edi                       
  0x002156FB  c60030                  mov      byte ptr [eax], 0x30           
  0x002156FE  c6460940                mov      byte ptr [esi + 9], 0x40       
  0x00215702  c74610b45c2100          mov      dword ptr [esi + 0x10], 0x215cb4 
  0x00215709  897e14                  mov      dword ptr [esi + 0x14], edi    
  0x0021570C  895e18                  mov      dword ptr [esi + 0x18], ebx    
  0x0021570F  895e20                  mov      dword ptr [esi + 0x20], ebx    
  0x00215712  895e1c                  mov      dword ptr [esi + 0x1c], ebx    
  0x00215715  885e24                  mov      byte ptr [esi + 0x24], bl      
  0x00215718  885e25                  mov      byte ptr [esi + 0x25], bl      
  0x0021571B  885e26                  mov      byte ptr [esi + 0x26], bl      
  0x0021571E  c6463023                mov      byte ptr [esi + 0x30], 0x23    
  0x00215722  c6463101                mov      byte ptr [esi + 0x31], 1       
  0x00215726  66895e36                mov      word ptr [esi + 0x36], bx      
  0x0021572A  e8f2eeffff              call     0x214621                       ; -> sub_00214621
  0x0021572F  eb0f                    jmp      0x215740                       
                                        ; XREF: 0x002156DB (cond_jump)
  0x00215731  6683663a00              and      word ptr [esi + 0x3a], 0       
  0x00215736  57                      push     edi                            
  0x00215737  83c608                  add      esi, 8                         
  0x0021573A  56                      push     esi                            
  0x0021573B  e874050000              call     0x215cb4                       ; -> sub_00215CB4
                                        ; XREF: 0x0021569C (jump), 0x0021572F (jump)
  0x00215740  5f                      pop      edi                            
  0x00215741  5e                      pop      esi                            
  0x00215742  5b                      pop      ebx                            
  0x00215743  5d                      pop      ebp                            
  0x00215744  c20400                  ret      4                              

; ============================================================
; Function: sub_00215747
; Start: 0x00215747  End: 0x00215769  Size: 34 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00214621
; Called by: sub_00215E76
; ============================================================
sub_00215747:
  0x00215747  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021574B  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x0021574F  50                      push     eax                            
  0x00215750  c6001c                  mov      byte ptr [eax], 0x1c           
  0x00215753  c64001c3                mov      byte ptr [eax + 1], 0xc3       
  0x00215757  c740080d552100          mov      dword ptr [eax + 8], 0x21550d  
  0x0021575E  89480c                  mov      dword ptr [eax + 0xc], ecx     
  0x00215761  e8bbeeffff              call     0x214621                       ; -> sub_00214621
  0x00215766  c20800                  ret      8                              
; end of function

; ============================================================
; Function: sub_00215769
; Start: 0x00215769  End: 0x00215839  Size: 208 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021427A, sub_00214621, sub_002154DD
; Called by: sub_00213AB0
; ============================================================
sub_00215769:
  0x00215769  55                      push     ebp                            
  0x0021576A  8bec                    mov      ebp, esp                       
  0x0021576C  51                      push     ecx                            
  0x0021576D  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x00215770  53                      push     ebx                            
  0x00215771  56                      push     esi                            
  0x00215772  57                      push     edi                            
  0x00215773  e802ebffff              call     0x21427a                       ; -> sub_0021427A
  0x00215778  33c9                    xor      ecx, ecx                       
  0x0021577A  3bc1                    cmp      eax, ecx                       
  0x0021577C  6a04                    push     4                              
  0x0021577E  b203                    mov      dl, 3                          
  0x00215780  5e                      pop      esi                            
  0x00215781  c745fc01000000          mov      dword ptr [ebp - 4], 1         
  0x00215788  bf4c552100              mov      edi, 0x21554c                  
  0x0021578D  0f84a6000000            je       0x215839                       
  0x00215793  8a5d0c                  mov      bl, byte ptr [ebp + 0xc]       
  0x00215796  385802                  cmp      byte ptr [eax + 2], bl         
  0x00215799  0f829a000000            jb       0x215839                       
  0x0021579F  384d14                  cmp      byte ptr [ebp + 0x14], cl      
  0x002157A2  8b4510                  mov      eax, dword ptr [ebp + 0x10]    
  0x002157A5  a3c8a24000              mov      dword ptr [0x40a2c8], eax      
  0x002157AA  7411                    je       0x2157bd                       
  0x002157AC  33d2                    xor      edx, edx                       
  0x002157AE  42                      inc      edx                            
  0x002157AF  8bf2                    mov      esi, edx                       
  0x002157B1  bf78552100              mov      edi, 0x215578                  
  0x002157B6  c745fc02000000          mov      dword ptr [ebp - 4], 2         
                                        ; XREF: 0x002157AA (cond_jump)
  0x002157BD  ff75fc                  push     dword ptr [ebp - 4]            
  0x002157C0  660fb6c3                movzx    ax, bl                         
  0x002157C4  893da0a24000            mov      dword ptr [0x40a2a0], edi      
  0x002157CA  8b7d08                  mov      edi, dword ptr [ebp + 8]       
  0x002157CD  c60598a2400030          mov      byte ptr [0x40a298], 0x30      
  0x002157D4  c60599a2400040          mov      byte ptr [0x40a299], 0x40      
  0x002157DB  893da4a24000            mov      dword ptr [0x40a2a4], edi      
  0x002157E1  890da8a24000            mov      dword ptr [0x40a2a8], ecx      
  0x002157E7  890db0a24000            mov      dword ptr [0x40a2b0], ecx      
  0x002157ED  890daca24000            mov      dword ptr [0x40a2ac], ecx      
  0x002157F3  880db4a24000            mov      byte ptr [0x40a2b4], cl        
  0x002157F9  880db5a24000            mov      byte ptr [0x40a2b5], cl        
  0x002157FF  880db6a24000            mov      byte ptr [0x40a2b6], cl        
  0x00215805  c605c0a2400023          mov      byte ptr [0x40a2c0], 0x23      
  0x0021580C  8815c1a24000            mov      byte ptr [0x40a2c1], dl        
  0x00215812  668935c2a24000          mov      word ptr [0x40a2c2], si        
  0x00215819  66a3c4a24000            mov      word ptr [0x40a2c4], ax        
  0x0021581F  66890dc6a24000          mov      word ptr [0x40a2c6], cx        
  0x00215826  e8b2fcffff              call     0x2154dd                       ; -> sub_002154DD
  0x0021582B  6898a24000              push     0x40a298                       
  0x00215830  8bcf                    mov      ecx, edi                       
  0x00215832  e8eaedffff              call     0x214621                       ; -> sub_00214621
  0x00215837  eb10                    jmp      0x215849                       
; end of function
                                        ; XREF: 0x0021578D (cond_jump), 0x00215799 (cond_jump)
  0x00215839  ff7510                  push     dword ptr [ebp + 0x10]         
  0x0021583C  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x0021583F  6800030080              push     0x80000300                     
  0x00215844  e876dfffff              call     0x2137bf                       ; -> sub_002137BF
                                        ; XREF: 0x00215837 (jump)
  0x00215849  5f                      pop      edi                            
  0x0021584A  5e                      pop      esi                            
  0x0021584B  5b                      pop      ebx                            
  0x0021584C  c9                      leave                                   
  0x0021584D  c21000                  ret      0x10                           

; ============================================================
; Function: sub_00215850
; Start: 0x00215850  End: 0x00215882  Size: 50 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021427A, sub_00214621
; Called by: sub_002159C9, sub_00215CB4
; ============================================================
sub_00215850:
  0x00215850  56                      push     esi                            
  0x00215851  8b742408                mov      esi, dword ptr [esp + 8]       
  0x00215855  8bce                    mov      ecx, esi                       
  0x00215857  e81eeaffff              call     0x21427a                       ; -> sub_0021427A
  0x0021585C  8b503c                  mov      edx, dword ptr [eax + 0x3c]    
  0x0021585F  8d4808                  lea      ecx, [eax + 8]                 
  0x00215862  c6011c                  mov      byte ptr [ecx], 0x1c           
  0x00215865  51                      push     ecx                            
  0x00215866  8bce                    mov      ecx, esi                       
  0x00215868  c6400943                mov      byte ptr [eax + 9], 0x43       
  0x0021586C  c7401047572100          mov      dword ptr [eax + 0x10], 0x215747 
  0x00215873  897014                  mov      dword ptr [eax + 0x14], esi    
  0x00215876  895018                  mov      dword ptr [eax + 0x18], edx    
  0x00215879  e8a3edffff              call     0x214621                       ; -> sub_00214621
  0x0021587E  5e                      pop      esi                            
  0x0021587F  c20400                  ret      4                              
; end of function
  0x00215882  53                      push     ebx                            
  0x00215883  8b5c240c                mov      ebx, dword ptr [esp + 0xc]     
  0x00215887  57                      push     edi                            
  0x00215888  8bcb                    mov      ecx, ebx                       
  0x0021588A  e8ebe9ffff              call     0x21427a                       ; -> sub_0021427A
  0x0021588F  8bf8                    mov      edi, eax                       
  0x00215891  8a07                    mov      al, byte ptr [edi]             
  0x00215893  a802                    test     al, 2                          
  0x00215895  7408                    je       0x21589f                       
  0x00215897  53                      push     ebx                            
  0x00215898  e8b3ffffff              call     0x215850                       ; -> sub_00215850
  0x0021589D  eb6f                    jmp      0x21590e                       
                                        ; XREF: 0x00215895 (cond_jump)
  0x0021589F  56                      push     esi                            
  0x002158A0  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x002158A4  837e0400                cmp      dword ptr [esi + 4], 0         
  0x002158A8  8bcb                    mov      ecx, ebx                       
  0x002158AA  7d0b                    jge      0x2158b7                       
  0x002158AC  0c08                    or       al, 8                          
  0x002158AE  8807                    mov      byte ptr [edi], al             
  0x002158B0  e8efe0ffff              call     0x2139a4                       ; -> sub_002139A4
  0x002158B5  eb56                    jmp      0x21590d                       
                                        ; XREF: 0x002158AA (cond_jump)
  0x002158B7  83660800                and      dword ptr [esi + 8], 0         
  0x002158BB  c60618                  mov      byte ptr [esi], 0x18           
  0x002158BE  c6460105                mov      byte ptr [esi + 1], 5          
  0x002158C2  8b473c                  mov      eax, dword ptr [edi + 0x3c]    
  0x002158C5  56                      push     esi                            
  0x002158C6  894610                  mov      dword ptr [esi + 0x10], eax    
  0x002158C9  c7461404000000          mov      dword ptr [esi + 0x14], 4      
  0x002158D0  e84cedffff              call     0x214621                       ; -> sub_00214621
  0x002158D5  c60628                  mov      byte ptr [esi], 0x28           
  0x002158D8  c6460141                mov      byte ptr [esi + 1], 0x41       
  0x002158DC  c74608195a2100          mov      dword ptr [esi + 8], 0x215a19  
  0x002158E3  895e0c                  mov      dword ptr [esi + 0xc], ebx     
  0x002158E6  8b473c                  mov      eax, dword ptr [edi + 0x3c]    
  0x002158E9  894610                  mov      dword ptr [esi + 0x10], eax    
  0x002158EC  8d4738                  lea      eax, [edi + 0x38]              
  0x002158EF  894618                  mov      dword ptr [esi + 0x18], eax    
  0x002158F2  0fb64707                movzx    eax, byte ptr [edi + 7]        
  0x002158F6  80661e00                and      byte ptr [esi + 0x1e], 0       
  0x002158FA  56                      push     esi                            
  0x002158FB  8bcb                    mov      ecx, ebx                       
  0x002158FD  894614                  mov      dword ptr [esi + 0x14], eax    
  0x00215900  c6461c02                mov      byte ptr [esi + 0x1c], 2       
  0x00215904  c6461d01                mov      byte ptr [esi + 0x1d], 1       
  0x00215908  e814edffff              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x002158B5 (jump)
  0x0021590D  5e                      pop      esi                            
                                        ; XREF: 0x0021589D (jump)
  0x0021590E  5f                      pop      edi                            
  0x0021590F  5b                      pop      ebx                            
  0x00215910  c20800                  ret      8                              
  0x00215913  56                      push     esi                            
  0x00215914  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00215918  8bce                    mov      ecx, esi                       
  0x0021591A  e85be9ffff              call     0x21427a                       ; -> sub_0021427A
  0x0021591F  8a08                    mov      cl, byte ptr [eax]             
  0x00215921  f6c102                  test     cl, 2                          
  0x00215924  7408                    je       0x21592e                       
  0x00215926  56                      push     esi                            
  0x00215927  e824ffffff              call     0x215850                       ; -> sub_00215850
  0x0021592C  eb3c                    jmp      0x21596a                       
                                        ; XREF: 0x00215924 (cond_jump)
  0x0021592E  8b542408                mov      edx, dword ptr [esp + 8]       
  0x00215932  837a0400                cmp      dword ptr [edx + 4], 0         
  0x00215936  7c0c                    jl       0x215944                       
  0x00215938  80600600                and      byte ptr [eax + 6], 0          
  0x0021593C  56                      push     esi                            
  0x0021593D  e8b7fcffff              call     0x2155f9                       ; -> sub_002155F9
  0x00215942  eb26                    jmp      0x21596a                       
                                        ; XREF: 0x00215936 (cond_jump)
  0x00215944  fe4006                  inc      byte ptr [eax + 6]             
  0x00215947  80780603                cmp      byte ptr [eax + 6], 3          
  0x0021594B  760e                    jbe      0x21595b                       
  0x0021594D  80c908                  or       cl, 8                          
  0x00215950  8808                    mov      byte ptr [eax], cl             
  0x00215952  8bce                    mov      ecx, esi                       
  0x00215954  e84be0ffff              call     0x2139a4                       ; -> sub_002139A4
  0x00215959  eb0f                    jmp      0x21596a                       
                                        ; XREF: 0x0021594B (cond_jump)
  0x0021595B  52                      push     edx                            
  0x0021595C  8bce                    mov      ecx, esi                       
  0x0021595E  c7421404000000          mov      dword ptr [edx + 0x14], 4      
  0x00215965  e8b7ecffff              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x0021592C (jump), 0x00215942 (jump), 0x00215959 (jump)
  0x0021596A  5e                      pop      esi                            
  0x0021596B  c20800                  ret      8                              
  0x0021596E  56                      push     esi                            
  0x0021596F  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00215973  8bce                    mov      ecx, esi                       
  0x00215975  e800e9ffff              call     0x21427a                       ; -> sub_0021427A
  0x0021597A  8a08                    mov      cl, byte ptr [eax]             
  0x0021597C  f6c102                  test     cl, 2                          
  0x0021597F  7408                    je       0x215989                       
  0x00215981  56                      push     esi                            
  0x00215982  e8c9feffff              call     0x215850                       ; -> sub_00215850
  0x00215987  eb3c                    jmp      0x2159c5                       
                                        ; XREF: 0x0021597F (cond_jump)
  0x00215989  8b542408                mov      edx, dword ptr [esp + 8]       
  0x0021598D  837a0400                cmp      dword ptr [edx + 4], 0         
  0x00215991  7d28                    jge      0x2159bb                       
  0x00215993  fe4006                  inc      byte ptr [eax + 6]             
  0x00215996  80780603                cmp      byte ptr [eax + 6], 3          
  0x0021599A  760e                    jbe      0x2159aa                       
  0x0021599C  80c908                  or       cl, 8                          
  0x0021599F  8808                    mov      byte ptr [eax], cl             
  0x002159A1  8bce                    mov      ecx, esi                       
  0x002159A3  e8fcdfffff              call     0x2139a4                       ; -> sub_002139A4
  0x002159A8  eb1b                    jmp      0x2159c5                       
                                        ; XREF: 0x0021599A (cond_jump)
  0x002159AA  52                      push     edx                            
  0x002159AB  8bce                    mov      ecx, esi                       
  0x002159AD  c7421404000000          mov      dword ptr [edx + 0x14], 4      
  0x002159B4  e868ecffff              call     0x214621                       ; -> sub_00214621
  0x002159B9  eb0a                    jmp      0x2159c5                       
                                        ; XREF: 0x00215991 (cond_jump)
  0x002159BB  80600600                and      byte ptr [eax + 6], 0          
  0x002159BF  56                      push     esi                            
  0x002159C0  e826020000              call     0x215beb                       ; -> sub_00215BEB
                                        ; XREF: 0x00215987 (jump), 0x002159A8 (jump), 0x002159B9 (jump)
  0x002159C5  5e                      pop      esi                            
  0x002159C6  c20800                  ret      8                              

; ============================================================
; Function: sub_002159C9
; Start: 0x002159C9  End: 0x00215A19  Size: 80 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021392D, sub_0021427A, sub_00215850
; ============================================================
sub_002159C9:
  0x002159C9  55                      push     ebp                            
  0x002159CA  8bec                    mov      ebp, esp                       
  0x002159CC  51                      push     ecx                            
  0x002159CD  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002159D0  53                      push     ebx                            
  0x002159D1  56                      push     esi                            
  0x002159D2  e8a3e8ffff              call     0x21427a                       ; -> sub_0021427A
  0x002159D7  8bf0                    mov      esi, eax                       
  0x002159D9  800e02                  or       byte ptr [esi], 2              
  0x002159DC  33db                    xor      ebx, ebx                       
  0x002159DE  43                      inc      ebx                            
  0x002159DF  885dfc                  mov      byte ptr [ebp - 4], bl         
                                        ; XREF: 0x00215A04 (cond_jump)
  0x002159E2  845e05                  test     byte ptr [esi + 5], bl         
  0x002159E5  7412                    je       0x2159f9                       
  0x002159E7  ff75fc                  push     dword ptr [ebp - 4]            
  0x002159EA  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x002159ED  e83bdfffff              call     0x21392d                       ; -> sub_0021392D
  0x002159F2  8ac3                    mov      al, bl                         
  0x002159F4  f6d0                    not      al                             
  0x002159F6  204605                  and      byte ptr [esi + 5], al         
                                        ; XREF: 0x002159E5 (cond_jump)
  0x002159F9  d1e3                    shl      ebx, 1                         
  0x002159FB  fe45fc                  inc      byte ptr [ebp - 4]             
  0x002159FE  8a45fc                  mov      al, byte ptr [ebp - 4]         
  0x00215A01  3a4602                  cmp      al, byte ptr [esi + 2]         
  0x00215A04  76dc                    jbe      0x2159e2                       
  0x00215A06  f60608                  test     byte ptr [esi], 8              
  0x00215A09  5e                      pop      esi                            
  0x00215A0A  5b                      pop      ebx                            
  0x00215A0B  7408                    je       0x215a15                       
  0x00215A0D  ff7508                  push     dword ptr [ebp + 8]            
  0x00215A10  e83bfeffff              call     0x215850                       ; -> sub_00215850
                                        ; XREF: 0x00215A0B (cond_jump)
  0x00215A15  c9                      leave                                   
  0x00215A16  c20400                  ret      4                              
; end of function
  0x00215A19  53                      push     ebx                            
  0x00215A1A  56                      push     esi                            
  0x00215A1B  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x00215A1F  8bce                    mov      ecx, esi                       
  0x00215A21  e854e8ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215A26  8a18                    mov      bl, byte ptr [eax]             
  0x00215A28  f6c302                  test     bl, 2                          
  0x00215A2B  740b                    je       0x215a38                       
  0x00215A2D  56                      push     esi                            
  0x00215A2E  e81dfeffff              call     0x215850                       ; -> sub_00215850
  0x00215A33  e983000000              jmp      0x215abb                       
                                        ; XREF: 0x00215A2B (cond_jump)
  0x00215A38  8b4c240c                mov      ecx, dword ptr [esp + 0xc]     
  0x00215A3C  33d2                    xor      edx, edx                       
  0x00215A3E  395104                  cmp      dword ptr [ecx + 4], edx       
  0x00215A41  7c1d                    jl       0x215a60                       
  0x00215A43  33c9                    xor      ecx, ecx                       
  0x00215A45  8a4802                  mov      cl, byte ptr [eax + 2]         
  0x00215A48  885006                  mov      byte ptr [eax + 6], dl         
  0x00215A4B  b201                    mov      dl, 1                          
  0x00215A4D  56                      push     esi                            
  0x00215A4E  41                      inc      ecx                            
  0x00215A4F  d2e2                    shl      dl, cl                         
  0x00215A51  feca                    dec      dl                             
  0x00215A53  225038                  and      dl, byte ptr [eax + 0x38]      
  0x00215A56  885004                  mov      byte ptr [eax + 4], dl         
  0x00215A59  e88d010000              call     0x215beb                       ; -> sub_00215BEB
  0x00215A5E  eb5b                    jmp      0x215abb                       
                                        ; XREF: 0x00215A41 (cond_jump)
  0x00215A60  fe4006                  inc      byte ptr [eax + 6]             
  0x00215A63  80780603                cmp      byte ptr [eax + 6], 3          
  0x00215A67  760e                    jbe      0x215a77                       
  0x00215A69  80cb08                  or       bl, 8                          
  0x00215A6C  8bce                    mov      ecx, esi                       
  0x00215A6E  8818                    mov      byte ptr [eax], bl             
  0x00215A70  e82fdfffff              call     0x2139a4                       ; -> sub_002139A4
  0x00215A75  eb44                    jmp      0x215abb                       
                                        ; XREF: 0x00215A67 (cond_jump)
  0x00215A77  c60130                  mov      byte ptr [ecx], 0x30           
  0x00215A7A  c6410140                mov      byte ptr [ecx + 1], 0x40       
  0x00215A7E  c7410882582100          mov      dword ptr [ecx + 8], 0x215882  
  0x00215A85  89710c                  mov      dword ptr [ecx + 0xc], esi     
  0x00215A88  895110                  mov      dword ptr [ecx + 0x10], edx    
  0x00215A8B  895118                  mov      dword ptr [ecx + 0x18], edx    
  0x00215A8E  895114                  mov      dword ptr [ecx + 0x14], edx    
  0x00215A91  88511c                  mov      byte ptr [ecx + 0x1c], dl      
  0x00215A94  88511d                  mov      byte ptr [ecx + 0x1d], dl      
  0x00215A97  88511e                  mov      byte ptr [ecx + 0x1e], dl      
  0x00215A9A  c6412802                mov      byte ptr [ecx + 0x28], 2       
  0x00215A9E  c6412901                mov      byte ptr [ecx + 0x29], 1       
  0x00215AA2  6689512a                mov      word ptr [ecx + 0x2a], dx      
  0x00215AA6  660fb64001              movzx    ax, byte ptr [eax + 1]         
  0x00215AAB  6689412c                mov      word ptr [ecx + 0x2c], ax      
  0x00215AAF  6689512e                mov      word ptr [ecx + 0x2e], dx      
  0x00215AB3  51                      push     ecx                            
  0x00215AB4  8bce                    mov      ecx, esi                       
  0x00215AB6  e866ebffff              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x00215A33 (jump), 0x00215A5E (jump), 0x00215A75 (jump)
  0x00215ABB  5e                      pop      esi                            
  0x00215ABC  5b                      pop      ebx                            
  0x00215ABD  c20800                  ret      8                              
  0x00215AC0  57                      push     edi                            
  0x00215AC1  8b7c240c                mov      edi, dword ptr [esp + 0xc]     
  0x00215AC5  8bcf                    mov      ecx, edi                       
  0x00215AC7  e8aee7ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215ACC  8a08                    mov      cl, byte ptr [eax]             
  0x00215ACE  f6c102                  test     cl, 2                          
  0x00215AD1  740b                    je       0x215ade                       
  0x00215AD3  57                      push     edi                            
  0x00215AD4  e877fdffff              call     0x215850                       ; -> sub_00215850
  0x00215AD9  e99e000000              jmp      0x215b7c                       
                                        ; XREF: 0x00215AD1 (cond_jump)
  0x00215ADE  56                      push     esi                            
  0x00215ADF  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00215AE3  33d2                    xor      edx, edx                       
  0x00215AE5  395604                  cmp      dword ptr [esi + 4], edx       
  0x00215AE8  7d28                    jge      0x215b12                       
  0x00215AEA  fe4006                  inc      byte ptr [eax + 6]             
  0x00215AED  80780603                cmp      byte ptr [eax + 6], 3          
  0x00215AF1  760e                    jbe      0x215b01                       
  0x00215AF3  80c908                  or       cl, 8                          
  0x00215AF6  8808                    mov      byte ptr [eax], cl             
  0x00215AF8  8bcf                    mov      ecx, edi                       
  0x00215AFA  e8a5deffff              call     0x2139a4                       ; -> sub_002139A4
  0x00215AFF  eb7a                    jmp      0x215b7b                       
                                        ; XREF: 0x00215AF1 (cond_jump)
  0x00215B01  c7461404000000          mov      dword ptr [esi + 0x14], 4      
  0x00215B08  56                      push     esi                            
                                        ; XREF: 0x00215B73 (jump)
  0x00215B09  8bcf                    mov      ecx, edi                       
  0x00215B0B  e811ebffff              call     0x214621                       ; -> sub_00214621
  0x00215B10  eb69                    jmp      0x215b7b                       
                                        ; XREF: 0x00215AE8 (cond_jump)
  0x00215B12  668b483a                mov      cx, word ptr [eax + 0x3a]      
  0x00215B16  f6c101                  test     cl, 1                          
  0x00215B19  885006                  mov      byte ptr [eax + 6], dl         
  0x00215B1C  7409                    je       0x215b27                       
  0x00215B1E  33f6                    xor      esi, esi                       
  0x00215B20  6681e1feff              and      cx, 0xfffe                     
  0x00215B25  eb0d                    jmp      0x215b34                       
                                        ; XREF: 0x00215B1C (cond_jump)
  0x00215B27  f6c102                  test     cl, 2                          
  0x00215B2A  7449                    je       0x215b75                       
  0x00215B2C  33f6                    xor      esi, esi                       
  0x00215B2E  46                      inc      esi                            
  0x00215B2F  6681e1fdff              and      cx, 0xfffd                     
                                        ; XREF: 0x00215B25 (jump)
  0x00215B34  6689483a                mov      word ptr [eax + 0x3a], cx      
  0x00215B38  8d4808                  lea      ecx, [eax + 8]                 
  0x00215B3B  c60130                  mov      byte ptr [ecx], 0x30           
  0x00215B3E  c6400940                mov      byte ptr [eax + 9], 0x40       
  0x00215B42  c740106e592100          mov      dword ptr [eax + 0x10], 0x21596e 
  0x00215B49  897814                  mov      dword ptr [eax + 0x14], edi    
  0x00215B4C  895018                  mov      dword ptr [eax + 0x18], edx    
  0x00215B4F  895020                  mov      dword ptr [eax + 0x20], edx    
  0x00215B52  89501c                  mov      dword ptr [eax + 0x1c], edx    
  0x00215B55  885024                  mov      byte ptr [eax + 0x24], dl      
  0x00215B58  885025                  mov      byte ptr [eax + 0x25], dl      
  0x00215B5B  885026                  mov      byte ptr [eax + 0x26], dl      
  0x00215B5E  c6403020                mov      byte ptr [eax + 0x30], 0x20    
  0x00215B62  c6403101                mov      byte ptr [eax + 0x31], 1       
  0x00215B66  66897032                mov      word ptr [eax + 0x32], si      
  0x00215B6A  66895034                mov      word ptr [eax + 0x34], dx      
  0x00215B6E  66895036                mov      word ptr [eax + 0x36], dx      
  0x00215B72  51                      push     ecx                            
  0x00215B73  eb94                    jmp      0x215b09                       
                                        ; XREF: 0x00215B2A (cond_jump)
  0x00215B75  57                      push     edi                            
  0x00215B76  e870000000              call     0x215beb                       ; -> sub_00215BEB
                                        ; XREF: 0x00215AFF (jump), 0x00215B10 (jump)
  0x00215B7B  5e                      pop      esi                            
                                        ; XREF: 0x00215AD9 (jump)
  0x00215B7C  5f                      pop      edi                            
  0x00215B7D  c20800                  ret      8                              
  0x00215B80  56                      push     esi                            
  0x00215B81  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00215B85  8bce                    mov      ecx, esi                       
  0x00215B87  e8eee6ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215B8C  8a4803                  mov      cl, byte ptr [eax + 3]         
  0x00215B8F  3a4802                  cmp      cl, byte ptr [eax + 2]         
  0x00215B92  7537                    jne      0x215bcb                       
  0x00215B94  8b483c                  mov      ecx, dword ptr [eax + 0x3c]    
  0x00215B97  80600300                and      byte ptr [eax + 3], 0          
  0x00215B9B  80602600                and      byte ptr [eax + 0x26], 0       
  0x00215B9F  894818                  mov      dword ptr [eax + 0x18], ecx    
  0x00215BA2  8d4838                  lea      ecx, [eax + 0x38]              
  0x00215BA5  894820                  mov      dword ptr [eax + 0x20], ecx    
  0x00215BA8  0fb64807                movzx    ecx, byte ptr [eax + 7]        
  0x00215BAC  c6400828                mov      byte ptr [eax + 8], 0x28       
  0x00215BB0  c6400941                mov      byte ptr [eax + 9], 0x41       
  0x00215BB4  c74010195a2100          mov      dword ptr [eax + 0x10], 0x215a19 
  0x00215BBB  897014                  mov      dword ptr [eax + 0x14], esi    
  0x00215BBE  89481c                  mov      dword ptr [eax + 0x1c], ecx    
  0x00215BC1  c6402402                mov      byte ptr [eax + 0x24], 2       
  0x00215BC5  c6402501                mov      byte ptr [eax + 0x25], 1       
  0x00215BC9  eb11                    jmp      0x215bdc                       
                                        ; XREF: 0x00215B92 (cond_jump)
  0x00215BCB  8b542408                mov      edx, dword ptr [esp + 8]       
  0x00215BCF  fec1                    inc      cl                             
  0x00215BD1  884803                  mov      byte ptr [eax + 3], cl         
  0x00215BD4  660fb6c9                movzx    cx, cl                         
  0x00215BD8  66894a2c                mov      word ptr [edx + 0x2c], cx      
                                        ; XREF: 0x00215BC9 (jump)
  0x00215BDC  83c008                  add      eax, 8                         
  0x00215BDF  50                      push     eax                            
  0x00215BE0  8bce                    mov      ecx, esi                       
  0x00215BE2  e83aeaffff              call     0x214621                       ; -> sub_00214621
  0x00215BE7  5e                      pop      esi                            
  0x00215BE8  c20800                  ret      8                              

; ============================================================
; Function: sub_00215BEB
; Start: 0x00215BEB  End: 0x00215C1C  Size: 49 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021427A
; ============================================================
sub_00215BEB:
  0x00215BEB  55                      push     ebp                            
  0x00215BEC  8bec                    mov      ebp, esp                       
  0x00215BEE  53                      push     ebx                            
  0x00215BEF  56                      push     esi                            
  0x00215BF0  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x00215BF3  57                      push     edi                            
  0x00215BF4  8bce                    mov      ecx, esi                       
  0x00215BF6  e87fe6ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215BFB  8a5804                  mov      bl, byte ptr [eax + 4]         
  0x00215BFE  33d2                    xor      edx, edx                       
  0x00215C00  b101                    mov      cl, 1                          
  0x00215C02  885003                  mov      byte ptr [eax + 3], dl         
  0x00215C05  885d0b                  mov      byte ptr [ebp + 0xb], bl       
                                        ; XREF: 0x00215C18 (cond_jump)
  0x00215C08  844d0b                  test     byte ptr [ebp + 0xb], cl       
  0x00215C0B  750f                    jne      0x215c1c                       
  0x00215C0D  fe4003                  inc      byte ptr [eax + 3]             
  0x00215C10  8a5803                  mov      bl, byte ptr [eax + 3]         
  0x00215C13  d0e1                    shl      cl, 1                          
  0x00215C15  3a5802                  cmp      bl, byte ptr [eax + 2]         
  0x00215C18  76ee                    jbe      0x215c08                       
  0x00215C1A  eb08                    jmp      0x215c24                       
; end of function
                                        ; XREF: 0x00215C0B (cond_jump)
  0x00215C1C  f6d1                    not      cl                             
  0x00215C1E  224804                  and      cl, byte ptr [eax + 4]         
  0x00215C21  884804                  mov      byte ptr [eax + 4], cl         
                                        ; XREF: 0x00215C1A (jump)
  0x00215C24  8a5803                  mov      bl, byte ptr [eax + 3]         
  0x00215C27  3a5802                  cmp      bl, byte ptr [eax + 2]         
  0x00215C2A  885026                  mov      byte ptr [eax + 0x26], dl      
  0x00215C2D  8d4808                  lea      ecx, [eax + 8]                 
  0x00215C30  7624                    jbe      0x215c56                       
  0x00215C32  8b783c                  mov      edi, dword ptr [eax + 0x3c]    
  0x00215C35  897818                  mov      dword ptr [eax + 0x18], edi    
  0x00215C38  8d7838                  lea      edi, [eax + 0x38]              
  0x00215C3B  897820                  mov      dword ptr [eax + 0x20], edi    
  0x00215C3E  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x00215C42  c60128                  mov      byte ptr [ecx], 0x28           
  0x00215C45  c6400941                mov      byte ptr [eax + 9], 0x41       
  0x00215C49  c74010195a2100          mov      dword ptr [eax + 0x10], 0x215a19 
  0x00215C50  c6402501                mov      byte ptr [eax + 0x25], 1       
  0x00215C54  eb45                    jmp      0x215c9b                       
                                        ; XREF: 0x00215C30 (cond_jump)
  0x00215C56  3ada                    cmp      bl, dl                         
  0x00215C58  8d7838                  lea      edi, [eax + 0x38]              
  0x00215C5B  6a04                    push     4                              
  0x00215C5D  897820                  mov      dword ptr [eax + 0x20], edi    
  0x00215C60  5f                      pop      edi                            
  0x00215C61  66895032                mov      word ptr [eax + 0x32], dx      
  0x00215C65  885031                  mov      byte ptr [eax + 0x31], dl      
  0x00215C68  885025                  mov      byte ptr [eax + 0x25], dl      
  0x00215C6B  895018                  mov      dword ptr [eax + 0x18], edx    
  0x00215C6E  c6400940                mov      byte ptr [eax + 9], 0x40       
  0x00215C72  c60130                  mov      byte ptr [ecx], 0x30           
  0x00215C75  66897836                mov      word ptr [eax + 0x36], di      
  0x00215C79  750d                    jne      0x215c88                       
  0x00215C7B  c74010c05a2100          mov      dword ptr [eax + 0x10], 0x215ac0 
  0x00215C82  c64030a0                mov      byte ptr [eax + 0x30], 0xa0    
  0x00215C86  eb0f                    jmp      0x215c97                       
                                        ; XREF: 0x00215C79 (cond_jump)
  0x00215C88  c7401013592100          mov      dword ptr [eax + 0x10], 0x215913 
  0x00215C8F  c64030a3                mov      byte ptr [eax + 0x30], 0xa3    
  0x00215C93  660fb6d3                movzx    dx, bl                         
                                        ; XREF: 0x00215C86 (jump)
  0x00215C97  66895034                mov      word ptr [eax + 0x34], dx      
                                        ; XREF: 0x00215C54 (jump)
  0x00215C9B  51                      push     ecx                            
  0x00215C9C  8bce                    mov      ecx, esi                       
  0x00215C9E  897014                  mov      dword ptr [eax + 0x14], esi    
  0x00215CA1  89781c                  mov      dword ptr [eax + 0x1c], edi    
  0x00215CA4  c6402402                mov      byte ptr [eax + 0x24], 2       
  0x00215CA8  e874e9ffff              call     0x214621                       ; -> sub_00214621
  0x00215CAD  5f                      pop      edi                            
  0x00215CAE  5e                      pop      esi                            
  0x00215CAF  5b                      pop      ebx                            
  0x00215CB0  5d                      pop      ebp                            
  0x00215CB1  c20400                  ret      4                              

; ============================================================
; Function: sub_00215CB4
; Start: 0x00215CB4  End: 0x00215CCD  Size: 25 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021427A, sub_00215850
; ============================================================
sub_00215CB4:
  0x00215CB4  56                      push     esi                            
  0x00215CB5  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x00215CB9  8bce                    mov      ecx, esi                       
  0x00215CBB  e8bae5ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215CC0  f60002                  test     byte ptr [eax], 2              
  0x00215CC3  56                      push     esi                            
  0x00215CC4  7407                    je       0x215ccd                       
  0x00215CC6  e885fbffff              call     0x215850                       ; -> sub_00215850
  0x00215CCB  eb13                    jmp      0x215ce0                       
; end of function
                                        ; XREF: 0x00215CC4 (cond_jump)
  0x00215CCD  6683783a00              cmp      word ptr [eax + 0x3a], 0       
  0x00215CD2  7407                    je       0x215cdb                       
  0x00215CD4  e820f9ffff              call     0x2155f9                       ; -> sub_002155F9
  0x00215CD9  eb05                    jmp      0x215ce0                       
                                        ; XREF: 0x00215CD2 (cond_jump)
  0x00215CDB  e80bffffff              call     0x215beb                       ; -> sub_00215BEB
                                        ; XREF: 0x00215CCB (jump), 0x00215CD9 (jump)
  0x00215CE0  5e                      pop      esi                            
  0x00215CE1  c20800                  ret      8                              
  0x00215CE4  53                      push     ebx                            
  0x00215CE5  56                      push     esi                            
  0x00215CE6  57                      push     edi                            
  0x00215CE7  8b7c2414                mov      edi, dword ptr [esp + 0x14]    
  0x00215CEB  8bcf                    mov      ecx, edi                       
  0x00215CED  e888e5ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215CF2  8bf0                    mov      esi, eax                       
  0x00215CF4  80663afe                and      byte ptr [esi + 0x3a], 0xfe    
  0x00215CF8  33c0                    xor      eax, eax                       
  0x00215CFA  8a4603                  mov      al, byte ptr [esi + 3]         
  0x00215CFD  33c9                    xor      ecx, ecx                       
  0x00215CFF  8ac8                    mov      cl, al                         
  0x00215D01  b301                    mov      bl, 1                          
  0x00215D03  6a05                    push     5                              
  0x00215D05  50                      push     eax                            
  0x00215D06  49                      dec      ecx                            
  0x00215D07  d2e3                    shl      bl, cl                         
  0x00215D09  8bcf                    mov      ecx, edi                       
  0x00215D0B  e86ed8ffff              call     0x21357e                       ; -> sub_0021357E
  0x00215D10  660fb65603              movzx    dx, byte ptr [esi + 3]         
  0x00215D15  085e05                  or       byte ptr [esi + 5], bl         
  0x00215D18  33c9                    xor      ecx, ecx                       
  0x00215D1A  8d4608                  lea      eax, [esi + 8]                 
  0x00215D1D  894e18                  mov      dword ptr [esi + 0x18], ecx    
  0x00215D20  894e20                  mov      dword ptr [esi + 0x20], ecx    
  0x00215D23  894e1c                  mov      dword ptr [esi + 0x1c], ecx    
  0x00215D26  884e24                  mov      byte ptr [esi + 0x24], cl      
  0x00215D29  884e25                  mov      byte ptr [esi + 0x25], cl      
  0x00215D2C  884e26                  mov      byte ptr [esi + 0x26], cl      
  0x00215D2F  66894e36                mov      word ptr [esi + 0x36], cx      
  0x00215D33  50                      push     eax                            
  0x00215D34  8bcf                    mov      ecx, edi                       
  0x00215D36  c60030                  mov      byte ptr [eax], 0x30           
  0x00215D39  c6460940                mov      byte ptr [esi + 9], 0x40       
  0x00215D3D  c74610b45c2100          mov      dword ptr [esi + 0x10], 0x215cb4 
  0x00215D44  897e14                  mov      dword ptr [esi + 0x14], edi    
  0x00215D47  c6463023                mov      byte ptr [esi + 0x30], 0x23    
  0x00215D4B  c6463101                mov      byte ptr [esi + 0x31], 1       
  0x00215D4F  66c746321000            mov      word ptr [esi + 0x32], 0x10    
  0x00215D55  66895634                mov      word ptr [esi + 0x34], dx      
  0x00215D59  e8c3e8ffff              call     0x214621                       ; -> sub_00214621
  0x00215D5E  5f                      pop      edi                            
  0x00215D5F  5e                      pop      esi                            
  0x00215D60  5b                      pop      ebx                            
  0x00215D61  c20800                  ret      8                              
  0x00215D64  53                      push     ebx                            
  0x00215D65  56                      push     esi                            
  0x00215D66  57                      push     edi                            
  0x00215D67  8b7c2414                mov      edi, dword ptr [esp + 0x14]    
  0x00215D6B  8bcf                    mov      ecx, edi                       
  0x00215D6D  e808e5ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215D72  68d0a24000              push     0x40a2d0                       
  0x00215D77  8bf0                    mov      esi, eax                       
  0x00215D79  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00215D7F  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x00215D83  33db                    xor      ebx, ebx                       
  0x00215D85  395804                  cmp      dword ptr [eax + 4], ebx       
  0x00215D88  7d08                    jge      0x215d92                       
  0x00215D8A  57                      push     edi                            
  0x00215D8B  e8c0faffff              call     0x215850                       ; -> sub_00215850
  0x00215D90  eb62                    jmp      0x215df4                       
                                        ; XREF: 0x00215D88 (cond_jump)
  0x00215D92  a06aa24000              mov      al, byte ptr [0x40a26a]        
  0x00215D97  3c07                    cmp      al, 7                          
  0x00215D99  884602                  mov      byte ptr [esi + 2], al         
  0x00215D9C  7604                    jbe      0x215da2                       
  0x00215D9E  c6460207                mov      byte ptr [esi + 2], 7          
                                        ; XREF: 0x00215D9C (cond_jump)
  0x00215DA2  53                      push     ebx                            
  0x00215DA3  8bcf                    mov      ecx, edi                       
  0x00215DA5  e8d8ddffff              call     0x213b82                       ; -> sub_00213B82
  0x00215DAA  8d4608                  lea      eax, [esi + 8]                 
  0x00215DAD  50                      push     eax                            
  0x00215DAE  8bcf                    mov      ecx, edi                       
  0x00215DB0  c6460301                mov      byte ptr [esi + 3], 1          
  0x00215DB4  c60030                  mov      byte ptr [eax], 0x30           
  0x00215DB7  c6460940                mov      byte ptr [esi + 9], 0x40       
  0x00215DBB  c74610805b2100          mov      dword ptr [esi + 0x10], 0x215b80 
  0x00215DC2  897e14                  mov      dword ptr [esi + 0x14], edi    
  0x00215DC5  895e18                  mov      dword ptr [esi + 0x18], ebx    
  0x00215DC8  895e20                  mov      dword ptr [esi + 0x20], ebx    
  0x00215DCB  895e1c                  mov      dword ptr [esi + 0x1c], ebx    
  0x00215DCE  885e24                  mov      byte ptr [esi + 0x24], bl      
  0x00215DD1  885e25                  mov      byte ptr [esi + 0x25], bl      
  0x00215DD4  885e26                  mov      byte ptr [esi + 0x26], bl      
  0x00215DD7  c6463023                mov      byte ptr [esi + 0x30], 0x23    
  0x00215DDB  c6463103                mov      byte ptr [esi + 0x31], 3       
  0x00215DDF  66c746320800            mov      word ptr [esi + 0x32], 8       
  0x00215DE5  66c746340100            mov      word ptr [esi + 0x34], 1       
  0x00215DEB  66895e36                mov      word ptr [esi + 0x36], bx      
  0x00215DEF  e82de8ffff              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x00215D90 (jump)
  0x00215DF4  5f                      pop      edi                            
  0x00215DF5  5e                      pop      esi                            
  0x00215DF6  5b                      pop      ebx                            
  0x00215DF7  c20800                  ret      8                              
  0x00215DFA  56                      push     esi                            
  0x00215DFB  68d0a24000              push     0x40a2d0                       
  0x00215E00  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00215E06  8b742408                mov      esi, dword ptr [esp + 8]       
  0x00215E0A  33c0                    xor      eax, eax                       
  0x00215E0C  394604                  cmp      dword ptr [esi + 4], eax       
  0x00215E0F  7d0b                    jge      0x215e1c                       
  0x00215E11  ff74240c                push     dword ptr [esp + 0xc]          
  0x00215E15  e836faffff              call     0x215850                       ; -> sub_00215850
  0x00215E1A  eb56                    jmp      0x215e72                       
                                        ; XREF: 0x00215E0F (cond_jump)
  0x00215E1C  57                      push     edi                            
  0x00215E1D  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00215E21  6a08                    push     8                              
  0x00215E23  59                      pop      ecx                            
  0x00215E24  50                      push     eax                            
  0x00215E25  c60630                  mov      byte ptr [esi], 0x30           
  0x00215E28  c6460140                mov      byte ptr [esi + 1], 0x40       
  0x00215E2C  c74608645d2100          mov      dword ptr [esi + 8], 0x215d64  
  0x00215E33  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x00215E36  894610                  mov      dword ptr [esi + 0x10], eax    
  0x00215E39  c7461868a24000          mov      dword ptr [esi + 0x18], 0x40a268 
  0x00215E40  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x00215E43  c6461c02                mov      byte ptr [esi + 0x1c], 2       
  0x00215E47  c6461d01                mov      byte ptr [esi + 0x1d], 1       
  0x00215E4B  88461e                  mov      byte ptr [esi + 0x1e], al      
  0x00215E4E  c64628a0                mov      byte ptr [esi + 0x28], 0xa0    
  0x00215E52  c6462906                mov      byte ptr [esi + 0x29], 6       
  0x00215E56  66c7462a0029            mov      word ptr [esi + 0x2a], 0x2900  
  0x00215E5C  6689462c                mov      word ptr [esi + 0x2c], ax      
  0x00215E60  66894e2e                mov      word ptr [esi + 0x2e], cx      
  0x00215E64  e874f6ffff              call     0x2154dd                       ; -> sub_002154DD
  0x00215E69  56                      push     esi                            
  0x00215E6A  8bcf                    mov      ecx, edi                       
  0x00215E6C  e8b0e7ffff              call     0x214621                       ; -> sub_00214621
  0x00215E71  5f                      pop      edi                            
                                        ; XREF: 0x00215E1A (jump)
  0x00215E72  5e                      pop      esi                            
  0x00215E73  c20800                  ret      8                              

; ============================================================
; Function: sub_00215E76
; Start: 0x00215E76  End: 0x00215EA9  Size: 51 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021427A, sub_00215747
; ============================================================
sub_00215E76:
  0x00215E76  55                      push     ebp                            
  0x00215E77  8bec                    mov      ebp, esp                       
  0x00215E79  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x00215E7C  53                      push     ebx                            
  0x00215E7D  56                      push     esi                            
  0x00215E7E  57                      push     edi                            
  0x00215E7F  e8f6e3ffff              call     0x21427a                       ; -> sub_0021427A
  0x00215E84  68d0a24000              push     0x40a2d0                       
  0x00215E89  8bf8                    mov      edi, eax                       
  0x00215E8B  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x00215E91  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x00215E94  33db                    xor      ebx, ebx                       
  0x00215E96  395e04                  cmp      dword ptr [esi + 4], ebx       
  0x00215E99  7d0e                    jge      0x215ea9                       
                                        ; XREF: 0x00215F0A (cond_jump)
  0x00215E9B  ff750c                  push     dword ptr [ebp + 0xc]          
  0x00215E9E  56                      push     esi                            
  0x00215E9F  e8a3f8ffff              call     0x215747                       ; -> sub_00215747
  0x00215EA4  e9b2000000              jmp      0x215f5b                       
; end of function
                                        ; XREF: 0x00215E99 (cond_jump)
  0x00215EA9  b868a24000              mov      eax, 0x40a268                  
  0x00215EAE  33c9                    xor      ecx, ecx                       
                                        ; XREF: 0x00215EBF (cond_jump)
  0x00215EB0  0fb600                  movzx    eax, byte ptr [eax]            
  0x00215EB3  03c8                    add      ecx, eax                       
  0x00215EB5  8d8168a24000            lea      eax, [ecx + 0x40a268]          
  0x00215EBB  80780105                cmp      byte ptr [eax + 1], 5          
  0x00215EBF  75ef                    jne      0x215eb0                       
  0x00215EC1  6683780404              cmp      word ptr [eax + 4], 4          
  0x00215EC6  7708                    ja       0x215ed0                       
  0x00215EC8  8a4804                  mov      cl, byte ptr [eax + 4]         
  0x00215ECB  884f07                  mov      byte ptr [edi + 7], cl         
  0x00215ECE  eb04                    jmp      0x215ed4                       
                                        ; XREF: 0x00215EC6 (cond_jump)
  0x00215ED0  c6470704                mov      byte ptr [edi + 7], 4          
                                        ; XREF: 0x00215ECE (jump)
  0x00215ED4  8a4802                  mov      cl, byte ptr [eax + 2]         
  0x00215ED7  884f01                  mov      byte ptr [edi + 1], cl         
  0x00215EDA  c60620                  mov      byte ptr [esi], 0x20           
  0x00215EDD  c6460102                mov      byte ptr [esi + 1], 2          
  0x00215EE1  895e08                  mov      dword ptr [esi + 8], ebx       
  0x00215EE4  8a4802                  mov      cl, byte ptr [eax + 2]         
  0x00215EE7  884e15                  mov      byte ptr [esi + 0x15], cl      
  0x00215EEA  8a4003                  mov      al, byte ptr [eax + 3]         
  0x00215EED  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x00215EF0  2403                    and      al, 3                          
  0x00215EF2  884616                  mov      byte ptr [esi + 0x16], al      
  0x00215EF5  c6461710                mov      byte ptr [esi + 0x17], 0x10    
  0x00215EF9  660fb64707              movzx    ax, byte ptr [edi + 7]         
  0x00215EFE  56                      push     esi                            
  0x00215EFF  6689461c                mov      word ptr [esi + 0x1c], ax      
  0x00215F03  e819e7ffff              call     0x214621                       ; -> sub_00214621
  0x00215F08  85c0                    test     eax, eax                       
  0x00215F0A  7c8f                    jl       0x215e9b                       
  0x00215F0C  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x00215F0F  89473c                  mov      dword ptr [edi + 0x3c], eax    
  0x00215F12  8b7d0c                  mov      edi, dword ptr [ebp + 0xc]     
  0x00215F15  53                      push     ebx                            
  0x00215F16  c60630                  mov      byte ptr [esi], 0x30           
  0x00215F19  c6460140                mov      byte ptr [esi + 1], 0x40       
  0x00215F1D  c74608fa5d2100          mov      dword ptr [esi + 8], 0x215dfa  
  0x00215F24  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x00215F27  895e10                  mov      dword ptr [esi + 0x10], ebx    
  0x00215F2A  895e18                  mov      dword ptr [esi + 0x18], ebx    
  0x00215F2D  895e14                  mov      dword ptr [esi + 0x14], ebx    
  0x00215F30  885e1c                  mov      byte ptr [esi + 0x1c], bl      
  0x00215F33  885e1d                  mov      byte ptr [esi + 0x1d], bl      
  0x00215F36  885e1e                  mov      byte ptr [esi + 0x1e], bl      
  0x00215F39  885e28                  mov      byte ptr [esi + 0x28], bl      
  0x00215F3C  c6462909                mov      byte ptr [esi + 0x29], 9       
  0x00215F40  66c7462a0100            mov      word ptr [esi + 0x2a], 1       
  0x00215F46  66895e2c                mov      word ptr [esi + 0x2c], bx      
  0x00215F4A  66895e2e                mov      word ptr [esi + 0x2e], bx      
  0x00215F4E  e88af5ffff              call     0x2154dd                       ; -> sub_002154DD
  0x00215F53  56                      push     esi                            
  0x00215F54  8bcf                    mov      ecx, edi                       
  0x00215F56  e8c6e6ffff              call     0x214621                       ; -> sub_00214621
                                        ; XREF: 0x00215EA4 (jump)
  0x00215F5B  5f                      pop      edi                            
  0x00215F5C  5e                      pop      esi                            
  0x00215F5D  5b                      pop      ebx                            
  0x00215F5E  5d                      pop      ebp                            
  0x00215F5F  c20800                  ret      8                              
  0x00215F62  66a11aa34000            mov      ax, word ptr [0x40a31a]        
  0x00215F68  53                      push     ebx                            
  0x00215F69  56                      push     esi                            
  0x00215F6A  33db                    xor      ebx, ebx                       
  0x00215F6C  33f6                    xor      esi, esi                       
  0x00215F6E  663b0518a34000          cmp      ax, word ptr [0x40a318]        
  0x00215F75  0f83ba000000            jae      0x216035                       
  0x00215F7B  f6051ca3400001          test     byte ptr [0x40a31c], 1         
  0x00215F82  b81ca34000              mov      eax, 0x40a31c                  
  0x00215F87  740b                    je       0x215f94                       
  0x00215F89  8bc8                    mov      ecx, eax                       
                                        ; XREF: 0x00215F92 (cond_jump)
  0x00215F8B  83c140                  add      ecx, 0x40                      
  0x00215F8E  46                      inc      esi                            
  0x00215F8F  f60101                  test     byte ptr [ecx], 1              
  0x00215F92  75f7                    jne      0x215f8b                       
                                        ; XREF: 0x00215F87 (cond_jump)
  0x00215F94  66ff051aa34000          inc      word ptr [0x40a31a]            
  0x00215F9B  55                      push     ebp                            
  0x00215F9C  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x00215FA0  c1e606                  shl      esi, 6                         
  0x00215FA3  57                      push     edi                            
  0x00215FA4  03f0                    add      esi, eax                       
  0x00215FA6  56                      push     esi                            
  0x00215FA7  8bcd                    mov      ecx, ebp                       
  0x00215FA9  e8d0e2ffff              call     0x21427e                       ; -> sub_0021427E
  0x00215FAE  8a06                    mov      al, byte ptr [esi]             
  0x00215FB0  24f5                    and      al, 0xf5                       
  0x00215FB2  8d7e08                  lea      edi, [esi + 8]                 
  0x00215FB5  0c01                    or       al, 1                          
  0x00215FB7  57                      push     edi                            
  0x00215FB8  8bcd                    mov      ecx, ebp                       
  0x00215FBA  8806                    mov      byte ptr [esi], al             
  0x00215FBC  885e05                  mov      byte ptr [esi + 5], bl         
  0x00215FBF  885e06                  mov      byte ptr [esi + 6], bl         
  0x00215FC2  c60720                  mov      byte ptr [edi], 0x20           
  0x00215FC5  c6460982                mov      byte ptr [esi + 9], 0x82       
  0x00215FC9  895e10                  mov      dword ptr [esi + 0x10], ebx    
  0x00215FCC  e850e6ffff              call     0x214621                       ; -> sub_00214621
  0x00215FD1  6a30                    push     0x30                           
  0x00215FD3  58                      pop      eax                            
  0x00215FD4  55                      push     ebp                            
  0x00215FD5  6889542100              push     0x215489                       
  0x00215FDA  68f8a24000              push     0x40a2f8                       
  0x00215FDF  8807                    mov      byte ptr [edi], al             
  0x00215FE1  c6460940                mov      byte ptr [esi + 9], 0x40       
  0x00215FE5  c74610765e2100          mov      dword ptr [esi + 0x10], 0x215e76 
  0x00215FEC  896e14                  mov      dword ptr [esi + 0x14], ebp    
  0x00215FEF  895e18                  mov      dword ptr [esi + 0x18], ebx    
  0x00215FF2  c7462068a24000          mov      dword ptr [esi + 0x20], 0x40a268 
  0x00215FF9  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x00215FFC  c6462402                mov      byte ptr [esi + 0x24], 2       
  0x00216000  c6462501                mov      byte ptr [esi + 0x25], 1       
  0x00216004  885e26                  mov      byte ptr [esi + 0x26], bl      
  0x00216007  c6463080                mov      byte ptr [esi + 0x30], 0x80    
  0x0021600B  c6463106                mov      byte ptr [esi + 0x31], 6       
  0x0021600F  66c746320002            mov      word ptr [esi + 0x32], 0x200   
  0x00216015  66895e34                mov      word ptr [esi + 0x34], bx      
  0x00216019  66894636                mov      word ptr [esi + 0x36], ax      
  0x0021601D  ff15187b2100            call     dword ptr [0x217b18]           ; -> xbox_KeInitializeDpc
  0x00216023  53                      push     ebx                            
  0x00216024  e8b4f4ffff              call     0x2154dd                       ; -> sub_002154DD
  0x00216029  57                      push     edi                            
  0x0021602A  8bcd                    mov      ecx, ebp                       
  0x0021602C  e8f0e5ffff              call     0x214621                       ; -> sub_00214621
  0x00216031  5f                      pop      edi                            
  0x00216032  5d                      pop      ebp                            
  0x00216033  eb0e                    jmp      0x216043                       
                                        ; XREF: 0x00215F75 (cond_jump)
  0x00216035  8b4c240c                mov      ecx, dword ptr [esp + 0xc]     
  0x00216039  6800010080              push     0x80000100                     
  0x0021603E  e83fdbffff              call     0x213b82                       ; -> sub_00213B82
                                        ; XREF: 0x00216033 (jump)
  0x00216043  5e                      pop      esi                            
  0x00216044  5b                      pop      ebx                            
  0x00216045  c20400                  ret      4                              

; ============================================================
; Function: sub_00216048
; Start: 0x00216048  End: 0x002160D7  Size: 143 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00213748, sub_00213A6A
; Called by: sub_00211213, sub_0021616C
; ============================================================
sub_00216048:
  0x00216048  51                      push     ecx                            
  0x00216049  53                      push     ebx                            
  0x0021604A  55                      push     ebp                            
  0x0021604B  56                      push     esi                            
  0x0021604C  8bf1                    mov      esi, ecx                       
  0x0021604E  80be6004000000          cmp      byte ptr [esi + 0x460], 0      
  0x00216055  8bea                    mov      ebp, edx                       
  0x00216057  b301                    mov      bl, 1                          
  0x00216059  7677                    jbe      0x2160d2                       
  0x0021605B  885c240c                mov      byte ptr [esp + 0xc], bl       
  0x0021605F  57                      push     edi                            
                                        ; XREF: 0x002160CF (cond_jump)
  0x00216060  660fb6c3                movzx    ax, bl                         
  0x00216064  66854500                test     word ptr [ebp], ax             
  0x00216068  7453                    je       0x2160bd                       
  0x0021606A  33c9                    xor      ecx, ecx                       
  0x0021606C  668b4d02                mov      cx, word ptr [ebp + 2]         
  0x00216070  23c8                    and      ecx, eax                       
  0x00216072  6685c9                  test     cx, cx                         
  0x00216075  7428                    je       0x21609f                       
  0x00216077  8d8e61040000            lea      ecx, [esi + 0x461]             
  0x0021607D  8a01                    mov      al, byte ptr [ecx]             
  0x0021607F  84c3                    test     bl, al                         
  0x00216081  740c                    je       0x21608f                       
  0x00216083  ff742410                push     dword ptr [esp + 0x10]         
  0x00216087  56                      push     esi                            
  0x00216088  e8ddd9ffff              call     0x213a6a                       ; -> sub_00213A6A
  0x0021608D  eb04                    jmp      0x216093                       
                                        ; XREF: 0x00216081 (cond_jump)
  0x0021608F  0ac3                    or       al, bl                         
  0x00216091  8801                    mov      byte ptr [ecx], al             
                                        ; XREF: 0x0021608D (jump)
  0x00216093  ff742410                push     dword ptr [esp + 0x10]         
  0x00216097  56                      push     esi                            
  0x00216098  e8abd6ffff              call     0x213748                       ; -> sub_00213748
  0x0021609D  eb1e                    jmp      0x2160bd                       
                                        ; XREF: 0x00216075 (cond_jump)
  0x0021609F  8dbe61040000            lea      edi, [esi + 0x461]             
  0x002160A5  8a07                    mov      al, byte ptr [edi]             
  0x002160A7  84c3                    test     bl, al                         
  0x002160A9  7412                    je       0x2160bd                       
  0x002160AB  ff742410                push     dword ptr [esp + 0x10]         
  0x002160AF  8acb                    mov      cl, bl                         
  0x002160B1  f6d1                    not      cl                             
  0x002160B3  22c8                    and      cl, al                         
  0x002160B5  56                      push     esi                            
  0x002160B6  880f                    mov      byte ptr [edi], cl             
  0x002160B8  e8add9ffff              call     0x213a6a                       ; -> sub_00213A6A
                                        ; XREF: 0x00216068 (cond_jump), 0x0021609D (jump), 0x002160A9 (cond_jump)
  0x002160BD  fe442410                inc      byte ptr [esp + 0x10]          
  0x002160C1  8a442410                mov      al, byte ptr [esp + 0x10]      
  0x002160C5  d0e3                    shl      bl, 1                          
  0x002160C7  fec8                    dec      al                             
  0x002160C9  3a8660040000            cmp      al, byte ptr [esi + 0x460]     
  0x002160CF  728f                    jb       0x216060                       
  0x002160D1  5f                      pop      edi                            
                                        ; XREF: 0x00216059 (cond_jump)
  0x002160D2  5e                      pop      esi                            
  0x002160D3  5d                      pop      ebp                            
  0x002160D4  5b                      pop      ebx                            
  0x002160D5  59                      pop      ecx                            
  0x002160D6  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002160D7
; Start: 0x002160D7  End: 0x00216124  Size: 77 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_002160D7:
  0x002160D7  55                      push     ebp                            
  0x002160D8  8bec                    mov      ebp, esp                       
  0x002160DA  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x002160DD  8b4d10                  mov      ecx, dword ptr [ebp + 0x10]    
  0x002160E0  8b550c                  mov      edx, dword ptr [ebp + 0xc]     
  0x002160E3  898870040000            mov      dword ptr [eax + 0x470], ecx   
  0x002160E9  8b4d14                  mov      ecx, dword ptr [ebp + 0x14]    
  0x002160EC  56                      push     esi                            
  0x002160ED  898874040000            mov      dword ptr [eax + 0x474], ecx   
  0x002160F3  8b08                    mov      ecx, dword ptr [eax]           
  0x002160F5  66c745081000            mov      word ptr [ebp + 8], 0x10       
  0x002160FB  8b7508                  mov      esi, dword ptr [ebp + 8]       
  0x002160FE  89749150                mov      dword ptr [ecx + edx*4 + 0x50], esi 
  0x00216102  8db0a0040000            lea      esi, [eax + 0x4a0]             
  0x00216108  56                      push     esi                            
  0x00216109  83caff                  or       edx, 0xffffffff                
  0x0021610C  52                      push     edx                            
  0x0021610D  b9c0bdf0ff              mov      ecx, 0xfff0bdc0                
  0x00216112  51                      push     ecx                            
  0x00216113  0578040000              add      eax, 0x478                     
  0x00216118  50                      push     eax                            
  0x00216119  ff152c7b2100            call     dword ptr [0x217b2c]           ; -> xbox_KeSetTimer
  0x0021611F  5e                      pop      esi                            
  0x00216120  5d                      pop      ebp                            
  0x00216121  c21000                  ret      0x10                           
; end of function

; ============================================================
; Function: sub_00216124
; Start: 0x00216124  End: 0x00216141  Size: 29 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_00213AB0
; ============================================================
sub_00216124:
  0x00216124  55                      push     ebp                            
  0x00216125  8bec                    mov      ebp, esp                       
  0x00216127  51                      push     ecx                            
  0x00216128  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x0021612B  8b00                    mov      eax, dword ptr [eax]           
  0x0021612D  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x00216130  66c745fc0100            mov      word ptr [ebp - 4], 1          
  0x00216136  8b55fc                  mov      edx, dword ptr [ebp - 4]       
  0x00216139  89548850                mov      dword ptr [eax + ecx*4 + 0x50], edx 
  0x0021613D  c9                      leave                                   
  0x0021613E  c20800                  ret      8                              
; end of function
  0x00216141  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x00216145  8d8170040000            lea      eax, [ecx + 0x470]             
  0x0021614B  8b10                    mov      edx, dword ptr [eax]           
  0x0021614D  81c174040000            add      ecx, 0x474                     
  0x00216153  85d2                    test     edx, edx                       
  0x00216155  56                      push     esi                            
  0x00216156  8b31                    mov      esi, dword ptr [ecx]           
  0x00216158  740e                    je       0x216168                       
  0x0021615A  832000                  and      dword ptr [eax], 0             
  0x0021615D  832100                  and      dword ptr [ecx], 0             
  0x00216160  56                      push     esi                            
  0x00216161  6800060080              push     0x80000600                     
  0x00216166  ffd2                    call     edx                            
                                        ; XREF: 0x00216158 (cond_jump)
  0x00216168  5e                      pop      esi                            
  0x00216169  c21000                  ret      0x10                           

; ============================================================
; Function: sub_0021616C
; Start: 0x0021616C  End: 0x0021622B  Size: 191 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00216048
; Called by: sub_00216C78
; ============================================================
sub_0021616C:
  0x0021616C  55                      push     ebp                            
  0x0021616D  8bec                    mov      ebp, esp                       
  0x0021616F  83ec1c                  sub      esp, 0x1c                      
  0x00216172  56                      push     esi                            
  0x00216173  8bf1                    mov      esi, ecx                       
  0x00216175  8b06                    mov      eax, dword ptr [esi]           
  0x00216177  0fb69660040000          movzx    edx, byte ptr [esi + 0x460]    
  0x0021617E  8d4854                  lea      ecx, [eax + 0x54]              
  0x00216181  8b01                    mov      eax, dword ptr [ecx]           
  0x00216183  57                      push     edi                            
  0x00216184  33c0                    xor      eax, eax                       
  0x00216186  85d2                    test     edx, edx                       
  0x00216188  8d7df8                  lea      edi, [ebp - 8]                 
  0x0021618B  ab                      stosd    dword ptr es:[edi], eax        
  0x0021618C  c745f401000000          mov      dword ptr [ebp - 0xc], 1       
  0x00216193  0f8684000000            jbe      0x21621d                       
  0x00216199  894df0                  mov      dword ptr [ebp - 0x10], ecx    
  0x0021619C  8955ec                  mov      dword ptr [ebp - 0x14], edx    
  0x0021619F  53                      push     ebx                            
                                        ; XREF: 0x0021621A (cond_jump)
  0x002161A0  8b39                    mov      edi, dword ptr [ecx]           
  0x002161A2  897dfc                  mov      dword ptr [ebp - 4], edi       
  0x002161A5  f645fe10                test     byte ptr [ebp - 2], 0x10       
  0x002161A9  743f                    je       0x2161ea                       
  0x002161AB  8b8e74040000            mov      ecx, dword ptr [esi + 0x474]   
  0x002161B1  8d9e70040000            lea      ebx, [esi + 0x470]             
  0x002161B7  8b03                    mov      eax, dword ptr [ebx]           
  0x002161B9  85c0                    test     eax, eax                       
  0x002161BB  8945e4                  mov      dword ptr [ebp - 0x1c], eax    
  0x002161BE  894de8                  mov      dword ptr [ebp - 0x18], ecx    
  0x002161C1  7427                    je       0x2161ea                       
  0x002161C3  8d8678040000            lea      eax, [esi + 0x478]             
  0x002161C9  50                      push     eax                            
  0x002161CA  ff15307b2100            call     dword ptr [0x217b30]           ; -> xbox_KeBugCheck
  0x002161D0  ff75e8                  push     dword ptr [ebp - 0x18]         
  0x002161D3  832300                  and      dword ptr [ebx], 0             
  0x002161D6  83a67404000000          and      dword ptr [esi + 0x474], 0     
  0x002161DD  81e700020000            and      edi, 0x200                     
  0x002161E3  c1e70f                  shl      edi, 0xf                       
  0x002161E6  57                      push     edi                            
  0x002161E7  ff55e4                  call     dword ptr [ebp - 0x1c]         
                                        ; XREF: 0x002161A9 (cond_jump), 0x002161C1 (cond_jump)
  0x002161EA  f645fe01                test     byte ptr [ebp - 2], 1          
  0x002161EE  7411                    je       0x216201                       
  0x002161F0  8b45f4                  mov      eax, dword ptr [ebp - 0xc]     
  0x002161F3  660945f8                or       word ptr [ebp - 8], ax         
  0x002161F7  f645fc01                test     byte ptr [ebp - 4], 1          
  0x002161FB  7404                    je       0x216201                       
  0x002161FD  660945fa                or       word ptr [ebp - 6], ax         
                                        ; XREF: 0x002161EE (cond_jump), 0x002161FB (cond_jump)
  0x00216201  668365fc00              and      word ptr [ebp - 4], 0          
  0x00216206  8b4df0                  mov      ecx, dword ptr [ebp - 0x10]    
  0x00216209  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x0021620C  d165f4                  shl      dword ptr [ebp - 0xc], 1       
  0x0021620F  8901                    mov      dword ptr [ecx], eax           
  0x00216211  83c104                  add      ecx, 4                         
  0x00216214  ff4dec                  dec      dword ptr [ebp - 0x14]         
  0x00216217  894df0                  mov      dword ptr [ebp - 0x10], ecx    
  0x0021621A  7584                    jne      0x2161a0                       
  0x0021621C  5b                      pop      ebx                            
                                        ; XREF: 0x00216193 (cond_jump)
  0x0021621D  8d55f8                  lea      edx, [ebp - 8]                 
  0x00216220  8bce                    mov      ecx, esi                       
  0x00216222  e821feffff              call     0x216048                       ; -> sub_00216048
  0x00216227  5f                      pop      edi                            
  0x00216228  5e                      pop      esi                            
  0x00216229  c9                      leave                                   
  0x0021622A  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_0021622B
; Start: 0x0021622B  End: 0x00216248  Size: 29 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002112C3, sub_00216913, sub_00216AC1
; ============================================================
sub_0021622B:
  0x0021622B  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021622F  834808ff                or       dword ptr [eax + 8], 0xffffffff 
  0x00216233  80481fff                or       byte ptr [eax + 0x1f], 0xff    
  0x00216237  8b0dacb9c600            mov      ecx, dword ptr [0xc6b9ac]      
  0x0021623D  894814                  mov      dword ptr [eax + 0x14], ecx    
  0x00216240  a3acb9c600              mov      dword ptr [0xc6b9ac], eax      
  0x00216245  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00216248
; Start: 0x00216248  End: 0x0021625C  Size: 20 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00216248:
  0x00216248  807a1100                cmp      byte ptr [edx + 0x11], 0       
  0x0021624C  8b01                    mov      eax, dword ptr [ecx]           
  0x0021624E  56                      push     esi                            
  0x0021624F  750b                    jne      0x21625c                       
  0x00216251  8db10c040000            lea      esi, [ecx + 0x40c]             
  0x00216257  83c020                  add      eax, 0x20                      
  0x0021625A  eb09                    jmp      0x216265                       
; end of function
                                        ; XREF: 0x0021624F (cond_jump)
  0x0021625C  8db110040000            lea      esi, [ecx + 0x410]             
  0x00216262  83c028                  add      eax, 0x28                      
                                        ; XREF: 0x0021625A (jump)
  0x00216265  8b0e                    mov      ecx, dword ptr [esi]           
  0x00216267  894a18                  mov      dword ptr [edx + 0x18], ecx    
  0x0021626A  8916                    mov      dword ptr [esi], edx           
  0x0021626C  8b4a18                  mov      ecx, dword ptr [edx + 0x18]    
  0x0021626F  85c9                    test     ecx, ecx                       
  0x00216271  5e                      pop      esi                            
  0x00216272  7505                    jne      0x216279                       
  0x00216274  214a0c                  and      dword ptr [edx + 0xc], ecx     
  0x00216277  eb06                    jmp      0x21627f                       
                                        ; XREF: 0x00216272 (cond_jump)
  0x00216279  8b4914                  mov      ecx, dword ptr [ecx + 0x14]    
  0x0021627C  894a0c                  mov      dword ptr [edx + 0xc], ecx     
                                        ; XREF: 0x00216277 (jump)
  0x0021627F  8b4a14                  mov      ecx, dword ptr [edx + 0x14]    
  0x00216282  8908                    mov      dword ptr [eax], ecx           
  0x00216284  c3                      ret                                     

; ============================================================
; Function: sub_00216285
; Start: 0x00216285  End: 0x0021629A  Size: 21 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00216285:
  0x00216285  807a1100                cmp      byte ptr [edx + 0x11], 0       
  0x00216289  56                      push     esi                            
  0x0021628A  57                      push     edi                            
  0x0021628B  750d                    jne      0x21629a                       
  0x0021628D  8db10c040000            lea      esi, [ecx + 0x40c]             
  0x00216293  8b09                    mov      ecx, dword ptr [ecx]           
  0x00216295  83c120                  add      ecx, 0x20                      
  0x00216298  eb0b                    jmp      0x2162a5                       
; end of function
                                        ; XREF: 0x0021628B (cond_jump)
  0x0021629A  8db110040000            lea      esi, [ecx + 0x410]             
  0x002162A0  8b09                    mov      ecx, dword ptr [ecx]           
  0x002162A2  83c128                  add      ecx, 0x28                      
                                        ; XREF: 0x00216298 (jump)
  0x002162A5  8b06                    mov      eax, dword ptr [esi]           
  0x002162A7  33ff                    xor      edi, edi                       
                                        ; XREF: 0x002162B4 (cond_jump)
  0x002162A9  3bd0                    cmp      edx, eax                       
  0x002162AB  7409                    je       0x2162b6                       
  0x002162AD  8bf8                    mov      edi, eax                       
  0x002162AF  8b4018                  mov      eax, dword ptr [eax + 0x18]    
  0x002162B2  85c0                    test     eax, eax                       
  0x002162B4  75f3                    jne      0x2162a9                       
                                        ; XREF: 0x002162AB (cond_jump)
  0x002162B6  85ff                    test     edi, edi                       
  0x002162B8  740e                    je       0x2162c8                       
  0x002162BA  8b4818                  mov      ecx, dword ptr [eax + 0x18]    
  0x002162BD  894f18                  mov      dword ptr [edi + 0x18], ecx    
  0x002162C0  8b400c                  mov      eax, dword ptr [eax + 0xc]     
  0x002162C3  89470c                  mov      dword ptr [edi + 0xc], eax     
  0x002162C6  eb0a                    jmp      0x2162d2                       
                                        ; XREF: 0x002162B8 (cond_jump)
  0x002162C8  8b5018                  mov      edx, dword ptr [eax + 0x18]    
  0x002162CB  8916                    mov      dword ptr [esi], edx           
  0x002162CD  8b400c                  mov      eax, dword ptr [eax + 0xc]     
  0x002162D0  8901                    mov      dword ptr [ecx], eax           
                                        ; XREF: 0x002162C6 (jump)
  0x002162D2  5f                      pop      edi                            
  0x002162D3  5e                      pop      esi                            
  0x002162D4  c3                      ret                                     

; ============================================================
; Function: sub_002162D5
; Start: 0x002162D5  End: 0x002162EB  Size: 22 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002162EB
; ============================================================
sub_002162D5:
  0x002162D5  33c0                    xor      eax, eax                       
  0x002162D7  85c9                    test     ecx, ecx                       
  0x002162D9  760f                    jbe      0x2162ea                       
  0x002162DB  56                      push     esi                            
                                        ; XREF: 0x002162E7 (cond_jump)
  0x002162DC  8bf2                    mov      esi, edx                       
  0x002162DE  83e601                  and      esi, 1                         
  0x002162E1  d1ea                    shr      edx, 1                         
  0x002162E3  49                      dec      ecx                            
  0x002162E4  8d0446                  lea      eax, [esi + eax*2]             
  0x002162E7  75f3                    jne      0x2162dc                       
  0x002162E9  5e                      pop      esi                            
                                        ; XREF: 0x002162D9 (cond_jump)
  0x002162EA  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002162EB
; Start: 0x002162EB  End: 0x00216313  Size: 40 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_002162D5
; Called by: sub_0021653E
; ============================================================
sub_002162EB:
  0x002162EB  55                      push     ebp                            
  0x002162EC  8bec                    mov      ebp, esp                       
  0x002162EE  51                      push     ecx                            
  0x002162EF  56                      push     esi                            
  0x002162F0  8bf1                    mov      esi, ecx                       
  0x002162F2  8a4d08                  mov      cl, byte ptr [ebp + 8]         
  0x002162F5  80f920                  cmp      cl, 0x20                       
  0x002162F8  57                      push     edi                            
  0x002162F9  8bfa                    mov      edi, edx                       
  0x002162FB  7216                    jb       0x216313                       
  0x002162FD  0fb6d1                  movzx    edx, cl                        
  0x00216300  6a05                    push     5                              
  0x00216302  83ea20                  sub      edx, 0x20                      
  0x00216305  59                      pop      ecx                            
  0x00216306  e8caffffff              call     0x2162d5                       ; -> sub_002162D5
  0x0021630B  8b4e08                  mov      ecx, dword ptr [esi + 8]       
  0x0021630E  893c81                  mov      dword ptr [ecx + eax*4], edi   
  0x00216311  eb49                    jmp      0x21635c                       
; end of function
                                        ; XREF: 0x002162FB (cond_jump)
  0x00216313  80650b00                and      byte ptr [ebp + 0xb], 0        
  0x00216317  8ac1                    mov      al, cl                         
  0x00216319  d0e0                    shl      al, 1                          
  0x0021631B  53                      push     ebx                            
  0x0021631C  fec0                    inc      al                             
  0x0021631E  8ad9                    mov      bl, cl                         
  0x00216320  8845fc                  mov      byte ptr [ebp - 4], al         
  0x00216323  d0e3                    shl      bl, 1                          
                                        ; XREF: 0x00216359 (cond_jump)
  0x00216325  0fb6c0                  movzx    eax, al                        
  0x00216328  c1e004                  shl      eax, 4                         
  0x0021632B  8d44300c                lea      eax, [eax + esi + 0xc]         
  0x0021632F  83780800                cmp      dword ptr [eax + 8], 0         
  0x00216333  750e                    jne      0x216343                       
  0x00216335  ff75fc                  push     dword ptr [ebp - 4]            
  0x00216338  8bd7                    mov      edx, edi                       
  0x0021633A  8bce                    mov      ecx, esi                       
  0x0021633C  e8aaffffff              call     0x2162eb                       ; -> sub_002162EB
  0x00216341  eb06                    jmp      0x216349                       
                                        ; XREF: 0x00216333 (cond_jump)
  0x00216343  8b400c                  mov      eax, dword ptr [eax + 0xc]     
  0x00216346  89780c                  mov      dword ptr [eax + 0xc], edi     
                                        ; XREF: 0x00216341 (jump)
  0x00216349  8ac3                    mov      al, bl                         
  0x0021634B  84c0                    test     al, al                         
  0x0021634D  8845fc                  mov      byte ptr [ebp - 4], al         
  0x00216350  7409                    je       0x21635b                       
  0x00216352  fe450b                  inc      byte ptr [ebp + 0xb]           
  0x00216355  807d0b02                cmp      byte ptr [ebp + 0xb], 2        
  0x00216359  72ca                    jb       0x216325                       
                                        ; XREF: 0x00216350 (cond_jump)
  0x0021635B  5b                      pop      ebx                            
                                        ; XREF: 0x00216311 (jump)
  0x0021635C  5f                      pop      edi                            
  0x0021635D  5e                      pop      esi                            
  0x0021635E  c9                      leave                                   
  0x0021635F  c20400                  ret      4                              

; ============================================================
; Function: sub_00216362
; Start: 0x00216362  End: 0x002163F3  Size: 145 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_00216362:
  0x00216362  55                      push     ebp                            
  0x00216363  8bec                    mov      ebp, esp                       
  0x00216365  83ec10                  sub      esp, 0x10                      
  0x00216368  53                      push     ebx                            
  0x00216369  56                      push     esi                            
  0x0021636A  8bf2                    mov      esi, edx                       
  0x0021636C  807e1103                cmp      byte ptr [esi + 0x11], 3       
  0x00216370  57                      push     edi                            
  0x00216371  7553                    jne      0x2163c6                       
  0x00216373  b020                    mov      al, 0x20                       
  0x00216375  384613                  cmp      byte ptr [esi + 0x13], al      
  0x00216378  7309                    jae      0x216383                       
  0x0021637A  8a5613                  mov      dl, byte ptr [esi + 0x13]      
                                        ; XREF: 0x00216381 (cond_jump)
  0x0021637D  d0e8                    shr      al, 1                          
  0x0021637F  3ac2                    cmp      al, dl                         
  0x00216381  77fa                    ja       0x21637d                       
                                        ; XREF: 0x00216378 (cond_jump)
  0x00216383  8ad8                    mov      bl, al                         
  0x00216385  d0e3                    shl      bl, 1                          
  0x00216387  fecb                    dec      bl                             
  0x00216389  3ac3                    cmp      al, bl                         
  0x0021638B  bfe02e0000              mov      edi, 0x2ee0                    
  0x00216390  8845ff                  mov      byte ptr [ebp - 1], al         
  0x00216393  773d                    ja       0x2163d2                       
  0x00216395  0fb6c0                  movzx    eax, al                        
  0x00216398  c1e004                  shl      eax, 4                         
  0x0021639B  8d540810                lea      edx, [eax + ecx + 0x10]        
                                        ; XREF: 0x002163C2 (cond_jump)
  0x0021639F  33c0                    xor      eax, eax                       
  0x002163A1  668b42fe                mov      ax, word ptr [edx - 2]         
  0x002163A5  66034202                add      ax, word ptr [edx + 2]         
  0x002163A9  660302                  add      ax, word ptr [edx]             
  0x002163AC  663bc7                  cmp      ax, di                         
  0x002163AF  7308                    jae      0x2163b9                       
  0x002163B1  8bf8                    mov      edi, eax                       
  0x002163B3  8a45ff                  mov      al, byte ptr [ebp - 1]         
  0x002163B6  8845fb                  mov      byte ptr [ebp - 5], al         
                                        ; XREF: 0x002163AF (cond_jump)
  0x002163B9  fe45ff                  inc      byte ptr [ebp - 1]             
  0x002163BC  83c210                  add      edx, 0x10                      
  0x002163BF  385dff                  cmp      byte ptr [ebp - 1], bl         
  0x002163C2  76db                    jbe      0x21639f                       
  0x002163C4  eb0c                    jmp      0x2163d2                       
                                        ; XREF: 0x00216371 (cond_jump)
  0x002163C6  668b7910                mov      di, word ptr [ecx + 0x10]      
  0x002163CA  6603790e                add      di, word ptr [ecx + 0xe]       
  0x002163CE  8065fb00                and      byte ptr [ebp - 5], 0          
                                        ; XREF: 0x00216393 (cond_jump), 0x002163C4 (jump)
  0x002163D2  668b4622                mov      ax, word ptr [esi + 0x22]      
  0x002163D6  0fb7ff                  movzx    edi, di                        
  0x002163D9  0fb7d0                  movzx    edx, ax                        
  0x002163DC  03d7                    add      edx, edi                       
  0x002163DE  0fb7b914040000          movzx    edi, word ptr [ecx + 0x414]    
  0x002163E5  3bd7                    cmp      edx, edi                       
  0x002163E7  7e0a                    jle      0x2163f3                       
  0x002163E9  b800080080              mov      eax, 0x80000800                
  0x002163EE  e946010000              jmp      0x216539                       
; end of function
                                        ; XREF: 0x002163E7 (cond_jump)
  0x002163F3  8a55fb                  mov      dl, byte ptr [ebp - 5]         
  0x002163F6  0fb6fa                  movzx    edi, dl                        
  0x002163F9  c1e704                  shl      edi, 4                         
  0x002163FC  8d7c0f0c                lea      edi, [edi + ecx + 0xc]         
  0x00216400  885612                  mov      byte ptr [esi + 0x12], dl      
  0x00216403  66014702                add      word ptr [edi + 2], ax         
  0x00216407  84d2                    test     dl, dl                         
  0x00216409  7507                    jne      0x216412                       
  0x0021640B  b001                    mov      al, 1                          
  0x0021640D  8845ff                  mov      byte ptr [ebp - 1], al         
  0x00216410  eb11                    jmp      0x216423                       
                                        ; XREF: 0x00216409 (cond_jump)
  0x00216412  8ac2                    mov      al, dl                         
  0x00216414  d0e0                    shl      al, 1                          
  0x00216416  8845ff                  mov      byte ptr [ebp - 1], al         
  0x00216419  8ac2                    mov      al, dl                         
  0x0021641B  d0e0                    shl      al, 1                          
  0x0021641D  fec0                    inc      al                             
  0x0021641F  3c40                    cmp      al, 0x40                       
  0x00216421  7740                    ja       0x216463                       
                                        ; XREF: 0x00216410 (jump), 0x0021645E (cond_jump)
  0x00216423  3845ff                  cmp      byte ptr [ebp - 1], al         
  0x00216426  772d                    ja       0x216455                       
  0x00216428  0fb655ff                movzx    edx, byte ptr [ebp - 1]        
  0x0021642C  c1e204                  shl      edx, 4                         
  0x0021642F  8d540a12                lea      edx, [edx + ecx + 0x12]        
  0x00216433  8955f0                  mov      dword ptr [ebp - 0x10], edx    
  0x00216436  8ad0                    mov      dl, al                         
  0x00216438  2a55ff                  sub      dl, byte ptr [ebp - 1]         
  0x0021643B  fec2                    inc      dl                             
  0x0021643D  0fb6d2                  movzx    edx, dl                        
  0x00216440  8955f4                  mov      dword ptr [ebp - 0xc], edx     
  0x00216443  8b55f0                  mov      edx, dword ptr [ebp - 0x10]    
                                        ; XREF: 0x00216453 (cond_jump)
  0x00216446  668b5e22                mov      bx, word ptr [esi + 0x22]      
  0x0021644A  66011a                  add      word ptr [edx], bx             
  0x0021644D  83c210                  add      edx, 0x10                      
  0x00216450  ff4df4                  dec      dword ptr [ebp - 0xc]          
  0x00216453  75f1                    jne      0x216446                       
                                        ; XREF: 0x00216426 (cond_jump)
  0x00216455  d065ff                  shl      byte ptr [ebp - 1], 1          
  0x00216458  d0e0                    shl      al, 1                          
  0x0021645A  fec0                    inc      al                             
  0x0021645C  3c40                    cmp      al, 0x40                       
  0x0021645E  76c3                    jbe      0x216423                       
  0x00216460  8a55fb                  mov      dl, byte ptr [ebp - 5]         
                                        ; XREF: 0x00216421 (cond_jump)
  0x00216463  80fa01                  cmp      dl, 1                          
  0x00216466  8855ff                  mov      byte ptr [ebp - 1], dl         
  0x00216469  7661                    jbe      0x2164cc                       
                                        ; XREF: 0x002164CA (cond_jump)
  0x0021646B  8a45ff                  mov      al, byte ptr [ebp - 1]         
  0x0021646E  0fb655ff                movzx    edx, byte ptr [ebp - 1]        
  0x00216472  3401                    xor      al, 1                          
  0x00216474  0fb6c0                  movzx    eax, al                        
  0x00216477  c1e204                  shl      edx, 4                         
  0x0021647A  8d540a0c                lea      edx, [edx + ecx + 0xc]         
  0x0021647E  c1e004                  shl      eax, 4                         
  0x00216481  33db                    xor      ebx, ebx                       
  0x00216483  668b5a04                mov      bx, word ptr [edx + 4]         
  0x00216487  668b5202                mov      dx, word ptr [edx + 2]         
  0x0021648B  8d44080c                lea      eax, [eax + ecx + 0xc]         
  0x0021648F  668955f4                mov      word ptr [ebp - 0xc], dx       
  0x00216493  0fb75004                movzx    edx, word ptr [eax + 4]        
  0x00216497  0fb74002                movzx    eax, word ptr [eax + 2]        
  0x0021649B  03d0                    add      edx, eax                       
  0x0021649D  0fb745f4                movzx    eax, word ptr [ebp - 0xc]      
  0x002164A1  895df0                  mov      dword ptr [ebp - 0x10], ebx    
  0x002164A4  0fb7db                  movzx    ebx, bx                        
  0x002164A7  03c3                    add      eax, ebx                       
  0x002164A9  3bc2                    cmp      eax, edx                       
  0x002164AB  7e1f                    jle      0x2164cc                       
  0x002164AD  8a45ff                  mov      al, byte ptr [ebp - 1]         
  0x002164B0  8b5df0                  mov      ebx, dword ptr [ebp - 0x10]    
  0x002164B3  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
  0x002164B6  d0e8                    shr      al, 1                          
  0x002164B8  03d3                    add      edx, ebx                       
  0x002164BA  0fb6d8                  movzx    ebx, al                        
  0x002164BD  c1e304                  shl      ebx, 4                         
  0x002164C0  3c01                    cmp      al, 1                          
  0x002164C2  6689540b10              mov      word ptr [ebx + ecx + 0x10], dx 
  0x002164C7  8845ff                  mov      byte ptr [ebp - 1], al         
  0x002164CA  779f                    ja       0x21646b                       
                                        ; XREF: 0x00216469 (cond_jump), 0x002164AB (cond_jump)
  0x002164CC  807dff01                cmp      byte ptr [ebp - 1], 1          
  0x002164D0  750c                    jne      0x2164de                       
  0x002164D2  668b4120                mov      ax, word ptr [ecx + 0x20]      
  0x002164D6  6603411e                add      ax, word ptr [ecx + 0x1e]      
  0x002164DA  66894110                mov      word ptr [ecx + 0x10], ax      
                                        ; XREF: 0x002164D0 (cond_jump)
  0x002164DE  8b4708                  mov      eax, dword ptr [edi + 8]       
  0x002164E1  33d2                    xor      edx, edx                       
  0x002164E3  3bc2                    cmp      eax, edx                       
  0x002164E5  7536                    jne      0x21651d                       
  0x002164E7  8a45fb                  mov      al, byte ptr [ebp - 5]         
  0x002164EA  eb04                    jmp      0x2164f0                       
                                        ; XREF: 0x002164FC (cond_jump)
  0x002164EC  84c0                    test     al, al                         
  0x002164EE  740e                    je       0x2164fe                       
                                        ; XREF: 0x002164EA (jump)
  0x002164F0  d0e8                    shr      al, 1                          
  0x002164F2  0fb6d8                  movzx    ebx, al                        
  0x002164F5  c1e304                  shl      ebx, 4                         
  0x002164F8  39540b14                cmp      dword ptr [ebx + ecx + 0x14], edx 
  0x002164FC  74ee                    je       0x2164ec                       
                                        ; XREF: 0x002164EE (cond_jump)
  0x002164FE  0fb6c0                  movzx    eax, al                        
  0x00216501  c1e004                  shl      eax, 4                         
  0x00216504  8b440814                mov      eax, dword ptr [eax + ecx + 0x14] 
  0x00216508  3bc2                    cmp      eax, edx                       
  0x0021650A  7406                    je       0x216512                       
  0x0021650C  8b4014                  mov      eax, dword ptr [eax + 0x14]    
  0x0021650F  89460c                  mov      dword ptr [esi + 0xc], eax     
                                        ; XREF: 0x0021650A (cond_jump)
  0x00216512  897708                  mov      dword ptr [edi + 8], esi       
  0x00216515  89770c                  mov      dword ptr [edi + 0xc], esi     
  0x00216518  895618                  mov      dword ptr [esi + 0x18], edx    
  0x0021651B  eb0f                    jmp      0x21652c                       
                                        ; XREF: 0x002164E5 (cond_jump)
  0x0021651D  894618                  mov      dword ptr [esi + 0x18], eax    
  0x00216520  897708                  mov      dword ptr [edi + 8], esi       
  0x00216523  8b4618                  mov      eax, dword ptr [esi + 0x18]    
  0x00216526  8b4014                  mov      eax, dword ptr [eax + 0x14]    
  0x00216529  89460c                  mov      dword ptr [esi + 0xc], eax     
                                        ; XREF: 0x0021651B (jump)
  0x0021652C  ff75fb                  push     dword ptr [ebp - 5]            
  0x0021652F  8b5614                  mov      edx, dword ptr [esi + 0x14]    
  0x00216532  e8b4fdffff              call     0x2162eb                       ; -> sub_002162EB
  0x00216537  33c0                    xor      eax, eax                       
                                        ; XREF: 0x002163EE (jump)
  0x00216539  5f                      pop      edi                            
  0x0021653A  5e                      pop      esi                            
  0x0021653B  5b                      pop      ebx                            
  0x0021653C  c9                      leave                                   
  0x0021653D  c3                      ret                                     

; ============================================================
; Function: sub_0021653E
; Start: 0x0021653E  End: 0x002165D5  Size: 151 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_002162EB
; Called by: sub_002152B7, sub_00216F45
; ============================================================
sub_0021653E:
  0x0021653E  55                      push     ebp                            
  0x0021653F  8bec                    mov      ebp, esp                       
  0x00216541  83ec14                  sub      esp, 0x14                      
  0x00216544  668b4222                mov      ax, word ptr [edx + 0x22]      
  0x00216548  8365fc00                and      dword ptr [ebp - 4], 0         
  0x0021654C  53                      push     ebx                            
  0x0021654D  8a5a12                  mov      bl, byte ptr [edx + 0x12]      
  0x00216550  668945f0                mov      word ptr [ebp - 0x10], ax      
  0x00216554  0fb6c3                  movzx    eax, bl                        
  0x00216557  56                      push     esi                            
  0x00216558  c1e004                  shl      eax, 4                         
  0x0021655B  8bf1                    mov      esi, ecx                       
  0x0021655D  57                      push     edi                            
  0x0021655E  8d7c300c                lea      edi, [eax + esi + 0xc]         
  0x00216562  8b4708                  mov      eax, dword ptr [edi + 8]       
  0x00216565  8955ec                  mov      dword ptr [ebp - 0x14], edx    
  0x00216568  885df4                  mov      byte ptr [ebp - 0xc], bl       
                                        ; XREF: 0x00216577 (cond_jump)
  0x0021656B  3bc2                    cmp      eax, edx                       
  0x0021656D  740a                    je       0x216579                       
  0x0021656F  8945fc                  mov      dword ptr [ebp - 4], eax       
  0x00216572  8b4018                  mov      eax, dword ptr [eax + 0x18]    
  0x00216575  85c0                    test     eax, eax                       
  0x00216577  75f2                    jne      0x21656b                       
                                        ; XREF: 0x0021656D (cond_jump)
  0x00216579  8b4818                  mov      ecx, dword ptr [eax + 0x18]    
  0x0021657C  85c9                    test     ecx, ecx                       
  0x0021657E  7534                    jne      0x2165b4                       
  0x00216580  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x00216583  33d2                    xor      edx, edx                       
  0x00216585  84db                    test     bl, bl                         
  0x00216587  894f0c                  mov      dword ptr [edi + 0xc], ecx     
  0x0021658A  8955f8                  mov      dword ptr [ebp - 8], edx       
  0x0021658D  742d                    je       0x2165bc                       
  0x0021658F  8ad3                    mov      dl, bl                         
  0x00216591  eb04                    jmp      0x216597                       
                                        ; XREF: 0x002165A4 (cond_jump)
  0x00216593  84d2                    test     dl, dl                         
  0x00216595  740f                    je       0x2165a6                       
                                        ; XREF: 0x00216591 (jump)
  0x00216597  d0ea                    shr      dl, 1                          
  0x00216599  0fb6ca                  movzx    ecx, dl                        
  0x0021659C  c1e104                  shl      ecx, 4                         
  0x0021659F  837c311400              cmp      dword ptr [ecx + esi + 0x14], 0 
  0x002165A4  74ed                    je       0x216593                       
                                        ; XREF: 0x00216595 (cond_jump)
  0x002165A6  0fb6ca                  movzx    ecx, dl                        
  0x002165A9  c1e104                  shl      ecx, 4                         
  0x002165AC  8b4c3114                mov      ecx, dword ptr [ecx + esi + 0x14] 
  0x002165B0  85c9                    test     ecx, ecx                       
  0x002165B2  7405                    je       0x2165b9                       
                                        ; XREF: 0x0021657E (cond_jump)
  0x002165B4  8b5114                  mov      edx, dword ptr [ecx + 0x14]    
  0x002165B7  eb03                    jmp      0x2165bc                       
                                        ; XREF: 0x002165B2 (cond_jump)
  0x002165B9  8b55f8                  mov      edx, dword ptr [ebp - 8]       
                                        ; XREF: 0x0021658D (cond_jump), 0x002165B7 (jump)
  0x002165BC  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x002165BF  85c9                    test     ecx, ecx                       
  0x002165C1  8b4018                  mov      eax, dword ptr [eax + 0x18]    
  0x002165C4  750f                    jne      0x2165d5                       
  0x002165C6  ff75f4                  push     dword ptr [ebp - 0xc]          
  0x002165C9  8bce                    mov      ecx, esi                       
  0x002165CB  894708                  mov      dword ptr [edi + 8], eax       
  0x002165CE  e818fdffff              call     0x2162eb                       ; -> sub_002162EB
  0x002165D3  eb06                    jmp      0x2165db                       
; end of function
                                        ; XREF: 0x002165C4 (cond_jump)
  0x002165D5  894118                  mov      dword ptr [ecx + 0x18], eax    
  0x002165D8  89510c                  mov      dword ptr [ecx + 0xc], edx     
                                        ; XREF: 0x002165D3 (jump)
  0x002165DB  668b45f0                mov      ax, word ptr [ebp - 0x10]      
  0x002165DF  66294702                sub      word ptr [edi + 2], ax         
  0x002165E3  84db                    test     bl, bl                         
  0x002165E5  0f85c3000000            jne      0x2166ae                       
  0x002165EB  b001                    mov      al, 1                          
  0x002165ED  8ac8                    mov      cl, al                         
                                        ; XREF: 0x00216623 (cond_jump)
  0x002165EF  3ac8                    cmp      cl, al                         
  0x002165F1  7728                    ja       0x21661b                       
  0x002165F3  0fb6d1                  movzx    edx, cl                        
  0x002165F6  c1e204                  shl      edx, 4                         
  0x002165F9  8d7c3212                lea      edi, [edx + esi + 0x12]        
  0x002165FD  8ad0                    mov      dl, al                         
  0x002165FF  2ad1                    sub      dl, cl                         
  0x00216601  fec2                    inc      dl                             
  0x00216603  0fb6d2                  movzx    edx, dl                        
  0x00216606  8955fc                  mov      dword ptr [ebp - 4], edx       
                                        ; XREF: 0x00216619 (cond_jump)
  0x00216609  8b55ec                  mov      edx, dword ptr [ebp - 0x14]    
  0x0021660C  668b5222                mov      dx, word ptr [edx + 0x22]      
  0x00216610  662917                  sub      word ptr [edi], dx             
  0x00216613  83c710                  add      edi, 0x10                      
  0x00216616  ff4dfc                  dec      dword ptr [ebp - 4]            
  0x00216619  75ee                    jne      0x216609                       
                                        ; XREF: 0x002165F1 (cond_jump), 0x002166B2 (jump)
  0x0021661B  d0e0                    shl      al, 1                          
  0x0021661D  d0e1                    shl      cl, 1                          
  0x0021661F  fec0                    inc      al                             
  0x00216621  3c40                    cmp      al, 0x40                       
  0x00216623  76ca                    jbe      0x2165ef                       
  0x00216625  80fb01                  cmp      bl, 1                          
  0x00216628  7671                    jbe      0x21669b                       
                                        ; XREF: 0x00216696 (cond_jump)
  0x0021662A  8ad3                    mov      dl, bl                         
  0x0021662C  80f201                  xor      dl, 1                          
  0x0021662F  0fb6ca                  movzx    ecx, dl                        
  0x00216632  c1e104                  shl      ecx, 4                         
  0x00216635  8d4c310c                lea      ecx, [ecx + esi + 0xc]         
  0x00216639  0fb77904                movzx    edi, word ptr [ecx + 4]        
  0x0021663D  0fb74902                movzx    ecx, word ptr [ecx + 2]        
  0x00216641  03f9                    add      edi, ecx                       
  0x00216643  0fb6cb                  movzx    ecx, bl                        
  0x00216646  c1e104                  shl      ecx, 4                         
  0x00216649  897dfc                  mov      dword ptr [ebp - 4], edi       
  0x0021664C  8d4c310c                lea      ecx, [ecx + esi + 0xc]         
  0x00216650  0fb77904                movzx    edi, word ptr [ecx + 4]        
  0x00216654  0fb74902                movzx    ecx, word ptr [ecx + 2]        
  0x00216658  8ac3                    mov      al, bl                         
  0x0021665A  03f9                    add      edi, ecx                       
  0x0021665C  d0e8                    shr      al, 1                          
  0x0021665E  3b7dfc                  cmp      edi, dword ptr [ebp - 4]       
  0x00216661  7f12                    jg       0x216675                       
  0x00216663  0fb6c8                  movzx    ecx, al                        
  0x00216666  c1e104                  shl      ecx, 4                         
  0x00216669  0fb74c3110              movzx    ecx, word ptr [ecx + esi + 0x10] 
  0x0021666E  394dfc                  cmp      dword ptr [ebp - 4], ecx       
  0x00216671  7425                    je       0x216698                       
  0x00216673  8ada                    mov      bl, dl                         
                                        ; XREF: 0x00216661 (cond_jump)
  0x00216675  0fb6cb                  movzx    ecx, bl                        
  0x00216678  c1e104                  shl      ecx, 4                         
  0x0021667B  8d4c310c                lea      ecx, [ecx + esi + 0xc]         
  0x0021667F  668b5104                mov      dx, word ptr [ecx + 4]         
  0x00216683  66035102                add      dx, word ptr [ecx + 2]         
  0x00216687  0fb6c8                  movzx    ecx, al                        
  0x0021668A  c1e104                  shl      ecx, 4                         
  0x0021668D  3c01                    cmp      al, 1                          
  0x0021668F  6689543110              mov      word ptr [ecx + esi + 0x10], dx 
  0x00216694  8ad8                    mov      bl, al                         
  0x00216696  7792                    ja       0x21662a                       
                                        ; XREF: 0x00216671 (cond_jump)
  0x00216698  80fb01                  cmp      bl, 1                          
                                        ; XREF: 0x00216628 (cond_jump)
  0x0021669B  750c                    jne      0x2166a9                       
  0x0021669D  668b4620                mov      ax, word ptr [esi + 0x20]      
  0x002166A1  6603461e                add      ax, word ptr [esi + 0x1e]      
  0x002166A5  66894610                mov      word ptr [esi + 0x10], ax      
                                        ; XREF: 0x0021669B (cond_jump)
  0x002166A9  5f                      pop      edi                            
  0x002166AA  5e                      pop      esi                            
  0x002166AB  5b                      pop      ebx                            
  0x002166AC  c9                      leave                                   
  0x002166AD  c3                      ret                                     
                                        ; XREF: 0x002165E5 (cond_jump)
  0x002166AE  8acb                    mov      cl, bl                         
  0x002166B0  8ac3                    mov      al, bl                         
  0x002166B2  e964ffffff              jmp      0x21661b                       
  0x002166B7  56                      push     esi                            
  0x002166B8  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x002166BC  8b0e                    mov      ecx, dword ptr [esi]           
  0x002166BE  ff05a0a44000            inc      dword ptr [0x40a4a0]           
  0x002166C4  8b4110                  mov      eax, dword ptr [ecx + 0x10]    
  0x002166C7  8b510c                  mov      edx, dword ptr [ecx + 0xc]     
  0x002166CA  23d0                    and      edx, eax                       
  0x002166CC  57                      push     edi                            
  0x002166CD  7477                    je       0x216746                       
  0x002166CF  bf00000080              mov      edi, 0x80000000                
  0x002166D4  85c7                    test     edi, eax                       
  0x002166D6  746e                    je       0x216746                       
  0x002166D8  33c0                    xor      eax, eax                       
  0x002166DA  40                      inc      eax                            
  0x002166DB  84d0                    test     al, dl                         
  0x002166DD  897914                  mov      dword ptr [ecx + 0x14], edi    
  0x002166E0  7406                    je       0x2166e8                       
  0x002166E2  89410c                  mov      dword ptr [ecx + 0xc], eax     
  0x002166E5  83e2fe                  and      edx, 0xfffffffe                
                                        ; XREF: 0x002166E0 (cond_jump)
  0x002166E8  f6c220                  test     dl, 0x20                       
  0x002166EB  742f                    je       0x21671c                       
  0x002166ED  53                      push     ebx                            
  0x002166EE  8b5e08                  mov      ebx, dword ptr [esi + 8]       
  0x002166F1  0fb79b80000000          movzx    ebx, word ptr [ebx + 0x80]     
  0x002166F8  8dbe18040000            lea      edi, [esi + 0x418]             
  0x002166FE  8b07                    mov      eax, dword ptr [edi]           
  0x00216700  33d8                    xor      ebx, eax                       
  0x00216702  81e300800000            and      ebx, 0x8000                    
  0x00216708  2bc3                    sub      eax, ebx                       
  0x0021670A  0500000100              add      eax, 0x10000                   
  0x0021670F  8907                    mov      dword ptr [edi], eax           
  0x00216711  c7410c20000000          mov      dword ptr [ecx + 0xc], 0x20    
  0x00216718  83e2df                  and      edx, 0xffffffdf                
  0x0021671B  5b                      pop      ebx                            
                                        ; XREF: 0x002166EB (cond_jump)
  0x0021671C  85d2                    test     edx, edx                       
  0x0021671E  7419                    je       0x216739                       
  0x00216720  6a00                    push     0                              
  0x00216722  899638040000            mov      dword ptr [esi + 0x438], edx   
  0x00216728  6a00                    push     0                              
  0x0021672A  81c640040000            add      esi, 0x440                     
  0x00216730  56                      push     esi                            
  0x00216731  ff158c7b2100            call     dword ptr [0x217b8c]           ; -> xbox_KeInsertQueueDpc
  0x00216737  eb09                    jmp      0x216742                       
                                        ; XREF: 0x0021671E (cond_jump)
  0x00216739  8b06                    mov      eax, dword ptr [esi]           
  0x0021673B  c7401000000080          mov      dword ptr [eax + 0x10], 0x80000000 
                                        ; XREF: 0x00216737 (jump)
  0x00216742  b001                    mov      al, 1                          
  0x00216744  eb02                    jmp      0x216748                       
                                        ; XREF: 0x002166CD (cond_jump), 0x002166D6 (cond_jump)
  0x00216746  32c0                    xor      al, al                         
                                        ; XREF: 0x00216744 (jump)
  0x00216748  5f                      pop      edi                            
  0x00216749  5e                      pop      esi                            
  0x0021674A  c20800                  ret      8                              

; ============================================================
; Function: sub_0021674D
; Start: 0x0021674D  End: 0x00216771  Size: 36 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00215188, sub_00215371, sub_00216C78, sub_00217298, sub_002172F2
; ============================================================
sub_0021674D:
  0x0021674D  8b4108                  mov      eax, dword ptr [ecx + 8]       
  0x00216750  8b9118040000            mov      edx, dword ptr [ecx + 0x418]   
  0x00216756  0fb78880000000          movzx    ecx, word ptr [eax + 0x80]     
  0x0021675D  8bc1                    mov      eax, ecx                       
  0x0021675F  33c2                    xor      eax, edx                       
  0x00216761  81e1ff7f0000            and      ecx, 0x7fff                    
  0x00216767  2500800000              and      eax, 0x8000                    
  0x0021676C  0bca                    or       ecx, edx                       
  0x0021676E  03c1                    add      eax, ecx                       
  0x00216770  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00216771
; Start: 0x00216771  End: 0x002167ED  Size: 124 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002128D5
; ============================================================
sub_00216771:
  0x00216771  53                      push     ebx                            
  0x00216772  56                      push     esi                            
  0x00216773  57                      push     edi                            
  0x00216774  8b7c2410                mov      edi, dword ptr [esp + 0x10]    
  0x00216778  8bf2                    mov      esi, edx                       
  0x0021677A  fe4e27                  dec      byte ptr [esi + 0x27]          
  0x0021677D  8b4718                  mov      eax, dword ptr [edi + 0x18]    
  0x00216780  85c0                    test     eax, eax                       
  0x00216782  8bd9                    mov      ebx, ecx                       
  0x00216784  740e                    je       0x216794                       
  0x00216786  0fb74f20                movzx    ecx, word ptr [edi + 0x20]     
  0x0021678A  6a01                    push     1                              
  0x0021678C  51                      push     ecx                            
  0x0021678D  50                      push     eax                            
  0x0021678E  ff15887b2100            call     dword ptr [0x217b88]           ; -> xbox_MmLockUnlockBufferPages
                                        ; XREF: 0x00216784 (cond_jump)
  0x00216794  f6472201                test     byte ptr [edi + 0x22], 1       
  0x00216798  7443                    je       0x2167dd                       
  0x0021679A  8b832c040000            mov      eax, dword ptr [ebx + 0x42c]   
  0x002167A0  33c9                    xor      ecx, ecx                       
  0x002167A2  85c0                    test     eax, eax                       
  0x002167A4  7437                    je       0x2167dd                       
                                        ; XREF: 0x002167B1 (cond_jump)
  0x002167A6  3bf8                    cmp      edi, eax                       
  0x002167A8  7409                    je       0x2167b3                       
  0x002167AA  8bc8                    mov      ecx, eax                       
  0x002167AC  8b4024                  mov      eax, dword ptr [eax + 0x24]    
  0x002167AF  85c0                    test     eax, eax                       
  0x002167B1  75f3                    jne      0x2167a6                       
                                        ; XREF: 0x002167A8 (cond_jump)
  0x002167B3  85c0                    test     eax, eax                       
  0x002167B5  7426                    je       0x2167dd                       
  0x002167B7  85c9                    test     ecx, ecx                       
  0x002167B9  750b                    jne      0x2167c6                       
  0x002167BB  8b4824                  mov      ecx, dword ptr [eax + 0x24]    
  0x002167BE  898b2c040000            mov      dword ptr [ebx + 0x42c], ecx   
  0x002167C4  eb06                    jmp      0x2167cc                       
                                        ; XREF: 0x002167B9 (cond_jump)
  0x002167C6  8b5024                  mov      edx, dword ptr [eax + 0x24]    
  0x002167C9  895124                  mov      dword ptr [ecx + 0x24], edx    
                                        ; XREF: 0x002167C4 (jump)
  0x002167CC  83602400                and      dword ptr [eax + 0x24], 0      
  0x002167D0  fe4e20                  dec      byte ptr [esi + 0x20]          
  0x002167D3  7508                    jne      0x2167dd                       
  0x002167D5  806610df                and      byte ptr [esi + 0x10], 0xdf    
  0x002167D9  806601bf                and      byte ptr [esi + 1], 0xbf       
                                        ; XREF: 0x00216798 (cond_jump), 0x002167A4 (cond_jump), 0x002167B5 (cond_jump), 0x002167D3 (cond_jump)
  0x002167DD  804f2208                or       byte ptr [edi + 0x22], 8       
  0x002167E1  57                      push     edi                            
  0x002167E2  e8eec0ffff              call     0x2128d5                       ; -> sub_002128D5
  0x002167E7  5f                      pop      edi                            
  0x002167E8  5e                      pop      esi                            
  0x002167E9  5b                      pop      ebx                            
  0x002167EA  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002167ED
; Start: 0x002167ED  End: 0x0021681E  Size: 49 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_002167ED:
  0x002167ED  0fb64211                movzx    eax, byte ptr [edx + 0x11]     
  0x002167F1  83e800                  sub      eax, 0                         
  0x002167F4  741c                    je       0x216812                       
  0x002167F6  48                      dec      eax                            
  0x002167F7  48                      dec      eax                            
  0x002167F8  740c                    je       0x216806                       
  0x002167FA  48                      dec      eax                            
  0x002167FB  7521                    jne      0x21681e                       
  0x002167FD  66ff4a24                dec      word ptr [edx + 0x24]          
  0x00216801  e9a50f0000              jmp      0x2177ab                       ; -> sub_002177AB
                                        ; XREF: 0x002167F8 (cond_jump)
  0x00216806  66ff05c6b9c600          inc      word ptr [0xc6b9c6]            
  0x0021680D  e9ec0f0000              jmp      0x2177fe                       ; -> sub_002177FE
                                        ; XREF: 0x002167F4 (cond_jump)
  0x00216812  66ff05c2b9c600          inc      word ptr [0xc6b9c2]            
  0x00216819  e92d100000              jmp      0x21784b                       ; -> sub_0021784B
; end of function
                                        ; XREF: 0x002167FB (cond_jump)
  0x0021681E  c3                      ret                                     

; ============================================================
; Function: sub_0021681F
; Start: 0x0021681F  End: 0x0021688E  Size: 111 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_00216913
; ============================================================
sub_0021681F:
  0x0021681F  55                      push     ebp                            
  0x00216820  8bec                    mov      ebp, esp                       
  0x00216822  83ec10                  sub      esp, 0x10                      
  0x00216825  8b02                    mov      eax, dword ptr [edx]           
  0x00216827  53                      push     ebx                            
  0x00216828  8b5a18                  mov      ebx, dword ptr [edx + 0x18]    
  0x0021682B  56                      push     esi                            
  0x0021682C  c1e81c                  shr      eax, 0x1c                      
  0x0021682F  83f809                  cmp      eax, 9                         
  0x00216832  57                      push     edi                            
  0x00216833  8b7a14                  mov      edi, dword ptr [edx + 0x14]    
  0x00216836  894df8                  mov      dword ptr [ebp - 8], ecx       
  0x00216839  895df0                  mov      dword ptr [ebp - 0x10], ebx    
  0x0021683C  c645ff01                mov      byte ptr [ebp - 1], 1          
  0x00216840  754c                    jne      0x21688e                       
  0x00216842  807b1d00                cmp      byte ptr [ebx + 0x1d], 0       
  0x00216846  7446                    je       0x21688e                       
  0x00216848  8b4204                  mov      eax, dword ptr [edx + 4]       
  0x0021684B  85c0                    test     eax, eax                       
  0x0021684D  742e                    je       0x21687d                       
  0x0021684F  8b4a0c                  mov      ecx, dword ptr [edx + 0xc]     
  0x00216852  beff0f0000              mov      esi, 0xfff                     
  0x00216857  23c6                    and      eax, esi                       
  0x00216859  23ce                    and      ecx, esi                       
  0x0021685B  3bc8                    cmp      ecx, eax                       
  0x0021685D  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x00216860  7c0b                    jl       0x21686d                       
  0x00216862  0fb6421d                movzx    eax, byte ptr [edx + 0x1d]     
  0x00216866  2bc1                    sub      eax, ecx                       
  0x00216868  0345f4                  add      eax, dword ptr [ebp - 0xc]     
  0x0021686B  eb0d                    jmp      0x21687a                       
                                        ; XREF: 0x00216860 (cond_jump)
  0x0021686D  0fb6721d                movzx    esi, byte ptr [edx + 0x1d]     
  0x00216871  2bf1                    sub      esi, ecx                       
  0x00216873  8d840600f0ffff          lea      eax, [esi + eax - 0x1000]      
                                        ; XREF: 0x0021686B (jump)
  0x0021687A  48                      dec      eax                            
  0x0021687B  eb04                    jmp      0x216881                       
                                        ; XREF: 0x0021684D (cond_jump)
  0x0021687D  0fb6421d                movzx    eax, byte ptr [edx + 0x1d]     
                                        ; XREF: 0x0021687B (jump)
  0x00216881  014314                  add      dword ptr [ebx + 0x14], eax    
  0x00216884  83630400                and      dword ptr [ebx + 4], 0         
  0x00216888  8065ff00                and      byte ptr [ebp - 1], 0          
  0x0021688C  eb19                    jmp      0x2168a7                       
; end of function
                                        ; XREF: 0x00216840 (cond_jump), 0x00216846 (cond_jump)
  0x0021688E  83f80f                  cmp      eax, 0xf                       
  0x00216891  7507                    jne      0x21689a                       
  0x00216893  c743040f0000c0          mov      dword ptr [ebx + 4], 0xc000000f 
                                        ; XREF: 0x00216891 (cond_jump)
  0x0021689A  8b02                    mov      eax, dword ptr [edx]           
  0x0021689C  c1e81c                  shr      eax, 0x1c                      
  0x0021689F  0d000000c0              or       eax, 0xc0000000                
  0x002168A4  894304                  mov      dword ptr [ebx + 4], eax       
                                        ; XREF: 0x0021688C (jump)
  0x002168A7  8b7708                  mov      esi, dword ptr [edi + 8]       
  0x002168AA  83e6f0                  and      esi, 0xfffffff0                
                                        ; XREF: 0x002168DC (cond_jump)
  0x002168AD  8a5a1c                  mov      bl, byte ptr [edx + 0x1c]      
  0x002168B0  52                      push     edx                            
  0x002168B1  80e302                  and      bl, 2                          
  0x002168B4  e872f9ffff              call     0x21622b                       ; -> sub_0021622B
  0x002168B9  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x002168BC  8bd7                    mov      edx, edi                       
  0x002168BE  e82affffff              call     0x2167ed                       ; -> sub_002167ED
  0x002168C3  a1a0b9c600              mov      eax, dword ptr [0xc6b9a0]      
  0x002168C8  8d1430                  lea      edx, [eax + esi]               
  0x002168CB  807a1e02                cmp      byte ptr [edx + 0x1e], 2       
  0x002168CF  8b7208                  mov      esi, dword ptr [edx + 8]       
  0x002168D2  7506                    jne      0x2168da                       
  0x002168D4  807dff00                cmp      byte ptr [ebp - 1], 0          
  0x002168D8  7404                    je       0x2168de                       
                                        ; XREF: 0x002168D2 (cond_jump)
  0x002168DA  84db                    test     bl, bl                         
  0x002168DC  74cf                    je       0x2168ad                       
                                        ; XREF: 0x002168D8 (cond_jump)
  0x002168DE  8b4210                  mov      eax, dword ptr [edx + 0x10]    
  0x002168E1  334708                  xor      eax, dword ptr [edi + 8]       
  0x002168E4  83e00f                  and      eax, 0xf                       
  0x002168E7  334210                  xor      eax, dword ptr [edx + 0x10]    
  0x002168EA  84db                    test     bl, bl                         
  0x002168EC  894708                  mov      dword ptr [edi + 8], eax       
  0x002168EF  740d                    je       0x2168fe                       
  0x002168F1  ff75f0                  push     dword ptr [ebp - 0x10]         
  0x002168F4  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x002168F7  8bd7                    mov      edx, edi                       
  0x002168F9  e873feffff              call     0x216771                       ; -> sub_00216771
                                        ; XREF: 0x002168EF (cond_jump)
  0x002168FE  807f1100                cmp      byte ptr [edi + 0x11], 0       
  0x00216902  7406                    je       0x21690a                       
  0x00216904  807dff00                cmp      byte ptr [ebp - 1], 0          
  0x00216908  7504                    jne      0x21690e                       
                                        ; XREF: 0x00216902 (cond_jump)
  0x0021690A  836708fe                and      dword ptr [edi + 8], 0xfffffffe 
                                        ; XREF: 0x00216908 (cond_jump)
  0x0021690E  5f                      pop      edi                            
  0x0021690F  5e                      pop      esi                            
  0x00216910  5b                      pop      ebx                            
  0x00216911  c9                      leave                                   
  0x00216912  c3                      ret                                     

; ============================================================
; Function: sub_00216913
; Start: 0x00216913  End: 0x0021695A  Size: 71 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021622B, sub_0021681F
; Called by: sub_002169BF, sub_00216AC1, sub_00216C78
; ============================================================
sub_00216913:
  0x00216913  55                      push     ebp                            
  0x00216914  8bec                    mov      ebp, esp                       
  0x00216916  51                      push     ecx                            
  0x00216917  51                      push     ecx                            
  0x00216918  56                      push     esi                            
  0x00216919  8bf2                    mov      esi, edx                       
  0x0021691B  807e1e01                cmp      byte ptr [esi + 0x1e], 1       
  0x0021691F  8b4614                  mov      eax, dword ptr [esi + 0x14]    
  0x00216922  57                      push     edi                            
  0x00216923  8b7e18                  mov      edi, dword ptr [esi + 0x18]    
  0x00216926  894dfc                  mov      dword ptr [ebp - 4], ecx       
  0x00216929  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x0021692C  751a                    jne      0x216948                       
  0x0021692E  8b460c                  mov      eax, dword ptr [esi + 0xc]     
  0x00216931  8b0da0b9c600            mov      ecx, dword ptr [0xc6b9a0]      
  0x00216937  8d4408f9                lea      eax, [eax + ecx - 7]           
  0x0021693B  50                      push     eax                            
  0x0021693C  e8eaf8ffff              call     0x21622b                       ; -> sub_0021622B
  0x00216941  66ff05c2b9c600          inc      word ptr [0xc6b9c2]            
                                        ; XREF: 0x0021692C (cond_jump)
  0x00216948  f64603f0                test     byte ptr [esi + 3], 0xf0       
  0x0021694C  740c                    je       0x21695a                       
  0x0021694E  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x00216951  8bd6                    mov      edx, esi                       
  0x00216953  e8c7feffff              call     0x21681f                       ; -> sub_0021681F
  0x00216958  eb61                    jmp      0x2169bb                       
; end of function
                                        ; XREF: 0x0021694C (cond_jump)
  0x0021695A  8b4604                  mov      eax, dword ptr [esi + 4]       
  0x0021695D  85c0                    test     eax, eax                       
  0x0021695F  53                      push     ebx                            
  0x00216960  7426                    je       0x216988                       
  0x00216962  8b4e0c                  mov      ecx, dword ptr [esi + 0xc]     
  0x00216965  baff0f0000              mov      edx, 0xfff                     
  0x0021696A  23c2                    and      eax, edx                       
  0x0021696C  8bd8                    mov      ebx, eax                       
  0x0021696E  0fb6461d                movzx    eax, byte ptr [esi + 0x1d]     
  0x00216972  23ca                    and      ecx, edx                       
  0x00216974  2bc1                    sub      eax, ecx                       
  0x00216976  3bcb                    cmp      ecx, ebx                       
  0x00216978  7c04                    jl       0x21697e                       
  0x0021697A  03c3                    add      eax, ebx                       
  0x0021697C  eb07                    jmp      0x216985                       
                                        ; XREF: 0x00216978 (cond_jump)
  0x0021697E  8d841800f0ffff          lea      eax, [eax + ebx - 0x1000]      
                                        ; XREF: 0x0021697C (jump)
  0x00216985  48                      dec      eax                            
  0x00216986  eb04                    jmp      0x21698c                       
                                        ; XREF: 0x00216960 (cond_jump)
  0x00216988  0fb6461d                movzx    eax, byte ptr [esi + 0x1d]     
                                        ; XREF: 0x00216986 (jump)
  0x0021698C  014714                  add      dword ptr [edi + 0x14], eax    
  0x0021698F  8a5e1c                  mov      bl, byte ptr [esi + 0x1c]      
  0x00216992  56                      push     esi                            
  0x00216993  80e302                  and      bl, 2                          
  0x00216996  e890f8ffff              call     0x21622b                       ; -> sub_0021622B
  0x0021699B  8b55f8                  mov      edx, dword ptr [ebp - 8]       
  0x0021699E  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x002169A1  e847feffff              call     0x2167ed                       ; -> sub_002167ED
  0x002169A6  84db                    test     bl, bl                         
  0x002169A8  5b                      pop      ebx                            
  0x002169A9  7410                    je       0x2169bb                       
  0x002169AB  8b55f8                  mov      edx, dword ptr [ebp - 8]       
  0x002169AE  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x002169B1  83670400                and      dword ptr [edi + 4], 0         
  0x002169B5  57                      push     edi                            
  0x002169B6  e8b6fdffff              call     0x216771                       ; -> sub_00216771
                                        ; XREF: 0x00216958 (jump), 0x002169A9 (cond_jump)
  0x002169BB  5f                      pop      edi                            
  0x002169BC  5e                      pop      esi                            
  0x002169BD  c9                      leave                                   
  0x002169BE  c3                      ret                                     

; ============================================================
; Function: sub_002169BF
; Start: 0x002169BF  End: 0x00216AC1  Size: 258 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00216913
; Called by: sub_00216C78
; ============================================================
sub_002169BF:
  0x002169BF  55                      push     ebp                            
  0x002169C0  8bec                    mov      ebp, esp                       
  0x002169C2  83ec18                  sub      esp, 0x18                      
  0x002169C5  53                      push     ebx                            
  0x002169C6  57                      push     edi                            
  0x002169C7  8bfa                    mov      edi, edx                       
  0x002169C9  8bd9                    mov      ebx, ecx                       
  0x002169CB  33d2                    xor      edx, edx                       
  0x002169CD  39932c040000            cmp      dword ptr [ebx + 0x42c], edx   
  0x002169D3  897dec                  mov      dword ptr [ebp - 0x14], edi    
  0x002169D6  895df8                  mov      dword ptr [ebp - 8], ebx       
  0x002169D9  8955fc                  mov      dword ptr [ebp - 4], edx       
  0x002169DC  0f84ce000000            je       0x216ab0                       
  0x002169E2  56                      push     esi                            
  0x002169E3  eb03                    jmp      0x2169e8                       
                                        ; XREF: 0x00216AA9 (cond_jump)
  0x002169E5  8b7dec                  mov      edi, dword ptr [ebp - 0x14]    
                                        ; XREF: 0x002169E3 (jump)
  0x002169E8  8b8b2c040000            mov      ecx, dword ptr [ebx + 0x42c]   
  0x002169EE  8b4124                  mov      eax, dword ptr [ecx + 0x24]    
  0x002169F1  89832c040000            mov      dword ptr [ebx + 0x42c], eax   
  0x002169F7  8b7110                  mov      esi, dword ptr [ecx + 0x10]    
  0x002169FA  3b7e1c                  cmp      edi, dword ptr [esi + 0x1c]    
  0x002169FD  730b                    jae      0x216a0a                       
                                        ; XREF: 0x00216A1A (jump)
  0x002169FF  895124                  mov      dword ptr [ecx + 0x24], edx    
  0x00216A02  894dfc                  mov      dword ptr [ebp - 4], ecx       
  0x00216A05  e995000000              jmp      0x216a9f                       
                                        ; XREF: 0x002169FD (cond_jump)
  0x00216A0A  8a4610                  mov      al, byte ptr [esi + 0x10]      
  0x00216A0D  a840                    test     al, 0x40                       
  0x00216A0F  740b                    je       0x216a1c                       
  0x00216A11  47                      inc      edi                            
  0x00216A12  24bf                    and      al, 0xbf                       
  0x00216A14  897e1c                  mov      dword ptr [esi + 0x1c], edi    
  0x00216A17  884610                  mov      byte ptr [esi + 0x10], al      
  0x00216A1A  ebe3                    jmp      0x2169ff                       
                                        ; XREF: 0x00216A0F (cond_jump)
  0x00216A1C  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x00216A1F  8b3da0b9c600            mov      edi, dword ptr [0xc6b9a0]      
  0x00216A25  8365f400                and      dword ptr [ebp - 0xc], 0       
  0x00216A29  8945e8                  mov      dword ptr [ebp - 0x18], eax    
  0x00216A2C  83e0f0                  and      eax, 0xfffffff0                
  0x00216A2F  8d1407                  lea      edx, [edi + eax]               
  0x00216A32  3b4a18                  cmp      ecx, dword ptr [edx + 0x18]    
  0x00216A35  8945f0                  mov      dword ptr [ebp - 0x10], eax    
  0x00216A38  7418                    je       0x216a52                       
  0x00216A3A  8b5e04                  mov      ebx, dword ptr [esi + 4]       
                                        ; XREF: 0x00216A4D (cond_jump)
  0x00216A3D  3bc3                    cmp      eax, ebx                       
  0x00216A3F  740e                    je       0x216a4f                       
  0x00216A41  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x00216A44  8b4208                  mov      eax, dword ptr [edx + 8]       
  0x00216A47  8d1407                  lea      edx, [edi + eax]               
  0x00216A4A  3b4a18                  cmp      ecx, dword ptr [edx + 0x18]    
  0x00216A4D  75ee                    jne      0x216a3d                       
                                        ; XREF: 0x00216A3F (cond_jump)
  0x00216A4F  8b5df8                  mov      ebx, dword ptr [ebp - 8]       
                                        ; XREF: 0x00216A38 (cond_jump)
  0x00216A52  8b4208                  mov      eax, dword ptr [edx + 8]       
  0x00216A55  3345e8                  xor      eax, dword ptr [ebp - 0x18]    
  0x00216A58  8bcb                    mov      ecx, ebx                       
  0x00216A5A  83e00f                  and      eax, 0xf                       
  0x00216A5D  334208                  xor      eax, dword ptr [edx + 8]       
  0x00216A60  894608                  mov      dword ptr [esi + 8], eax       
  0x00216A63  804a03f0                or       byte ptr [edx + 3], 0xf0       
  0x00216A67  e8a7feffff              call     0x216913                       ; -> sub_00216913
  0x00216A6C  8b45f4                  mov      eax, dword ptr [ebp - 0xc]     
  0x00216A6F  85c0                    test     eax, eax                       
  0x00216A71  741f                    je       0x216a92                       
  0x00216A73  8b4e08                  mov      ecx, dword ptr [esi + 8]       
  0x00216A76  8b15a0b9c600            mov      edx, dword ptr [0xc6b9a0]      
  0x00216A7C  83e1f0                  and      ecx, 0xfffffff0                
  0x00216A7F  894c0208                mov      dword ptr [edx + eax + 8], ecx 
  0x00216A83  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x00216A86  3345f0                  xor      eax, dword ptr [ebp - 0x10]    
  0x00216A89  83e00f                  and      eax, 0xf                       
  0x00216A8C  3345f0                  xor      eax, dword ptr [ebp - 0x10]    
  0x00216A8F  894608                  mov      dword ptr [esi + 8], eax       
                                        ; XREF: 0x00216A71 (cond_jump)
  0x00216A92  fe4e20                  dec      byte ptr [esi + 0x20]          
  0x00216A95  7508                    jne      0x216a9f                       
  0x00216A97  806610df                and      byte ptr [esi + 0x10], 0xdf    
  0x00216A9B  806601bf                and      byte ptr [esi + 1], 0xbf       
                                        ; XREF: 0x00216A05 (jump), 0x00216A95 (cond_jump)
  0x00216A9F  83bb2c04000000          cmp      dword ptr [ebx + 0x42c], 0     
  0x00216AA6  8b55fc                  mov      edx, dword ptr [ebp - 4]       
  0x00216AA9  0f8536ffffff            jne      0x2169e5                       
  0x00216AAF  5e                      pop      esi                            
                                        ; XREF: 0x002169DC (cond_jump)
  0x00216AB0  33c0                    xor      eax, eax                       
  0x00216AB2  85d2                    test     edx, edx                       
  0x00216AB4  5f                      pop      edi                            
  0x00216AB5  89932c040000            mov      dword ptr [ebx + 0x42c], edx   
  0x00216ABB  0f95c0                  setne    al                             
  0x00216ABE  5b                      pop      ebx                            
  0x00216ABF  c9                      leave                                   
  0x00216AC0  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00216AC1
; Start: 0x00216AC1  End: 0x00216B17  Size: 86 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021622B, sub_00216913
; Called by: sub_00216B17, sub_00216B9E
; ============================================================
sub_00216AC1:
  0x00216AC1  53                      push     ebx                            
  0x00216AC2  56                      push     esi                            
  0x00216AC3  57                      push     edi                            
  0x00216AC4  8bf2                    mov      esi, edx                       
  0x00216AC6  8bf9                    mov      edi, ecx                       
  0x00216AC8  32db                    xor      bl, bl                         
                                        ; XREF: 0x00216B11 (cond_jump)
  0x00216ACA  8b4608                  mov      eax, dword ptr [esi + 8]       
  0x00216ACD  8bc8                    mov      ecx, eax                       
  0x00216ACF  83e1f0                  and      ecx, 0xfffffff0                
  0x00216AD2  743f                    je       0x216b13                       
  0x00216AD4  8b15a0b9c600            mov      edx, dword ptr [0xc6b9a0]      
  0x00216ADA  03d1                    add      edx, ecx                       
  0x00216ADC  3b4e04                  cmp      ecx, dword ptr [esi + 4]       
  0x00216ADF  741c                    je       0x216afd                       
  0x00216AE1  8b4208                  mov      eax, dword ptr [edx + 8]       
  0x00216AE4  804a03f0                or       byte ptr [edx + 3], 0xf0       
  0x00216AE8  334608                  xor      eax, dword ptr [esi + 8]       
  0x00216AEB  8bcf                    mov      ecx, edi                       
  0x00216AED  83e00f                  and      eax, 0xf                       
  0x00216AF0  334208                  xor      eax, dword ptr [edx + 8]       
  0x00216AF3  894608                  mov      dword ptr [esi + 8], eax       
  0x00216AF6  e818feffff              call     0x216913                       ; -> sub_00216913
  0x00216AFB  eb12                    jmp      0x216b0f                       
                                        ; XREF: 0x00216ADF (cond_jump)
  0x00216AFD  83660400                and      dword ptr [esi + 4], 0         
  0x00216B01  83e00f                  and      eax, 0xf                       
  0x00216B04  52                      push     edx                            
  0x00216B05  894608                  mov      dword ptr [esi + 8], eax       
  0x00216B08  e81ef7ffff              call     0x21622b                       ; -> sub_0021622B
  0x00216B0D  b301                    mov      bl, 1                          
                                        ; XREF: 0x00216AFB (jump)
  0x00216B0F  84db                    test     bl, bl                         
  0x00216B11  74b7                    je       0x216aca                       
                                        ; XREF: 0x00216AD2 (cond_jump)
  0x00216B13  5f                      pop      edi                            
  0x00216B14  5e                      pop      esi                            
  0x00216B15  5b                      pop      ebx                            
  0x00216B16  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00216B17
; Start: 0x00216B17  End: 0x00216B9E  Size: 135 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002128D5, sub_00216AC1
; Called by: sub_00216C78
; ============================================================
sub_00216B17:
  0x00216B17  51                      push     ecx                            
  0x00216B18  53                      push     ebx                            
  0x00216B19  55                      push     ebp                            
  0x00216B1A  8bd9                    mov      ebx, ecx                       
  0x00216B1C  33ed                    xor      ebp, ebp                       
  0x00216B1E  39ab30040000            cmp      dword ptr [ebx + 0x430], ebp   
  0x00216B24  89542408                mov      dword ptr [esp + 8], edx       
  0x00216B28  7463                    je       0x216b8d                       
  0x00216B2A  56                      push     esi                            
  0x00216B2B  57                      push     edi                            
                                        ; XREF: 0x00216B89 (cond_jump)
  0x00216B2C  8bbb30040000            mov      edi, dword ptr [ebx + 0x430]   
  0x00216B32  8b4724                  mov      eax, dword ptr [edi + 0x24]    
  0x00216B35  898330040000            mov      dword ptr [ebx + 0x430], eax   
  0x00216B3B  8b7710                  mov      esi, dword ptr [edi + 0x10]    
  0x00216B3E  3b561c                  cmp      edx, dword ptr [esi + 0x1c]    
  0x00216B41  7307                    jae      0x216b4a                       
                                        ; XREF: 0x00216B5C (jump)
  0x00216B43  896f14                  mov      dword ptr [edi + 0x14], ebp    
  0x00216B46  8bef                    mov      ebp, edi                       
  0x00216B48  eb38                    jmp      0x216b82                       
                                        ; XREF: 0x00216B41 (cond_jump)
  0x00216B4A  8a4610                  mov      al, byte ptr [esi + 0x10]      
  0x00216B4D  a840                    test     al, 0x40                       
  0x00216B4F  740d                    je       0x216b5e                       
  0x00216B51  8d4a01                  lea      ecx, [edx + 1]                 
  0x00216B54  24bf                    and      al, 0xbf                       
  0x00216B56  894e1c                  mov      dword ptr [esi + 0x1c], ecx    
  0x00216B59  884610                  mov      byte ptr [esi + 0x10], al      
  0x00216B5C  ebe5                    jmp      0x216b43                       
                                        ; XREF: 0x00216B4F (cond_jump)
  0x00216B5E  8bd6                    mov      edx, esi                       
  0x00216B60  8bcb                    mov      ecx, ebx                       
  0x00216B62  e85affffff              call     0x216ac1                       ; -> sub_00216AC1
  0x00216B67  fe4e20                  dec      byte ptr [esi + 0x20]          
  0x00216B6A  7508                    jne      0x216b74                       
  0x00216B6C  806610df                and      byte ptr [esi + 0x10], 0xdf    
  0x00216B70  806601bf                and      byte ptr [esi + 1], 0xbf       
                                        ; XREF: 0x00216B6A (cond_jump)
  0x00216B74  83670400                and      dword ptr [edi + 4], 0         
  0x00216B78  57                      push     edi                            
  0x00216B79  e857bdffff              call     0x2128d5                       ; -> sub_002128D5
  0x00216B7E  8b542410                mov      edx, dword ptr [esp + 0x10]    
                                        ; XREF: 0x00216B48 (jump)
  0x00216B82  83bb3004000000          cmp      dword ptr [ebx + 0x430], 0     
  0x00216B89  75a1                    jne      0x216b2c                       
  0x00216B8B  5f                      pop      edi                            
  0x00216B8C  5e                      pop      esi                            
                                        ; XREF: 0x00216B28 (cond_jump)
  0x00216B8D  33c0                    xor      eax, eax                       
  0x00216B8F  89ab30040000            mov      dword ptr [ebx + 0x430], ebp   
  0x00216B95  85ed                    test     ebp, ebp                       
  0x00216B97  5d                      pop      ebp                            
  0x00216B98  0f95c0                  setne    al                             
  0x00216B9B  5b                      pop      ebx                            
  0x00216B9C  59                      pop      ecx                            
  0x00216B9D  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00216B9E
; Start: 0x00216B9E  End: 0x00216C78  Size: 218 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_002128D5, sub_00216AC1, sub_00216F89
; Called by: sub_00216C78
; ============================================================
sub_00216B9E:
  0x00216B9E  55                      push     ebp                            
  0x00216B9F  8bec                    mov      ebp, esp                       
  0x00216BA1  83ec0c                  sub      esp, 0xc                       
  0x00216BA4  53                      push     ebx                            
  0x00216BA5  8bd9                    mov      ebx, ecx                       
  0x00216BA7  33c9                    xor      ecx, ecx                       
  0x00216BA9  398b34040000            cmp      dword ptr [ebx + 0x434], ecx   
  0x00216BAF  8955f8                  mov      dword ptr [ebp - 8], edx       
  0x00216BB2  894dfc                  mov      dword ptr [ebp - 4], ecx       
  0x00216BB5  0f84ad000000            je       0x216c68                       
  0x00216BBB  56                      push     esi                            
  0x00216BBC  57                      push     edi                            
                                        ; XREF: 0x00216C60 (cond_jump)
  0x00216BBD  8bb334040000            mov      esi, dword ptr [ebx + 0x434]   
  0x00216BC3  8b4614                  mov      eax, dword ptr [esi + 0x14]    
  0x00216BC6  898334040000            mov      dword ptr [ebx + 0x434], eax   
  0x00216BCC  8b7e10                  mov      edi, dword ptr [esi + 0x10]    
  0x00216BCF  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x00216BD2  3b471c                  cmp      eax, dword ptr [edi + 0x1c]    
  0x00216BD5  7308                    jae      0x216bdf                       
                                        ; XREF: 0x00216BEB (jump)
  0x00216BD7  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x00216BDA  8975fc                  mov      dword ptr [ebp - 4], esi       
  0x00216BDD  eb77                    jmp      0x216c56                       
                                        ; XREF: 0x00216BD5 (cond_jump)
  0x00216BDF  8a4710                  mov      al, byte ptr [edi + 0x10]      
  0x00216BE2  a840                    test     al, 0x40                       
  0x00216BE4  7407                    je       0x216bed                       
  0x00216BE6  24bf                    and      al, 0xbf                       
  0x00216BE8  884710                  mov      byte ptr [edi + 0x10], al      
  0x00216BEB  ebea                    jmp      0x216bd7                       
                                        ; XREF: 0x00216BE4 (cond_jump)
  0x00216BED  807e014a                cmp      byte ptr [esi + 1], 0x4a       
  0x00216BF1  8bcb                    mov      ecx, ebx                       
  0x00216BF3  7509                    jne      0x216bfe                       
  0x00216BF5  8bd6                    mov      edx, esi                       
  0x00216BF7  e88d030000              call     0x216f89                       ; -> sub_00216F89
  0x00216BFC  eb58                    jmp      0x216c56                       
                                        ; XREF: 0x00216BF3 (cond_jump)
  0x00216BFE  8bd7                    mov      edx, edi                       
  0x00216C00  e8bcfeffff              call     0x216ac1                       ; -> sub_00216AC1
  0x00216C05  8b5618                  mov      edx, dword ptr [esi + 0x18]    
  0x00216C08  85d2                    test     edx, edx                       
  0x00216C0A  7432                    je       0x216c3e                       
  0x00216C0C  8b0f                    mov      ecx, dword ptr [edi]           
  0x00216C0E  894df4                  mov      dword ptr [ebp - 0xc], ecx     
  0x00216C11  c1e907                  shr      ecx, 7                         
  0x00216C14  33c0                    xor      eax, eax                       
  0x00216C16  83e10f                  and      ecx, 0xf                       
  0x00216C19  40                      inc      eax                            
  0x00216C1A  d3e0                    shl      eax, cl                        
  0x00216C1C  8b4df4                  mov      ecx, dword ptr [ebp - 0xc]     
  0x00216C1F  81e100180000            and      ecx, 0x1800                    
  0x00216C25  81f900100000            cmp      ecx, 0x1000                    
  0x00216C2B  7503                    jne      0x216c30                       
  0x00216C2D  c1e010                  shl      eax, 0x10                      
                                        ; XREF: 0x00216C2B (cond_jump)
  0x00216C30  f6470802                test     byte ptr [edi + 8], 2          
  0x00216C34  7404                    je       0x216c3a                       
  0x00216C36  0902                    or       dword ptr [edx], eax           
  0x00216C38  eb04                    jmp      0x216c3e                       
                                        ; XREF: 0x00216C34 (cond_jump)
  0x00216C3A  f7d0                    not      eax                            
  0x00216C3C  2102                    and      dword ptr [edx], eax           
                                        ; XREF: 0x00216C0A (cond_jump), 0x00216C38 (jump)
  0x00216C3E  a1a8b9c600              mov      eax, dword ptr [0xc6b9a8]      
  0x00216C43  894718                  mov      dword ptr [edi + 0x18], eax    
  0x00216C46  893da8b9c600            mov      dword ptr [0xc6b9a8], edi      
  0x00216C4C  83660400                and      dword ptr [esi + 4], 0         
  0x00216C50  56                      push     esi                            
  0x00216C51  e87fbcffff              call     0x2128d5                       ; -> sub_002128D5
                                        ; XREF: 0x00216BDD (jump), 0x00216BFC (jump)
  0x00216C56  83bb3404000000          cmp      dword ptr [ebx + 0x434], 0     
  0x00216C5D  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x00216C60  0f8557ffffff            jne      0x216bbd                       
  0x00216C66  5f                      pop      edi                            
  0x00216C67  5e                      pop      esi                            
                                        ; XREF: 0x00216BB5 (cond_jump)
  0x00216C68  33c0                    xor      eax, eax                       
  0x00216C6A  85c9                    test     ecx, ecx                       
  0x00216C6C  898b34040000            mov      dword ptr [ebx + 0x434], ecx   
  0x00216C72  0f95c0                  setne    al                             
  0x00216C75  5b                      pop      ebx                            
  0x00216C76  c9                      leave                                   
  0x00216C77  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00216C78
; Start: 0x00216C78  End: 0x00216D86  Size: 270 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021616C, sub_0021674D, sub_00216913, sub_002169BF, sub_00216B17, sub_00216B9E, sub_002172F2
; ============================================================
sub_00216C78:
  0x00216C78  55                      push     ebp                            
  0x00216C79  8bec                    mov      ebp, esp                       
  0x00216C7B  51                      push     ecx                            
  0x00216C7C  51                      push     ecx                            
  0x00216C7D  53                      push     ebx                            
  0x00216C7E  56                      push     esi                            
  0x00216C7F  8b750c                  mov      esi, dword ptr [ebp + 0xc]     
  0x00216C82  80650f00                and      byte ptr [ebp + 0xf], 0        
  0x00216C86  8b1e                    mov      ebx, dword ptr [esi]           
  0x00216C88  8d8638040000            lea      eax, [esi + 0x438]             
  0x00216C8E  8b08                    mov      ecx, dword ptr [eax]           
  0x00216C90  57                      push     edi                            
  0x00216C91  33ff                    xor      edi, edi                       
  0x00216C93  2138                    and      dword ptr [eax], edi           
  0x00216C95  f6c102                  test     cl, 2                          
  0x00216C98  894dfc                  mov      dword ptr [ebp - 4], ecx       
  0x00216C9B  7457                    je       0x216cf4                       
  0x00216C9D  8b4e08                  mov      ecx, dword ptr [esi + 8]       
  0x00216CA0  81c184000000            add      ecx, 0x84                      
  0x00216CA6  8b01                    mov      eax, dword ptr [ecx]           
  0x00216CA8  83e0f0                  and      eax, 0xfffffff0                
  0x00216CAB  8939                    mov      dword ptr [ecx], edi           
  0x00216CAD  7431                    je       0x216ce0                       
                                        ; XREF: 0x00216CC1 (cond_jump)
  0x00216CAF  8b0da0b9c600            mov      ecx, dword ptr [0xc6b9a0]      
  0x00216CB5  03c8                    add      ecx, eax                       
  0x00216CB7  8b4108                  mov      eax, dword ptr [ecx + 8]       
  0x00216CBA  85c0                    test     eax, eax                       
  0x00216CBC  897908                  mov      dword ptr [ecx + 8], edi       
  0x00216CBF  8bf9                    mov      edi, ecx                       
  0x00216CC1  75ec                    jne      0x216caf                       
                                        ; XREF: 0x00216CDE (cond_jump)
  0x00216CC3  8bd7                    mov      edx, edi                       
  0x00216CC5  f6420201                test     byte ptr [edx + 2], 1          
  0x00216CC9  8b7f08                  mov      edi, dword ptr [edi + 8]       
  0x00216CCC  8bce                    mov      ecx, esi                       
  0x00216CCE  7407                    je       0x216cd7                       
  0x00216CD0  e81d060000              call     0x2172f2                       ; -> sub_002172F2
  0x00216CD5  eb05                    jmp      0x216cdc                       
                                        ; XREF: 0x00216CCE (cond_jump)
  0x00216CD7  e837fcffff              call     0x216913                       ; -> sub_00216913
                                        ; XREF: 0x00216CD5 (jump)
  0x00216CDC  85ff                    test     edi, edi                       
  0x00216CDE  75e3                    jne      0x216cc3                       
                                        ; XREF: 0x00216CAD (cond_jump)
  0x00216CE0  8365fcfd                and      dword ptr [ebp - 4], 0xfffffffd 
  0x00216CE4  c7430c02000000          mov      dword ptr [ebx + 0xc], 2       
  0x00216CEB  8b06                    mov      eax, dword ptr [esi]           
  0x00216CED  c7400806000000          mov      dword ptr [eax + 8], 6         
                                        ; XREF: 0x00216C9B (cond_jump)
  0x00216CF4  f645fc04                test     byte ptr [ebp - 4], 4          
  0x00216CF8  6a04                    push     4                              
  0x00216CFA  5f                      pop      edi                            
  0x00216CFB  740a                    je       0x216d07                       
  0x00216CFD  8365fcfb                and      dword ptr [ebp - 4], 0xfffffffb 
  0x00216D01  897b0c                  mov      dword ptr [ebx + 0xc], edi     
  0x00216D04  897b14                  mov      dword ptr [ebx + 0x14], edi    
                                        ; XREF: 0x00216CFB (cond_jump)
  0x00216D07  8bce                    mov      ecx, esi                       
  0x00216D09  e83ffaffff              call     0x21674d                       ; -> sub_0021674D
  0x00216D0E  8bd0                    mov      edx, eax                       
  0x00216D10  8bce                    mov      ecx, esi                       
  0x00216D12  8955f8                  mov      dword ptr [ebp - 8], edx       
  0x00216D15  e8a5fcffff              call     0x2169bf                       ; -> sub_002169BF
  0x00216D1A  84c0                    test     al, al                         
  0x00216D1C  7404                    je       0x216d22                       
  0x00216D1E  c6450f01                mov      byte ptr [ebp + 0xf], 1        
                                        ; XREF: 0x00216D1C (cond_jump)
  0x00216D22  8b55f8                  mov      edx, dword ptr [ebp - 8]       
  0x00216D25  8bce                    mov      ecx, esi                       
  0x00216D27  e8ebfdffff              call     0x216b17                       ; -> sub_00216B17
  0x00216D2C  84c0                    test     al, al                         
  0x00216D2E  7404                    je       0x216d34                       
  0x00216D30  c6450f01                mov      byte ptr [ebp + 0xf], 1        
                                        ; XREF: 0x00216D2E (cond_jump)
  0x00216D34  8b55f8                  mov      edx, dword ptr [ebp - 8]       
  0x00216D37  8bce                    mov      ecx, esi                       
  0x00216D39  e860feffff              call     0x216b9e                       ; -> sub_00216B9E
  0x00216D3E  84c0                    test     al, al                         
  0x00216D40  7404                    je       0x216d46                       
  0x00216D42  c6450f01                mov      byte ptr [ebp + 0xf], 1        
                                        ; XREF: 0x00216D40 (cond_jump)
  0x00216D46  807d0f00                cmp      byte ptr [ebp + 0xf], 0        
  0x00216D4A  740a                    je       0x216d56                       
  0x00216D4C  8b06                    mov      eax, dword ptr [esi]           
  0x00216D4E  89780c                  mov      dword ptr [eax + 0xc], edi     
  0x00216D51  8b06                    mov      eax, dword ptr [esi]           
  0x00216D53  897810                  mov      dword ptr [eax + 0x10], edi    
                                        ; XREF: 0x00216D4A (cond_jump)
  0x00216D56  f645fc40                test     byte ptr [ebp - 4], 0x40       
  0x00216D5A  7412                    je       0x216d6e                       
  0x00216D5C  8bce                    mov      ecx, esi                       
  0x00216D5E  e809f4ffff              call     0x21616c                       ; -> sub_0021616C
  0x00216D63  8365fcbf                and      dword ptr [ebp - 4], 0xffffffbf 
  0x00216D67  c7430c40000000          mov      dword ptr [ebx + 0xc], 0x40    
                                        ; XREF: 0x00216D5A (cond_jump)
  0x00216D6E  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00216D71  85c0                    test     eax, eax                       
  0x00216D73  7403                    je       0x216d78                       
  0x00216D75  89430c                  mov      dword ptr [ebx + 0xc], eax     
                                        ; XREF: 0x00216D73 (cond_jump)
  0x00216D78  5f                      pop      edi                            
  0x00216D79  5e                      pop      esi                            
  0x00216D7A  c7431000000080          mov      dword ptr [ebx + 0x10], 0x80000000 
  0x00216D81  5b                      pop      ebx                            
  0x00216D82  c9                      leave                                   
  0x00216D83  c21000                  ret      0x10                           
; end of function

; ============================================================
; Function: sub_00216D86
; Start: 0x00216D86  End: 0x00216D98  Size: 18 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00216D98
; ============================================================
sub_00216D86:
  0x00216D86  a1c8b9c600              mov      eax, dword ptr [0xc6b9c8]      
  0x00216D8B  85c0                    test     eax, eax                       
  0x00216D8D  7408                    je       0x216d97                       
  0x00216D8F  8b08                    mov      ecx, dword ptr [eax]           
  0x00216D91  890dc8b9c600            mov      dword ptr [0xc6b9c8], ecx      
                                        ; XREF: 0x00216D8D (cond_jump)
  0x00216D97  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00216D98
; Start: 0x00216D98  End: 0x00216DC5  Size: 45 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00216D86
; Called by: sub_00215371
; ============================================================
sub_00216D98:
  0x00216D98  55                      push     ebp                            
  0x00216D99  8bec                    mov      ebp, esp                       
  0x00216D9B  83ec14                  sub      esp, 0x14                      
  0x00216D9E  a1ccb9c600              mov      eax, dword ptr [0xc6b9cc]      
  0x00216DA3  53                      push     ebx                            
  0x00216DA4  57                      push     edi                            
  0x00216DA5  8bda                    mov      ebx, edx                       
  0x00216DA7  894dec                  mov      dword ptr [ebp - 0x14], ecx    
  0x00216DAA  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x00216DAD  e8d4ffffff              call     0x216d86                       ; -> sub_00216D86
  0x00216DB2  8bf8                    mov      edi, eax                       
  0x00216DB4  85ff                    test     edi, edi                       
  0x00216DB6  897df4                  mov      dword ptr [ebp - 0xc], edi     
  0x00216DB9  750a                    jne      0x216dc5                       
  0x00216DBB  bf00010080              mov      edi, 0x80000100                
  0x00216DC0  e977010000              jmp      0x216f3c                       
; end of function
                                        ; XREF: 0x00216DB9 (cond_jump)
  0x00216DC5  56                      push     esi                            
  0x00216DC6  8b75f8                  mov      esi, dword ptr [ebp - 8]       
  0x00216DC9  c1e606                  shl      esi, 6                         
  0x00216DCC  33c0                    xor      eax, eax                       
  0x00216DCE  8d4e30                  lea      ecx, [esi + 0x30]              
  0x00216DD1  8bd1                    mov      edx, ecx                       
  0x00216DD3  c1e902                  shr      ecx, 2                         
  0x00216DD6  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x00216DD8  8bca                    mov      ecx, edx                       
  0x00216DDA  83e103                  and      ecx, 3                         
  0x00216DDD  f3aa                    rep stosb byte ptr es:[edi], al          
  0x00216DDF  8b45f4                  mov      eax, dword ptr [ebp - 0xc]     
  0x00216DE2  03f0                    add      esi, eax                       
  0x00216DE4  89462c                  mov      dword ptr [esi + 0x2c], eax    
  0x00216DE7  8bc6                    mov      eax, esi                       
  0x00216DE9  2b05a0b9c600            sub      eax, dword ptr [0xc6b9a0]      
  0x00216DEF  c6461101                mov      byte ptr [esi + 0x11], 1       
  0x00216DF3  894614                  mov      dword ptr [esi + 0x14], eax    
  0x00216DF6  33c0                    xor      eax, eax                       
  0x00216DF8  c6461301                mov      byte ptr [esi + 0x13], 1       
  0x00216DFC  668b4316                mov      ax, word ptr [ebx + 0x16]      
  0x00216E00  6a00                    push     0                              
  0x00216E02  6a01                    push     1                              
  0x00216E04  50                      push     eax                            
  0x00216E05  e8dfbaffff              call     0x2128e9                       ; -> sub_002128E9
  0x00216E0A  8b55f8                  mov      edx, dword ptr [ebp - 8]       
  0x00216E0D  66894622                mov      word ptr [esi + 0x22], ax      
  0x00216E11  885624                  mov      byte ptr [esi + 0x24], dl      
  0x00216E14  8a4318                  mov      al, byte ptr [ebx + 0x18]      
  0x00216E17  2401                    and      al, 1                          
  0x00216E19  884610                  mov      byte ptr [esi + 0x10], al      
  0x00216E1C  33c0                    xor      eax, eax                       
  0x00216E1E  8a4314                  mov      al, byte ptr [ebx + 0x14]      
  0x00216E21  6a00                    push     0                              
  0x00216E23  3306                    xor      eax, dword ptr [esi]           
  0x00216E25  83e07f                  and      eax, 0x7f                      
  0x00216E28  3106                    xor      dword ptr [esi], eax           
  0x00216E2A  0fb64315                movzx    eax, byte ptr [ebx + 0x15]     
  0x00216E2E  8b0e                    mov      ecx, dword ptr [esi]           
  0x00216E30  c1e007                  shl      eax, 7                         
  0x00216E33  33c1                    xor      eax, ecx                       
  0x00216E35  2580070000              and      eax, 0x780                     
  0x00216E3A  33c1                    xor      eax, ecx                       
  0x00216E3C  8906                    mov      dword ptr [esi], eax           
  0x00216E3E  f6431580                test     byte ptr [ebx + 0x15], 0x80    
  0x00216E42  59                      pop      ecx                            
  0x00216E43  0f95c1                  setne    cl                             
  0x00216E46  25ffc7ffff              and      eax, 0xffffc7ff                
  0x00216E4B  41                      inc      ecx                            
  0x00216E4C  83e103                  and      ecx, 3                         
  0x00216E4F  83c918                  or       ecx, 0x18                      
  0x00216E52  c1e10b                  shl      ecx, 0xb                       
  0x00216E55  0bc8                    or       ecx, eax                       
  0x00216E57  890e                    mov      dword ptr [esi], ecx           
  0x00216E59  0fb74316                movzx    eax, word ptr [ebx + 0x16]     
  0x00216E5D  c1e010                  shl      eax, 0x10                      
  0x00216E60  33c1                    xor      eax, ecx                       
  0x00216E62  250000ff07              and      eax, 0x7ff0000                 
  0x00216E67  33c1                    xor      eax, ecx                       
  0x00216E69  8b4e08                  mov      ecx, dword ptr [esi + 8]       
  0x00216E6C  8906                    mov      dword ptr [esi], eax           
  0x00216E6E  8b462c                  mov      eax, dword ptr [esi + 0x2c]    
  0x00216E71  2b05a0b9c600            sub      eax, dword ptr [0xc6b9a0]      
  0x00216E77  33c8                    xor      ecx, eax                       
  0x00216E79  83e10f                  and      ecx, 0xf                       
  0x00216E7C  33c8                    xor      ecx, eax                       
  0x00216E7E  894e08                  mov      dword ptr [esi + 8], ecx       
  0x00216E81  32c9                    xor      cl, cl                         
  0x00216E83  85d2                    test     edx, edx                       
  0x00216E85  894604                  mov      dword ptr [esi + 4], eax       
  0x00216E88  884dff                  mov      byte ptr [ebp - 1], cl         
  0x00216E8B  765d                    jbe      0x216eea                       
  0x00216E8D  33c0                    xor      eax, eax                       
                                        ; XREF: 0x00216EE8 (cond_jump)
  0x00216E8F  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216E92  c1e006                  shl      eax, 6                         
  0x00216E95  8d3c10                  lea      edi, [eax + edx]               
  0x00216E98  897df0                  mov      dword ptr [ebp - 0x10], edi    
  0x00216E9B  2b3da0b9c600            sub      edi, dword ptr [0xc6b9a0]      
  0x00216EA1  8b55f0                  mov      edx, dword ptr [ebp - 0x10]    
  0x00216EA4  83c740                  add      edi, 0x40                      
  0x00216EA7  fec9                    dec      cl                             
  0x00216EA9  884a2d                  mov      byte ptr [edx + 0x2d], cl      
  0x00216EAC  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216EAF  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x00216EB2  884c102c                mov      byte ptr [eax + edx + 0x2c], cl 
  0x00216EB6  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216EB9  89741020                mov      dword ptr [eax + edx + 0x20], esi 
  0x00216EBD  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216EC0  8364102800              and      dword ptr [eax + edx + 0x28], 0 
  0x00216EC5  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216EC8  8364102400              and      dword ptr [eax + edx + 0x24], 0 
  0x00216ECD  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216ED0  03d0                    add      edx, eax                       
  0x00216ED2  804a0201                or       byte ptr [edx + 2], 1          
  0x00216ED6  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216ED9  fec1                    inc      cl                             
  0x00216EDB  897c1008                mov      dword ptr [eax + edx + 8], edi 
  0x00216EDF  0fb6c1                  movzx    eax, cl                        
  0x00216EE2  3b45f8                  cmp      eax, dword ptr [ebp - 8]       
  0x00216EE5  884dff                  mov      byte ptr [ebp - 1], cl         
  0x00216EE8  72a5                    jb       0x216e8f                       
                                        ; XREF: 0x00216E8B (cond_jump)
  0x00216EEA  8b562c                  mov      edx, dword ptr [esi + 0x2c]    
  0x00216EED  0fb6c1                  movzx    eax, cl                        
  0x00216EF0  c1e006                  shl      eax, 6                         
  0x00216EF3  836410c800              and      dword ptr [eax + edx - 0x38], 0 
  0x00216EF8  8b462c                  mov      eax, dword ptr [esi + 0x2c]    
  0x00216EFB  fec9                    dec      cl                             
  0x00216EFD  88482d                  mov      byte ptr [eax + 0x2d], cl      
  0x00216F00  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00216F06  8b4dec                  mov      ecx, dword ptr [ebp - 0x14]    
  0x00216F09  8bd6                    mov      edx, esi                       
  0x00216F0B  8845ff                  mov      byte ptr [ebp - 1], al         
  0x00216F0E  e84ff4ffff              call     0x216362                       ; -> sub_00216362
  0x00216F13  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x00216F16  8bf8                    mov      edi, eax                       
  0x00216F18  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00216F1E  85ff                    test     edi, edi                       
  0x00216F20  7c05                    jl       0x216f27                       
  0x00216F22  897310                  mov      dword ptr [ebx + 0x10], esi    
  0x00216F25  eb14                    jmp      0x216f3b                       
                                        ; XREF: 0x00216F20 (cond_jump)
  0x00216F27  83631000                and      dword ptr [ebx + 0x10], 0      
  0x00216F2B  8b0dc8b9c600            mov      ecx, dword ptr [0xc6b9c8]      
  0x00216F31  8b45f4                  mov      eax, dword ptr [ebp - 0xc]     
  0x00216F34  8908                    mov      dword ptr [eax], ecx           
  0x00216F36  a3c8b9c600              mov      dword ptr [0xc6b9c8], eax      
                                        ; XREF: 0x00216F25 (jump)
  0x00216F3B  5e                      pop      esi                            
                                        ; XREF: 0x00216DC0 (jump)
  0x00216F3C  897b04                  mov      dword ptr [ebx + 4], edi       
  0x00216F3F  8bc7                    mov      eax, edi                       
  0x00216F41  5f                      pop      edi                            
  0x00216F42  5b                      pop      ebx                            
  0x00216F43  c9                      leave                                   
  0x00216F44  c3                      ret                                     

; ============================================================
; Function: sub_00216F45
; Start: 0x00216F45  End: 0x00216F89  Size: 68 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00215188, sub_0021653E
; Called by: sub_00215371
; ============================================================
sub_00216F45:
  0x00216F45  53                      push     ebx                            
  0x00216F46  55                      push     ebp                            
  0x00216F47  56                      push     esi                            
  0x00216F48  8bf2                    mov      esi, edx                       
  0x00216F4A  8b6e10                  mov      ebp, dword ptr [esi + 0x10]    
  0x00216F4D  57                      push     edi                            
  0x00216F4E  8bf9                    mov      edi, ecx                       
  0x00216F50  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00216F56  8bd5                    mov      edx, ebp                       
  0x00216F58  8bcf                    mov      ecx, edi                       
  0x00216F5A  8ad8                    mov      bl, al                         
  0x00216F5C  e8ddf5ffff              call     0x21653e                       ; -> sub_0021653E
  0x00216F61  8bd5                    mov      edx, ebp                       
  0x00216F63  8bcf                    mov      ecx, edi                       
  0x00216F65  e81ee2ffff              call     0x215188                       ; -> sub_00215188
  0x00216F6A  8d8734040000            lea      eax, [edi + 0x434]             
  0x00216F70  8b08                    mov      ecx, dword ptr [eax]           
  0x00216F72  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x00216F75  8acb                    mov      cl, bl                         
  0x00216F77  8930                    mov      dword ptr [eax], esi           
  0x00216F79  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00216F7F  5f                      pop      edi                            
  0x00216F80  5e                      pop      esi                            
  0x00216F81  5d                      pop      ebp                            
  0x00216F82  b800000040              mov      eax, 0x40000000                
  0x00216F87  5b                      pop      ebx                            
  0x00216F88  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00216F89
; Start: 0x00216F89  End: 0x00217012  Size: 137 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_002128D5
; Called by: sub_00216B9E
; ============================================================
sub_00216F89:
  0x00216F89  53                      push     ebx                            
  0x00216F8A  55                      push     ebp                            
  0x00216F8B  8bea                    mov      ebp, edx                       
  0x00216F8D  56                      push     esi                            
  0x00216F8E  8b7510                  mov      esi, dword ptr [ebp + 0x10]    
  0x00216F91  8a4625                  mov      al, byte ptr [esi + 0x25]      
  0x00216F94  0fb65e26                movzx    ebx, byte ptr [esi + 0x26]     
  0x00216F98  0fb6c8                  movzx    ecx, al                        
  0x00216F9B  2bd9                    sub      ebx, ecx                       
  0x00216F9D  0fb64e24                movzx    ecx, byte ptr [esi + 0x24]     
  0x00216FA1  03d9                    add      ebx, ecx                       
  0x00216FA3  84c0                    test     al, al                         
  0x00216FA5  7444                    je       0x216feb                       
  0x00216FA7  57                      push     edi                            
                                        ; XREF: 0x00216FE8 (cond_jump)
  0x00216FA8  0fb64e24                movzx    ecx, byte ptr [esi + 0x24]     
  0x00216FAC  8bc3                    mov      eax, ebx                       
  0x00216FAE  33d2                    xor      edx, edx                       
  0x00216FB0  f7f1                    div      ecx                            
  0x00216FB2  fe4e25                  dec      byte ptr [esi + 0x25]          
  0x00216FB5  6a01                    push     1                              
  0x00216FB7  8bda                    mov      ebx, edx                       
  0x00216FB9  8bfb                    mov      edi, ebx                       
  0x00216FBB  c1e706                  shl      edi, 6                         
  0x00216FBE  037e2c                  add      edi, dword ptr [esi + 0x2c]    
  0x00216FC1  43                      inc      ebx                            
  0x00216FC2  ff7704                  push     dword ptr [edi + 4]            
  0x00216FC5  ff15907b2100            call     dword ptr [0x217b90]           ; -> xbox_MmLockUnlockPhysicalPage
  0x00216FCB  8b470c                  mov      eax, dword ptr [edi + 0xc]     
  0x00216FCE  8b4f04                  mov      ecx, dword ptr [edi + 4]       
  0x00216FD1  33c8                    xor      ecx, eax                       
  0x00216FD3  f7c100f0ffff            test     ecx, 0xfffff000                
  0x00216FD9  7409                    je       0x216fe4                       
  0x00216FDB  6a01                    push     1                              
  0x00216FDD  50                      push     eax                            
  0x00216FDE  ff15907b2100            call     dword ptr [0x217b90]           ; -> xbox_MmLockUnlockPhysicalPage
                                        ; XREF: 0x00216FD9 (cond_jump)
  0x00216FE4  807e2500                cmp      byte ptr [esi + 0x25], 0       
  0x00216FE8  75be                    jne      0x216fa8                       
  0x00216FEA  5f                      pop      edi                            
                                        ; XREF: 0x00216FA5 (cond_jump)
  0x00216FEB  0fb64624                movzx    eax, byte ptr [esi + 0x24]     
  0x00216FEF  fe4e25                  dec      byte ptr [esi + 0x25]          
  0x00216FF2  c1e006                  shl      eax, 6                         
  0x00216FF5  2bf0                    sub      esi, eax                       
  0x00216FF7  a1c8b9c600              mov      eax, dword ptr [0xc6b9c8]      
  0x00216FFC  8906                    mov      dword ptr [esi], eax           
  0x00216FFE  8935c8b9c600            mov      dword ptr [0xc6b9c8], esi      
  0x00217004  83650400                and      dword ptr [ebp + 4], 0         
  0x00217008  55                      push     ebp                            
  0x00217009  e8c7b8ffff              call     0x2128d5                       ; -> sub_002128D5
  0x0021700E  5e                      pop      esi                            
  0x0021700F  5d                      pop      ebp                            
  0x00217010  5b                      pop      ebx                            
  0x00217011  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00217012
; Start: 0x00217012  End: 0x00217049  Size: 55 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_00215371
; ============================================================
sub_00217012:
  0x00217012  55                      push     ebp                            
  0x00217013  8bec                    mov      ebp, esp                       
  0x00217015  83ec24                  sub      esp, 0x24                      
  0x00217018  8365ec00                and      dword ptr [ebp - 0x14], 0      
  0x0021701C  56                      push     esi                            
  0x0021701D  8bf2                    mov      esi, edx                       
  0x0021701F  57                      push     edi                            
  0x00217020  8b7e10                  mov      edi, dword ptr [esi + 0x10]    
  0x00217023  8975f0                  mov      dword ptr [ebp - 0x10], esi    
  0x00217026  894ddc                  mov      dword ptr [ebp - 0x24], ecx    
  0x00217029  897de0                  mov      dword ptr [ebp - 0x20], edi    
  0x0021702C  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00217032  8a4f24                  mov      cl, byte ptr [edi + 0x24]      
  0x00217035  3a4f25                  cmp      cl, byte ptr [edi + 0x25]      
  0x00217038  8845ff                  mov      byte ptr [ebp - 1], al         
  0x0021703B  750c                    jne      0x217049                       
  0x0021703D  c745ec000d00c0          mov      dword ptr [ebp - 0x14], 0xc0000d00 
  0x00217044  e915010000              jmp      0x21715e                       
; end of function
                                        ; XREF: 0x0021703B (cond_jump)
  0x00217049  0fb64726                movzx    eax, byte ptr [edi + 0x26]     
  0x0021704D  53                      push     ebx                            
  0x0021704E  8bd8                    mov      ebx, eax                       
  0x00217050  c1e306                  shl      ebx, 6                         
  0x00217053  035f2c                  add      ebx, dword ptr [edi + 0x2c]    
  0x00217056  40                      inc      eax                            
  0x00217057  99                      cdq                                     
  0x00217058  0fb6c9                  movzx    ecx, cl                        
  0x0021705B  f7f9                    idiv     ecx                            
  0x0021705D  8b7618                  mov      esi, dword ptr [esi + 0x18]    
  0x00217060  8365f400                and      dword ptr [ebp - 0xc], 0       
  0x00217064  8975e4                  mov      dword ptr [ebp - 0x1c], esi    
  0x00217067  885726                  mov      byte ptr [edi + 0x26], dl      
  0x0021706A  8b4618                  mov      eax, dword ptr [esi + 0x18]    
  0x0021706D  894324                  mov      dword ptr [ebx + 0x24], eax    
  0x00217070  8b461c                  mov      eax, dword ptr [esi + 0x1c]    
  0x00217073  894328                  mov      dword ptr [ebx + 0x28], eax    
  0x00217076  8b45f0                  mov      eax, dword ptr [ebp - 0x10]    
  0x00217079  897b20                  mov      dword ptr [ebx + 0x20], edi    
  0x0021707C  0fb64014                movzx    eax, byte ptr [eax + 0x14]     
  0x00217080  c1e015                  shl      eax, 0x15                      
  0x00217083  3303                    xor      eax, dword ptr [ebx]           
  0x00217085  250000e000              and      eax, 0xe00000                  
  0x0021708A  3103                    xor      dword ptr [ebx], eax           
  0x0021708C  8b0e                    mov      ecx, dword ptr [esi]           
  0x0021708E  8b03                    mov      eax, dword ptr [ebx]           
  0x00217090  49                      dec      ecx                            
  0x00217091  c1e118                  shl      ecx, 0x18                      
  0x00217094  33c8                    xor      ecx, eax                       
  0x00217096  81e100000007            and      ecx, 0x7000000                 
  0x0021709C  33c8                    xor      ecx, eax                       
  0x0021709E  890b                    mov      dword ptr [ebx], ecx           
  0x002170A0  8b4604                  mov      eax, dword ptr [esi + 4]       
  0x002170A3  25ff0f0000              and      eax, 0xfff                     
  0x002170A8  833e00                  cmp      dword ptr [esi], 0             
  0x002170AB  8945e8                  mov      dword ptr [ebp - 0x18], eax    
  0x002170AE  762b                    jbe      0x2170db                       
  0x002170B0  8d4e08                  lea      ecx, [esi + 8]                 
  0x002170B3  894df8                  mov      dword ptr [ebp - 8], ecx       
  0x002170B6  8d4b10                  lea      ecx, [ebx + 0x10]              
                                        ; XREF: 0x002170D9 (cond_jump)
  0x002170B9  8bd0                    mov      edx, eax                       
  0x002170BB  6681ca00e0              or       dx, 0xe000                     
  0x002170C0  668911                  mov      word ptr [ecx], dx             
  0x002170C3  8b55f8                  mov      edx, dword ptr [ebp - 8]       
  0x002170C6  0fb712                  movzx    edx, word ptr [edx]            
  0x002170C9  8345f802                add      dword ptr [ebp - 8], 2         
  0x002170CD  03c2                    add      eax, edx                       
  0x002170CF  ff45f4                  inc      dword ptr [ebp - 0xc]          
  0x002170D2  8b55f4                  mov      edx, dword ptr [ebp - 0xc]     
  0x002170D5  41                      inc      ecx                            
  0x002170D6  41                      inc      ecx                            
  0x002170D7  3b16                    cmp      edx, dword ptr [esi]           
  0x002170D9  72de                    jb       0x2170b9                       
                                        ; XREF: 0x002170AE (cond_jump)
  0x002170DB  2b45e8                  sub      eax, dword ptr [ebp - 0x18]    
  0x002170DE  6a00                    push     0                              
  0x002170E0  50                      push     eax                            
  0x002170E1  ff7604                  push     dword ptr [esi + 4]            
  0x002170E4  8945e8                  mov      dword ptr [ebp - 0x18], eax    
  0x002170E7  ff15887b2100            call     dword ptr [0x217b88]           ; -> xbox_MmLockUnlockBufferPages
  0x002170ED  ff7604                  push     dword ptr [esi + 4]            
  0x002170F0  ff15847b2100            call     dword ptr [0x217b84]           ; -> xbox_MmGetPhysicalAddress
  0x002170F6  8b4de8                  mov      ecx, dword ptr [ebp - 0x18]    
  0x002170F9  894304                  mov      dword ptr [ebx + 4], eax       
  0x002170FC  8b4604                  mov      eax, dword ptr [esi + 4]       
  0x002170FF  8d4408ff                lea      eax, [eax + ecx - 1]           
  0x00217103  50                      push     eax                            
  0x00217104  ff15847b2100            call     dword ptr [0x217b84]           ; -> xbox_MmGetPhysicalAddress
  0x0021710A  89430c                  mov      dword ptr [ebx + 0xc], eax     
  0x0021710D  f6471001                test     byte ptr [edi + 0x10], 1       
  0x00217111  7410                    je       0x217123                       
  0x00217113  8d7310                  lea      esi, [ebx + 0x10]              
  0x00217116  8d7b30                  lea      edi, [ebx + 0x30]              
  0x00217119  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x0021711A  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x0021711B  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x0021711C  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x0021711D  8b75e4                  mov      esi, dword ptr [ebp - 0x1c]    
  0x00217120  8b7de0                  mov      edi, dword ptr [ebp - 0x20]    
                                        ; XREF: 0x00217111 (cond_jump)
  0x00217123  f6471002                test     byte ptr [edi + 0x10], 2       
  0x00217127  7420                    je       0x217149                       
  0x00217129  8b4ddc                  mov      ecx, dword ptr [ebp - 0x24]    
  0x0021712C  e81cf6ffff              call     0x21674d                       ; -> sub_0021674D
  0x00217131  8b4f28                  mov      ecx, dword ptr [edi + 0x28]    
  0x00217134  40                      inc      eax                            
  0x00217135  8bd1                    mov      edx, ecx                       
  0x00217137  2bd0                    sub      edx, eax                       
  0x00217139  85d2                    test     edx, edx                       
  0x0021713B  7e02                    jle      0x21713f                       
  0x0021713D  8bc1                    mov      eax, ecx                       
                                        ; XREF: 0x0021713B (cond_jump)
  0x0021713F  668903                  mov      word ptr [ebx], ax             
  0x00217142  8b0e                    mov      ecx, dword ptr [esi]           
  0x00217144  03c8                    add      ecx, eax                       
  0x00217146  894f28                  mov      dword ptr [edi + 0x28], ecx    
                                        ; XREF: 0x00217127 (cond_jump)
  0x00217149  fe4725                  inc      byte ptr [edi + 0x25]          
  0x0021714C  8a4725                  mov      al, byte ptr [edi + 0x25]      
  0x0021714F  3a4724                  cmp      al, byte ptr [edi + 0x24]      
  0x00217152  8b75f0                  mov      esi, dword ptr [ebp - 0x10]    
  0x00217155  7406                    je       0x21715d                       
  0x00217157  8b4308                  mov      eax, dword ptr [ebx + 8]       
  0x0021715A  894704                  mov      dword ptr [edi + 4], eax       
                                        ; XREF: 0x00217155 (cond_jump)
  0x0021715D  5b                      pop      ebx                            
                                        ; XREF: 0x00217044 (jump)
  0x0021715E  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x00217161  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00217167  8b7dec                  mov      edi, dword ptr [ebp - 0x14]    
  0x0021716A  56                      push     esi                            
  0x0021716B  897e04                  mov      dword ptr [esi + 4], edi       
  0x0021716E  e862b7ffff              call     0x2128d5                       ; -> sub_002128D5
  0x00217173  8bc7                    mov      eax, edi                       
  0x00217175  5f                      pop      edi                            
  0x00217176  5e                      pop      esi                            
  0x00217177  c9                      leave                                   
  0x00217178  c3                      ret                                     

; ============================================================
; Function: sub_00217179
; Start: 0x00217179  End: 0x002171AC  Size: 51 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_00215371
; ============================================================
sub_00217179:
  0x00217179  55                      push     ebp                            
  0x0021717A  8bec                    mov      ebp, esp                       
  0x0021717C  83ec18                  sub      esp, 0x18                      
  0x0021717F  8365f000                and      dword ptr [ebp - 0x10], 0      
  0x00217183  53                      push     ebx                            
  0x00217184  8b1d007b2100            mov      ebx, dword ptr [0x217b00]      
  0x0021718A  56                      push     esi                            
  0x0021718B  57                      push     edi                            
  0x0021718C  8bfa                    mov      edi, edx                       
  0x0021718E  8b7710                  mov      esi, dword ptr [edi + 0x10]    
  0x00217191  897df4                  mov      dword ptr [ebp - 0xc], edi     
  0x00217194  894df8                  mov      dword ptr [ebp - 8], ecx       
  0x00217197  ffd3                    call     ebx                            
  0x00217199  f6461002                test     byte ptr [esi + 0x10], 2       
  0x0021719D  8845ff                  mov      byte ptr [ebp - 1], al         
  0x002171A0  740a                    je       0x2171ac                       
  0x002171A2  be000e00c0              mov      esi, 0xc0000e00                
  0x002171A7  e9cc000000              jmp      0x217278                       
; end of function
                                        ; XREF: 0x002171A0 (cond_jump)
  0x002171AC  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x002171AF  e899f5ffff              call     0x21674d                       ; -> sub_0021674D
  0x002171B4  8a5610                  mov      dl, byte ptr [esi + 0x10]      
  0x002171B7  f6c204                  test     dl, 4                          
  0x002171BA  743a                    je       0x2171f6                       
  0x002171BC  80e2fb                  and      dl, 0xfb                       
  0x002171BF  39461c                  cmp      dword ptr [esi + 0x1c], eax    
  0x002171C2  885610                  mov      byte ptr [esi + 0x10], dl      
  0x002171C5  7527                    jne      0x2171ee                       
  0x002171C7  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x002171CA  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002171D0  834decff                or       dword ptr [ebp - 0x14], 0xffffffff 
  0x002171D4  8d45e8                  lea      eax, [ebp - 0x18]              
  0x002171D7  50                      push     eax                            
  0x002171D8  6a00                    push     0                              
  0x002171DA  6a00                    push     0                              
  0x002171DC  c745e8f0d8ffff          mov      dword ptr [ebp - 0x18], 0xffffd8f0 
  0x002171E3  ff15c07a2100            call     dword ptr [0x217ac0]           ; -> xbox_KeCancelTimer
  0x002171E9  ffd3                    call     ebx                            
  0x002171EB  8845ff                  mov      byte ptr [ebp - 1], al         
                                        ; XREF: 0x002171C5 (cond_jump)
  0x002171EE  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x002171F1  e857f5ffff              call     0x21674d                       ; -> sub_0021674D
                                        ; XREF: 0x002171BA (cond_jump)
  0x002171F6  f6461001                test     byte ptr [esi + 0x10], 1       
  0x002171FA  740f                    je       0x21720b                       
  0x002171FC  8a4e24                  mov      cl, byte ptr [esi + 0x24]      
  0x002171FF  3a4e25                  cmp      cl, byte ptr [esi + 0x25]      
  0x00217202  7407                    je       0x21720b                       
  0x00217204  be001000c0              mov      esi, 0xc0001000                
  0x00217209  eb6d                    jmp      0x217278                       
                                        ; XREF: 0x002171FA (cond_jump), 0x00217202 (cond_jump)
  0x0021720B  f6471801                test     byte ptr [edi + 0x18], 1       
  0x0021720F  8d4801                  lea      ecx, [eax + 1]                 
  0x00217212  7512                    jne      0x217226                       
  0x00217214  8b5714                  mov      edx, dword ptr [edi + 0x14]    
  0x00217217  8bc2                    mov      eax, edx                       
  0x00217219  2bc1                    sub      eax, ecx                       
  0x0021721B  7874                    js       0x217291                       
  0x0021721D  3d00040000              cmp      eax, 0x400                     
  0x00217222  7f6d                    jg       0x217291                       
  0x00217224  8bca                    mov      ecx, edx                       
                                        ; XREF: 0x00217212 (cond_jump)
  0x00217226  8a5624                  mov      dl, byte ptr [esi + 0x24]      
  0x00217229  8ac2                    mov      al, dl                         
  0x0021722B  2a4625                  sub      al, byte ptr [esi + 0x25]      
  0x0021722E  0fb6fa                  movzx    edi, dl                        
  0x00217231  024626                  add      al, byte ptr [esi + 0x26]      
  0x00217234  0fb6c0                  movzx    eax, al                        
  0x00217237  99                      cdq                                     
  0x00217238  f7ff                    idiv     edi                            
  0x0021723A  8b7e2c                  mov      edi, dword ptr [esi + 0x2c]    
                                        ; XREF: 0x00217265 (cond_jump)
  0x0021723D  0fb6d2                  movzx    edx, dl                        
  0x00217240  8bc2                    mov      eax, edx                       
  0x00217242  c1e006                  shl      eax, 6                         
  0x00217245  66890c07                mov      word ptr [edi + eax], cx       
  0x00217249  8b7e2c                  mov      edi, dword ptr [esi + 0x2c]    
  0x0021724C  0fb6440703              movzx    eax, byte ptr [edi + eax + 3]  
  0x00217251  0fb65e24                movzx    ebx, byte ptr [esi + 0x24]     
  0x00217255  83e007                  and      eax, 7                         
  0x00217258  8d4c0101                lea      ecx, [ecx + eax + 1]           
  0x0021725C  8d4201                  lea      eax, [edx + 1]                 
  0x0021725F  99                      cdq                                     
  0x00217260  f7fb                    idiv     ebx                            
  0x00217262  3a5626                  cmp      dl, byte ptr [esi + 0x26]      
  0x00217265  75d6                    jne      0x21723d                       
  0x00217267  806601bf                and      byte ptr [esi + 1], 0xbf       
  0x0021726B  804e1002                or       byte ptr [esi + 0x10], 2       
  0x0021726F  8b7df4                  mov      edi, dword ptr [ebp - 0xc]     
  0x00217272  894e28                  mov      dword ptr [esi + 0x28], ecx    
  0x00217275  8b75f0                  mov      esi, dword ptr [ebp - 0x10]    
                                        ; XREF: 0x002171A7 (jump), 0x00217209 (jump), 0x00217296 (jump)
  0x00217278  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x0021727B  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x00217281  57                      push     edi                            
  0x00217282  897704                  mov      dword ptr [edi + 4], esi       
  0x00217285  e84bb6ffff              call     0x2128d5                       ; -> sub_002128D5
  0x0021728A  5f                      pop      edi                            
  0x0021728B  8bc6                    mov      eax, esi                       
  0x0021728D  5e                      pop      esi                            
  0x0021728E  5b                      pop      ebx                            
  0x0021728F  c9                      leave                                   
  0x00217290  c3                      ret                                     
                                        ; XREF: 0x0021721B (cond_jump), 0x00217222 (cond_jump)
  0x00217291  be000b00c0              mov      esi, 0xc0000b00                
  0x00217296  ebe0                    jmp      0x217278                       

; ============================================================
; Function: sub_00217298
; Start: 0x00217298  End: 0x002172D2  Size: 58 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_0021674D
; ============================================================
sub_00217298:
  0x00217298  51                      push     ecx                            
  0x00217299  53                      push     ebx                            
  0x0021729A  55                      push     ebp                            
  0x0021729B  56                      push     esi                            
  0x0021729C  57                      push     edi                            
  0x0021729D  8bfa                    mov      edi, edx                       
  0x0021729F  8b7710                  mov      esi, dword ptr [edi + 0x10]    
  0x002172A2  8bd9                    mov      ebx, ecx                       
  0x002172A4  33ed                    xor      ebp, ebp                       
  0x002172A6  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x002172AC  f6461002                test     byte ptr [esi + 0x10], 2       
  0x002172B0  88442413                mov      byte ptr [esp + 0x13], al      
  0x002172B4  741c                    je       0x2172d2                       
  0x002172B6  804e0140                or       byte ptr [esi + 1], 0x40       
  0x002172BA  8bcb                    mov      ecx, ebx                       
  0x002172BC  e88cf4ffff              call     0x21674d                       ; -> sub_0021674D
  0x002172C1  40                      inc      eax                            
  0x002172C2  40                      inc      eax                            
  0x002172C3  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x002172C6  8a4610                  mov      al, byte ptr [esi + 0x10]      
  0x002172C9  24fd                    and      al, 0xfd                       
  0x002172CB  0c04                    or       al, 4                          
  0x002172CD  884610                  mov      byte ptr [esi + 0x10], al      
  0x002172D0  eb05                    jmp      0x2172d7                       
; end of function
                                        ; XREF: 0x002172B4 (cond_jump)
  0x002172D2  bd000f00c0              mov      ebp, 0xc0000f00                
                                        ; XREF: 0x002172D0 (jump)
  0x002172D7  8a4c2413                mov      cl, byte ptr [esp + 0x13]      
  0x002172DB  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002172E1  57                      push     edi                            
  0x002172E2  896f04                  mov      dword ptr [edi + 4], ebp       
  0x002172E5  e8ebb5ffff              call     0x2128d5                       ; -> sub_002128D5
  0x002172EA  5f                      pop      edi                            
  0x002172EB  5e                      pop      esi                            
  0x002172EC  8bc5                    mov      eax, ebp                       
  0x002172EE  5d                      pop      ebp                            
  0x002172EF  5b                      pop      ebx                            
  0x002172F0  59                      pop      ecx                            
  0x002172F1  c3                      ret                                     

; ============================================================
; Function: sub_002172F2
; Start: 0x002172F2  End: 0x00217399  Size: 167 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_0021674D
; Called by: sub_00216C78
; ============================================================
sub_002172F2:
  0x002172F2  55                      push     ebp                            
  0x002172F3  8bec                    mov      ebp, esp                       
  0x002172F5  83ec2c                  sub      esp, 0x2c                      
  0x002172F8  53                      push     ebx                            
  0x002172F9  8bda                    mov      ebx, edx                       
  0x002172FB  8b03                    mov      eax, dword ptr [ebx]           
  0x002172FD  56                      push     esi                            
  0x002172FE  8b5320                  mov      edx, dword ptr [ebx + 0x20]    
  0x00217301  83630800                and      dword ptr [ebx + 8], 0         
  0x00217305  8bf0                    mov      esi, eax                       
  0x00217307  c1ee1c                  shr      esi, 0x1c                      
  0x0021730A  8975d4                  mov      dword ptr [ebp - 0x2c], esi    
  0x0021730D  c1e818                  shr      eax, 0x18                      
  0x00217310  83e007                  and      eax, 7                         
  0x00217313  57                      push     edi                            
  0x00217314  40                      inc      eax                            
  0x00217315  8945d8                  mov      dword ptr [ebp - 0x28], eax    
  0x00217318  8b4328                  mov      eax, dword ptr [ebx + 0x28]    
  0x0021731B  8d7310                  lea      esi, [ebx + 0x10]              
  0x0021731E  8975f8                  mov      dword ptr [ebp - 8], esi       
  0x00217321  8d7ddc                  lea      edi, [ebp - 0x24]              
  0x00217324  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00217325  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00217326  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00217327  8945f0                  mov      dword ptr [ebp - 0x10], eax    
  0x0021732A  8b4324                  mov      eax, dword ptr [ebx + 0x24]    
  0x0021732D  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x0021732E  8b7a2c                  mov      edi, dword ptr [edx + 0x2c]    
  0x00217331  8945ec                  mov      dword ptr [ebp - 0x14], eax    
  0x00217334  0fb6432d                movzx    eax, byte ptr [ebx + 0x2d]     
  0x00217338  8bf3                    mov      esi, ebx                       
  0x0021733A  2b35a0b9c600            sub      esi, dword ptr [0xc6b9a0]      
  0x00217340  c1e006                  shl      eax, 6                         
  0x00217343  89743808                mov      dword ptr [eax + edi + 8], esi 
  0x00217347  f6421001                test     byte ptr [edx + 0x10], 1       
  0x0021734B  8955fc                  mov      dword ptr [ebp - 4], edx       
  0x0021734E  8975f4                  mov      dword ptr [ebp - 0xc], esi     
  0x00217351  7446                    je       0x217399                       
  0x00217353  e8f5f3ffff              call     0x21674d                       ; -> sub_0021674D
  0x00217358  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x0021735B  8bf0                    mov      esi, eax                       
  0x0021735D  8b4128                  mov      eax, dword ptr [ecx + 0x28]    
  0x00217360  46                      inc      esi                            
  0x00217361  8bd0                    mov      edx, eax                       
  0x00217363  2bd6                    sub      edx, esi                       
  0x00217365  7802                    js       0x217369                       
  0x00217367  8bf0                    mov      esi, eax                       
                                        ; XREF: 0x00217365 (cond_jump)
  0x00217369  8b7df8                  mov      edi, dword ptr [ebp - 8]       
  0x0021736C  668933                  mov      word ptr [ebx], si             
  0x0021736F  33c0                    xor      eax, eax                       
  0x00217371  8a4303                  mov      al, byte ptr [ebx + 3]         
  0x00217374  83e007                  and      eax, 7                         
  0x00217377  8d443001                lea      eax, [eax + esi + 1]           
  0x0021737B  894128                  mov      dword ptr [ecx + 0x28], eax    
  0x0021737E  8d7330                  lea      esi, [ebx + 0x30]              
  0x00217381  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00217382  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00217383  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00217384  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00217385  0fb64126                movzx    eax, byte ptr [ecx + 0x26]     
  0x00217389  0fb67124                movzx    esi, byte ptr [ecx + 0x24]     
  0x0021738D  40                      inc      eax                            
  0x0021738E  99                      cdq                                     
  0x0021738F  f7fe                    idiv     esi                            
  0x00217391  8b75f4                  mov      esi, dword ptr [ebp - 0xc]     
  0x00217394  885126                  mov      byte ptr [ecx + 0x26], dl      
  0x00217397  eb37                    jmp      0x2173d0                       
; end of function
                                        ; XREF: 0x00217351 (cond_jump)
  0x00217399  8b3d907b2100            mov      edi, dword ptr [0x217b90]      
  0x0021739F  6a01                    push     1                              
  0x002173A1  ff7304                  push     dword ptr [ebx + 4]            
  0x002173A4  ffd7                    call     edi                            
  0x002173A6  8b430c                  mov      eax, dword ptr [ebx + 0xc]     
  0x002173A9  8b4b04                  mov      ecx, dword ptr [ebx + 4]       
  0x002173AC  33c8                    xor      ecx, eax                       
  0x002173AE  f7c100f0ffff            test     ecx, 0xfffff000                
  0x002173B4  7405                    je       0x2173bb                       
  0x002173B6  6a01                    push     1                              
  0x002173B8  50                      push     eax                            
  0x002173B9  ffd7                    call     edi                            
                                        ; XREF: 0x002173B4 (cond_jump)
  0x002173BB  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x002173BE  8a4125                  mov      al, byte ptr [ecx + 0x25]      
  0x002173C1  3a4124                  cmp      al, byte ptr [ecx + 0x24]      
  0x002173C4  0f94c2                  sete     dl                             
  0x002173C7  fec8                    dec      al                             
  0x002173C9  84d2                    test     dl, dl                         
  0x002173CB  884125                  mov      byte ptr [ecx + 0x25], al      
  0x002173CE  7403                    je       0x2173d3                       
                                        ; XREF: 0x00217397 (jump)
  0x002173D0  897104                  mov      dword ptr [ecx + 4], esi       
                                        ; XREF: 0x002173CE (cond_jump)
  0x002173D3  ff75f0                  push     dword ptr [ebp - 0x10]         
  0x002173D6  8d45d4                  lea      eax, [ebp - 0x2c]              
  0x002173D9  50                      push     eax                            
  0x002173DA  ff55ec                  call     dword ptr [ebp - 0x14]         
  0x002173DD  5f                      pop      edi                            
  0x002173DE  5e                      pop      esi                            
  0x002173DF  5b                      pop      ebx                            
  0x002173E0  c9                      leave                                   
  0x002173E1  c3                      ret                                     

; ============================================================
; Function: sub_002173E2
; Start: 0x002173E2  End: 0x00217402  Size: 32 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_002173E2:
  0x002173E2  a1acb9c600              mov      eax, dword ptr [0xc6b9ac]      
  0x002173E7  85c0                    test     eax, eax                       
  0x002173E9  7409                    je       0x2173f4                       
  0x002173EB  8b4814                  mov      ecx, dword ptr [eax + 0x14]    
  0x002173EE  890dacb9c600            mov      dword ptr [0xc6b9ac], ecx      
                                        ; XREF: 0x002173E9 (cond_jump)
  0x002173F4  8a4c2404                mov      cl, byte ptr [esp + 4]         
  0x002173F8  806002fe                and      byte ptr [eax + 2], 0xfe       
  0x002173FC  88481f                  mov      byte ptr [eax + 0x1f], cl      
  0x002173FF  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_00217402
; Start: 0x00217402  End: 0x00217414  Size: 18 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_0021784B
; ============================================================
sub_00217402:
  0x00217402  668b442404              mov      ax, word ptr [esp + 4]         
  0x00217407  663905c2b9c600          cmp      word ptr [0xc6b9c2], ax        
  0x0021740E  7304                    jae      0x217414                       
  0x00217410  33c0                    xor      eax, eax                       
  0x00217412  eb0a                    jmp      0x21741e                       
; end of function
                                        ; XREF: 0x0021740E (cond_jump)
  0x00217414  662905c2b9c600          sub      word ptr [0xc6b9c2], ax        
  0x0021741B  33c0                    xor      eax, eax                       
  0x0021741D  40                      inc      eax                            
                                        ; XREF: 0x00217412 (jump)
  0x0021741E  c20400                  ret      4                              

; ============================================================
; Function: sub_00217421
; Start: 0x00217421  End: 0x00217433  Size: 18 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_002177FE
; ============================================================
sub_00217421:
  0x00217421  668b442404              mov      ax, word ptr [esp + 4]         
  0x00217426  663905c6b9c600          cmp      word ptr [0xc6b9c6], ax        
  0x0021742D  7304                    jae      0x217433                       
  0x0021742F  33c0                    xor      eax, eax                       
  0x00217431  eb0a                    jmp      0x21743d                       
; end of function
                                        ; XREF: 0x0021742D (cond_jump)
  0x00217433  662905c6b9c600          sub      word ptr [0xc6b9c6], ax        
  0x0021743A  33c0                    xor      eax, eax                       
  0x0021743C  40                      inc      eax                            
                                        ; XREF: 0x00217431 (jump)
  0x0021743D  c20400                  ret      4                              

; ============================================================
; Function: sub_00217440
; Start: 0x00217440  End: 0x00217473  Size: 51 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00217943
; ============================================================
sub_00217440:
  0x00217440  8b4914                  mov      ecx, dword ptr [ecx + 0x14]    
  0x00217443  56                      push     esi                            
  0x00217444  8bf2                    mov      esi, edx                       
  0x00217446  33d2                    xor      edx, edx                       
  0x00217448  668b5602                mov      dx, word ptr [esi + 2]         
  0x0021744C  33c0                    xor      eax, eax                       
  0x0021744E  81e2ff070000            and      edx, 0x7ff                     
  0x00217454  85c9                    test     ecx, ecx                       
  0x00217456  7410                    je       0x217468                       
  0x00217458  57                      push     edi                            
  0x00217459  0fb7fa                  movzx    edi, dx                        
  0x0021745C  33d2                    xor      edx, edx                       
  0x0021745E  8bc1                    mov      eax, ecx                       
  0x00217460  f7f7                    div      edi                            
  0x00217462  5f                      pop      edi                            
  0x00217463  85d2                    test     edx, edx                       
  0x00217465  7401                    je       0x217468                       
  0x00217467  40                      inc      eax                            
                                        ; XREF: 0x00217456 (cond_jump), 0x00217465 (cond_jump)
  0x00217468  807e1100                cmp      byte ptr [esi + 0x11], 0       
  0x0021746C  5e                      pop      esi                            
  0x0021746D  7503                    jne      0x217472                       
  0x0021746F  83c003                  add      eax, 3                         
                                        ; XREF: 0x0021746D (cond_jump)
  0x00217472  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00217473
; Start: 0x00217473  End: 0x002174AD  Size: 58 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00217473:
  0x00217473  53                      push     ebx                            
  0x00217474  56                      push     esi                            
  0x00217475  57                      push     edi                            
  0x00217476  8bf1                    mov      esi, ecx                       
  0x00217478  ff36                    push     dword ptr [esi]                
  0x0021747A  8bfa                    mov      edi, edx                       
  0x0021747C  ff15847b2100            call     dword ptr [0x217b84]           ; -> xbox_MmGetPhysicalAddress
  0x00217482  8b0e                    mov      ecx, dword ptr [esi]           
  0x00217484  81e1ff0f0000            and      ecx, 0xfff                     
  0x0021748A  ba00100000              mov      edx, 0x1000                    
  0x0021748F  2bd1                    sub      edx, ecx                       
  0x00217491  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x00217495  8911                    mov      dword ptr [ecx], edx           
  0x00217497  8b1f                    mov      ebx, dword ptr [edi]           
  0x00217499  3bd3                    cmp      edx, ebx                       
  0x0021749B  7602                    jbe      0x21749f                       
  0x0021749D  8919                    mov      dword ptr [ecx], ebx           
                                        ; XREF: 0x0021749B (cond_jump)
  0x0021749F  8b11                    mov      edx, dword ptr [ecx]           
  0x002174A1  2917                    sub      dword ptr [edi], edx           
  0x002174A3  8b09                    mov      ecx, dword ptr [ecx]           
  0x002174A5  010e                    add      dword ptr [esi], ecx           
  0x002174A7  5f                      pop      edi                            
  0x002174A8  5e                      pop      esi                            
  0x002174A9  5b                      pop      ebx                            
  0x002174AA  c20400                  ret      4                              
; end of function

; ============================================================
; Function: sub_002174AD
; Start: 0x002174AD  End: 0x00217513  Size: 102 bytes
; Detection: prologue (confidence: 0.95)
; Called by: sub_002177AB, sub_002177FE, sub_0021784B
; ============================================================
sub_002174AD:
  0x002174AD  55                      push     ebp                            
  0x002174AE  8bec                    mov      ebp, esp                       
  0x002174B0  83ec28                  sub      esp, 0x28                      
  0x002174B3  33c0                    xor      eax, eax                       
  0x002174B5  53                      push     ebx                            
  0x002174B6  8bda                    mov      ebx, edx                       
  0x002174B8  668b4302                mov      ax, word ptr [ebx + 2]         
  0x002174BC  56                      push     esi                            
  0x002174BD  57                      push     edi                            
  0x002174BE  8b7d08                  mov      edi, dword ptr [ebp + 8]       
  0x002174C1  894de0                  mov      dword ptr [ebp - 0x20], ecx    
  0x002174C4  8a895c040000            mov      cl, byte ptr [ecx + 0x45c]     
  0x002174CA  884de8                  mov      byte ptr [ebp - 0x18], cl      
  0x002174CD  25ff070000              and      eax, 0x7ff                     
  0x002174D2  8945d8                  mov      dword ptr [ebp - 0x28], eax    
  0x002174D5  8b4714                  mov      eax, dword ptr [edi + 0x14]    
  0x002174D8  8945f0                  mov      dword ptr [ebp - 0x10], eax    
  0x002174DB  33c0                    xor      eax, eax                       
  0x002174DD  20450b                  and      byte ptr [ebp + 0xb], al       
  0x002174E0  fe4b26                  dec      byte ptr [ebx + 0x26]          
  0x002174E3  fe4327                  inc      byte ptr [ebx + 0x27]          
  0x002174E6  668b4f22                mov      cx, word ptr [edi + 0x22]      
  0x002174EA  6681e1fdff              and      cx, 0xfffd                     
  0x002174EF  6683c904                or       cx, 4                          
  0x002174F3  66894f22                mov      word ptr [edi + 0x22], cx      
  0x002174F7  8b7304                  mov      esi, dword ptr [ebx + 4]       
  0x002174FA  3bf0                    cmp      esi, eax                       
  0x002174FC  8945ec                  mov      dword ptr [ebp - 0x14], eax    
  0x002174FF  8945e4                  mov      dword ptr [ebp - 0x1c], eax    
  0x00217502  8945f4                  mov      dword ptr [ebp - 0xc], eax     
  0x00217505  8945f8                  mov      dword ptr [ebp - 8], eax       
  0x00217508  7409                    je       0x217513                       
  0x0021750A  a1a0b9c600              mov      eax, dword ptr [0xc6b9a0]      
  0x0021750F  03f0                    add      esi, eax                       
  0x00217511  eb19                    jmp      0x21752c                       
; end of function
                                        ; XREF: 0x00217508 (cond_jump)
  0x00217513  ff75e8                  push     dword ptr [ebp - 0x18]         
  0x00217516  e8c7feffff              call     0x2173e2                       ; -> sub_002173E2
  0x0021751B  8bf0                    mov      esi, eax                       
  0x0021751D  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x00217520  334308                  xor      eax, dword ptr [ebx + 8]       
  0x00217523  83e00f                  and      eax, 0xf                       
  0x00217526  334610                  xor      eax, dword ptr [esi + 0x10]    
  0x00217529  894308                  mov      dword ptr [ebx + 8], eax       
                                        ; XREF: 0x00217511 (jump)
  0x0021752C  807b1100                cmp      byte ptr [ebx + 0x11], 0       
  0x00217530  8975fc                  mov      dword ptr [ebp - 4], esi       
  0x00217533  7566                    jne      0x21759b                       
  0x00217535  33c0                    xor      eax, eax                       
  0x00217537  b0fe                    mov      al, 0xfe                       
  0x00217539  2a45e8                  sub      al, byte ptr [ebp - 0x18]      
  0x0021753C  50                      push     eax                            
  0x0021753D  e8a0feffff              call     0x2173e2                       ; -> sub_002173E2
  0x00217542  8b4f28                  mov      ecx, dword ptr [edi + 0x28]    
  0x00217545  ff75e8                  push     dword ptr [ebp - 0x18]         
  0x00217548  8908                    mov      dword ptr [eax], ecx           
  0x0021754A  8b4f2c                  mov      ecx, dword ptr [edi + 0x2c]    
  0x0021754D  8945dc                  mov      dword ptr [ebp - 0x24], eax    
  0x00217550  894804                  mov      dword ptr [eax + 4], ecx       
  0x00217553  e88afeffff              call     0x2173e2                       ; -> sub_002173E2
  0x00217558  8bf0                    mov      esi, eax                       
  0x0021755A  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x0021755D  8b08                    mov      ecx, dword ptr [eax]           
  0x0021755F  81e1ffff0300            and      ecx, 0x3ffff                   
  0x00217565  81c90000e0e2            or       ecx, 0xe2e00000                
  0x0021756B  8908                    mov      dword ptr [eax], ecx           
  0x0021756D  8b4ddc                  mov      ecx, dword ptr [ebp - 0x24]    
  0x00217570  8b5110                  mov      edx, dword ptr [ecx + 0x10]    
  0x00217573  895004                  mov      dword ptr [eax + 4], edx       
  0x00217576  8b5610                  mov      edx, dword ptr [esi + 0x10]    
  0x00217579  895008                  mov      dword ptr [eax + 8], edx       
  0x0021757C  8b4910                  mov      ecx, dword ptr [ecx + 0x10]    
  0x0021757F  80601c00                and      byte ptr [eax + 0x1c], 0       
  0x00217583  83c107                  add      ecx, 7                         
  0x00217586  80601d00                and      byte ptr [eax + 0x1d], 0       
  0x0021758A  c6450b02                mov      byte ptr [ebp + 0xb], 2        
  0x0021758E  89480c                  mov      dword ptr [eax + 0xc], ecx     
  0x00217591  895814                  mov      dword ptr [eax + 0x14], ebx    
  0x00217594  c6401e01                mov      byte ptr [eax + 0x1e], 1       
  0x00217598  897818                  mov      dword ptr [eax + 0x18], edi    
                                        ; XREF: 0x00217533 (cond_jump)
  0x0021759B  837df000                cmp      dword ptr [ebp - 0x10], 0      
  0x0021759F  7416                    je       0x2175b7                       
  0x002175A1  6a00                    push     0                              
  0x002175A3  ff75f0                  push     dword ptr [ebp - 0x10]         
  0x002175A6  ff7718                  push     dword ptr [edi + 0x18]         
  0x002175A9  ff15887b2100            call     dword ptr [0x217b88]           ; -> xbox_MmLockUnlockBufferPages
  0x002175AF  8b4718                  mov      eax, dword ptr [edi + 0x18]    
  0x002175B2  8945dc                  mov      dword ptr [ebp - 0x24], eax    
  0x002175B5  eb04                    jmp      0x2175bb                       
                                        ; XREF: 0x0021759F (cond_jump)
  0x002175B7  c6471c01                mov      byte ptr [edi + 0x1c], 1       
                                        ; XREF: 0x002175B5 (jump)
  0x002175BB  837df000                cmp      dword ptr [ebp - 0x10], 0      
  0x002175BF  0f8411010000            je       0x2176d6                       
                                        ; XREF: 0x002176BD (cond_jump)
  0x002175C5  8d45ec                  lea      eax, [ebp - 0x14]              
  0x002175C8  50                      push     eax                            
  0x002175C9  8d55f0                  lea      edx, [ebp - 0x10]              
  0x002175CC  8d4ddc                  lea      ecx, [ebp - 0x24]              
  0x002175CF  e89ffeffff              call     0x217473                       ; -> sub_00217473
  0x002175D4  8945fc                  mov      dword ptr [ebp - 4], eax       
                                        ; XREF: 0x002176AB (jump)
  0x002175D7  8b55ec                  mov      edx, dword ptr [ebp - 0x14]    
  0x002175DA  8b45f4                  mov      eax, dword ptr [ebp - 0xc]     
  0x002175DD  8b4dd8                  mov      ecx, dword ptr [ebp - 0x28]    
  0x002175E0  03c2                    add      eax, edx                       
  0x002175E2  3bc1                    cmp      eax, ecx                       
  0x002175E4  7312                    jae      0x2175f8                       
  0x002175E6  85d2                    test     edx, edx                       
  0x002175E8  0f84c2000000            je       0x2176b0                       
  0x002175EE  837df000                cmp      dword ptr [ebp - 0x10], 0      
  0x002175F2  0f85b8000000            jne      0x2176b0                       
                                        ; XREF: 0x002175E4 (cond_jump)
  0x002175F8  837df400                cmp      dword ptr [ebp - 0xc], 0       
  0x002175FC  7422                    je       0x217620                       
  0x002175FE  2b4df4                  sub      ecx, dword ptr [ebp - 0xc]     
  0x00217601  8b45e4                  mov      eax, dword ptr [ebp - 0x1c]    
  0x00217604  3bd1                    cmp      edx, ecx                       
  0x00217606  894604                  mov      dword ptr [esi + 4], eax       
  0x00217609  7302                    jae      0x21760d                       
  0x0021760B  8bca                    mov      ecx, edx                       
                                        ; XREF: 0x00217609 (cond_jump)
  0x0021760D  8a45f4                  mov      al, byte ptr [ebp - 0xc]       
  0x00217610  014dfc                  add      dword ptr [ebp - 4], ecx       
  0x00217613  02c1                    add      al, cl                         
  0x00217615  2bd1                    sub      edx, ecx                       
  0x00217617  8365f400                and      dword ptr [ebp - 0xc], 0       
  0x0021761B  88461d                  mov      byte ptr [esi + 0x1d], al      
  0x0021761E  eb1e                    jmp      0x21763e                       
                                        ; XREF: 0x002175FC (cond_jump)
  0x00217620  3bd1                    cmp      edx, ecx                       
  0x00217622  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00217625  894604                  mov      dword ptr [esi + 4], eax       
  0x00217628  730c                    jae      0x217636                       
  0x0021762A  0155fc                  add      dword ptr [ebp - 4], edx       
  0x0021762D  8365ec00                and      dword ptr [ebp - 0x14], 0      
  0x00217631  88561d                  mov      byte ptr [esi + 0x1d], dl      
  0x00217634  eb0b                    jmp      0x217641                       
                                        ; XREF: 0x00217628 (cond_jump)
  0x00217636  014dfc                  add      dword ptr [ebp - 4], ecx       
  0x00217639  884e1d                  mov      byte ptr [esi + 0x1d], cl      
  0x0021763C  2bd1                    sub      edx, ecx                       
                                        ; XREF: 0x0021761E (jump)
  0x0021763E  8955ec                  mov      dword ptr [ebp - 0x14], edx    
                                        ; XREF: 0x00217634 (jump)
  0x00217641  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x00217644  8b0e                    mov      ecx, dword ptr [esi]           
  0x00217646  80750b01                xor      byte ptr [ebp + 0xb], 1        
  0x0021764A  80661c00                and      byte ptr [esi + 0x1c], 0       
  0x0021764E  80661e00                and      byte ptr [esi + 0x1e], 0       
  0x00217652  48                      dec      eax                            
  0x00217653  89460c                  mov      dword ptr [esi + 0xc], eax     
  0x00217656  0fb6450b                movzx    eax, byte ptr [ebp + 0xb]      
  0x0021765A  ff75e8                  push     dword ptr [ebp - 0x18]         
  0x0021765D  81e1fffffb0f            and      ecx, 0xffbffff                 
  0x00217663  81c9000000e0            or       ecx, 0xe0000000                
  0x00217669  c1e018                  shl      eax, 0x18                      
  0x0021766C  33c1                    xor      eax, ecx                       
  0x0021766E  2500000003              and      eax, 0x3000000                 
  0x00217673  33c1                    xor      eax, ecx                       
  0x00217675  890e                    mov      dword ptr [esi], ecx           
  0x00217677  0d0000e000              or       eax, 0xe00000                  
  0x0021767C  33c9                    xor      ecx, ecx                       
  0x0021767E  8906                    mov      dword ptr [esi], eax           
  0x00217680  895e14                  mov      dword ptr [esi + 0x14], ebx    
  0x00217683  8a4f1c                  mov      cl, byte ptr [edi + 0x1c]      
  0x00217686  25ffffe7f3              and      eax, 0xf3e7ffff                
  0x0021768B  897e18                  mov      dword ptr [esi + 0x18], edi    
  0x0021768E  8975f8                  mov      dword ptr [ebp - 8], esi       
  0x00217691  83e103                  and      ecx, 3                         
  0x00217694  c1e113                  shl      ecx, 0x13                      
  0x00217697  0bc8                    or       ecx, eax                       
  0x00217699  890e                    mov      dword ptr [esi], ecx           
  0x0021769B  e842fdffff              call     0x2173e2                       ; -> sub_002173E2
  0x002176A0  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x002176A3  8bf0                    mov      esi, eax                       
  0x002176A5  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x002176A8  894108                  mov      dword ptr [ecx + 8], eax       
  0x002176AB  e927ffffff              jmp      0x2175d7                       
                                        ; XREF: 0x002175E8 (cond_jump), 0x002175F2 (cond_jump)
  0x002176B0  837df000                cmp      dword ptr [ebp - 0x10], 0      
  0x002176B4  8b45fc                  mov      eax, dword ptr [ebp - 4]       
  0x002176B7  8945e4                  mov      dword ptr [ebp - 0x1c], eax    
  0x002176BA  8955f4                  mov      dword ptr [ebp - 0xc], edx     
  0x002176BD  0f8502ffffff            jne      0x2175c5                       
  0x002176C3  837df800                cmp      dword ptr [ebp - 8], 0         
  0x002176C7  740d                    je       0x2176d6                       
  0x002176C9  807f1d00                cmp      byte ptr [edi + 0x1d], 0       
  0x002176CD  7407                    je       0x2176d6                       
  0x002176CF  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x002176D2  80480204                or       byte ptr [eax + 2], 4          
                                        ; XREF: 0x002175BF (cond_jump), 0x002176C7 (cond_jump), 0x002176CD (cond_jump)
  0x002176D6  807b1100                cmp      byte ptr [ebx + 0x11], 0       
  0x002176DA  756a                    jne      0x217746                       
  0x002176DC  806602fb                and      byte ptr [esi + 2], 0xfb       
  0x002176E0  8b0e                    mov      ecx, dword ptr [esi]           
  0x002176E2  33c0                    xor      eax, eax                       
  0x002176E4  807f1c02                cmp      byte ptr [edi + 0x1c], 2       
  0x002176E8  ff75e8                  push     dword ptr [ebp - 0x18]         
  0x002176EB  0f95c0                  setne    al                             
  0x002176EE  8975f8                  mov      dword ptr [ebp - 8], esi       
  0x002176F1  40                      inc      eax                            
  0x002176F2  c1e013                  shl      eax, 0x13                      
  0x002176F5  33c1                    xor      eax, ecx                       
  0x002176F7  2500001800              and      eax, 0x180000                  
  0x002176FC  33c1                    xor      eax, ecx                       
  0x002176FE  33c9                    xor      ecx, ecx                       
  0x00217700  8906                    mov      dword ptr [esi], eax           
  0x00217702  8a4f1e                  mov      cl, byte ptr [edi + 0x1e]      
  0x00217705  83660400                and      dword ptr [esi + 4], 0         
  0x00217709  83660c00                and      dword ptr [esi + 0xc], 0       
  0x0021770D  80661d00                and      byte ptr [esi + 0x1d], 0       
  0x00217711  25ffff1f00              and      eax, 0x1fffff                  
  0x00217716  895e14                  mov      dword ptr [esi + 0x14], ebx    
  0x00217719  c6461c02                mov      byte ptr [esi + 0x1c], 2       
  0x0021771D  83e107                  and      ecx, 7                         
  0x00217720  81c918ffffff            or       ecx, 0xffffff18                
  0x00217726  c1e115                  shl      ecx, 0x15                      
  0x00217729  0bc8                    or       ecx, eax                       
  0x0021772B  890e                    mov      dword ptr [esi], ecx           
  0x0021772D  c6461e02                mov      byte ptr [esi + 0x1e], 2       
  0x00217731  897e18                  mov      dword ptr [esi + 0x18], edi    
  0x00217734  e8a9fcffff              call     0x2173e2                       ; -> sub_002173E2
  0x00217739  8b4df8                  mov      ecx, dword ptr [ebp - 8]       
  0x0021773C  8bf0                    mov      esi, eax                       
  0x0021773E  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x00217741  894108                  mov      dword ptr [ecx + 8], eax       
  0x00217744  eb18                    jmp      0x21775e                       
                                        ; XREF: 0x002176DA (cond_jump)
  0x00217746  0fb64f1e                movzx    ecx, byte ptr [edi + 0x1e]     
  0x0021774A  8b45f8                  mov      eax, dword ptr [ebp - 8]       
  0x0021774D  c1e115                  shl      ecx, 0x15                      
  0x00217750  3308                    xor      ecx, dword ptr [eax]           
  0x00217752  c6401c02                mov      byte ptr [eax + 0x1c], 2       
  0x00217756  81e10000e000            and      ecx, 0xe00000                  
  0x0021775C  3108                    xor      dword ptr [eax], ecx           
                                        ; XREF: 0x00217744 (jump)
  0x0021775E  c6461e03                mov      byte ptr [esi + 0x1e], 3       
  0x00217762  668b4714                mov      ax, word ptr [edi + 0x14]      
  0x00217766  83671400                and      dword ptr [edi + 0x14], 0      
  0x0021776A  66894720                mov      word ptr [edi + 0x20], ax      
  0x0021776E  807b2000                cmp      byte ptr [ebx + 0x20], 0       
  0x00217772  8b4610                  mov      eax, dword ptr [esi + 0x10]    
  0x00217775  894304                  mov      dword ptr [ebx + 4], eax       
  0x00217778  7504                    jne      0x21777e                       
  0x0021777A  806301bf                and      byte ptr [ebx + 1], 0xbf       
                                        ; XREF: 0x00217778 (cond_jump)
  0x0021777E  8a5b11                  mov      bl, byte ptr [ebx + 0x11]      
  0x00217781  84db                    test     bl, bl                         
  0x00217783  750e                    jne      0x217793                       
  0x00217785  8b45e0                  mov      eax, dword ptr [ebp - 0x20]    
  0x00217788  8b00                    mov      eax, dword ptr [eax]           
  0x0021778A  c7400802000000          mov      dword ptr [eax + 8], 2         
  0x00217791  eb11                    jmp      0x2177a4                       
                                        ; XREF: 0x00217783 (cond_jump)
  0x00217793  80fb02                  cmp      bl, 2                          
  0x00217796  750c                    jne      0x2177a4                       
  0x00217798  8b45e0                  mov      eax, dword ptr [ebp - 0x20]    
  0x0021779B  8b00                    mov      eax, dword ptr [eax]           
  0x0021779D  c7400804000000          mov      dword ptr [eax + 8], 4         
                                        ; XREF: 0x00217791 (jump), 0x00217796 (cond_jump)
  0x002177A4  5f                      pop      edi                            
  0x002177A5  5e                      pop      esi                            
  0x002177A6  5b                      pop      ebx                            
  0x002177A7  c9                      leave                                   
  0x002177A8  c20400                  ret      4                              

; ============================================================
; Function: sub_002177AB
; Start: 0x002177AB  End: 0x002177FE  Size: 83 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_002174AD
; ============================================================
sub_002177AB:
  0x002177AB  55                      push     ebp                            
  0x002177AC  8bec                    mov      ebp, esp                       
  0x002177AE  51                      push     ecx                            
  0x002177AF  56                      push     esi                            
  0x002177B0  8bf2                    mov      esi, edx                       
  0x002177B2  837e2800                cmp      dword ptr [esi + 0x28], 0      
  0x002177B6  894dfc                  mov      dword ptr [ebp - 4], ecx       
  0x002177B9  7440                    je       0x2177fb                       
  0x002177BB  53                      push     ebx                            
                                        ; XREF: 0x002177F8 (cond_jump)
  0x002177BC  8b4628                  mov      eax, dword ptr [esi + 0x28]    
  0x002177BF  668b5624                mov      dx, word ptr [esi + 0x24]      
  0x002177C3  0fb75820                movzx    ebx, word ptr [eax + 0x20]     
  0x002177C7  0fb7ca                  movzx    ecx, dx                        
  0x002177CA  03cb                    add      ecx, ebx                       
  0x002177CC  83f903                  cmp      ecx, 3                         
  0x002177CF  7f29                    jg       0x2177fa                       
  0x002177D1  8b4824                  mov      ecx, dword ptr [eax + 0x24]    
  0x002177D4  85c9                    test     ecx, ecx                       
  0x002177D6  894e28                  mov      dword ptr [esi + 0x28], ecx    
  0x002177D9  7503                    jne      0x2177de                       
  0x002177DB  214e2c                  and      dword ptr [esi + 0x2c], ecx    
                                        ; XREF: 0x002177D9 (cond_jump)
  0x002177DE  668b4820                mov      cx, word ptr [eax + 0x20]      
  0x002177E2  6603ca                  add      cx, dx                         
  0x002177E5  66894e24                mov      word ptr [esi + 0x24], cx      
  0x002177E9  8b4dfc                  mov      ecx, dword ptr [ebp - 4]       
  0x002177EC  50                      push     eax                            
  0x002177ED  8bd6                    mov      edx, esi                       
  0x002177EF  e8b9fcffff              call     0x2174ad                       ; -> sub_002174AD
  0x002177F4  837e2800                cmp      dword ptr [esi + 0x28], 0      
  0x002177F8  75c2                    jne      0x2177bc                       
                                        ; XREF: 0x002177CF (cond_jump)
  0x002177FA  5b                      pop      ebx                            
                                        ; XREF: 0x002177B9 (cond_jump)
  0x002177FB  5e                      pop      esi                            
  0x002177FC  c9                      leave                                   
  0x002177FD  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_002177FE
; Start: 0x002177FE  End: 0x0021784B  Size: 77 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00217421, sub_002174AD
; ============================================================
sub_002177FE:
  0x002177FE  56                      push     esi                            
  0x002177FF  8bf1                    mov      esi, ecx                       
  0x00217801  83be2404000000          cmp      dword ptr [esi + 0x424], 0     
  0x00217808  743f                    je       0x217849                       
  0x0021780A  57                      push     edi                            
                                        ; XREF: 0x00217846 (cond_jump)
  0x0021780B  8bbe24040000            mov      edi, dword ptr [esi + 0x424]   
  0x00217811  33c0                    xor      eax, eax                       
  0x00217813  668b4720                mov      ax, word ptr [edi + 0x20]      
  0x00217817  50                      push     eax                            
  0x00217818  e804fcffff              call     0x217421                       ; -> sub_00217421
  0x0021781D  84c0                    test     al, al                         
  0x0021781F  7427                    je       0x217848                       
  0x00217821  8b4724                  mov      eax, dword ptr [edi + 0x24]    
  0x00217824  85c0                    test     eax, eax                       
  0x00217826  898624040000            mov      dword ptr [esi + 0x424], eax   
  0x0021782C  7506                    jne      0x217834                       
  0x0021782E  218628040000            and      dword ptr [esi + 0x428], eax   
                                        ; XREF: 0x0021782C (cond_jump)
  0x00217834  8b5710                  mov      edx, dword ptr [edi + 0x10]    
  0x00217837  57                      push     edi                            
  0x00217838  8bce                    mov      ecx, esi                       
  0x0021783A  e86efcffff              call     0x2174ad                       ; -> sub_002174AD
  0x0021783F  83be2404000000          cmp      dword ptr [esi + 0x424], 0     
  0x00217846  75c3                    jne      0x21780b                       
                                        ; XREF: 0x0021781F (cond_jump)
  0x00217848  5f                      pop      edi                            
                                        ; XREF: 0x00217808 (cond_jump)
  0x00217849  5e                      pop      esi                            
  0x0021784A  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_0021784B
; Start: 0x0021784B  End: 0x00217898  Size: 77 bytes
; Detection: call_target (confidence: 0.90)
; Calls: sub_00217402, sub_002174AD
; ============================================================
sub_0021784B:
  0x0021784B  56                      push     esi                            
  0x0021784C  8bf1                    mov      esi, ecx                       
  0x0021784E  83be1c04000000          cmp      dword ptr [esi + 0x41c], 0     
  0x00217855  743f                    je       0x217896                       
  0x00217857  57                      push     edi                            
                                        ; XREF: 0x00217893 (cond_jump)
  0x00217858  8bbe1c040000            mov      edi, dword ptr [esi + 0x41c]   
  0x0021785E  33c0                    xor      eax, eax                       
  0x00217860  668b4720                mov      ax, word ptr [edi + 0x20]      
  0x00217864  50                      push     eax                            
  0x00217865  e898fbffff              call     0x217402                       ; -> sub_00217402
  0x0021786A  84c0                    test     al, al                         
  0x0021786C  7427                    je       0x217895                       
  0x0021786E  8b4724                  mov      eax, dword ptr [edi + 0x24]    
  0x00217871  85c0                    test     eax, eax                       
  0x00217873  89861c040000            mov      dword ptr [esi + 0x41c], eax   
  0x00217879  7506                    jne      0x217881                       
  0x0021787B  218620040000            and      dword ptr [esi + 0x420], eax   
                                        ; XREF: 0x00217879 (cond_jump)
  0x00217881  8b5710                  mov      edx, dword ptr [edi + 0x10]    
  0x00217884  57                      push     edi                            
  0x00217885  8bce                    mov      ecx, esi                       
  0x00217887  e821fcffff              call     0x2174ad                       ; -> sub_002174AD
  0x0021788C  83be1c04000000          cmp      dword ptr [esi + 0x41c], 0     
  0x00217893  75c3                    jne      0x217858                       
                                        ; XREF: 0x0021786C (cond_jump)
  0x00217895  5f                      pop      edi                            
                                        ; XREF: 0x00217855 (cond_jump)
  0x00217896  5e                      pop      esi                            
  0x00217897  c3                      ret                                     
; end of function

; ============================================================
; Function: sub_00217898
; Start: 0x00217898  End: 0x002178AA  Size: 18 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00217943
; ============================================================
sub_00217898:
  0x00217898  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0021789C  6683782003              cmp      word ptr [eax + 0x20], 3       
  0x002178A1  7607                    jbe      0x2178aa                       
  0x002178A3  b800050080              mov      eax, 0x80000500                
  0x002178A8  eb1e                    jmp      0x2178c8                       
; end of function
                                        ; XREF: 0x002178A1 (cond_jump)
  0x002178AA  56                      push     esi                            
  0x002178AB  8b722c                  mov      esi, dword ptr [edx + 0x2c]    
  0x002178AE  85f6                    test     esi, esi                       
  0x002178B0  7405                    je       0x2178b7                       
  0x002178B2  894624                  mov      dword ptr [esi + 0x24], eax    
  0x002178B5  eb03                    jmp      0x2178ba                       
                                        ; XREF: 0x002178B0 (cond_jump)
  0x002178B7  894228                  mov      dword ptr [edx + 0x28], eax    
                                        ; XREF: 0x002178B5 (jump)
  0x002178BA  89422c                  mov      dword ptr [edx + 0x2c], eax    
  0x002178BD  e8e9feffff              call     0x2177ab                       ; -> sub_002177AB
  0x002178C2  b800000040              mov      eax, 0x40000000                
  0x002178C7  5e                      pop      esi                            
                                        ; XREF: 0x002178A8 (jump)
  0x002178C8  c20400                  ret      4                              

; ============================================================
; Function: sub_002178CB
; Start: 0x002178CB  End: 0x002178DE  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; Called by: sub_00217943
; ============================================================
sub_002178CB:
  0x002178CB  668b4220                mov      ax, word ptr [edx + 0x20]      
  0x002178CF  663b05c4b9c600          cmp      ax, word ptr [0xc6b9c4]        
  0x002178D6  7606                    jbe      0x2178de                       
  0x002178D8  b800050080              mov      eax, 0x80000500                
  0x002178DD  c3                      ret                                     
; end of function
                                        ; XREF: 0x002178D6 (cond_jump)
  0x002178DE  8d8124040000            lea      eax, [ecx + 0x424]             
  0x002178E4  833800                  cmp      dword ptr [eax], 0             
  0x002178E7  740b                    je       0x2178f4                       
  0x002178E9  8b8128040000            mov      eax, dword ptr [ecx + 0x428]   
  0x002178EF  895024                  mov      dword ptr [eax + 0x24], edx    
  0x002178F2  eb02                    jmp      0x2178f6                       
                                        ; XREF: 0x002178E7 (cond_jump)
  0x002178F4  8910                    mov      dword ptr [eax], edx           
                                        ; XREF: 0x002178F2 (jump)
  0x002178F6  899128040000            mov      dword ptr [ecx + 0x428], edx   
  0x002178FC  e8fdfeffff              call     0x2177fe                       ; -> sub_002177FE
  0x00217901  b800000040              mov      eax, 0x40000000                
  0x00217906  c3                      ret                                     

; ============================================================
; Function: sub_00217907
; Start: 0x00217907  End: 0x0021791A  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_00217907:
  0x00217907  668b4220                mov      ax, word ptr [edx + 0x20]      
  0x0021790B  663b05c0b9c600          cmp      ax, word ptr [0xc6b9c0]        
  0x00217912  7606                    jbe      0x21791a                       
  0x00217914  b800050080              mov      eax, 0x80000500                
  0x00217919  c3                      ret                                     
; end of function
                                        ; XREF: 0x00217912 (cond_jump)
  0x0021791A  8d811c040000            lea      eax, [ecx + 0x41c]             
  0x00217920  833800                  cmp      dword ptr [eax], 0             
  0x00217923  740b                    je       0x217930                       
  0x00217925  8b8120040000            mov      eax, dword ptr [ecx + 0x420]   
  0x0021792B  895024                  mov      dword ptr [eax + 0x24], edx    
  0x0021792E  eb02                    jmp      0x217932                       
                                        ; XREF: 0x00217923 (cond_jump)
  0x00217930  8910                    mov      dword ptr [eax], edx           
                                        ; XREF: 0x0021792E (jump)
  0x00217932  899120040000            mov      dword ptr [ecx + 0x420], edx   
  0x00217938  e80effffff              call     0x21784b                       ; -> sub_0021784B
  0x0021793D  b800000040              mov      eax, 0x40000000                
  0x00217942  c3                      ret                                     

; ============================================================
; Function: sub_00217943
; Start: 0x00217943  End: 0x002179A2  Size: 95 bytes
; Detection: prologue (confidence: 0.95)
; Calls: sub_00217440, sub_00217898, sub_002178CB
; Called by: sub_00215371
; ============================================================
sub_00217943:
  0x00217943  55                      push     ebp                            
  0x00217944  8bec                    mov      ebp, esp                       
  0x00217946  51                      push     ecx                            
  0x00217947  53                      push     ebx                            
  0x00217948  56                      push     esi                            
  0x00217949  8bf2                    mov      esi, edx                       
  0x0021794B  57                      push     edi                            
  0x0021794C  8b7e10                  mov      edi, dword ptr [esi + 0x10]    
  0x0021794F  8bd9                    mov      ebx, ecx                       
  0x00217951  8bd7                    mov      edx, edi                       
  0x00217953  8bce                    mov      ecx, esi                       
  0x00217955  e8e6faffff              call     0x217440                       ; -> sub_00217440
  0x0021795A  66894620                mov      word ptr [esi + 0x20], ax      
  0x0021795E  ff15007b2100            call     dword ptr [0x217b00]           ; -> xbox_KeRaiseIrqlToDpcLevel
  0x00217964  fe4726                  inc      byte ptr [edi + 0x26]          
  0x00217967  83662400                and      dword ptr [esi + 0x24], 0      
  0x0021796B  8845ff                  mov      byte ptr [ebp - 1], al         
  0x0021796E  66c746220200            mov      word ptr [esi + 0x22], 2       
  0x00217974  0fb64711                movzx    eax, byte ptr [edi + 0x11]     
  0x00217978  83e800                  sub      eax, 0                         
  0x0021797B  7425                    je       0x2179a2                       
  0x0021797D  48                      dec      eax                            
  0x0021797E  48                      dec      eax                            
  0x0021797F  7416                    je       0x217997                       
  0x00217981  48                      dec      eax                            
  0x00217982  7407                    je       0x21798b                       
  0x00217984  bb00060080              mov      ebx, 0x80000600                
  0x00217989  eb26                    jmp      0x2179b1                       
                                        ; XREF: 0x00217982 (cond_jump)
  0x0021798B  56                      push     esi                            
  0x0021798C  8bd7                    mov      edx, edi                       
  0x0021798E  8bcb                    mov      ecx, ebx                       
  0x00217990  e803ffffff              call     0x217898                       ; -> sub_00217898
  0x00217995  eb14                    jmp      0x2179ab                       
                                        ; XREF: 0x0021797F (cond_jump)
  0x00217997  8bd6                    mov      edx, esi                       
  0x00217999  8bcb                    mov      ecx, ebx                       
  0x0021799B  e82bffffff              call     0x2178cb                       ; -> sub_002178CB
  0x002179A0  eb09                    jmp      0x2179ab                       
; end of function
                                        ; XREF: 0x0021797B (cond_jump)
  0x002179A2  8bd6                    mov      edx, esi                       
  0x002179A4  8bcb                    mov      ecx, ebx                       
  0x002179A6  e85cffffff              call     0x217907                       ; -> sub_00217907
                                        ; XREF: 0x00217995 (jump), 0x002179A0 (jump)
  0x002179AB  8bd8                    mov      ebx, eax                       
  0x002179AD  85db                    test     ebx, ebx                       
  0x002179AF  7d08                    jge      0x2179b9                       
                                        ; XREF: 0x00217989 (jump)
  0x002179B1  6683662200              and      word ptr [esi + 0x22], 0       
  0x002179B6  fe4f26                  dec      byte ptr [edi + 0x26]          
                                        ; XREF: 0x002179AF (cond_jump)
  0x002179B9  8a4dff                  mov      cl, byte ptr [ebp - 1]         
  0x002179BC  ff15fc7a2100            call     dword ptr [0x217afc]           ; -> xbox_KfLowerIrql
  0x002179C2  5f                      pop      edi                            
  0x002179C3  5e                      pop      esi                            
  0x002179C4  8bc3                    mov      eax, ebx                       
  0x002179C6  5b                      pop      ebx                            
  0x002179C7  c9                      leave                                   
  0x002179C8  c3                      ret                                     
  0x002179C9  cc                      int3                                    
  0x002179CA  cc                      int3                                    
  0x002179CB  cc                      int3                                    

; ============================================================
; Function: sub_002179CC
; Start: 0x002179CC  End: 0x002179FB  Size: 47 bytes
; Detection: cc_boundary (confidence: 0.85)
; ============================================================
sub_002179CC:
  0x002179CC  5c                      pop      esp                            
  0x002179CD  44                      inc      esp                            
  0x002179CE  657669                  jbe      0x217a3a                       
  0x002179D1  63655c                  arpl     word ptr [ebp + 0x5c], sp      
  0x002179D4  4d                      dec      ebp                            
  0x002179D5  55                      push     ebp                            
  0x002179D6  5f                      pop      edi                            
  0x002179D7  3000                    xor      byte ptr [eax], al             
  0x002179D9  0000                    add      byte ptr [eax], al             
  0x002179DB  0039                    add      byte ptr [ecx], bh             
  0x002179DD  3531463045              xor      eax, 0x45304631                
  0x002179E2  46                      inc      esi                            
  0x002179E3  363330                  xor      esi, dword ptr ss:[eax]        
  0x002179E6  44                      inc      esp                            
  0x002179E7  43                      inc      ebx                            
  0x002179E8  3436                    xor      al, 0x36                       
  0x002179EA  64395f43                cmp      dword ptr fs:[edi + 0x43], ebx 
  0x002179EE  4f                      dec      edi                            
  0x002179EF  52                      push     edx                            
  0x002179F0  52                      push     edx                            
  0x002179F1  55                      push     ebp                            
  0x002179F2  50                      push     eax                            
  0x002179F3  54                      push     esp                            
  0x002179F4  5f                      pop      edi                            
  0x002179F5  53                      push     ebx                            
  0x002179F6  45                      inc      ebp                            
  0x002179F7  43                      inc      ebx                            
  0x002179F8  54                      push     esp                            
  0x002179F9  4f                      dec      edi                            
  0x002179FA  52                      push     edx                            
; end of function
