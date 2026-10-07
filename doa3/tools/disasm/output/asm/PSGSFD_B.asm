; ============================================================
; Section: PSGSFD_B
; VA: 0x0020FC60 - 0x00210320
; Size: 1728 bytes (1.7 KB)
; Functions: 3
; Instructions: 594
; ============================================================


; ============================================================
; Function: sub_0020FC60
; Start: 0x0020FC60  End: 0x0020FC73  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020FC60:
  0x0020FC60  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0020FC64  33c9                    xor      ecx, ecx                       
  0x0020FC66  894810                  mov      dword ptr [eax + 0x10], ecx    
  0x0020FC69  894814                  mov      dword ptr [eax + 0x14], ecx    
  0x0020FC6C  894818                  mov      dword ptr [eax + 0x18], ecx    
  0x0020FC6F  89481c                  mov      dword ptr [eax + 0x1c], ecx    
  0x0020FC72  c3                      ret                                     
; end of function
  0x0020FC73  90                      nop                                     
  0x0020FC74  90                      nop                                     
  0x0020FC75  90                      nop                                     
  0x0020FC76  90                      nop                                     
  0x0020FC77  90                      nop                                     
  0x0020FC78  90                      nop                                     
  0x0020FC79  90                      nop                                     
  0x0020FC7A  90                      nop                                     
  0x0020FC7B  90                      nop                                     
  0x0020FC7C  90                      nop                                     
  0x0020FC7D  90                      nop                                     
  0x0020FC7E  90                      nop                                     
  0x0020FC7F  90                      nop                                     

; ============================================================
; Function: sub_0020FC80
; Start: 0x0020FC80  End: 0x0020FC9C  Size: 28 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020FC80:
  0x0020FC80  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0020FC84  b900040000              mov      ecx, 0x400                     
  0x0020FC89  8988ac020000            mov      dword ptr [eax + 0x2ac], ecx   
  0x0020FC8F  8988b4020000            mov      dword ptr [eax + 0x2b4], ecx   
  0x0020FC95  8988b0020000            mov      dword ptr [eax + 0x2b0], ecx   
  0x0020FC9B  c3                      ret                                     
; end of function
  0x0020FC9C  90                      nop                                     
  0x0020FC9D  90                      nop                                     
  0x0020FC9E  90                      nop                                     
  0x0020FC9F  90                      nop                                     

; ============================================================
; Function: sub_0020FCA0
; Start: 0x0020FCA0  End: 0x0020FCE9  Size: 73 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020FCA0:
  0x0020FCA0  83ec08                  sub      esp, 8                         
  0x0020FCA3  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x0020FCA7  8b500c                  mov      edx, dword ptr [eax + 0xc]     
  0x0020FCAA  53                      push     ebx                            
  0x0020FCAB  55                      push     ebp                            
  0x0020FCAC  8b6804                  mov      ebp, dword ptr [eax + 4]       
  0x0020FCAF  56                      push     esi                            
  0x0020FCB0  8b7008                  mov      esi, dword ptr [eax + 8]       
  0x0020FCB3  57                      push     edi                            
  0x0020FCB4  8b38                    mov      edi, dword ptr [eax]           
  0x0020FCB6  8bdf                    mov      ebx, edi                       
  0x0020FCB8  c1eb15                  shr      ebx, 0x15                      
  0x0020FCBB  83fe15                  cmp      esi, 0x15                      
  0x0020FCBE  c744241400000000        mov      dword ptr [esp + 0x14], 0      
  0x0020FCC6  7e11                    jle      0x20fcd9                       
  0x0020FCC8  b935000000              mov      ecx, 0x35                      
  0x0020FCCD  2bce                    sub      ecx, esi                       
  0x0020FCCF  8bc5                    mov      eax, ebp                       
  0x0020FCD1  d3e8                    shr      eax, cl                        
  0x0020FCD3  0bd8                    or       ebx, eax                       
  0x0020FCD5  8b44241c                mov      eax, dword ptr [esp + 0x1c]    
                                        ; XREF: 0x0020FCC6 (cond_jump)
  0x0020FCD9  f7c380ffffff            test     ebx, 0xffffff80                
  0x0020FCDF  7508                    jne      0x20fce9                       
  0x0020FCE1  8b0de84ac800            mov      ecx, dword ptr [0xc84ae8]      
  0x0020FCE7  eb09                    jmp      0x20fcf2                       
; end of function
                                        ; XREF: 0x0020FCDF (cond_jump)
  0x0020FCE9  8b0dcc3fc800            mov      ecx, dword ptr [0xc83fcc]      
  0x0020FCEF  c1eb06                  shr      ebx, 6                         
                                        ; XREF: 0x0020FCE7 (jump)
  0x0020FCF2  0fbf0c59                movsx    ecx, word ptr [ecx + ebx*2]    
  0x0020FCF6  0fbed9                  movsx    ebx, cl                        
  0x0020FCF9  83fb7f                  cmp      ebx, 0x7f                      
  0x0020FCFC  895c241c                mov      dword ptr [esp + 0x1c], ebx    
  0x0020FD00  750d                    jne      0x20fd0f                       
  0x0020FD02  c7442414ffffffff        mov      dword ptr [esp + 0x14], 0xffffffff 
  0x0020FD0A  e92d010000              jmp      0x20fe3c                       
                                        ; XREF: 0x0020FD00 (cond_jump)
  0x0020FD0F  c1e908                  shr      ecx, 8                         
  0x0020FD12  0fb6c9                  movzx    ecx, cl                        
  0x0020FD15  03f1                    add      esi, ecx                       
  0x0020FD17  83fe20                  cmp      esi, 0x20                      
  0x0020FD1A  7c2f                    jl       0x20fd4b                       
  0x0020FD1C  83ee20                  sub      esi, 0x20                      
  0x0020FD1F  8bfd                    mov      edi, ebp                       
  0x0020FD21  0fb66a01                movzx    ebp, byte ptr [edx + 1]        
  0x0020FD25  8bce                    mov      ecx, esi                       
  0x0020FD27  d3e7                    shl      edi, cl                        
  0x0020FD29  0fbe0a                  movsx    ecx, byte ptr [edx]            
  0x0020FD2C  42                      inc      edx                            
  0x0020FD2D  c1e108                  shl      ecx, 8                         
  0x0020FD30  0bcd                    or       ecx, ebp                       
  0x0020FD32  0fb66a01                movzx    ebp, byte ptr [edx + 1]        
  0x0020FD36  42                      inc      edx                            
  0x0020FD37  c1e108                  shl      ecx, 8                         
  0x0020FD3A  0bcd                    or       ecx, ebp                       
  0x0020FD3C  0fb66a01                movzx    ebp, byte ptr [edx + 1]        
  0x0020FD40  42                      inc      edx                            
  0x0020FD41  c1e108                  shl      ecx, 8                         
  0x0020FD44  0bcd                    or       ecx, ebp                       
  0x0020FD46  8be9                    mov      ebp, ecx                       
  0x0020FD48  42                      inc      edx                            
  0x0020FD49  eb02                    jmp      0x20fd4d                       
                                        ; XREF: 0x0020FD1A (cond_jump)
  0x0020FD4B  d3e7                    shl      edi, cl                        
                                        ; XREF: 0x0020FD49 (jump)
  0x0020FD4D  85db                    test     ebx, ebx                       
  0x0020FD4F  7511                    jne      0x20fd62                       
  0x0020FD51  8b4c2428                mov      ecx, dword ptr [esp + 0x28]    
  0x0020FD55  8b19                    mov      ebx, dword ptr [ecx]           
  0x0020FD57  8b4c2424                mov      ecx, dword ptr [esp + 0x24]    
  0x0020FD5B  8919                    mov      dword ptr [ecx], ebx           
  0x0020FD5D  e9cf000000              jmp      0x20fe31                       
                                        ; XREF: 0x0020FD4F (cond_jump)
  0x0020FD62  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020FD66  8b5904                  mov      ebx, dword ptr [ecx + 4]       
  0x0020FD69  85db                    test     ebx, ebx                       
  0x0020FD6B  0f8494000000            je       0x20fe05                       
  0x0020FD71  b920000000              mov      ecx, 0x20                      
  0x0020FD76  2bcb                    sub      ecx, ebx                       
  0x0020FD78  3bf1                    cmp      esi, ecx                       
  0x0020FD7A  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020FD7E  7c49                    jl       0x20fdc9                       
  0x0020FD80  8d741ee0                lea      esi, [esi + ebx - 0x20]        
  0x0020FD84  85f6                    test     esi, esi                       
  0x0020FD86  7416                    je       0x20fd9e                       
  0x0020FD88  8bcb                    mov      ecx, ebx                       
  0x0020FD8A  2bce                    sub      ecx, esi                       
  0x0020FD8C  8bdd                    mov      ebx, ebp                       
  0x0020FD8E  d3eb                    shr      ebx, cl                        
  0x0020FD90  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x0020FD94  0bdf                    or       ebx, edi                       
  0x0020FD96  d3eb                    shr      ebx, cl                        
  0x0020FD98  8bce                    mov      ecx, esi                       
  0x0020FD9A  d3e5                    shl      ebp, cl                        
  0x0020FD9C  eb04                    jmp      0x20fda2                       
                                        ; XREF: 0x0020FD86 (cond_jump)
  0x0020FD9E  8bdf                    mov      ebx, edi                       
  0x0020FDA0  d3eb                    shr      ebx, cl                        
                                        ; XREF: 0x0020FD9C (jump)
  0x0020FDA2  0fbe0a                  movsx    ecx, byte ptr [edx]            
  0x0020FDA5  42                      inc      edx                            
  0x0020FDA6  c1e108                  shl      ecx, 8                         
  0x0020FDA9  8bfd                    mov      edi, ebp                       
  0x0020FDAB  0fb62a                  movzx    ebp, byte ptr [edx]            
  0x0020FDAE  0bcd                    or       ecx, ebp                       
  0x0020FDB0  0fb66a01                movzx    ebp, byte ptr [edx + 1]        
  0x0020FDB4  42                      inc      edx                            
  0x0020FDB5  c1e108                  shl      ecx, 8                         
  0x0020FDB8  0bcd                    or       ecx, ebp                       
  0x0020FDBA  0fb66a01                movzx    ebp, byte ptr [edx + 1]        
  0x0020FDBE  42                      inc      edx                            
  0x0020FDBF  c1e108                  shl      ecx, 8                         
  0x0020FDC2  0bcd                    or       ecx, ebp                       
  0x0020FDC4  8be9                    mov      ebp, ecx                       
  0x0020FDC6  42                      inc      edx                            
  0x0020FDC7  eb0f                    jmp      0x20fdd8                       
                                        ; XREF: 0x0020FD7E (cond_jump)
  0x0020FDC9  03f3                    add      esi, ebx                       
  0x0020FDCB  8bdf                    mov      ebx, edi                       
  0x0020FDCD  d3eb                    shr      ebx, cl                        
  0x0020FDCF  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020FDD3  8b4904                  mov      ecx, dword ptr [ecx + 4]       
  0x0020FDD6  d3e7                    shl      edi, cl                        
                                        ; XREF: 0x0020FDC7 (jump)
  0x0020FDD8  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020FDDC  8b490c                  mov      ecx, dword ptr [ecx + 0xc]     
  0x0020FDDF  2bcb                    sub      ecx, ebx                       
  0x0020FDE1  8b5c241c                mov      ebx, dword ptr [esp + 0x1c]    
  0x0020FDE5  49                      dec      ecx                            
  0x0020FDE6  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020FDEA  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020FDEE  8b4904                  mov      ecx, dword ptr [ecx + 4]       
  0x0020FDF1  d3e3                    shl      ebx, cl                        
  0x0020FDF3  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x0020FDF7  85db                    test     ebx, ebx                       
  0x0020FDF9  7e04                    jle      0x20fdff                       
  0x0020FDFB  2bd9                    sub      ebx, ecx                       
  0x0020FDFD  eb02                    jmp      0x20fe01                       
                                        ; XREF: 0x0020FDF9 (cond_jump)
  0x0020FDFF  03d9                    add      ebx, ecx                       
                                        ; XREF: 0x0020FDFD (jump)
  0x0020FE01  895c241c                mov      dword ptr [esp + 0x1c], ebx    
                                        ; XREF: 0x0020FD6B (cond_jump)
  0x0020FE05  8b4c2428                mov      ecx, dword ptr [esp + 0x28]    
  0x0020FE09  8b19                    mov      ebx, dword ptr [ecx]           
  0x0020FE0B  035c241c                add      ebx, dword ptr [esp + 0x1c]    
  0x0020FE0F  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020FE13  8b4908                  mov      ecx, dword ptr [ecx + 8]       
  0x0020FE16  d3e3                    shl      ebx, cl                        
  0x0020FE18  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020FE1C  8b4908                  mov      ecx, dword ptr [ecx + 8]       
  0x0020FE1F  d3fb                    sar      ebx, cl                        
  0x0020FE21  8b4c2424                mov      ecx, dword ptr [esp + 0x24]    
  0x0020FE25  8919                    mov      dword ptr [ecx], ebx           
  0x0020FE27  8b4c2428                mov      ecx, dword ptr [esp + 0x28]    
  0x0020FE2B  8919                    mov      dword ptr [ecx], ebx           
  0x0020FE2D  8b4c2424                mov      ecx, dword ptr [esp + 0x24]    
                                        ; XREF: 0x0020FD5D (jump)
  0x0020FE31  8b5c2420                mov      ebx, dword ptr [esp + 0x20]    
  0x0020FE35  833b00                  cmp      dword ptr [ebx], 0             
  0x0020FE38  7402                    je       0x20fe3c                       
  0x0020FE3A  d121                    shl      dword ptr [ecx], 1             
                                        ; XREF: 0x0020FD0A (jump), 0x0020FE38 (cond_jump)
  0x0020FE3C  8938                    mov      dword ptr [eax], edi           
  0x0020FE3E  5f                      pop      edi                            
  0x0020FE3F  897008                  mov      dword ptr [eax + 8], esi       
  0x0020FE42  5e                      pop      esi                            
  0x0020FE43  896804                  mov      dword ptr [eax + 4], ebp       
  0x0020FE46  5d                      pop      ebp                            
  0x0020FE47  89500c                  mov      dword ptr [eax + 0xc], edx     
  0x0020FE4A  8b442408                mov      eax, dword ptr [esp + 8]       
  0x0020FE4E  5b                      pop      ebx                            
  0x0020FE4F  83c408                  add      esp, 8                         
  0x0020FE52  c3                      ret                                     
  0x0020FE53  90                      nop                                     
  0x0020FE54  90                      nop                                     
  0x0020FE55  90                      nop                                     
  0x0020FE56  90                      nop                                     
  0x0020FE57  90                      nop                                     
  0x0020FE58  90                      nop                                     
  0x0020FE59  90                      nop                                     
  0x0020FE5A  90                      nop                                     
  0x0020FE5B  90                      nop                                     
  0x0020FE5C  90                      nop                                     
  0x0020FE5D  90                      nop                                     
  0x0020FE5E  90                      nop                                     
  0x0020FE5F  90                      nop                                     
  0x0020FE60  83ec0c                  sub      esp, 0xc                       
  0x0020FE63  53                      push     ebx                            
  0x0020FE64  55                      push     ebp                            
  0x0020FE65  56                      push     esi                            
  0x0020FE66  8b74241c                mov      esi, dword ptr [esp + 0x1c]    
  0x0020FE6A  8b5604                  mov      edx, dword ptr [esi + 4]       
  0x0020FE6D  57                      push     edi                            
  0x0020FE6E  c744241401000000        mov      dword ptr [esp + 0x14], 1      
  0x0020FE76  89542420                mov      dword ptr [esp + 0x20], edx    
                                        ; XREF: 0x00210277 (jump), 0x002102D5 (jump)
  0x0020FE7A  8b1e                    mov      ebx, dword ptr [esi]           
  0x0020FE7C  8b6e08                  mov      ebp, dword ptr [esi + 8]       
  0x0020FE7F  8b7e0c                  mov      edi, dword ptr [esi + 0xc]     
  0x0020FE82  8bc3                    mov      eax, ebx                       
  0x0020FE84  c1e809                  shr      eax, 9                         
  0x0020FE87  83fd09                  cmp      ebp, 9                         
  0x0020FE8A  89442410                mov      dword ptr [esp + 0x10], eax    
  0x0020FE8E  7e13                    jle      0x20fea3                       
  0x0020FE90  b929000000              mov      ecx, 0x29                      
  0x0020FE95  2bcd                    sub      ecx, ebp                       
  0x0020FE97  8bc2                    mov      eax, edx                       
  0x0020FE99  d3e8                    shr      eax, cl                        
  0x0020FE9B  8bc8                    mov      ecx, eax                       
  0x0020FE9D  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x0020FEA1  0bc1                    or       eax, ecx                       
                                        ; XREF: 0x0020FE8E (cond_jump)
  0x0020FEA3  85c0                    test     eax, eax                       
  0x0020FEA5  0f842f040000            je       0x2102da                       
  0x0020FEAB  8b869c020000            mov      eax, dword ptr [esi + 0x29c]   
  0x0020FEB1  89442418                mov      dword ptr [esp + 0x18], eax    
                                        ; XREF: 0x0020FF49 (cond_jump), 0x0020FF5B (jump)
  0x0020FEB5  8bc3                    mov      eax, ebx                       
  0x0020FEB7  c1e815                  shr      eax, 0x15                      
  0x0020FEBA  83fd15                  cmp      ebp, 0x15                      
  0x0020FEBD  7e0f                    jle      0x20fece                       
  0x0020FEBF  b935000000              mov      ecx, 0x35                      
  0x0020FEC4  2bcd                    sub      ecx, ebp                       
  0x0020FEC6  d3ea                    shr      edx, cl                        
  0x0020FEC8  0bc2                    or       eax, edx                       
  0x0020FECA  8b542420                mov      edx, dword ptr [esp + 0x20]    
                                        ; XREF: 0x0020FEBD (cond_jump)
  0x0020FECE  a980ffffff              test     eax, 0xffffff80                
  0x0020FED3  7510                    jne      0x20fee5                       
  0x0020FED5  8b0d0046c800            mov      ecx, dword ptr [0xc84600]      
  0x0020FEDB  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x0020FEDF  89442410                mov      dword ptr [esp + 0x10], eax    
  0x0020FEE3  eb11                    jmp      0x20fef6                       
                                        ; XREF: 0x0020FED3 (cond_jump)
  0x0020FEE5  8b0de443c800            mov      ecx, dword ptr [0xc843e4]      
  0x0020FEEB  c1e806                  shr      eax, 6                         
  0x0020FEEE  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x0020FEF2  89442410                mov      dword ptr [esp + 0x10], eax    
                                        ; XREF: 0x0020FEE3 (jump)
  0x0020FEF6  8bc8                    mov      ecx, eax                       
  0x0020FEF8  83e10f                  and      ecx, 0xf                       
  0x0020FEFB  03e9                    add      ebp, ecx                       
  0x0020FEFD  83fd20                  cmp      ebp, 0x20                      
  0x0020FF00  7c37                    jl       0x20ff39                       
  0x0020FF02  0fbe07                  movsx    eax, byte ptr [edi]            
  0x0020FF05  83ed20                  sub      ebp, 0x20                      
  0x0020FF08  8bda                    mov      ebx, edx                       
  0x0020FF0A  8bcd                    mov      ecx, ebp                       
  0x0020FF0C  d3e3                    shl      ebx, cl                        
  0x0020FF0E  0fb64f01                movzx    ecx, byte ptr [edi + 1]        
  0x0020FF12  c1e008                  shl      eax, 8                         
  0x0020FF15  47                      inc      edi                            
  0x0020FF16  0fb65701                movzx    edx, byte ptr [edi + 1]        
  0x0020FF1A  0bc1                    or       eax, ecx                       
  0x0020FF1C  47                      inc      edi                            
  0x0020FF1D  0fb64f01                movzx    ecx, byte ptr [edi + 1]        
  0x0020FF21  c1e008                  shl      eax, 8                         
  0x0020FF24  0bc2                    or       eax, edx                       
  0x0020FF26  47                      inc      edi                            
  0x0020FF27  c1e008                  shl      eax, 8                         
  0x0020FF2A  0bc1                    or       eax, ecx                       
  0x0020FF2C  89442420                mov      dword ptr [esp + 0x20], eax    
  0x0020FF30  8bd0                    mov      edx, eax                       
  0x0020FF32  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x0020FF36  47                      inc      edi                            
  0x0020FF37  eb02                    jmp      0x20ff3b                       
                                        ; XREF: 0x0020FF00 (cond_jump)
  0x0020FF39  d3e3                    shl      ebx, cl                        
                                        ; XREF: 0x0020FF37 (jump)
  0x0020FF3B  8bc8                    mov      ecx, eax                       
  0x0020FF3D  c1e902                  shr      ecx, 2                         
  0x0020FF40  0fb6c9                  movzx    ecx, cl                        
  0x0020FF43  c1e902                  shr      ecx, 2                         
  0x0020FF46  83f922                  cmp      ecx, 0x22                      
  0x0020FF49  0f8466ffffff            je       0x20feb5                       
  0x0020FF4F  83f923                  cmp      ecx, 0x23                      
  0x0020FF52  750c                    jne      0x20ff60                       
  0x0020FF54  83869c02000021          add      dword ptr [esi + 0x29c], 0x21  
  0x0020FF5B  e955ffffff              jmp      0x20feb5                       
                                        ; XREF: 0x0020FF52 (cond_jump)
  0x0020FF60  83f924                  cmp      ecx, 0x24                      
  0x0020FF63  0f8471030000            je       0x2102da                       
  0x0020FF69  018e9c020000            add      dword ptr [esi + 0x29c], ecx   
  0x0020FF6F  8b8e9c020000            mov      ecx, dword ptr [esi + 0x29c]   
  0x0020FF75  c1e80a                  shr      eax, 0xa                       
  0x0020FF78  8986a4020000            mov      dword ptr [esi + 0x2a4], eax   
  0x0020FF7E  3b8ea0020000            cmp      ecx, dword ptr [esi + 0x2a0]   
  0x0020FF84  0f8f50030000            jg       0x2102da                       
  0x0020FF8A  2b4c2418                sub      ecx, dword ptr [esp + 0x18]    
  0x0020FF8E  83f9fe                  cmp      ecx, -2                        
  0x0020FF91  0f8443030000            je       0x2102da                       
  0x0020FF97  8b442414                mov      eax, dword ptr [esp + 0x14]    
  0x0020FF9B  85c0                    test     eax, eax                       
  0x0020FF9D  752b                    jne      0x20ffca                       
  0x0020FF9F  83f901                  cmp      ecx, 1                         
  0x0020FFA2  7626                    jbe      0x20ffca                       
  0x0020FFA4  51                      push     ecx                            
  0x0020FFA5  56                      push     esi                            
  0x0020FFA6  ff9630020000            call     dword ptr [esi + 0x230]        
  0x0020FFAC  8b542428                mov      edx, dword ptr [esp + 0x28]    
  0x0020FFB0  b800040000              mov      eax, 0x400                     
  0x0020FFB5  83c408                  add      esp, 8                         
  0x0020FFB8  8986ac020000            mov      dword ptr [esi + 0x2ac], eax   
  0x0020FFBE  8986b4020000            mov      dword ptr [esi + 0x2b4], eax   
  0x0020FFC4  8986b0020000            mov      dword ptr [esi + 0x2b0], eax   
                                        ; XREF: 0x0020FF9D (cond_jump), 0x0020FFA2 (cond_jump)
  0x0020FFCA  f686a402000020          test     byte ptr [esi + 0x2a4], 0x20   
  0x0020FFD1  7575                    jne      0x210048                       
  0x0020FFD3  8bc3                    mov      eax, ebx                       
  0x0020FFD5  c1e81a                  shr      eax, 0x1a                      
  0x0020FFD8  83fd1a                  cmp      ebp, 0x1a                      
  0x0020FFDB  89442410                mov      dword ptr [esp + 0x10], eax    
  0x0020FFDF  7e13                    jle      0x20fff4                       
  0x0020FFE1  b93a000000              mov      ecx, 0x3a                      
  0x0020FFE6  2bcd                    sub      ecx, ebp                       
  0x0020FFE8  8bc2                    mov      eax, edx                       
  0x0020FFEA  d3e8                    shr      eax, cl                        
  0x0020FFEC  8bc8                    mov      ecx, eax                       
  0x0020FFEE  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x0020FFF2  0bc1                    or       eax, ecx                       
                                        ; XREF: 0x0020FFDF (cond_jump)
  0x0020FFF4  8b0da049c800            mov      ecx, dword ptr [0xc849a0]      
  0x0020FFFA  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x0020FFFE  8bc8                    mov      ecx, eax                       
  0x00210000  c1e908                  shr      ecx, 8                         
  0x00210003  898ea4020000            mov      dword ptr [esi + 0x2a4], ecx   
  0x00210009  0fb6c8                  movzx    ecx, al                        
  0x0021000C  03e9                    add      ebp, ecx                       
  0x0021000E  83fd20                  cmp      ebp, 0x20                      
  0x00210011  7c33                    jl       0x210046                       
  0x00210013  0fbe07                  movsx    eax, byte ptr [edi]            
  0x00210016  83ed20                  sub      ebp, 0x20                      
  0x00210019  8bda                    mov      ebx, edx                       
  0x0021001B  0fb65701                movzx    edx, byte ptr [edi + 1]        
  0x0021001F  8bcd                    mov      ecx, ebp                       
  0x00210021  d3e3                    shl      ebx, cl                        
  0x00210023  47                      inc      edi                            
  0x00210024  0fb64f01                movzx    ecx, byte ptr [edi + 1]        
  0x00210028  c1e008                  shl      eax, 8                         
  0x0021002B  0bc2                    or       eax, edx                       
  0x0021002D  47                      inc      edi                            
  0x0021002E  0fb65701                movzx    edx, byte ptr [edi + 1]        
  0x00210032  c1e008                  shl      eax, 8                         
  0x00210035  0bc1                    or       eax, ecx                       
  0x00210037  47                      inc      edi                            
  0x00210038  c1e008                  shl      eax, 8                         
  0x0021003B  0bc2                    or       eax, edx                       
  0x0021003D  89442420                mov      dword ptr [esp + 0x20], eax    
  0x00210041  47                      inc      edi                            
  0x00210042  8bd0                    mov      edx, eax                       
  0x00210044  eb02                    jmp      0x210048                       
                                        ; XREF: 0x00210011 (cond_jump)
  0x00210046  d3e3                    shl      ebx, cl                        
                                        ; XREF: 0x0020FFD1 (cond_jump), 0x00210044 (jump)
  0x00210048  f686a402000010          test     byte ptr [esi + 0x2a4], 0x10   
  0x0021004F  7461                    je       0x2100b2                       
  0x00210051  83fd1b                  cmp      ebp, 0x1b                      
  0x00210054  7c4b                    jl       0x2100a1                       
  0x00210056  83ed1b                  sub      ebp, 0x1b                      
  0x00210059  7416                    je       0x210071                       
  0x0021005B  b905000000              mov      ecx, 5                         
  0x00210060  2bcd                    sub      ecx, ebp                       
  0x00210062  8bc2                    mov      eax, edx                       
  0x00210064  d3e8                    shr      eax, cl                        
  0x00210066  8bcd                    mov      ecx, ebp                       
  0x00210068  0bc3                    or       eax, ebx                       
  0x0021006A  c1e81b                  shr      eax, 0x1b                      
  0x0021006D  d3e2                    shl      edx, cl                        
  0x0021006F  eb05                    jmp      0x210076                       
                                        ; XREF: 0x00210059 (cond_jump)
  0x00210071  8bc3                    mov      eax, ebx                       
  0x00210073  c1e81b                  shr      eax, 0x1b                      
                                        ; XREF: 0x0021006F (jump)
  0x00210076  0fbe0f                  movsx    ecx, byte ptr [edi]            
  0x00210079  47                      inc      edi                            
  0x0021007A  c1e108                  shl      ecx, 8                         
  0x0021007D  8bda                    mov      ebx, edx                       
  0x0021007F  0fb617                  movzx    edx, byte ptr [edi]            
  0x00210082  0bca                    or       ecx, edx                       
  0x00210084  0fb65701                movzx    edx, byte ptr [edi + 1]        
  0x00210088  47                      inc      edi                            
  0x00210089  c1e108                  shl      ecx, 8                         
  0x0021008C  0bca                    or       ecx, edx                       
  0x0021008E  0fb65701                movzx    edx, byte ptr [edi + 1]        
  0x00210092  47                      inc      edi                            
  0x00210093  c1e108                  shl      ecx, 8                         
  0x00210096  0bca                    or       ecx, edx                       
  0x00210098  894c2420                mov      dword ptr [esp + 0x20], ecx    
  0x0021009C  47                      inc      edi                            
  0x0021009D  8bd1                    mov      edx, ecx                       
  0x0021009F  eb0b                    jmp      0x2100ac                       
                                        ; XREF: 0x00210054 (cond_jump)
  0x002100A1  8bc3                    mov      eax, ebx                       
  0x002100A3  83c505                  add      ebp, 5                         
  0x002100A6  c1e81b                  shr      eax, 0x1b                      
  0x002100A9  c1e305                  shl      ebx, 5                         
                                        ; XREF: 0x0021009F (jump)
  0x002100AC  898650020000            mov      dword ptr [esi + 0x250], eax   
                                        ; XREF: 0x0021004F (cond_jump)
  0x002100B2  f686a402000008          test     byte ptr [esi + 0x2a4], 8      
  0x002100B9  7461                    je       0x21011c                       
  0x002100BB  8d8664020000            lea      eax, [esi + 0x264]             
  0x002100C1  50                      push     eax                            
  0x002100C2  8d8e6c020000            lea      ecx, [esi + 0x26c]             
  0x002100C8  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x002100CB  51                      push     ecx                            
  0x002100CC  8dbe54020000            lea      edi, [esi + 0x254]             
  0x002100D2  57                      push     edi                            
  0x002100D3  56                      push     esi                            
  0x002100D4  891e                    mov      dword ptr [esi], ebx           
  0x002100D6  895604                  mov      dword ptr [esi + 4], edx       
  0x002100D9  896e08                  mov      dword ptr [esi + 8], ebp       
  0x002100DC  e8bffbffff              call     0x20fca0                       ; -> sub_0020FCA0
  0x002100E1  8d9668020000            lea      edx, [esi + 0x268]             
  0x002100E7  52                      push     edx                            
  0x002100E8  8944242c                mov      dword ptr [esp + 0x2c], eax    
  0x002100EC  8d8670020000            lea      eax, [esi + 0x270]             
  0x002100F2  50                      push     eax                            
  0x002100F3  57                      push     edi                            
  0x002100F4  56                      push     esi                            
  0x002100F5  e8a6fbffff              call     0x20fca0                       ; -> sub_0020FCA0
  0x002100FA  8b4e04                  mov      ecx, dword ptr [esi + 4]       
  0x002100FD  8b1e                    mov      ebx, dword ptr [esi]           
  0x002100FF  8b6e08                  mov      ebp, dword ptr [esi + 8]       
  0x00210102  8b7e0c                  mov      edi, dword ptr [esi + 0xc]     
  0x00210105  894c2440                mov      dword ptr [esp + 0x40], ecx    
  0x00210109  8b4c2438                mov      ecx, dword ptr [esp + 0x38]    
  0x0021010D  83c420                  add      esp, 0x20                      
  0x00210110  0bc1                    or       eax, ecx                       
  0x00210112  0f85c2010000            jne      0x2102da                       
  0x00210118  8b542420                mov      edx, dword ptr [esp + 0x20]    
                                        ; XREF: 0x002100B9 (cond_jump)
  0x0021011C  f686a402000004          test     byte ptr [esi + 0x2a4], 4      
  0x00210123  7461                    je       0x210186                       
  0x00210125  895604                  mov      dword ptr [esi + 4], edx       
  0x00210128  8d9688020000            lea      edx, [esi + 0x288]             
  0x0021012E  52                      push     edx                            
  0x0021012F  8d8690020000            lea      eax, [esi + 0x290]             
  0x00210135  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x00210138  50                      push     eax                            
  0x00210139  8dbe78020000            lea      edi, [esi + 0x278]             
  0x0021013F  57                      push     edi                            
  0x00210140  56                      push     esi                            
  0x00210141  891e                    mov      dword ptr [esi], ebx           
  0x00210143  896e08                  mov      dword ptr [esi + 8], ebp       
  0x00210146  e855fbffff              call     0x20fca0                       ; -> sub_0020FCA0
  0x0021014B  8d8e8c020000            lea      ecx, [esi + 0x28c]             
  0x00210151  51                      push     ecx                            
  0x00210152  8d9694020000            lea      edx, [esi + 0x294]             
  0x00210158  52                      push     edx                            
  0x00210159  57                      push     edi                            
  0x0021015A  56                      push     esi                            
  0x0021015B  89442438                mov      dword ptr [esp + 0x38], eax    
  0x0021015F  e83cfbffff              call     0x20fca0                       ; -> sub_0020FCA0
  0x00210164  8b4e04                  mov      ecx, dword ptr [esi + 4]       
  0x00210167  8b1e                    mov      ebx, dword ptr [esi]           
  0x00210169  8b6e08                  mov      ebp, dword ptr [esi + 8]       
  0x0021016C  8b7e0c                  mov      edi, dword ptr [esi + 0xc]     
  0x0021016F  894c2440                mov      dword ptr [esp + 0x40], ecx    
  0x00210173  8b4c2438                mov      ecx, dword ptr [esp + 0x38]    
  0x00210177  83c420                  add      esp, 0x20                      
  0x0021017A  0bc1                    or       eax, ecx                       
  0x0021017C  0f8558010000            jne      0x2102da                       
  0x00210182  8b542420                mov      edx, dword ptr [esp + 0x20]    
                                        ; XREF: 0x00210123 (cond_jump)
  0x00210186  f686a402000002          test     byte ptr [esi + 0x2a4], 2      
  0x0021018D  747a                    je       0x210209                       
  0x0021018F  8bc3                    mov      eax, ebx                       
  0x00210191  c1e817                  shr      eax, 0x17                      
  0x00210194  83fd17                  cmp      ebp, 0x17                      
  0x00210197  89442410                mov      dword ptr [esp + 0x10], eax    
  0x0021019B  7e13                    jle      0x2101b0                       
  0x0021019D  b937000000              mov      ecx, 0x37                      
  0x002101A2  2bcd                    sub      ecx, ebp                       
  0x002101A4  8bc2                    mov      eax, edx                       
  0x002101A6  d3e8                    shr      eax, cl                        
  0x002101A8  8bc8                    mov      ecx, eax                       
  0x002101AA  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x002101AE  0bc1                    or       eax, ecx                       
                                        ; XREF: 0x0021019B (cond_jump)
  0x002101B0  8b0d044ac800            mov      ecx, dword ptr [0xc84a04]      
  0x002101B6  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x002101BA  8bc8                    mov      ecx, eax                       
  0x002101BC  83e1f0                  and      ecx, 0xfffffff0                
  0x002101BF  c1e110                  shl      ecx, 0x10                      
  0x002101C2  898ea8020000            mov      dword ptr [esi + 0x2a8], ecx   
  0x002101C8  0fb6c8                  movzx    ecx, al                        
  0x002101CB  03e9                    add      ebp, ecx                       
  0x002101CD  83fd20                  cmp      ebp, 0x20                      
  0x002101D0  7c33                    jl       0x210205                       
  0x002101D2  0fbe07                  movsx    eax, byte ptr [edi]            
  0x002101D5  83ed20                  sub      ebp, 0x20                      
  0x002101D8  8bda                    mov      ebx, edx                       
  0x002101DA  0fb65701                movzx    edx, byte ptr [edi + 1]        
  0x002101DE  8bcd                    mov      ecx, ebp                       
  0x002101E0  d3e3                    shl      ebx, cl                        
  0x002101E2  47                      inc      edi                            
  0x002101E3  0fb64f01                movzx    ecx, byte ptr [edi + 1]        
  0x002101E7  c1e008                  shl      eax, 8                         
  0x002101EA  0bc2                    or       eax, edx                       
  0x002101EC  47                      inc      edi                            
  0x002101ED  0fb65701                movzx    edx, byte ptr [edi + 1]        
  0x002101F1  c1e008                  shl      eax, 8                         
  0x002101F4  0bc1                    or       eax, ecx                       
  0x002101F6  47                      inc      edi                            
  0x002101F7  c1e008                  shl      eax, 8                         
  0x002101FA  0bc2                    or       eax, edx                       
  0x002101FC  89442420                mov      dword ptr [esp + 0x20], eax    
  0x00210200  47                      inc      edi                            
  0x00210201  8bd0                    mov      edx, eax                       
  0x00210203  eb0e                    jmp      0x210213                       
                                        ; XREF: 0x002101D0 (cond_jump)
  0x00210205  d3e3                    shl      ebx, cl                        
  0x00210207  eb0a                    jmp      0x210213                       
                                        ; XREF: 0x0021018D (cond_jump)
  0x00210209  c786a802000000000000    mov      dword ptr [esi + 0x2a8], 0     
                                        ; XREF: 0x00210203 (jump), 0x00210207 (jump)
  0x00210213  f686a402000001          test     byte ptr [esi + 0x2a4], 1      
  0x0021021A  891e                    mov      dword ptr [esi], ebx           
  0x0021021C  895604                  mov      dword ptr [esi + 4], edx       
  0x0021021F  896e08                  mov      dword ptr [esi + 8], ebp       
  0x00210222  897e0c                  mov      dword ptr [esi + 0xc], edi     
  0x00210225  7455                    je       0x21027c                       
  0x00210227  56                      push     esi                            
  0x00210228  ff9634020000            call     dword ptr [esi + 0x234]        
  0x0021022E  56                      push     esi                            
  0x0021022F  ff963c020000            call     dword ptr [esi + 0x23c]        
  0x00210235  83c408                  add      esp, 8                         
  0x00210238  33c0                    xor      eax, eax                       
  0x0021023A  898664020000            mov      dword ptr [esi + 0x264], eax   
  0x00210240  898668020000            mov      dword ptr [esi + 0x268], eax   
  0x00210246  89866c020000            mov      dword ptr [esi + 0x26c], eax   
  0x0021024C  898670020000            mov      dword ptr [esi + 0x270], eax   
  0x00210252  898688020000            mov      dword ptr [esi + 0x288], eax   
  0x00210258  89868c020000            mov      dword ptr [esi + 0x28c], eax   
  0x0021025E  898690020000            mov      dword ptr [esi + 0x290], eax   
  0x00210264  898694020000            mov      dword ptr [esi + 0x294], eax   
  0x0021026A  8b4e04                  mov      ecx, dword ptr [esi + 4]       
  0x0021026D  894c2420                mov      dword ptr [esp + 0x20], ecx    
  0x00210271  89442414                mov      dword ptr [esp + 0x14], eax    
  0x00210275  8bd1                    mov      edx, ecx                       
  0x00210277  e9fefbffff              jmp      0x20fe7a                       
                                        ; XREF: 0x00210225 (cond_jump)
  0x0021027C  8b86a8020000            mov      eax, dword ptr [esi + 0x2a8]   
  0x00210282  85c0                    test     eax, eax                       
  0x00210284  740a                    je       0x210290                       
  0x00210286  56                      push     esi                            
  0x00210287  ff9638020000            call     dword ptr [esi + 0x238]        
  0x0021028D  83c404                  add      esp, 4                         
                                        ; XREF: 0x00210284 (cond_jump)
  0x00210290  8b86a4020000            mov      eax, dword ptr [esi + 0x2a4]   
  0x00210296  c1f802                  sar      eax, 2                         
  0x00210299  83e003                  and      eax, 3                         
  0x0021029C  8b848640020000          mov      eax, dword ptr [esi + eax*4 + 0x240] 
  0x002102A3  56                      push     esi                            
  0x002102A4  898640020000            mov      dword ptr [esi + 0x240], eax   
  0x002102AA  ffd0                    call     eax                            
  0x002102AC  8b4e04                  mov      ecx, dword ptr [esi + 4]       
  0x002102AF  b800040000              mov      eax, 0x400                     
  0x002102B4  83c404                  add      esp, 4                         
  0x002102B7  8986ac020000            mov      dword ptr [esi + 0x2ac], eax   
  0x002102BD  8986b4020000            mov      dword ptr [esi + 0x2b4], eax   
  0x002102C3  8986b0020000            mov      dword ptr [esi + 0x2b0], eax   
  0x002102C9  33c0                    xor      eax, eax                       
  0x002102CB  894c2420                mov      dword ptr [esp + 0x20], ecx    
  0x002102CF  89442414                mov      dword ptr [esp + 0x14], eax    
  0x002102D3  8bd1                    mov      edx, ecx                       
  0x002102D5  e9a0fbffff              jmp      0x20fe7a                       
                                        ; XREF: 0x0020FEA5 (cond_jump), 0x0020FF63 (cond_jump), 0x0020FF84 (cond_jump), 0x0020FF91 (cond_jump), 0x00210112 (cond_jump), ... (+1 more)
  0x002102DA  8b542424                mov      edx, dword ptr [esp + 0x24]    
  0x002102DE  8b1a                    mov      ebx, dword ptr [edx]           
  0x002102E0  8b74242c                mov      esi, dword ptr [esp + 0x2c]    
  0x002102E4  83c507                  add      ebp, 7                         
  0x002102E7  c1fd03                  sar      ebp, 3                         
  0x002102EA  8d4c2ff8                lea      ecx, [edi + ebp - 8]           
  0x002102EE  8b3e                    mov      edi, dword ptr [esi]           
  0x002102F0  8bc1                    mov      eax, ecx                       
  0x002102F2  2bc3                    sub      eax, ebx                       
  0x002102F4  03f8                    add      edi, eax                       
  0x002102F6  893e                    mov      dword ptr [esi], edi           
  0x002102F8  8b7c2428                mov      edi, dword ptr [esp + 0x28]    
  0x002102FC  8b2f                    mov      ebp, dword ptr [edi]           
  0x002102FE  56                      push     esi                            
  0x002102FF  2be8                    sub      ebp, eax                       
  0x00210301  57                      push     edi                            
  0x00210302  892f                    mov      dword ptr [edi], ebp           
  0x00210304  52                      push     edx                            
  0x00210305  890a                    mov      dword ptr [edx], ecx           
  0x00210307  e8949df9ff              call     0x1aa0a0                       ; -> sub_001AA0A0
  0x0021030C  83c40c                  add      esp, 0xc                       
  0x0021030F  5f                      pop      edi                            
  0x00210310  5e                      pop      esi                            
  0x00210311  5d                      pop      ebp                            
  0x00210312  5b                      pop      ebx                            
  0x00210313  83c40c                  add      esp, 0xc                       
  0x00210316  c3                      ret                                     
  0x00210317  90                      nop                                     
  0x00210318  90                      nop                                     
  0x00210319  90                      nop                                     
  0x0021031A  90                      nop                                     
  0x0021031B  90                      nop                                     
  0x0021031C  90                      nop                                     
  0x0021031D  90                      nop                                     
  0x0021031E  90                      nop                                     
  0x0021031F  90                      nop                                     
