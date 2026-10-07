; ============================================================
; Section: PSGSFD_I
; VA: 0x0020FA60 - 0x0020FC50
; Size: 496 bytes (0.5 KB)
; Functions: 0
; Instructions: 177
; ============================================================

  0x0020FA60  51                      push     ecx                            
  0x0020FA61  53                      push     ebx                            
  0x0020FA62  55                      push     ebp                            
  0x0020FA63  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020FA67  56                      push     esi                            
  0x0020FA68  57                      push     edi                            
                                        ; XREF: 0x0020FC06 (jump)
  0x0020FA69  8b7d00                  mov      edi, dword ptr [ebp]           
  0x0020FA6C  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x0020FA6F  8b5d04                  mov      ebx, dword ptr [ebp + 4]       
  0x0020FA72  8b450c                  mov      eax, dword ptr [ebp + 0xc]     
  0x0020FA75  8bcf                    mov      ecx, edi                       
  0x0020FA77  c1e909                  shr      ecx, 9                         
  0x0020FA7A  83fa09                  cmp      edx, 9                         
  0x0020FA7D  895c2418                mov      dword ptr [esp + 0x18], ebx    
  0x0020FA81  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020FA85  7e11                    jle      0x20fa98                       
  0x0020FA87  b929000000              mov      ecx, 0x29                      
  0x0020FA8C  2bca                    sub      ecx, edx                       
  0x0020FA8E  8bf3                    mov      esi, ebx                       
  0x0020FA90  d3ee                    shr      esi, cl                        
  0x0020FA92  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x0020FA96  0bce                    or       ecx, esi                       
                                        ; XREF: 0x0020FA85 (cond_jump)
  0x0020FA98  85c9                    test     ecx, ecx                       
  0x0020FA9A  0f846b010000            je       0x20fc0b                       
  0x0020FAA0  8b8d9c020000            mov      ecx, dword ptr [ebp + 0x29c]   
  0x0020FAA6  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020FAAA  8d9b00000000            lea      ebx, [ebx]                     
                                        ; XREF: 0x0020FB35 (cond_jump), 0x0020FB47 (jump)
  0x0020FAB0  8bf7                    mov      esi, edi                       
  0x0020FAB2  c1ee14                  shr      esi, 0x14                      
  0x0020FAB5  83fa14                  cmp      edx, 0x14                      
  0x0020FAB8  7e0f                    jle      0x20fac9                       
  0x0020FABA  b934000000              mov      ecx, 0x34                      
  0x0020FABF  2bca                    sub      ecx, edx                       
  0x0020FAC1  d3eb                    shr      ebx, cl                        
  0x0020FAC3  0bf3                    or       esi, ebx                       
  0x0020FAC5  8b5c2418                mov      ebx, dword ptr [esp + 0x18]    
                                        ; XREF: 0x0020FAB8 (cond_jump)
  0x0020FAC9  f7c600ffffff            test     esi, 0xffffff00                
  0x0020FACF  7508                    jne      0x20fad9                       
  0x0020FAD1  8b0de44ac800            mov      ecx, dword ptr [0xc84ae4]      
  0x0020FAD7  eb09                    jmp      0x20fae2                       
                                        ; XREF: 0x0020FACF (cond_jump)
  0x0020FAD9  8b0dc047c800            mov      ecx, dword ptr [0xc847c0]      
  0x0020FADF  c1ee06                  shr      esi, 6                         
                                        ; XREF: 0x0020FAD7 (jump)
  0x0020FAE2  0fbf3471                movsx    esi, word ptr [ecx + esi*2]    
  0x0020FAE6  8bce                    mov      ecx, esi                       
  0x0020FAE8  83e10f                  and      ecx, 0xf                       
  0x0020FAEB  03d1                    add      edx, ecx                       
  0x0020FAED  83fa20                  cmp      edx, 0x20                      
  0x0020FAF0  7c33                    jl       0x20fb25                       
  0x0020FAF2  83ea20                  sub      edx, 0x20                      
  0x0020FAF5  8bfb                    mov      edi, ebx                       
  0x0020FAF7  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020FAFB  8bca                    mov      ecx, edx                       
  0x0020FAFD  d3e7                    shl      edi, cl                        
  0x0020FAFF  0fbe08                  movsx    ecx, byte ptr [eax]            
  0x0020FB02  40                      inc      eax                            
  0x0020FB03  c1e108                  shl      ecx, 8                         
  0x0020FB06  0bcb                    or       ecx, ebx                       
  0x0020FB08  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020FB0C  40                      inc      eax                            
  0x0020FB0D  c1e108                  shl      ecx, 8                         
  0x0020FB10  0bcb                    or       ecx, ebx                       
  0x0020FB12  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020FB16  40                      inc      eax                            
  0x0020FB17  c1e108                  shl      ecx, 8                         
  0x0020FB1A  0bcb                    or       ecx, ebx                       
  0x0020FB1C  8bd9                    mov      ebx, ecx                       
  0x0020FB1E  895c2418                mov      dword ptr [esp + 0x18], ebx    
  0x0020FB22  40                      inc      eax                            
  0x0020FB23  eb02                    jmp      0x20fb27                       
                                        ; XREF: 0x0020FAF0 (cond_jump)
  0x0020FB25  d3e7                    shl      edi, cl                        
                                        ; XREF: 0x0020FB23 (jump)
  0x0020FB27  8bce                    mov      ecx, esi                       
  0x0020FB29  c1e902                  shr      ecx, 2                         
  0x0020FB2C  0fb6c9                  movzx    ecx, cl                        
  0x0020FB2F  c1e902                  shr      ecx, 2                         
  0x0020FB32  83f922                  cmp      ecx, 0x22                      
  0x0020FB35  0f8475ffffff            je       0x20fab0                       
  0x0020FB3B  83f923                  cmp      ecx, 0x23                      
  0x0020FB3E  750c                    jne      0x20fb4c                       
  0x0020FB40  83859c02000021          add      dword ptr [ebp + 0x29c], 0x21  
  0x0020FB47  e964ffffff              jmp      0x20fab0                       
                                        ; XREF: 0x0020FB3E (cond_jump)
  0x0020FB4C  83f924                  cmp      ecx, 0x24                      
  0x0020FB4F  0f84b6000000            je       0x20fc0b                       
  0x0020FB55  018d9c020000            add      dword ptr [ebp + 0x29c], ecx   
  0x0020FB5B  8b8d9c020000            mov      ecx, dword ptr [ebp + 0x29c]   
  0x0020FB61  c1ee0a                  shr      esi, 0xa                       
  0x0020FB64  89b5a4020000            mov      dword ptr [ebp + 0x2a4], esi   
  0x0020FB6A  3b8da0020000            cmp      ecx, dword ptr [ebp + 0x2a0]   
  0x0020FB70  0f8f95000000            jg       0x20fc0b                       
  0x0020FB76  2b4c2410                sub      ecx, dword ptr [esp + 0x10]    
  0x0020FB7A  83f9fe                  cmp      ecx, -2                        
  0x0020FB7D  0f8488000000            je       0x20fc0b                       
  0x0020FB83  f685a402000010          test     byte ptr [ebp + 0x2a4], 0x10   
  0x0020FB8A  745d                    je       0x20fbe9                       
  0x0020FB8C  83fa1b                  cmp      edx, 0x1b                      
  0x0020FB8F  7c47                    jl       0x20fbd8                       
  0x0020FB91  83ea1b                  sub      edx, 0x1b                      
  0x0020FB94  7416                    je       0x20fbac                       
  0x0020FB96  b905000000              mov      ecx, 5                         
  0x0020FB9B  2bca                    sub      ecx, edx                       
  0x0020FB9D  8bf3                    mov      esi, ebx                       
  0x0020FB9F  d3ee                    shr      esi, cl                        
  0x0020FBA1  8bca                    mov      ecx, edx                       
  0x0020FBA3  0bf7                    or       esi, edi                       
  0x0020FBA5  c1ee1b                  shr      esi, 0x1b                      
  0x0020FBA8  d3e3                    shl      ebx, cl                        
  0x0020FBAA  eb05                    jmp      0x20fbb1                       
                                        ; XREF: 0x0020FB94 (cond_jump)
  0x0020FBAC  8bf7                    mov      esi, edi                       
  0x0020FBAE  c1ee1b                  shr      esi, 0x1b                      
                                        ; XREF: 0x0020FBAA (jump)
  0x0020FBB1  0fbe08                  movsx    ecx, byte ptr [eax]            
  0x0020FBB4  40                      inc      eax                            
  0x0020FBB5  c1e108                  shl      ecx, 8                         
  0x0020FBB8  8bfb                    mov      edi, ebx                       
  0x0020FBBA  0fb618                  movzx    ebx, byte ptr [eax]            
  0x0020FBBD  0bcb                    or       ecx, ebx                       
  0x0020FBBF  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020FBC3  40                      inc      eax                            
  0x0020FBC4  c1e108                  shl      ecx, 8                         
  0x0020FBC7  0bcb                    or       ecx, ebx                       
  0x0020FBC9  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020FBCD  40                      inc      eax                            
  0x0020FBCE  c1e108                  shl      ecx, 8                         
  0x0020FBD1  0bcb                    or       ecx, ebx                       
  0x0020FBD3  8bd9                    mov      ebx, ecx                       
  0x0020FBD5  40                      inc      eax                            
  0x0020FBD6  eb0b                    jmp      0x20fbe3                       
                                        ; XREF: 0x0020FB8F (cond_jump)
  0x0020FBD8  8bf7                    mov      esi, edi                       
  0x0020FBDA  83c205                  add      edx, 5                         
  0x0020FBDD  c1ee1b                  shr      esi, 0x1b                      
  0x0020FBE0  c1e705                  shl      edi, 5                         
                                        ; XREF: 0x0020FBD6 (jump)
  0x0020FBE3  89b550020000            mov      dword ptr [ebp + 0x250], esi   
                                        ; XREF: 0x0020FB8A (cond_jump)
  0x0020FBE9  55                      push     ebp                            
  0x0020FBEA  897d00                  mov      dword ptr [ebp], edi           
  0x0020FBED  895d04                  mov      dword ptr [ebp + 4], ebx       
  0x0020FBF0  895508                  mov      dword ptr [ebp + 8], edx       
  0x0020FBF3  89450c                  mov      dword ptr [ebp + 0xc], eax     
  0x0020FBF6  ff9534020000            call     dword ptr [ebp + 0x234]        
  0x0020FBFC  55                      push     ebp                            
  0x0020FBFD  ff953c020000            call     dword ptr [ebp + 0x23c]        
  0x0020FC03  83c408                  add      esp, 8                         
  0x0020FC06  e95efeffff              jmp      0x20fa69                       
                                        ; XREF: 0x0020FA9A (cond_jump), 0x0020FB4F (cond_jump), 0x0020FB70 (cond_jump), 0x0020FB7D (cond_jump)
  0x0020FC0B  8b742424                mov      esi, dword ptr [esp + 0x24]    
  0x0020FC0F  8b3e                    mov      edi, dword ptr [esi]           
  0x0020FC11  83c207                  add      edx, 7                         
  0x0020FC14  c1fa03                  sar      edx, 3                         
  0x0020FC17  8d4c02f8                lea      ecx, [edx + eax - 8]           
  0x0020FC1B  8b54241c                mov      edx, dword ptr [esp + 0x1c]    
  0x0020FC1F  8b1a                    mov      ebx, dword ptr [edx]           
  0x0020FC21  8bc1                    mov      eax, ecx                       
  0x0020FC23  2bc3                    sub      eax, ebx                       
  0x0020FC25  03f8                    add      edi, eax                       
  0x0020FC27  893e                    mov      dword ptr [esi], edi           
  0x0020FC29  8b7c2420                mov      edi, dword ptr [esp + 0x20]    
  0x0020FC2D  8b2f                    mov      ebp, dword ptr [edi]           
  0x0020FC2F  56                      push     esi                            
  0x0020FC30  2be8                    sub      ebp, eax                       
  0x0020FC32  57                      push     edi                            
  0x0020FC33  892f                    mov      dword ptr [edi], ebp           
  0x0020FC35  52                      push     edx                            
  0x0020FC36  890a                    mov      dword ptr [edx], ecx           
  0x0020FC38  e863a4f9ff              call     0x1aa0a0                       ; -> sub_001AA0A0
  0x0020FC3D  83c40c                  add      esp, 0xc                       
  0x0020FC40  5f                      pop      edi                            
  0x0020FC41  5e                      pop      esi                            
  0x0020FC42  5d                      pop      ebp                            
  0x0020FC43  5b                      pop      ebx                            
  0x0020FC44  59                      pop      ecx                            
  0x0020FC45  c3                      ret                                     
  0x0020FC46  90                      nop                                     
  0x0020FC47  90                      nop                                     
  0x0020FC48  90                      nop                                     
  0x0020FC49  90                      nop                                     
  0x0020FC4A  90                      nop                                     
  0x0020FC4B  90                      nop                                     
  0x0020FC4C  90                      nop                                     
  0x0020FC4D  90                      nop                                     
  0x0020FC4E  90                      nop                                     
  0x0020FC4F  90                      nop                                     
