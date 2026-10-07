; ============================================================
; Section: PSGSFD_P
; VA: 0x00210320 - 0x00210750
; Size: 1072 bytes (1.0 KB)
; Functions: 0
; Instructions: 345
; ============================================================

  0x00210320  83ec0c                  sub      esp, 0xc                       
  0x00210323  53                      push     ebx                            
  0x00210324  55                      push     ebp                            
  0x00210325  56                      push     esi                            
  0x00210326  57                      push     edi                            
  0x00210327  8b7c2420                mov      edi, dword ptr [esp + 0x20]    
  0x0021032B  8b5704                  mov      edx, dword ptr [edi + 4]       
  0x0021032E  c744241401000000        mov      dword ptr [esp + 0x14], 1      
  0x00210336  89542420                mov      dword ptr [esp + 0x20], edx    
                                        ; XREF: 0x0021070C (jump)
  0x0021033A  8b1f                    mov      ebx, dword ptr [edi]           
  0x0021033C  8b6f08                  mov      ebp, dword ptr [edi + 8]       
  0x0021033F  8b770c                  mov      esi, dword ptr [edi + 0xc]     
  0x00210342  8bc3                    mov      eax, ebx                       
  0x00210344  c1e809                  shr      eax, 9                         
  0x00210347  83fd09                  cmp      ebp, 9                         
  0x0021034A  89442410                mov      dword ptr [esp + 0x10], eax    
  0x0021034E  7e13                    jle      0x210363                       
  0x00210350  b929000000              mov      ecx, 0x29                      
  0x00210355  2bcd                    sub      ecx, ebp                       
  0x00210357  8bc2                    mov      eax, edx                       
  0x00210359  d3e8                    shr      eax, cl                        
  0x0021035B  8bc8                    mov      ecx, eax                       
  0x0021035D  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x00210361  0bc1                    or       eax, ecx                       
                                        ; XREF: 0x0021034E (cond_jump)
  0x00210363  85c0                    test     eax, eax                       
  0x00210365  0f84a6030000            je       0x210711                       
  0x0021036B  8b879c020000            mov      eax, dword ptr [edi + 0x29c]   
  0x00210371  89442418                mov      dword ptr [esp + 0x18], eax    
                                        ; XREF: 0x00210409 (cond_jump), 0x0021041B (jump)
  0x00210375  8bc3                    mov      eax, ebx                       
  0x00210377  c1e815                  shr      eax, 0x15                      
  0x0021037A  83fd15                  cmp      ebp, 0x15                      
  0x0021037D  7e0f                    jle      0x21038e                       
  0x0021037F  b935000000              mov      ecx, 0x35                      
  0x00210384  2bcd                    sub      ecx, ebp                       
  0x00210386  d3ea                    shr      edx, cl                        
  0x00210388  0bc2                    or       eax, edx                       
  0x0021038A  8b542420                mov      edx, dword ptr [esp + 0x20]    
                                        ; XREF: 0x0021037D (cond_jump)
  0x0021038E  a980ffffff              test     eax, 0xffffff80                
  0x00210393  7510                    jne      0x2103a5                       
  0x00210395  8b0dc847c800            mov      ecx, dword ptr [0xc847c8]      
  0x0021039B  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x0021039F  89442410                mov      dword ptr [esp + 0x10], eax    
  0x002103A3  eb11                    jmp      0x2103b6                       
                                        ; XREF: 0x00210393 (cond_jump)
  0x002103A5  8b0dc447c800            mov      ecx, dword ptr [0xc847c4]      
  0x002103AB  c1e806                  shr      eax, 6                         
  0x002103AE  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x002103B2  89442410                mov      dword ptr [esp + 0x10], eax    
                                        ; XREF: 0x002103A3 (jump)
  0x002103B6  8bc8                    mov      ecx, eax                       
  0x002103B8  83e10f                  and      ecx, 0xf                       
  0x002103BB  03e9                    add      ebp, ecx                       
  0x002103BD  83fd20                  cmp      ebp, 0x20                      
  0x002103C0  7c37                    jl       0x2103f9                       
  0x002103C2  0fbe06                  movsx    eax, byte ptr [esi]            
  0x002103C5  83ed20                  sub      ebp, 0x20                      
  0x002103C8  8bda                    mov      ebx, edx                       
  0x002103CA  8bcd                    mov      ecx, ebp                       
  0x002103CC  d3e3                    shl      ebx, cl                        
  0x002103CE  0fb64e01                movzx    ecx, byte ptr [esi + 1]        
  0x002103D2  c1e008                  shl      eax, 8                         
  0x002103D5  46                      inc      esi                            
  0x002103D6  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x002103DA  0bc1                    or       eax, ecx                       
  0x002103DC  46                      inc      esi                            
  0x002103DD  0fb64e01                movzx    ecx, byte ptr [esi + 1]        
  0x002103E1  c1e008                  shl      eax, 8                         
  0x002103E4  0bc2                    or       eax, edx                       
  0x002103E6  46                      inc      esi                            
  0x002103E7  c1e008                  shl      eax, 8                         
  0x002103EA  0bc1                    or       eax, ecx                       
  0x002103EC  89442420                mov      dword ptr [esp + 0x20], eax    
  0x002103F0  8bd0                    mov      edx, eax                       
  0x002103F2  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x002103F6  46                      inc      esi                            
  0x002103F7  eb02                    jmp      0x2103fb                       
                                        ; XREF: 0x002103C0 (cond_jump)
  0x002103F9  d3e3                    shl      ebx, cl                        
                                        ; XREF: 0x002103F7 (jump)
  0x002103FB  8bc8                    mov      ecx, eax                       
  0x002103FD  c1e902                  shr      ecx, 2                         
  0x00210400  0fb6c9                  movzx    ecx, cl                        
  0x00210403  c1e902                  shr      ecx, 2                         
  0x00210406  83f922                  cmp      ecx, 0x22                      
  0x00210409  0f8466ffffff            je       0x210375                       
  0x0021040F  83f923                  cmp      ecx, 0x23                      
  0x00210412  750c                    jne      0x210420                       
  0x00210414  83879c02000021          add      dword ptr [edi + 0x29c], 0x21  
  0x0021041B  e955ffffff              jmp      0x210375                       
                                        ; XREF: 0x00210412 (cond_jump)
  0x00210420  83f924                  cmp      ecx, 0x24                      
  0x00210423  0f84e8020000            je       0x210711                       
  0x00210429  018f9c020000            add      dword ptr [edi + 0x29c], ecx   
  0x0021042F  8b8f9c020000            mov      ecx, dword ptr [edi + 0x29c]   
  0x00210435  c1e80a                  shr      eax, 0xa                       
  0x00210438  8987a4020000            mov      dword ptr [edi + 0x2a4], eax   
  0x0021043E  3b8fa0020000            cmp      ecx, dword ptr [edi + 0x2a0]   
  0x00210444  0f8fc7020000            jg       0x210711                       
  0x0021044A  2b4c2418                sub      ecx, dword ptr [esp + 0x18]    
  0x0021044E  83f9fe                  cmp      ecx, -2                        
  0x00210451  0f84ba020000            je       0x210711                       
  0x00210457  8b442414                mov      eax, dword ptr [esp + 0x14]    
  0x0021045B  85c0                    test     eax, eax                       
  0x0021045D  7545                    jne      0x2104a4                       
  0x0021045F  83f901                  cmp      ecx, 1                         
  0x00210462  7640                    jbe      0x2104a4                       
  0x00210464  51                      push     ecx                            
  0x00210465  57                      push     edi                            
  0x00210466  ff9730020000            call     dword ptr [edi + 0x230]        
  0x0021046C  8b542428                mov      edx, dword ptr [esp + 0x28]    
  0x00210470  83c408                  add      esp, 8                         
  0x00210473  33c0                    xor      eax, eax                       
  0x00210475  898764020000            mov      dword ptr [edi + 0x264], eax   
  0x0021047B  898768020000            mov      dword ptr [edi + 0x268], eax   
  0x00210481  89876c020000            mov      dword ptr [edi + 0x26c], eax   
  0x00210487  898770020000            mov      dword ptr [edi + 0x270], eax   
  0x0021048D  b800040000              mov      eax, 0x400                     
  0x00210492  8987ac020000            mov      dword ptr [edi + 0x2ac], eax   
  0x00210498  8987b4020000            mov      dword ptr [edi + 0x2b4], eax   
  0x0021049E  8987b0020000            mov      dword ptr [edi + 0x2b0], eax   
                                        ; XREF: 0x0021045D (cond_jump), 0x00210462 (cond_jump)
  0x002104A4  f687a402000020          test     byte ptr [edi + 0x2a4], 0x20   
  0x002104AB  7575                    jne      0x210522                       
  0x002104AD  8bc3                    mov      eax, ebx                       
  0x002104AF  c1e81b                  shr      eax, 0x1b                      
  0x002104B2  83fd1b                  cmp      ebp, 0x1b                      
  0x002104B5  89442410                mov      dword ptr [esp + 0x10], eax    
  0x002104B9  7e13                    jle      0x2104ce                       
  0x002104BB  b93b000000              mov      ecx, 0x3b                      
  0x002104C0  2bcd                    sub      ecx, ebp                       
  0x002104C2  8bc2                    mov      eax, edx                       
  0x002104C4  d3e8                    shr      eax, cl                        
  0x002104C6  8bc8                    mov      ecx, eax                       
  0x002104C8  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x002104CC  0bc1                    or       eax, ecx                       
                                        ; XREF: 0x002104B9 (cond_jump)
  0x002104CE  8b0de043c800            mov      ecx, dword ptr [0xc843e0]      
  0x002104D4  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x002104D8  8bc8                    mov      ecx, eax                       
  0x002104DA  c1e908                  shr      ecx, 8                         
  0x002104DD  898fa4020000            mov      dword ptr [edi + 0x2a4], ecx   
  0x002104E3  0fb6c8                  movzx    ecx, al                        
  0x002104E6  03e9                    add      ebp, ecx                       
  0x002104E8  83fd20                  cmp      ebp, 0x20                      
  0x002104EB  7c33                    jl       0x210520                       
  0x002104ED  0fbe06                  movsx    eax, byte ptr [esi]            
  0x002104F0  83ed20                  sub      ebp, 0x20                      
  0x002104F3  8bda                    mov      ebx, edx                       
  0x002104F5  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x002104F9  8bcd                    mov      ecx, ebp                       
  0x002104FB  d3e3                    shl      ebx, cl                        
  0x002104FD  46                      inc      esi                            
  0x002104FE  0fb64e01                movzx    ecx, byte ptr [esi + 1]        
  0x00210502  c1e008                  shl      eax, 8                         
  0x00210505  0bc2                    or       eax, edx                       
  0x00210507  46                      inc      esi                            
  0x00210508  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0021050C  c1e008                  shl      eax, 8                         
  0x0021050F  0bc1                    or       eax, ecx                       
  0x00210511  46                      inc      esi                            
  0x00210512  c1e008                  shl      eax, 8                         
  0x00210515  0bc2                    or       eax, edx                       
  0x00210517  89442420                mov      dword ptr [esp + 0x20], eax    
  0x0021051B  46                      inc      esi                            
  0x0021051C  8bd0                    mov      edx, eax                       
  0x0021051E  eb02                    jmp      0x210522                       
                                        ; XREF: 0x002104EB (cond_jump)
  0x00210520  d3e3                    shl      ebx, cl                        
                                        ; XREF: 0x002104AB (cond_jump), 0x0021051E (jump)
  0x00210522  f687a402000010          test     byte ptr [edi + 0x2a4], 0x10   
  0x00210529  7461                    je       0x21058c                       
  0x0021052B  83fd1b                  cmp      ebp, 0x1b                      
  0x0021052E  7c4b                    jl       0x21057b                       
  0x00210530  83ed1b                  sub      ebp, 0x1b                      
  0x00210533  7416                    je       0x21054b                       
  0x00210535  b905000000              mov      ecx, 5                         
  0x0021053A  2bcd                    sub      ecx, ebp                       
  0x0021053C  8bc2                    mov      eax, edx                       
  0x0021053E  d3e8                    shr      eax, cl                        
  0x00210540  8bcd                    mov      ecx, ebp                       
  0x00210542  0bc3                    or       eax, ebx                       
  0x00210544  c1e81b                  shr      eax, 0x1b                      
  0x00210547  d3e2                    shl      edx, cl                        
  0x00210549  eb05                    jmp      0x210550                       
                                        ; XREF: 0x00210533 (cond_jump)
  0x0021054B  8bc3                    mov      eax, ebx                       
  0x0021054D  c1e81b                  shr      eax, 0x1b                      
                                        ; XREF: 0x00210549 (jump)
  0x00210550  0fbe0e                  movsx    ecx, byte ptr [esi]            
  0x00210553  46                      inc      esi                            
  0x00210554  c1e108                  shl      ecx, 8                         
  0x00210557  8bda                    mov      ebx, edx                       
  0x00210559  0fb616                  movzx    edx, byte ptr [esi]            
  0x0021055C  0bca                    or       ecx, edx                       
  0x0021055E  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x00210562  46                      inc      esi                            
  0x00210563  c1e108                  shl      ecx, 8                         
  0x00210566  0bca                    or       ecx, edx                       
  0x00210568  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0021056C  46                      inc      esi                            
  0x0021056D  c1e108                  shl      ecx, 8                         
  0x00210570  0bca                    or       ecx, edx                       
  0x00210572  894c2420                mov      dword ptr [esp + 0x20], ecx    
  0x00210576  46                      inc      esi                            
  0x00210577  8bd1                    mov      edx, ecx                       
  0x00210579  eb0b                    jmp      0x210586                       
                                        ; XREF: 0x0021052E (cond_jump)
  0x0021057B  8bc3                    mov      eax, ebx                       
  0x0021057D  83c505                  add      ebp, 5                         
  0x00210580  c1e81b                  shr      eax, 0x1b                      
  0x00210583  c1e305                  shl      ebx, 5                         
                                        ; XREF: 0x00210579 (jump)
  0x00210586  898750020000            mov      dword ptr [edi + 0x250], eax   
                                        ; XREF: 0x00210529 (cond_jump)
  0x0021058C  f687a402000008          test     byte ptr [edi + 0x2a4], 8      
  0x00210593  7463                    je       0x2105f8                       
  0x00210595  8d8764020000            lea      eax, [edi + 0x264]             
  0x0021059B  50                      push     eax                            
  0x0021059C  8d8f6c020000            lea      ecx, [edi + 0x26c]             
  0x002105A2  89770c                  mov      dword ptr [edi + 0xc], esi     
  0x002105A5  51                      push     ecx                            
  0x002105A6  8db754020000            lea      esi, [edi + 0x254]             
  0x002105AC  56                      push     esi                            
  0x002105AD  57                      push     edi                            
  0x002105AE  891f                    mov      dword ptr [edi], ebx           
  0x002105B0  895704                  mov      dword ptr [edi + 4], edx       
  0x002105B3  896f08                  mov      dword ptr [edi + 8], ebp       
  0x002105B6  e8e5f6ffff              call     0x20fca0                       ; -> sub_0020FCA0
  0x002105BB  8d9768020000            lea      edx, [edi + 0x268]             
  0x002105C1  52                      push     edx                            
  0x002105C2  8944242c                mov      dword ptr [esp + 0x2c], eax    
  0x002105C6  8d8770020000            lea      eax, [edi + 0x270]             
  0x002105CC  50                      push     eax                            
  0x002105CD  56                      push     esi                            
  0x002105CE  57                      push     edi                            
  0x002105CF  e8ccf6ffff              call     0x20fca0                       ; -> sub_0020FCA0
  0x002105D4  8b4f04                  mov      ecx, dword ptr [edi + 4]       
  0x002105D7  8b1f                    mov      ebx, dword ptr [edi]           
  0x002105D9  8b6f08                  mov      ebp, dword ptr [edi + 8]       
  0x002105DC  8b770c                  mov      esi, dword ptr [edi + 0xc]     
  0x002105DF  894c2440                mov      dword ptr [esp + 0x40], ecx    
  0x002105E3  8b4c2438                mov      ecx, dword ptr [esp + 0x38]    
  0x002105E7  83c420                  add      esp, 0x20                      
  0x002105EA  0bc1                    or       eax, ecx                       
  0x002105EC  0f851f010000            jne      0x210711                       
  0x002105F2  8b542420                mov      edx, dword ptr [esp + 0x20]    
  0x002105F6  eb1a                    jmp      0x210612                       
                                        ; XREF: 0x00210593 (cond_jump)
  0x002105F8  33c0                    xor      eax, eax                       
  0x002105FA  898764020000            mov      dword ptr [edi + 0x264], eax   
  0x00210600  898768020000            mov      dword ptr [edi + 0x268], eax   
  0x00210606  89876c020000            mov      dword ptr [edi + 0x26c], eax   
  0x0021060C  898770020000            mov      dword ptr [edi + 0x270], eax   
                                        ; XREF: 0x002105F6 (jump)
  0x00210612  f687a402000002          test     byte ptr [edi + 0x2a4], 2      
  0x00210619  747a                    je       0x210695                       
  0x0021061B  8bc3                    mov      eax, ebx                       
  0x0021061D  c1e817                  shr      eax, 0x17                      
  0x00210620  83fd17                  cmp      ebp, 0x17                      
  0x00210623  89442410                mov      dword ptr [esp + 0x10], eax    
  0x00210627  7e13                    jle      0x21063c                       
  0x00210629  b937000000              mov      ecx, 0x37                      
  0x0021062E  2bcd                    sub      ecx, ebp                       
  0x00210630  8bc2                    mov      eax, edx                       
  0x00210632  d3e8                    shr      eax, cl                        
  0x00210634  8bc8                    mov      ecx, eax                       
  0x00210636  8b442410                mov      eax, dword ptr [esp + 0x10]    
  0x0021063A  0bc1                    or       eax, ecx                       
                                        ; XREF: 0x00210627 (cond_jump)
  0x0021063C  8b0d044ac800            mov      ecx, dword ptr [0xc84a04]      
  0x00210642  0fbf0441                movsx    eax, word ptr [ecx + eax*2]    
  0x00210646  8bc8                    mov      ecx, eax                       
  0x00210648  83e1f0                  and      ecx, 0xfffffff0                
  0x0021064B  c1e110                  shl      ecx, 0x10                      
  0x0021064E  898fa8020000            mov      dword ptr [edi + 0x2a8], ecx   
  0x00210654  0fb6c8                  movzx    ecx, al                        
  0x00210657  03e9                    add      ebp, ecx                       
  0x00210659  83fd20                  cmp      ebp, 0x20                      
  0x0021065C  7c33                    jl       0x210691                       
  0x0021065E  0fbe06                  movsx    eax, byte ptr [esi]            
  0x00210661  83ed20                  sub      ebp, 0x20                      
  0x00210664  8bda                    mov      ebx, edx                       
  0x00210666  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0021066A  8bcd                    mov      ecx, ebp                       
  0x0021066C  d3e3                    shl      ebx, cl                        
  0x0021066E  46                      inc      esi                            
  0x0021066F  0fb64e01                movzx    ecx, byte ptr [esi + 1]        
  0x00210673  c1e008                  shl      eax, 8                         
  0x00210676  0bc2                    or       eax, edx                       
  0x00210678  46                      inc      esi                            
  0x00210679  0fb65601                movzx    edx, byte ptr [esi + 1]        
  0x0021067D  c1e008                  shl      eax, 8                         
  0x00210680  0bc1                    or       eax, ecx                       
  0x00210682  46                      inc      esi                            
  0x00210683  c1e008                  shl      eax, 8                         
  0x00210686  0bc2                    or       eax, edx                       
  0x00210688  89442420                mov      dword ptr [esp + 0x20], eax    
  0x0021068C  46                      inc      esi                            
  0x0021068D  8bd0                    mov      edx, eax                       
  0x0021068F  eb0e                    jmp      0x21069f                       
                                        ; XREF: 0x0021065C (cond_jump)
  0x00210691  d3e3                    shl      ebx, cl                        
  0x00210693  eb0a                    jmp      0x21069f                       
                                        ; XREF: 0x00210619 (cond_jump)
  0x00210695  c787a802000000000000    mov      dword ptr [edi + 0x2a8], 0     
                                        ; XREF: 0x0021068F (jump), 0x00210693 (jump)
  0x0021069F  f687a402000001          test     byte ptr [edi + 0x2a4], 1      
  0x002106A6  891f                    mov      dword ptr [edi], ebx           
  0x002106A8  895704                  mov      dword ptr [edi + 4], edx       
  0x002106AB  896f08                  mov      dword ptr [edi + 8], ebp       
  0x002106AE  89770c                  mov      dword ptr [edi + 0xc], esi     
  0x002106B1  7413                    je       0x2106c6                       
  0x002106B3  57                      push     edi                            
  0x002106B4  ff9734020000            call     dword ptr [edi + 0x234]        
  0x002106BA  57                      push     edi                            
  0x002106BB  ff973c020000            call     dword ptr [edi + 0x23c]        
  0x002106C1  83c408                  add      esp, 8                         
  0x002106C4  eb35                    jmp      0x2106fb                       
                                        ; XREF: 0x002106B1 (cond_jump)
  0x002106C6  8b87a8020000            mov      eax, dword ptr [edi + 0x2a8]   
  0x002106CC  85c0                    test     eax, eax                       
  0x002106CE  740a                    je       0x2106da                       
  0x002106D0  57                      push     edi                            
  0x002106D1  ff9738020000            call     dword ptr [edi + 0x238]        
  0x002106D7  83c404                  add      esp, 4                         
                                        ; XREF: 0x002106CE (cond_jump)
  0x002106DA  57                      push     edi                            
  0x002106DB  ff9748020000            call     dword ptr [edi + 0x248]        
  0x002106E1  b800040000              mov      eax, 0x400                     
  0x002106E6  83c404                  add      esp, 4                         
  0x002106E9  8987ac020000            mov      dword ptr [edi + 0x2ac], eax   
  0x002106EF  8987b4020000            mov      dword ptr [edi + 0x2b4], eax   
  0x002106F5  8987b0020000            mov      dword ptr [edi + 0x2b0], eax   
                                        ; XREF: 0x002106C4 (jump)
  0x002106FB  8b4704                  mov      eax, dword ptr [edi + 4]       
  0x002106FE  89442420                mov      dword ptr [esp + 0x20], eax    
  0x00210702  c744241400000000        mov      dword ptr [esp + 0x14], 0      
  0x0021070A  8bd0                    mov      edx, eax                       
  0x0021070C  e929fcffff              jmp      0x21033a                       
                                        ; XREF: 0x00210365 (cond_jump), 0x00210423 (cond_jump), 0x00210444 (cond_jump), 0x00210451 (cond_jump), 0x002105EC (cond_jump)
  0x00210711  8b542424                mov      edx, dword ptr [esp + 0x24]    
  0x00210715  8b1a                    mov      ebx, dword ptr [edx]           
  0x00210717  83c507                  add      ebp, 7                         
  0x0021071A  c1fd03                  sar      ebp, 3                         
  0x0021071D  8d4c2ef8                lea      ecx, [esi + ebp - 8]           
  0x00210721  8b74242c                mov      esi, dword ptr [esp + 0x2c]    
  0x00210725  8b3e                    mov      edi, dword ptr [esi]           
  0x00210727  8bc1                    mov      eax, ecx                       
  0x00210729  2bc3                    sub      eax, ebx                       
  0x0021072B  03f8                    add      edi, eax                       
  0x0021072D  893e                    mov      dword ptr [esi], edi           
  0x0021072F  8b7c2428                mov      edi, dword ptr [esp + 0x28]    
  0x00210733  8b2f                    mov      ebp, dword ptr [edi]           
  0x00210735  56                      push     esi                            
  0x00210736  2be8                    sub      ebp, eax                       
  0x00210738  57                      push     edi                            
  0x00210739  892f                    mov      dword ptr [edi], ebp           
  0x0021073B  52                      push     edx                            
  0x0021073C  890a                    mov      dword ptr [edx], ecx           
  0x0021073E  e85d99f9ff              call     0x1aa0a0                       ; -> sub_001AA0A0
  0x00210743  83c40c                  add      esp, 0xc                       
  0x00210746  5f                      pop      edi                            
  0x00210747  5e                      pop      esi                            
  0x00210748  5d                      pop      ebp                            
  0x00210749  5b                      pop      ebx                            
  0x0021074A  83c40c                  add      esp, 0xc                       
  0x0021074D  c3                      ret                                     
  0x0021074E  90                      nop                                     
  0x0021074F  90                      nop                                     
