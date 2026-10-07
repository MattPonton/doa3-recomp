; ============================================================
; Section: PSGSFD00
; VA: 0x0020C6C0 - 0x0020FA50
; Size: 13200 bytes (12.9 KB)
; Functions: 11
; Instructions: 4485
; ============================================================

  0x0020C6C0  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0020C6C4  8b88f0010000            mov      ecx, dword ptr [eax + 0x1f0]   
  0x0020C6CA  8d9180010000            lea      edx, [ecx + 0x180]             
  0x0020C6D0  8990f0010000            mov      dword ptr [eax + 0x1f0], edx   
  0x0020C6D6  8b5038                  mov      edx, dword ptr [eax + 0x38]    
  0x0020C6D9  52                      push     edx                            
  0x0020C6DA  8d902c010000            lea      edx, [eax + 0x12c]             
  0x0020C6E0  52                      push     edx                            
  0x0020C6E1  51                      push     ecx                            
  0x0020C6E2  05e0020000              add      eax, 0x2e0                     
  0x0020C6E7  50                      push     eax                            
  0x0020C6E8  e833240000              call     0x20eb20                       ; -> sub_0020EB20
  0x0020C6ED  83c410                  add      esp, 0x10                      
  0x0020C6F0  c3                      ret                                     
  0x0020C6F1  90                      nop                                     
  0x0020C6F2  90                      nop                                     
  0x0020C6F3  90                      nop                                     
  0x0020C6F4  90                      nop                                     
  0x0020C6F5  90                      nop                                     
  0x0020C6F6  90                      nop                                     
  0x0020C6F7  90                      nop                                     
  0x0020C6F8  90                      nop                                     
  0x0020C6F9  90                      nop                                     
  0x0020C6FA  90                      nop                                     
  0x0020C6FB  90                      nop                                     
  0x0020C6FC  90                      nop                                     
  0x0020C6FD  90                      nop                                     
  0x0020C6FE  90                      nop                                     
  0x0020C6FF  90                      nop                                     
  0x0020C700  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0020C704  53                      push     ebx                            
  0x0020C705  8b98f0010000            mov      ebx, dword ptr [eax + 0x1f0]   
  0x0020C70B  55                      push     ebp                            
  0x0020C70C  56                      push     esi                            
  0x0020C70D  57                      push     edi                            
  0x0020C70E  8d9380010000            lea      edx, [ebx + 0x180]             
  0x0020C714  8990f0010000            mov      dword ptr [eax + 0x1f0], edx   
  0x0020C71A  8b909c020000            mov      edx, dword ptr [eax + 0x29c]   
  0x0020C720  0fbeb450680c0000        movsx    esi, byte ptr [eax + edx*2 + 0xc68] 
  0x0020C728  0fbebc50690c0000        movsx    edi, byte ptr [eax + edx*2 + 0xc69] 
  0x0020C730  0fbfa8ee010000          movsx    ebp, word ptr [eax + 0x1ee]    
  0x0020C737  8d9450680c0000          lea      edx, [eax + edx*2 + 0xc68]     
  0x0020C73E  0fbf90ec010000          movsx    edx, word ptr [eax + 0x1ec]    
  0x0020C745  c1e603                  shl      esi, 3                         
  0x0020C748  0fafd6                  imul     edx, esi                       
  0x0020C74B  03f6                    add      esi, esi                       
  0x0020C74D  0faff5                  imul     esi, ebp                       
  0x0020C750  c1e703                  shl      edi, 3                         
  0x0020C753  03d7                    add      edx, edi                       
  0x0020C755  8d88f8000000            lea      ecx, [eax + 0xf8]              
  0x0020C75B  8d347e                  lea      esi, [esi + edi*2]             
  0x0020C75E  8bb8e0010000            mov      edi, dword ptr [eax + 0x1e0]   
  0x0020C764  03fa                    add      edi, edx                       
  0x0020C766  897904                  mov      dword ptr [ecx + 4], edi       
  0x0020C769  8bb8e4010000            mov      edi, dword ptr [eax + 0x1e4]   
  0x0020C76F  03fa                    add      edi, edx                       
  0x0020C771  89790c                  mov      dword ptr [ecx + 0xc], edi     
  0x0020C774  8b90e8010000            mov      edx, dword ptr [eax + 0x1e8]   
  0x0020C77A  03d6                    add      edx, esi                       
  0x0020C77C  895114                  mov      dword ptr [ecx + 0x14], edx    
  0x0020C77F  83c208                  add      edx, 8                         
  0x0020C782  89511c                  mov      dword ptr [ecx + 0x1c], edx    
  0x0020C785  8b5114                  mov      edx, dword ptr [ecx + 0x14]    
  0x0020C788  8d14ea                  lea      edx, [edx + ebp*8]             
  0x0020C78B  895124                  mov      dword ptr [ecx + 0x24], edx    
  0x0020C78E  83c208                  add      edx, 8                         
  0x0020C791  89512c                  mov      dword ptr [ecx + 0x2c], edx    
  0x0020C794  8b5038                  mov      edx, dword ptr [eax + 0x38]    
  0x0020C797  52                      push     edx                            
  0x0020C798  51                      push     ecx                            
  0x0020C799  05e0020000              add      eax, 0x2e0                     
  0x0020C79E  53                      push     ebx                            
  0x0020C79F  50                      push     eax                            
  0x0020C7A0  e87b230000              call     0x20eb20                       ; -> sub_0020EB20
  0x0020C7A5  83c410                  add      esp, 0x10                      
  0x0020C7A8  5f                      pop      edi                            
  0x0020C7A9  5e                      pop      esi                            
  0x0020C7AA  5d                      pop      ebp                            
  0x0020C7AB  5b                      pop      ebx                            
  0x0020C7AC  c3                      ret                                     
  0x0020C7AD  90                      nop                                     
  0x0020C7AE  90                      nop                                     
  0x0020C7AF  90                      nop                                     

; ============================================================
; Function: sub_0020C7B0
; Start: 0x0020C7B0  End: 0x0020C9AC  Size: 508 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020C7B0:
  0x0020C7B0  83ec30                  sub      esp, 0x30                      
  0x0020C7B3  8b442434                mov      eax, dword ptr [esp + 0x34]    
  0x0020C7B7  8b4c2440                mov      ecx, dword ptr [esp + 0x40]    
  0x0020C7BB  53                      push     ebx                            
  0x0020C7BC  55                      push     ebp                            
  0x0020C7BD  0fbf690e                movsx    ebp, word ptr [ecx + 0xe]      
  0x0020C7C1  56                      push     esi                            
  0x0020C7C2  0fbf710c                movsx    esi, word ptr [ecx + 0xc]      
  0x0020C7C6  8b889c020000            mov      ecx, dword ptr [eax + 0x29c]   
  0x0020C7CC  8d8c48680c0000          lea      ecx, [eax + ecx*2 + 0xc68]     
  0x0020C7D3  8bde                    mov      ebx, esi                       
  0x0020C7D5  57                      push     edi                            
  0x0020C7D6  8bb874010000            mov      edi, dword ptr [eax + 0x174]   
  0x0020C7DC  0fbe01                  movsx    eax, byte ptr [ecx]            
  0x0020C7DF  0fbe4901                movsx    ecx, byte ptr [ecx + 1]        
  0x0020C7E3  c1e003                  shl      eax, 3                         
  0x0020C7E6  0fafd8                  imul     ebx, eax                       
  0x0020C7E9  03c0                    add      eax, eax                       
  0x0020C7EB  0fafc5                  imul     eax, ebp                       
  0x0020C7EE  89742410                mov      dword ptr [esp + 0x10], esi    
  0x0020C7F2  8b74244c                mov      esi, dword ptr [esp + 0x4c]    
  0x0020C7F6  c1e103                  shl      ecx, 3                         
  0x0020C7F9  03d9                    add      ebx, ecx                       
  0x0020C7FB  8d0c48                  lea      ecx, [eax + ecx*2]             
  0x0020C7FE  894e04                  mov      dword ptr [esi + 4], ecx       
  0x0020C801  891e                    mov      dword ptr [esi], ebx           
  0x0020C803  8b421c                  mov      eax, dword ptr [edx + 0x1c]    
  0x0020C806  8b5a18                  mov      ebx, dword ptr [edx + 0x18]    
  0x0020C809  89442414                mov      dword ptr [esp + 0x14], eax    
  0x0020C80D  99                      cdq                                     
  0x0020C80E  2bc2                    sub      eax, edx                       
  0x0020C810  8bf0                    mov      esi, eax                       
  0x0020C812  d1fe                    sar      esi, 1                         
  0x0020C814  894c2418                mov      dword ptr [esp + 0x18], ecx    
  0x0020C818  8bce                    mov      ecx, esi                       
  0x0020C81A  8bc3                    mov      eax, ebx                       
  0x0020C81C  99                      cdq                                     
  0x0020C81D  2bc2                    sub      eax, edx                       
  0x0020C81F  d1f8                    sar      eax, 1                         
  0x0020C821  8bd0                    mov      edx, eax                       
  0x0020C823  83e001                  and      eax, 1                         
  0x0020C826  89442428                mov      dword ptr [esp + 0x28], eax    
  0x0020C82A  8b44244c                mov      eax, dword ptr [esp + 0x4c]    
  0x0020C82E  d1f9                    sar      ecx, 1                         
  0x0020C830  0faf4c2410              imul     ecx, dword ptr [esp + 0x10]    
  0x0020C835  83e601                  and      esi, 1                         
  0x0020C838  8974242c                mov      dword ptr [esp + 0x2c], esi    
  0x0020C83C  0308                    add      ecx, dword ptr [eax]           
  0x0020C83E  8b742414                mov      esi, dword ptr [esp + 0x14]    
  0x0020C842  8bc6                    mov      eax, esi                       
  0x0020C844  d1f8                    sar      eax, 1                         
  0x0020C846  0fafc5                  imul     eax, ebp                       
  0x0020C849  d1fa                    sar      edx, 1                         
  0x0020C84B  03ca                    add      ecx, edx                       
  0x0020C84D  83e601                  and      esi, 1                         
  0x0020C850  8974243c                mov      dword ptr [esp + 0x3c], esi    
  0x0020C854  03442418                add      eax, dword ptr [esp + 0x18]    
  0x0020C858  8b742444                mov      esi, dword ptr [esp + 0x44]    
  0x0020C85C  8bd3                    mov      edx, ebx                       
  0x0020C85E  d1fa                    sar      edx, 1                         
  0x0020C860  03c2                    add      eax, edx                       
  0x0020C862  8b54242c                mov      edx, dword ptr [esp + 0x2c]    
  0x0020C866  89442418                mov      dword ptr [esp + 0x18], eax    
  0x0020C86A  8d047a                  lea      eax, [edx + edi*2]             
  0x0020C86D  8b542428                mov      edx, dword ptr [esp + 0x28]    
  0x0020C871  8d0442                  lea      eax, [edx + eax*2]             
  0x0020C874  8b0485ccb02100          mov      eax, dword ptr [eax*4 + 0x21b0cc] 
  0x0020C87B  8944244c                mov      dword ptr [esp + 0x4c], eax    
  0x0020C87F  8b44243c                mov      eax, dword ptr [esp + 0x3c]    
  0x0020C883  8d0478                  lea      eax, [eax + edi*2]             
  0x0020C886  83e301                  and      ebx, 1                         
  0x0020C889  8d0443                  lea      eax, [ebx + eax*2]             
  0x0020C88C  8b0485ccb02100          mov      eax, dword ptr [eax*4 + 0x21b0cc] 
  0x0020C893  89442414                mov      dword ptr [esp + 0x14], eax    
  0x0020C897  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x0020C89B  81c6c4000000            add      esi, 0xc4                      
  0x0020C8A1  894610                  mov      dword ptr [esi + 0x10], eax    
  0x0020C8A4  8b442448                mov      eax, dword ptr [esp + 0x48]    
  0x0020C8A8  894c241c                mov      dword ptr [esp + 0x1c], ecx    
  0x0020C8AC  895c2438                mov      dword ptr [esp + 0x38], ebx    
  0x0020C8B0  894608                  mov      dword ptr [esi + 8], eax       
  0x0020C8B3  8b442450                mov      eax, dword ptr [esp + 0x50]    
  0x0020C8B7  8b00                    mov      eax, dword ptr [eax]           
  0x0020C8B9  03c1                    add      eax, ecx                       
  0x0020C8BB  8bdf                    mov      ebx, edi                       
  0x0020C8BD  f7db                    neg      ebx                            
  0x0020C8BF  1bdb                    sbb      ebx, ebx                       
  0x0020C8C1  23da                    and      ebx, edx                       
  0x0020C8C3  8b542410                mov      edx, dword ptr [esp + 0x10]    
  0x0020C8C7  8d0c03                  lea      ecx, [ebx + eax]               
  0x0020C8CA  03ca                    add      ecx, edx                       
  0x0020C8CC  56                      push     esi                            
  0x0020C8CD  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020C8D0  894e18                  mov      dword ptr [esi + 0x18], ecx    
  0x0020C8D3  ff542450                call     dword ptr [esp + 0x50]         
  0x0020C8D7  8b54244c                mov      edx, dword ptr [esp + 0x4c]    
  0x0020C8DB  8b442454                mov      eax, dword ptr [esp + 0x54]    
  0x0020C8DF  8b4c2414                mov      ecx, dword ptr [esp + 0x14]    
  0x0020C8E3  83c240                  add      edx, 0x40                      
  0x0020C8E6  895608                  mov      dword ptr [esi + 8], edx       
  0x0020C8E9  8b4004                  mov      eax, dword ptr [eax + 4]       
  0x0020C8EC  03442420                add      eax, dword ptr [esp + 0x20]    
  0x0020C8F0  03d8                    add      ebx, eax                       
  0x0020C8F2  03d9                    add      ebx, ecx                       
  0x0020C8F4  56                      push     esi                            
  0x0020C8F5  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020C8F8  895e18                  mov      dword ptr [esi + 0x18], ebx    
  0x0020C8FB  ff542454                call     dword ptr [esp + 0x54]         
  0x0020C8FF  8b5c2450                mov      ebx, dword ptr [esp + 0x50]    
  0x0020C903  8b542458                mov      edx, dword ptr [esp + 0x58]    
  0x0020C907  8d8b80000000            lea      ecx, [ebx + 0x80]              
  0x0020C90D  896e10                  mov      dword ptr [esi + 0x10], ebp    
  0x0020C910  894e08                  mov      dword ptr [esi + 8], ecx       
  0x0020C913  8b4208                  mov      eax, dword ptr [edx + 8]       
  0x0020C916  03442420                add      eax, dword ptr [esp + 0x20]    
  0x0020C91A  8b542440                mov      edx, dword ptr [esp + 0x40]    
  0x0020C91E  f7df                    neg      edi                            
  0x0020C920  1bff                    sbb      edi, edi                       
  0x0020C922  23fa                    and      edi, edx                       
  0x0020C924  03f8                    add      edi, eax                       
  0x0020C926  03fd                    add      edi, ebp                       
  0x0020C928  897e18                  mov      dword ptr [esi + 0x18], edi    
  0x0020C92B  8b7c241c                mov      edi, dword ptr [esp + 0x1c]    
  0x0020C92F  56                      push     esi                            
  0x0020C930  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020C933  ffd7                    call     edi                            
  0x0020C935  8b5614                  mov      edx, dword ptr [esi + 0x14]    
  0x0020C938  8b4e18                  mov      ecx, dword ptr [esi + 0x18]    
  0x0020C93B  8d83c0000000            lea      eax, [ebx + 0xc0]              
  0x0020C941  894608                  mov      dword ptr [esi + 8], eax       
  0x0020C944  b808000000              mov      eax, 8                         
  0x0020C949  03d0                    add      edx, eax                       
  0x0020C94B  03c8                    add      ecx, eax                       
  0x0020C94D  56                      push     esi                            
  0x0020C94E  895614                  mov      dword ptr [esi + 0x14], edx    
  0x0020C951  894e18                  mov      dword ptr [esi + 0x18], ecx    
  0x0020C954  ffd7                    call     edi                            
  0x0020C956  8b4614                  mov      eax, dword ptr [esi + 0x14]    
  0x0020C959  8d14edf8ffffff          lea      edx, [ebp*8 - 8]               
  0x0020C960  03c2                    add      eax, edx                       
  0x0020C962  8d8b00010000            lea      ecx, [ebx + 0x100]             
  0x0020C968  894e08                  mov      dword ptr [esi + 8], ecx       
  0x0020C96B  8b4e18                  mov      ecx, dword ptr [esi + 0x18]    
  0x0020C96E  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020C971  8d04edf8ffffff          lea      eax, [ebp*8 - 8]               
  0x0020C978  03c8                    add      ecx, eax                       
  0x0020C97A  56                      push     esi                            
  0x0020C97B  894e18                  mov      dword ptr [esi + 0x18], ecx    
  0x0020C97E  ffd7                    call     edi                            
  0x0020C980  8b5618                  mov      edx, dword ptr [esi + 0x18]    
  0x0020C983  81c340010000            add      ebx, 0x140                     
  0x0020C989  895e08                  mov      dword ptr [esi + 8], ebx       
  0x0020C98C  8b5e14                  mov      ebx, dword ptr [esi + 0x14]    
  0x0020C98F  b808000000              mov      eax, 8                         
  0x0020C994  03d8                    add      ebx, eax                       
  0x0020C996  03d0                    add      edx, eax                       
  0x0020C998  895e14                  mov      dword ptr [esi + 0x14], ebx    
  0x0020C99B  895618                  mov      dword ptr [esi + 0x18], edx    
  0x0020C99E  56                      push     esi                            
  0x0020C99F  ffd7                    call     edi                            
  0x0020C9A1  83c418                  add      esp, 0x18                      
  0x0020C9A4  5f                      pop      edi                            
  0x0020C9A5  5e                      pop      esi                            
  0x0020C9A6  5d                      pop      ebp                            
  0x0020C9A7  5b                      pop      ebx                            
  0x0020C9A8  83c430                  add      esp, 0x30                      
  0x0020C9AB  c3                      ret                                     
; end of function
  0x0020C9AC  90                      nop                                     
  0x0020C9AD  90                      nop                                     
  0x0020C9AE  90                      nop                                     
  0x0020C9AF  90                      nop                                     
  0x0020C9B0  83ec10                  sub      esp, 0x10                      
  0x0020C9B3  53                      push     ebx                            
  0x0020C9B4  8b5c2418                mov      ebx, dword ptr [esp + 0x18]    
  0x0020C9B8  0fbf83de010000          movsx    eax, word ptr [ebx + 0x1de]    
  0x0020C9BF  0fbf8bdc010000          movsx    ecx, word ptr [ebx + 0x1dc]    
  0x0020C9C6  89442408                mov      dword ptr [esp + 8], eax       
  0x0020C9CA  8b44241c                mov      eax, dword ptr [esp + 0x1c]    
  0x0020C9CE  894c2404                mov      dword ptr [esp + 4], ecx       
  0x0020C9D2  8b8bf0010000            mov      ecx, dword ptr [ebx + 0x1f0]   
  0x0020C9D8  8d1440                  lea      edx, [eax + eax*2]             
  0x0020C9DB  c1e207                  shl      edx, 7                         
  0x0020C9DE  56                      push     esi                            
  0x0020C9DF  894c241c                mov      dword ptr [esp + 0x1c], ecx    
  0x0020C9E3  8d8c0a80feffff          lea      ecx, [edx + ecx - 0x180]       
  0x0020C9EA  48                      dec      eax                            
  0x0020C9EB  57                      push     edi                            
  0x0020C9EC  8dbb60060000            lea      edi, [ebx + 0x660]             
  0x0020C9F2  8db3c4000000            lea      esi, [ebx + 0xc4]              
  0x0020C9F8  898bf0010000            mov      dword ptr [ebx + 0x1f0], ecx   
  0x0020C9FE  89442424                mov      dword ptr [esp + 0x24], eax    
  0x0020CA02  0f8422010000            je       0x20cb2a                       
  0x0020CA08  55                      push     ebp                            
  0x0020CA09  8da42400000000          lea      esp, [esp]                     
                                        ; XREF: 0x0020CB23 (cond_jump)
  0x0020CA10  8b939c020000            mov      edx, dword ptr [ebx + 0x29c]   
  0x0020CA16  0fbfabde010000          movsx    ebp, word ptr [ebx + 0x1de]    
  0x0020CA1D  2bd0                    sub      edx, eax                       
  0x0020CA1F  0fbe8c53680c0000        movsx    ecx, byte ptr [ebx + edx*2 + 0xc68] 
  0x0020CA27  8d8453680c0000          lea      eax, [ebx + edx*2 + 0xc68]     
  0x0020CA2E  0fbe5001                movsx    edx, byte ptr [eax + 1]        
  0x0020CA32  0fbf83dc010000          movsx    eax, word ptr [ebx + 0x1dc]    
  0x0020CA39  c1e103                  shl      ecx, 3                         
  0x0020CA3C  0fafc1                  imul     eax, ecx                       
  0x0020CA3F  03c9                    add      ecx, ecx                       
  0x0020CA41  0fafcd                  imul     ecx, ebp                       
  0x0020CA44  c1e203                  shl      edx, 3                         
  0x0020CA47  03c2                    add      eax, edx                       
  0x0020CA49  8d1451                  lea      edx, [ecx + edx*2]             
  0x0020CA4C  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x0020CA50  8954241c                mov      dword ptr [esp + 0x1c], edx    
  0x0020CA54  8b542424                mov      edx, dword ptr [esp + 0x24]    
  0x0020CA58  894e0c                  mov      dword ptr [esi + 0xc], ecx     
  0x0020CA5B  895604                  mov      dword ptr [esi + 4], edx       
  0x0020CA5E  8b8bd0010000            mov      ecx, dword ptr [ebx + 0x1d0]   
  0x0020CA64  03c8                    add      ecx, eax                       
  0x0020CA66  57                      push     edi                            
  0x0020CA67  56                      push     esi                            
  0x0020CA68  89442420                mov      dword ptr [esp + 0x20], eax    
  0x0020CA6C  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x0020CA6F  e8ec2e0000              call     0x20f960                       ; -> sub_0020F960
  0x0020CA74  894604                  mov      dword ptr [esi + 4], eax       
  0x0020CA77  8b93d4010000            mov      edx, dword ptr [ebx + 0x1d4]   
  0x0020CA7D  8b442420                mov      eax, dword ptr [esp + 0x20]    
  0x0020CA81  83c740                  add      edi, 0x40                      
  0x0020CA84  03d0                    add      edx, eax                       
  0x0020CA86  57                      push     edi                            
  0x0020CA87  56                      push     esi                            
  0x0020CA88  895614                  mov      dword ptr [esi + 0x14], edx    
  0x0020CA8B  e8d02e0000              call     0x20f960                       ; -> sub_0020F960
  0x0020CA90  8b6c2424                mov      ebp, dword ptr [esp + 0x24]    
  0x0020CA94  8b54242c                mov      edx, dword ptr [esp + 0x2c]    
  0x0020CA98  894604                  mov      dword ptr [esi + 4], eax       
  0x0020CA9B  896e0c                  mov      dword ptr [esi + 0xc], ebp     
  0x0020CA9E  8b83d8010000            mov      eax, dword ptr [ebx + 0x1d8]   
  0x0020CAA4  83c740                  add      edi, 0x40                      
  0x0020CAA7  03c2                    add      eax, edx                       
  0x0020CAA9  57                      push     edi                            
  0x0020CAAA  56                      push     esi                            
  0x0020CAAB  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020CAAE  e8ad2e0000              call     0x20f960                       ; -> sub_0020F960
  0x0020CAB3  8b5614                  mov      edx, dword ptr [esi + 0x14]    
  0x0020CAB6  83c740                  add      edi, 0x40                      
  0x0020CAB9  83c208                  add      edx, 8                         
  0x0020CABC  57                      push     edi                            
  0x0020CABD  56                      push     esi                            
  0x0020CABE  894604                  mov      dword ptr [esi + 4], eax       
  0x0020CAC1  895614                  mov      dword ptr [esi + 0x14], edx    
  0x0020CAC4  e8972e0000              call     0x20f960                       ; -> sub_0020F960
  0x0020CAC9  894604                  mov      dword ptr [esi + 4], eax       
  0x0020CACC  8b4614                  mov      eax, dword ptr [esi + 0x14]    
  0x0020CACF  83c740                  add      edi, 0x40                      
  0x0020CAD2  8d0cedf8ffffff          lea      ecx, [ebp*8 - 8]               
  0x0020CAD9  03c1                    add      eax, ecx                       
  0x0020CADB  57                      push     edi                            
  0x0020CADC  56                      push     esi                            
  0x0020CADD  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020CAE0  e87b2e0000              call     0x20f960                       ; -> sub_0020F960
  0x0020CAE5  894604                  mov      dword ptr [esi + 4], eax       
  0x0020CAE8  8b4614                  mov      eax, dword ptr [esi + 0x14]    
  0x0020CAEB  83c740                  add      edi, 0x40                      
  0x0020CAEE  83c008                  add      eax, 8                         
  0x0020CAF1  57                      push     edi                            
  0x0020CAF2  56                      push     esi                            
  0x0020CAF3  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020CAF6  e8652e0000              call     0x20f960                       ; -> sub_0020F960
  0x0020CAFB  89442454                mov      dword ptr [esp + 0x54], eax    
  0x0020CAFF  8d83e0010000            lea      eax, [ebx + 0x1e0]             
  0x0020CB05  50                      push     eax                            
  0x0020CB06  8d54244c                lea      edx, [esp + 0x4c]              
  0x0020CB0A  52                      push     edx                            
  0x0020CB0B  8dbb60060000            lea      edi, [ebx + 0x660]             
  0x0020CB11  57                      push     edi                            
  0x0020CB12  e8d92a0000              call     0x20f5f0                       ; -> sub_0020F5F0
  0x0020CB17  8b442464                mov      eax, dword ptr [esp + 0x64]    
  0x0020CB1B  83c43c                  add      esp, 0x3c                      
  0x0020CB1E  48                      dec      eax                            
  0x0020CB1F  89442428                mov      dword ptr [esp + 0x28], eax    
  0x0020CB23  0f85e7feffff            jne      0x20ca10                       
  0x0020CB29  5d                      pop      ebp                            
                                        ; XREF: 0x0020CA02 (cond_jump)
  0x0020CB2A  5f                      pop      edi                            
  0x0020CB2B  5e                      pop      esi                            
  0x0020CB2C  5b                      pop      ebx                            
  0x0020CB2D  83c410                  add      esp, 0x10                      
  0x0020CB30  c3                      ret                                     
  0x0020CB31  90                      nop                                     
  0x0020CB32  90                      nop                                     
  0x0020CB33  90                      nop                                     
  0x0020CB34  90                      nop                                     
  0x0020CB35  90                      nop                                     
  0x0020CB36  90                      nop                                     
  0x0020CB37  90                      nop                                     
  0x0020CB38  90                      nop                                     
  0x0020CB39  90                      nop                                     
  0x0020CB3A  90                      nop                                     
  0x0020CB3B  90                      nop                                     
  0x0020CB3C  90                      nop                                     
  0x0020CB3D  90                      nop                                     
  0x0020CB3E  90                      nop                                     
  0x0020CB3F  90                      nop                                     
  0x0020CB40  83ec08                  sub      esp, 8                         
  0x0020CB43  53                      push     ebx                            
  0x0020CB44  56                      push     esi                            
  0x0020CB45  8b742414                mov      esi, dword ptr [esp + 0x14]    
  0x0020CB49  8b8ef0000000            mov      ecx, dword ptr [esi + 0xf0]    
  0x0020CB4F  57                      push     edi                            
  0x0020CB50  8dbed0010000            lea      edi, [esi + 0x1d0]             
  0x0020CB56  57                      push     edi                            
  0x0020CB57  8d442410                lea      eax, [esp + 0x10]              
  0x0020CB5B  8d9ee8000000            lea      ebx, [esi + 0xe8]              
  0x0020CB61  50                      push     eax                            
  0x0020CB62  51                      push     ecx                            
  0x0020CB63  8d9654020000            lea      edx, [esi + 0x254]             
  0x0020CB69  56                      push     esi                            
  0x0020CB6A  e841fcffff              call     0x20c7b0                       ; -> sub_0020C7B0
  0x0020CB6F  8b4720                  mov      eax, dword ptr [edi + 0x20]    
  0x0020CB72  8d9080010000            lea      edx, [eax + 0x180]             
  0x0020CB78  895720                  mov      dword ptr [edi + 0x20], edx    
  0x0020CB7B  8b8ea8020000            mov      ecx, dword ptr [esi + 0x2a8]   
  0x0020CB81  51                      push     ecx                            
  0x0020CB82  53                      push     ebx                            
  0x0020CB83  50                      push     eax                            
  0x0020CB84  e817210000              call     0x20eca0                       ; -> sub_0020ECA0
  0x0020CB89  8b4308                  mov      eax, dword ptr [ebx + 8]       
  0x0020CB8C  83c710                  add      edi, 0x10                      
  0x0020CB8F  57                      push     edi                            
  0x0020CB90  8d54242c                lea      edx, [esp + 0x2c]              
  0x0020CB94  52                      push     edx                            
  0x0020CB95  50                      push     eax                            
  0x0020CB96  e8552a0000              call     0x20f5f0                       ; -> sub_0020F5F0
  0x0020CB9B  83c428                  add      esp, 0x28                      
  0x0020CB9E  5f                      pop      edi                            
  0x0020CB9F  5e                      pop      esi                            
  0x0020CBA0  5b                      pop      ebx                            
  0x0020CBA1  83c408                  add      esp, 8                         
  0x0020CBA4  c3                      ret                                     
  0x0020CBA5  90                      nop                                     
  0x0020CBA6  90                      nop                                     
  0x0020CBA7  90                      nop                                     
  0x0020CBA8  90                      nop                                     
  0x0020CBA9  90                      nop                                     
  0x0020CBAA  90                      nop                                     
  0x0020CBAB  90                      nop                                     
  0x0020CBAC  90                      nop                                     
  0x0020CBAD  90                      nop                                     
  0x0020CBAE  90                      nop                                     
  0x0020CBAF  90                      nop                                     
  0x0020CBB0  83ec08                  sub      esp, 8                         
  0x0020CBB3  53                      push     ebx                            
  0x0020CBB4  56                      push     esi                            
  0x0020CBB5  8b742414                mov      esi, dword ptr [esp + 0x14]    
  0x0020CBB9  8b8ef0000000            mov      ecx, dword ptr [esi + 0xf0]    
  0x0020CBBF  57                      push     edi                            
  0x0020CBC0  8dbed0010000            lea      edi, [esi + 0x1d0]             
  0x0020CBC6  57                      push     edi                            
  0x0020CBC7  8d442410                lea      eax, [esp + 0x10]              
  0x0020CBCB  8d9ee8000000            lea      ebx, [esi + 0xe8]              
  0x0020CBD1  50                      push     eax                            
  0x0020CBD2  51                      push     ecx                            
  0x0020CBD3  8d9654020000            lea      edx, [esi + 0x254]             
  0x0020CBD9  56                      push     esi                            
  0x0020CBDA  e8d1fbffff              call     0x20c7b0                       ; -> sub_0020C7B0
  0x0020CBDF  8b4720                  mov      eax, dword ptr [edi + 0x20]    
  0x0020CBE2  8d9080010000            lea      edx, [eax + 0x180]             
  0x0020CBE8  895720                  mov      dword ptr [edi + 0x20], edx    
  0x0020CBEB  8b8ea8020000            mov      ecx, dword ptr [esi + 0x2a8]   
  0x0020CBF1  51                      push     ecx                            
  0x0020CBF2  53                      push     ebx                            
  0x0020CBF3  50                      push     eax                            
  0x0020CBF4  e8a7200000              call     0x20eca0                       ; -> sub_0020ECA0
  0x0020CBF9  83c41c                  add      esp, 0x1c                      
  0x0020CBFC  5f                      pop      edi                            
  0x0020CBFD  5e                      pop      esi                            
  0x0020CBFE  5b                      pop      ebx                            
  0x0020CBFF  83c408                  add      esp, 8                         
  0x0020CC02  c3                      ret                                     
  0x0020CC03  90                      nop                                     
  0x0020CC04  90                      nop                                     
  0x0020CC05  90                      nop                                     
  0x0020CC06  90                      nop                                     
  0x0020CC07  90                      nop                                     
  0x0020CC08  90                      nop                                     
  0x0020CC09  90                      nop                                     
  0x0020CC0A  90                      nop                                     
  0x0020CC0B  90                      nop                                     
  0x0020CC0C  90                      nop                                     
  0x0020CC0D  90                      nop                                     
  0x0020CC0E  90                      nop                                     
  0x0020CC0F  90                      nop                                     
  0x0020CC10  83ec08                  sub      esp, 8                         
  0x0020CC13  56                      push     esi                            
  0x0020CC14  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x0020CC18  57                      push     edi                            
  0x0020CC19  8d86e0010000            lea      eax, [esi + 0x1e0]             
  0x0020CC1F  50                      push     eax                            
  0x0020CC20  8b86f0000000            mov      eax, dword ptr [esi + 0xf0]    
  0x0020CC26  8d4c240c                lea      ecx, [esp + 0xc]               
  0x0020CC2A  8dbee8000000            lea      edi, [esi + 0xe8]              
  0x0020CC30  51                      push     ecx                            
  0x0020CC31  50                      push     eax                            
  0x0020CC32  8d9678020000            lea      edx, [esi + 0x278]             
  0x0020CC38  56                      push     esi                            
  0x0020CC39  e872fbffff              call     0x20c7b0                       ; -> sub_0020C7B0
  0x0020CC3E  8b86f0010000            mov      eax, dword ptr [esi + 0x1f0]   
  0x0020CC44  8d8880010000            lea      ecx, [eax + 0x180]             
  0x0020CC4A  898ef0010000            mov      dword ptr [esi + 0x1f0], ecx   
  0x0020CC50  8b96a8020000            mov      edx, dword ptr [esi + 0x2a8]   
  0x0020CC56  52                      push     edx                            
  0x0020CC57  57                      push     edi                            
  0x0020CC58  50                      push     eax                            
  0x0020CC59  e842200000              call     0x20eca0                       ; -> sub_0020ECA0
  0x0020CC5E  83c41c                  add      esp, 0x1c                      
  0x0020CC61  5f                      pop      edi                            
  0x0020CC62  5e                      pop      esi                            
  0x0020CC63  83c408                  add      esp, 8                         
  0x0020CC66  c3                      ret                                     
  0x0020CC67  90                      nop                                     
  0x0020CC68  90                      nop                                     
  0x0020CC69  90                      nop                                     
  0x0020CC6A  90                      nop                                     
  0x0020CC6B  90                      nop                                     
  0x0020CC6C  90                      nop                                     
  0x0020CC6D  90                      nop                                     
  0x0020CC6E  90                      nop                                     
  0x0020CC6F  90                      nop                                     
  0x0020CC70  83ec08                  sub      esp, 8                         
  0x0020CC73  53                      push     ebx                            
  0x0020CC74  56                      push     esi                            
  0x0020CC75  8b742414                mov      esi, dword ptr [esp + 0x14]    
  0x0020CC79  8b8ef0000000            mov      ecx, dword ptr [esi + 0xf0]    
  0x0020CC7F  57                      push     edi                            
  0x0020CC80  8dbed0010000            lea      edi, [esi + 0x1d0]             
  0x0020CC86  57                      push     edi                            
  0x0020CC87  8d442410                lea      eax, [esp + 0x10]              
  0x0020CC8B  8d9ee8000000            lea      ebx, [esi + 0xe8]              
  0x0020CC91  50                      push     eax                            
  0x0020CC92  51                      push     ecx                            
  0x0020CC93  8d9654020000            lea      edx, [esi + 0x254]             
  0x0020CC99  56                      push     esi                            
  0x0020CC9A  e811fbffff              call     0x20c7b0                       ; -> sub_0020C7B0
  0x0020CC9F  8d4710                  lea      eax, [edi + 0x10]              
  0x0020CCA2  50                      push     eax                            
  0x0020CCA3  8b430c                  mov      eax, dword ptr [ebx + 0xc]     
  0x0020CCA6  8d4c2420                lea      ecx, [esp + 0x20]              
  0x0020CCAA  51                      push     ecx                            
  0x0020CCAB  50                      push     eax                            
  0x0020CCAC  8d9678020000            lea      edx, [esi + 0x278]             
  0x0020CCB2  56                      push     esi                            
  0x0020CCB3  e8f8faffff              call     0x20c7b0                       ; -> sub_0020C7B0
  0x0020CCB8  8b4720                  mov      eax, dword ptr [edi + 0x20]    
  0x0020CCBB  8d8880010000            lea      ecx, [eax + 0x180]             
  0x0020CCC1  894f20                  mov      dword ptr [edi + 0x20], ecx    
  0x0020CCC4  8b96a8020000            mov      edx, dword ptr [esi + 0x2a8]   
  0x0020CCCA  52                      push     edx                            
  0x0020CCCB  53                      push     ebx                            
  0x0020CCCC  50                      push     eax                            
  0x0020CCCD  e89e220000              call     0x20ef70                       ; -> sub_0020EF70
  0x0020CCD2  83c42c                  add      esp, 0x2c                      
  0x0020CCD5  5f                      pop      edi                            
  0x0020CCD6  5e                      pop      esi                            
  0x0020CCD7  5b                      pop      ebx                            
  0x0020CCD8  83c408                  add      esp, 8                         
  0x0020CCDB  c3                      ret                                     
  0x0020CCDC  90                      nop                                     
  0x0020CCDD  90                      nop                                     
  0x0020CCDE  90                      nop                                     
  0x0020CCDF  90                      nop                                     

; ============================================================
; Function: sub_0020CCE0
; Start: 0x0020CCE0  End: 0x0020CCF3  Size: 19 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020CCE0:
  0x0020CCE0  83ec0c                  sub      esp, 0xc                       
  0x0020CCE3  53                      push     ebx                            
  0x0020CCE4  55                      push     ebp                            
  0x0020CCE5  57                      push     edi                            
  0x0020CCE6  83c204                  add      edx, 4                         
  0x0020CCE9  c744241006000000        mov      dword ptr [esp + 0x10], 6      
  0x0020CCF1  eb04                    jmp      0x20ccf7                       
; end of function
                                        ; XREF: 0x0020CF7C (cond_jump)
  0x0020CCF3  8b542414                mov      edx, dword ptr [esp + 0x14]    
                                        ; XREF: 0x0020CCF1 (jump)
  0x0020CCF7  8b02                    mov      eax, dword ptr [edx]           
  0x0020CCF9  8b7a04                  mov      edi, dword ptr [edx + 4]       
  0x0020CCFC  83c204                  add      edx, 4                         
  0x0020CCFF  83c204                  add      edx, 4                         
  0x0020CD02  a81f                    test     al, 0x1f                       
  0x0020CD04  89542414                mov      dword ptr [esp + 0x14], edx    
  0x0020CD08  0f8583000000            jne      0x20cd91                       
  0x0020CD0E  bd08000000              mov      ebp, 8                         
                                        ; XREF: 0x0020CD8A (cond_jump)
  0x0020CD13  0fbf5904                movsx    ebx, word ptr [ecx + 4]        
  0x0020CD17  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CD1A  0fbf5102                movsx    edx, word ptr [ecx + 2]        
  0x0020CD1E  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CD21  0f1808                  prefetcht0 byte ptr [eax]                 
  0x0020CD24  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CD28  0fbf5906                movsx    ebx, word ptr [ecx + 6]        
  0x0020CD2C  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CD2F  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CD33  0fbf19                  movsx    ebx, word ptr [ecx]            
  0x0020CD36  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CD39  885001                  mov      byte ptr [eax + 1], dl         
  0x0020CD3C  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CD40  885002                  mov      byte ptr [eax + 2], dl         
  0x0020CD43  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CD47  885003                  mov      byte ptr [eax + 3], dl         
  0x0020CD4A  8818                    mov      byte ptr [eax], bl             
  0x0020CD4C  0fbf590c                movsx    ebx, word ptr [ecx + 0xc]      
  0x0020CD50  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CD53  0fbf510a                movsx    edx, word ptr [ecx + 0xa]      
  0x0020CD57  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CD5A  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CD5E  0fbf590e                movsx    ebx, word ptr [ecx + 0xe]      
  0x0020CD62  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CD65  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CD69  0fbf5908                movsx    ebx, word ptr [ecx + 8]        
  0x0020CD6D  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CD70  885005                  mov      byte ptr [eax + 5], dl         
  0x0020CD73  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CD77  885006                  mov      byte ptr [eax + 6], dl         
  0x0020CD7A  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CD7E  885804                  mov      byte ptr [eax + 4], bl         
  0x0020CD81  885007                  mov      byte ptr [eax + 7], dl         
  0x0020CD84  83c110                  add      ecx, 0x10                      
  0x0020CD87  03c7                    add      eax, edi                       
  0x0020CD89  4d                      dec      ebp                            
  0x0020CD8A  7587                    jne      0x20cd13                       
  0x0020CD8C  e9e7010000              jmp      0x20cf78                       
                                        ; XREF: 0x0020CD08 (cond_jump)
  0x0020CD91  bd02000000              mov      ebp, 2                         
  0x0020CD96  eb08                    jmp      0x20cda0                       
  0x0020CD98  8da42400000000          lea      esp, [esp]                     
  0x0020CD9F  90                      nop                                     
                                        ; XREF: 0x0020CD96 (jump), 0x0020CF72 (cond_jump)
  0x0020CDA0  0fbf5904                movsx    ebx, word ptr [ecx + 4]        
  0x0020CDA4  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CDA7  0fbf5102                movsx    edx, word ptr [ecx + 2]        
  0x0020CDAB  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CDAE  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CDB2  0fbf5906                movsx    ebx, word ptr [ecx + 6]        
  0x0020CDB6  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CDB9  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CDBD  0fbf19                  movsx    ebx, word ptr [ecx]            
  0x0020CDC0  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CDC3  885001                  mov      byte ptr [eax + 1], dl         
  0x0020CDC6  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CDCA  885002                  mov      byte ptr [eax + 2], dl         
  0x0020CDCD  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CDD1  885003                  mov      byte ptr [eax + 3], dl         
  0x0020CDD4  8818                    mov      byte ptr [eax], bl             
  0x0020CDD6  0fbf590c                movsx    ebx, word ptr [ecx + 0xc]      
  0x0020CDDA  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CDDD  0fbf510a                movsx    edx, word ptr [ecx + 0xa]      
  0x0020CDE1  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CDE4  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CDE8  0fbf590e                movsx    ebx, word ptr [ecx + 0xe]      
  0x0020CDEC  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CDEF  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CDF3  0fbf5908                movsx    ebx, word ptr [ecx + 8]        
  0x0020CDF7  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CDFA  885005                  mov      byte ptr [eax + 5], dl         
  0x0020CDFD  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CE01  885006                  mov      byte ptr [eax + 6], dl         
  0x0020CE04  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CE08  885007                  mov      byte ptr [eax + 7], dl         
  0x0020CE0B  885804                  mov      byte ptr [eax + 4], bl         
  0x0020CE0E  0fbf5914                movsx    ebx, word ptr [ecx + 0x14]     
  0x0020CE12  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CE15  0fbf5112                movsx    edx, word ptr [ecx + 0x12]     
  0x0020CE19  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CE1C  83c110                  add      ecx, 0x10                      
  0x0020CE1F  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CE23  0fbf5906                movsx    ebx, word ptr [ecx + 6]        
  0x0020CE27  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CE2A  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CE2E  0fbf19                  movsx    ebx, word ptr [ecx]            
  0x0020CE31  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CE34  88543801                mov      byte ptr [eax + edi + 1], dl   
  0x0020CE38  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CE3C  88543802                mov      byte ptr [eax + edi + 2], dl   
  0x0020CE40  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CE44  88543803                mov      byte ptr [eax + edi + 3], dl   
  0x0020CE48  881c38                  mov      byte ptr [eax + edi], bl       
  0x0020CE4B  0fbf590c                movsx    ebx, word ptr [ecx + 0xc]      
  0x0020CE4F  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CE52  0fbf510a                movsx    edx, word ptr [ecx + 0xa]      
  0x0020CE56  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CE59  03c7                    add      eax, edi                       
  0x0020CE5B  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CE5F  0fbf590e                movsx    ebx, word ptr [ecx + 0xe]      
  0x0020CE63  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CE66  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CE6A  0fbf5908                movsx    ebx, word ptr [ecx + 8]        
  0x0020CE6E  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CE71  885005                  mov      byte ptr [eax + 5], dl         
  0x0020CE74  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CE78  885006                  mov      byte ptr [eax + 6], dl         
  0x0020CE7B  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CE7F  885007                  mov      byte ptr [eax + 7], dl         
  0x0020CE82  885804                  mov      byte ptr [eax + 4], bl         
  0x0020CE85  0fbf5914                movsx    ebx, word ptr [ecx + 0x14]     
  0x0020CE89  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CE8C  0fbf5112                movsx    edx, word ptr [ecx + 0x12]     
  0x0020CE90  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CE93  83c110                  add      ecx, 0x10                      
  0x0020CE96  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CE9A  0fbf5906                movsx    ebx, word ptr [ecx + 6]        
  0x0020CE9E  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CEA1  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CEA5  0fbf19                  movsx    ebx, word ptr [ecx]            
  0x0020CEA8  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CEAB  03c7                    add      eax, edi                       
  0x0020CEAD  885001                  mov      byte ptr [eax + 1], dl         
  0x0020CEB0  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CEB4  8818                    mov      byte ptr [eax], bl             
  0x0020CEB6  885002                  mov      byte ptr [eax + 2], dl         
  0x0020CEB9  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CEBD  885003                  mov      byte ptr [eax + 3], dl         
  0x0020CEC0  0fbf590c                movsx    ebx, word ptr [ecx + 0xc]      
  0x0020CEC4  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CEC7  0fbf510a                movsx    edx, word ptr [ecx + 0xa]      
  0x0020CECB  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CECE  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CED2  0fbf590e                movsx    ebx, word ptr [ecx + 0xe]      
  0x0020CED6  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CED9  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CEDD  0fbf5908                movsx    ebx, word ptr [ecx + 8]        
  0x0020CEE1  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CEE4  885005                  mov      byte ptr [eax + 5], dl         
  0x0020CEE7  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CEEB  885006                  mov      byte ptr [eax + 6], dl         
  0x0020CEEE  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CEF2  885007                  mov      byte ptr [eax + 7], dl         
  0x0020CEF5  885804                  mov      byte ptr [eax + 4], bl         
  0x0020CEF8  0fbf5914                movsx    ebx, word ptr [ecx + 0x14]     
  0x0020CEFC  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CEFF  0fbf5112                movsx    edx, word ptr [ecx + 0x12]     
  0x0020CF03  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CF06  83c110                  add      ecx, 0x10                      
  0x0020CF09  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CF0D  0fbf5906                movsx    ebx, word ptr [ecx + 6]        
  0x0020CF11  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CF14  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CF18  0fbf19                  movsx    ebx, word ptr [ecx]            
  0x0020CF1B  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CF1E  88543801                mov      byte ptr [eax + edi + 1], dl   
  0x0020CF22  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CF26  03c7                    add      eax, edi                       
  0x0020CF28  885002                  mov      byte ptr [eax + 2], dl         
  0x0020CF2B  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CF2F  885003                  mov      byte ptr [eax + 3], dl         
  0x0020CF32  8818                    mov      byte ptr [eax], bl             
  0x0020CF34  0fbf590c                movsx    ebx, word ptr [ecx + 0xc]      
  0x0020CF38  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CF3B  0fbf510a                movsx    edx, word ptr [ecx + 0xa]      
  0x0020CF3F  8a1432                  mov      dl, byte ptr [edx + esi]       
  0x0020CF42  885c240e                mov      byte ptr [esp + 0xe], bl       
  0x0020CF46  0fbf590e                movsx    ebx, word ptr [ecx + 0xe]      
  0x0020CF4A  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CF4D  885c240f                mov      byte ptr [esp + 0xf], bl       
  0x0020CF51  0fbf5908                movsx    ebx, word ptr [ecx + 8]        
  0x0020CF55  8a1c33                  mov      bl, byte ptr [ebx + esi]       
  0x0020CF58  885005                  mov      byte ptr [eax + 5], dl         
  0x0020CF5B  8a54240e                mov      dl, byte ptr [esp + 0xe]       
  0x0020CF5F  885006                  mov      byte ptr [eax + 6], dl         
  0x0020CF62  8a54240f                mov      dl, byte ptr [esp + 0xf]       
  0x0020CF66  885804                  mov      byte ptr [eax + 4], bl         
  0x0020CF69  885007                  mov      byte ptr [eax + 7], dl         
  0x0020CF6C  83c110                  add      ecx, 0x10                      
  0x0020CF6F  03c7                    add      eax, edi                       
  0x0020CF71  4d                      dec      ebp                            
  0x0020CF72  0f8528feffff            jne      0x20cda0                       
                                        ; XREF: 0x0020CD8C (jump)
  0x0020CF78  ff4c2410                dec      dword ptr [esp + 0x10]         
  0x0020CF7C  0f8571fdffff            jne      0x20ccf3                       
  0x0020CF82  5f                      pop      edi                            
  0x0020CF83  5d                      pop      ebp                            
  0x0020CF84  5b                      pop      ebx                            
  0x0020CF85  83c40c                  add      esp, 0xc                       
  0x0020CF88  c3                      ret                                     
  0x0020CF89  90                      nop                                     
  0x0020CF8A  90                      nop                                     
  0x0020CF8B  90                      nop                                     
  0x0020CF8C  90                      nop                                     
  0x0020CF8D  90                      nop                                     
  0x0020CF8E  90                      nop                                     
  0x0020CF8F  90                      nop                                     

; ============================================================
; Function: sub_0020CF90
; Start: 0x0020CF90  End: 0x0020D047  Size: 183 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020CF90:
  0x0020CF90  83ec10                  sub      esp, 0x10                      
  0x0020CF93  53                      push     ebx                            
  0x0020CF94  55                      push     ebp                            
  0x0020CF95  56                      push     esi                            
  0x0020CF96  8b7004                  mov      esi, dword ptr [eax + 4]       
  0x0020CF99  8bd9                    mov      ebx, ecx                       
  0x0020CF9B  8b4808                  mov      ecx, dword ptr [eax + 8]       
  0x0020CF9E  57                      push     edi                            
  0x0020CF9F  8b38                    mov      edi, dword ptr [eax]           
  0x0020CFA1  83c304                  add      ebx, 4                         
  0x0020CFA4  c744241806000000        mov      dword ptr [esp + 0x18], 6      
  0x0020CFAC  8d642400                lea      esp, [esp]                     
                                        ; XREF: 0x0020D2DD (cond_jump)
  0x0020CFB0  8b03                    mov      eax, dword ptr [ebx]           
  0x0020CFB2  8b5304                  mov      edx, dword ptr [ebx + 4]       
  0x0020CFB5  8b6c2424                mov      ebp, dword ptr [esp + 0x24]    
  0x0020CFB9  83c304                  add      ebx, 4                         
  0x0020CFBC  83c304                  add      ebx, 4                         
  0x0020CFBF  85ed                    test     ebp, ebp                       
  0x0020CFC1  89542410                mov      dword ptr [esp + 0x10], edx    
  0x0020CFC5  895c241c                mov      dword ptr [esp + 0x1c], ebx    
  0x0020CFC9  7c7c                    jl       0x20d047                       
  0x0020CFCB  8b29                    mov      ebp, dword ptr [ecx]           
  0x0020CFCD  8928                    mov      dword ptr [eax], ebp           
  0x0020CFCF  8b6904                  mov      ebp, dword ptr [ecx + 4]       
  0x0020CFD2  896804                  mov      dword ptr [eax + 4], ebp       
  0x0020CFD5  8b6908                  mov      ebp, dword ptr [ecx + 8]       
  0x0020CFD8  892c10                  mov      dword ptr [eax + edx], ebp     
  0x0020CFDB  8b690c                  mov      ebp, dword ptr [ecx + 0xc]     
  0x0020CFDE  896c1004                mov      dword ptr [eax + edx + 4], ebp 
  0x0020CFE2  8b6910                  mov      ebp, dword ptr [ecx + 0x10]    
  0x0020CFE5  03c2                    add      eax, edx                       
  0x0020CFE7  892c10                  mov      dword ptr [eax + edx], ebp     
  0x0020CFEA  8b6914                  mov      ebp, dword ptr [ecx + 0x14]    
  0x0020CFED  896c1004                mov      dword ptr [eax + edx + 4], ebp 
  0x0020CFF1  8b6918                  mov      ebp, dword ptr [ecx + 0x18]    
  0x0020CFF4  03c2                    add      eax, edx                       
  0x0020CFF6  892c10                  mov      dword ptr [eax + edx], ebp     
  0x0020CFF9  8b691c                  mov      ebp, dword ptr [ecx + 0x1c]    
  0x0020CFFC  896c1004                mov      dword ptr [eax + edx + 4], ebp 
  0x0020D000  8b6920                  mov      ebp, dword ptr [ecx + 0x20]    
  0x0020D003  03c2                    add      eax, edx                       
  0x0020D005  892c10                  mov      dword ptr [eax + edx], ebp     
  0x0020D008  8b6924                  mov      ebp, dword ptr [ecx + 0x24]    
  0x0020D00B  896c1004                mov      dword ptr [eax + edx + 4], ebp 
  0x0020D00F  8b6928                  mov      ebp, dword ptr [ecx + 0x28]    
  0x0020D012  03c2                    add      eax, edx                       
  0x0020D014  892c10                  mov      dword ptr [eax + edx], ebp     
  0x0020D017  8b692c                  mov      ebp, dword ptr [ecx + 0x2c]    
  0x0020D01A  03c2                    add      eax, edx                       
  0x0020D01C  896804                  mov      dword ptr [eax + 4], ebp       
  0x0020D01F  8b6930                  mov      ebp, dword ptr [ecx + 0x30]    
  0x0020D022  03c2                    add      eax, edx                       
  0x0020D024  8928                    mov      dword ptr [eax], ebp           
  0x0020D026  8b6934                  mov      ebp, dword ptr [ecx + 0x34]    
  0x0020D029  896804                  mov      dword ptr [eax + 4], ebp       
  0x0020D02C  8b6938                  mov      ebp, dword ptr [ecx + 0x38]    
  0x0020D02F  892c02                  mov      dword ptr [edx + eax], ebp     
  0x0020D032  8b693c                  mov      ebp, dword ptr [ecx + 0x3c]    
  0x0020D035  81c680000000            add      esi, 0x80                      
  0x0020D03B  896c0204                mov      dword ptr [edx + eax + 4], ebp 
  0x0020D03F  83c140                  add      ecx, 0x40                      
  0x0020D042  e983020000              jmp      0x20d2ca                       
; end of function
                                        ; XREF: 0x0020CFC9 (cond_jump)
  0x0020D047  c744241402000000        mov      dword ptr [esp + 0x14], 2      
  0x0020D04F  90                      nop                                     
                                        ; XREF: 0x0020D2C0 (cond_jump)
  0x0020D050  0fb611                  movzx    edx, byte ptr [ecx]            
  0x0020D053  0fbf1e                  movsx    ebx, word ptr [esi]            
  0x0020D056  8bef                    mov      ebp, edi                       
  0x0020D058  03ea                    add      ebp, edx                       
  0x0020D05A  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D05D  8810                    mov      byte ptr [eax], dl             
  0x0020D05F  0fb65101                movzx    edx, byte ptr [ecx + 1]        
  0x0020D063  0fbf5e02                movsx    ebx, word ptr [esi + 2]        
  0x0020D067  8bef                    mov      ebp, edi                       
  0x0020D069  03ea                    add      ebp, edx                       
  0x0020D06B  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D06E  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D071  0fb65102                movzx    edx, byte ptr [ecx + 2]        
  0x0020D075  0fbf5e04                movsx    ebx, word ptr [esi + 4]        
  0x0020D079  8bef                    mov      ebp, edi                       
  0x0020D07B  03ea                    add      ebp, edx                       
  0x0020D07D  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D080  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D083  0fb65103                movzx    edx, byte ptr [ecx + 3]        
  0x0020D087  0fbf5e06                movsx    ebx, word ptr [esi + 6]        
  0x0020D08B  8bef                    mov      ebp, edi                       
  0x0020D08D  03ea                    add      ebp, edx                       
  0x0020D08F  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D092  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D095  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020D099  0fbf5e08                movsx    ebx, word ptr [esi + 8]        
  0x0020D09D  8bef                    mov      ebp, edi                       
  0x0020D09F  03ea                    add      ebp, edx                       
  0x0020D0A1  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D0A4  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D0A7  0fb65105                movzx    edx, byte ptr [ecx + 5]        
  0x0020D0AB  0fbf5e0a                movsx    ebx, word ptr [esi + 0xa]      
  0x0020D0AF  8bef                    mov      ebp, edi                       
  0x0020D0B1  03ea                    add      ebp, edx                       
  0x0020D0B3  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D0B6  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D0B9  0fb65106                movzx    edx, byte ptr [ecx + 6]        
  0x0020D0BD  0fbf5e0c                movsx    ebx, word ptr [esi + 0xc]      
  0x0020D0C1  8bef                    mov      ebp, edi                       
  0x0020D0C3  03ea                    add      ebp, edx                       
  0x0020D0C5  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D0C8  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D0CB  0fb65107                movzx    edx, byte ptr [ecx + 7]        
  0x0020D0CF  0fbf5e0e                movsx    ebx, word ptr [esi + 0xe]      
  0x0020D0D3  8bef                    mov      ebp, edi                       
  0x0020D0D5  03ea                    add      ebp, edx                       
  0x0020D0D7  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D0DA  8b5c2410                mov      ebx, dword ptr [esp + 0x10]    
  0x0020D0DE  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D0E1  0fb65108                movzx    edx, byte ptr [ecx + 8]        
  0x0020D0E5  03c3                    add      eax, ebx                       
  0x0020D0E7  0fbf5e10                movsx    ebx, word ptr [esi + 0x10]     
  0x0020D0EB  83c108                  add      ecx, 8                         
  0x0020D0EE  83c610                  add      esi, 0x10                      
  0x0020D0F1  8bef                    mov      ebp, edi                       
  0x0020D0F3  03ea                    add      ebp, edx                       
  0x0020D0F5  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D0F8  8810                    mov      byte ptr [eax], dl             
  0x0020D0FA  0fb65101                movzx    edx, byte ptr [ecx + 1]        
  0x0020D0FE  0fbf5e02                movsx    ebx, word ptr [esi + 2]        
  0x0020D102  8bef                    mov      ebp, edi                       
  0x0020D104  03ea                    add      ebp, edx                       
  0x0020D106  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D109  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D10C  0fb65102                movzx    edx, byte ptr [ecx + 2]        
  0x0020D110  0fbf5e04                movsx    ebx, word ptr [esi + 4]        
  0x0020D114  8bef                    mov      ebp, edi                       
  0x0020D116  03ea                    add      ebp, edx                       
  0x0020D118  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D11B  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D11E  0fb65103                movzx    edx, byte ptr [ecx + 3]        
  0x0020D122  0fbf5e06                movsx    ebx, word ptr [esi + 6]        
  0x0020D126  8bef                    mov      ebp, edi                       
  0x0020D128  03ea                    add      ebp, edx                       
  0x0020D12A  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D12D  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D130  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020D134  0fbf5e08                movsx    ebx, word ptr [esi + 8]        
  0x0020D138  8bef                    mov      ebp, edi                       
  0x0020D13A  03ea                    add      ebp, edx                       
  0x0020D13C  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D13F  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D142  0fb65105                movzx    edx, byte ptr [ecx + 5]        
  0x0020D146  0fbf5e0a                movsx    ebx, word ptr [esi + 0xa]      
  0x0020D14A  8bef                    mov      ebp, edi                       
  0x0020D14C  03ea                    add      ebp, edx                       
  0x0020D14E  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D151  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D154  0fb65106                movzx    edx, byte ptr [ecx + 6]        
  0x0020D158  0fbf5e0c                movsx    ebx, word ptr [esi + 0xc]      
  0x0020D15C  8bef                    mov      ebp, edi                       
  0x0020D15E  03ea                    add      ebp, edx                       
  0x0020D160  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D163  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D166  0fb65107                movzx    edx, byte ptr [ecx + 7]        
  0x0020D16A  0fbf5e0e                movsx    ebx, word ptr [esi + 0xe]      
  0x0020D16E  83c108                  add      ecx, 8                         
  0x0020D171  83c610                  add      esi, 0x10                      
  0x0020D174  8bef                    mov      ebp, edi                       
  0x0020D176  03ea                    add      ebp, edx                       
  0x0020D178  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D17B  8b5c2410                mov      ebx, dword ptr [esp + 0x10]    
  0x0020D17F  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D182  0fb611                  movzx    edx, byte ptr [ecx]            
  0x0020D185  03c3                    add      eax, ebx                       
  0x0020D187  0fbf1e                  movsx    ebx, word ptr [esi]            
  0x0020D18A  8bef                    mov      ebp, edi                       
  0x0020D18C  03ea                    add      ebp, edx                       
  0x0020D18E  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D191  8810                    mov      byte ptr [eax], dl             
  0x0020D193  0fb65101                movzx    edx, byte ptr [ecx + 1]        
  0x0020D197  0fbf5e02                movsx    ebx, word ptr [esi + 2]        
  0x0020D19B  8bef                    mov      ebp, edi                       
  0x0020D19D  03ea                    add      ebp, edx                       
  0x0020D19F  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D1A2  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D1A5  0fb65102                movzx    edx, byte ptr [ecx + 2]        
  0x0020D1A9  0fbf5e04                movsx    ebx, word ptr [esi + 4]        
  0x0020D1AD  8bef                    mov      ebp, edi                       
  0x0020D1AF  03ea                    add      ebp, edx                       
  0x0020D1B1  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D1B4  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D1B7  0fb65103                movzx    edx, byte ptr [ecx + 3]        
  0x0020D1BB  0fbf5e06                movsx    ebx, word ptr [esi + 6]        
  0x0020D1BF  8bef                    mov      ebp, edi                       
  0x0020D1C1  03ea                    add      ebp, edx                       
  0x0020D1C3  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D1C6  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D1C9  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020D1CD  0fbf5e08                movsx    ebx, word ptr [esi + 8]        
  0x0020D1D1  8bef                    mov      ebp, edi                       
  0x0020D1D3  03ea                    add      ebp, edx                       
  0x0020D1D5  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D1D8  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D1DB  0fb65105                movzx    edx, byte ptr [ecx + 5]        
  0x0020D1DF  0fbf5e0a                movsx    ebx, word ptr [esi + 0xa]      
  0x0020D1E3  8bef                    mov      ebp, edi                       
  0x0020D1E5  03ea                    add      ebp, edx                       
  0x0020D1E7  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D1EA  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D1ED  0fb65106                movzx    edx, byte ptr [ecx + 6]        
  0x0020D1F1  0fbf5e0c                movsx    ebx, word ptr [esi + 0xc]      
  0x0020D1F5  8bef                    mov      ebp, edi                       
  0x0020D1F7  03ea                    add      ebp, edx                       
  0x0020D1F9  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D1FC  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D1FF  0fb65107                movzx    edx, byte ptr [ecx + 7]        
  0x0020D203  0fbf5e0e                movsx    ebx, word ptr [esi + 0xe]      
  0x0020D207  8bef                    mov      ebp, edi                       
  0x0020D209  03ea                    add      ebp, edx                       
  0x0020D20B  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D20E  8b5c2410                mov      ebx, dword ptr [esp + 0x10]    
  0x0020D212  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D215  0fb65108                movzx    edx, byte ptr [ecx + 8]        
  0x0020D219  83c108                  add      ecx, 8                         
  0x0020D21C  83c610                  add      esi, 0x10                      
  0x0020D21F  03c3                    add      eax, ebx                       
  0x0020D221  0fbf1e                  movsx    ebx, word ptr [esi]            
  0x0020D224  8bef                    mov      ebp, edi                       
  0x0020D226  03ea                    add      ebp, edx                       
  0x0020D228  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D22B  8810                    mov      byte ptr [eax], dl             
  0x0020D22D  0fb65101                movzx    edx, byte ptr [ecx + 1]        
  0x0020D231  0fbf5e02                movsx    ebx, word ptr [esi + 2]        
  0x0020D235  8bef                    mov      ebp, edi                       
  0x0020D237  03ea                    add      ebp, edx                       
  0x0020D239  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D23C  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D23F  0fb65102                movzx    edx, byte ptr [ecx + 2]        
  0x0020D243  0fbf5e04                movsx    ebx, word ptr [esi + 4]        
  0x0020D247  8bef                    mov      ebp, edi                       
  0x0020D249  03ea                    add      ebp, edx                       
  0x0020D24B  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D24E  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D251  0fb65103                movzx    edx, byte ptr [ecx + 3]        
  0x0020D255  0fbf5e06                movsx    ebx, word ptr [esi + 6]        
  0x0020D259  8bef                    mov      ebp, edi                       
  0x0020D25B  03ea                    add      ebp, edx                       
  0x0020D25D  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D260  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D263  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020D267  0fbf5e08                movsx    ebx, word ptr [esi + 8]        
  0x0020D26B  8bef                    mov      ebp, edi                       
  0x0020D26D  03ea                    add      ebp, edx                       
  0x0020D26F  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D272  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D275  0fb65105                movzx    edx, byte ptr [ecx + 5]        
  0x0020D279  0fbf5e0a                movsx    ebx, word ptr [esi + 0xa]      
  0x0020D27D  8bef                    mov      ebp, edi                       
  0x0020D27F  03ea                    add      ebp, edx                       
  0x0020D281  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D284  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D287  0fb65106                movzx    edx, byte ptr [ecx + 6]        
  0x0020D28B  0fbf5e0c                movsx    ebx, word ptr [esi + 0xc]      
  0x0020D28F  8bef                    mov      ebp, edi                       
  0x0020D291  03ea                    add      ebp, edx                       
  0x0020D293  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D296  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D299  0fb65107                movzx    edx, byte ptr [ecx + 7]        
  0x0020D29D  0fbf5e0e                movsx    ebx, word ptr [esi + 0xe]      
  0x0020D2A1  8bef                    mov      ebp, edi                       
  0x0020D2A3  03ea                    add      ebp, edx                       
  0x0020D2A5  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D2A8  8b5c2410                mov      ebx, dword ptr [esp + 0x10]    
  0x0020D2AC  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D2AF  8b542414                mov      edx, dword ptr [esp + 0x14]    
  0x0020D2B3  83c108                  add      ecx, 8                         
  0x0020D2B6  83c610                  add      esi, 0x10                      
  0x0020D2B9  03c3                    add      eax, ebx                       
  0x0020D2BB  4a                      dec      edx                            
  0x0020D2BC  89542414                mov      dword ptr [esp + 0x14], edx    
  0x0020D2C0  0f858afdffff            jne      0x20d050                       
  0x0020D2C6  8b5c241c                mov      ebx, dword ptr [esp + 0x1c]    
                                        ; XREF: 0x0020D042 (jump)
  0x0020D2CA  8b542424                mov      edx, dword ptr [esp + 0x24]    
  0x0020D2CE  8b442418                mov      eax, dword ptr [esp + 0x18]    
  0x0020D2D2  d1e2                    shl      edx, 1                         
  0x0020D2D4  48                      dec      eax                            
  0x0020D2D5  89542424                mov      dword ptr [esp + 0x24], edx    
  0x0020D2D9  89442418                mov      dword ptr [esp + 0x18], eax    
  0x0020D2DD  0f85cdfcffff            jne      0x20cfb0                       
  0x0020D2E3  5f                      pop      edi                            
  0x0020D2E4  5e                      pop      esi                            
  0x0020D2E5  5d                      pop      ebp                            
  0x0020D2E6  5b                      pop      ebx                            
  0x0020D2E7  83c410                  add      esp, 0x10                      
  0x0020D2EA  c3                      ret                                     
  0x0020D2EB  90                      nop                                     
  0x0020D2EC  90                      nop                                     
  0x0020D2ED  90                      nop                                     
  0x0020D2EE  90                      nop                                     
  0x0020D2EF  90                      nop                                     

; ============================================================
; Function: sub_0020D2F0
; Start: 0x0020D2F0  End: 0x0020D594  Size: 676 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020D2F0:
  0x0020D2F0  83ec14                  sub      esp, 0x14                      
  0x0020D2F3  8b4808                  mov      ecx, dword ptr [eax + 8]       
  0x0020D2F6  53                      push     ebx                            
  0x0020D2F7  55                      push     ebp                            
  0x0020D2F8  56                      push     esi                            
  0x0020D2F9  8b700c                  mov      esi, dword ptr [eax + 0xc]     
  0x0020D2FC  57                      push     edi                            
  0x0020D2FD  8b7804                  mov      edi, dword ptr [eax + 4]       
  0x0020D300  8b00                    mov      eax, dword ptr [eax]           
  0x0020D302  83c204                  add      edx, 4                         
  0x0020D305  89442410                mov      dword ptr [esp + 0x10], eax    
  0x0020D309  c744242006000000        mov      dword ptr [esp + 0x20], 6      
                                        ; XREF: 0x0020D9B9 (cond_jump)
  0x0020D311  8b02                    mov      eax, dword ptr [edx]           
  0x0020D313  8b6a04                  mov      ebp, dword ptr [edx + 4]       
  0x0020D316  83c204                  add      edx, 4                         
  0x0020D319  83c204                  add      edx, 4                         
  0x0020D31C  8954241c                mov      dword ptr [esp + 0x1c], edx    
  0x0020D320  8b542428                mov      edx, dword ptr [esp + 0x28]    
  0x0020D324  85d2                    test     edx, edx                       
  0x0020D326  bb02000000              mov      ebx, 2                         
  0x0020D32B  896c2414                mov      dword ptr [esp + 0x14], ebp    
  0x0020D32F  0f8c5f020000            jl       0x20d594                       
  0x0020D335  81c780000000            add      edi, 0x80                      
  0x0020D33B  895c2414                mov      dword ptr [esp + 0x14], ebx    
  0x0020D33F  90                      nop                                     
                                        ; XREF: 0x0020D589 (cond_jump)
  0x0020D340  0fb619                  movzx    ebx, byte ptr [ecx]            
  0x0020D343  0fb616                  movzx    edx, byte ptr [esi]            
  0x0020D346  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D34A  d1fa                    sar      edx, 1                         
  0x0020D34C  8810                    mov      byte ptr [eax], dl             
  0x0020D34E  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020D352  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0020D356  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D35A  d1fa                    sar      edx, 1                         
  0x0020D35C  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D35F  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020D363  0fb65602                movzx    edx, byte ptr [esi + 2]        
  0x0020D367  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D36B  d1fa                    sar      edx, 1                         
  0x0020D36D  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D370  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D374  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D378  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D37C  d1fa                    sar      edx, 1                         
  0x0020D37E  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D381  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D385  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D389  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D38D  d1fa                    sar      edx, 1                         
  0x0020D38F  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D392  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D396  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D39A  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D39E  d1fa                    sar      edx, 1                         
  0x0020D3A0  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D3A3  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D3A7  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D3AB  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D3AF  d1fa                    sar      edx, 1                         
  0x0020D3B1  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D3B4  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D3B8  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D3BC  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D3C0  d1fa                    sar      edx, 1                         
  0x0020D3C2  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D3C5  0fb65908                movzx    ebx, byte ptr [ecx + 8]        
  0x0020D3C9  0fb65608                movzx    edx, byte ptr [esi + 8]        
  0x0020D3CD  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D3D1  d1fa                    sar      edx, 1                         
  0x0020D3D3  881428                  mov      byte ptr [eax + ebp], dl       
  0x0020D3D6  0fb65909                movzx    ebx, byte ptr [ecx + 9]        
  0x0020D3DA  0fb65609                movzx    edx, byte ptr [esi + 9]        
  0x0020D3DE  83c108                  add      ecx, 8                         
  0x0020D3E1  83c608                  add      esi, 8                         
  0x0020D3E4  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D3E8  d1fa                    sar      edx, 1                         
  0x0020D3EA  88542801                mov      byte ptr [eax + ebp + 1], dl   
  0x0020D3EE  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020D3F2  0fb65602                movzx    edx, byte ptr [esi + 2]        
  0x0020D3F6  03c5                    add      eax, ebp                       
  0x0020D3F8  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D3FC  d1fa                    sar      edx, 1                         
  0x0020D3FE  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D401  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D405  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D409  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D40D  d1fa                    sar      edx, 1                         
  0x0020D40F  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D412  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D416  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D41A  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D41E  d1fa                    sar      edx, 1                         
  0x0020D420  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D423  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D427  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D42B  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D42F  d1fa                    sar      edx, 1                         
  0x0020D431  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D434  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D438  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D43C  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D440  d1fa                    sar      edx, 1                         
  0x0020D442  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D445  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D449  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D44D  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D451  d1fa                    sar      edx, 1                         
  0x0020D453  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D456  0fb65908                movzx    ebx, byte ptr [ecx + 8]        
  0x0020D45A  0fb65608                movzx    edx, byte ptr [esi + 8]        
  0x0020D45E  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D462  d1fa                    sar      edx, 1                         
  0x0020D464  881428                  mov      byte ptr [eax + ebp], dl       
  0x0020D467  0fb65909                movzx    ebx, byte ptr [ecx + 9]        
  0x0020D46B  0fb65609                movzx    edx, byte ptr [esi + 9]        
  0x0020D46F  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D473  d1fa                    sar      edx, 1                         
  0x0020D475  88542801                mov      byte ptr [eax + ebp + 1], dl   
  0x0020D479  0fb6590a                movzx    ebx, byte ptr [ecx + 0xa]      
  0x0020D47D  0fb6560a                movzx    edx, byte ptr [esi + 0xa]      
  0x0020D481  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D485  83c108                  add      ecx, 8                         
  0x0020D488  83c608                  add      esi, 8                         
  0x0020D48B  d1fa                    sar      edx, 1                         
  0x0020D48D  88542802                mov      byte ptr [eax + ebp + 2], dl   
  0x0020D491  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D495  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D499  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D49D  03c5                    add      eax, ebp                       
  0x0020D49F  d1fa                    sar      edx, 1                         
  0x0020D4A1  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D4A4  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D4A8  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D4AC  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D4B0  d1fa                    sar      edx, 1                         
  0x0020D4B2  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D4B5  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D4B9  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D4BD  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D4C1  d1fa                    sar      edx, 1                         
  0x0020D4C3  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D4C6  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D4CA  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D4CE  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D4D2  d1fa                    sar      edx, 1                         
  0x0020D4D4  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D4D7  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D4DB  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D4DF  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D4E3  d1fa                    sar      edx, 1                         
  0x0020D4E5  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D4E8  0fb65908                movzx    ebx, byte ptr [ecx + 8]        
  0x0020D4EC  0fb65608                movzx    edx, byte ptr [esi + 8]        
  0x0020D4F0  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D4F4  d1fa                    sar      edx, 1                         
  0x0020D4F6  881428                  mov      byte ptr [eax + ebp], dl       
  0x0020D4F9  0fb65909                movzx    ebx, byte ptr [ecx + 9]        
  0x0020D4FD  0fb65609                movzx    edx, byte ptr [esi + 9]        
  0x0020D501  83c108                  add      ecx, 8                         
  0x0020D504  83c608                  add      esi, 8                         
  0x0020D507  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D50B  03c5                    add      eax, ebp                       
  0x0020D50D  d1fa                    sar      edx, 1                         
  0x0020D50F  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D512  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020D516  0fb65602                movzx    edx, byte ptr [esi + 2]        
  0x0020D51A  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D51E  d1fa                    sar      edx, 1                         
  0x0020D520  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D523  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D527  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D52B  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D52F  d1fa                    sar      edx, 1                         
  0x0020D531  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D534  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D538  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D53C  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D540  d1fa                    sar      edx, 1                         
  0x0020D542  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D545  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D549  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D54D  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D551  d1fa                    sar      edx, 1                         
  0x0020D553  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D556  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D55A  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D55E  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D562  d1fa                    sar      edx, 1                         
  0x0020D564  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D567  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D56B  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D56F  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D573  d1fa                    sar      edx, 1                         
  0x0020D575  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D578  8b542414                mov      edx, dword ptr [esp + 0x14]    
  0x0020D57C  83c108                  add      ecx, 8                         
  0x0020D57F  83c608                  add      esi, 8                         
  0x0020D582  03c5                    add      eax, ebp                       
  0x0020D584  4a                      dec      edx                            
  0x0020D585  89542414                mov      dword ptr [esp + 0x14], edx    
  0x0020D589  0f85b1fdffff            jne      0x20d340                       
  0x0020D58F  e90e040000              jmp      0x20d9a2                       
; end of function
                                        ; XREF: 0x0020D32F (cond_jump)
  0x0020D594  895c2418                mov      dword ptr [esp + 0x18], ebx    
  0x0020D598  eb06                    jmp      0x20d5a0                       
  0x0020D59A  8d9b00000000            lea      ebx, [ebx]                     
                                        ; XREF: 0x0020D598 (jump), 0x0020D99C (cond_jump)
  0x0020D5A0  0fb619                  movzx    ebx, byte ptr [ecx]            
  0x0020D5A3  0fb616                  movzx    edx, byte ptr [esi]            
  0x0020D5A6  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D5AA  0fbf1f                  movsx    ebx, word ptr [edi]            
  0x0020D5AD  d1fa                    sar      edx, 1                         
  0x0020D5AF  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D5B3  03ea                    add      ebp, edx                       
  0x0020D5B5  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D5B8  8810                    mov      byte ptr [eax], dl             
  0x0020D5BA  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020D5BE  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0020D5C2  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D5C6  0fbf5f02                movsx    ebx, word ptr [edi + 2]        
  0x0020D5CA  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D5CE  d1fa                    sar      edx, 1                         
  0x0020D5D0  03ea                    add      ebp, edx                       
  0x0020D5D2  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D5D5  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D5D8  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020D5DC  0fb65602                movzx    edx, byte ptr [esi + 2]        
  0x0020D5E0  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D5E4  0fbf5f04                movsx    ebx, word ptr [edi + 4]        
  0x0020D5E8  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D5EC  d1fa                    sar      edx, 1                         
  0x0020D5EE  03ea                    add      ebp, edx                       
  0x0020D5F0  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D5F3  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D5F6  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D5FA  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D5FE  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D602  0fbf5f06                movsx    ebx, word ptr [edi + 6]        
  0x0020D606  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D60A  d1fa                    sar      edx, 1                         
  0x0020D60C  03ea                    add      ebp, edx                       
  0x0020D60E  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D611  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D614  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D618  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D61C  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D620  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D624  0fbf5f08                movsx    ebx, word ptr [edi + 8]        
  0x0020D628  d1fa                    sar      edx, 1                         
  0x0020D62A  03ea                    add      ebp, edx                       
  0x0020D62C  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D62F  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D633  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D636  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D63A  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D63E  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D642  0fbf5f0a                movsx    ebx, word ptr [edi + 0xa]      
  0x0020D646  d1fa                    sar      edx, 1                         
  0x0020D648  03ea                    add      ebp, edx                       
  0x0020D64A  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D64D  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D651  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D654  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D658  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D65C  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D660  0fbf5f0c                movsx    ebx, word ptr [edi + 0xc]      
  0x0020D664  d1fa                    sar      edx, 1                         
  0x0020D666  03ea                    add      ebp, edx                       
  0x0020D668  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D66B  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D66F  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D672  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D676  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D67A  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D67E  0fbf5f0e                movsx    ebx, word ptr [edi + 0xe]      
  0x0020D682  d1fa                    sar      edx, 1                         
  0x0020D684  03ea                    add      ebp, edx                       
  0x0020D686  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D689  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D68C  8b542414                mov      edx, dword ptr [esp + 0x14]    
  0x0020D690  0fb65908                movzx    ebx, byte ptr [ecx + 8]        
  0x0020D694  83c108                  add      ecx, 8                         
  0x0020D697  83c608                  add      esi, 8                         
  0x0020D69A  03c2                    add      eax, edx                       
  0x0020D69C  0fb616                  movzx    edx, byte ptr [esi]            
  0x0020D69F  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D6A3  83c710                  add      edi, 0x10                      
  0x0020D6A6  d1fa                    sar      edx, 1                         
  0x0020D6A8  0fbf1f                  movsx    ebx, word ptr [edi]            
  0x0020D6AB  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D6AF  03ea                    add      ebp, edx                       
  0x0020D6B1  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D6B4  8810                    mov      byte ptr [eax], dl             
  0x0020D6B6  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020D6BA  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0020D6BE  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D6C2  0fbf5f02                movsx    ebx, word ptr [edi + 2]        
  0x0020D6C6  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D6CA  d1fa                    sar      edx, 1                         
  0x0020D6CC  03ea                    add      ebp, edx                       
  0x0020D6CE  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D6D1  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D6D4  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020D6D8  0fb65602                movzx    edx, byte ptr [esi + 2]        
  0x0020D6DC  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D6E0  0fbf5f04                movsx    ebx, word ptr [edi + 4]        
  0x0020D6E4  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D6E8  d1fa                    sar      edx, 1                         
  0x0020D6EA  03ea                    add      ebp, edx                       
  0x0020D6EC  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D6EF  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D6F2  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D6F6  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D6FA  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D6FE  0fbf5f06                movsx    ebx, word ptr [edi + 6]        
  0x0020D702  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D706  d1fa                    sar      edx, 1                         
  0x0020D708  03ea                    add      ebp, edx                       
  0x0020D70A  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D70D  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D710  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D714  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D718  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D71C  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D720  0fbf5f08                movsx    ebx, word ptr [edi + 8]        
  0x0020D724  d1fa                    sar      edx, 1                         
  0x0020D726  03ea                    add      ebp, edx                       
  0x0020D728  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D72B  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D72E  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D732  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D736  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D73A  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D73E  0fbf5f0a                movsx    ebx, word ptr [edi + 0xa]      
  0x0020D742  d1fa                    sar      edx, 1                         
  0x0020D744  03ea                    add      ebp, edx                       
  0x0020D746  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D749  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D74D  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D750  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D754  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D758  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D75C  0fbf5f0c                movsx    ebx, word ptr [edi + 0xc]      
  0x0020D760  d1fa                    sar      edx, 1                         
  0x0020D762  03ea                    add      ebp, edx                       
  0x0020D764  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D767  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D76B  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D76E  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D772  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D776  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D77A  0fbf5f0e                movsx    ebx, word ptr [edi + 0xe]      
  0x0020D77E  d1fa                    sar      edx, 1                         
  0x0020D780  03ea                    add      ebp, edx                       
  0x0020D782  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D785  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D789  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D78C  8b542414                mov      edx, dword ptr [esp + 0x14]    
  0x0020D790  0fb65908                movzx    ebx, byte ptr [ecx + 8]        
  0x0020D794  03c2                    add      eax, edx                       
  0x0020D796  0fb65608                movzx    edx, byte ptr [esi + 8]        
  0x0020D79A  83c108                  add      ecx, 8                         
  0x0020D79D  83c608                  add      esi, 8                         
  0x0020D7A0  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D7A4  0fbf5f10                movsx    ebx, word ptr [edi + 0x10]     
  0x0020D7A8  83c710                  add      edi, 0x10                      
  0x0020D7AB  d1fa                    sar      edx, 1                         
  0x0020D7AD  03ea                    add      ebp, edx                       
  0x0020D7AF  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D7B2  8810                    mov      byte ptr [eax], dl             
  0x0020D7B4  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020D7B8  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0020D7BC  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D7C0  0fbf5f02                movsx    ebx, word ptr [edi + 2]        
  0x0020D7C4  d1fa                    sar      edx, 1                         
  0x0020D7C6  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D7CA  03ea                    add      ebp, edx                       
  0x0020D7CC  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D7CF  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D7D2  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020D7D6  0fb65602                movzx    edx, byte ptr [esi + 2]        
  0x0020D7DA  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D7DE  0fbf5f04                movsx    ebx, word ptr [edi + 4]        
  0x0020D7E2  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D7E6  d1fa                    sar      edx, 1                         
  0x0020D7E8  03ea                    add      ebp, edx                       
  0x0020D7EA  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D7ED  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D7F0  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D7F4  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D7F8  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D7FC  0fbf5f06                movsx    ebx, word ptr [edi + 6]        
  0x0020D800  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D804  d1fa                    sar      edx, 1                         
  0x0020D806  03ea                    add      ebp, edx                       
  0x0020D808  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D80B  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D80E  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D812  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D816  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D81A  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D81E  0fbf5f08                movsx    ebx, word ptr [edi + 8]        
  0x0020D822  d1fa                    sar      edx, 1                         
  0x0020D824  03ea                    add      ebp, edx                       
  0x0020D826  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D829  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D82C  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D830  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D834  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D838  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D83C  0fbf5f0a                movsx    ebx, word ptr [edi + 0xa]      
  0x0020D840  d1fa                    sar      edx, 1                         
  0x0020D842  03ea                    add      ebp, edx                       
  0x0020D844  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D847  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D84B  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D84E  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D852  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D856  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D85A  0fbf5f0c                movsx    ebx, word ptr [edi + 0xc]      
  0x0020D85E  d1fa                    sar      edx, 1                         
  0x0020D860  03ea                    add      ebp, edx                       
  0x0020D862  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D865  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D869  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D86C  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D870  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D874  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D878  0fbf5f0e                movsx    ebx, word ptr [edi + 0xe]      
  0x0020D87C  d1fa                    sar      edx, 1                         
  0x0020D87E  03ea                    add      ebp, edx                       
  0x0020D880  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D883  8b6c2414                mov      ebp, dword ptr [esp + 0x14]    
  0x0020D887  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D88A  0fb65908                movzx    ebx, byte ptr [ecx + 8]        
  0x0020D88E  0fb65608                movzx    edx, byte ptr [esi + 8]        
  0x0020D892  83c108                  add      ecx, 8                         
  0x0020D895  83c608                  add      esi, 8                         
  0x0020D898  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D89C  0fbf5f10                movsx    ebx, word ptr [edi + 0x10]     
  0x0020D8A0  83c710                  add      edi, 0x10                      
  0x0020D8A3  03c5                    add      eax, ebp                       
  0x0020D8A5  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D8A9  d1fa                    sar      edx, 1                         
  0x0020D8AB  03ea                    add      ebp, edx                       
  0x0020D8AD  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D8B0  8810                    mov      byte ptr [eax], dl             
  0x0020D8B2  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0020D8B6  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020D8BA  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D8BE  0fbf5f02                movsx    ebx, word ptr [edi + 2]        
  0x0020D8C2  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D8C6  d1fa                    sar      edx, 1                         
  0x0020D8C8  03ea                    add      ebp, edx                       
  0x0020D8CA  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D8CD  885001                  mov      byte ptr [eax + 1], dl         
  0x0020D8D0  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020D8D4  0fb65602                movzx    edx, byte ptr [esi + 2]        
  0x0020D8D8  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D8DC  0fbf5f04                movsx    ebx, word ptr [edi + 4]        
  0x0020D8E0  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D8E4  d1fa                    sar      edx, 1                         
  0x0020D8E6  03ea                    add      ebp, edx                       
  0x0020D8E8  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D8EB  885002                  mov      byte ptr [eax + 2], dl         
  0x0020D8EE  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020D8F2  0fb65603                movzx    edx, byte ptr [esi + 3]        
  0x0020D8F6  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D8FA  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D8FE  0fbf5f06                movsx    ebx, word ptr [edi + 6]        
  0x0020D902  d1fa                    sar      edx, 1                         
  0x0020D904  03ea                    add      ebp, edx                       
  0x0020D906  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D909  885003                  mov      byte ptr [eax + 3], dl         
  0x0020D90C  0fb65904                movzx    ebx, byte ptr [ecx + 4]        
  0x0020D910  0fb65604                movzx    edx, byte ptr [esi + 4]        
  0x0020D914  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D918  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D91C  0fbf5f08                movsx    ebx, word ptr [edi + 8]        
  0x0020D920  d1fa                    sar      edx, 1                         
  0x0020D922  03ea                    add      ebp, edx                       
  0x0020D924  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D927  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D92B  885004                  mov      byte ptr [eax + 4], dl         
  0x0020D92E  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020D932  0fb65605                movzx    edx, byte ptr [esi + 5]        
  0x0020D936  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D93A  0fbf5f0a                movsx    ebx, word ptr [edi + 0xa]      
  0x0020D93E  d1fa                    sar      edx, 1                         
  0x0020D940  03ea                    add      ebp, edx                       
  0x0020D942  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D945  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D949  885005                  mov      byte ptr [eax + 5], dl         
  0x0020D94C  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020D950  0fb65606                movzx    edx, byte ptr [esi + 6]        
  0x0020D954  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D958  0fbf5f0c                movsx    ebx, word ptr [edi + 0xc]      
  0x0020D95C  d1fa                    sar      edx, 1                         
  0x0020D95E  03ea                    add      ebp, edx                       
  0x0020D960  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D963  8b6c2410                mov      ebp, dword ptr [esp + 0x10]    
  0x0020D967  885006                  mov      byte ptr [eax + 6], dl         
  0x0020D96A  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020D96E  0fb65607                movzx    edx, byte ptr [esi + 7]        
  0x0020D972  8d541a01                lea      edx, [edx + ebx + 1]           
  0x0020D976  0fbf5f0e                movsx    ebx, word ptr [edi + 0xe]      
  0x0020D97A  d1fa                    sar      edx, 1                         
  0x0020D97C  03ea                    add      ebp, edx                       
  0x0020D97E  8a142b                  mov      dl, byte ptr [ebx + ebp]       
  0x0020D981  8b5c2414                mov      ebx, dword ptr [esp + 0x14]    
  0x0020D985  885007                  mov      byte ptr [eax + 7], dl         
  0x0020D988  8b542418                mov      edx, dword ptr [esp + 0x18]    
  0x0020D98C  83c108                  add      ecx, 8                         
  0x0020D98F  83c608                  add      esi, 8                         
  0x0020D992  83c710                  add      edi, 0x10                      
  0x0020D995  03c3                    add      eax, ebx                       
  0x0020D997  4a                      dec      edx                            
  0x0020D998  89542418                mov      dword ptr [esp + 0x18], edx    
  0x0020D99C  0f85fefbffff            jne      0x20d5a0                       
                                        ; XREF: 0x0020D58F (jump)
  0x0020D9A2  8b5c2428                mov      ebx, dword ptr [esp + 0x28]    
  0x0020D9A6  8b442420                mov      eax, dword ptr [esp + 0x20]    
  0x0020D9AA  8b54241c                mov      edx, dword ptr [esp + 0x1c]    
  0x0020D9AE  d1e3                    shl      ebx, 1                         
  0x0020D9B0  48                      dec      eax                            
  0x0020D9B1  895c2428                mov      dword ptr [esp + 0x28], ebx    
  0x0020D9B5  89442420                mov      dword ptr [esp + 0x20], eax    
  0x0020D9B9  0f8552f9ffff            jne      0x20d311                       
  0x0020D9BF  5f                      pop      edi                            
  0x0020D9C0  5e                      pop      esi                            
  0x0020D9C1  5d                      pop      ebp                            
  0x0020D9C2  5b                      pop      ebx                            
  0x0020D9C3  83c414                  add      esp, 0x14                      
  0x0020D9C6  c3                      ret                                     
  0x0020D9C7  90                      nop                                     
  0x0020D9C8  90                      nop                                     
  0x0020D9C9  90                      nop                                     
  0x0020D9CA  90                      nop                                     
  0x0020D9CB  90                      nop                                     
  0x0020D9CC  90                      nop                                     
  0x0020D9CD  90                      nop                                     
  0x0020D9CE  90                      nop                                     
  0x0020D9CF  90                      nop                                     

; ============================================================
; Function: sub_0020D9D0
; Start: 0x0020D9D0  End: 0x0020DAEC  Size: 284 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_0020D9D0:
  0x0020D9D0  55                      push     ebp                            
  0x0020D9D1  8bec                    mov      ebp, esp                       
  0x0020D9D3  83e4f8                  and      esp, 0xfffffff8                
  0x0020D9D6  83ec08                  sub      esp, 8                         
  0x0020D9D9  0fbf430c                movsx    eax, word ptr [ebx + 0xc]      
  0x0020D9DD  8b4d08                  mov      ecx, dword ptr [ebp + 8]       
  0x0020D9E0  99                      cdq                                     
  0x0020D9E1  83e207                  and      edx, 7                         
  0x0020D9E4  03c2                    add      eax, edx                       
  0x0020D9E6  8b11                    mov      edx, dword ptr [ecx]           
  0x0020D9E8  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x0020D9EB  56                      push     esi                            
  0x0020D9EC  c1f803                  sar      eax, 3                         
  0x0020D9EF  57                      push     edi                            
  0x0020D9F0  8d3c00                  lea      edi, [eax + eax]               
  0x0020D9F3  c1e703                  shl      edi, 3                         
  0x0020D9F6  f6c21f                  test     dl, 0x1f                       
  0x0020D9F9  0f85ed000000            jne      0x20daec                       
  0x0020D9FF  8b31                    mov      esi, dword ptr [ecx]           
  0x0020DA01  dd0416                  fld      qword ptr [esi + edx]          
  0x0020DA04  8b0b                    mov      ecx, dword ptr [ebx]           
  0x0020DA06  0f180c11                prefetcht0 byte ptr [ecx + edx]           
  0x0020DA0A  03ca                    add      ecx, edx                       
  0x0020DA0C  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DA10  03f2                    add      esi, edx                       
  0x0020DA12  dd04c6                  fld      qword ptr [esi + eax*8]        
  0x0020DA15  03f7                    add      esi, edi                       
  0x0020DA17  d9c9                    fxch     st(1)                          
  0x0020DA19  dd19                    fstp     qword ptr [ecx]                
  0x0020DA1B  dd1cc1                  fstp     qword ptr [ecx + eax*8]        
  0x0020DA1E  0f180c39                prefetcht0 byte ptr [ecx + edi]           
  0x0020DA22  dd06                    fld      qword ptr [esi]                
  0x0020DA24  03cf                    add      ecx, edi                       
  0x0020DA26  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DA2A  dd04c6                  fld      qword ptr [esi + eax*8]        
  0x0020DA2D  03f7                    add      esi, edi                       
  0x0020DA2F  d9c9                    fxch     st(1)                          
  0x0020DA31  dd19                    fstp     qword ptr [ecx]                
  0x0020DA33  dd1cc1                  fstp     qword ptr [ecx + eax*8]        
  0x0020DA36  0f180c39                prefetcht0 byte ptr [ecx + edi]           
  0x0020DA3A  dd06                    fld      qword ptr [esi]                
  0x0020DA3C  03cf                    add      ecx, edi                       
  0x0020DA3E  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DA42  dd04c6                  fld      qword ptr [esi + eax*8]        
  0x0020DA45  d9c9                    fxch     st(1)                          
  0x0020DA47  03f7                    add      esi, edi                       
  0x0020DA49  dd19                    fstp     qword ptr [ecx]                
  0x0020DA4B  dd1cc1                  fstp     qword ptr [ecx + eax*8]        
  0x0020DA4E  0f180c39                prefetcht0 byte ptr [ecx + edi]           
  0x0020DA52  dd04c6                  fld      qword ptr [esi + eax*8]        
  0x0020DA55  03cf                    add      ecx, edi                       
  0x0020DA57  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DA5B  8d14c1                  lea      edx, [ecx + eax*8]             
  0x0020DA5E  8954240c                mov      dword ptr [esp + 0xc], edx     
  0x0020DA62  8b16                    mov      edx, dword ptr [esi]           
  0x0020DA64  8911                    mov      dword ptr [ecx], edx           
  0x0020DA66  8b5604                  mov      edx, dword ptr [esi + 4]       
  0x0020DA69  895104                  mov      dword ptr [ecx + 4], edx       
  0x0020DA6C  8b4c240c                mov      ecx, dword ptr [esp + 0xc]     
  0x0020DA70  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x0020DA73  dd19                    fstp     qword ptr [ecx]                
  0x0020DA75  8b32                    mov      esi, dword ptr [edx]           
  0x0020DA77  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x0020DA7A  8b5104                  mov      edx, dword ptr [ecx + 4]       
  0x0020DA7D  dd0432                  fld      qword ptr [edx + esi]          
  0x0020DA80  8b4b04                  mov      ecx, dword ptr [ebx + 4]       
  0x0020DA83  0f180c31                prefetcht0 byte ptr [ecx + esi]           
  0x0020DA87  03d6                    add      edx, esi                       
  0x0020DA89  dd04c2                  fld      qword ptr [edx + eax*8]        
  0x0020DA8C  03ce                    add      ecx, esi                       
  0x0020DA8E  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DA92  d9c9                    fxch     st(1)                          
  0x0020DA94  dd19                    fstp     qword ptr [ecx]                
  0x0020DA96  03d7                    add      edx, edi                       
  0x0020DA98  dd1cc1                  fstp     qword ptr [ecx + eax*8]        
  0x0020DA9B  0f180c39                prefetcht0 byte ptr [ecx + edi]           
  0x0020DA9F  dd02                    fld      qword ptr [edx]                
  0x0020DAA1  03cf                    add      ecx, edi                       
  0x0020DAA3  dd04c2                  fld      qword ptr [edx + eax*8]        
  0x0020DAA6  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DAAA  d9c9                    fxch     st(1)                          
  0x0020DAAC  03d7                    add      edx, edi                       
  0x0020DAAE  dd19                    fstp     qword ptr [ecx]                
  0x0020DAB0  dd1cc1                  fstp     qword ptr [ecx + eax*8]        
  0x0020DAB3  0f180c39                prefetcht0 byte ptr [ecx + edi]           
  0x0020DAB7  dd02                    fld      qword ptr [edx]                
  0x0020DAB9  03cf                    add      ecx, edi                       
  0x0020DABB  dd04c2                  fld      qword ptr [edx + eax*8]        
  0x0020DABE  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DAC2  d9c9                    fxch     st(1)                          
  0x0020DAC4  03d7                    add      edx, edi                       
  0x0020DAC6  dd19                    fstp     qword ptr [ecx]                
  0x0020DAC8  dd1cc1                  fstp     qword ptr [ecx + eax*8]        
  0x0020DACB  0f180c39                prefetcht0 byte ptr [ecx + edi]           
  0x0020DACF  dd04c2                  fld      qword ptr [edx + eax*8]        
  0x0020DAD2  03cf                    add      ecx, edi                       
  0x0020DAD4  0f180cc1                prefetcht0 byte ptr [ecx + eax*8]         
  0x0020DAD8  8d34c1                  lea      esi, [ecx + eax*8]             
  0x0020DADB  8b02                    mov      eax, dword ptr [edx]           
  0x0020DADD  8901                    mov      dword ptr [ecx], eax           
  0x0020DADF  8b5204                  mov      edx, dword ptr [edx + 4]       
  0x0020DAE2  895104                  mov      dword ptr [ecx + 4], edx       
  0x0020DAE5  dd1e                    fstp     qword ptr [esi]                
  0x0020DAE7  e9a1000000              jmp      0x20db8d                       
; end of function
                                        ; XREF: 0x0020D9F9 (cond_jump)
  0x0020DAEC  8b09                    mov      ecx, dword ptr [ecx]           
  0x0020DAEE  dd0411                  fld      qword ptr [ecx + edx]          
  0x0020DAF1  8b33                    mov      esi, dword ptr [ebx]           
  0x0020DAF3  03ca                    add      ecx, edx                       
  0x0020DAF5  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DAF8  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x0020DAFB  0332                    add      esi, dword ptr [edx]           
  0x0020DAFD  d9c9                    fxch     st(1)                          
  0x0020DAFF  03cf                    add      ecx, edi                       
  0x0020DB01  dd1e                    fstp     qword ptr [esi]                
  0x0020DB03  dd1cc6                  fstp     qword ptr [esi + eax*8]        
  0x0020DB06  03f7                    add      esi, edi                       
  0x0020DB08  dd01                    fld      qword ptr [ecx]                
  0x0020DB0A  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DB0D  03cf                    add      ecx, edi                       
  0x0020DB0F  d9c9                    fxch     st(1)                          
  0x0020DB11  dd1e                    fstp     qword ptr [esi]                
  0x0020DB13  dd1cc6                  fstp     qword ptr [esi + eax*8]        
  0x0020DB16  03f7                    add      esi, edi                       
  0x0020DB18  dd01                    fld      qword ptr [ecx]                
  0x0020DB1A  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DB1D  03cf                    add      ecx, edi                       
  0x0020DB1F  d9c9                    fxch     st(1)                          
  0x0020DB21  dd1e                    fstp     qword ptr [esi]                
  0x0020DB23  dd1cc6                  fstp     qword ptr [esi + eax*8]        
  0x0020DB26  8b11                    mov      edx, dword ptr [ecx]           
  0x0020DB28  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DB2B  89143e                  mov      dword ptr [esi + edi], edx     
  0x0020DB2E  8b4904                  mov      ecx, dword ptr [ecx + 4]       
  0x0020DB31  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x0020DB34  894c3e04                mov      dword ptr [esi + edi + 4], ecx 
  0x0020DB38  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x0020DB3B  03f7                    add      esi, edi                       
  0x0020DB3D  dd1cc6                  fstp     qword ptr [esi + eax*8]        
  0x0020DB40  8b32                    mov      esi, dword ptr [edx]           
  0x0020DB42  8b4904                  mov      ecx, dword ptr [ecx + 4]       
  0x0020DB45  dd0431                  fld      qword ptr [ecx + esi]          
  0x0020DB48  8b5304                  mov      edx, dword ptr [ebx + 4]       
  0x0020DB4B  03ce                    add      ecx, esi                       
  0x0020DB4D  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DB50  03d6                    add      edx, esi                       
  0x0020DB52  03cf                    add      ecx, edi                       
  0x0020DB54  d9c9                    fxch     st(1)                          
  0x0020DB56  dd1a                    fstp     qword ptr [edx]                
  0x0020DB58  dd1cc2                  fstp     qword ptr [edx + eax*8]        
  0x0020DB5B  03d7                    add      edx, edi                       
  0x0020DB5D  dd01                    fld      qword ptr [ecx]                
  0x0020DB5F  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DB62  03cf                    add      ecx, edi                       
  0x0020DB64  d9c9                    fxch     st(1)                          
  0x0020DB66  dd1a                    fstp     qword ptr [edx]                
  0x0020DB68  dd1cc2                  fstp     qword ptr [edx + eax*8]        
  0x0020DB6B  03d7                    add      edx, edi                       
  0x0020DB6D  dd01                    fld      qword ptr [ecx]                
  0x0020DB6F  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DB72  03cf                    add      ecx, edi                       
  0x0020DB74  d9c9                    fxch     st(1)                          
  0x0020DB76  dd1a                    fstp     qword ptr [edx]                
  0x0020DB78  dd1cc2                  fstp     qword ptr [edx + eax*8]        
  0x0020DB7B  8b31                    mov      esi, dword ptr [ecx]           
  0x0020DB7D  dd04c1                  fld      qword ptr [ecx + eax*8]        
  0x0020DB80  03d7                    add      edx, edi                       
  0x0020DB82  8932                    mov      dword ptr [edx], esi           
  0x0020DB84  8b4904                  mov      ecx, dword ptr [ecx + 4]       
  0x0020DB87  894a04                  mov      dword ptr [edx + 4], ecx       
  0x0020DB8A  dd1cc2                  fstp     qword ptr [edx + eax*8]        
                                        ; XREF: 0x0020DAE7 (jump)
  0x0020DB8D  0fbf430e                movsx    eax, word ptr [ebx + 0xe]      
  0x0020DB91  8b4d0c                  mov      ecx, dword ptr [ebp + 0xc]     
  0x0020DB94  99                      cdq                                     
  0x0020DB95  83e207                  and      edx, 7                         
  0x0020DB98  03c2                    add      eax, edx                       
  0x0020DB9A  8b5508                  mov      edx, dword ptr [ebp + 8]       
  0x0020DB9D  8bf0                    mov      esi, eax                       
  0x0020DB9F  8b4204                  mov      eax, dword ptr [edx + 4]       
  0x0020DBA2  c1fe03                  sar      esi, 3                         
  0x0020DBA5  a81f                    test     al, 0x1f                       
  0x0020DBA7  0f855b010000            jne      0x20dd08                       
  0x0020DBAD  8b5108                  mov      edx, dword ptr [ecx + 8]       
  0x0020DBB0  dd0402                  fld      qword ptr [edx + eax]          
  0x0020DBB3  8b4b08                  mov      ecx, dword ptr [ebx + 8]       
  0x0020DBB6  dd440208                fld      qword ptr [edx + eax + 8]      
  0x0020DBBA  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DBBE  03d0                    add      edx, eax                       
  0x0020DBC0  d9c9                    fxch     st(1)                          
  0x0020DBC2  dd1c01                  fstp     qword ptr [ecx + eax]          
  0x0020DBC5  03c8                    add      ecx, eax                       
  0x0020DBC7  8d04f500000000          lea      eax, [esi*8]                   
  0x0020DBCE  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DBD1  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DBD5  dd0402                  fld      qword ptr [edx + eax]          
  0x0020DBD8  03d0                    add      edx, eax                       
  0x0020DBDA  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DBDD  03c8                    add      ecx, eax                       
  0x0020DBDF  03d0                    add      edx, eax                       
  0x0020DBE1  d9c9                    fxch     st(1)                          
  0x0020DBE3  dd19                    fstp     qword ptr [ecx]                
  0x0020DBE5  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DBE8  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DBEC  dd02                    fld      qword ptr [edx]                
  0x0020DBEE  03c8                    add      ecx, eax                       
  0x0020DBF0  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DBF3  03d0                    add      edx, eax                       
  0x0020DBF5  d9c9                    fxch     st(1)                          
  0x0020DBF7  dd19                    fstp     qword ptr [ecx]                
  0x0020DBF9  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DBFC  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC00  dd02                    fld      qword ptr [edx]                
  0x0020DC02  03c8                    add      ecx, eax                       
  0x0020DC04  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC07  03d0                    add      edx, eax                       
  0x0020DC09  d9c9                    fxch     st(1)                          
  0x0020DC0B  dd19                    fstp     qword ptr [ecx]                
  0x0020DC0D  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC10  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC14  dd02                    fld      qword ptr [edx]                
  0x0020DC16  03c8                    add      ecx, eax                       
  0x0020DC18  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC1B  03d0                    add      edx, eax                       
  0x0020DC1D  d9c9                    fxch     st(1)                          
  0x0020DC1F  dd19                    fstp     qword ptr [ecx]                
  0x0020DC21  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC24  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC28  dd02                    fld      qword ptr [edx]                
  0x0020DC2A  03c8                    add      ecx, eax                       
  0x0020DC2C  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC2F  03d0                    add      edx, eax                       
  0x0020DC31  d9c9                    fxch     st(1)                          
  0x0020DC33  dd19                    fstp     qword ptr [ecx]                
  0x0020DC35  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC38  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC3C  dd02                    fld      qword ptr [edx]                
  0x0020DC3E  03c8                    add      ecx, eax                       
  0x0020DC40  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC43  03d0                    add      edx, eax                       
  0x0020DC45  d9c9                    fxch     st(1)                          
  0x0020DC47  dd19                    fstp     qword ptr [ecx]                
  0x0020DC49  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC4C  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC50  dd02                    fld      qword ptr [edx]                
  0x0020DC52  03c8                    add      ecx, eax                       
  0x0020DC54  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC57  03d0                    add      edx, eax                       
  0x0020DC59  d9c9                    fxch     st(1)                          
  0x0020DC5B  dd19                    fstp     qword ptr [ecx]                
  0x0020DC5D  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC60  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC64  dd02                    fld      qword ptr [edx]                
  0x0020DC66  03c8                    add      ecx, eax                       
  0x0020DC68  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC6B  03d0                    add      edx, eax                       
  0x0020DC6D  d9c9                    fxch     st(1)                          
  0x0020DC6F  dd19                    fstp     qword ptr [ecx]                
  0x0020DC71  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC74  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC78  dd02                    fld      qword ptr [edx]                
  0x0020DC7A  03c8                    add      ecx, eax                       
  0x0020DC7C  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC7F  03d0                    add      edx, eax                       
  0x0020DC81  d9c9                    fxch     st(1)                          
  0x0020DC83  dd19                    fstp     qword ptr [ecx]                
  0x0020DC85  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC88  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DC8C  dd02                    fld      qword ptr [edx]                
  0x0020DC8E  03c8                    add      ecx, eax                       
  0x0020DC90  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DC93  03d0                    add      edx, eax                       
  0x0020DC95  d9c9                    fxch     st(1)                          
  0x0020DC97  dd19                    fstp     qword ptr [ecx]                
  0x0020DC99  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DC9C  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DCA0  dd02                    fld      qword ptr [edx]                
  0x0020DCA2  03c8                    add      ecx, eax                       
  0x0020DCA4  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DCA7  03d0                    add      edx, eax                       
  0x0020DCA9  d9c9                    fxch     st(1)                          
  0x0020DCAB  dd19                    fstp     qword ptr [ecx]                
  0x0020DCAD  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DCB0  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DCB4  dd02                    fld      qword ptr [edx]                
  0x0020DCB6  03c8                    add      ecx, eax                       
  0x0020DCB8  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DCBB  03d0                    add      edx, eax                       
  0x0020DCBD  d9c9                    fxch     st(1)                          
  0x0020DCBF  dd19                    fstp     qword ptr [ecx]                
  0x0020DCC1  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DCC4  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DCC8  dd02                    fld      qword ptr [edx]                
  0x0020DCCA  03c8                    add      ecx, eax                       
  0x0020DCCC  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DCCF  03d0                    add      edx, eax                       
  0x0020DCD1  d9c9                    fxch     st(1)                          
  0x0020DCD3  dd19                    fstp     qword ptr [ecx]                
  0x0020DCD5  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DCD8  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DCDC  dd02                    fld      qword ptr [edx]                
  0x0020DCDE  03c8                    add      ecx, eax                       
  0x0020DCE0  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DCE3  03d0                    add      edx, eax                       
  0x0020DCE5  d9c9                    fxch     st(1)                          
  0x0020DCE7  dd19                    fstp     qword ptr [ecx]                
  0x0020DCE9  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DCEC  0f180c01                prefetcht0 byte ptr [ecx + eax]           
  0x0020DCF0  dd4208                  fld      qword ptr [edx + 8]            
  0x0020DCF3  03c8                    add      ecx, eax                       
  0x0020DCF5  8b02                    mov      eax, dword ptr [edx]           
  0x0020DCF7  8901                    mov      dword ptr [ecx], eax           
  0x0020DCF9  8b5204                  mov      edx, dword ptr [edx + 4]       
  0x0020DCFC  dd5908                  fstp     qword ptr [ecx + 8]            
  0x0020DCFF  895104                  mov      dword ptr [ecx + 4], edx       
  0x0020DD02  5f                      pop      edi                            
  0x0020DD03  5e                      pop      esi                            
  0x0020DD04  8be5                    mov      esp, ebp                       
  0x0020DD06  5d                      pop      ebp                            
  0x0020DD07  c3                      ret                                     
                                        ; XREF: 0x0020DBA7 (cond_jump)
  0x0020DD08  8b4908                  mov      ecx, dword ptr [ecx + 8]       
  0x0020DD0B  dd0401                  fld      qword ptr [ecx + eax]          
  0x0020DD0E  8b5308                  mov      edx, dword ptr [ebx + 8]       
  0x0020DD11  dd440108                fld      qword ptr [ecx + eax + 8]      
  0x0020DD15  03c8                    add      ecx, eax                       
  0x0020DD17  03d0                    add      edx, eax                       
  0x0020DD19  d9c9                    fxch     st(1)                          
  0x0020DD1B  dd1a                    fstp     qword ptr [edx]                
  0x0020DD1D  8d04f500000000          lea      eax, [esi*8]                   
  0x0020DD24  03c8                    add      ecx, eax                       
  0x0020DD26  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD29  03d0                    add      edx, eax                       
  0x0020DD2B  dd01                    fld      qword ptr [ecx]                
  0x0020DD2D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DD30  03c8                    add      ecx, eax                       
  0x0020DD32  d9c9                    fxch     st(1)                          
  0x0020DD34  dd1a                    fstp     qword ptr [edx]                
  0x0020DD36  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD39  03d0                    add      edx, eax                       
  0x0020DD3B  dd01                    fld      qword ptr [ecx]                
  0x0020DD3D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DD40  03c8                    add      ecx, eax                       
  0x0020DD42  d9c9                    fxch     st(1)                          
  0x0020DD44  dd1a                    fstp     qword ptr [edx]                
  0x0020DD46  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD49  03d0                    add      edx, eax                       
  0x0020DD4B  dd01                    fld      qword ptr [ecx]                
  0x0020DD4D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DD50  03c8                    add      ecx, eax                       
  0x0020DD52  d9c9                    fxch     st(1)                          
  0x0020DD54  dd1a                    fstp     qword ptr [edx]                
  0x0020DD56  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD59  03d0                    add      edx, eax                       
  0x0020DD5B  dd01                    fld      qword ptr [ecx]                
  0x0020DD5D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DD60  03c8                    add      ecx, eax                       
  0x0020DD62  d9c9                    fxch     st(1)                          
  0x0020DD64  dd1a                    fstp     qword ptr [edx]                
  0x0020DD66  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD69  03d0                    add      edx, eax                       
  0x0020DD6B  dd01                    fld      qword ptr [ecx]                
  0x0020DD6D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DD70  03c8                    add      ecx, eax                       
  0x0020DD72  d9c9                    fxch     st(1)                          
  0x0020DD74  dd1a                    fstp     qword ptr [edx]                
  0x0020DD76  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD79  03d0                    add      edx, eax                       
  0x0020DD7B  dd01                    fld      qword ptr [ecx]                
  0x0020DD7D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DD80  03c8                    add      ecx, eax                       
  0x0020DD82  d9c9                    fxch     st(1)                          
  0x0020DD84  dd1a                    fstp     qword ptr [edx]                
  0x0020DD86  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD89  03d0                    add      edx, eax                       
  0x0020DD8B  dd01                    fld      qword ptr [ecx]                
  0x0020DD8D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DD90  03c8                    add      ecx, eax                       
  0x0020DD92  d9c9                    fxch     st(1)                          
  0x0020DD94  dd1a                    fstp     qword ptr [edx]                
  0x0020DD96  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DD99  03d0                    add      edx, eax                       
  0x0020DD9B  dd01                    fld      qword ptr [ecx]                
  0x0020DD9D  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DDA0  03c8                    add      ecx, eax                       
  0x0020DDA2  d9c9                    fxch     st(1)                          
  0x0020DDA4  dd1a                    fstp     qword ptr [edx]                
  0x0020DDA6  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DDA9  03d0                    add      edx, eax                       
  0x0020DDAB  dd01                    fld      qword ptr [ecx]                
  0x0020DDAD  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DDB0  03c8                    add      ecx, eax                       
  0x0020DDB2  d9c9                    fxch     st(1)                          
  0x0020DDB4  dd1a                    fstp     qword ptr [edx]                
  0x0020DDB6  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DDB9  03d0                    add      edx, eax                       
  0x0020DDBB  dd01                    fld      qword ptr [ecx]                
  0x0020DDBD  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DDC0  03c8                    add      ecx, eax                       
  0x0020DDC2  d9c9                    fxch     st(1)                          
  0x0020DDC4  dd1a                    fstp     qword ptr [edx]                
  0x0020DDC6  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DDC9  dd01                    fld      qword ptr [ecx]                
  0x0020DDCB  03d0                    add      edx, eax                       
  0x0020DDCD  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DDD0  03c8                    add      ecx, eax                       
  0x0020DDD2  d9c9                    fxch     st(1)                          
  0x0020DDD4  5f                      pop      edi                            
  0x0020DDD5  dd1a                    fstp     qword ptr [edx]                
  0x0020DDD7  5e                      pop      esi                            
  0x0020DDD8  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DDDB  03d0                    add      edx, eax                       
  0x0020DDDD  dd01                    fld      qword ptr [ecx]                
  0x0020DDDF  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DDE2  03c8                    add      ecx, eax                       
  0x0020DDE4  d9c9                    fxch     st(1)                          
  0x0020DDE6  dd1a                    fstp     qword ptr [edx]                
  0x0020DDE8  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DDEB  03d0                    add      edx, eax                       
  0x0020DDED  dd01                    fld      qword ptr [ecx]                
  0x0020DDEF  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DDF2  03c8                    add      ecx, eax                       
  0x0020DDF4  d9c9                    fxch     st(1)                          
  0x0020DDF6  dd1a                    fstp     qword ptr [edx]                
  0x0020DDF8  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DDFB  03d0                    add      edx, eax                       
  0x0020DDFD  dd01                    fld      qword ptr [ecx]                
  0x0020DDFF  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DE02  03c8                    add      ecx, eax                       
  0x0020DE04  d9c9                    fxch     st(1)                          
  0x0020DE06  dd1a                    fstp     qword ptr [edx]                
  0x0020DE08  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DE0B  03d0                    add      edx, eax                       
  0x0020DE0D  8b01                    mov      eax, dword ptr [ecx]           
  0x0020DE0F  dd4108                  fld      qword ptr [ecx + 8]            
  0x0020DE12  8902                    mov      dword ptr [edx], eax           
  0x0020DE14  8b4904                  mov      ecx, dword ptr [ecx + 4]       
  0x0020DE17  dd5a08                  fstp     qword ptr [edx + 8]            
  0x0020DE1A  894a04                  mov      dword ptr [edx + 4], ecx       
  0x0020DE1D  8be5                    mov      esp, ebp                       
  0x0020DE1F  5d                      pop      ebp                            
  0x0020DE20  c3                      ret                                     
  0x0020DE21  90                      nop                                     
  0x0020DE22  90                      nop                                     
  0x0020DE23  90                      nop                                     
  0x0020DE24  90                      nop                                     
  0x0020DE25  90                      nop                                     
  0x0020DE26  90                      nop                                     
  0x0020DE27  90                      nop                                     
  0x0020DE28  90                      nop                                     
  0x0020DE29  90                      nop                                     
  0x0020DE2A  90                      nop                                     
  0x0020DE2B  90                      nop                                     
  0x0020DE2C  90                      nop                                     
  0x0020DE2D  90                      nop                                     
  0x0020DE2E  90                      nop                                     
  0x0020DE2F  90                      nop                                     
  0x0020DE30  8b542408                mov      edx, dword ptr [esp + 8]       
  0x0020DE34  53                      push     ebx                            
  0x0020DE35  55                      push     ebp                            
  0x0020DE36  56                      push     esi                            
  0x0020DE37  57                      push     edi                            
  0x0020DE38  8b7c2414                mov      edi, dword ptr [esp + 0x14]    
  0x0020DE3C  8b9f9c020000            mov      ebx, dword ptr [edi + 0x29c]   
  0x0020DE42  8baf40020000            mov      ebp, dword ptr [edi + 0x240]   
  0x0020DE48  8bf3                    mov      esi, ebx                       
  0x0020DE4A  2bf2                    sub      esi, edx                       
  0x0020DE4C  46                      inc      esi                            
  0x0020DE4D  3bf3                    cmp      esi, ebx                       
  0x0020DE4F  c787a802000000000000    mov      dword ptr [edi + 0x2a8], 0     
  0x0020DE59  7d16                    jge      0x20de71                       
  0x0020DE5B  eb03                    jmp      0x20de60                       
  0x0020DE5D  8d4900                  lea      ecx, [ecx]                     
                                        ; XREF: 0x0020DE5B (jump), 0x0020DE6F (cond_jump)
  0x0020DE60  57                      push     edi                            
  0x0020DE61  89b79c020000            mov      dword ptr [edi + 0x29c], esi   
  0x0020DE67  ffd5                    call     ebp                            
  0x0020DE69  83c404                  add      esp, 4                         
  0x0020DE6C  46                      inc      esi                            
  0x0020DE6D  3bf3                    cmp      esi, ebx                       
  0x0020DE6F  7cef                    jl       0x20de60                       
                                        ; XREF: 0x0020DE59 (cond_jump)
  0x0020DE71  899f9c020000            mov      dword ptr [edi + 0x29c], ebx   
  0x0020DE77  5f                      pop      edi                            
  0x0020DE78  5e                      pop      esi                            
  0x0020DE79  5d                      pop      ebp                            
  0x0020DE7A  5b                      pop      ebx                            
  0x0020DE7B  c3                      ret                                     
  0x0020DE7C  90                      nop                                     
  0x0020DE7D  90                      nop                                     
  0x0020DE7E  90                      nop                                     
  0x0020DE7F  90                      nop                                     
  0x0020DE80  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0020DE84  8b889c020000            mov      ecx, dword ptr [eax + 0x29c]   
  0x0020DE8A  8d8c48680c0000          lea      ecx, [eax + ecx*2 + 0xc68]     
  0x0020DE91  53                      push     ebx                            
  0x0020DE92  0fbf980e020000          movsx    ebx, word ptr [eax + 0x20e]    
  0x0020DE99  56                      push     esi                            
  0x0020DE9A  0fbe31                  movsx    esi, byte ptr [ecx]            
  0x0020DE9D  57                      push     edi                            
  0x0020DE9E  0fbe7901                movsx    edi, byte ptr [ecx + 1]        
  0x0020DEA2  0fbf880c020000          movsx    ecx, word ptr [eax + 0x20c]    
  0x0020DEA9  c1e603                  shl      esi, 3                         
  0x0020DEAC  0fafce                  imul     ecx, esi                       
  0x0020DEAF  03f6                    add      esi, esi                       
  0x0020DEB1  0faff3                  imul     esi, ebx                       
  0x0020DEB4  c1e703                  shl      edi, 3                         
  0x0020DEB7  03cf                    add      ecx, edi                       
  0x0020DEB9  8d347e                  lea      esi, [esi + edi*2]             
  0x0020DEBC  8bb800020000            mov      edi, dword ptr [eax + 0x200]   
  0x0020DEC2  03f9                    add      edi, ecx                       
  0x0020DEC4  8d90f8000000            lea      edx, [eax + 0xf8]              
  0x0020DECA  897a04                  mov      dword ptr [edx + 4], edi       
  0x0020DECD  8bb804020000            mov      edi, dword ptr [eax + 0x204]   
  0x0020DED3  03f9                    add      edi, ecx                       
  0x0020DED5  897a0c                  mov      dword ptr [edx + 0xc], edi     
  0x0020DED8  8b8808020000            mov      ecx, dword ptr [eax + 0x208]   
  0x0020DEDE  03ce                    add      ecx, esi                       
  0x0020DEE0  894a14                  mov      dword ptr [edx + 0x14], ecx    
  0x0020DEE3  83c108                  add      ecx, 8                         
  0x0020DEE6  894a1c                  mov      dword ptr [edx + 0x1c], ecx    
  0x0020DEE9  8b4a14                  mov      ecx, dword ptr [edx + 0x14]    
  0x0020DEEC  8d0cd9                  lea      ecx, [ecx + ebx*8]             
  0x0020DEEF  894a24                  mov      dword ptr [edx + 0x24], ecx    
  0x0020DEF2  83c108                  add      ecx, 8                         
  0x0020DEF5  894a2c                  mov      dword ptr [edx + 0x2c], ecx    
  0x0020DEF8  8b7038                  mov      esi, dword ptr [eax + 0x38]    
  0x0020DEFB  8d88e0020000            lea      ecx, [eax + 0x2e0]             
  0x0020DF01  e8daedffff              call     0x20cce0                       ; -> sub_0020CCE0
  0x0020DF06  5f                      pop      edi                            
  0x0020DF07  5e                      pop      esi                            
  0x0020DF08  5b                      pop      ebx                            
  0x0020DF09  c3                      ret                                     
  0x0020DF0A  90                      nop                                     
  0x0020DF0B  90                      nop                                     
  0x0020DF0C  90                      nop                                     
  0x0020DF0D  90                      nop                                     
  0x0020DF0E  90                      nop                                     
  0x0020DF0F  90                      nop                                     

; ============================================================
; Function: sub_0020DF10
; Start: 0x0020DF10  End: 0x0020E0E9  Size: 473 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020DF10:
  0x0020DF10  83ec14                  sub      esp, 0x14                      
  0x0020DF13  53                      push     ebx                            
  0x0020DF14  55                      push     ebp                            
  0x0020DF15  56                      push     esi                            
  0x0020DF16  57                      push     edi                            
  0x0020DF17  8bf8                    mov      edi, eax                       
  0x0020DF19  8b442428                mov      eax, dword ptr [esp + 0x28]    
  0x0020DF1D  8b8874010000            mov      ecx, dword ptr [eax + 0x174]   
  0x0020DF23  8b909c020000            mov      edx, dword ptr [eax + 0x29c]   
  0x0020DF29  8d9450680c0000          lea      edx, [eax + edx*2 + 0xc68]     
  0x0020DF30  0fbe02                  movsx    eax, byte ptr [edx]            
  0x0020DF33  0fbe5201                movsx    edx, byte ptr [edx + 1]        
  0x0020DF37  8b742430                mov      esi, dword ptr [esp + 0x30]    
  0x0020DF3B  c1e003                  shl      eax, 3                         
  0x0020DF3E  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020DF42  8b4c2434                mov      ecx, dword ptr [esp + 0x34]    
  0x0020DF46  0fbf690e                movsx    ebp, word ptr [ecx + 0xe]      
  0x0020DF4A  0fbf490c                movsx    ecx, word ptr [ecx + 0xc]      
  0x0020DF4E  c1e203                  shl      edx, 3                         
  0x0020DF51  8bd9                    mov      ebx, ecx                       
  0x0020DF53  0fafd8                  imul     ebx, eax                       
  0x0020DF56  03c0                    add      eax, eax                       
  0x0020DF58  0fafc5                  imul     eax, ebp                       
  0x0020DF5B  03da                    add      ebx, edx                       
  0x0020DF5D  8d1450                  lea      edx, [eax + edx*2]             
  0x0020DF60  895604                  mov      dword ptr [esi + 4], edx       
  0x0020DF63  891e                    mov      dword ptr [esi], ebx           
  0x0020DF65  8b5f1c                  mov      ebx, dword ptr [edi + 0x1c]    
  0x0020DF68  8b4718                  mov      eax, dword ptr [edi + 0x18]    
  0x0020DF6B  8bfb                    mov      edi, ebx                       
  0x0020DF6D  d1ff                    sar      edi, 1                         
  0x0020DF6F  0faffd                  imul     edi, ebp                       
  0x0020DF72  8bf0                    mov      esi, eax                       
  0x0020DF74  03d7                    add      edx, edi                       
  0x0020DF76  d1fe                    sar      esi, 1                         
  0x0020DF78  03f2                    add      esi, edx                       
  0x0020DF7A  8b542410                mov      edx, dword ptr [esp + 0x10]    
  0x0020DF7E  8974241c                mov      dword ptr [esp + 0x1c], esi    
  0x0020DF82  8bf8                    mov      edi, eax                       
  0x0020DF84  03d2                    add      edx, edx                       
  0x0020DF86  8bf3                    mov      esi, ebx                       
  0x0020DF88  83e601                  and      esi, 1                         
  0x0020DF8B  03f2                    add      esi, edx                       
  0x0020DF8D  83e701                  and      edi, 1                         
  0x0020DF90  8d1477                  lea      edx, [edi + esi*2]             
  0x0020DF93  8b1495e0b22100          mov      edx, dword ptr [edx*4 + 0x21b2e0] 
  0x0020DF9A  89542420                mov      dword ptr [esp + 0x20], edx    
  0x0020DF9E  99                      cdq                                     
  0x0020DF9F  2bc2                    sub      eax, edx                       
  0x0020DFA1  8bf0                    mov      esi, eax                       
  0x0020DFA3  8bc3                    mov      eax, ebx                       
  0x0020DFA5  99                      cdq                                     
  0x0020DFA6  2bc2                    sub      eax, edx                       
  0x0020DFA8  d1f8                    sar      eax, 1                         
  0x0020DFAA  8bd8                    mov      ebx, eax                       
  0x0020DFAC  d1fb                    sar      ebx, 1                         
  0x0020DFAE  0fafd9                  imul     ebx, ecx                       
  0x0020DFB1  d1fe                    sar      esi, 1                         
  0x0020DFB3  8bd6                    mov      edx, esi                       
  0x0020DFB5  d1fa                    sar      edx, 1                         
  0x0020DFB7  03da                    add      ebx, edx                       
  0x0020DFB9  8b542430                mov      edx, dword ptr [esp + 0x30]    
  0x0020DFBD  031a                    add      ebx, dword ptr [edx]           
  0x0020DFBF  83e601                  and      esi, 1                         
  0x0020DFC2  8bd6                    mov      edx, esi                       
  0x0020DFC4  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x0020DFC8  83e001                  and      eax, 1                         
  0x0020DFCB  03f6                    add      esi, esi                       
  0x0020DFCD  03c6                    add      eax, esi                       
  0x0020DFCF  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x0020DFD3  8d0442                  lea      eax, [edx + eax*2]             
  0x0020DFD6  8b0485e0b22100          mov      eax, dword ptr [eax*4 + 0x21b2e0] 
  0x0020DFDD  23d6                    and      edx, esi                       
  0x0020DFDF  8b742428                mov      esi, dword ptr [esp + 0x28]    
  0x0020DFE3  89442430                mov      dword ptr [esp + 0x30], eax    
  0x0020DFE7  8b44242c                mov      eax, dword ptr [esp + 0x2c]    
  0x0020DFEB  81c6c4000000            add      esi, 0xc4                      
  0x0020DFF1  894608                  mov      dword ptr [esi + 8], eax       
  0x0020DFF4  8b442434                mov      eax, dword ptr [esp + 0x34]    
  0x0020DFF8  894c2418                mov      dword ptr [esp + 0x18], ecx    
  0x0020DFFC  89542414                mov      dword ptr [esp + 0x14], edx    
  0x0020E000  894e10                  mov      dword ptr [esi + 0x10], ecx    
  0x0020E003  8b00                    mov      eax, dword ptr [eax]           
  0x0020E005  03c3                    add      eax, ebx                       
  0x0020E007  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020E00A  03c2                    add      eax, edx                       
  0x0020E00C  03c1                    add      eax, ecx                       
  0x0020E00E  56                      push     esi                            
  0x0020E00F  894618                  mov      dword ptr [esi + 0x18], eax    
  0x0020E012  ff542434                call     dword ptr [esp + 0x34]         
  0x0020E016  8b4c2430                mov      ecx, dword ptr [esp + 0x30]    
  0x0020E01A  8b542438                mov      edx, dword ptr [esp + 0x38]    
  0x0020E01E  83c140                  add      ecx, 0x40                      
  0x0020E021  894e08                  mov      dword ptr [esi + 8], ecx       
  0x0020E024  8b4204                  mov      eax, dword ptr [edx + 4]       
  0x0020E027  8b4c2418                mov      ecx, dword ptr [esp + 0x18]    
  0x0020E02B  03c3                    add      eax, ebx                       
  0x0020E02D  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020E030  03c1                    add      eax, ecx                       
  0x0020E032  0344241c                add      eax, dword ptr [esp + 0x1c]    
  0x0020E036  56                      push     esi                            
  0x0020E037  894618                  mov      dword ptr [esi + 0x18], eax    
  0x0020E03A  ff542438                call     dword ptr [esp + 0x38]         
  0x0020E03E  8b5c2434                mov      ebx, dword ptr [esp + 0x34]    
  0x0020E042  8b44243c                mov      eax, dword ptr [esp + 0x3c]    
  0x0020E046  8b4c2424                mov      ecx, dword ptr [esp + 0x24]    
  0x0020E04A  8d9380000000            lea      edx, [ebx + 0x80]              
  0x0020E050  895608                  mov      dword ptr [esi + 8], edx       
  0x0020E053  8b542418                mov      edx, dword ptr [esp + 0x18]    
  0x0020E057  896e10                  mov      dword ptr [esi + 0x10], ebp    
  0x0020E05A  8b4008                  mov      eax, dword ptr [eax + 8]       
  0x0020E05D  23fa                    and      edi, edx                       
  0x0020E05F  03c1                    add      eax, ecx                       
  0x0020E061  03f8                    add      edi, eax                       
  0x0020E063  03fd                    add      edi, ebp                       
  0x0020E065  897e18                  mov      dword ptr [esi + 0x18], edi    
  0x0020E068  8b7c2428                mov      edi, dword ptr [esp + 0x28]    
  0x0020E06C  56                      push     esi                            
  0x0020E06D  894614                  mov      dword ptr [esi + 0x14], eax    
  0x0020E070  ffd7                    call     edi                            
  0x0020E072  8b5614                  mov      edx, dword ptr [esi + 0x14]    
  0x0020E075  8d8bc0000000            lea      ecx, [ebx + 0xc0]              
  0x0020E07B  894e08                  mov      dword ptr [esi + 8], ecx       
  0x0020E07E  8b4e18                  mov      ecx, dword ptr [esi + 0x18]    
  0x0020E081  b808000000              mov      eax, 8                         
  0x0020E086  03d0                    add      edx, eax                       
  0x0020E088  03c8                    add      ecx, eax                       
  0x0020E08A  56                      push     esi                            
  0x0020E08B  895614                  mov      dword ptr [esi + 0x14], edx    
  0x0020E08E  894e18                  mov      dword ptr [esi + 0x18], ecx    
  0x0020E091  ffd7                    call     edi                            
  0x0020E093  8b4e14                  mov      ecx, dword ptr [esi + 0x14]    
  0x0020E096  8d04edf8ffffff          lea      eax, [ebp*8 - 8]               
  0x0020E09D  03c8                    add      ecx, eax                       
  0x0020E09F  8d9300010000            lea      edx, [ebx + 0x100]             
  0x0020E0A5  895608                  mov      dword ptr [esi + 8], edx       
  0x0020E0A8  8b5618                  mov      edx, dword ptr [esi + 0x18]    
  0x0020E0AB  894e14                  mov      dword ptr [esi + 0x14], ecx    
  0x0020E0AE  8d0cedf8ffffff          lea      ecx, [ebp*8 - 8]               
  0x0020E0B5  03d1                    add      edx, ecx                       
  0x0020E0B7  56                      push     esi                            
  0x0020E0B8  895618                  mov      dword ptr [esi + 0x18], edx    
  0x0020E0BB  ffd7                    call     edi                            
  0x0020E0BD  8b5618                  mov      edx, dword ptr [esi + 0x18]    
  0x0020E0C0  81c340010000            add      ebx, 0x140                     
  0x0020E0C6  895e08                  mov      dword ptr [esi + 8], ebx       
  0x0020E0C9  8b5e14                  mov      ebx, dword ptr [esi + 0x14]    
  0x0020E0CC  b808000000              mov      eax, 8                         
  0x0020E0D1  03d8                    add      ebx, eax                       
  0x0020E0D3  03d0                    add      edx, eax                       
  0x0020E0D5  56                      push     esi                            
  0x0020E0D6  895e14                  mov      dword ptr [esi + 0x14], ebx    
  0x0020E0D9  895618                  mov      dword ptr [esi + 0x18], edx    
  0x0020E0DC  ffd7                    call     edi                            
  0x0020E0DE  83c418                  add      esp, 0x18                      
  0x0020E0E1  5f                      pop      edi                            
  0x0020E0E2  5e                      pop      esi                            
  0x0020E0E3  5d                      pop      ebp                            
  0x0020E0E4  5b                      pop      ebx                            
  0x0020E0E5  83c414                  add      esp, 0x14                      
  0x0020E0E8  c3                      ret                                     
; end of function
  0x0020E0E9  90                      nop                                     
  0x0020E0EA  90                      nop                                     
  0x0020E0EB  90                      nop                                     
  0x0020E0EC  90                      nop                                     
  0x0020E0ED  90                      nop                                     
  0x0020E0EE  90                      nop                                     
  0x0020E0EF  90                      nop                                     
  0x0020E0F0  83ec08                  sub      esp, 8                         
  0x0020E0F3  53                      push     ebx                            
  0x0020E0F4  55                      push     ebp                            
  0x0020E0F5  8b6c2414                mov      ebp, dword ptr [esp + 0x14]    
  0x0020E0F9  56                      push     esi                            
  0x0020E0FA  57                      push     edi                            
  0x0020E0FB  8b7c2420                mov      edi, dword ptr [esp + 0x20]    
  0x0020E0FF  4f                      dec      edi                            
  0x0020E100  8db5d0010000            lea      esi, [ebp + 0x1d0]             
  0x0020E106  8d5e10                  lea      ebx, [esi + 0x10]              
  0x0020E109  744f                    je       0x20e15a                       
  0x0020E10B  eb03                    jmp      0x20e110                       
  0x0020E10D  8d4900                  lea      ecx, [ecx]                     
                                        ; XREF: 0x0020E10B (jump), 0x0020E158 (cond_jump)
  0x0020E110  8b859c020000            mov      eax, dword ptr [ebp + 0x29c]   
  0x0020E116  0fbf560c                movsx    edx, word ptr [esi + 0xc]      
  0x0020E11A  2bc7                    sub      eax, edi                       
  0x0020E11C  8d8c45680c0000          lea      ecx, [ebp + eax*2 + 0xc68]     
  0x0020E123  0fbe01                  movsx    eax, byte ptr [ecx]            
  0x0020E126  0fbe4901                movsx    ecx, byte ptr [ecx + 1]        
  0x0020E12A  c1e003                  shl      eax, 3                         
  0x0020E12D  0fafd0                  imul     edx, eax                       
  0x0020E130  c1e103                  shl      ecx, 3                         
  0x0020E133  03d1                    add      edx, ecx                       
  0x0020E135  89542410                mov      dword ptr [esp + 0x10], edx    
  0x0020E139  0fbf560e                movsx    edx, word ptr [esi + 0xe]      
  0x0020E13D  03c0                    add      eax, eax                       
  0x0020E13F  0fafc2                  imul     eax, edx                       
  0x0020E142  8d0448                  lea      eax, [eax + ecx*2]             
  0x0020E145  8d4c2410                lea      ecx, [esp + 0x10]              
  0x0020E149  56                      push     esi                            
  0x0020E14A  51                      push     ecx                            
  0x0020E14B  8944241c                mov      dword ptr [esp + 0x1c], eax    
  0x0020E14F  e87cf8ffff              call     0x20d9d0                       ; -> sub_0020D9D0
  0x0020E154  83c408                  add      esp, 8                         
  0x0020E157  4f                      dec      edi                            
  0x0020E158  75b6                    jne      0x20e110                       
                                        ; XREF: 0x0020E109 (cond_jump)
  0x0020E15A  5f                      pop      edi                            
  0x0020E15B  5e                      pop      esi                            
  0x0020E15C  5d                      pop      ebp                            
  0x0020E15D  5b                      pop      ebx                            
  0x0020E15E  83c408                  add      esp, 8                         
  0x0020E161  c3                      ret                                     
  0x0020E162  90                      nop                                     
  0x0020E163  90                      nop                                     
  0x0020E164  90                      nop                                     
  0x0020E165  90                      nop                                     
  0x0020E166  90                      nop                                     
  0x0020E167  90                      nop                                     
  0x0020E168  90                      nop                                     
  0x0020E169  90                      nop                                     
  0x0020E16A  90                      nop                                     
  0x0020E16B  90                      nop                                     
  0x0020E16C  90                      nop                                     
  0x0020E16D  90                      nop                                     
  0x0020E16E  90                      nop                                     
  0x0020E16F  90                      nop                                     
  0x0020E170  83ec08                  sub      esp, 8                         
  0x0020E173  56                      push     esi                            
  0x0020E174  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x0020E178  57                      push     edi                            
  0x0020E179  8d8ed0010000            lea      ecx, [esi + 0x1d0]             
  0x0020E17F  51                      push     ecx                            
  0x0020E180  8b8ef0000000            mov      ecx, dword ptr [esi + 0xf0]    
  0x0020E186  8d54240c                lea      edx, [esp + 0xc]               
  0x0020E18A  8dbee8000000            lea      edi, [esi + 0xe8]              
  0x0020E190  52                      push     edx                            
  0x0020E191  51                      push     ecx                            
  0x0020E192  8d8654020000            lea      eax, [esi + 0x254]             
  0x0020E198  56                      push     esi                            
  0x0020E199  e872fdffff              call     0x20df10                       ; -> sub_0020DF10
  0x0020E19E  8b9600020000            mov      edx, dword ptr [esi + 0x200]   
  0x0020E1A4  8b442418                mov      eax, dword ptr [esp + 0x18]    
  0x0020E1A8  03d0                    add      edx, eax                       
  0x0020E1AA  8d8ef8000000            lea      ecx, [esi + 0xf8]              
  0x0020E1B0  895104                  mov      dword ptr [ecx + 4], edx       
  0x0020E1B3  8b9604020000            mov      edx, dword ptr [esi + 0x204]   
  0x0020E1B9  03d0                    add      edx, eax                       
  0x0020E1BB  89510c                  mov      dword ptr [ecx + 0xc], edx     
  0x0020E1BE  8b8608020000            mov      eax, dword ptr [esi + 0x208]   
  0x0020E1C4  8b54241c                mov      edx, dword ptr [esp + 0x1c]    
  0x0020E1C8  03c2                    add      eax, edx                       
  0x0020E1CA  894114                  mov      dword ptr [ecx + 0x14], eax    
  0x0020E1CD  8b5114                  mov      edx, dword ptr [ecx + 0x14]    
  0x0020E1D0  83c008                  add      eax, 8                         
  0x0020E1D3  89411c                  mov      dword ptr [ecx + 0x1c], eax    
  0x0020E1D6  0fbf860e020000          movsx    eax, word ptr [esi + 0x20e]    
  0x0020E1DD  8d04c2                  lea      eax, [edx + eax*8]             
  0x0020E1E0  894124                  mov      dword ptr [ecx + 0x24], eax    
  0x0020E1E3  83c008                  add      eax, 8                         
  0x0020E1E6  89412c                  mov      dword ptr [ecx + 0x2c], eax    
  0x0020E1E9  8b86a8020000            mov      eax, dword ptr [esi + 0x2a8]   
  0x0020E1EF  50                      push     eax                            
  0x0020E1F0  8bc7                    mov      eax, edi                       
  0x0020E1F2  e899edffff              call     0x20cf90                       ; -> sub_0020CF90
  0x0020E1F7  83c414                  add      esp, 0x14                      
  0x0020E1FA  5f                      pop      edi                            
  0x0020E1FB  5e                      pop      esi                            
  0x0020E1FC  83c408                  add      esp, 8                         
  0x0020E1FF  c3                      ret                                     
  0x0020E200  83ec08                  sub      esp, 8                         
  0x0020E203  56                      push     esi                            
  0x0020E204  8b742410                mov      esi, dword ptr [esp + 0x10]    
  0x0020E208  57                      push     edi                            
  0x0020E209  8d8ee0010000            lea      ecx, [esi + 0x1e0]             
  0x0020E20F  51                      push     ecx                            
  0x0020E210  8b8ef0000000            mov      ecx, dword ptr [esi + 0xf0]    
  0x0020E216  8d54240c                lea      edx, [esp + 0xc]               
  0x0020E21A  8dbee8000000            lea      edi, [esi + 0xe8]              
  0x0020E220  52                      push     edx                            
  0x0020E221  51                      push     ecx                            
  0x0020E222  8d8678020000            lea      eax, [esi + 0x278]             
  0x0020E228  56                      push     esi                            
  0x0020E229  e8e2fcffff              call     0x20df10                       ; -> sub_0020DF10
  0x0020E22E  8b9600020000            mov      edx, dword ptr [esi + 0x200]   
  0x0020E234  8b442418                mov      eax, dword ptr [esp + 0x18]    
  0x0020E238  03d0                    add      edx, eax                       
  0x0020E23A  8d8ef8000000            lea      ecx, [esi + 0xf8]              
  0x0020E240  895104                  mov      dword ptr [ecx + 4], edx       
  0x0020E243  8b9604020000            mov      edx, dword ptr [esi + 0x204]   
  0x0020E249  03d0                    add      edx, eax                       
  0x0020E24B  89510c                  mov      dword ptr [ecx + 0xc], edx     
  0x0020E24E  8b8608020000            mov      eax, dword ptr [esi + 0x208]   
  0x0020E254  8b54241c                mov      edx, dword ptr [esp + 0x1c]    
  0x0020E258  03c2                    add      eax, edx                       
  0x0020E25A  894114                  mov      dword ptr [ecx + 0x14], eax    
  0x0020E25D  8b5114                  mov      edx, dword ptr [ecx + 0x14]    
  0x0020E260  83c008                  add      eax, 8                         
  0x0020E263  89411c                  mov      dword ptr [ecx + 0x1c], eax    
  0x0020E266  0fbf860e020000          movsx    eax, word ptr [esi + 0x20e]    
  0x0020E26D  8d04c2                  lea      eax, [edx + eax*8]             
  0x0020E270  894124                  mov      dword ptr [ecx + 0x24], eax    
  0x0020E273  83c008                  add      eax, 8                         
  0x0020E276  89412c                  mov      dword ptr [ecx + 0x2c], eax    
  0x0020E279  8b86a8020000            mov      eax, dword ptr [esi + 0x2a8]   
  0x0020E27F  50                      push     eax                            
  0x0020E280  8bc7                    mov      eax, edi                       
  0x0020E282  e809edffff              call     0x20cf90                       ; -> sub_0020CF90
  0x0020E287  83c414                  add      esp, 0x14                      
  0x0020E28A  5f                      pop      edi                            
  0x0020E28B  5e                      pop      esi                            
  0x0020E28C  83c408                  add      esp, 8                         
  0x0020E28F  c3                      ret                                     
  0x0020E290  83ec08                  sub      esp, 8                         
  0x0020E293  53                      push     ebx                            
  0x0020E294  56                      push     esi                            
  0x0020E295  8b742414                mov      esi, dword ptr [esp + 0x14]    
  0x0020E299  8b96f0000000            mov      edx, dword ptr [esi + 0xf0]    
  0x0020E29F  57                      push     edi                            
  0x0020E2A0  8d9ed0010000            lea      ebx, [esi + 0x1d0]             
  0x0020E2A6  53                      push     ebx                            
  0x0020E2A7  8d4c2410                lea      ecx, [esp + 0x10]              
  0x0020E2AB  8dbee8000000            lea      edi, [esi + 0xe8]              
  0x0020E2B1  51                      push     ecx                            
  0x0020E2B2  52                      push     edx                            
  0x0020E2B3  8d8654020000            lea      eax, [esi + 0x254]             
  0x0020E2B9  56                      push     esi                            
  0x0020E2BA  e851fcffff              call     0x20df10                       ; -> sub_0020DF10
  0x0020E2BF  8b570c                  mov      edx, dword ptr [edi + 0xc]     
  0x0020E2C2  83c310                  add      ebx, 0x10                      
  0x0020E2C5  53                      push     ebx                            
  0x0020E2C6  8d4c2420                lea      ecx, [esp + 0x20]              
  0x0020E2CA  51                      push     ecx                            
  0x0020E2CB  52                      push     edx                            
  0x0020E2CC  8d8678020000            lea      eax, [esi + 0x278]             
  0x0020E2D2  56                      push     esi                            
  0x0020E2D3  e838fcffff              call     0x20df10                       ; -> sub_0020DF10
  0x0020E2D8  8b8e00020000            mov      ecx, dword ptr [esi + 0x200]   
  0x0020E2DE  8b44242c                mov      eax, dword ptr [esp + 0x2c]    
  0x0020E2E2  03c8                    add      ecx, eax                       
  0x0020E2E4  8d96f8000000            lea      edx, [esi + 0xf8]              
  0x0020E2EA  894a04                  mov      dword ptr [edx + 4], ecx       
  0x0020E2ED  8b8e04020000            mov      ecx, dword ptr [esi + 0x204]   
  0x0020E2F3  03c8                    add      ecx, eax                       
  0x0020E2F5  894a0c                  mov      dword ptr [edx + 0xc], ecx     
  0x0020E2F8  8b8608020000            mov      eax, dword ptr [esi + 0x208]   
  0x0020E2FE  8b4c2430                mov      ecx, dword ptr [esp + 0x30]    
  0x0020E302  03c1                    add      eax, ecx                       
  0x0020E304  894214                  mov      dword ptr [edx + 0x14], eax    
  0x0020E307  8b4a14                  mov      ecx, dword ptr [edx + 0x14]    
  0x0020E30A  83c008                  add      eax, 8                         
  0x0020E30D  89421c                  mov      dword ptr [edx + 0x1c], eax    
  0x0020E310  0fbf860e020000          movsx    eax, word ptr [esi + 0x20e]    
  0x0020E317  8d04c1                  lea      eax, [ecx + eax*8]             
  0x0020E31A  894224                  mov      dword ptr [edx + 0x24], eax    
  0x0020E31D  83c008                  add      eax, 8                         
  0x0020E320  89422c                  mov      dword ptr [edx + 0x2c], eax    
  0x0020E323  8b86a8020000            mov      eax, dword ptr [esi + 0x2a8]   
  0x0020E329  50                      push     eax                            
  0x0020E32A  8bc7                    mov      eax, edi                       
  0x0020E32C  e8bfefffff              call     0x20d2f0                       ; -> sub_0020D2F0
  0x0020E331  83c424                  add      esp, 0x24                      
  0x0020E334  5f                      pop      edi                            
  0x0020E335  5e                      pop      esi                            
  0x0020E336  5b                      pop      ebx                            
  0x0020E337  83c408                  add      esp, 8                         
  0x0020E33A  c3                      ret                                     
  0x0020E33B  90                      nop                                     
  0x0020E33C  90                      nop                                     
  0x0020E33D  90                      nop                                     
  0x0020E33E  90                      nop                                     
  0x0020E33F  90                      nop                                     
  0x0020E340  83ec24                  sub      esp, 0x24                      
  0x0020E343  53                      push     ebx                            
  0x0020E344  8b5c242c                mov      ebx, dword ptr [esp + 0x2c]    
  0x0020E348  55                      push     ebp                            
  0x0020E349  56                      push     esi                            
  0x0020E34A  57                      push     edi                            
  0x0020E34B  33c0                    xor      eax, eax                       
  0x0020E34D  8d733c                  lea      esi, [ebx + 0x3c]              
  0x0020E350  8dabe0020000            lea      ebp, [ebx + 0x2e0]             
  0x0020E356  b9c0000000              mov      ecx, 0xc0                      
  0x0020E35B  8bfd                    mov      edi, ebp                       
  0x0020E35D  f3ab                    rep stosd dword ptr es:[edi], eax        
  0x0020E35F  8b8350020000            mov      eax, dword ptr [ebx + 0x250]   
  0x0020E365  8d8be0050000            lea      ecx, [ebx + 0x5e0]             
  0x0020E36B  894624                  mov      dword ptr [esi + 0x24], eax    
  0x0020E36E  894e20                  mov      dword ptr [esi + 0x20], ecx    
  0x0020E371  c7463000000000          mov      dword ptr [esi + 0x30], 0      
  0x0020E378  8b152047c800            mov      edx, dword ptr [0xc84720]      
  0x0020E37E  8d83ac020000            lea      eax, [ebx + 0x2ac]             
  0x0020E384  56                      push     esi                            
  0x0020E385  8dbbe0030000            lea      edi, [ebx + 0x3e0]             
  0x0020E38B  53                      push     ebx                            
  0x0020E38C  89562c                  mov      dword ptr [esi + 0x2c], edx    
  0x0020E38F  894628                  mov      dword ptr [esi + 0x28], eax    
  0x0020E392  897e1c                  mov      dword ptr [esi + 0x1c], edi    
  0x0020E395  e8a628faff              call     0x1b0c40                       ; -> sub_001B0C40
  0x0020E39A  8944242c                mov      dword ptr [esp + 0x2c], eax    
  0x0020E39E  8d8360040000            lea      eax, [ebx + 0x460]             
  0x0020E3A4  56                      push     esi                            
  0x0020E3A5  53                      push     ebx                            
  0x0020E3A6  89442420                mov      dword ptr [esp + 0x20], eax    
  0x0020E3AA  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x0020E3AD  e88e28faff              call     0x1b0c40                       ; -> sub_001B0C40
  0x0020E3B2  89442438                mov      dword ptr [esp + 0x38], eax    
  0x0020E3B6  8d83e0040000            lea      eax, [ebx + 0x4e0]             
  0x0020E3BC  56                      push     esi                            
  0x0020E3BD  53                      push     ebx                            
  0x0020E3BE  8944242c                mov      dword ptr [esp + 0x2c], eax    
  0x0020E3C2  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x0020E3C5  e87628faff              call     0x1b0c40                       ; -> sub_001B0C40
  0x0020E3CA  89442444                mov      dword ptr [esp + 0x44], eax    
  0x0020E3CE  8d8360050000            lea      eax, [ebx + 0x560]             
  0x0020E3D4  56                      push     esi                            
  0x0020E3D5  53                      push     ebx                            
  0x0020E3D6  89442438                mov      dword ptr [esp + 0x38], eax    
  0x0020E3DA  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x0020E3DD  e85e28faff              call     0x1b0c40                       ; -> sub_001B0C40
  0x0020E3E2  8b0de04ac800            mov      ecx, dword ptr [0xc84ae0]      
  0x0020E3E8  8d93b0020000            lea      edx, [ebx + 0x2b0]             
  0x0020E3EE  56                      push     esi                            
  0x0020E3EF  53                      push     ebx                            
  0x0020E3F0  89442458                mov      dword ptr [esp + 0x58], eax    
  0x0020E3F4  894e2c                  mov      dword ptr [esi + 0x2c], ecx    
  0x0020E3F7  895628                  mov      dword ptr [esi + 0x28], edx    
  0x0020E3FA  896e1c                  mov      dword ptr [esi + 0x1c], ebp    
  0x0020E3FD  e83e28faff              call     0x1b0c40                       ; -> sub_001B0C40
  0x0020E402  89442444                mov      dword ptr [esp + 0x44], eax    
  0x0020E406  8d83b4020000            lea      eax, [ebx + 0x2b4]             
  0x0020E40C  894628                  mov      dword ptr [esi + 0x28], eax    
  0x0020E40F  8d8360030000            lea      eax, [ebx + 0x360]             
  0x0020E415  56                      push     esi                            
  0x0020E416  53                      push     ebx                            
  0x0020E417  89442468                mov      dword ptr [esp + 0x68], eax    
  0x0020E41B  89461c                  mov      dword ptr [esi + 0x1c], eax    
  0x0020E41E  e81d28faff              call     0x1b0c40                       ; -> sub_001B0C40
  0x0020E423  8b4c244c                mov      ecx, dword ptr [esp + 0x4c]    
  0x0020E427  51                      push     ecx                            
  0x0020E428  55                      push     ebp                            
  0x0020E429  8bf0                    mov      esi, eax                       
  0x0020E42B  e8901ffaff              call     0x1b03c0                       ; -> sub_001B03C0
  0x0020E430  8b542470                mov      edx, dword ptr [esp + 0x70]    
  0x0020E434  56                      push     esi                            
  0x0020E435  52                      push     edx                            
  0x0020E436  e8851ffaff              call     0x1b03c0                       ; -> sub_001B03C0
  0x0020E43B  8b442464                mov      eax, dword ptr [esp + 0x64]    
  0x0020E43F  83c440                  add      esp, 0x40                      
  0x0020E442  50                      push     eax                            
  0x0020E443  57                      push     edi                            
  0x0020E444  e8771ffaff              call     0x1b03c0                       ; -> sub_001B03C0
  0x0020E449  8b4c2430                mov      ecx, dword ptr [esp + 0x30]    
  0x0020E44D  8b542418                mov      edx, dword ptr [esp + 0x18]    
  0x0020E451  51                      push     ecx                            
  0x0020E452  52                      push     edx                            
  0x0020E453  e8681ffaff              call     0x1b03c0                       ; -> sub_001B03C0
  0x0020E458  8b44243c                mov      eax, dword ptr [esp + 0x3c]    
  0x0020E45C  8b4c2424                mov      ecx, dword ptr [esp + 0x24]    
  0x0020E460  50                      push     eax                            
  0x0020E461  51                      push     ecx                            
  0x0020E462  e8591ffaff              call     0x1b03c0                       ; -> sub_001B03C0
  0x0020E467  8b542448                mov      edx, dword ptr [esp + 0x48]    
  0x0020E46B  8b442430                mov      eax, dword ptr [esp + 0x30]    
  0x0020E46F  52                      push     edx                            
  0x0020E470  50                      push     eax                            
  0x0020E471  e84a1ffaff              call     0x1b03c0                       ; -> sub_001B03C0
  0x0020E476  83c420                  add      esp, 0x20                      
  0x0020E479  5f                      pop      edi                            
  0x0020E47A  5e                      pop      esi                            
  0x0020E47B  5d                      pop      ebp                            
  0x0020E47C  33c0                    xor      eax, eax                       
  0x0020E47E  5b                      pop      ebx                            
  0x0020E47F  83c424                  add      esp, 0x24                      
  0x0020E482  c3                      ret                                     
  0x0020E483  90                      nop                                     
  0x0020E484  90                      nop                                     
  0x0020E485  90                      nop                                     
  0x0020E486  90                      nop                                     
  0x0020E487  90                      nop                                     
  0x0020E488  90                      nop                                     
  0x0020E489  90                      nop                                     
  0x0020E48A  90                      nop                                     
  0x0020E48B  90                      nop                                     
  0x0020E48C  90                      nop                                     
  0x0020E48D  90                      nop                                     
  0x0020E48E  90                      nop                                     
  0x0020E48F  90                      nop                                     
  0x0020E490  83ec20                  sub      esp, 0x20                      
  0x0020E493  53                      push     ebx                            
  0x0020E494  55                      push     ebp                            
  0x0020E495  56                      push     esi                            
  0x0020E496  8b742430                mov      esi, dword ptr [esp + 0x30]    
  0x0020E49A  8b8650020000            mov      eax, dword ptr [esi + 0x250]   
  0x0020E4A0  57                      push     edi                            
  0x0020E4A1  8d7e3c                  lea      edi, [esi + 0x3c]              
  0x0020E4A4  8d8e20060000            lea      ecx, [esi + 0x620]             
  0x0020E4AA  894724                  mov      dword ptr [edi + 0x24], eax    
  0x0020E4AD  894f20                  mov      dword ptr [edi + 0x20], ecx    
  0x0020E4B0  c7473001000000          mov      dword ptr [edi + 0x30], 1      
  0x0020E4B7  8baea8020000            mov      ebp, dword ptr [esi + 0x2a8]   
  0x0020E4BD  c1e502                  shl      ebp, 2                         
  0x0020E4C0  8d86b8020000            lea      eax, [esi + 0x2b8]             
  0x0020E4C6  896c2434                mov      dword ptr [esp + 0x34], ebp    
  0x0020E4CA  33db                    xor      ebx, ebx                       
  0x0020E4CC  89442414                mov      dword ptr [esp + 0x14], eax    
  0x0020E4D0  89442410                mov      dword ptr [esp + 0x10], eax    
                                        ; XREF: 0x0020E50C (cond_jump)
  0x0020E4D4  8b442434                mov      eax, dword ptr [esp + 0x34]    
  0x0020E4D8  85c0                    test     eax, eax                       
  0x0020E4DA  7d17                    jge      0x20e4f3                       
  0x0020E4DC  8b542410                mov      edx, dword ptr [esp + 0x10]    
  0x0020E4E0  8b02                    mov      eax, dword ptr [edx]           
  0x0020E4E2  57                      push     edi                            
  0x0020E4E3  56                      push     esi                            
  0x0020E4E4  89471c                  mov      dword ptr [edi + 0x1c], eax    
  0x0020E4E7  e8c45afaff              call     0x1b3fb0                       ; -> sub_001B3FB0
  0x0020E4EC  83c408                  add      esp, 8                         
  0x0020E4EF  89449c18                mov      dword ptr [esp + ebx*4 + 0x18], eax 
                                        ; XREF: 0x0020E4DA (cond_jump)
  0x0020E4F3  8b442434                mov      eax, dword ptr [esp + 0x34]    
  0x0020E4F7  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x0020E4FB  d1e0                    shl      eax, 1                         
  0x0020E4FD  43                      inc      ebx                            
  0x0020E4FE  83c104                  add      ecx, 4                         
  0x0020E501  83fb06                  cmp      ebx, 6                         
  0x0020E504  89442434                mov      dword ptr [esp + 0x34], eax    
  0x0020E508  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020E50C  7cc6                    jl       0x20e4d4                       
  0x0020E50E  8b7c2414                mov      edi, dword ptr [esp + 0x14]    
  0x0020E512  33f6                    xor      esi, esi                       
                                        ; XREF: 0x0020E531 (cond_jump)
  0x0020E514  85ed                    test     ebp, ebp                       
  0x0020E516  7d10                    jge      0x20e528                       
  0x0020E518  8b4cb418                mov      ecx, dword ptr [esp + esi*4 + 0x18] 
  0x0020E51C  8b17                    mov      edx, dword ptr [edi]           
  0x0020E51E  51                      push     ecx                            
  0x0020E51F  52                      push     edx                            
  0x0020E520  e89b1efaff              call     0x1b03c0                       ; -> sub_001B03C0
  0x0020E525  83c408                  add      esp, 8                         
                                        ; XREF: 0x0020E516 (cond_jump)
  0x0020E528  d1e5                    shl      ebp, 1                         
  0x0020E52A  46                      inc      esi                            
  0x0020E52B  83c704                  add      edi, 4                         
  0x0020E52E  83fe06                  cmp      esi, 6                         
  0x0020E531  7ce1                    jl       0x20e514                       
  0x0020E533  5f                      pop      edi                            
  0x0020E534  5e                      pop      esi                            
  0x0020E535  5d                      pop      ebp                            
  0x0020E536  33c0                    xor      eax, eax                       
  0x0020E538  5b                      pop      ebx                            
  0x0020E539  83c420                  add      esp, 0x20                      
  0x0020E53C  c3                      ret                                     
  0x0020E53D  90                      nop                                     
  0x0020E53E  90                      nop                                     
  0x0020E53F  90                      nop                                     
  0x0020E540  8b542404                mov      edx, dword ptr [esp + 4]       
  0x0020E544  8b4214                  mov      eax, dword ptr [edx + 0x14]    
  0x0020E547  8b4a18                  mov      ecx, dword ptr [edx + 0x18]    
  0x0020E54A  53                      push     ebx                            
  0x0020E54B  55                      push     ebp                            
  0x0020E54C  56                      push     esi                            
  0x0020E54D  8b7208                  mov      esi, dword ptr [edx + 8]       
  0x0020E550  57                      push     edi                            
  0x0020E551  8b7a10                  mov      edi, dword ptr [edx + 0x10]    
  0x0020E554  bd02000000              mov      ebp, 2                         
  0x0020E559  8da42400000000          lea      esp, [esp]                     
                                        ; XREF: 0x0020E932 (cond_jump)
  0x0020E560  0fb618                  movzx    ebx, byte ptr [eax]            
  0x0020E563  0fb611                  movzx    edx, byte ptr [ecx]            
  0x0020E566  03d3                    add      edx, ebx                       
  0x0020E568  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020E56C  03d3                    add      edx, ebx                       
  0x0020E56E  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E572  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E576  c1fa02                  sar      edx, 2                         
  0x0020E579  8816                    mov      byte ptr [esi], dl             
  0x0020E57B  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E57F  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E583  03d3                    add      edx, ebx                       
  0x0020E585  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020E589  03d3                    add      edx, ebx                       
  0x0020E58B  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E58F  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E593  c1fa02                  sar      edx, 2                         
  0x0020E596  885601                  mov      byte ptr [esi + 1], dl         
  0x0020E599  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E59D  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E5A1  03d3                    add      edx, ebx                       
  0x0020E5A3  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E5A7  03d3                    add      edx, ebx                       
  0x0020E5A9  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E5AD  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E5B1  c1fa02                  sar      edx, 2                         
  0x0020E5B4  885602                  mov      byte ptr [esi + 2], dl         
  0x0020E5B7  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E5BB  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E5BF  03d3                    add      edx, ebx                       
  0x0020E5C1  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E5C5  03d3                    add      edx, ebx                       
  0x0020E5C7  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E5CB  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E5CF  c1fa02                  sar      edx, 2                         
  0x0020E5D2  885603                  mov      byte ptr [esi + 3], dl         
  0x0020E5D5  0fb65805                movzx    ebx, byte ptr [eax + 5]        
  0x0020E5D9  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E5DD  03d3                    add      edx, ebx                       
  0x0020E5DF  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E5E3  03d3                    add      edx, ebx                       
  0x0020E5E5  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E5E9  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E5ED  c1fa02                  sar      edx, 2                         
  0x0020E5F0  885604                  mov      byte ptr [esi + 4], dl         
  0x0020E5F3  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E5F7  0fb65005                movzx    edx, byte ptr [eax + 5]        
  0x0020E5FB  03d3                    add      edx, ebx                       
  0x0020E5FD  0fb65806                movzx    ebx, byte ptr [eax + 6]        
  0x0020E601  03d3                    add      edx, ebx                       
  0x0020E603  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E607  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E60B  c1fa02                  sar      edx, 2                         
  0x0020E60E  885605                  mov      byte ptr [esi + 5], dl         
  0x0020E611  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E615  0fb65006                movzx    edx, byte ptr [eax + 6]        
  0x0020E619  03d3                    add      edx, ebx                       
  0x0020E61B  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E61F  03d3                    add      edx, ebx                       
  0x0020E621  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E625  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E629  c1fa02                  sar      edx, 2                         
  0x0020E62C  885606                  mov      byte ptr [esi + 6], dl         
  0x0020E62F  0fb65808                movzx    ebx, byte ptr [eax + 8]        
  0x0020E633  0fb65108                movzx    edx, byte ptr [ecx + 8]        
  0x0020E637  03d3                    add      edx, ebx                       
  0x0020E639  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E63D  03d3                    add      edx, ebx                       
  0x0020E63F  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E643  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E647  c1fa02                  sar      edx, 2                         
  0x0020E64A  885607                  mov      byte ptr [esi + 7], dl         
  0x0020E64D  0fb61c38                movzx    ebx, byte ptr [eax + edi]      
  0x0020E651  0fb61439                movzx    edx, byte ptr [ecx + edi]      
  0x0020E655  03c7                    add      eax, edi                       
  0x0020E657  03cf                    add      ecx, edi                       
  0x0020E659  03d3                    add      edx, ebx                       
  0x0020E65B  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020E65F  03d3                    add      edx, ebx                       
  0x0020E661  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E665  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E669  c1fa02                  sar      edx, 2                         
  0x0020E66C  885608                  mov      byte ptr [esi + 8], dl         
  0x0020E66F  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E673  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E677  03d3                    add      edx, ebx                       
  0x0020E679  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020E67D  03d3                    add      edx, ebx                       
  0x0020E67F  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E683  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E687  c1fa02                  sar      edx, 2                         
  0x0020E68A  885609                  mov      byte ptr [esi + 9], dl         
  0x0020E68D  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E691  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E695  03d3                    add      edx, ebx                       
  0x0020E697  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E69B  03d3                    add      edx, ebx                       
  0x0020E69D  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E6A1  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E6A5  c1fa02                  sar      edx, 2                         
  0x0020E6A8  88560a                  mov      byte ptr [esi + 0xa], dl       
  0x0020E6AB  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E6AF  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E6B3  03d3                    add      edx, ebx                       
  0x0020E6B5  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E6B9  03d3                    add      edx, ebx                       
  0x0020E6BB  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E6BF  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E6C3  c1fa02                  sar      edx, 2                         
  0x0020E6C6  88560b                  mov      byte ptr [esi + 0xb], dl       
  0x0020E6C9  0fb65805                movzx    ebx, byte ptr [eax + 5]        
  0x0020E6CD  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E6D1  03d3                    add      edx, ebx                       
  0x0020E6D3  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E6D7  03d3                    add      edx, ebx                       
  0x0020E6D9  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E6DD  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E6E1  c1fa02                  sar      edx, 2                         
  0x0020E6E4  88560c                  mov      byte ptr [esi + 0xc], dl       
  0x0020E6E7  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E6EB  0fb65005                movzx    edx, byte ptr [eax + 5]        
  0x0020E6EF  03d3                    add      edx, ebx                       
  0x0020E6F1  0fb65806                movzx    ebx, byte ptr [eax + 6]        
  0x0020E6F5  03d3                    add      edx, ebx                       
  0x0020E6F7  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E6FB  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E6FF  c1fa02                  sar      edx, 2                         
  0x0020E702  88560d                  mov      byte ptr [esi + 0xd], dl       
  0x0020E705  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E709  0fb65006                movzx    edx, byte ptr [eax + 6]        
  0x0020E70D  03d3                    add      edx, ebx                       
  0x0020E70F  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E713  03d3                    add      edx, ebx                       
  0x0020E715  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E719  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E71D  c1fa02                  sar      edx, 2                         
  0x0020E720  88560e                  mov      byte ptr [esi + 0xe], dl       
  0x0020E723  0fb65808                movzx    ebx, byte ptr [eax + 8]        
  0x0020E727  0fb65108                movzx    edx, byte ptr [ecx + 8]        
  0x0020E72B  03d3                    add      edx, ebx                       
  0x0020E72D  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E731  03d3                    add      edx, ebx                       
  0x0020E733  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E737  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E73B  c1fa02                  sar      edx, 2                         
  0x0020E73E  88560f                  mov      byte ptr [esi + 0xf], dl       
  0x0020E741  0fb61c38                movzx    ebx, byte ptr [eax + edi]      
  0x0020E745  0fb61439                movzx    edx, byte ptr [ecx + edi]      
  0x0020E749  03cf                    add      ecx, edi                       
  0x0020E74B  03c7                    add      eax, edi                       
  0x0020E74D  03d3                    add      edx, ebx                       
  0x0020E74F  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020E753  03d3                    add      edx, ebx                       
  0x0020E755  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E759  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E75D  c1fa02                  sar      edx, 2                         
  0x0020E760  885610                  mov      byte ptr [esi + 0x10], dl      
  0x0020E763  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E767  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E76B  03d3                    add      edx, ebx                       
  0x0020E76D  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020E771  03d3                    add      edx, ebx                       
  0x0020E773  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E777  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E77B  c1fa02                  sar      edx, 2                         
  0x0020E77E  885611                  mov      byte ptr [esi + 0x11], dl      
  0x0020E781  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E785  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E789  03d3                    add      edx, ebx                       
  0x0020E78B  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E78F  03d3                    add      edx, ebx                       
  0x0020E791  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E795  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E799  c1fa02                  sar      edx, 2                         
  0x0020E79C  885612                  mov      byte ptr [esi + 0x12], dl      
  0x0020E79F  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E7A3  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E7A7  03d3                    add      edx, ebx                       
  0x0020E7A9  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E7AD  03d3                    add      edx, ebx                       
  0x0020E7AF  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E7B3  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E7B7  c1fa02                  sar      edx, 2                         
  0x0020E7BA  885613                  mov      byte ptr [esi + 0x13], dl      
  0x0020E7BD  0fb65805                movzx    ebx, byte ptr [eax + 5]        
  0x0020E7C1  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E7C5  03d3                    add      edx, ebx                       
  0x0020E7C7  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E7CB  03d3                    add      edx, ebx                       
  0x0020E7CD  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E7D1  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E7D5  c1fa02                  sar      edx, 2                         
  0x0020E7D8  885614                  mov      byte ptr [esi + 0x14], dl      
  0x0020E7DB  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E7DF  0fb65005                movzx    edx, byte ptr [eax + 5]        
  0x0020E7E3  03d3                    add      edx, ebx                       
  0x0020E7E5  0fb65806                movzx    ebx, byte ptr [eax + 6]        
  0x0020E7E9  03d3                    add      edx, ebx                       
  0x0020E7EB  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E7EF  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E7F3  c1fa02                  sar      edx, 2                         
  0x0020E7F6  885615                  mov      byte ptr [esi + 0x15], dl      
  0x0020E7F9  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E7FD  0fb65006                movzx    edx, byte ptr [eax + 6]        
  0x0020E801  03d3                    add      edx, ebx                       
  0x0020E803  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E807  03d3                    add      edx, ebx                       
  0x0020E809  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E80D  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E811  c1fa02                  sar      edx, 2                         
  0x0020E814  885616                  mov      byte ptr [esi + 0x16], dl      
  0x0020E817  0fb65808                movzx    ebx, byte ptr [eax + 8]        
  0x0020E81B  0fb65108                movzx    edx, byte ptr [ecx + 8]        
  0x0020E81F  03d3                    add      edx, ebx                       
  0x0020E821  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E825  03d3                    add      edx, ebx                       
  0x0020E827  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E82B  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E82F  c1fa02                  sar      edx, 2                         
  0x0020E832  885617                  mov      byte ptr [esi + 0x17], dl      
  0x0020E835  0fb61c38                movzx    ebx, byte ptr [eax + edi]      
  0x0020E839  0fb61439                movzx    edx, byte ptr [ecx + edi]      
  0x0020E83D  03d3                    add      edx, ebx                       
  0x0020E83F  0fb65c3901              movzx    ebx, byte ptr [ecx + edi + 1]  
  0x0020E844  03c7                    add      eax, edi                       
  0x0020E846  03cf                    add      ecx, edi                       
  0x0020E848  03d3                    add      edx, ebx                       
  0x0020E84A  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E84E  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E852  c1fa02                  sar      edx, 2                         
  0x0020E855  885618                  mov      byte ptr [esi + 0x18], dl      
  0x0020E858  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E85C  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E860  03d3                    add      edx, ebx                       
  0x0020E862  0fb65901                movzx    ebx, byte ptr [ecx + 1]        
  0x0020E866  03d3                    add      edx, ebx                       
  0x0020E868  0fb65801                movzx    ebx, byte ptr [eax + 1]        
  0x0020E86C  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E870  c1fa02                  sar      edx, 2                         
  0x0020E873  885619                  mov      byte ptr [esi + 0x19], dl      
  0x0020E876  0fb65002                movzx    edx, byte ptr [eax + 2]        
  0x0020E87A  0fb65902                movzx    ebx, byte ptr [ecx + 2]        
  0x0020E87E  03d3                    add      edx, ebx                       
  0x0020E880  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E884  03d3                    add      edx, ebx                       
  0x0020E886  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E88A  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E88E  c1fa02                  sar      edx, 2                         
  0x0020E891  88561a                  mov      byte ptr [esi + 0x1a], dl      
  0x0020E894  0fb65803                movzx    ebx, byte ptr [eax + 3]        
  0x0020E898  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E89C  03d3                    add      edx, ebx                       
  0x0020E89E  0fb65903                movzx    ebx, byte ptr [ecx + 3]        
  0x0020E8A2  03d3                    add      edx, ebx                       
  0x0020E8A4  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E8A8  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E8AC  c1fa02                  sar      edx, 2                         
  0x0020E8AF  88561b                  mov      byte ptr [esi + 0x1b], dl      
  0x0020E8B2  0fb65805                movzx    ebx, byte ptr [eax + 5]        
  0x0020E8B6  0fb65104                movzx    edx, byte ptr [ecx + 4]        
  0x0020E8BA  03d3                    add      edx, ebx                       
  0x0020E8BC  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E8C0  03d3                    add      edx, ebx                       
  0x0020E8C2  0fb65804                movzx    ebx, byte ptr [eax + 4]        
  0x0020E8C6  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E8CA  c1fa02                  sar      edx, 2                         
  0x0020E8CD  88561c                  mov      byte ptr [esi + 0x1c], dl      
  0x0020E8D0  0fb65905                movzx    ebx, byte ptr [ecx + 5]        
  0x0020E8D4  0fb65005                movzx    edx, byte ptr [eax + 5]        
  0x0020E8D8  03d3                    add      edx, ebx                       
  0x0020E8DA  0fb65806                movzx    ebx, byte ptr [eax + 6]        
  0x0020E8DE  03d3                    add      edx, ebx                       
  0x0020E8E0  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E8E4  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E8E8  c1fa02                  sar      edx, 2                         
  0x0020E8EB  88561d                  mov      byte ptr [esi + 0x1d], dl      
  0x0020E8EE  0fb65906                movzx    ebx, byte ptr [ecx + 6]        
  0x0020E8F2  0fb65006                movzx    edx, byte ptr [eax + 6]        
  0x0020E8F6  03d3                    add      edx, ebx                       
  0x0020E8F8  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E8FC  03d3                    add      edx, ebx                       
  0x0020E8FE  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E902  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E906  c1fa02                  sar      edx, 2                         
  0x0020E909  88561e                  mov      byte ptr [esi + 0x1e], dl      
  0x0020E90C  0fb65808                movzx    ebx, byte ptr [eax + 8]        
  0x0020E910  0fb65108                movzx    edx, byte ptr [ecx + 8]        
  0x0020E914  03d3                    add      edx, ebx                       
  0x0020E916  0fb65807                movzx    ebx, byte ptr [eax + 7]        
  0x0020E91A  03d3                    add      edx, ebx                       
  0x0020E91C  0fb65907                movzx    ebx, byte ptr [ecx + 7]        
  0x0020E920  8d541a02                lea      edx, [edx + ebx + 2]           
  0x0020E924  c1fa02                  sar      edx, 2                         
  0x0020E927  88561f                  mov      byte ptr [esi + 0x1f], dl      
  0x0020E92A  03c7                    add      eax, edi                       
  0x0020E92C  03cf                    add      ecx, edi                       
  0x0020E92E  83c620                  add      esi, 0x20                      
  0x0020E931  4d                      dec      ebp                            
  0x0020E932  0f8528fcffff            jne      0x20e560                       
  0x0020E938  5f                      pop      edi                            
  0x0020E939  5e                      pop      esi                            
  0x0020E93A  5d                      pop      ebp                            
  0x0020E93B  5b                      pop      ebx                            
  0x0020E93C  c3                      ret                                     
  0x0020E93D  90                      nop                                     
  0x0020E93E  90                      nop                                     
  0x0020E93F  90                      nop                                     
  0x0020E940  83ec0c                  sub      esp, 0xc                       
  0x0020E943  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x0020E947  8b4810                  mov      ecx, dword ptr [eax + 0x10]    
  0x0020E94A  8b5014                  mov      edx, dword ptr [eax + 0x14]    
  0x0020E94D  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020E951  8b4818                  mov      ecx, dword ptr [eax + 0x18]    
  0x0020E954  89542408                mov      dword ptr [esp + 8], edx       
  0x0020E958  8b5008                  mov      edx, dword ptr [eax + 8]       
  0x0020E95B  53                      push     ebx                            
  0x0020E95C  894c2408                mov      dword ptr [esp + 8], ecx       
  0x0020E960  89542404                mov      dword ptr [esp + 4], edx       
  0x0020E964  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x0020E968  8b5c2408                mov      ebx, dword ptr [esp + 8]       
  0x0020E96C  8b4c2404                mov      ecx, dword ptr [esp + 4]       
  0x0020E970  8b542414                mov      edx, dword ptr [esp + 0x14]    
  0x0020E974  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E977  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E97A  0f7f01                  movq     qword ptr [ecx], mm0           
  0x0020E97D  03c2                    add      eax, edx                       
  0x0020E97F  03da                    add      ebx, edx                       
  0x0020E981  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E984  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E987  0f7f4108                movq     qword ptr [ecx + 8], mm0       
  0x0020E98B  03c2                    add      eax, edx                       
  0x0020E98D  03da                    add      ebx, edx                       
  0x0020E98F  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E992  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E995  0f7f4110                movq     qword ptr [ecx + 0x10], mm0    
  0x0020E999  03c2                    add      eax, edx                       
  0x0020E99B  03da                    add      ebx, edx                       
  0x0020E99D  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E9A0  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E9A3  0f7f4118                movq     qword ptr [ecx + 0x18], mm0    
  0x0020E9A7  03c2                    add      eax, edx                       
  0x0020E9A9  03da                    add      ebx, edx                       
  0x0020E9AB  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E9AE  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E9B1  0f7f4120                movq     qword ptr [ecx + 0x20], mm0    
  0x0020E9B5  03c2                    add      eax, edx                       
  0x0020E9B7  03da                    add      ebx, edx                       
  0x0020E9B9  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E9BC  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E9BF  0f7f4128                movq     qword ptr [ecx + 0x28], mm0    
  0x0020E9C3  03c2                    add      eax, edx                       
  0x0020E9C5  03da                    add      ebx, edx                       
  0x0020E9C7  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E9CA  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E9CD  0f7f4130                movq     qword ptr [ecx + 0x30], mm0    
  0x0020E9D1  03c2                    add      eax, edx                       
  0x0020E9D3  03da                    add      ebx, edx                       
  0x0020E9D5  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020E9D8  0fe003                  pavgb    mm0, qword ptr [ebx]           
  0x0020E9DB  0f7f4138                movq     qword ptr [ecx + 0x38], mm0    
  0x0020E9DF  0f77                    emms                                    
  0x0020E9E1  5b                      pop      ebx                            
  0x0020E9E2  83c40c                  add      esp, 0xc                       
  0x0020E9E5  c3                      ret                                     
  0x0020E9E6  90                      nop                                     
  0x0020E9E7  90                      nop                                     
  0x0020E9E8  90                      nop                                     
  0x0020E9E9  90                      nop                                     
  0x0020E9EA  90                      nop                                     
  0x0020E9EB  90                      nop                                     
  0x0020E9EC  90                      nop                                     
  0x0020E9ED  90                      nop                                     
  0x0020E9EE  90                      nop                                     
  0x0020E9EF  90                      nop                                     
  0x0020E9F0  83ec08                  sub      esp, 8                         
  0x0020E9F3  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x0020E9F7  8b4810                  mov      ecx, dword ptr [eax + 0x10]    
  0x0020E9FA  8b5014                  mov      edx, dword ptr [eax + 0x14]    
  0x0020E9FD  8b4008                  mov      eax, dword ptr [eax + 8]       
  0x0020EA00  894c240c                mov      dword ptr [esp + 0xc], ecx     
  0x0020EA04  89542404                mov      dword ptr [esp + 4], edx       
  0x0020EA08  89442400                mov      dword ptr [esp], eax           
  0x0020EA0C  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0020EA10  8b4c2400                mov      ecx, dword ptr [esp]           
  0x0020EA14  8b54240c                mov      edx, dword ptr [esp + 0xc]     
  0x0020EA18  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA1B  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA1F  0f7f01                  movq     qword ptr [ecx], mm0           
  0x0020EA22  03c2                    add      eax, edx                       
  0x0020EA24  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA27  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA2B  0f7f4108                movq     qword ptr [ecx + 8], mm0       
  0x0020EA2F  03c2                    add      eax, edx                       
  0x0020EA31  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA34  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA38  0f7f4110                movq     qword ptr [ecx + 0x10], mm0    
  0x0020EA3C  03c2                    add      eax, edx                       
  0x0020EA3E  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA41  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA45  0f7f4118                movq     qword ptr [ecx + 0x18], mm0    
  0x0020EA49  03c2                    add      eax, edx                       
  0x0020EA4B  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA4E  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA52  0f7f4120                movq     qword ptr [ecx + 0x20], mm0    
  0x0020EA56  03c2                    add      eax, edx                       
  0x0020EA58  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA5B  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA5F  0f7f4128                movq     qword ptr [ecx + 0x28], mm0    
  0x0020EA63  03c2                    add      eax, edx                       
  0x0020EA65  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA68  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA6C  0f7f4130                movq     qword ptr [ecx + 0x30], mm0    
  0x0020EA70  03c2                    add      eax, edx                       
  0x0020EA72  0f6f00                  movq     mm0, qword ptr [eax]           
  0x0020EA75  0fe04001                pavgb    mm0, qword ptr [eax + 1]       
  0x0020EA79  0f7f4138                movq     qword ptr [ecx + 0x38], mm0    
  0x0020EA7D  0f77                    emms                                    
  0x0020EA7F  83c408                  add      esp, 8                         
  0x0020EA82  c3                      ret                                     
  0x0020EA83  90                      nop                                     
  0x0020EA84  90                      nop                                     
  0x0020EA85  90                      nop                                     
  0x0020EA86  90                      nop                                     
  0x0020EA87  90                      nop                                     
  0x0020EA88  90                      nop                                     
  0x0020EA89  90                      nop                                     
  0x0020EA8A  90                      nop                                     
  0x0020EA8B  90                      nop                                     
  0x0020EA8C  90                      nop                                     
  0x0020EA8D  90                      nop                                     
  0x0020EA8E  90                      nop                                     
  0x0020EA8F  90                      nop                                     
  0x0020EA90  83ec08                  sub      esp, 8                         
  0x0020EA93  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x0020EA97  8b4814                  mov      ecx, dword ptr [eax + 0x14]    
  0x0020EA9A  8b5008                  mov      edx, dword ptr [eax + 8]       
  0x0020EA9D  8b4010                  mov      eax, dword ptr [eax + 0x10]    
  0x0020EAA0  56                      push     esi                            
  0x0020EAA1  57                      push     edi                            
  0x0020EAA2  894c240c                mov      dword ptr [esp + 0xc], ecx     
  0x0020EAA6  89542414                mov      dword ptr [esp + 0x14], edx    
  0x0020EAAA  89442408                mov      dword ptr [esp + 8], eax       
  0x0020EAAE  8b74240c                mov      esi, dword ptr [esp + 0xc]     
  0x0020EAB2  8b442408                mov      eax, dword ptr [esp + 8]       
  0x0020EAB6  8b7c2414                mov      edi, dword ptr [esp + 0x14]    
  0x0020EABA  0f6f06                  movq     mm0, qword ptr [esi]           
  0x0020EABD  0f7f07                  movq     qword ptr [edi], mm0           
  0x0020EAC0  03f0                    add      esi, eax                       
  0x0020EAC2  83c708                  add      edi, 8                         
  0x0020EAC5  0f6f0e                  movq     mm1, qword ptr [esi]           
  0x0020EAC8  0f7f0f                  movq     qword ptr [edi], mm1           
  0x0020EACB  03f0                    add      esi, eax                       
  0x0020EACD  83c708                  add      edi, 8                         
  0x0020EAD0  0f6f16                  movq     mm2, qword ptr [esi]           
  0x0020EAD3  0f7f17                  movq     qword ptr [edi], mm2           
  0x0020EAD6  03f0                    add      esi, eax                       
  0x0020EAD8  83c708                  add      edi, 8                         
  0x0020EADB  0f6f1e                  movq     mm3, qword ptr [esi]           
  0x0020EADE  0f7f1f                  movq     qword ptr [edi], mm3           
  0x0020EAE1  03f0                    add      esi, eax                       
  0x0020EAE3  83c708                  add      edi, 8                         
  0x0020EAE6  0f6f26                  movq     mm4, qword ptr [esi]           
  0x0020EAE9  0f7f27                  movq     qword ptr [edi], mm4           
  0x0020EAEC  03f0                    add      esi, eax                       
  0x0020EAEE  83c708                  add      edi, 8                         
  0x0020EAF1  0f6f2e                  movq     mm5, qword ptr [esi]           
  0x0020EAF4  0f7f2f                  movq     qword ptr [edi], mm5           
  0x0020EAF7  03f0                    add      esi, eax                       
  0x0020EAF9  83c708                  add      edi, 8                         
  0x0020EAFC  0f6f36                  movq     mm6, qword ptr [esi]           
  0x0020EAFF  0f7f37                  movq     qword ptr [edi], mm6           
  0x0020EB02  03f0                    add      esi, eax                       
  0x0020EB04  83c708                  add      edi, 8                         
  0x0020EB07  0f6f3e                  movq     mm7, qword ptr [esi]           
  0x0020EB0A  0f7f3f                  movq     qword ptr [edi], mm7           
  0x0020EB0D  0f77                    emms                                    
  0x0020EB0F  5f                      pop      edi                            
  0x0020EB10  5e                      pop      esi                            
  0x0020EB11  83c408                  add      esp, 8                         
  0x0020EB14  c3                      ret                                     
  0x0020EB15  90                      nop                                     
  0x0020EB16  90                      nop                                     
  0x0020EB17  90                      nop                                     
  0x0020EB18  90                      nop                                     
  0x0020EB19  90                      nop                                     
  0x0020EB1A  90                      nop                                     
  0x0020EB1B  90                      nop                                     
  0x0020EB1C  90                      nop                                     
  0x0020EB1D  90                      nop                                     
  0x0020EB1E  90                      nop                                     
  0x0020EB1F  90                      nop                                     

; ============================================================
; Function: sub_0020EB20
; Start: 0x0020EB20  End: 0x0020EB4C  Size: 44 bytes
; Detection: prologue (confidence: 0.95)
; ============================================================
sub_0020EB20:
  0x0020EB20  55                      push     ebp                            
  0x0020EB21  8bec                    mov      ebp, esp                       
  0x0020EB23  83e4f8                  and      esp, 0xfffffff8                
  0x0020EB26  83ec1c                  sub      esp, 0x1c                      
  0x0020EB29  8b4510                  mov      eax, dword ptr [ebp + 0x10]    
  0x0020EB2C  8b08                    mov      ecx, dword ptr [eax]           
  0x0020EB2E  8b5514                  mov      edx, dword ptr [ebp + 0x14]    
  0x0020EB31  53                      push     ebx                            
  0x0020EB32  56                      push     esi                            
  0x0020EB33  894c2418                mov      dword ptr [esp + 0x18], ecx    
  0x0020EB37  b906000000              mov      ecx, 6                         
  0x0020EB3C  57                      push     edi                            
  0x0020EB3D  8b7d0c                  mov      edi, dword ptr [ebp + 0xc]     
  0x0020EB40  8d7004                  lea      esi, [eax + 4]                 
  0x0020EB43  8b4508                  mov      eax, dword ptr [ebp + 8]       
  0x0020EB46  894c2418                mov      dword ptr [esp + 0x18], ecx    
  0x0020EB4A  eb04                    jmp      0x20eb50                       
; end of function
                                        ; XREF: 0x0020EC8D (cond_jump)
  0x0020EB4C  8b742420                mov      esi, dword ptr [esp + 0x20]    
                                        ; XREF: 0x0020EB4A (jump)
  0x0020EB50  3b4c241c                cmp      ecx, dword ptr [esp + 0x1c]    
  0x0020EB54  7503                    jne      0x20eb59                       
  0x0020EB56  83ea10                  sub      edx, 0x10                      
                                        ; XREF: 0x0020EB54 (cond_jump)
  0x0020EB59  8b0e                    mov      ecx, dword ptr [esi]           
  0x0020EB5B  8b5e04                  mov      ebx, dword ptr [esi + 4]       
  0x0020EB5E  0f1809                  prefetcht0 byte ptr [ecx]                 
  0x0020EB61  83c604                  add      esi, 4                         
  0x0020EB64  83c604                  add      esi, 4                         
  0x0020EB67  89742420                mov      dword ptr [esp + 0x20], esi    
  0x0020EB6B  895c2414                mov      dword ptr [esp + 0x14], ebx    
  0x0020EB6F  8bf1                    mov      esi, ecx                       
  0x0020EB71  c744241008000000        mov      dword ptr [esp + 0x10], 8      
  0x0020EB79  8da42400000000          lea      esp, [esp]                     
                                        ; XREF: 0x0020EC01 (cond_jump)
  0x0020EB80  8b5c2414                mov      ebx, dword ptr [esp + 0x14]    
  0x0020EB84  0f180c0b                prefetcht0 byte ptr [ebx + ecx]           
  0x0020EB88  03d9                    add      ebx, ecx                       
  0x0020EB8A  895c2424                mov      dword ptr [esp + 0x24], ebx    
  0x0020EB8E  0fbf18                  movsx    ebx, word ptr [eax]            
  0x0020EB91  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EB94  8819                    mov      byte ptr [ecx], bl             
  0x0020EB96  0fbf5802                movsx    ebx, word ptr [eax + 2]        
  0x0020EB9A  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EB9D  83c002                  add      eax, 2                         
  0x0020EBA0  885901                  mov      byte ptr [ecx + 1], bl         
  0x0020EBA3  0fbf5802                movsx    ebx, word ptr [eax + 2]        
  0x0020EBA7  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EBAA  83c002                  add      eax, 2                         
  0x0020EBAD  885902                  mov      byte ptr [ecx + 2], bl         
  0x0020EBB0  0fbf5802                movsx    ebx, word ptr [eax + 2]        
  0x0020EBB4  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EBB7  83c002                  add      eax, 2                         
  0x0020EBBA  885903                  mov      byte ptr [ecx + 3], bl         
  0x0020EBBD  0fbf5802                movsx    ebx, word ptr [eax + 2]        
  0x0020EBC1  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EBC4  83c002                  add      eax, 2                         
  0x0020EBC7  885904                  mov      byte ptr [ecx + 4], bl         
  0x0020EBCA  0fbf5802                movsx    ebx, word ptr [eax + 2]        
  0x0020EBCE  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EBD1  83c002                  add      eax, 2                         
  0x0020EBD4  885905                  mov      byte ptr [ecx + 5], bl         
  0x0020EBD7  0fbf5802                movsx    ebx, word ptr [eax + 2]        
  0x0020EBDB  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EBDE  83c002                  add      eax, 2                         
  0x0020EBE1  885906                  mov      byte ptr [ecx + 6], bl         
  0x0020EBE4  0fbf5802                movsx    ebx, word ptr [eax + 2]        
  0x0020EBE8  8a1c13                  mov      bl, byte ptr [ebx + edx]       
  0x0020EBEB  83c002                  add      eax, 2                         
  0x0020EBEE  885907                  mov      byte ptr [ecx + 7], bl         
  0x0020EBF1  8b5c2410                mov      ebx, dword ptr [esp + 0x10]    
  0x0020EBF5  8b4c2424                mov      ecx, dword ptr [esp + 0x24]    
  0x0020EBF9  83c002                  add      eax, 2                         
  0x0020EBFC  4b                      dec      ebx                            
  0x0020EBFD  895c2410                mov      dword ptr [esp + 0x10], ebx    
  0x0020EC01  0f8579ffffff            jne      0x20eb80                       
  0x0020EC07  8b0e                    mov      ecx, dword ptr [esi]           
  0x0020EC09  890f                    mov      dword ptr [edi], ecx           
  0x0020EC0B  8b4e04                  mov      ecx, dword ptr [esi + 4]       
  0x0020EC0E  894f04                  mov      dword ptr [edi + 4], ecx       
  0x0020EC11  8b4c2414                mov      ecx, dword ptr [esp + 0x14]    
  0x0020EC15  c1e903                  shr      ecx, 3                         
  0x0020EC18  c1e103                  shl      ecx, 3                         
  0x0020EC1B  8b1c0e                  mov      ebx, dword ptr [esi + ecx]     
  0x0020EC1E  895f08                  mov      dword ptr [edi + 8], ebx       
  0x0020EC21  8b5c0e04                mov      ebx, dword ptr [esi + ecx + 4] 
  0x0020EC25  895f0c                  mov      dword ptr [edi + 0xc], ebx     
  0x0020EC28  03f1                    add      esi, ecx                       
  0x0020EC2A  8b1c0e                  mov      ebx, dword ptr [esi + ecx]     
  0x0020EC2D  895f10                  mov      dword ptr [edi + 0x10], ebx    
  0x0020EC30  8b5c0e04                mov      ebx, dword ptr [esi + ecx + 4] 
  0x0020EC34  895f14                  mov      dword ptr [edi + 0x14], ebx    
  0x0020EC37  03f1                    add      esi, ecx                       
  0x0020EC39  8b1c0e                  mov      ebx, dword ptr [esi + ecx]     
  0x0020EC3C  895f18                  mov      dword ptr [edi + 0x18], ebx    
  0x0020EC3F  8b5c0e04                mov      ebx, dword ptr [esi + ecx + 4] 
  0x0020EC43  895f1c                  mov      dword ptr [edi + 0x1c], ebx    
  0x0020EC46  03f1                    add      esi, ecx                       
  0x0020EC48  8b1c0e                  mov      ebx, dword ptr [esi + ecx]     
  0x0020EC4B  83c720                  add      edi, 0x20                      
  0x0020EC4E  03f1                    add      esi, ecx                       
  0x0020EC50  891f                    mov      dword ptr [edi], ebx           
  0x0020EC52  8b5e04                  mov      ebx, dword ptr [esi + 4]       
  0x0020EC55  03f1                    add      esi, ecx                       
  0x0020EC57  895f04                  mov      dword ptr [edi + 4], ebx       
  0x0020EC5A  8b1e                    mov      ebx, dword ptr [esi]           
  0x0020EC5C  895f08                  mov      dword ptr [edi + 8], ebx       
  0x0020EC5F  8b5e04                  mov      ebx, dword ptr [esi + 4]       
  0x0020EC62  03f1                    add      esi, ecx                       
  0x0020EC64  895f0c                  mov      dword ptr [edi + 0xc], ebx     
  0x0020EC67  8b1e                    mov      ebx, dword ptr [esi]           
  0x0020EC69  895f10                  mov      dword ptr [edi + 0x10], ebx    
  0x0020EC6C  8b5e04                  mov      ebx, dword ptr [esi + 4]       
  0x0020EC6F  895f14                  mov      dword ptr [edi + 0x14], ebx    
  0x0020EC72  8b1c31                  mov      ebx, dword ptr [ecx + esi]     
  0x0020EC75  895f18                  mov      dword ptr [edi + 0x18], ebx    
  0x0020EC78  8b4c3104                mov      ecx, dword ptr [ecx + esi + 4] 
  0x0020EC7C  894f1c                  mov      dword ptr [edi + 0x1c], ecx    
  0x0020EC7F  8b4c2418                mov      ecx, dword ptr [esp + 0x18]    
  0x0020EC83  83c720                  add      edi, 0x20                      
  0x0020EC86  49                      dec      ecx                            
  0x0020EC87  85c9                    test     ecx, ecx                       
  0x0020EC89  894c2418                mov      dword ptr [esp + 0x18], ecx    
  0x0020EC8D  0f8fb9feffff            jg       0x20eb4c                       
  0x0020EC93  5f                      pop      edi                            
  0x0020EC94  5e                      pop      esi                            
  0x0020EC95  5b                      pop      ebx                            
  0x0020EC96  8be5                    mov      esp, ebp                       
  0x0020EC98  5d                      pop      ebp                            
  0x0020EC99  c3                      ret                                     
  0x0020EC9A  90                      nop                                     
  0x0020EC9B  90                      nop                                     
  0x0020EC9C  90                      nop                                     
  0x0020EC9D  90                      nop                                     
  0x0020EC9E  90                      nop                                     
  0x0020EC9F  90                      nop                                     

; ============================================================
; Function: sub_0020ECA0
; Start: 0x0020ECA0  End: 0x0020ECD3  Size: 51 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020ECA0:
  0x0020ECA0  8b4c2408                mov      ecx, dword ptr [esp + 8]       
  0x0020ECA4  8b442404                mov      eax, dword ptr [esp + 4]       
  0x0020ECA8  8b5104                  mov      edx, dword ptr [ecx + 4]       
  0x0020ECAB  53                      push     ebx                            
  0x0020ECAC  55                      push     ebp                            
  0x0020ECAD  8b29                    mov      ebp, dword ptr [ecx]           
  0x0020ECAF  56                      push     esi                            
  0x0020ECB0  89442410                mov      dword ptr [esp + 0x10], eax    
  0x0020ECB4  8b4108                  mov      eax, dword ptr [ecx + 8]       
  0x0020ECB7  57                      push     edi                            
  0x0020ECB8  c744241806000000        mov      dword ptr [esp + 0x18], 6      
                                        ; XREF: 0x0020EF58 (cond_jump)
  0x0020ECC0  8b4c241c                mov      ecx, dword ptr [esp + 0x1c]    
  0x0020ECC4  85c9                    test     ecx, ecx                       
  0x0020ECC6  7c0b                    jl       0x20ecd3                       
  0x0020ECC8  81c280000000            add      edx, 0x80                      
  0x0020ECCE  e95f020000              jmp      0x20ef32                       
; end of function
                                        ; XREF: 0x0020ECC6 (cond_jump)
  0x0020ECD3  be02000000              mov      esi, 2                         
                                        ; XREF: 0x0020EF29 (cond_jump)
  0x0020ECD8  0fbf3a                  movsx    edi, word ptr [edx]            
  0x0020ECDB  0fb608                  movzx    ecx, byte ptr [eax]            
  0x0020ECDE  8bdd                    mov      ebx, ebp                       
  0x0020ECE0  03d9                    add      ebx, ecx                       
  0x0020ECE2  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ECE5  0fb67801                movzx    edi, byte ptr [eax + 1]        
  0x0020ECE9  8808                    mov      byte ptr [eax], cl             
  0x0020ECEB  0fbf4a02                movsx    ecx, word ptr [edx + 2]        
  0x0020ECEF  8bdd                    mov      ebx, ebp                       
  0x0020ECF1  03d9                    add      ebx, ecx                       
  0x0020ECF3  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ECF6  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020ECFA  884801                  mov      byte ptr [eax + 1], cl         
  0x0020ECFD  0fbf4a04                movsx    ecx, word ptr [edx + 4]        
  0x0020ED01  8bdd                    mov      ebx, ebp                       
  0x0020ED03  03d9                    add      ebx, ecx                       
  0x0020ED05  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED08  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020ED0C  884802                  mov      byte ptr [eax + 2], cl         
  0x0020ED0F  0fbf4a06                movsx    ecx, word ptr [edx + 6]        
  0x0020ED13  8bdd                    mov      ebx, ebp                       
  0x0020ED15  03d9                    add      ebx, ecx                       
  0x0020ED17  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED1A  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020ED1E  884803                  mov      byte ptr [eax + 3], cl         
  0x0020ED21  0fbf4a08                movsx    ecx, word ptr [edx + 8]        
  0x0020ED25  8bdd                    mov      ebx, ebp                       
  0x0020ED27  03d9                    add      ebx, ecx                       
  0x0020ED29  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED2C  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020ED30  884804                  mov      byte ptr [eax + 4], cl         
  0x0020ED33  0fbf4a0a                movsx    ecx, word ptr [edx + 0xa]      
  0x0020ED37  8bdd                    mov      ebx, ebp                       
  0x0020ED39  03d9                    add      ebx, ecx                       
  0x0020ED3B  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED3E  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020ED42  884805                  mov      byte ptr [eax + 5], cl         
  0x0020ED45  0fbf4a0c                movsx    ecx, word ptr [edx + 0xc]      
  0x0020ED49  8bdd                    mov      ebx, ebp                       
  0x0020ED4B  03d9                    add      ebx, ecx                       
  0x0020ED4D  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED50  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x0020ED54  884806                  mov      byte ptr [eax + 6], cl         
  0x0020ED57  0fbf4a0e                movsx    ecx, word ptr [edx + 0xe]      
  0x0020ED5B  83c008                  add      eax, 8                         
  0x0020ED5E  8bdd                    mov      ebx, ebp                       
  0x0020ED60  03d9                    add      ebx, ecx                       
  0x0020ED62  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED65  8848ff                  mov      byte ptr [eax - 1], cl         
  0x0020ED68  0fbf7a10                movsx    edi, word ptr [edx + 0x10]     
  0x0020ED6C  0fb608                  movzx    ecx, byte ptr [eax]            
  0x0020ED6F  83c210                  add      edx, 0x10                      
  0x0020ED72  8bdd                    mov      ebx, ebp                       
  0x0020ED74  03d9                    add      ebx, ecx                       
  0x0020ED76  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED79  0fb67801                movzx    edi, byte ptr [eax + 1]        
  0x0020ED7D  8808                    mov      byte ptr [eax], cl             
  0x0020ED7F  0fbf4a02                movsx    ecx, word ptr [edx + 2]        
  0x0020ED83  8bdd                    mov      ebx, ebp                       
  0x0020ED85  03d9                    add      ebx, ecx                       
  0x0020ED87  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED8A  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020ED8E  884801                  mov      byte ptr [eax + 1], cl         
  0x0020ED91  0fbf4a04                movsx    ecx, word ptr [edx + 4]        
  0x0020ED95  8bdd                    mov      ebx, ebp                       
  0x0020ED97  03d9                    add      ebx, ecx                       
  0x0020ED99  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020ED9C  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020EDA0  884802                  mov      byte ptr [eax + 2], cl         
  0x0020EDA3  0fbf4a06                movsx    ecx, word ptr [edx + 6]        
  0x0020EDA7  8bdd                    mov      ebx, ebp                       
  0x0020EDA9  03d9                    add      ebx, ecx                       
  0x0020EDAB  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EDAE  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020EDB2  884803                  mov      byte ptr [eax + 3], cl         
  0x0020EDB5  0fbf4a08                movsx    ecx, word ptr [edx + 8]        
  0x0020EDB9  8bdd                    mov      ebx, ebp                       
  0x0020EDBB  03d9                    add      ebx, ecx                       
  0x0020EDBD  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EDC0  884804                  mov      byte ptr [eax + 4], cl         
  0x0020EDC3  0fbf4a0a                movsx    ecx, word ptr [edx + 0xa]      
  0x0020EDC7  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020EDCB  8bdd                    mov      ebx, ebp                       
  0x0020EDCD  03d9                    add      ebx, ecx                       
  0x0020EDCF  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EDD2  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020EDD6  884805                  mov      byte ptr [eax + 5], cl         
  0x0020EDD9  0fbf4a0c                movsx    ecx, word ptr [edx + 0xc]      
  0x0020EDDD  83c008                  add      eax, 8                         
  0x0020EDE0  8bdd                    mov      ebx, ebp                       
  0x0020EDE2  03d9                    add      ebx, ecx                       
  0x0020EDE4  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EDE7  0fb678ff                movzx    edi, byte ptr [eax - 1]        
  0x0020EDEB  8848fe                  mov      byte ptr [eax - 2], cl         
  0x0020EDEE  0fbf4a0e                movsx    ecx, word ptr [edx + 0xe]      
  0x0020EDF2  8bdd                    mov      ebx, ebp                       
  0x0020EDF4  03d9                    add      ebx, ecx                       
  0x0020EDF6  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EDF9  8848ff                  mov      byte ptr [eax - 1], cl         
  0x0020EDFC  0fbf7a10                movsx    edi, word ptr [edx + 0x10]     
  0x0020EE00  0fb608                  movzx    ecx, byte ptr [eax]            
  0x0020EE03  83c210                  add      edx, 0x10                      
  0x0020EE06  8bdd                    mov      ebx, ebp                       
  0x0020EE08  03d9                    add      ebx, ecx                       
  0x0020EE0A  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE0D  0fb67801                movzx    edi, byte ptr [eax + 1]        
  0x0020EE11  8808                    mov      byte ptr [eax], cl             
  0x0020EE13  0fbf4a02                movsx    ecx, word ptr [edx + 2]        
  0x0020EE17  8bdd                    mov      ebx, ebp                       
  0x0020EE19  03d9                    add      ebx, ecx                       
  0x0020EE1B  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE1E  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020EE22  884801                  mov      byte ptr [eax + 1], cl         
  0x0020EE25  0fbf4a04                movsx    ecx, word ptr [edx + 4]        
  0x0020EE29  8bdd                    mov      ebx, ebp                       
  0x0020EE2B  03d9                    add      ebx, ecx                       
  0x0020EE2D  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE30  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020EE34  884802                  mov      byte ptr [eax + 2], cl         
  0x0020EE37  0fbf4a06                movsx    ecx, word ptr [edx + 6]        
  0x0020EE3B  8bdd                    mov      ebx, ebp                       
  0x0020EE3D  03d9                    add      ebx, ecx                       
  0x0020EE3F  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE42  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020EE46  884803                  mov      byte ptr [eax + 3], cl         
  0x0020EE49  0fbf4a08                movsx    ecx, word ptr [edx + 8]        
  0x0020EE4D  8bdd                    mov      ebx, ebp                       
  0x0020EE4F  03d9                    add      ebx, ecx                       
  0x0020EE51  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE54  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020EE58  884804                  mov      byte ptr [eax + 4], cl         
  0x0020EE5B  0fbf4a0a                movsx    ecx, word ptr [edx + 0xa]      
  0x0020EE5F  8bdd                    mov      ebx, ebp                       
  0x0020EE61  03d9                    add      ebx, ecx                       
  0x0020EE63  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE66  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020EE6A  884805                  mov      byte ptr [eax + 5], cl         
  0x0020EE6D  0fbf4a0c                movsx    ecx, word ptr [edx + 0xc]      
  0x0020EE71  8bdd                    mov      ebx, ebp                       
  0x0020EE73  03d9                    add      ebx, ecx                       
  0x0020EE75  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE78  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x0020EE7C  884806                  mov      byte ptr [eax + 6], cl         
  0x0020EE7F  0fbf4a0e                movsx    ecx, word ptr [edx + 0xe]      
  0x0020EE83  8bdd                    mov      ebx, ebp                       
  0x0020EE85  03d9                    add      ebx, ecx                       
  0x0020EE87  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EE8A  884807                  mov      byte ptr [eax + 7], cl         
  0x0020EE8D  0fb64808                movzx    ecx, byte ptr [eax + 8]        
  0x0020EE91  0fbf7a10                movsx    edi, word ptr [edx + 0x10]     
  0x0020EE95  83c008                  add      eax, 8                         
  0x0020EE98  83c210                  add      edx, 0x10                      
  0x0020EE9B  8bdd                    mov      ebx, ebp                       
  0x0020EE9D  03d9                    add      ebx, ecx                       
  0x0020EE9F  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EEA2  0fb67801                movzx    edi, byte ptr [eax + 1]        
  0x0020EEA6  8808                    mov      byte ptr [eax], cl             
  0x0020EEA8  0fbf4a02                movsx    ecx, word ptr [edx + 2]        
  0x0020EEAC  8bdd                    mov      ebx, ebp                       
  0x0020EEAE  03d9                    add      ebx, ecx                       
  0x0020EEB0  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EEB3  884801                  mov      byte ptr [eax + 1], cl         
  0x0020EEB6  0fbf4a04                movsx    ecx, word ptr [edx + 4]        
  0x0020EEBA  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020EEBE  8bdd                    mov      ebx, ebp                       
  0x0020EEC0  03d9                    add      ebx, ecx                       
  0x0020EEC2  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EEC5  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020EEC9  884802                  mov      byte ptr [eax + 2], cl         
  0x0020EECC  0fbf4a06                movsx    ecx, word ptr [edx + 6]        
  0x0020EED0  8bdd                    mov      ebx, ebp                       
  0x0020EED2  03d9                    add      ebx, ecx                       
  0x0020EED4  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EED7  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020EEDB  884803                  mov      byte ptr [eax + 3], cl         
  0x0020EEDE  0fbf4a08                movsx    ecx, word ptr [edx + 8]        
  0x0020EEE2  8bdd                    mov      ebx, ebp                       
  0x0020EEE4  03d9                    add      ebx, ecx                       
  0x0020EEE6  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EEE9  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020EEED  884804                  mov      byte ptr [eax + 4], cl         
  0x0020EEF0  0fbf4a0a                movsx    ecx, word ptr [edx + 0xa]      
  0x0020EEF4  8bdd                    mov      ebx, ebp                       
  0x0020EEF6  03d9                    add      ebx, ecx                       
  0x0020EEF8  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EEFB  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020EEFF  884805                  mov      byte ptr [eax + 5], cl         
  0x0020EF02  0fbf4a0c                movsx    ecx, word ptr [edx + 0xc]      
  0x0020EF06  8bdd                    mov      ebx, ebp                       
  0x0020EF08  03d9                    add      ebx, ecx                       
  0x0020EF0A  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EF0D  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x0020EF11  884806                  mov      byte ptr [eax + 6], cl         
  0x0020EF14  0fbf4a0e                movsx    ecx, word ptr [edx + 0xe]      
  0x0020EF18  8bdd                    mov      ebx, ebp                       
  0x0020EF1A  03d9                    add      ebx, ecx                       
  0x0020EF1C  8a0c1f                  mov      cl, byte ptr [edi + ebx]       
  0x0020EF1F  884807                  mov      byte ptr [eax + 7], cl         
  0x0020EF22  83c008                  add      eax, 8                         
  0x0020EF25  83c210                  add      edx, 0x10                      
  0x0020EF28  4e                      dec      esi                            
  0x0020EF29  0f85a9fdffff            jne      0x20ecd8                       
  0x0020EF2F  83e840                  sub      eax, 0x40                      
                                        ; XREF: 0x0020ECCE (jump)
  0x0020EF32  d164241c                shl      dword ptr [esp + 0x1c], 1      
  0x0020EF36  8b5c2414                mov      ebx, dword ptr [esp + 0x14]    
  0x0020EF3A  8bf0                    mov      esi, eax                       
  0x0020EF3C  8bfb                    mov      edi, ebx                       
  0x0020EF3E  b910000000              mov      ecx, 0x10                      
  0x0020EF43  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x0020EF45  8b4c2418                mov      ecx, dword ptr [esp + 0x18]    
  0x0020EF49  83c340                  add      ebx, 0x40                      
  0x0020EF4C  83c040                  add      eax, 0x40                      
  0x0020EF4F  49                      dec      ecx                            
  0x0020EF50  895c2414                mov      dword ptr [esp + 0x14], ebx    
  0x0020EF54  894c2418                mov      dword ptr [esp + 0x18], ecx    
  0x0020EF58  0f8562fdffff            jne      0x20ecc0                       
  0x0020EF5E  5f                      pop      edi                            
  0x0020EF5F  5e                      pop      esi                            
  0x0020EF60  5d                      pop      ebp                            
  0x0020EF61  5b                      pop      ebx                            
  0x0020EF62  c3                      ret                                     
  0x0020EF63  90                      nop                                     
  0x0020EF64  90                      nop                                     
  0x0020EF65  90                      nop                                     
  0x0020EF66  90                      nop                                     
  0x0020EF67  90                      nop                                     
  0x0020EF68  90                      nop                                     
  0x0020EF69  90                      nop                                     
  0x0020EF6A  90                      nop                                     
  0x0020EF6B  90                      nop                                     
  0x0020EF6C  90                      nop                                     
  0x0020EF6D  90                      nop                                     
  0x0020EF6E  90                      nop                                     
  0x0020EF6F  90                      nop                                     

; ============================================================
; Function: sub_0020EF70
; Start: 0x0020EF70  End: 0x0020F1EF  Size: 639 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020EF70:
  0x0020EF70  51                      push     ecx                            
  0x0020EF71  8b4c240c                mov      ecx, dword ptr [esp + 0xc]     
  0x0020EF75  8b442408                mov      eax, dword ptr [esp + 8]       
  0x0020EF79  8b510c                  mov      edx, dword ptr [ecx + 0xc]     
  0x0020EF7C  53                      push     ebx                            
  0x0020EF7D  8b19                    mov      ebx, dword ptr [ecx]           
  0x0020EF7F  55                      push     ebp                            
  0x0020EF80  8b6904                  mov      ebp, dword ptr [ecx + 4]       
  0x0020EF83  56                      push     esi                            
  0x0020EF84  89442414                mov      dword ptr [esp + 0x14], eax    
  0x0020EF88  8b4108                  mov      eax, dword ptr [ecx + 8]       
  0x0020EF8B  57                      push     edi                            
  0x0020EF8C  c744241006000000        mov      dword ptr [esp + 0x10], 6      
                                        ; XREF: 0x0020F5DE (cond_jump)
  0x0020EF94  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020EF98  85c9                    test     ecx, ecx                       
  0x0020EF9A  0f8c4f020000            jl       0x20f1ef                       
  0x0020EFA0  81c580000000            add      ebp, 0x80                      
  0x0020EFA6  be02000000              mov      esi, 2                         
  0x0020EFAB  eb03                    jmp      0x20efb0                       
  0x0020EFAD  8d4900                  lea      ecx, [ecx]                     
                                        ; XREF: 0x0020EFAB (jump), 0x0020F1E4 (cond_jump)
  0x0020EFB0  0fb60a                  movzx    ecx, byte ptr [edx]            
  0x0020EFB3  0fb638                  movzx    edi, byte ptr [eax]            
  0x0020EFB6  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020EFBA  d1f9                    sar      ecx, 1                         
  0x0020EFBC  8808                    mov      byte ptr [eax], cl             
  0x0020EFBE  0fb64a01                movzx    ecx, byte ptr [edx + 1]        
  0x0020EFC2  0fb67801                movzx    edi, byte ptr [eax + 1]        
  0x0020EFC6  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020EFCA  d1f9                    sar      ecx, 1                         
  0x0020EFCC  884801                  mov      byte ptr [eax + 1], cl         
  0x0020EFCF  0fb64a02                movzx    ecx, byte ptr [edx + 2]        
  0x0020EFD3  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020EFD7  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020EFDB  d1f9                    sar      ecx, 1                         
  0x0020EFDD  884802                  mov      byte ptr [eax + 2], cl         
  0x0020EFE0  0fb64a03                movzx    ecx, byte ptr [edx + 3]        
  0x0020EFE4  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020EFE8  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020EFEC  d1f9                    sar      ecx, 1                         
  0x0020EFEE  884803                  mov      byte ptr [eax + 3], cl         
  0x0020EFF1  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020EFF5  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020EFF9  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020EFFD  d1f9                    sar      ecx, 1                         
  0x0020EFFF  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F002  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F006  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020F00A  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F00E  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020F012  d1f9                    sar      ecx, 1                         
  0x0020F014  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F017  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F01B  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F01F  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x0020F023  d1f9                    sar      ecx, 1                         
  0x0020F025  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F028  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F02C  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F030  0fb67808                movzx    edi, byte ptr [eax + 8]        
  0x0020F034  d1f9                    sar      ecx, 1                         
  0x0020F036  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F039  0fb64a08                movzx    ecx, byte ptr [edx + 8]        
  0x0020F03D  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F041  0fb67809                movzx    edi, byte ptr [eax + 9]        
  0x0020F045  83c008                  add      eax, 8                         
  0x0020F048  d1f9                    sar      ecx, 1                         
  0x0020F04A  8808                    mov      byte ptr [eax], cl             
  0x0020F04C  0fb64a09                movzx    ecx, byte ptr [edx + 9]        
  0x0020F050  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F054  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020F058  83c208                  add      edx, 8                         
  0x0020F05B  d1f9                    sar      ecx, 1                         
  0x0020F05D  884801                  mov      byte ptr [eax + 1], cl         
  0x0020F060  0fb64a02                movzx    ecx, byte ptr [edx + 2]        
  0x0020F064  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F068  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020F06C  d1f9                    sar      ecx, 1                         
  0x0020F06E  884802                  mov      byte ptr [eax + 2], cl         
  0x0020F071  0fb64a03                movzx    ecx, byte ptr [edx + 3]        
  0x0020F075  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F079  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020F07D  d1f9                    sar      ecx, 1                         
  0x0020F07F  884803                  mov      byte ptr [eax + 3], cl         
  0x0020F082  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020F086  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F08A  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020F08E  d1f9                    sar      ecx, 1                         
  0x0020F090  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F093  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F097  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F09B  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020F09F  d1f9                    sar      ecx, 1                         
  0x0020F0A1  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F0A4  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F0A8  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F0AC  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x0020F0B0  d1f9                    sar      ecx, 1                         
  0x0020F0B2  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F0B5  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F0B9  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F0BD  d1f9                    sar      ecx, 1                         
  0x0020F0BF  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F0C2  0fb64a08                movzx    ecx, byte ptr [edx + 8]        
  0x0020F0C6  0fb67808                movzx    edi, byte ptr [eax + 8]        
  0x0020F0CA  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F0CE  d1f9                    sar      ecx, 1                         
  0x0020F0D0  884808                  mov      byte ptr [eax + 8], cl         
  0x0020F0D3  0fb64a09                movzx    ecx, byte ptr [edx + 9]        
  0x0020F0D7  0fb67809                movzx    edi, byte ptr [eax + 9]        
  0x0020F0DB  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F0DF  d1f9                    sar      ecx, 1                         
  0x0020F0E1  884809                  mov      byte ptr [eax + 9], cl         
  0x0020F0E4  0fb64a0a                movzx    ecx, byte ptr [edx + 0xa]      
  0x0020F0E8  83c008                  add      eax, 8                         
  0x0020F0EB  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020F0EF  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F0F3  d1f9                    sar      ecx, 1                         
  0x0020F0F5  884802                  mov      byte ptr [eax + 2], cl         
  0x0020F0F8  0fb64a0b                movzx    ecx, byte ptr [edx + 0xb]      
  0x0020F0FC  83c208                  add      edx, 8                         
  0x0020F0FF  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020F103  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F107  d1f9                    sar      ecx, 1                         
  0x0020F109  884803                  mov      byte ptr [eax + 3], cl         
  0x0020F10C  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020F110  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020F114  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F118  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020F11C  d1f9                    sar      ecx, 1                         
  0x0020F11E  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F121  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F125  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F129  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020F12D  d1f9                    sar      ecx, 1                         
  0x0020F12F  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F132  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F136  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F13A  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x0020F13E  d1f9                    sar      ecx, 1                         
  0x0020F140  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F143  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F147  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F14B  0fb67808                movzx    edi, byte ptr [eax + 8]        
  0x0020F14F  d1f9                    sar      ecx, 1                         
  0x0020F151  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F154  0fb64a08                movzx    ecx, byte ptr [edx + 8]        
  0x0020F158  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F15C  0fb67809                movzx    edi, byte ptr [eax + 9]        
  0x0020F160  83c008                  add      eax, 8                         
  0x0020F163  d1f9                    sar      ecx, 1                         
  0x0020F165  83c208                  add      edx, 8                         
  0x0020F168  8808                    mov      byte ptr [eax], cl             
  0x0020F16A  0fb64a01                movzx    ecx, byte ptr [edx + 1]        
  0x0020F16E  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F172  0fb67802                movzx    edi, byte ptr [eax + 2]        
  0x0020F176  d1f9                    sar      ecx, 1                         
  0x0020F178  884801                  mov      byte ptr [eax + 1], cl         
  0x0020F17B  0fb64a02                movzx    ecx, byte ptr [edx + 2]        
  0x0020F17F  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F183  0fb67803                movzx    edi, byte ptr [eax + 3]        
  0x0020F187  d1f9                    sar      ecx, 1                         
  0x0020F189  884802                  mov      byte ptr [eax + 2], cl         
  0x0020F18C  0fb64a03                movzx    ecx, byte ptr [edx + 3]        
  0x0020F190  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F194  0fb67804                movzx    edi, byte ptr [eax + 4]        
  0x0020F198  d1f9                    sar      ecx, 1                         
  0x0020F19A  884803                  mov      byte ptr [eax + 3], cl         
  0x0020F19D  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020F1A1  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F1A5  0fb67805                movzx    edi, byte ptr [eax + 5]        
  0x0020F1A9  d1f9                    sar      ecx, 1                         
  0x0020F1AB  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F1AE  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F1B2  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F1B6  0fb67806                movzx    edi, byte ptr [eax + 6]        
  0x0020F1BA  d1f9                    sar      ecx, 1                         
  0x0020F1BC  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F1BF  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F1C3  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F1C7  d1f9                    sar      ecx, 1                         
  0x0020F1C9  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F1CC  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F1D0  0fb67807                movzx    edi, byte ptr [eax + 7]        
  0x0020F1D4  8d4c3901                lea      ecx, [ecx + edi + 1]           
  0x0020F1D8  d1f9                    sar      ecx, 1                         
  0x0020F1DA  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F1DD  83c008                  add      eax, 8                         
  0x0020F1E0  83c208                  add      edx, 8                         
  0x0020F1E3  4e                      dec      esi                            
  0x0020F1E4  0f85c6fdffff            jne      0x20efb0                       
  0x0020F1EA  e9be030000              jmp      0x20f5ad                       
; end of function
                                        ; XREF: 0x0020EF9A (cond_jump)
  0x0020F1EF  c744241c02000000        mov      dword ptr [esp + 0x1c], 2      
  0x0020F1F7  eb07                    jmp      0x20f200                       
  0x0020F1F9  8da42400000000          lea      esp, [esp]                     
                                        ; XREF: 0x0020F1F7 (jump), 0x0020F5A7 (cond_jump)
  0x0020F200  0fb60a                  movzx    ecx, byte ptr [edx]            
  0x0020F203  0fb630                  movzx    esi, byte ptr [eax]            
  0x0020F206  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F20A  0fbf7500                movsx    esi, word ptr [ebp]            
  0x0020F20E  d1f9                    sar      ecx, 1                         
  0x0020F210  8bfb                    mov      edi, ebx                       
  0x0020F212  03f9                    add      edi, ecx                       
  0x0020F214  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F217  8808                    mov      byte ptr [eax], cl             
  0x0020F219  0fb64a01                movzx    ecx, byte ptr [edx + 1]        
  0x0020F21D  0fb67001                movzx    esi, byte ptr [eax + 1]        
  0x0020F221  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F225  0fbf7502                movsx    esi, word ptr [ebp + 2]        
  0x0020F229  d1f9                    sar      ecx, 1                         
  0x0020F22B  8bfb                    mov      edi, ebx                       
  0x0020F22D  03f9                    add      edi, ecx                       
  0x0020F22F  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F232  884801                  mov      byte ptr [eax + 1], cl         
  0x0020F235  0fb64a02                movzx    ecx, byte ptr [edx + 2]        
  0x0020F239  0fb67002                movzx    esi, byte ptr [eax + 2]        
  0x0020F23D  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F241  0fbf7504                movsx    esi, word ptr [ebp + 4]        
  0x0020F245  d1f9                    sar      ecx, 1                         
  0x0020F247  8bfb                    mov      edi, ebx                       
  0x0020F249  03f9                    add      edi, ecx                       
  0x0020F24B  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F24E  0fb67003                movzx    esi, byte ptr [eax + 3]        
  0x0020F252  884802                  mov      byte ptr [eax + 2], cl         
  0x0020F255  0fb64a03                movzx    ecx, byte ptr [edx + 3]        
  0x0020F259  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F25D  0fbf7506                movsx    esi, word ptr [ebp + 6]        
  0x0020F261  d1f9                    sar      ecx, 1                         
  0x0020F263  8bfb                    mov      edi, ebx                       
  0x0020F265  03f9                    add      edi, ecx                       
  0x0020F267  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F26A  0fb67004                movzx    esi, byte ptr [eax + 4]        
  0x0020F26E  884803                  mov      byte ptr [eax + 3], cl         
  0x0020F271  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020F275  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F279  0fbf7508                movsx    esi, word ptr [ebp + 8]        
  0x0020F27D  d1f9                    sar      ecx, 1                         
  0x0020F27F  8bfb                    mov      edi, ebx                       
  0x0020F281  03f9                    add      edi, ecx                       
  0x0020F283  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F286  0fb67005                movzx    esi, byte ptr [eax + 5]        
  0x0020F28A  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F28D  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F291  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F295  0fbf750a                movsx    esi, word ptr [ebp + 0xa]      
  0x0020F299  d1f9                    sar      ecx, 1                         
  0x0020F29B  8bfb                    mov      edi, ebx                       
  0x0020F29D  03f9                    add      edi, ecx                       
  0x0020F29F  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F2A2  0fb67006                movzx    esi, byte ptr [eax + 6]        
  0x0020F2A6  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F2A9  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F2AD  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F2B1  0fbf750c                movsx    esi, word ptr [ebp + 0xc]      
  0x0020F2B5  d1f9                    sar      ecx, 1                         
  0x0020F2B7  8bfb                    mov      edi, ebx                       
  0x0020F2B9  03f9                    add      edi, ecx                       
  0x0020F2BB  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F2BE  0fb67007                movzx    esi, byte ptr [eax + 7]        
  0x0020F2C2  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F2C5  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F2C9  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F2CD  0fbf750e                movsx    esi, word ptr [ebp + 0xe]      
  0x0020F2D1  d1f9                    sar      ecx, 1                         
  0x0020F2D3  8bfb                    mov      edi, ebx                       
  0x0020F2D5  03f9                    add      edi, ecx                       
  0x0020F2D7  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F2DA  0fb67008                movzx    esi, byte ptr [eax + 8]        
  0x0020F2DE  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F2E1  0fb64a08                movzx    ecx, byte ptr [edx + 8]        
  0x0020F2E5  83c008                  add      eax, 8                         
  0x0020F2E8  83c208                  add      edx, 8                         
  0x0020F2EB  83c510                  add      ebp, 0x10                      
  0x0020F2EE  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F2F2  0fbf7500                movsx    esi, word ptr [ebp]            
  0x0020F2F6  d1f9                    sar      ecx, 1                         
  0x0020F2F8  8bfb                    mov      edi, ebx                       
  0x0020F2FA  03f9                    add      edi, ecx                       
  0x0020F2FC  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F2FF  8808                    mov      byte ptr [eax], cl             
  0x0020F301  0fb64a01                movzx    ecx, byte ptr [edx + 1]        
  0x0020F305  0fb67001                movzx    esi, byte ptr [eax + 1]        
  0x0020F309  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F30D  0fbf7502                movsx    esi, word ptr [ebp + 2]        
  0x0020F311  d1f9                    sar      ecx, 1                         
  0x0020F313  8bfb                    mov      edi, ebx                       
  0x0020F315  03f9                    add      edi, ecx                       
  0x0020F317  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F31A  884801                  mov      byte ptr [eax + 1], cl         
  0x0020F31D  0fb64a02                movzx    ecx, byte ptr [edx + 2]        
  0x0020F321  0fb67002                movzx    esi, byte ptr [eax + 2]        
  0x0020F325  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F329  0fbf7504                movsx    esi, word ptr [ebp + 4]        
  0x0020F32D  d1f9                    sar      ecx, 1                         
  0x0020F32F  8bfb                    mov      edi, ebx                       
  0x0020F331  03f9                    add      edi, ecx                       
  0x0020F333  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F336  0fb67003                movzx    esi, byte ptr [eax + 3]        
  0x0020F33A  884802                  mov      byte ptr [eax + 2], cl         
  0x0020F33D  0fb64a03                movzx    ecx, byte ptr [edx + 3]        
  0x0020F341  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F345  0fbf7506                movsx    esi, word ptr [ebp + 6]        
  0x0020F349  d1f9                    sar      ecx, 1                         
  0x0020F34B  8bfb                    mov      edi, ebx                       
  0x0020F34D  03f9                    add      edi, ecx                       
  0x0020F34F  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F352  0fb67004                movzx    esi, byte ptr [eax + 4]        
  0x0020F356  884803                  mov      byte ptr [eax + 3], cl         
  0x0020F359  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020F35D  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F361  0fbf7508                movsx    esi, word ptr [ebp + 8]        
  0x0020F365  d1f9                    sar      ecx, 1                         
  0x0020F367  8bfb                    mov      edi, ebx                       
  0x0020F369  03f9                    add      edi, ecx                       
  0x0020F36B  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F36E  0fb67005                movzx    esi, byte ptr [eax + 5]        
  0x0020F372  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F375  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F379  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F37D  0fbf750a                movsx    esi, word ptr [ebp + 0xa]      
  0x0020F381  d1f9                    sar      ecx, 1                         
  0x0020F383  8bfb                    mov      edi, ebx                       
  0x0020F385  03f9                    add      edi, ecx                       
  0x0020F387  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F38A  0fb67006                movzx    esi, byte ptr [eax + 6]        
  0x0020F38E  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F391  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F395  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F399  0fbf750c                movsx    esi, word ptr [ebp + 0xc]      
  0x0020F39D  d1f9                    sar      ecx, 1                         
  0x0020F39F  8bfb                    mov      edi, ebx                       
  0x0020F3A1  03f9                    add      edi, ecx                       
  0x0020F3A3  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F3A6  0fb67007                movzx    esi, byte ptr [eax + 7]        
  0x0020F3AA  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F3AD  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F3B1  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F3B5  0fbf750e                movsx    esi, word ptr [ebp + 0xe]      
  0x0020F3B9  d1f9                    sar      ecx, 1                         
  0x0020F3BB  8bfb                    mov      edi, ebx                       
  0x0020F3BD  03f9                    add      edi, ecx                       
  0x0020F3BF  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F3C2  0fb67008                movzx    esi, byte ptr [eax + 8]        
  0x0020F3C6  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F3C9  0fb64a08                movzx    ecx, byte ptr [edx + 8]        
  0x0020F3CD  83c008                  add      eax, 8                         
  0x0020F3D0  83c208                  add      edx, 8                         
  0x0020F3D3  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F3D7  0fbf7510                movsx    esi, word ptr [ebp + 0x10]     
  0x0020F3DB  83c510                  add      ebp, 0x10                      
  0x0020F3DE  d1f9                    sar      ecx, 1                         
  0x0020F3E0  8bfb                    mov      edi, ebx                       
  0x0020F3E2  03f9                    add      edi, ecx                       
  0x0020F3E4  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F3E7  0fb67001                movzx    esi, byte ptr [eax + 1]        
  0x0020F3EB  8808                    mov      byte ptr [eax], cl             
  0x0020F3ED  0fb64a01                movzx    ecx, byte ptr [edx + 1]        
  0x0020F3F1  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F3F5  0fbf7502                movsx    esi, word ptr [ebp + 2]        
  0x0020F3F9  d1f9                    sar      ecx, 1                         
  0x0020F3FB  8bfb                    mov      edi, ebx                       
  0x0020F3FD  03f9                    add      edi, ecx                       
  0x0020F3FF  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F402  884801                  mov      byte ptr [eax + 1], cl         
  0x0020F405  0fb64a02                movzx    ecx, byte ptr [edx + 2]        
  0x0020F409  0fb67002                movzx    esi, byte ptr [eax + 2]        
  0x0020F40D  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F411  0fbf7504                movsx    esi, word ptr [ebp + 4]        
  0x0020F415  d1f9                    sar      ecx, 1                         
  0x0020F417  8bfb                    mov      edi, ebx                       
  0x0020F419  03f9                    add      edi, ecx                       
  0x0020F41B  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F41E  884802                  mov      byte ptr [eax + 2], cl         
  0x0020F421  0fb64a03                movzx    ecx, byte ptr [edx + 3]        
  0x0020F425  0fb67003                movzx    esi, byte ptr [eax + 3]        
  0x0020F429  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F42D  0fbf7506                movsx    esi, word ptr [ebp + 6]        
  0x0020F431  d1f9                    sar      ecx, 1                         
  0x0020F433  8bfb                    mov      edi, ebx                       
  0x0020F435  03f9                    add      edi, ecx                       
  0x0020F437  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F43A  0fb67004                movzx    esi, byte ptr [eax + 4]        
  0x0020F43E  884803                  mov      byte ptr [eax + 3], cl         
  0x0020F441  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020F445  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F449  0fbf7508                movsx    esi, word ptr [ebp + 8]        
  0x0020F44D  d1f9                    sar      ecx, 1                         
  0x0020F44F  8bfb                    mov      edi, ebx                       
  0x0020F451  03f9                    add      edi, ecx                       
  0x0020F453  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F456  0fb67005                movzx    esi, byte ptr [eax + 5]        
  0x0020F45A  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F45D  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F461  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F465  0fbf750a                movsx    esi, word ptr [ebp + 0xa]      
  0x0020F469  d1f9                    sar      ecx, 1                         
  0x0020F46B  8bfb                    mov      edi, ebx                       
  0x0020F46D  03f9                    add      edi, ecx                       
  0x0020F46F  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F472  0fb67006                movzx    esi, byte ptr [eax + 6]        
  0x0020F476  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F479  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F47D  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F481  0fbf750c                movsx    esi, word ptr [ebp + 0xc]      
  0x0020F485  d1f9                    sar      ecx, 1                         
  0x0020F487  8bfb                    mov      edi, ebx                       
  0x0020F489  03f9                    add      edi, ecx                       
  0x0020F48B  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F48E  0fb67007                movzx    esi, byte ptr [eax + 7]        
  0x0020F492  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F495  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F499  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F49D  0fbf750e                movsx    esi, word ptr [ebp + 0xe]      
  0x0020F4A1  d1f9                    sar      ecx, 1                         
  0x0020F4A3  8bfb                    mov      edi, ebx                       
  0x0020F4A5  03f9                    add      edi, ecx                       
  0x0020F4A7  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F4AA  0fb67008                movzx    esi, byte ptr [eax + 8]        
  0x0020F4AE  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F4B1  0fb64a08                movzx    ecx, byte ptr [edx + 8]        
  0x0020F4B5  83c008                  add      eax, 8                         
  0x0020F4B8  83c208                  add      edx, 8                         
  0x0020F4BB  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F4BF  0fbf7510                movsx    esi, word ptr [ebp + 0x10]     
  0x0020F4C3  d1f9                    sar      ecx, 1                         
  0x0020F4C5  83c510                  add      ebp, 0x10                      
  0x0020F4C8  8bfb                    mov      edi, ebx                       
  0x0020F4CA  03f9                    add      edi, ecx                       
  0x0020F4CC  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F4CF  0fb67001                movzx    esi, byte ptr [eax + 1]        
  0x0020F4D3  8808                    mov      byte ptr [eax], cl             
  0x0020F4D5  0fb64a01                movzx    ecx, byte ptr [edx + 1]        
  0x0020F4D9  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F4DD  0fbf7502                movsx    esi, word ptr [ebp + 2]        
  0x0020F4E1  d1f9                    sar      ecx, 1                         
  0x0020F4E3  8bfb                    mov      edi, ebx                       
  0x0020F4E5  03f9                    add      edi, ecx                       
  0x0020F4E7  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F4EA  884801                  mov      byte ptr [eax + 1], cl         
  0x0020F4ED  0fb64a02                movzx    ecx, byte ptr [edx + 2]        
  0x0020F4F1  0fb67002                movzx    esi, byte ptr [eax + 2]        
  0x0020F4F5  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F4F9  0fbf7504                movsx    esi, word ptr [ebp + 4]        
  0x0020F4FD  d1f9                    sar      ecx, 1                         
  0x0020F4FF  8bfb                    mov      edi, ebx                       
  0x0020F501  03f9                    add      edi, ecx                       
  0x0020F503  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F506  0fb67003                movzx    esi, byte ptr [eax + 3]        
  0x0020F50A  884802                  mov      byte ptr [eax + 2], cl         
  0x0020F50D  0fb64a03                movzx    ecx, byte ptr [edx + 3]        
  0x0020F511  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F515  0fbf7506                movsx    esi, word ptr [ebp + 6]        
  0x0020F519  d1f9                    sar      ecx, 1                         
  0x0020F51B  8bfb                    mov      edi, ebx                       
  0x0020F51D  03f9                    add      edi, ecx                       
  0x0020F51F  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F522  0fb67004                movzx    esi, byte ptr [eax + 4]        
  0x0020F526  884803                  mov      byte ptr [eax + 3], cl         
  0x0020F529  0fb64a04                movzx    ecx, byte ptr [edx + 4]        
  0x0020F52D  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F531  0fbf7508                movsx    esi, word ptr [ebp + 8]        
  0x0020F535  d1f9                    sar      ecx, 1                         
  0x0020F537  8bfb                    mov      edi, ebx                       
  0x0020F539  03f9                    add      edi, ecx                       
  0x0020F53B  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F53E  0fb67005                movzx    esi, byte ptr [eax + 5]        
  0x0020F542  884804                  mov      byte ptr [eax + 4], cl         
  0x0020F545  0fb64a05                movzx    ecx, byte ptr [edx + 5]        
  0x0020F549  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F54D  0fbf750a                movsx    esi, word ptr [ebp + 0xa]      
  0x0020F551  d1f9                    sar      ecx, 1                         
  0x0020F553  8bfb                    mov      edi, ebx                       
  0x0020F555  03f9                    add      edi, ecx                       
  0x0020F557  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F55A  0fb67006                movzx    esi, byte ptr [eax + 6]        
  0x0020F55E  884805                  mov      byte ptr [eax + 5], cl         
  0x0020F561  0fb64a06                movzx    ecx, byte ptr [edx + 6]        
  0x0020F565  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F569  0fbf750c                movsx    esi, word ptr [ebp + 0xc]      
  0x0020F56D  d1f9                    sar      ecx, 1                         
  0x0020F56F  8bfb                    mov      edi, ebx                       
  0x0020F571  03f9                    add      edi, ecx                       
  0x0020F573  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F576  0fb67007                movzx    esi, byte ptr [eax + 7]        
  0x0020F57A  884806                  mov      byte ptr [eax + 6], cl         
  0x0020F57D  0fb64a07                movzx    ecx, byte ptr [edx + 7]        
  0x0020F581  8d4c3101                lea      ecx, [ecx + esi + 1]           
  0x0020F585  0fbf750e                movsx    esi, word ptr [ebp + 0xe]      
  0x0020F589  d1f9                    sar      ecx, 1                         
  0x0020F58B  8bfb                    mov      edi, ebx                       
  0x0020F58D  03f9                    add      edi, ecx                       
  0x0020F58F  8a0c3e                  mov      cl, byte ptr [esi + edi]       
  0x0020F592  884807                  mov      byte ptr [eax + 7], cl         
  0x0020F595  8b4c241c                mov      ecx, dword ptr [esp + 0x1c]    
  0x0020F599  83c008                  add      eax, 8                         
  0x0020F59C  83c208                  add      edx, 8                         
  0x0020F59F  83c510                  add      ebp, 0x10                      
  0x0020F5A2  49                      dec      ecx                            
  0x0020F5A3  894c241c                mov      dword ptr [esp + 0x1c], ecx    
  0x0020F5A7  0f8553fcffff            jne      0x20f200                       
                                        ; XREF: 0x0020F1EA (jump)
  0x0020F5AD  8b4c2420                mov      ecx, dword ptr [esp + 0x20]    
  0x0020F5B1  8b7c2418                mov      edi, dword ptr [esp + 0x18]    
  0x0020F5B5  d1e1                    shl      ecx, 1                         
  0x0020F5B7  83e840                  sub      eax, 0x40                      
  0x0020F5BA  894c2420                mov      dword ptr [esp + 0x20], ecx    
  0x0020F5BE  8bf0                    mov      esi, eax                       
  0x0020F5C0  b910000000              mov      ecx, 0x10                      
  0x0020F5C5  f3a5                    rep movsd dword ptr es:[edi], dword ptr [esi] 
  0x0020F5C7  8b7c2418                mov      edi, dword ptr [esp + 0x18]    
  0x0020F5CB  8b4c2410                mov      ecx, dword ptr [esp + 0x10]    
  0x0020F5CF  83c740                  add      edi, 0x40                      
  0x0020F5D2  83c040                  add      eax, 0x40                      
  0x0020F5D5  49                      dec      ecx                            
  0x0020F5D6  897c2418                mov      dword ptr [esp + 0x18], edi    
  0x0020F5DA  894c2410                mov      dword ptr [esp + 0x10], ecx    
  0x0020F5DE  0f85b0f9ffff            jne      0x20ef94                       
  0x0020F5E4  5f                      pop      edi                            
  0x0020F5E5  5e                      pop      esi                            
  0x0020F5E6  5d                      pop      ebp                            
  0x0020F5E7  5b                      pop      ebx                            
  0x0020F5E8  59                      pop      ecx                            
  0x0020F5E9  c3                      ret                                     
  0x0020F5EA  90                      nop                                     
  0x0020F5EB  90                      nop                                     
  0x0020F5EC  90                      nop                                     
  0x0020F5ED  90                      nop                                     
  0x0020F5EE  90                      nop                                     
  0x0020F5EF  90                      nop                                     

; ============================================================
; Function: sub_0020F5F0
; Start: 0x0020F5F0  End: 0x0020F957  Size: 871 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020F5F0:
  0x0020F5F0  53                      push     ebx                            
  0x0020F5F1  8b5c240c                mov      ebx, dword ptr [esp + 0xc]     
  0x0020F5F5  8b0b                    mov      ecx, dword ptr [ebx]           
  0x0020F5F7  56                      push     esi                            
  0x0020F5F8  8b742414                mov      esi, dword ptr [esp + 0x14]    
  0x0020F5FC  8b06                    mov      eax, dword ptr [esi]           
  0x0020F5FE  0fbf560c                movsx    edx, word ptr [esi + 0xc]      
  0x0020F602  03c8                    add      ecx, eax                       
  0x0020F604  8b44240c                mov      eax, dword ptr [esp + 0xc]     
  0x0020F608  57                      push     edi                            
  0x0020F609  8b38                    mov      edi, dword ptr [eax]           
  0x0020F60B  8939                    mov      dword ptr [ecx], edi           
  0x0020F60D  8b7804                  mov      edi, dword ptr [eax + 4]       
  0x0020F610  897904                  mov      dword ptr [ecx + 4], edi       
  0x0020F613  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F616  83c008                  add      eax, 8                         
  0x0020F619  83c008                  add      eax, 8                         
  0x0020F61C  c1ea03                  shr      edx, 3                         
  0x0020F61F  c1e203                  shl      edx, 3                         
  0x0020F622  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F625  8b78fc                  mov      edi, dword ptr [eax - 4]       
  0x0020F628  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F62C  8b38                    mov      edi, dword ptr [eax]           
  0x0020F62E  03ca                    add      ecx, edx                       
  0x0020F630  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F633  8b7804                  mov      edi, dword ptr [eax + 4]       
  0x0020F636  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F63A  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F63D  03ca                    add      ecx, edx                       
  0x0020F63F  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F642  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F645  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F649  83c008                  add      eax, 8                         
  0x0020F64C  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F64F  03ca                    add      ecx, edx                       
  0x0020F651  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F654  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F657  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F65B  83c008                  add      eax, 8                         
  0x0020F65E  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F661  03ca                    add      ecx, edx                       
  0x0020F663  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F666  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F669  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F66D  83c008                  add      eax, 8                         
  0x0020F670  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F673  03ca                    add      ecx, edx                       
  0x0020F675  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F678  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F67B  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F67F  83c008                  add      eax, 8                         
  0x0020F682  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F685  03ca                    add      ecx, edx                       
  0x0020F687  893c0a                  mov      dword ptr [edx + ecx], edi     
  0x0020F68A  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F68D  897c0a04                mov      dword ptr [edx + ecx + 4], edi 
  0x0020F691  8b3b                    mov      edi, dword ptr [ebx]           
  0x0020F693  8b4e04                  mov      ecx, dword ptr [esi + 4]       
  0x0020F696  83c008                  add      eax, 8                         
  0x0020F699  03cf                    add      ecx, edi                       
  0x0020F69B  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F69E  8939                    mov      dword ptr [ecx], edi           
  0x0020F6A0  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F6A3  83c008                  add      eax, 8                         
  0x0020F6A6  897904                  mov      dword ptr [ecx + 4], edi       
  0x0020F6A9  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F6AC  83c008                  add      eax, 8                         
  0x0020F6AF  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F6B2  8b7804                  mov      edi, dword ptr [eax + 4]       
  0x0020F6B5  03ca                    add      ecx, edx                       
  0x0020F6B7  897904                  mov      dword ptr [ecx + 4], edi       
  0x0020F6BA  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F6BD  83c008                  add      eax, 8                         
  0x0020F6C0  03ca                    add      ecx, edx                       
  0x0020F6C2  8939                    mov      dword ptr [ecx], edi           
  0x0020F6C4  8b7804                  mov      edi, dword ptr [eax + 4]       
  0x0020F6C7  897904                  mov      dword ptr [ecx + 4], edi       
  0x0020F6CA  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F6CD  83c008                  add      eax, 8                         
  0x0020F6D0  03ca                    add      ecx, edx                       
  0x0020F6D2  8939                    mov      dword ptr [ecx], edi           
  0x0020F6D4  8b7804                  mov      edi, dword ptr [eax + 4]       
  0x0020F6D7  897904                  mov      dword ptr [ecx + 4], edi       
  0x0020F6DA  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F6DD  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F6E0  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F6E3  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F6E7  03ca                    add      ecx, edx                       
  0x0020F6E9  83c008                  add      eax, 8                         
  0x0020F6EC  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F6EF  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F6F2  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F6F5  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F6F9  03ca                    add      ecx, edx                       
  0x0020F6FB  83c008                  add      eax, 8                         
  0x0020F6FE  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F701  893c11                  mov      dword ptr [ecx + edx], edi     
  0x0020F704  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F707  897c1104                mov      dword ptr [ecx + edx + 4], edi 
  0x0020F70B  03ca                    add      ecx, edx                       
  0x0020F70D  83c008                  add      eax, 8                         
  0x0020F710  8b7808                  mov      edi, dword ptr [eax + 8]       
  0x0020F713  893c0a                  mov      dword ptr [edx + ecx], edi     
  0x0020F716  8b780c                  mov      edi, dword ptr [eax + 0xc]     
  0x0020F719  897c0a04                mov      dword ptr [edx + ecx + 4], edi 
  0x0020F71D  8b4b04                  mov      ecx, dword ptr [ebx + 4]       
  0x0020F720  8b5608                  mov      edx, dword ptr [esi + 8]       
  0x0020F723  0fbf7e0e                movsx    edi, word ptr [esi + 0xe]      
  0x0020F727  83c008                  add      eax, 8                         
  0x0020F72A  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F72D  891c11                  mov      dword ptr [ecx + edx], ebx     
  0x0020F730  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F733  895c1104                mov      dword ptr [ecx + edx + 4], ebx 
  0x0020F737  03ca                    add      ecx, edx                       
  0x0020F739  83c008                  add      eax, 8                         
  0x0020F73C  8b5840                  mov      ebx, dword ptr [eax + 0x40]    
  0x0020F73F  895908                  mov      dword ptr [ecx + 8], ebx       
  0x0020F742  8b5844                  mov      ebx, dword ptr [eax + 0x44]    
  0x0020F745  89590c                  mov      dword ptr [ecx + 0xc], ebx     
  0x0020F748  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F74B  8d5040                  lea      edx, [eax + 0x40]              
  0x0020F74E  8d7108                  lea      esi, [ecx + 8]                 
  0x0020F751  83c008                  add      eax, 8                         
  0x0020F754  c1ef03                  shr      edi, 3                         
  0x0020F757  c1e703                  shl      edi, 3                         
  0x0020F75A  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F75D  8b5804                  mov      ebx, dword ptr [eax + 4]       
  0x0020F760  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F764  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F767  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F76A  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F76D  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F771  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F774  03cf                    add      ecx, edi                       
  0x0020F776  83c208                  add      edx, 8                         
  0x0020F779  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F77C  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F77F  03f7                    add      esi, edi                       
  0x0020F781  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F785  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F788  83c008                  add      eax, 8                         
  0x0020F78B  03cf                    add      ecx, edi                       
  0x0020F78D  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F790  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F793  83c208                  add      edx, 8                         
  0x0020F796  03f7                    add      esi, edi                       
  0x0020F798  895e04                  mov      dword ptr [esi + 4], ebx       
  0x0020F79B  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F79E  83c008                  add      eax, 8                         
  0x0020F7A1  03cf                    add      ecx, edi                       
  0x0020F7A3  8919                    mov      dword ptr [ecx], ebx           
  0x0020F7A5  8b5804                  mov      ebx, dword ptr [eax + 4]       
  0x0020F7A8  895904                  mov      dword ptr [ecx + 4], ebx       
  0x0020F7AB  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F7AE  83c208                  add      edx, 8                         
  0x0020F7B1  03f7                    add      esi, edi                       
  0x0020F7B3  891e                    mov      dword ptr [esi], ebx           
  0x0020F7B5  8b5a04                  mov      ebx, dword ptr [edx + 4]       
  0x0020F7B8  83c008                  add      eax, 8                         
  0x0020F7BB  03cf                    add      ecx, edi                       
  0x0020F7BD  895e04                  mov      dword ptr [esi + 4], ebx       
  0x0020F7C0  83c208                  add      edx, 8                         
  0x0020F7C3  8b18                    mov      ebx, dword ptr [eax]           
  0x0020F7C5  8919                    mov      dword ptr [ecx], ebx           
  0x0020F7C7  8b5804                  mov      ebx, dword ptr [eax + 4]       
  0x0020F7CA  895904                  mov      dword ptr [ecx + 4], ebx       
  0x0020F7CD  8b1a                    mov      ebx, dword ptr [edx]           
  0x0020F7CF  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F7D2  8b5a04                  mov      ebx, dword ptr [edx + 4]       
  0x0020F7D5  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F7D9  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F7DC  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F7DF  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F7E2  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F7E6  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F7E9  03f7                    add      esi, edi                       
  0x0020F7EB  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F7EE  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F7F1  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F7F5  83c008                  add      eax, 8                         
  0x0020F7F8  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F7FB  03cf                    add      ecx, edi                       
  0x0020F7FD  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F800  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F803  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F807  83c208                  add      edx, 8                         
  0x0020F80A  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F80D  03f7                    add      esi, edi                       
  0x0020F80F  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F812  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F815  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F819  83c008                  add      eax, 8                         
  0x0020F81C  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F81F  03cf                    add      ecx, edi                       
  0x0020F821  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F824  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F827  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F82B  83c208                  add      edx, 8                         
  0x0020F82E  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F831  03f7                    add      esi, edi                       
  0x0020F833  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F836  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F839  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F83D  83c008                  add      eax, 8                         
  0x0020F840  8b5848                  mov      ebx, dword ptr [eax + 0x48]    
  0x0020F843  03cf                    add      ecx, edi                       
  0x0020F845  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F848  8b584c                  mov      ebx, dword ptr [eax + 0x4c]    
  0x0020F84B  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F84F  83c208                  add      edx, 8                         
  0x0020F852  8b5a48                  mov      ebx, dword ptr [edx + 0x48]    
  0x0020F855  03f7                    add      esi, edi                       
  0x0020F857  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F85A  8b5a4c                  mov      ebx, dword ptr [edx + 0x4c]    
  0x0020F85D  83c048                  add      eax, 0x48                      
  0x0020F860  03cf                    add      ecx, edi                       
  0x0020F862  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F866  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F869  83c248                  add      edx, 0x48                      
  0x0020F86C  03f7                    add      esi, edi                       
  0x0020F86E  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F871  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F874  83c008                  add      eax, 8                         
  0x0020F877  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F87B  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F87E  03cf                    add      ecx, edi                       
  0x0020F880  83c208                  add      edx, 8                         
  0x0020F883  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F886  8b5a04                  mov      ebx, dword ptr [edx + 4]       
  0x0020F889  03f7                    add      esi, edi                       
  0x0020F88B  895e04                  mov      dword ptr [esi + 4], ebx       
  0x0020F88E  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F891  83c008                  add      eax, 8                         
  0x0020F894  03cf                    add      ecx, edi                       
  0x0020F896  8919                    mov      dword ptr [ecx], ebx           
  0x0020F898  8b5804                  mov      ebx, dword ptr [eax + 4]       
  0x0020F89B  83c208                  add      edx, 8                         
  0x0020F89E  895904                  mov      dword ptr [ecx + 4], ebx       
  0x0020F8A1  8b1a                    mov      ebx, dword ptr [edx]           
  0x0020F8A3  03f7                    add      esi, edi                       
  0x0020F8A5  83c008                  add      eax, 8                         
  0x0020F8A8  03cf                    add      ecx, edi                       
  0x0020F8AA  891e                    mov      dword ptr [esi], ebx           
  0x0020F8AC  8b5a04                  mov      ebx, dword ptr [edx + 4]       
  0x0020F8AF  895e04                  mov      dword ptr [esi + 4], ebx       
  0x0020F8B2  8b18                    mov      ebx, dword ptr [eax]           
  0x0020F8B4  8919                    mov      dword ptr [ecx], ebx           
  0x0020F8B6  8b5804                  mov      ebx, dword ptr [eax + 4]       
  0x0020F8B9  895904                  mov      dword ptr [ecx + 4], ebx       
  0x0020F8BC  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F8BF  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F8C2  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F8C5  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F8C9  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F8CC  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F8CF  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F8D2  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F8D6  83c208                  add      edx, 8                         
  0x0020F8D9  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F8DC  03f7                    add      esi, edi                       
  0x0020F8DE  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F8E1  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F8E4  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F8E8  83c008                  add      eax, 8                         
  0x0020F8EB  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F8EE  03cf                    add      ecx, edi                       
  0x0020F8F0  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F8F3  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F8F6  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F8FA  83c208                  add      edx, 8                         
  0x0020F8FD  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F900  03f7                    add      esi, edi                       
  0x0020F902  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F905  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F908  895c3e04                mov      dword ptr [esi + edi + 4], ebx 
  0x0020F90C  83c008                  add      eax, 8                         
  0x0020F90F  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F912  03cf                    add      ecx, edi                       
  0x0020F914  83c208                  add      edx, 8                         
  0x0020F917  891c39                  mov      dword ptr [ecx + edi], ebx     
  0x0020F91A  8b580c                  mov      ebx, dword ptr [eax + 0xc]     
  0x0020F91D  03f7                    add      esi, edi                       
  0x0020F91F  895c3904                mov      dword ptr [ecx + edi + 4], ebx 
  0x0020F923  8b5a08                  mov      ebx, dword ptr [edx + 8]       
  0x0020F926  83c008                  add      eax, 8                         
  0x0020F929  03cf                    add      ecx, edi                       
  0x0020F92B  891c3e                  mov      dword ptr [esi + edi], ebx     
  0x0020F92E  8b5a0c                  mov      ebx, dword ptr [edx + 0xc]     
  0x0020F931  83c208                  add      edx, 8                         
  0x0020F934  03f7                    add      esi, edi                       
  0x0020F936  895e04                  mov      dword ptr [esi + 4], ebx       
  0x0020F939  8b5808                  mov      ebx, dword ptr [eax + 8]       
  0x0020F93C  891c0f                  mov      dword ptr [edi + ecx], ebx     
  0x0020F93F  8b400c                  mov      eax, dword ptr [eax + 0xc]     
  0x0020F942  89440f04                mov      dword ptr [edi + ecx + 4], eax 
  0x0020F946  8b4a08                  mov      ecx, dword ptr [edx + 8]       
  0x0020F949  890c37                  mov      dword ptr [edi + esi], ecx     
  0x0020F94C  8b520c                  mov      edx, dword ptr [edx + 0xc]     
  0x0020F94F  89543704                mov      dword ptr [edi + esi + 4], edx 
  0x0020F953  5f                      pop      edi                            
  0x0020F954  5e                      pop      esi                            
  0x0020F955  5b                      pop      ebx                            
  0x0020F956  c3                      ret                                     
; end of function
  0x0020F957  90                      nop                                     
  0x0020F958  90                      nop                                     
  0x0020F959  90                      nop                                     
  0x0020F95A  90                      nop                                     
  0x0020F95B  90                      nop                                     
  0x0020F95C  90                      nop                                     
  0x0020F95D  90                      nop                                     
  0x0020F95E  90                      nop                                     
  0x0020F95F  90                      nop                                     

; ============================================================
; Function: sub_0020F960
; Start: 0x0020F960  End: 0x0020FA4E  Size: 238 bytes
; Detection: call_target (confidence: 0.90)
; ============================================================
sub_0020F960:
  0x0020F960  8b442408                mov      eax, dword ptr [esp + 8]       
  0x0020F964  56                      push     esi                            
  0x0020F965  8b742408                mov      esi, dword ptr [esp + 8]       
  0x0020F969  8b4e14                  mov      ecx, dword ptr [esi + 0x14]    
  0x0020F96C  8b560c                  mov      edx, dword ptr [esi + 0xc]     
  0x0020F96F  c1ea03                  shr      edx, 3                         
  0x0020F972  c1e203                  shl      edx, 3                         
  0x0020F975  57                      push     edi                            
  0x0020F976  8b39                    mov      edi, dword ptr [ecx]           
  0x0020F978  8938                    mov      dword ptr [eax], edi           
  0x0020F97A  8b7904                  mov      edi, dword ptr [ecx + 4]       
  0x0020F97D  897804                  mov      dword ptr [eax + 4], edi       
  0x0020F980  8b3c11                  mov      edi, dword ptr [ecx + edx]     
  0x0020F983  897808                  mov      dword ptr [eax + 8], edi       
  0x0020F986  8b7c1104                mov      edi, dword ptr [ecx + edx + 4] 
  0x0020F98A  03ca                    add      ecx, edx                       
  0x0020F98C  89780c                  mov      dword ptr [eax + 0xc], edi     
  0x0020F98F  8b3c11                  mov      edi, dword ptr [ecx + edx]     
  0x0020F992  897810                  mov      dword ptr [eax + 0x10], edi    
  0x0020F995  8b7c1104                mov      edi, dword ptr [ecx + edx + 4] 
  0x0020F999  03ca                    add      ecx, edx                       
  0x0020F99B  897814                  mov      dword ptr [eax + 0x14], edi    
  0x0020F99E  8b3c11                  mov      edi, dword ptr [ecx + edx]     
  0x0020F9A1  897818                  mov      dword ptr [eax + 0x18], edi    
  0x0020F9A4  8b7c1104                mov      edi, dword ptr [ecx + edx + 4] 
  0x0020F9A8  03ca                    add      ecx, edx                       
  0x0020F9AA  89781c                  mov      dword ptr [eax + 0x1c], edi    
  0x0020F9AD  8b3c11                  mov      edi, dword ptr [ecx + edx]     
  0x0020F9B0  897820                  mov      dword ptr [eax + 0x20], edi    
  0x0020F9B3  8b7c1104                mov      edi, dword ptr [ecx + edx + 4] 
  0x0020F9B7  03ca                    add      ecx, edx                       
  0x0020F9B9  897824                  mov      dword ptr [eax + 0x24], edi    
  0x0020F9BC  8b3c11                  mov      edi, dword ptr [ecx + edx]     
  0x0020F9BF  03ca                    add      ecx, edx                       
  0x0020F9C1  897828                  mov      dword ptr [eax + 0x28], edi    
  0x0020F9C4  8b7904                  mov      edi, dword ptr [ecx + 4]       
  0x0020F9C7  89782c                  mov      dword ptr [eax + 0x2c], edi    
  0x0020F9CA  8b3c11                  mov      edi, dword ptr [ecx + edx]     
  0x0020F9CD  03ca                    add      ecx, edx                       
  0x0020F9CF  897830                  mov      dword ptr [eax + 0x30], edi    
  0x0020F9D2  8b7904                  mov      edi, dword ptr [ecx + 4]       
  0x0020F9D5  897834                  mov      dword ptr [eax + 0x34], edi    
  0x0020F9D8  8b3c0a                  mov      edi, dword ptr [edx + ecx]     
  0x0020F9DB  897838                  mov      dword ptr [eax + 0x38], edi    
  0x0020F9DE  8b4c0a04                mov      ecx, dword ptr [edx + ecx + 4] 
  0x0020F9E2  8b10                    mov      edx, dword ptr [eax]           
  0x0020F9E4  89483c                  mov      dword ptr [eax + 0x3c], ecx    
  0x0020F9E7  8b4e04                  mov      ecx, dword ptr [esi + 4]       
  0x0020F9EA  8911                    mov      dword ptr [ecx], edx           
  0x0020F9EC  8b5004                  mov      edx, dword ptr [eax + 4]       
  0x0020F9EF  895104                  mov      dword ptr [ecx + 4], edx       
  0x0020F9F2  8b5008                  mov      edx, dword ptr [eax + 8]       
  0x0020F9F5  895108                  mov      dword ptr [ecx + 8], edx       
  0x0020F9F8  8b500c                  mov      edx, dword ptr [eax + 0xc]     
  0x0020F9FB  89510c                  mov      dword ptr [ecx + 0xc], edx     
  0x0020F9FE  8b5010                  mov      edx, dword ptr [eax + 0x10]    
  0x0020FA01  895110                  mov      dword ptr [ecx + 0x10], edx    
  0x0020FA04  8b5014                  mov      edx, dword ptr [eax + 0x14]    
  0x0020FA07  895114                  mov      dword ptr [ecx + 0x14], edx    
  0x0020FA0A  8b5018                  mov      edx, dword ptr [eax + 0x18]    
  0x0020FA0D  895118                  mov      dword ptr [ecx + 0x18], edx    
  0x0020FA10  8b501c                  mov      edx, dword ptr [eax + 0x1c]    
  0x0020FA13  89511c                  mov      dword ptr [ecx + 0x1c], edx    
  0x0020FA16  8b5020                  mov      edx, dword ptr [eax + 0x20]    
  0x0020FA19  83c120                  add      ecx, 0x20                      
  0x0020FA1C  8911                    mov      dword ptr [ecx], edx           
  0x0020FA1E  8b5024                  mov      edx, dword ptr [eax + 0x24]    
  0x0020FA21  895104                  mov      dword ptr [ecx + 4], edx       
  0x0020FA24  8b5028                  mov      edx, dword ptr [eax + 0x28]    
  0x0020FA27  895108                  mov      dword ptr [ecx + 8], edx       
  0x0020FA2A  8b502c                  mov      edx, dword ptr [eax + 0x2c]    
  0x0020FA2D  89510c                  mov      dword ptr [ecx + 0xc], edx     
  0x0020FA30  8b5030                  mov      edx, dword ptr [eax + 0x30]    
  0x0020FA33  895110                  mov      dword ptr [ecx + 0x10], edx    
  0x0020FA36  8b5034                  mov      edx, dword ptr [eax + 0x34]    
  0x0020FA39  895114                  mov      dword ptr [ecx + 0x14], edx    
  0x0020FA3C  8b5038                  mov      edx, dword ptr [eax + 0x38]    
  0x0020FA3F  895118                  mov      dword ptr [ecx + 0x18], edx    
  0x0020FA42  8b403c                  mov      eax, dword ptr [eax + 0x3c]    
  0x0020FA45  89411c                  mov      dword ptr [ecx + 0x1c], eax    
  0x0020FA48  8d4120                  lea      eax, [ecx + 0x20]              
  0x0020FA4B  5f                      pop      edi                            
  0x0020FA4C  5e                      pop      esi                            
  0x0020FA4D  c3                      ret                                     
; end of function
  0x0020FA4E  90                      nop                                     
  0x0020FA4F  90                      nop                                     
