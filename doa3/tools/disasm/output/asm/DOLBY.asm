; ============================================================
; Section: DOLBY
; VA: 0x00CA2560 - 0x00CA92F8
; Size: 28056 bytes (27.4 KB)
; Functions: 0
; Instructions: 11957
; ============================================================

  0x00CA2560  080c0500000000          or       byte ptr [eax], cl             
  0x00CA2567  0001                    add      byte ptr [ecx], al             
  0x00CA2569  0000                    add      byte ptr [eax], al             
  0x00CA256B  0001                    add      byte ptr [ecx], al             
  0x00CA256D  0000                    add      byte ptr [eax], al             
  0x00CA256F  0000                    add      byte ptr [eax], al             
  0x00CA2571  0000                    add      byte ptr [eax], al             
  0x00CA2573  0000                    add      byte ptr [eax], al             
  0x00CA2575  0000                    add      byte ptr [eax], al             
  0x00CA2577  00cc                    add      ah, cl                         
  0x00CA2579  cc                      int3                                    
  0x00CA257A  cc                      int3                                    
  0x00CA257B  0000                    add      byte ptr [eax], al             
  0x00CA257D  0000                    add      byte ptr [eax], al             
  0x00CA257F  0000                    add      byte ptr [eax], al             
  0x00CA2581  002400                  add      byte ptr [eax + eax], ah       
  0x00CA2584  847007                  test     byte ptr [eax + 7], dh         
  0x00CA2587  000400                  add      byte ptr [eax + eax], al       
  0x00CA258A  0000                    add      byte ptr [eax], al             
  0x00CA258C  847007                  test     byte ptr [eax + 7], dh         
  0x00CA258F  000500000032            add      byte ptr [0x32000000], al      
  0x00CA2595  f4                      hlt                                     
  0x00CA2596  07                      pop      es                             
  0x00CA2597  00ff                    add      bh, bh                         
  0x00CA259A  ff00                    inc      dword ptr [eax]                
  0x00CA259C  30f4                    xor      ah, dh                         
  0x00CA259E  07                      pop      es                             
  0x00CA259F  0001                    add      byte ptr [ecx], al             
  0x00CA25A1  0000                    add      byte ptr [eax], al             
  0x00CA25A3  0031                    add      byte ptr [ecx], dh             
  0x00CA25A5  f4                      hlt                                     
  0x00CA25A6  07                      pop      es                             
  0x00CA25A7  0001                    add      byte ptr [ecx], al             
  0x00CA25A9  0000                    add      byte ptr [eax], al             
  0x00CA25AB  00bb0005002a            add      byte ptr [ebx + 0x2a000500], bh 
  0x00CA25B1  f4                      hlt                                     
  0x00CA25B2  0500e00b00              add      eax, 0xbe000                   
  0x00CA25B7  00b820050074            add      byte ptr [eax + 0x74000520], bh 
  0x00CA25BD  fa                      cli                                     
  0x00CA25BE  0a00                    or       al, byte ptr [eax]             
  0x00CA25C0  1300                    adc      eax, dword ptr [eax]           
  0x00CA25C2  2000                    and      byte ptr [eax], al             
  0x00CA25C4  887007                  mov      byte ptr [eax + 7], dh         
  0x00CA25C7  0001                    add      byte ptr [ecx], al             
  0x00CA25C9  0000                    add      byte ptr [eax], al             
  0x00CA25CB  0008                    add      byte ptr [eax], cl             
  0x00CA25CD  0000                    add      byte ptr [eax], al             
  0x00CA25CF  008870070002            add      byte ptr [eax + 0x2000770], cl 
  0x00CA25D5  0000                    add      byte ptr [eax], al             
  0x00CA25D7  008870070003            add      byte ptr [eax + 0x3000770], cl 
  0x00CA25DD  0000                    add      byte ptr [eax], al             
  0x00CA25DF  0000                    add      byte ptr [eax], al             
  0x00CA25E1  f4                      hlt                                     
  0x00CA25E2  56                      push     esi                            
  0x00CA25E3  0000                    add      byte ptr [eax], al             
  0x00CA25E5  0000                    add      byte ptr [eax], al             
  0x00CA25E7  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA25ED  0100                    add      dword ptr [eax], eax           
  0x00CA25EF  0003                    add      byte ptr [ebx], al             
  0x00CA25F1  0020                    add      byte ptr [eax], ah             
  0x00CA25F3  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA25F6  0d00a20000              or       eax, 0xa200                    
  0x00CA25FB  0085f4080002            add      byte ptr [ebp + 0x20008f4], al 
  0x00CA2601  0000                    add      byte ptr [eax], al             
  0x00CA2603  008ef0070007            add      byte ptr [esi + 0x70007f0], cl 
  0x00CA2609  0000                    add      byte ptr [eax], al             
  0x00CA260B  00804101008e            add      byte ptr [eax - 0x71fffebf], al 
  0x00CA2611  7007                    jo       0xca261a                       
  0x00CA2613  0007                    add      byte ptr [edi], al             
  0x00CA2615  0000                    add      byte ptr [eax], al             
  0x00CA2617  0084f408000100          add      byte ptr [esp + esi*8 + 0x10008], al 
  0x00CA261E  0000                    add      byte ptr [eax], al             
  0x00CA2620  1300                    adc      eax, dword ptr [eax]           
  0x00CA2622  2000                    and      byte ptr [eax], al             
  0x00CA2624  80410100                add      byte ptr [ecx + 1], 0          
  0x00CA2628  81850a003100000000f0    add      dword ptr [ebp + 0x31000a], 0xf0000000 
  0x00CA2632  44                      inc      esp                            
  0x00CA2633  00b3ffff0084            add      byte ptr [ebx - 0x7bff0001], dh 
  0x00CA2639  7007                    jo       0xca2642                       
  0x00CA263B  000500000085            add      byte ptr [0x85000000], al      
  0x00CA2641  f4                      hlt                                     
                                        ; XREF: 0x00CA2639 (cond_jump)
  0x00CA2642  0800                    or       byte ptr [eax], al             
  0x00CA2644  0200                    add      al, byte ptr [eax]             
  0x00CA2646  0000                    add      byte ptr [eax], al             
  0x00CA264A  07                      pop      es                             
  0x00CA264B  0001                    add      byte ptr [ecx], al             
  0x00CA264D  0000                    add      byte ptr [eax], al             
  0x00CA264F  0003                    add      byte ptr [ebx], al             
  0x00CA2651  f4                      hlt                                     
  0x00CA2652  60                      pushal                                  
  0x00CA2653  00c0                    add      al, al                         
  0x00CA2655  0b00                    or       eax, dword ptr [eax]           
  0x00CA2657  0009                    add      byte ptr [ecx], cl             
  0x00CA2659  2405                    and      al, 5                          
  0x00CA265B  0000                    add      byte ptr [eax], al             
  0x00CA265D  f4                      hlt                                     
  0x00CA265E  56                      push     esi                            
  0x00CA265F  000a                    add      byte ptr [edx], cl             
  0x00CA2661  0000                    add      byte ptr [eax], al             
  0x00CA2663  0000                    add      byte ptr [eax], al             
  0x00CA2665  2038                    and      byte ptr [eax], bh             
  0x00CA2667  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA266D  0100                    add      dword ptr [eax], eax           
  0x00CA266F  0003                    add      byte ptr [ebx], al             
  0x00CA2671  0020                    add      byte ptr [eax], ah             
  0x00CA2673  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA2676  0d00820000              or       eax, 0x8200                    
  0x00CA267B  0000                    add      byte ptr [eax], al             
  0x00CA267E  56                      push     esi                            
  0x00CA267F  00c1                    add      cl, al                         
  0x00CA2681  0b00                    or       eax, dword ptr [eax]           
  0x00CA2683  0003                    add      byte ptr [ebx], al             
  0x00CA2685  f4                      hlt                                     
  0x00CA2686  60                      pushal                                  
  0x00CA2687  0000                    add      byte ptr [eax], al             
  0x00CA2689  0300                    add      eax, dword ptr [eax]           
  0x00CA268B  0012                    add      byte ptr [edx], dl             
  0x00CA268D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA268E  050000f456              add      eax, 0x56f40000                
  0x00CA2693  0001                    add      byte ptr [ecx], al             
  0x00CA2695  0000                    add      byte ptr [eax], al             
  0x00CA2697  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA269D  0100                    add      dword ptr [eax], eax           
  0x00CA269F  0003                    add      byte ptr [ebx], al             
  0x00CA26A1  0020                    add      byte ptr [eax], ah             
  0x00CA26A3  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA26A6  0d00760000              or       eax, 0x7600                    
  0x00CA26AB  008ff0070002            add      byte ptr [edi + 0x20007f0], cl 
  0x00CA26B1  0000                    add      byte ptr [eax], al             
  0x00CA26B3  0000                    add      byte ptr [eax], al             
  0x00CA26B5  f4                      hlt                                     
  0x00CA26B6  60                      pushal                                  
  0x00CA26B7  00c0                    add      al, al                         
  0x00CA26B9  0b00                    or       eax, dword ptr [eax]           
  0x00CA26BB  0080f00b0004            add      byte ptr [eax + 0x4000bf0], al 
  0x00CA26C1  0300                    add      eax, dword ptr [eax]           
  0x00CA26C3  0000                    add      byte ptr [eax], al             
  0x00CA26C5  002400                  add      byte ptr [eax + eax], ah       
  0x00CA26C8  847007                  test     byte ptr [eax + 7], dh         
  0x00CA26CB  0002                    add      byte ptr [edx], al             
  0x00CA26CD  0000                    add      byte ptr [eax], al             
  0x00CA26CF  001c0c                  add      byte ptr [esp + ecx], bl       
  0x00CA26D2  050000f057              add      eax, 0x57f00000                
  0x00CA26D7  00c3                    add      bl, al                         
  0x00CA26D9  0b00                    or       eax, dword ptr [eax]           
  0x00CA26DB  000b                    add      byte ptr [ebx], cl             
  0x00CA26DD  f4                      hlt                                     
  0x00CA26DE  60                      pushal                                  
  0x00CA26DF  0000                    add      byte ptr [eax], al             
  0x00CA26E1  0300                    add      eax, dword ptr [eax]           
  0x00CA26E3  0017                    add      byte ptr [edi], dl             
  0x00CA26E5  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA26E6  050013f444              add      eax, 0x44f41300                
  0x00CA26EB  0005000000cd            add      byte ptr [0xcd000000], al      
  0x00CA26F1  40                      inc      eax                            
  0x00CA26F2  0100                    add      dword ptr [eax], eax           
  0x00CA26F4  0100                    add      dword ptr [eax], eax           
  0x00CA26F6  0000                    add      byte ptr [eax], al             
  0x00CA26F8  41                      inc      ecx                            
  0x00CA26F9  2a20                    sub      ah, byte ptr [eax]             
  0x00CA26FB  0000                    add      byte ptr [eax], al             
  0x00CA26FD  f4                      hlt                                     
  0x00CA26FE  44                      inc      esp                            
  0x00CA26FF  0006                    add      byte ptr [esi], al             
  0x00CA2701  0000                    add      byte ptr [eax], al             
  0x00CA2703  00cd                    add      ch, cl                         
  0x00CA2705  40                      inc      eax                            
  0x00CA2706  0100                    add      dword ptr [eax], eax           
  0x00CA2708  0200                    add      al, byte ptr [eax]             
  0x00CA270A  0000                    add      byte ptr [eax], al             
  0x00CA270C  41                      inc      ecx                            
  0x00CA270D  2a20                    sub      ah, byte ptr [eax]             
  0x00CA270F  0003                    add      byte ptr [ebx], al             
  0x00CA2711  0020                    add      byte ptr [eax], ah             
  0x00CA2713  000b                    add      byte ptr [ebx], cl             
  0x00CA2715  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA2716  050080f00b              add      eax, 0xbf08000                 
  0x00CA271B  008001000003            add      byte ptr [eax + 0x3000001], al 
  0x00CA2721  0020                    add      byte ptr [eax], ah             
  0x00CA2723  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA2726  0d00560000              or       eax, 0x5600                    
  0x00CA272B  0000                    add      byte ptr [eax], al             
  0x00CA272D  f4                      hlt                                     
  0x00CA272E  60                      pushal                                  
  0x00CA272F  00c0                    add      al, al                         
  0x00CA2731  0b00                    or       eax, dword ptr [eax]           
  0x00CA2733  0080f00b0004            add      byte ptr [eax + 0x4000bf0], al 
  0x00CA2739  0300                    add      eax, dword ptr [eax]           
  0x00CA273B  0001                    add      byte ptr [ecx], al             
  0x00CA273D  0c05                    or       al, 5                          
  0x00CA273F  0000                    add      byte ptr [eax], al             
  0x00CA2742  56                      push     esi                            
  0x00CA2743  00c2                    add      dl, al                         
  0x00CA2745  0b00                    or       eax, dword ptr [eax]           
  0x00CA2747  0003                    add      byte ptr [ebx], al             
  0x00CA2749  f4                      hlt                                     
  0x00CA274A  60                      pushal                                  
  0x00CA274B  0000                    add      byte ptr [eax], al             
  0x00CA274D  0300                    add      eax, dword ptr [eax]           
  0x00CA274F  004fa4                  add      byte ptr [edi - 0x5c], cl      
  0x00CA2752  050000f456              add      eax, 0x56f40000                
  0x00CA2757  0002                    add      byte ptr [edx], al             
  0x00CA2759  0000                    add      byte ptr [eax], al             
  0x00CA275B  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA2761  0100                    add      dword ptr [eax], eax           
  0x00CA2763  0003                    add      byte ptr [ebx], al             
  0x00CA2765  0020                    add      byte ptr [eax], ah             
  0x00CA2767  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA276A  0d00450000              or       eax, 0x4500                    
  0x00CA276F  0000                    add      byte ptr [eax], al             
  0x00CA2771  f4                      hlt                                     
  0x00CA2772  60                      pushal                                  
  0x00CA2773  00c0                    add      al, al                         
  0x00CA2775  0b00                    or       eax, dword ptr [eax]           
  0x00CA2777  008ff0070003            add      byte ptr [edi + 0x30007f0], cl 
  0x00CA277D  0000                    add      byte ptr [eax], al             
  0x00CA277F  0084f007000100          add      byte ptr [eax + esi*8 + 0x10007], al 
  0x00CA2786  0000                    add      byte ptr [eax], al             
  0x00CA2788  80f00b                  xor      al, 0xb                        
  0x00CA278B  000403                  add      byte ptr [ebx + eax], al       
  0x00CA278E  0000                    add      byte ptr [eax], al             
  0x00CA2790  00f4                    add      ah, dh                         
  0x00CA2792  60                      pushal                                  
  0x00CA2793  0000                    add      byte ptr [eax], al             
  0x00CA2795  0300                    add      eax, dword ptr [eax]           
  0x00CA2797  0000                    add      byte ptr [eax], al             
  0x00CA2799  f4                      hlt                                     
  0x00CA279A  56                      push     esi                            
  0x00CA279B  0003                    add      byte ptr [ebx], al             
  0x00CA279D  0000                    add      byte ptr [eax], al             
  0x00CA279F  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA27A5  0100                    add      dword ptr [eax], eax           
  0x00CA27A7  0003                    add      byte ptr [ebx], al             
  0x00CA27A9  0020                    add      byte ptr [eax], ah             
  0x00CA27AB  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA27AE  0d00340000              or       eax, 0x3400                    
  0x00CA27B3  008ff0070003            add      byte ptr [edi + 0x30007f0], cl 
  0x00CA27B9  0000                    add      byte ptr [eax], al             
  0x00CA27BB  0080f00b0004            add      byte ptr [eax + 0x4000bf0], al 
  0x00CA27C1  0300                    add      eax, dword ptr [eax]           
  0x00CA27C3  0000                    add      byte ptr [eax], al             
  0x00CA27C5  f4                      hlt                                     
  0x00CA27C6  60                      pushal                                  
  0x00CA27C7  0000                    add      byte ptr [eax], al             
  0x00CA27C9  0300                    add      eax, dword ptr [eax]           
  0x00CA27CB  0000                    add      byte ptr [eax], al             
  0x00CA27CD  f4                      hlt                                     
  0x00CA27CE  56                      push     esi                            
  0x00CA27CF  000400                  add      byte ptr [eax + eax], al       
  0x00CA27D2  0000                    add      byte ptr [eax], al             
  0x00CA27D4  80f00b                  xor      al, 0xb                        
  0x00CA27D7  008001000003            add      byte ptr [eax + 0x3000001], al 
  0x00CA27DD  0020                    add      byte ptr [eax], ah             
  0x00CA27DF  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA27E2  0d00270000              or       eax, 0x2700                    
  0x00CA27E7  008ff0070003            add      byte ptr [edi + 0x30007f0], cl 
  0x00CA27ED  0000                    add      byte ptr [eax], al             
  0x00CA27EF  0084f007000100          add      byte ptr [eax + esi*8 + 0x10007], al 
  0x00CA27F6  0000                    add      byte ptr [eax], al             
  0x00CA27F8  80f00b                  xor      al, 0xb                        
  0x00CA27FB  000403                  add      byte ptr [ebx + eax], al       
  0x00CA27FE  0000                    add      byte ptr [eax], al             
  0x00CA2800  0000                    add      byte ptr [eax], al             
  0x00CA2802  2400                    and      al, 0                          
  0x00CA2804  847007                  test     byte ptr [eax + 7], dh         
  0x00CA2807  0003                    add      byte ptr [ebx], al             
  0x00CA2809  0000                    add      byte ptr [eax], al             
  0x00CA280B  008ef0070001            add      byte ptr [esi + 0x10007f0], cl 
  0x00CA2811  0000                    add      byte ptr [eax], al             
  0x00CA2813  008041010085            add      byte ptr [eax - 0x7afffebf], al 
  0x00CA2819  46                      inc      esi                            
  0x00CA281A  0100                    add      dword ptr [eax], eax           
  0x00CA281C  1321                    adc      esp, dword ptr [ecx]           
  0x00CA281E  2000                    and      byte ptr [eax], al             
  0x00CA2821  7007                    jo       0xca282a                       
  0x00CA2823  0001                    add      byte ptr [ecx], al             
  0x00CA2825  0000                    add      byte ptr [eax], al             
  0x00CA2827  0000                    add      byte ptr [eax], al             
  0x00CA2829  f4                      hlt                                     
                                        ; XREF: 0x00CA2821 (cond_jump)
  0x00CA282A  56                      push     esi                            
  0x00CA282B  000b                    add      byte ptr [ebx], cl             
  0x00CA282D  0000                    add      byte ptr [eax], al             
  0x00CA282F  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA2835  0100                    add      dword ptr [eax], eax           
  0x00CA2837  0000                    add      byte ptr [eax], al             
  0x00CA283A  56                      push     esi                            
  0x00CA283B  00b3ffff0084            add      byte ptr [ebx - 0x7bff0001], dh 
  0x00CA2842  07                      pop      es                             
  0x00CA2843  000500000044            add      byte ptr [0x44000000], al      
  0x00CA2849  0020                    add      byte ptr [eax], ah             
  0x00CA284B  008c7007000400          add      byte ptr [eax + esi*2 + 0x40007], cl 
  0x00CA2852  0000                    add      byte ptr [eax], al             
  0x00CA2854  00f4                    add      ah, dh                         
  0x00CA2856  44                      inc      esp                            
  0x00CA2857  00a0cd0a00f8            add      byte ptr [eax - 0x7fff533], ah 
  0x00CA285D  1f                      pop      ds                             
  0x00CA285E  0c00                    or       al, 0                          
  0x00CA2860  c9                      leave                                   
  0x00CA2861  96                      xchg     esi, eax                       
  0x00CA2862  050000f444              add      eax, 0x44f40000                
  0x00CA2867  00bbbbbb0084            add      byte ptr [ebx - 0x7bff4445], bh 
  0x00CA286D  7007                    jo       0xca2876                       
  0x00CA286F  0006                    add      byte ptr [esi], al             
  0x00CA2871  0000                    add      byte ptr [eax], al             
  0x00CA2873  0000                    add      byte ptr [eax], al             
  0x00CA2875  0c05                    or       al, 5                          
  0x00CA2877  00c3                    add      bl, al                         
  0x00CA2879  0e                      push     cs                             
  0x00CA287A  0500c20e05              add      eax, 0x50ec200                 
  0x00CA287F  0000                    add      byte ptr [eax], al             
  0x00CA2881  f4                      hlt                                     
  0x00CA2882  6200                    bound    eax, qword ptr [eax]           
  0x00CA2884  0001                    add      byte ptr [ecx], al             
  0x00CA2886  0000                    add      byte ptr [eax], al             
  0x00CA2888  009a21000001            add      byte ptr [edx + 0x1000021], bl 
  0x00CA288E  3d00854001              cmp      eax, 0x1408500                 
  0x00CA2893  00d0                    add      al, dl                         
  0x00CA2895  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA2896  0500004a20              add      eax, 0x204a0000                
  0x00CA289B  0000                    add      byte ptr [eax], al             
  0x00CA289D  4a                      dec      edx                            
  0x00CA289E  2000                    and      byte ptr [eax], al             
  0x00CA28A0  91                      xchg     ecx, eax                       
  0x00CA28A1  da07                    fiadd    dword ptr [edi]                
  0x00CA28A3  00854e01008f            add      byte ptr [ebp - 0x70fffeb2], al 
  0x00CA28A9  1405                    adc      al, 5                          
  0x00CA28AB  008546010040            add      byte ptr [ebp + 0x40000146], al 
  0x00CA28B1  f4                      hlt                                     
  0x00CA28B2  0500854901              add      eax, 0x1498500                 
  0x00CA28B7  0058a4                  add      byte ptr [eax - 0x5c], bl      
  0x00CA28BA  0500854a01              add      eax, 0x14a8500                 
  0x00CA28BF  0017                    add      byte ptr [edi], dl             
  0x00CA28C1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA28C2  0500854b01              add      eax, 0x14b8500                 
  0x00CA28C7  001da4050085            add      byte ptr [0x850005a4], bl      
  0x00CA28CD  4c                      dec      esp                            
  0x00CA28CE  0100                    add      dword ptr [eax], eax           
  0x00CA28D0  56                      push     esi                            
  0x00CA28D1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA28D2  0500854d01              add      eax, 0x14d8500                 
  0x00CA28D7  0059a4                  add      byte ptr [ecx - 0x5c], bl      
  0x00CA28DA  0500854801              add      eax, 0x1488500                 
  0x00CA28DF  0047a4                  add      byte ptr [edi - 0x5c], al      
  0x00CA28E2  05001c0c05              add      eax, 0x50c1c00                 
  0x00CA28E7  0000                    add      byte ptr [eax], al             
  0x00CA28E9  0000                    add      byte ptr [eax], al             
  0x00CA28EB  0000                    add      byte ptr [eax], al             
  0x00CA28ED  0000                    add      byte ptr [eax], al             
  0x00CA28EF  0000                    add      byte ptr [eax], al             
  0x00CA28F1  0000                    add      byte ptr [eax], al             
  0x00CA28F3  0001                    add      byte ptr [ecx], al             
  0x00CA28F5  0000                    add      byte ptr [eax], al             
  0x00CA28F7  0000                    add      byte ptr [eax], al             
  0x00CA28F9  0000                    add      byte ptr [eax], al             
  0x00CA28FB  0000                    add      byte ptr [eax], al             
  0x00CA28FD  0000                    add      byte ptr [eax], al             
  0x00CA28FF  0000                    add      byte ptr [eax], al             
  0x00CA2901  0000                    add      byte ptr [eax], al             
  0x00CA2903  0000                    add      byte ptr [eax], al             
  0x00CA2905  0000                    add      byte ptr [eax], al             
  0x00CA2907  0000                    add      byte ptr [eax], al             
  0x00CA2909  0000                    add      byte ptr [eax], al             
  0x00CA290B  0000                    add      byte ptr [eax], al             
  0x00CA290D  0000                    add      byte ptr [eax], al             
  0x00CA290F  0000                    add      byte ptr [eax], al             
  0x00CA2911  0000                    add      byte ptr [eax], al             
  0x00CA2913  0000                    add      byte ptr [eax], al             
  0x00CA2915  0000                    add      byte ptr [eax], al             
  0x00CA2917  0000                    add      byte ptr [eax], al             
  0x00CA2919  0000                    add      byte ptr [eax], al             
  0x00CA291B  008eda070000            add      byte ptr [esi + 0x7da], cl     
  0x00CA2921  0423                    add      al, 0x23                       
  0x00CA2923  004598                  add      byte ptr [ebp - 0x68], al      
  0x00CA2926  2100                    and      dword ptr [eax], eax           
  0x00CA2928  9b                      wait                                    
  0x00CA2929  080500850c05            or       byte ptr [0x50c8500], al       
  0x00CA292F  0098da070014            add      byte ptr [eax + 0x140007da], bl 
  0x00CA2935  090500820c05            or       dword ptr [0x50c8200], eax     
  0x00CA293B  0084f007001601          add      byte ptr [eax + esi*8 + 0x1160007], al 
  0x00CA2942  0000                    add      byte ptr [eax], al             
  0x00CA2944  48                      dec      eax                            
  0x00CA2945  c40b                    les      ecx, ptr [ebx]                 
  0x00CA2947  00847007001601          add      byte ptr [eax + esi*2 + 0x1160007], al 
  0x00CA294E  0000                    add      byte ptr [eax], al             
  0x00CA2950  5c                      pop      esp                            
  0x00CA2951  0c05                    or       al, 5                          
  0x00CA2953  0000                    add      byte ptr [eax], al             
  0x00CA2955  2f                      das                                     
  0x00CA2956  2300                    and      eax, dword ptr [eax]           
  0x00CA2958  93                      xchg     ebx, eax                       
  0x00CA2959  1d0c000024              sbb      eax, 0x2400000c                
  0x00CA295E  2200                    and      al, byte ptr [eax]             
  0x00CA2960  48                      dec      eax                            
  0x00CA2961  0020                    add      byte ptr [eax], ah             
  0x00CA2963  008ef0070016            add      byte ptr [esi + 0x160007f0], cl 
  0x00CA2969  0100                    add      dword ptr [eax], eax           
  0x00CA296B  0010                    add      byte ptr [eax], dl             
  0x00CA296D  0020                    add      byte ptr [eax], ah             
  0x00CA296F  0000                    add      byte ptr [eax], al             
  0x00CA2971  91                      xchg     ecx, eax                       
  0x00CA2972  2100                    and      dword ptr [eax], eax           
  0x00CA2974  8808                    mov      byte ptr [eax], cl             
  0x00CA2976  0500520c05              add      eax, 0x50c5200                 
  0x00CA297B  0000                    add      byte ptr [eax], al             
  0x00CA297D  2e2300                  and      eax, dword ptr cs:[eax]        
  0x00CA2980  854001                  test     dword ptr [eax + 1], eax       
  0x00CA2983  004f24                  add      byte ptr [edi + 0x24], cl      
  0x00CA2986  050000013a              add      eax, 0x3a010000                
  0x00CA298B  0000                    add      byte ptr [eax], al             
  0x00CA298D  003c00                  add      byte ptr [eax + eax], bh       
  0x00CA2990  95                      xchg     ebp, eax                       
  0x00CA2991  0805004b0c05            or       byte ptr [0x50c4b00], al       
  0x00CA2997  0000                    add      byte ptr [eax], al             
  0x00CA2999  003a                    add      byte ptr [edx], bh             
  0x00CA299B  0000                    add      byte ptr [eax], al             
  0x00CA299D  003c00                  add      byte ptr [eax + eax], bh       
  0x00CA29A0  91                      xchg     ecx, eax                       
  0x00CA29A1  080500470c05            or       byte ptr [0x50c4700], al       
  0x00CA29A7  0000                    add      byte ptr [eax], al             
  0x00CA29A9  003a                    add      byte ptr [edx], bh             
  0x00CA29AB  0000                    add      byte ptr [eax], al             
  0x00CA29AD  013c00                  add      dword ptr [eax + eax], edi     
  0x00CA29B0  0000                    add      byte ptr [eax], al             
  0x00CA29B2  3d008c0805              cmp      eax, 0x5088c00                 
  0x00CA29B7  00420c                  add      byte ptr [edx + 0xc], al       
  0x00CA29BA  050000003a              add      eax, 0x3a000000                
  0x00CA29BF  0000                    add      byte ptr [eax], al             
  0x00CA29C1  003c00                  add      byte ptr [eax + eax], bh       
  0x00CA29C4  8808                    mov      byte ptr [eax], cl             
  0x00CA29C6  05001e0c05              add      eax, 0x50c1e00                 
  0x00CA29CB  00050200000f            add      byte ptr [0xf000002], al       
  0x00CA29D1  0200                    add      al, byte ptr [eax]             
  0x00CA29D3  002b                    add      byte ptr [ebx], ch             
  0x00CA29D5  0200                    add      al, byte ptr [eax]             
  0x00CA29D7  0036                    add      byte ptr [esi], dh             
  0x00CA29D9  0200                    add      al, byte ptr [eax]             
  0x00CA29DB  004102                  add      byte ptr [ecx + 2], al         
  0x00CA29DE  0000                    add      byte ptr [eax], al             
  0x00CA29E0  4c                      dec      esp                            
  0x00CA29E1  0200                    add      al, byte ptr [eax]             
  0x00CA29E3  008d40010008            add      byte ptr [ebp + 0x8000140], cl 
  0x00CA29E9  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA29EA  050000bc21              add      eax, 0x21bc0000                
  0x00CA29EF  0000                    add      byte ptr [eax], al             
  0x00CA29F1  f4                      hlt                                     
  0x00CA29F2  6400d2                  add      dl, dl                         
  0x00CA29F5  0100                    add      dword ptr [eax], eax           
  0x00CA29F7  0000                    add      byte ptr [eax], al             
  0x00CA29F9  49                      dec      ecx                            
  0x00CA29FA  2000                    and      byte ptr [eax], al             
  0x00CA29FC  96                      xchg     esi, eax                       
  0x00CA29FD  ec                      in       al, dx                         
  0x00CA29FE  07                      pop      es                             
  0x00CA29FF  0080e60b000f            add      byte ptr [eax + 0xf000be6], al 
  0x00CA2A05  0c05                    or       al, 5                          
  0x00CA2A07  0000                    add      byte ptr [eax], al             
  0x00CA2A09  0f2300                  mov      dr0, eax                       
  0x00CA2A0E  07                      pop      es                             
  0x00CA2A0F  009b01000084            add      byte ptr [ebx - 0x7bffffff], bl 
  0x00CA2A16  07                      pop      es                             
  0x00CA2A17  009a01000014            add      byte ptr [edx + 0x14000001], bl 
  0x00CA2A1D  0020                    add      byte ptr [eax], ah             
  0x00CA2A1F  000a                    add      byte ptr [edx], cl             
  0x00CA2A21  94                      xchg     esp, eax                       
  0x00CA2A22  0500485220              add      eax, 0x20524800                
  0x00CA2A27  00845a0700985a          add      byte ptr [edx + ebx*2 + 0x5a980007], al 
  0x00CA2A2E  07                      pop      es                             
  0x00CA2A2F  008c7007009b01          add      byte ptr [eax + esi*2 + 0x19b0007], cl 
  0x00CA2A36  0000                    add      byte ptr [eax], al             
  0x00CA2A38  8d7007                  lea      esi, [eax + 7]                 
  0x00CA2A3B  009a01000013            add      byte ptr [edx + 0x13000001], bl 
  0x00CA2A41  0020                    add      byte ptr [eax], ah             
  0x00CA2A43  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2A46  0000                    add      byte ptr [eax], al             
  0x00CA2A48  00f4                    add      ah, dh                         
  0x00CA2A4A  56                      push     esi                            
  0x00CA2A4B  000400                  add      byte ptr [eax + eax], al       
  0x00CA2A4E  0000                    add      byte ptr [eax], al             
  0x00CA2A50  0c00                    or       al, 0                          
  0x00CA2A52  0000                    add      byte ptr [eax], al             
  0x00CA2A54  84f0                    test     al, dh                         
  0x00CA2A56  07                      pop      es                             
  0x00CA2A57  001c01                  add      byte ptr [ecx + eax], bl       
  0x00CA2A5A  0000                    add      byte ptr [eax], al             
  0x00CA2A5C  847007                  test     byte ptr [eax + 7], dh         
  0x00CA2A5F  009a01000084            add      byte ptr [edx - 0x7bffffff], bl 
  0x00CA2A66  07                      pop      es                             
  0x00CA2A67  001d01000084            add      byte ptr [0x84000001], bl      
  0x00CA2A6D  7007                    jo       0xca2a76                       
  0x00CA2A6F  009b01000013            add      byte ptr [ebx + 0x13000001], bl 
  0x00CA2A75  f4                      hlt                                     
                                        ; XREF: 0x00CA2A6D (cond_jump)
  0x00CA2A76  60                      pushal                                  
  0x00CA2A77  001c01                  add      byte ptr [ecx + eax], bl       
  0x00CA2A7A  0000                    add      byte ptr [eax], al             
  0x00CA2A7C  90                      nop                                     
  0x00CA2A7D  2406                    and      al, 6                          
  0x00CA2A7F  0002                    add      byte ptr [edx], al             
  0x00CA2A81  0000                    add      byte ptr [eax], al             
  0x00CA2A83  008e58070080            add      byte ptr [esi - 0x7ffff8a8], cl 
  0x00CA2A89  100d00ce0000            adc      byte ptr [0xce00], cl          
  0x00CA2A8F  00cc                    add      ah, cl                         
  0x00CA2A91  0f05                    syscall                                 
  0x00CA2A93  0000                    add      byte ptr [eax], al             
  0x00CA2A95  f4                      hlt                                     
  0x00CA2A96  64009e01000000          add      byte ptr fs:[esi + 1], bl      
  0x00CA2A9D  0c22                    or       al, 0x22                       
  0x00CA2A9F  008040010000            add      byte ptr [eax + 0x140], al     
  0x00CA2AA5  90                      nop                                     
  0x00CA2AA6  2100                    and      dword ptr [eax], eax           
  0x00CA2AA8  80f00b                  xor      al, 0xb                        
  0x00CA2AAB  005702                  add      byte ptr [edi + 2], dl         
  0x00CA2AAE  0000                    add      byte ptr [eax], al             
  0x00CA2AB0  80f00b                  xor      al, 0xb                        
  0x00CA2AB3  00c2                    add      dl, al                         
  0x00CA2AB5  0200                    add      al, byte ptr [eax]             
  0x00CA2AB7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2ABA  0000                    add      byte ptr [eax], al             
  0x00CA2ABC  00f4                    add      ah, dh                         
  0x00CA2ABE  64009e01000000          add      byte ptr fs:[esi + 1], bl      
  0x00CA2AC5  0c22                    or       al, 0x22                       
  0x00CA2AC7  008040010000            add      byte ptr [eax + 0x140], al     
  0x00CA2ACD  90                      nop                                     
  0x00CA2ACE  2100                    and      dword ptr [eax], eax           
  0x00CA2AD0  80f00b                  xor      al, 0xb                        
  0x00CA2AD3  006902                  add      byte ptr [ecx + 2], ch         
  0x00CA2AD6  0000                    add      byte ptr [eax], al             
  0x00CA2AD8  80f00b                  xor      al, 0xb                        
  0x00CA2ADB  00c2                    add      dl, al                         
  0x00CA2ADD  0200                    add      al, byte ptr [eax]             
  0x00CA2ADF  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2AE2  0000                    add      byte ptr [eax], al             
  0x00CA2AE4  00f4                    add      ah, dh                         
  0x00CA2AE6  64009e01000000          add      byte ptr fs:[esi + 1], bl      
  0x00CA2AED  0c22                    or       al, 0x22                       
  0x00CA2AEF  008040010000            add      byte ptr [eax + 0x140], al     
  0x00CA2AF5  90                      nop                                     
  0x00CA2AF6  2100                    and      dword ptr [eax], eax           
  0x00CA2AF8  004e23                  add      byte ptr [esi + 0x23], cl      
  0x00CA2AFB  008540010042            add      byte ptr [ebp + 0x42000140], al 
  0x00CA2B01  100d00060000            adc      byte ptr [0x600], cl           
  0x00CA2B07  0080f00b007b            add      byte ptr [eax + 0x7b000bf0], al 
  0x00CA2B0D  0200                    add      al, byte ptr [eax]             
  0x00CA2B0F  00c0                    add      al, al                         
  0x00CA2B11  100d00040000            adc      byte ptr [0x400], cl           
  0x00CA2B17  0080f00b0092            add      byte ptr [eax - 0x6dfff410], al 
  0x00CA2B1D  0200                    add      al, byte ptr [eax]             
  0x00CA2B1F  0080f00b00c2            add      byte ptr [eax - 0x3dfff410], al 
  0x00CA2B25  0200                    add      al, byte ptr [eax]             
  0x00CA2B27  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2B2A  0000                    add      byte ptr [eax], al             
  0x00CA2B2C  00f4                    add      ah, dh                         
  0x00CA2B2E  64009e01000000          add      byte ptr fs:[esi + 1], bl      
  0x00CA2B35  0c22                    or       al, 0x22                       
  0x00CA2B37  00c0                    add      al, al                         
  0x00CA2B39  40                      inc      eax                            
  0x00CA2B3A  0100                    add      dword ptr [eax], eax           
  0x00CA2B3C  0018                    add      byte ptr [eax], bl             
  0x00CA2B3E  0000                    add      byte ptr [eax], al             
  0x00CA2B40  0090210080f0            add      byte ptr [eax - 0xf7fffdf], dl 
  0x00CA2B46  0b00                    or       eax, dword ptr [eax]           
  0x00CA2B48  57                      push     edi                            
  0x00CA2B49  0200                    add      al, byte ptr [eax]             
  0x00CA2B4B  0080f00b00c2            add      byte ptr [eax - 0x3dfff410], al 
  0x00CA2B51  0200                    add      al, byte ptr [eax]             
  0x00CA2B53  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2B56  0000                    add      byte ptr [eax], al             
  0x00CA2B58  00f4                    add      ah, dh                         
  0x00CA2B5A  64009e01000000          add      byte ptr fs:[esi + 1], bl      
  0x00CA2B61  0c22                    or       al, 0x22                       
  0x00CA2B63  00c0                    add      al, al                         
  0x00CA2B65  40                      inc      eax                            
  0x00CA2B66  0100                    add      dword ptr [eax], eax           
  0x00CA2B68  0018                    add      byte ptr [eax], bl             
  0x00CA2B6A  0000                    add      byte ptr [eax], al             
  0x00CA2B6C  0090210080f0            add      byte ptr [eax - 0xf7fffdf], dl 
  0x00CA2B72  0b00                    or       eax, dword ptr [eax]           
  0x00CA2B74  6902000080f0            imul     eax, dword ptr [edx], 0xf0800000 
  0x00CA2B7A  0b00                    or       eax, dword ptr [eax]           
  0x00CA2B7C  c20200                  ret      2                              
  0x00CA2B7F  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2B82  0000                    add      byte ptr [eax], al             
  0x00CA2B84  00f4                    add      ah, dh                         
  0x00CA2B86  64009e01000000          add      byte ptr fs:[esi + 1], bl      
  0x00CA2B8D  0c22                    or       al, 0x22                       
  0x00CA2B8F  00c0                    add      al, al                         
  0x00CA2B91  40                      inc      eax                            
  0x00CA2B92  0100                    add      dword ptr [eax], eax           
  0x00CA2B94  0028                    add      byte ptr [eax], ch             
  0x00CA2B96  0000                    add      byte ptr [eax], al             
  0x00CA2B98  0090210080f0            add      byte ptr [eax - 0xf7fffdf], dl 
  0x00CA2B9E  0b00                    or       eax, dword ptr [eax]           
  0x00CA2BA0  57                      push     edi                            
  0x00CA2BA1  0200                    add      al, byte ptr [eax]             
  0x00CA2BA3  0080f00b00c2            add      byte ptr [eax - 0x3dfff410], al 
  0x00CA2BA9  0200                    add      al, byte ptr [eax]             
  0x00CA2BAB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2BAE  0000                    add      byte ptr [eax], al             
  0x00CA2BB0  00f4                    add      ah, dh                         
  0x00CA2BB2  64009e01000000          add      byte ptr fs:[esi + 1], bl      
  0x00CA2BB9  0c22                    or       al, 0x22                       
  0x00CA2BBB  00c0                    add      al, al                         
  0x00CA2BBD  40                      inc      eax                            
  0x00CA2BBE  0100                    add      dword ptr [eax], eax           
  0x00CA2BC0  0028                    add      byte ptr [eax], ch             
  0x00CA2BC2  0000                    add      byte ptr [eax], al             
  0x00CA2BC4  0090210080f0            add      byte ptr [eax - 0xf7fffdf], dl 
  0x00CA2BCA  0b00                    or       eax, dword ptr [eax]           
  0x00CA2BCC  6902000080f0            imul     eax, dword ptr [edx], 0xf0800000 
  0x00CA2BD2  0b00                    or       eax, dword ptr [eax]           
  0x00CA2BD4  c20200                  ret      2                              
  0x00CA2BD7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2BDA  0000                    add      byte ptr [eax], al             
  0x00CA2BDC  80f00b                  xor      al, 0xb                        
  0x00CA2BDF  00b102000000            add      byte ptr [ecx + 2], dh         
  0x00CA2BE5  95                      xchg     ebp, eax                       
  0x00CA2BE6  2200                    and      al, byte ptr [eax]             
  0x00CA2BE8  008c2200c64001          add      byte ptr [edx + 0x140c600], cl 
  0x00CA2BEF  00ff                    add      bh, bh                         
  0x00CA2BF1  3f                      aas                                     
  0x00CA2BF2  0000                    add      byte ptr [eax], al             
  0x00CA2BF4  c24001                  ret      0x140                          
  0x00CA2BF7  0000                    add      byte ptr [eax], al             
  0x00CA2BF9  40                      inc      eax                            
  0x00CA2BFA  0000                    add      byte ptr [eax], al             
  0x00CA2BFC  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2BFF  0000                    add      byte ptr [eax], al             
  0x00CA2C01  f4                      hlt                                     
  0x00CA2C02  54                      push     esp                            
  0x00CA2C03  00e0                    add      al, ah                         
  0x00CA2C05  5b                      pop      ebx                            
  0x00CA2C06  0000                    add      byte ptr [eax], al             
  0x00CA2C08  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2C0B  00985d070090            add      byte ptr [eax - 0x6ffff8a3], bl 
  0x00CA2C11  5d                      pop      ebp                            
  0x00CA2C12  07                      pop      es                             
  0x00CA2C13  0000                    add      byte ptr [eax], al             
  0x00CA2C15  2e2200                  and      al, byte ptr cs:[eax]          
  0x00CA2C18  841e                    test     byte ptr [esi], bl             
  0x00CA2C1A  0c00                    or       al, 0                          
  0x00CA2C1C  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2C1F  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2C22  0000                    add      byte ptr [eax], al             
  0x00CA2C24  80f00b                  xor      al, 0xb                        
  0x00CA2C27  00b102000000            add      byte ptr [ecx + 2], dh         
  0x00CA2C2D  95                      xchg     ebp, eax                       
  0x00CA2C2E  2200                    and      al, byte ptr [eax]             
  0x00CA2C30  008c2200c64001          add      byte ptr [edx + 0x140c600], cl 
  0x00CA2C37  00ff                    add      bh, bh                         
  0x00CA2C39  3f                      aas                                     
  0x00CA2C3A  0000                    add      byte ptr [eax], al             
  0x00CA2C3C  c24001                  ret      0x140                          
  0x00CA2C3F  0000                    add      byte ptr [eax], al             
  0x00CA2C41  40                      inc      eax                            
  0x00CA2C42  0000                    add      byte ptr [eax], al             
  0x00CA2C44  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2C47  0000                    add      byte ptr [eax], al             
  0x00CA2C49  f4                      hlt                                     
  0x00CA2C4A  54                      push     esp                            
  0x00CA2C4B  00e2                    add      dl, ah                         
  0x00CA2C4D  5b                      pop      ebx                            
  0x00CA2C4E  0000                    add      byte ptr [eax], al             
  0x00CA2C50  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2C53  00985d070090            add      byte ptr [eax - 0x6ffff8a3], bl 
  0x00CA2C59  5d                      pop      ebp                            
  0x00CA2C5A  07                      pop      es                             
  0x00CA2C5B  0000                    add      byte ptr [eax], al             
  0x00CA2C5D  2e2200                  and      al, byte ptr cs:[eax]          
  0x00CA2C60  841e                    test     byte ptr [esi], bl             
  0x00CA2C62  0c00                    or       al, 0                          
  0x00CA2C64  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2C67  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2C6A  0000                    add      byte ptr [eax], al             
  0x00CA2C6C  80f00b                  xor      al, 0xb                        
  0x00CA2C6F  00b102000000            add      byte ptr [ecx + 2], dh         
  0x00CA2C75  95                      xchg     ebp, eax                       
  0x00CA2C76  2200                    and      al, byte ptr [eax]             
  0x00CA2C78  008c2200c64001          add      byte ptr [edx + 0x140c600], cl 
  0x00CA2C7F  00ff                    add      bh, bh                         
  0x00CA2C81  3f                      aas                                     
  0x00CA2C82  0000                    add      byte ptr [eax], al             
  0x00CA2C84  c24001                  ret      0x140                          
  0x00CA2C87  0000                    add      byte ptr [eax], al             
  0x00CA2C89  40                      inc      eax                            
  0x00CA2C8A  0000                    add      byte ptr [eax], al             
  0x00CA2C8C  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2C8F  0000                    add      byte ptr [eax], al             
  0x00CA2C91  f4                      hlt                                     
  0x00CA2C92  54                      push     esp                            
  0x00CA2C93  0002                    add      byte ptr [edx], al             
  0x00CA2C95  46                      inc      esi                            
  0x00CA2C96  0000                    add      byte ptr [eax], al             
  0x00CA2C98  002f                    add      byte ptr [edi], ch             
  0x00CA2C9A  2200                    and      al, byte ptr [eax]             
  0x00CA2C9C  8b1e                    mov      ebx, dword ptr [esi]           
  0x00CA2C9E  0c00                    or       al, 0                          
  0x00CA2CA0  00e5                    add      ch, ah                         
  0x00CA2CA2  2100                    and      dword ptr [eax], eax           
  0x00CA2CA4  6200                    bound    eax, qword ptr [eax]           
  0x00CA2CA6  2000                    and      byte ptr [eax], al             
  0x00CA2CA8  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2CAB  00985d070000            add      byte ptr [eax + 0x75d], bl     
  0x00CA2CB1  8e23                    mov      fs, word ptr [ebx]             
  0x00CA2CB3  009c1e0c000004          add      byte ptr [esi + ebx + 0x400000c], bl 
  0x00CA2CBA  2200                    and      al, byte ptr [eax]             
  0x00CA2CBC  40                      inc      eax                            
  0x00CA2CBD  0020                    add      byte ptr [eax], ah             
  0x00CA2CBF  008c5d07000c00          add      byte ptr [ebp + ebx*2 + 0xc0007], cl 
  0x00CA2CC6  0000                    add      byte ptr [eax], al             
  0x00CA2CC8  80f00b                  xor      al, 0xb                        
  0x00CA2CCB  00b102000000            add      byte ptr [ecx + 2], dh         
  0x00CA2CD1  95                      xchg     ebp, eax                       
  0x00CA2CD2  2200                    and      al, byte ptr [eax]             
  0x00CA2CD4  008c2200c64001          add      byte ptr [edx + 0x140c600], cl 
  0x00CA2CDB  00ff                    add      bh, bh                         
  0x00CA2CDD  3f                      aas                                     
  0x00CA2CDE  0000                    add      byte ptr [eax], al             
  0x00CA2CE0  c24001                  ret      0x140                          
  0x00CA2CE3  0000                    add      byte ptr [eax], al             
  0x00CA2CE5  40                      inc      eax                            
  0x00CA2CE6  0000                    add      byte ptr [eax], al             
  0x00CA2CE8  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2CEB  0000                    add      byte ptr [eax], al             
  0x00CA2CED  f4                      hlt                                     
  0x00CA2CEE  54                      push     esp                            
  0x00CA2CEF  0003                    add      byte ptr [ebx], al             
  0x00CA2CF1  06                      push     es                             
  0x00CA2CF2  0000                    add      byte ptr [eax], al             
  0x00CA2CF4  002f                    add      byte ptr [edi], ch             
  0x00CA2CF6  2200                    and      al, byte ptr [eax]             
  0x00CA2CF8  8b1e                    mov      ebx, dword ptr [esi]           
  0x00CA2CFA  0c00                    or       al, 0                          
  0x00CA2CFC  00e5                    add      ch, ah                         
  0x00CA2CFE  2100                    and      dword ptr [eax], eax           
  0x00CA2D00  6200                    bound    eax, qword ptr [eax]           
  0x00CA2D02  2000                    and      byte ptr [eax], al             
  0x00CA2D04  000f                    add      byte ptr [edi], cl             
  0x00CA2D06  2300                    and      eax, dword ptr [eax]           
  0x00CA2D08  9d                      popfd                                   
  0x00CA2D09  1e                      push     ds                             
  0x00CA2D0A  0c00                    or       al, 0                          
  0x00CA2D0C  00e5                    add      ch, ah                         
  0x00CA2D0E  2100                    and      dword ptr [eax], eax           
  0x00CA2D10  6200                    bound    eax, qword ptr [eax]           
  0x00CA2D12  2000                    and      byte ptr [eax], al             
  0x00CA2D14  8c5d07                  mov      word ptr [ebp + 7], ds         
  0x00CA2D17  0000                    add      byte ptr [eax], al             
  0x00CA2D19  0f2300                  mov      dr0, eax                       
  0x00CA2D1C  891e                    mov      dword ptr [esi], ebx           
  0x00CA2D1E  0c00                    or       al, 0                          
  0x00CA2D20  004523                  add      byte ptr [ebp + 0x23], al      
  0x00CA2D23  006800                  add      byte ptr [eax], ch             
  0x00CA2D26  2000                    and      byte ptr [eax], al             
  0x00CA2D28  8d5d07                  lea      ebx, [ebp + 7]                 
  0x00CA2D2B  0000                    add      byte ptr [eax], al             
  0x00CA2D2D  8e23                    mov      fs, word ptr [ebx]             
  0x00CA2D2F  009c1e0c000004          add      byte ptr [esi + ebx + 0x400000c], bl 
  0x00CA2D36  2200                    and      al, byte ptr [eax]             
  0x00CA2D38  40                      inc      eax                            
  0x00CA2D39  0020                    add      byte ptr [eax], ah             
  0x00CA2D3B  008c5d07000c00          add      byte ptr [ebp + ebx*2 + 0xc0007], cl 
  0x00CA2D42  0000                    add      byte ptr [eax], al             
  0x00CA2D44  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA2D45  96                      xchg     esi, eax                       
  0x00CA2D46  0a00                    or       al, byte ptr [eax]             
  0x00CA2D48  b102                    mov      cl, 2                          
  0x00CA2D4A  0000                    add      byte ptr [eax], al             
  0x00CA2D4C  85f4                    test     esp, esi                       
  0x00CA2D4E  0800                    or       byte ptr [eax], al             
  0x00CA2D50  800000                  add      byte ptr [eax], 0              
  0x00CA2D53  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2D56  0000                    add      byte ptr [eax], al             
  0x00CA2D58  96                      xchg     esi, eax                       
  0x00CA2D59  f4                      hlt                                     
  0x00CA2D5A  0800                    or       byte ptr [eax], al             
  0x00CA2D5C  0100                    add      dword ptr [eax], eax           
  0x00CA2D5E  0000                    add      byte ptr [eax], al             
  0x00CA2D60  84960a00b802            test     byte ptr [esi + 0x2b8000a], dl 
  0x00CA2D66  0000                    add      byte ptr [eax], al             
  0x00CA2D68  0c00                    or       al, 0                          
  0x00CA2D6A  0000                    add      byte ptr [eax], al             
  0x00CA2D6C  aa                      stosb    byte ptr es:[edi], al          
  0x00CA2D6D  850a                    test     dword ptr [edx], ecx           
  0x00CA2D6F  00df                    add      bh, bl                         
  0x00CA2D71  0200                    add      al, byte ptr [eax]             
  0x00CA2D73  0087850a00bb            add      byte ptr [edi - 0x44fff57b], al 
  0x00CA2D79  0200                    add      al, byte ptr [eax]             
  0x00CA2D7B  0085f4080080            add      byte ptr [ebp - 0x7ffff70c], al 
  0x00CA2D81  0000                    add      byte ptr [eax], al             
  0x00CA2D83  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2D86  0000                    add      byte ptr [eax], al             
  0x00CA2D88  008e2200c040            add      byte ptr [esi + 0x40c00022], cl 
  0x00CA2D8E  0100                    add      dword ptr [eax], eax           
  0x00CA2D90  0028                    add      byte ptr [eax], ch             
  0x00CA2D92  0000                    add      byte ptr [eax], al             
  0x00CA2D94  14ce                    adc      al, 0xce                       
  0x00CA2D96  0800                    or       byte ptr [eax], al             
  0x00CA2D98  80f00b                  xor      al, 0xb                        
  0x00CA2D9B  00b60200001b            add      byte ptr [esi + 0x1b000002], dh 
  0x00CA2DA1  0020                    add      byte ptr [eax], ah             
  0x00CA2DA3  0000                    add      byte ptr [eax], al             
  0x00CA2DA5  af                      scasd    eax, dword ptr es:[edi]        
  0x00CA2DA6  2300                    and      eax, dword ptr [eax]           
  0x00CA2DA8  8d4001                  lea      eax, [eax + 1]                 
  0x00CA2DAB  004a10                  add      byte ptr [edx + 0x10], cl      
  0x00CA2DAE  0d00040000              or       eax, 0x400                     
  0x00CA2DB3  0080f00b00bb            add      byte ptr [eax - 0x44fff410], al 
  0x00CA2DB9  0200                    add      al, byte ptr [eax]             
  0x00CA2DBB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA2DBE  0000                    add      byte ptr [eax], al             
  0x00CA2DC0  85f4                    test     esp, esi                       
  0x00CA2DC2  0800                    or       byte ptr [eax], al             
  0x00CA2DC4  ff0f                    dec      dword ptr [edi]                
  0x00CA2DC6  0000                    add      byte ptr [eax], al             
  0x00CA2DC8  84f4                    test     ah, dh                         
  0x00CA2DCA  0800                    or       byte ptr [eax], al             
  0x00CA2DCC  0100                    add      dword ptr [eax], eax           
  0x00CA2DCE  0000                    add      byte ptr [eax], al             
  0x00CA2DD0  8af4                    mov      dh, ah                         
  0x00CA2DD2  0800                    or       byte ptr [eax], al             
  0x00CA2DD4  0000                    add      byte ptr [eax], al             
  0x00CA2DD6  0000                    add      byte ptr [eax], al             
  0x00CA2DD8  00f4                    add      ah, dh                         
  0x00CA2DDA  44                      inc      esp                            
  0x00CA2DDB  0000                    add      byte ptr [eax], al             
  0x00CA2DDD  40                      inc      eax                            
  0x00CA2DDE  0000                    add      byte ptr [eax], al             
  0x00CA2DE0  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2DE3  00d5                    add      ch, dl                         
  0x00CA2DE6  ff00                    inc      dword ptr [eax]                
  0x00CA2DE8  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2DEB  00d4                    add      ah, dl                         
  0x00CA2DEE  ff00                    inc      dword ptr [eax]                
  0x00CA2DF0  97                      xchg     edi, eax                       
  0x00CA2DF1  f4                      hlt                                     
  0x00CA2DF2  0800                    or       byte ptr [eax], al             
  0x00CA2DF4  0000                    add      byte ptr [eax], al             
  0x00CA2DF6  0000                    add      byte ptr [eax], al             
  0x00CA2DF8  0c00                    or       al, 0                          
  0x00CA2DFA  0000                    add      byte ptr [eax], al             
  0x00CA2DFC  000c0500401bd0          add      byte ptr [eax - 0x2fe4c000], cl 
  0x00CA2E03  00b302000060            add      byte ptr [ebx + 0x60000002], dh 
  0x00CA2E09  0101                    add      dword ptr [ecx], eax           
  0x00CA2E0B  0053c2                  add      byte ptr [ebx - 0x3e], dl      
  0x00CA2E0E  b900400c05              mov      ecx, 0x50c4000                 
  0x00CA2E13  0037                    add      byte ptr [edi], dh             
  0x00CA2E15  0c04                    or       al, 4                          
  0x00CA2E17  00a78a040084            add      byte ptr [edi - 0x7bfffb76], ah 
  0x00CA2E1D  180500b1b705            sbb      byte ptr [0x5b7b100], al       
  0x00CA2E23  004a6a                  add      byte ptr [edx + 0x6a], cl      
  0x00CA2E26  06                      push     es                             
  0x00CA2E27  00ae32070085            add      byte ptr [esi - 0x7afff8ce], ch 
  0x00CA2E2D  1308                    adc      ecx, dword ptr [eax]           
  0x00CA2E2F  00cc                    add      ah, cl                         
  0x00CA2E31  0f09                    wbinvd                                  
  0x00CA2E33  00db                    add      bl, bl                         
  0x00CA2E35  2a0a                    sub      cl, byte ptr [edx]             
  0x00CA2E37  007368                  add      byte ptr [ebx + 0x68], dh      
  0x00CA2E3A  0b00                    or       eax, dword ptr [eax]           
  0x00CA2E3C  cdcc                    int      0xcc                           
  0x00CA2E3E  0c00                    or       al, 0                          
  0x00CA2E40  a15c0e003f              mov      eax, dword ptr [0x3f000e5c]    
  0x00CA2E45  1d10009a14              sbb      eax, 0x149a0010                
  0x00CA2E4A  1200                    adc      al, byte ptr [eax]             
  0x00CA2E4C  61                      popal                                   
  0x00CA2E4D  49                      dec      ecx                            
  0x00CA2E4E  1400                    adc      al, 0                          
  0x00CA2E50  11c3                    adc      ebx, eax                       
  0x00CA2E52  16                      push     ss                             
  0x00CA2E53  0013                    add      byte ptr [ebx], dl             
  0x00CA2E55  8a19                    mov      bl, byte ptr [ecx]             
  0x00CA2E57  00d7                    add      bh, dl                         
  0x00CA2E59  a7                      cmpsd    dword ptr [esi], dword ptr es:[edi] 
  0x00CA2E5A  1c00                    sbb      al, 0                          
  0x00CA2E5C  f3262000                and      byte ptr es:[eax], al          
  0x00CA2E60  47                      inc      edi                            
  0x00CA2E61  132400                  adc      esp, dword ptr [eax + eax]     
  0x00CA2E64  27                      daa                                     
  0x00CA2E65  7a28                    jp       0xca2e8f                       
  0x00CA2E67  00866a2d002d            add      byte ptr [esi + 0x2d002d6a], al 
  0x00CA2E6D  f5                      cmc                                     
  0x00CA2E6E  3200                    xor      al, byte ptr [eax]             
  0x00CA2E70  ee                      out      dx, al                         
  0x00CA2E71  2c39                    sub      al, 0x39                       
  0x00CA2E73  00e7                    add      bh, ah                         
  0x00CA2E75  2640                    inc      eax                            
  0x00CA2E77  00cd                    add      ch, cl                         
  0x00CA2E79  fa                      cli                                     
  0x00CA2E7A  47                      inc      edi                            
  0x00CA2E7B  0036                    add      byte ptr [esi], dh             
  0x00CA2E7D  c3                      ret                                     
  0x00CA2E7E  50                      push     eax                            
  0x00CA2E7F  00f8                    add      al, bh                         
  0x00CA2E81  9d                      popfd                                   
  0x00CA2E82  5a                      pop      edx                            
  0x00CA2E83  008cac65008314          add      byte ptr [esp + ebp*4 + 0x14830065], cl 
  0x00CA2E8A  7200                    jb       0xca2e8c                       
  0x00CA2E8E  7f00                    jg       0xca2e90                       
                                        ; XREF: 0x00CA2E8E (cond_jump)
  0x00CA2E90  007060                  add      byte ptr [eax + 0x60], dh      
  0x00CA2E93  002e                    add      byte ptr [esi], ch             
  0x00CA2E95  06                      push     es                             
  0x00CA2E96  0000                    add      byte ptr [eax], al             
  0x00CA2E98  0b00                    or       eax, dword ptr [eax]           
  0x00CA2E9A  2000                    and      byte ptr [eax], al             
  0x00CA2E9C  06                      push     es                             
  0x00CA2E9D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA2E9E  050000f444              add      eax, 0x44f40000                
  0x00CA2EA3  0002                    add      byte ptr [edx], al             
  0x00CA2EA5  800000                  add      byte ptr [eax], 0              
  0x00CA2EA8  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2EAB  000406                  add      byte ptr [esi + eax], al       
  0x00CA2EAE  0000                    add      byte ptr [eax], al             
  0x00CA2EB0  050c050000              add      eax, 0x50c                     
  0x00CA2EB5  f4                      hlt                                     
  0x00CA2EB6  44                      inc      esp                            
  0x00CA2EB7  0002                    add      byte ptr [edx], al             
  0x00CA2EB9  0000                    add      byte ptr [eax], al             
  0x00CA2EBB  0000                    add      byte ptr [eax], al             
  0x00CA2EBD  7044                    jo       0xca2f03                       
  0x00CA2EBF  000406                  add      byte ptr [esi + eax], al       
  0x00CA2EC2  0000                    add      byte ptr [eax], al             
  0x00CA2EC4  00f4                    add      ah, dh                         
  0x00CA2EC6  44                      inc      esp                            
  0x00CA2EC7  000a                    add      byte ptr [edx], cl             
  0x00CA2EC9  0000                    add      byte ptr [eax], al             
  0x00CA2ECB  0000                    add      byte ptr [eax], al             
  0x00CA2ECD  7044                    jo       0xca2f13                       
  0x00CA2ECF  0000                    add      byte ptr [eax], al             
  0x00CA2ED1  06                      push     es                             
  0x00CA2ED2  0000                    add      byte ptr [eax], al             
  0x00CA2ED4  00f4                    add      ah, dh                         
  0x00CA2ED6  44                      inc      esp                            
  0x00CA2ED7  000a                    add      byte ptr [edx], cl             
  0x00CA2ED9  06                      push     es                             
  0x00CA2EDA  0000                    add      byte ptr [eax], al             
  0x00CA2EDC  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2EDF  0001                    add      byte ptr [ecx], al             
  0x00CA2EE1  06                      push     es                             
  0x00CA2EE2  0000                    add      byte ptr [eax], al             
  0x00CA2EE4  00f4                    add      ah, dh                         
  0x00CA2EE6  44                      inc      esp                            
  0x00CA2EE7  0010                    add      byte ptr [eax], dl             
  0x00CA2EE9  06                      push     es                             
  0x00CA2EEA  0000                    add      byte ptr [eax], al             
  0x00CA2EEC  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2EEF  0002                    add      byte ptr [edx], al             
  0x00CA2EF1  06                      push     es                             
  0x00CA2EF2  0000                    add      byte ptr [eax], al             
  0x00CA2EF4  00f4                    add      ah, dh                         
  0x00CA2EF6  44                      inc      esp                            
  0x00CA2EF7  0016                    add      byte ptr [esi], dl             
  0x00CA2EF9  06                      push     es                             
  0x00CA2EFA  0000                    add      byte ptr [eax], al             
  0x00CA2EFC  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2EFF  0003                    add      byte ptr [ebx], al             
  0x00CA2F01  06                      push     es                             
  0x00CA2F02  0000                    add      byte ptr [eax], al             
  0x00CA2F04  00f4                    add      ah, dh                         
  0x00CA2F06  44                      inc      esp                            
  0x00CA2F07  001c06                  add      byte ptr [esi + eax], bl       
  0x00CA2F0A  0000                    add      byte ptr [eax], al             
  0x00CA2F0C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2F0F  000506000000            add      byte ptr [6], al               
  0x00CA2F15  f4                      hlt                                     
  0x00CA2F16  44                      inc      esp                            
  0x00CA2F17  0022                    add      byte ptr [edx], ah             
  0x00CA2F19  06                      push     es                             
  0x00CA2F1A  0000                    add      byte ptr [eax], al             
  0x00CA2F1C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2F1F  0006                    add      byte ptr [esi], al             
  0x00CA2F21  06                      push     es                             
  0x00CA2F22  0000                    add      byte ptr [eax], al             
  0x00CA2F24  00f4                    add      ah, dh                         
  0x00CA2F26  44                      inc      esp                            
  0x00CA2F27  0028                    add      byte ptr [eax], ch             
  0x00CA2F29  06                      push     es                             
  0x00CA2F2A  0000                    add      byte ptr [eax], al             
  0x00CA2F2C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA2F2F  0007                    add      byte ptr [edi], al             
  0x00CA2F31  06                      push     es                             
  0x00CA2F32  0000                    add      byte ptr [eax], al             
  0x00CA2F34  00f4                    add      ah, dh                         
  0x00CA2F36  44                      inc      esp                            
  0x00CA2F37  0000                    add      byte ptr [eax], al             
  0x00CA2F39  0000                    add      byte ptr [eax], al             
  0x00CA2F3B  0000                    add      byte ptr [eax], al             
  0x00CA2F3D  7044                    jo       0xca2f83                       
  0x00CA2F3F  0008                    add      byte ptr [eax], cl             
  0x00CA2F41  06                      push     es                             
  0x00CA2F42  0000                    add      byte ptr [eax], al             
  0x00CA2F44  00f4                    add      ah, dh                         
  0x00CA2F46  44                      inc      esp                            
  0x00CA2F47  0000                    add      byte ptr [eax], al             
  0x00CA2F49  0100                    add      dword ptr [eax], eax           
  0x00CA2F4B  0000                    add      byte ptr [eax], al             
  0x00CA2F4D  7044                    jo       0xca2f93                       
  0x00CA2F4F  0009                    add      byte ptr [ecx], cl             
  0x00CA2F51  06                      push     es                             
  0x00CA2F52  0000                    add      byte ptr [eax], al             
  0x00CA2F54  00f4                    add      ah, dh                         
  0x00CA2F56  60                      pushal                                  
  0x00CA2F57  000a                    add      byte ptr [edx], cl             
  0x00CA2F59  06                      push     es                             
  0x00CA2F5A  0000                    add      byte ptr [eax], al             
  0x00CA2F5C  00f4                    add      ah, dh                         
  0x00CA2F5E  44                      inc      esp                            
  0x00CA2F5F  0000                    add      byte ptr [eax], al             
  0x00CA2F61  0000                    add      byte ptr [eax], al             
  0x00CA2F63  0000                    add      byte ptr [eax], al             
  0x00CA2F65  58                      pop      eax                            
  0x00CA2F66  44                      inc      esp                            
  0x00CA2F67  0000                    add      byte ptr [eax], al             
  0x00CA2F69  f4                      hlt                                     
  0x00CA2F6A  44                      inc      esp                            
  0x00CA2F6B  0000                    add      byte ptr [eax], al             
  0x00CA2F6D  0100                    add      dword ptr [eax], eax           
  0x00CA2F6F  0000                    add      byte ptr [eax], al             
  0x00CA2F71  58                      pop      eax                            
  0x00CA2F72  44                      inc      esp                            
  0x00CA2F73  0000                    add      byte ptr [eax], al             
  0x00CA2F75  f4                      hlt                                     
  0x00CA2F76  44                      inc      esp                            
  0x00CA2F77  0000                    add      byte ptr [eax], al             
  0x00CA2F79  0200                    add      al, byte ptr [eax]             
  0x00CA2F7B  0000                    add      byte ptr [eax], al             
  0x00CA2F7D  58                      pop      eax                            
  0x00CA2F7E  44                      inc      esp                            
  0x00CA2F7F  0000                    add      byte ptr [eax], al             
  0x00CA2F81  f4                      hlt                                     
  0x00CA2F82  44                      inc      esp                            
                                        ; XREF: 0x00CA2F3D (cond_jump)
  0x00CA2F83  0000                    add      byte ptr [eax], al             
  0x00CA2F85  0300                    add      eax, dword ptr [eax]           
  0x00CA2F87  0000                    add      byte ptr [eax], al             
  0x00CA2F89  58                      pop      eax                            
  0x00CA2F8A  44                      inc      esp                            
  0x00CA2F8B  0000                    add      byte ptr [eax], al             
  0x00CA2F8D  f4                      hlt                                     
  0x00CA2F8E  44                      inc      esp                            
  0x00CA2F8F  0000                    add      byte ptr [eax], al             
  0x00CA2F91  0400                    add      al, 0                          
                                        ; XREF: 0x00CA2F4D (cond_jump)
  0x00CA2F93  0000                    add      byte ptr [eax], al             
  0x00CA2F95  58                      pop      eax                            
  0x00CA2F96  44                      inc      esp                            
  0x00CA2F97  0000                    add      byte ptr [eax], al             
  0x00CA2F99  f4                      hlt                                     
  0x00CA2F9A  44                      inc      esp                            
  0x00CA2F9B  00ff                    add      bh, bh                         
  0x00CA2F9E  ff00                    inc      dword ptr [eax]                
  0x00CA2FA0  006044                  add      byte ptr [eax + 0x44], ah      
  0x00CA2FA3  0000                    add      byte ptr [eax], al             
  0x00CA2FA5  f4                      hlt                                     
  0x00CA2FA6  60                      pushal                                  
  0x00CA2FA7  0010                    add      byte ptr [eax], dl             
  0x00CA2FA9  06                      push     es                             
  0x00CA2FAA  0000                    add      byte ptr [eax], al             
  0x00CA2FAC  00f4                    add      ah, dh                         
  0x00CA2FAE  44                      inc      esp                            
  0x00CA2FAF  0001                    add      byte ptr [ecx], al             
  0x00CA2FB1  0000                    add      byte ptr [eax], al             
  0x00CA2FB3  0000                    add      byte ptr [eax], al             
  0x00CA2FB5  58                      pop      eax                            
  0x00CA2FB6  44                      inc      esp                            
  0x00CA2FB7  0000                    add      byte ptr [eax], al             
  0x00CA2FB9  58                      pop      eax                            
  0x00CA2FBA  44                      inc      esp                            
  0x00CA2FBB  0000                    add      byte ptr [eax], al             
  0x00CA2FBD  58                      pop      eax                            
  0x00CA2FBE  44                      inc      esp                            
  0x00CA2FBF  0000                    add      byte ptr [eax], al             
  0x00CA2FC1  58                      pop      eax                            
  0x00CA2FC2  44                      inc      esp                            
  0x00CA2FC3  0000                    add      byte ptr [eax], al             
  0x00CA2FC5  58                      pop      eax                            
  0x00CA2FC6  44                      inc      esp                            
  0x00CA2FC7  0000                    add      byte ptr [eax], al             
  0x00CA2FC9  002400                  add      byte ptr [eax + eax], ah       
  0x00CA2FCC  006044                  add      byte ptr [eax + 0x44], ah      
  0x00CA2FCF  0000                    add      byte ptr [eax], al             
  0x00CA2FD1  f4                      hlt                                     
  0x00CA2FD2  60                      pushal                                  
  0x00CA2FD3  0016                    add      byte ptr [esi], dl             
  0x00CA2FD5  06                      push     es                             
  0x00CA2FD6  0000                    add      byte ptr [eax], al             
  0x00CA2FD8  00f4                    add      ah, dh                         
  0x00CA2FDA  44                      inc      esp                            
  0x00CA2FDB  00ff                    add      bh, bh                         
  0x00CA2FDE  ff00                    inc      dword ptr [eax]                
  0x00CA2FE0  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA2FE3  0000                    add      byte ptr [eax], al             
  0x00CA2FE5  58                      pop      eax                            
  0x00CA2FE6  44                      inc      esp                            
  0x00CA2FE7  0000                    add      byte ptr [eax], al             
  0x00CA2FE9  58                      pop      eax                            
  0x00CA2FEA  44                      inc      esp                            
  0x00CA2FEB  0000                    add      byte ptr [eax], al             
  0x00CA2FED  58                      pop      eax                            
  0x00CA2FEE  44                      inc      esp                            
  0x00CA2FEF  0000                    add      byte ptr [eax], al             
  0x00CA2FF1  58                      pop      eax                            
  0x00CA2FF2  44                      inc      esp                            
  0x00CA2FF3  0000                    add      byte ptr [eax], al             
  0x00CA2FF5  60                      pushal                                  
  0x00CA2FF6  44                      inc      esp                            
  0x00CA2FF7  0000                    add      byte ptr [eax], al             
  0x00CA2FF9  f4                      hlt                                     
  0x00CA2FFA  60                      pushal                                  
  0x00CA2FFB  001c06                  add      byte ptr [esi + eax], bl       
  0x00CA2FFE  0000                    add      byte ptr [eax], al             
  0x00CA3000  00f4                    add      ah, dh                         
  0x00CA3002  44                      inc      esp                            
  0x00CA3003  0000                    add      byte ptr [eax], al             
  0x00CA3005  0400                    add      al, 0                          
  0x00CA3007  0000                    add      byte ptr [eax], al             
  0x00CA3009  58                      pop      eax                            
  0x00CA300A  44                      inc      esp                            
  0x00CA300B  0000                    add      byte ptr [eax], al             
  0x00CA300D  f4                      hlt                                     
  0x00CA300E  44                      inc      esp                            
  0x00CA300F  00ff                    add      bh, bh                         
  0x00CA3012  ff00                    inc      dword ptr [eax]                
  0x00CA3014  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA3017  0000                    add      byte ptr [eax], al             
  0x00CA3019  f4                      hlt                                     
  0x00CA301A  44                      inc      esp                            
  0x00CA301B  0000                    add      byte ptr [eax], al             
  0x00CA301D  0500000058              add      eax, 0x58000000                
  0x00CA3022  44                      inc      esp                            
  0x00CA3023  0000                    add      byte ptr [eax], al             
  0x00CA3025  f4                      hlt                                     
  0x00CA3026  44                      inc      esp                            
  0x00CA3027  00ff                    add      bh, bh                         
  0x00CA302A  ff00                    inc      dword ptr [eax]                
  0x00CA302C  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA302F  0000                    add      byte ptr [eax], al             
  0x00CA3031  f4                      hlt                                     
  0x00CA3032  44                      inc      esp                            
  0x00CA3033  00ff                    add      bh, bh                         
  0x00CA3036  ff00                    inc      dword ptr [eax]                
  0x00CA3038  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA303B  0000                    add      byte ptr [eax], al             
  0x00CA303D  f4                      hlt                                     
  0x00CA303E  44                      inc      esp                            
  0x00CA303F  00ff                    add      bh, bh                         
  0x00CA3042  ff00                    inc      dword ptr [eax]                
  0x00CA3044  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA3047  0000                    add      byte ptr [eax], al             
  0x00CA3049  f4                      hlt                                     
  0x00CA304A  60                      pushal                                  
  0x00CA304B  0022                    add      byte ptr [edx], ah             
  0x00CA304D  06                      push     es                             
  0x00CA304E  0000                    add      byte ptr [eax], al             
  0x00CA3050  00f4                    add      ah, dh                         
  0x00CA3052  44                      inc      esp                            
  0x00CA3053  0001                    add      byte ptr [ecx], al             
  0x00CA3055  0000                    add      byte ptr [eax], al             
  0x00CA3057  0000                    add      byte ptr [eax], al             
  0x00CA3059  58                      pop      eax                            
  0x00CA305A  44                      inc      esp                            
  0x00CA305B  0000                    add      byte ptr [eax], al             
  0x00CA305D  002400                  add      byte ptr [eax + eax], ah       
  0x00CA3060  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA3063  0000                    add      byte ptr [eax], al             
  0x00CA3065  f4                      hlt                                     
  0x00CA3066  44                      inc      esp                            
  0x00CA3067  0001                    add      byte ptr [ecx], al             
  0x00CA3069  0000                    add      byte ptr [eax], al             
  0x00CA306B  0000                    add      byte ptr [eax], al             
  0x00CA306D  58                      pop      eax                            
  0x00CA306E  44                      inc      esp                            
  0x00CA306F  0000                    add      byte ptr [eax], al             
  0x00CA3071  002400                  add      byte ptr [eax + eax], ah       
  0x00CA3074  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA3077  0000                    add      byte ptr [eax], al             
  0x00CA3079  002400                  add      byte ptr [eax + eax], ah       
  0x00CA307C  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA307F  0000                    add      byte ptr [eax], al             
  0x00CA3081  002400                  add      byte ptr [eax + eax], ah       
  0x00CA3084  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA3087  0000                    add      byte ptr [eax], al             
  0x00CA3089  f4                      hlt                                     
  0x00CA308A  60                      pushal                                  
  0x00CA308B  0028                    add      byte ptr [eax], ch             
  0x00CA308D  06                      push     es                             
  0x00CA308E  0000                    add      byte ptr [eax], al             
  0x00CA3090  00f4                    add      ah, dh                         
  0x00CA3092  44                      inc      esp                            
  0x00CA3093  00ff                    add      bh, bh                         
  0x00CA3096  ff00                    inc      dword ptr [eax]                
  0x00CA3098  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA309B  0000                    add      byte ptr [eax], al             
  0x00CA309D  58                      pop      eax                            
  0x00CA309E  44                      inc      esp                            
  0x00CA309F  0000                    add      byte ptr [eax], al             
  0x00CA30A1  58                      pop      eax                            
  0x00CA30A2  44                      inc      esp                            
  0x00CA30A3  0000                    add      byte ptr [eax], al             
  0x00CA30A5  58                      pop      eax                            
  0x00CA30A6  44                      inc      esp                            
  0x00CA30A7  0000                    add      byte ptr [eax], al             
  0x00CA30A9  58                      pop      eax                            
  0x00CA30AA  44                      inc      esp                            
  0x00CA30AB  0000                    add      byte ptr [eax], al             
  0x00CA30AD  58                      pop      eax                            
  0x00CA30AE  44                      inc      esp                            
  0x00CA30AF  0000                    add      byte ptr [eax], al             
  0x00CA30B1  f4                      hlt                                     
  0x00CA30B2  56                      push     esi                            
  0x00CA30B3  0007                    add      byte ptr [edi], al             
  0x00CA30B5  0000                    add      byte ptr [eax], al             
  0x00CA30B7  0000                    add      byte ptr [eax], al             
  0x00CA30B9  f4                      hlt                                     
  0x00CA30BA  60                      pushal                                  
  0x00CA30BB  0000                    add      byte ptr [eax], al             
  0x00CA30BD  0000                    add      byte ptr [eax], al             
  0x00CA30BF  0000                    add      byte ptr [eax], al             
  0x00CA30C1  f4                      hlt                                     
  0x00CA30C2  7000                    jo       0xca30c4                       
                                        ; XREF: 0x00CA30C2 (cond_jump)
  0x00CA30C4  0001                    add      byte ptr [ecx], al             
  0x00CA30C6  0000                    add      byte ptr [eax], al             
  0x00CA30C8  0000                    add      byte ptr [eax], al             
  0x00CA30CA  3900                    cmp      dword ptr [eax], eax           
  0x00CA30CC  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA30CF  0000                    add      byte ptr [eax], al             
  0x00CA30D1  f4                      hlt                                     
  0x00CA30D2  56                      push     esi                            
  0x00CA30D3  0007                    add      byte ptr [edi], al             
  0x00CA30D5  0000                    add      byte ptr [eax], al             
  0x00CA30D7  0000                    add      byte ptr [eax], al             
  0x00CA30D9  f4                      hlt                                     
  0x00CA30DA  60                      pushal                                  
  0x00CA30DB  0000                    add      byte ptr [eax], al             
  0x00CA30DD  0100                    add      dword ptr [eax], eax           
  0x00CA30DF  0000                    add      byte ptr [eax], al             
  0x00CA30E1  f4                      hlt                                     
  0x00CA30E2  7000                    jo       0xca30e4                       
                                        ; XREF: 0x00CA30E2 (cond_jump)
  0x00CA30E4  0001                    add      byte ptr [ecx], al             
  0x00CA30E6  0000                    add      byte ptr [eax], al             
  0x00CA30E8  0001                    add      byte ptr [ecx], al             
  0x00CA30EA  3900                    cmp      dword ptr [eax], eax           
  0x00CA30EC  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA30EF  0000                    add      byte ptr [eax], al             
  0x00CA30F1  f4                      hlt                                     
  0x00CA30F2  56                      push     esi                            
  0x00CA30F3  0007                    add      byte ptr [edi], al             
  0x00CA30F5  0000                    add      byte ptr [eax], al             
  0x00CA30F7  0000                    add      byte ptr [eax], al             
  0x00CA30F9  f4                      hlt                                     
  0x00CA30FA  60                      pushal                                  
  0x00CA30FB  0000                    add      byte ptr [eax], al             
  0x00CA30FD  0200                    add      al, byte ptr [eax]             
  0x00CA30FF  0000                    add      byte ptr [eax], al             
  0x00CA3101  f4                      hlt                                     
  0x00CA3102  7000                    jo       0xca3104                       
                                        ; XREF: 0x00CA3102 (cond_jump)
  0x00CA3104  0001                    add      byte ptr [ecx], al             
  0x00CA3106  0000                    add      byte ptr [eax], al             
  0x00CA3108  0002                    add      byte ptr [edx], al             
  0x00CA310A  3900                    cmp      dword ptr [eax], eax           
  0x00CA310C  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA310F  0000                    add      byte ptr [eax], al             
  0x00CA3111  f4                      hlt                                     
  0x00CA3112  56                      push     esi                            
  0x00CA3113  0007                    add      byte ptr [edi], al             
  0x00CA3115  0000                    add      byte ptr [eax], al             
  0x00CA3117  0000                    add      byte ptr [eax], al             
  0x00CA3119  f4                      hlt                                     
  0x00CA311A  60                      pushal                                  
  0x00CA311B  0000                    add      byte ptr [eax], al             
  0x00CA311D  0300                    add      eax, dword ptr [eax]           
  0x00CA311F  0000                    add      byte ptr [eax], al             
  0x00CA3121  f4                      hlt                                     
  0x00CA3122  7000                    jo       0xca3124                       
                                        ; XREF: 0x00CA3122 (cond_jump)
  0x00CA3124  0001                    add      byte ptr [ecx], al             
  0x00CA3126  0000                    add      byte ptr [eax], al             
  0x00CA3128  0003                    add      byte ptr [ebx], al             
  0x00CA312A  3900                    cmp      dword ptr [eax], eax           
  0x00CA312C  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA312F  0000                    add      byte ptr [eax], al             
  0x00CA3131  f4                      hlt                                     
  0x00CA3132  56                      push     esi                            
  0x00CA3133  0007                    add      byte ptr [edi], al             
  0x00CA3135  0000                    add      byte ptr [eax], al             
  0x00CA3137  0000                    add      byte ptr [eax], al             
  0x00CA3139  f4                      hlt                                     
  0x00CA313A  60                      pushal                                  
  0x00CA313B  0000                    add      byte ptr [eax], al             
  0x00CA313D  0400                    add      al, 0                          
  0x00CA313F  0000                    add      byte ptr [eax], al             
  0x00CA3141  f4                      hlt                                     
  0x00CA3142  7000                    jo       0xca3144                       
                                        ; XREF: 0x00CA3142 (cond_jump)
  0x00CA3144  0001                    add      byte ptr [ecx], al             
  0x00CA3146  0000                    add      byte ptr [eax], al             
  0x00CA3148  000439                  add      byte ptr [ecx + edi], al       
  0x00CA314B  0080010d0013            add      byte ptr [eax + 0x13000d01], al 
  0x00CA3152  6200                    bound    eax, qword ptr [eax]           
  0x00CA3154  2e06                    push     es                             
  0x00CA3156  0000                    add      byte ptr [eax], al             
  0x00CA3158  dc1a                    fcomp    qword ptr [edx]                
  0x00CA315A  0200                    add      al, byte ptr [eax]             
  0x00CA315C  00f4                    add      ah, dh                         
  0x00CA315E  44                      inc      esp                            
  0x00CA315F  0001                    add      byte ptr [ecx], al             
  0x00CA3161  0000                    add      byte ptr [eax], al             
  0x00CA3163  004500                  add      byte ptr [ebp], al             
  0x00CA3166  2000                    and      byte ptr [eax], al             
  0x00CA3168  41                      inc      ecx                            
  0x00CA3169  2920                    sub      dword ptr [eax], esp           
  0x00CA316B  0000                    add      byte ptr [eax], al             
  0x00CA316D  f4                      hlt                                     
  0x00CA316E  44                      inc      esp                            
  0x00CA316F  001f                    add      byte ptr [edi], bl             
  0x00CA3171  0000                    add      byte ptr [eax], al             
  0x00CA3173  004500                  add      byte ptr [ebp], al             
  0x00CA3176  2000                    and      byte ptr [eax], al             
  0x00CA3178  41                      inc      ecx                            
  0x00CA3179  27                      daa                                     
  0x00CA317A  2000                    and      byte ptr [eax], al             
  0x00CA317C  0098210000f4            add      byte ptr [eax - 0xbffffdf], bl 
  0x00CA3182  60                      pushal                                  
  0x00CA3183  000503000085            add      byte ptr [0x85000003], al      
  0x00CA3189  e807001808              call     0x8e23195                      
  0x00CA318E  050013f460              add      eax, 0x60f41300                
  0x00CA3193  0000                    add      byte ptr [eax], al             
  0x00CA3195  06                      push     es                             
  0x00CA3196  0000                    add      byte ptr [eax], al             
  0x00CA3198  00f4                    add      ah, dh                         
  0x00CA319A  57                      push     edi                            
  0x00CA319B  0016                    add      byte ptr [esi], dl             
  0x00CA319D  0000                    add      byte ptr [eax], al             
  0x00CA319F  0080100d001c            add      byte ptr [eax + 0x1c000d10], al 
  0x00CA31A5  0000                    add      byte ptr [eax], al             
  0x00CA31A7  0000                    add      byte ptr [eax], al             
  0x00CA31A9  f4                      hlt                                     
  0x00CA31AA  56                      push     esi                            
  0x00CA31AB  0008                    add      byte ptr [eax], cl             
  0x00CA31AD  0000                    add      byte ptr [eax], al             
  0x00CA31AF  0000                    add      byte ptr [eax], al             
  0x00CA31B1  f4                      hlt                                     
  0x00CA31B2  60                      pushal                                  
  0x00CA31B3  0000                    add      byte ptr [eax], al             
  0x00CA31B5  0400                    add      al, 0                          
  0x00CA31B7  0000                    add      byte ptr [eax], al             
  0x00CA31B9  f4                      hlt                                     
  0x00CA31BA  7000                    jo       0xca31bc                       
                                        ; XREF: 0x00CA31BA (cond_jump)
  0x00CA31BC  0001                    add      byte ptr [ecx], al             
  0x00CA31BE  0000                    add      byte ptr [eax], al             
  0x00CA31C0  0000                    add      byte ptr [eax], al             
  0x00CA31C2  3900                    cmp      dword ptr [eax], eax           
  0x00CA31C4  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA31C7  0000                    add      byte ptr [eax], al             
  0x00CA31C9  f4                      hlt                                     
  0x00CA31CA  56                      push     esi                            
  0x00CA31CB  0008                    add      byte ptr [eax], cl             
  0x00CA31CD  0000                    add      byte ptr [eax], al             
  0x00CA31CF  0000                    add      byte ptr [eax], al             
  0x00CA31D1  f4                      hlt                                     
  0x00CA31D2  60                      pushal                                  
  0x00CA31D3  0000                    add      byte ptr [eax], al             
  0x00CA31D5  05000000f4              add      eax, 0xf4000000                
  0x00CA31DA  7000                    jo       0xca31dc                       
                                        ; XREF: 0x00CA31DA (cond_jump)
  0x00CA31DC  0001                    add      byte ptr [ecx], al             
  0x00CA31DE  0000                    add      byte ptr [eax], al             
  0x00CA31E0  0001                    add      byte ptr [ecx], al             
  0x00CA31E2  3900                    cmp      dword ptr [eax], eax           
  0x00CA31E4  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA31E7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA31EA  0000                    add      byte ptr [eax], al             
  0x00CA31EC  00f4                    add      ah, dh                         
  0x00CA31EE  60                      pushal                                  
  0x00CA31EF  0000                    add      byte ptr [eax], al             
  0x00CA31F1  0000                    add      byte ptr [eax], al             
  0x00CA31F3  009280060005            add      byte ptr [edx + 0x5000680], dl 
  0x00CA31F9  0000                    add      byte ptr [eax], al             
  0x00CA31FB  0000                    add      byte ptr [eax], al             
  0x00CA31FD  d84400a1                fadd     dword ptr [eax + eax - 0x5f]   
  0x00CA3201  d04600                  rol      byte ptr [esi], 1              
  0x00CA3204  e958560000              jmp      0xca8861                       
  0x00CA3209  58                      pop      eax                            
  0x00CA320A  57                      push     edi                            
  0x00CA320B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA320E  0000                    add      byte ptr [eax], al             
  0x00CA3210  20f4                    and      ah, dh                         
  0x00CA3212  0500ffffff              add      eax, 0xffffff00                
  0x00CA3217  00a0610400a0            add      byte ptr [eax - 0x5ffffb9f], ah 
  0x00CA321D  620400                  bound    eax, qword ptr [eax + eax]     
  0x00CA3220  a0640400a0              mov      al, byte ptr [0xa0000464]      
  0x00CA3225  650400                  add      al, 0                          
  0x00CA3228  a0660400b8              mov      al, byte ptr [0xb8000466]      
  0x00CA322D  f30000                  add      byte ptr [eax], al             
  0x00CA3230  00f4                    add      ah, dh                         
  0x00CA3232  44                      inc      esp                            
  0x00CA3233  0016                    add      byte ptr [esi], dl             
  0x00CA3235  0000                    add      byte ptr [eax], al             
  0x00CA3237  004d00                  add      byte ptr [ebp], cl             
  0x00CA323A  2000                    and      byte ptr [eax], al             
  0x00CA323C  4a                      dec      edx                            
  0x00CA323D  100d00080000            adc      byte ptr [0x800], cl           
  0x00CA3243  0000                    add      byte ptr [eax], al             
  0x00CA3245  0030                    add      byte ptr [eax], dh             
  0x00CA3247  0000                    add      byte ptr [eax], al             
  0x00CA3249  f4                      hlt                                     
  0x00CA324A  56                      push     esi                            
  0x00CA324B  0000                    add      byte ptr [eax], al             
  0x00CA324D  0000                    add      byte ptr [eax], al             
  0x00CA324F  0000                    add      byte ptr [eax], al             
  0x00CA3251  f4                      hlt                                     
  0x00CA3252  57                      push     edi                            
  0x00CA3253  00ff                    add      bh, bh                         
  0x00CA3256  ff00                    inc      dword ptr [eax]                
  0x00CA3258  0c00                    or       al, 0                          
  0x00CA325A  0000                    add      byte ptr [eax], al             
  0x00CA325C  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA325F  00b800000000            add      byte ptr [eax], bh             
  0x00CA3266  56                      push     esi                            
  0x00CA3267  0032                    add      byte ptr [edx], dh             
  0x00CA3269  06                      push     es                             
  0x00CA326A  0000                    add      byte ptr [eax], al             
  0x00CA326C  0300                    add      eax, dword ptr [eax]           
  0x00CA326E  2000                    and      byte ptr [eax], al             
  0x00CA3270  06                      push     es                             
  0x00CA3271  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA3272  050080100d              add      eax, 0xd108000                 
  0x00CA3277  005001                  add      byte ptr [eax + 1], dl         
  0x00CA327A  0000                    add      byte ptr [eax], al             
  0x00CA327C  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA327F  002f                    add      byte ptr [edi], ch             
  0x00CA3281  0100                    add      dword ptr [eax], eax           
  0x00CA3283  0003                    add      byte ptr [ebx], al             
  0x00CA3285  0c05                    or       al, 5                          
  0x00CA3287  0080100d0053            add      byte ptr [eax + 0x53000d10], al 
  0x00CA328D  0100                    add      dword ptr [eax], eax           
  0x00CA328F  0080100d0030            add      byte ptr [eax + 0x30000d10], al 
  0x00CA3295  0100                    add      dword ptr [eax], eax           
  0x00CA3297  0000                    add      byte ptr [eax], al             
  0x00CA329A  56                      push     esi                            
  0x00CA329B  0033                    add      byte ptr [ebx], dh             
  0x00CA329D  06                      push     es                             
  0x00CA329E  0000                    add      byte ptr [eax], al             
  0x00CA32A0  854001                  test     dword ptr [eax + 1], eax       
  0x00CA32A3  0017                    add      byte ptr [edi], dl             
  0x00CA32A5  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA32A6  050000f066              add      eax, 0x66f00000                
  0x00CA32AB  002f                    add      byte ptr [edi], ch             
  0x00CA32AD  06                      push     es                             
  0x00CA32AE  0000                    add      byte ptr [eax], al             
  0x00CA32B0  0003                    add      byte ptr [ebx], al             
  0x00CA32B2  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA32B5  ee                      out      dx, al                         
  0x00CA32B6  60                      pushal                                  
  0x00CA32B7  0000                    add      byte ptr [eax], al             
  0x00CA32B9  043e                    add      al, 0x3e                       
  0x00CA32BB  0000                    add      byte ptr [eax], al             
  0x00CA32BD  ee                      out      dx, al                         
  0x00CA32BE  61                      popal                                   
  0x00CA32BF  0000                    add      byte ptr [eax], al             
  0x00CA32C1  f0660030                lock add byte ptr [eax], dh             
  0x00CA32C5  06                      push     es                             
  0x00CA32C6  0000                    add      byte ptr [eax], al             
  0x00CA32C8  0003                    add      byte ptr [ebx], al             
  0x00CA32CA  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA32CD  ee                      out      dx, al                         
  0x00CA32CE  7000                    jo       0xca32d0                       
                                        ; XREF: 0x00CA32CE (cond_jump)
  0x00CA32D0  00043e                  add      byte ptr [esi + edi], al       
  0x00CA32D3  0000                    add      byte ptr [eax], al             
  0x00CA32D5  ee                      out      dx, al                         
  0x00CA32D6  7100                    jno      0xca32d8                       
                                        ; XREF: 0x00CA32D6 (cond_jump)
  0x00CA32D8  00f4                    add      ah, dh                         
  0x00CA32DA  46                      inc      esi                            
  0x00CA32DB  007a82                  add      byte ptr [edx - 0x7e], bh      
  0x00CA32DE  5a                      pop      edx                            
  0x00CA32DF  0000                    add      byte ptr [eax], al             
  0x00CA32E2  6200                    bound    eax, qword ptr [eax]           
  0x00CA32E4  3806                    cmp      byte ptr [esi], al             
  0x00CA32E6  0000                    add      byte ptr [eax], al             
  0x00CA32E8  10d2                    adc      dl, dl                         
  0x00CA32EA  06                      push     es                             
  0x00CA32EB  000500000000            add      byte ptr [0], al               
  0x00CA32F1  e044                    loopne   0xca3337                       
  0x00CA32F3  00d0                    add      al, dl                         
  0x00CA32F5  c9                      leave                                   
  0x00CA32F6  44                      inc      esp                            
  0x00CA32F7  00d3                    add      bl, dl                         
  0x00CA32F9  0020                    add      byte ptr [eax], ah             
  0x00CA32FB  0000                    add      byte ptr [eax], al             
  0x00CA32FD  48                      dec      eax                            
  0x00CA32FE  56                      push     esi                            
  0x00CA32FF  0000                    add      byte ptr [eax], al             
  0x00CA3301  f066002f                lock add byte ptr [edi], ch             
  0x00CA3305  06                      push     es                             
  0x00CA3306  0000                    add      byte ptr [eax], al             
  0x00CA3308  0000                    add      byte ptr [eax], al             
  0x00CA330A  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA330D  ee                      out      dx, al                         
  0x00CA330E  60                      pushal                                  
  0x00CA330F  0000                    add      byte ptr [eax], al             
  0x00CA3311  023e                    add      bh, byte ptr [esi]             
  0x00CA3313  0000                    add      byte ptr [eax], al             
  0x00CA3315  ee                      out      dx, al                         
  0x00CA3316  61                      popal                                   
  0x00CA3317  0000                    add      byte ptr [eax], al             
  0x00CA3319  013e                    add      dword ptr [esi], edi           
  0x00CA331B  0000                    add      byte ptr [eax], al             
  0x00CA331D  ee                      out      dx, al                         
  0x00CA331E  6200                    bound    eax, qword ptr [eax]           
  0x00CA3320  00f0                    add      al, dh                         
  0x00CA3322  66003406                add      byte ptr [esi + eax], dh       
  0x00CA3326  0000                    add      byte ptr [eax], al             
  0x00CA3328  0000                    add      byte ptr [eax], al             
  0x00CA332A  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA332D  ee                      out      dx, al                         
  0x00CA332E  640000                  add      byte ptr fs:[eax], al          
  0x00CA3331  023e                    add      bh, byte ptr [esi]             
  0x00CA3333  0000                    add      byte ptr [eax], al             
  0x00CA3335  ee                      out      dx, al                         
  0x00CA3336  650000                  add      byte ptr gs:[eax], al          
  0x00CA3339  f0660030                lock add byte ptr [eax], dh             
  0x00CA333D  06                      push     es                             
  0x00CA333E  0000                    add      byte ptr [eax], al             
  0x00CA3340  0000                    add      byte ptr [eax], al             
  0x00CA3342  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA3345  ee                      out      dx, al                         
  0x00CA3346  7000                    jo       0xca3348                       
                                        ; XREF: 0x00CA3346 (cond_jump)
  0x00CA3348  0002                    add      byte ptr [edx], al             
  0x00CA334A  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA334D  ee                      out      dx, al                         
  0x00CA334E  7100                    jno      0xca3350                       
                                        ; XREF: 0x00CA334E (cond_jump)
  0x00CA3350  0001                    add      byte ptr [ecx], al             
  0x00CA3352  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA3355  ee                      out      dx, al                         
  0x00CA3356  7200                    jb       0xca3358                       
                                        ; XREF: 0x00CA3356 (cond_jump)
  0x00CA3358  00f0                    add      al, dh                         
  0x00CA335A  66003506000000          add      byte ptr [6], dh               
  0x00CA3361  003e                    add      byte ptr [esi], bh             
  0x00CA3363  0000                    add      byte ptr [eax], al             
  0x00CA3365  ee                      out      dx, al                         
  0x00CA3366  7400                    je       0xca3368                       
                                        ; XREF: 0x00CA3366 (cond_jump)
  0x00CA3368  00f4                    add      ah, dh                         
  0x00CA336A  7600                    jbe      0xca336c                       
                                        ; XREF: 0x00CA336A (cond_jump)
  0x00CA336C  0200                    add      al, byte ptr [eax]             
  0x00CA336E  0000                    add      byte ptr [eax], al             
  0x00CA3370  00ee                    add      dh, ch                         
  0x00CA3372  7500                    jne      0xca3374                       
                                        ; XREF: 0x00CA3372 (cond_jump)
  0x00CA3374  00f4                    add      ah, dh                         
  0x00CA3376  45                      inc      ebp                            
  0x00CA3377  007a82                  add      byte ptr [edx - 0x7e], bh      
  0x00CA337A  5a                      pop      edx                            
  0x00CA337B  0000                    add      byte ptr [eax], al             
  0x00CA337D  f0660038                lock add byte ptr [eax], bh             
  0x00CA3381  06                      push     es                             
  0x00CA3382  0000                    add      byte ptr [eax], al             
  0x00CA3384  00d6                    add      dh, dl                         
  0x00CA3386  06                      push     es                             
  0x00CA3387  006704                  add      byte ptr [edi + 4], ah         
  0x00CA338A  0000                    add      byte ptr [eax], al             
  0x00CA338C  00ca                    add      dl, cl                         
  0x00CA338E  44                      inc      esp                            
  0x00CA338F  0000                    add      byte ptr [eax], al             
  0x00CA3391  c85600a3                enter    0x56, -0x5d                    
  0x00CA3395  c9                      leave                                   
  0x00CA3396  57                      push     edi                            
  0x00CA3397  00ab4c560000            add      byte ptr [ebx + 0x564c], ch    
  0x00CA339D  4d                      dec      ebp                            
  0x00CA339E  57                      push     edi                            
  0x00CA339F  0000                    add      byte ptr [eax], al             
  0x00CA33A1  f4                      hlt                                     
  0x00CA33A2  61                      popal                                   
  0x00CA33A3  0039                    add      byte ptr [ecx], bh             
  0x00CA33A5  06                      push     es                             
  0x00CA33A6  0000                    add      byte ptr [eax], al             
  0x00CA33A8  00f0                    add      al, dh                         
  0x00CA33AA  65008706000000          add      byte ptr gs:[edi + 6], al      
  0x00CA33B1  f4                      hlt                                     
  0x00CA33B2  6200                    bound    eax, qword ptr [eax]           
  0x00CA33B4  6d                      insd     dword ptr es:[edi], dx         
  0x00CA33B5  06                      push     es                             
  0x00CA33B6  0000                    add      byte ptr [eax], al             
  0x00CA33B8  00f0                    add      al, dh                         
  0x00CA33BA  66003406                add      byte ptr [esi + eax], dh       
  0x00CA33BE  0000                    add      byte ptr [eax], al             
  0x00CA33C0  0000                    add      byte ptr [eax], al             
  0x00CA33C2  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA33C5  ee                      out      dx, al                         
  0x00CA33C6  60                      pushal                                  
  0x00CA33C7  0000                    add      byte ptr [eax], al             
  0x00CA33C9  1422                    adc      al, 0x22                       
  0x00CA33CB  0000                    add      byte ptr [eax], al             
  0x00CA33CD  f066003506000000        lock add byte ptr [6], dh               
  0x00CA33D5  ee                      out      dx, al                         
  0x00CA33D6  7000                    jo       0xca33d8                       
                                        ; XREF: 0x00CA33D6 (cond_jump)
  0x00CA33D8  001c23                  add      byte ptr [ebx], bl             
  0x00CA33DB  0000                    add      byte ptr [eax], al             
  0x00CA33DE  50                      push     eax                            
  0x00CA33DF  0038                    add      byte ptr [eax], bh             
  0x00CA33E1  06                      push     es                             
  0x00CA33E2  0000                    add      byte ptr [eax], al             
  0x00CA33E4  0a00                    or       al, byte ptr [eax]             
  0x00CA33E6  0000                    add      byte ptr [eax], al             
  0x00CA33E8  001e                    add      byte ptr [esi], bl             
  0x00CA33EA  2100                    and      dword ptr [eax], eax           
  0x00CA33EC  00f4                    add      ah, dh                         
  0x00CA33EE  7200                    jb       0xca33f0                       
                                        ; XREF: 0x00CA33EE (cond_jump)
  0x00CA33F0  0400                    add      al, 0                          
  0x00CA33F2  0000                    add      byte ptr [eax], al             
  0x00CA33F4  80f00b                  xor      al, 0xb                        
  0x00CA33F7  008b05000000            add      byte ptr [ebx + 5], cl         
  0x00CA33FD  f4                      hlt                                     
  0x00CA33FE  61                      popal                                   
  0x00CA33FF  004906                  add      byte ptr [ecx + 6], cl         
  0x00CA3402  0000                    add      byte ptr [eax], al             
  0x00CA3404  00f0                    add      al, dh                         
  0x00CA3406  65008706000000          add      byte ptr gs:[edi + 6], al      
  0x00CA340D  f4                      hlt                                     
  0x00CA340E  6200                    bound    eax, qword ptr [eax]           
  0x00CA3410  7506                    jne      0xca3418                       
  0x00CA3412  0000                    add      byte ptr [eax], al             
  0x00CA3414  00f0                    add      al, dh                         
  0x00CA3416  66003406                add      byte ptr [esi + eax], dh       
  0x00CA341A  0000                    add      byte ptr [eax], al             
  0x00CA341C  0002                    add      byte ptr [edx], al             
  0x00CA341E  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA3421  ee                      out      dx, al                         
  0x00CA3422  60                      pushal                                  
  0x00CA3423  0000                    add      byte ptr [eax], al             
  0x00CA3425  1422                    adc      al, 0x22                       
  0x00CA3427  0000                    add      byte ptr [eax], al             
  0x00CA3429  f066003506000000        lock add byte ptr [6], dh               
  0x00CA3431  ee                      out      dx, al                         
  0x00CA3432  7000                    jo       0xca3434                       
                                        ; XREF: 0x00CA3432 (cond_jump)
  0x00CA3434  001c23                  add      byte ptr [ebx], bl             
  0x00CA3437  0000                    add      byte ptr [eax], al             
  0x00CA343A  50                      push     eax                            
  0x00CA343B  0038                    add      byte ptr [eax], bh             
  0x00CA343D  06                      push     es                             
  0x00CA343E  0000                    add      byte ptr [eax], al             
  0x00CA3440  0a00                    or       al, byte ptr [eax]             
  0x00CA3442  0000                    add      byte ptr [eax], al             
  0x00CA3444  001e                    add      byte ptr [esi], bl             
  0x00CA3446  2100                    and      dword ptr [eax], eax           
  0x00CA3448  00f4                    add      ah, dh                         
  0x00CA344A  7200                    jb       0xca344c                       
                                        ; XREF: 0x00CA344A (cond_jump)
  0x00CA344C  0400                    add      al, 0                          
  0x00CA344E  0000                    add      byte ptr [eax], al             
  0x00CA3450  80f00b                  xor      al, 0xb                        
  0x00CA3453  008b05000000            add      byte ptr [ebx + 5], cl         
  0x00CA3459  f4                      hlt                                     
  0x00CA345A  61                      popal                                   
  0x00CA345B  005906                  add      byte ptr [ecx + 6], bl         
  0x00CA345E  0000                    add      byte ptr [eax], al             
  0x00CA3460  00f0                    add      al, dh                         
  0x00CA3462  65008806000000          add      byte ptr gs:[eax + 6], cl      
  0x00CA3469  f4                      hlt                                     
  0x00CA346A  6200                    bound    eax, qword ptr [eax]           
  0x00CA346C  7d06                    jge      0xca3474                       
  0x00CA346E  0000                    add      byte ptr [eax], al             
  0x00CA3470  00f0                    add      al, dh                         
  0x00CA3472  66002f                  add      byte ptr [edi], ch             
  0x00CA3475  06                      push     es                             
  0x00CA3476  0000                    add      byte ptr [eax], al             
  0x00CA3478  0003                    add      byte ptr [ebx], al             
  0x00CA347A  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA347D  ee                      out      dx, al                         
  0x00CA347E  60                      pushal                                  
  0x00CA347F  0000                    add      byte ptr [eax], al             
  0x00CA3481  1422                    adc      al, 0x22                       
  0x00CA3483  0000                    add      byte ptr [eax], al             
  0x00CA3485  f0660030                lock add byte ptr [eax], dh             
  0x00CA3489  06                      push     es                             
  0x00CA348A  0000                    add      byte ptr [eax], al             
  0x00CA348C  00ee                    add      dh, ch                         
  0x00CA348E  7000                    jo       0xca3490                       
                                        ; XREF: 0x00CA348E (cond_jump)
  0x00CA3490  001c23                  add      byte ptr [ebx], bl             
  0x00CA3493  0000                    add      byte ptr [eax], al             
  0x00CA3496  50                      push     eax                            
  0x00CA3497  0038                    add      byte ptr [eax], bh             
  0x00CA3499  06                      push     es                             
  0x00CA349A  0000                    add      byte ptr [eax], al             
  0x00CA349C  0a00                    or       al, byte ptr [eax]             
  0x00CA349E  0000                    add      byte ptr [eax], al             
  0x00CA34A0  001e                    add      byte ptr [esi], bl             
  0x00CA34A2  2100                    and      dword ptr [eax], eax           
  0x00CA34A4  00f4                    add      ah, dh                         
  0x00CA34A6  7200                    jb       0xca34a8                       
                                        ; XREF: 0x00CA34A6 (cond_jump)
  0x00CA34A8  0500000080              add      eax, 0x80000000                
  0x00CA34AD  f00b00                  lock or  eax, dword ptr [eax]           
  0x00CA34B0  8b05000000f0            mov      eax, dword ptr [0xf0000000]    
  0x00CA34B6  66003406                add      byte ptr [esi + eax], dh       
  0x00CA34BA  0000                    add      byte ptr [eax], al             
  0x00CA34BC  0000                    add      byte ptr [eax], al             
  0x00CA34BE  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA34C1  ee                      out      dx, al                         
  0x00CA34C2  60                      pushal                                  
  0x00CA34C3  0000                    add      byte ptr [eax], al             
  0x00CA34C5  023e                    add      bh, byte ptr [esi]             
  0x00CA34C7  0000                    add      byte ptr [eax], al             
  0x00CA34C9  ee                      out      dx, al                         
  0x00CA34CA  61                      popal                                   
  0x00CA34CB  0000                    add      byte ptr [eax], al             
  0x00CA34CD  f066002f                lock add byte ptr [edi], ch             
  0x00CA34D1  06                      push     es                             
  0x00CA34D2  0000                    add      byte ptr [eax], al             
  0x00CA34D4  0003                    add      byte ptr [ebx], al             
  0x00CA34D6  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA34D9  ee                      out      dx, al                         
  0x00CA34DA  6200                    bound    eax, qword ptr [eax]           
  0x00CA34DC  00f0                    add      al, dh                         
  0x00CA34DE  66003506000000          add      byte ptr [6], dh               
  0x00CA34E5  003e                    add      byte ptr [esi], bh             
  0x00CA34E7  0000                    add      byte ptr [eax], al             
  0x00CA34E9  ee                      out      dx, al                         
  0x00CA34EA  7000                    jo       0xca34ec                       
                                        ; XREF: 0x00CA34EA (cond_jump)
  0x00CA34EC  0002                    add      byte ptr [edx], al             
  0x00CA34EE  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA34F1  ee                      out      dx, al                         
  0x00CA34F2  7100                    jno      0xca34f4                       
                                        ; XREF: 0x00CA34F2 (cond_jump)
  0x00CA34F4  00f0                    add      al, dh                         
  0x00CA34F6  660030                  add      byte ptr [eax], dh             
  0x00CA34F9  06                      push     es                             
  0x00CA34FA  0000                    add      byte ptr [eax], al             
  0x00CA34FC  0003                    add      byte ptr [ebx], al             
  0x00CA34FE  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA3501  ee                      out      dx, al                         
  0x00CA3502  7200                    jb       0xca3504                       
                                        ; XREF: 0x00CA3502 (cond_jump)
  0x00CA3504  00f4                    add      ah, dh                         
  0x00CA3506  45                      inc      ebp                            
  0x00CA3507  007a82                  add      byte ptr [edx - 0x7e], bh      
  0x00CA350A  5a                      pop      edx                            
  0x00CA350B  0000                    add      byte ptr [eax], al             
  0x00CA350D  f0640038                lock add byte ptr fs:[eax], bh          
  0x00CA3511  06                      push     es                             
  0x00CA3512  0000                    add      byte ptr [eax], al             
  0x00CA3514  00d4                    add      ah, dl                         
  0x00CA3516  06                      push     es                             
  0x00CA3517  00cb                    add      bl, cl                         
  0x00CA3519  0400                    add      al, 0                          
  0x00CA351B  0000                    add      byte ptr [eax], al             
  0x00CA351D  ca4400                  retf     0x44                           
  0x00CA3520  00e0                    add      al, ah                         
  0x00CA3522  56                      push     esi                            
  0x00CA3523  00a3e15700af            add      byte ptr [ebx - 0x50ffa81f], ah 
  0x00CA3529  48                      dec      eax                            
  0x00CA352A  56                      push     esi                            
  0x00CA352B  0000                    add      byte ptr [eax], al             
  0x00CA352D  49                      dec      ecx                            
  0x00CA352E  57                      push     edi                            
  0x00CA352F  0080100d00b4            add      byte ptr [eax - 0x4bfff2f0], al 
  0x00CA3535  0000                    add      byte ptr [eax], al             
  0x00CA3537  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA353A  0000                    add      byte ptr [eax], al             
  0x00CA353C  005820                  add      byte ptr [eax + 0x20], bl      
  0x00CA353F  0000                    add      byte ptr [eax], al             
  0x00CA3541  d8440000                fadd     dword ptr [eax + eax]          
  0x00CA3545  7044                    jo       0xca358b                       
  0x00CA3547  002f                    add      byte ptr [edi], ch             
  0x00CA3549  06                      push     es                             
  0x00CA354A  0000                    add      byte ptr [eax], al             
  0x00CA354C  00d8                    add      al, bl                         
  0x00CA354E  44                      inc      esp                            
  0x00CA354F  0000                    add      byte ptr [eax], al             
  0x00CA3551  7044                    jo       0xca3597                       
  0x00CA3553  0030                    add      byte ptr [eax], dh             
  0x00CA3555  06                      push     es                             
  0x00CA3556  0000                    add      byte ptr [eax], al             
  0x00CA3558  00d8                    add      al, bl                         
  0x00CA355A  44                      inc      esp                            
  0x00CA355B  0000                    add      byte ptr [eax], al             
  0x00CA355D  7044                    jo       0xca35a3                       
  0x00CA355F  0031                    add      byte ptr [ecx], dh             
  0x00CA3561  06                      push     es                             
  0x00CA3562  0000                    add      byte ptr [eax], al             
  0x00CA3564  00d8                    add      al, bl                         
  0x00CA3566  57                      push     edi                            
  0x00CA3567  0090180c0027            add      byte ptr [eax + 0x27000c18], dl 
  0x00CA356D  1000                    adc      byte ptr [eax], al             
  0x00CA356F  0000                    add      byte ptr [eax], al             
  0x00CA3571  7050                    jo       0xca35c3                       
  0x00CA3573  0032                    add      byte ptr [edx], dh             
  0x00CA3575  06                      push     es                             
  0x00CA3576  0000                    add      byte ptr [eax], al             
  0x00CA3578  90                      nop                                     
  0x00CA3579  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA357C  1910                    sbb      dword ptr [eax], edx           
  0x00CA357E  0000                    add      byte ptr [eax], al             
  0x00CA3580  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA3583  0033                    add      byte ptr [ebx], dh             
  0x00CA3585  06                      push     es                             
  0x00CA3586  0000                    add      byte ptr [eax], al             
  0x00CA3588  00d8                    add      al, bl                         
  0x00CA358A  44                      inc      esp                            
                                        ; XREF: 0x00CA3545 (cond_jump)
  0x00CA358B  0000                    add      byte ptr [eax], al             
  0x00CA358D  7044                    jo       0xca35d3                       
  0x00CA358F  003406                  add      byte ptr [esi + eax], dh       
  0x00CA3592  0000                    add      byte ptr [eax], al             
  0x00CA3594  00d8                    add      al, bl                         
  0x00CA3596  44                      inc      esp                            
                                        ; XREF: 0x00CA3551 (cond_jump)
  0x00CA3597  0000                    add      byte ptr [eax], al             
  0x00CA3599  7044                    jo       0xca35df                       
  0x00CA359B  003506000000            add      byte ptr [6], dh               
  0x00CA35A1  d8440000                fadd     dword ptr [eax + eax]          
  0x00CA35A5  7044                    jo       0xca35eb                       
  0x00CA35A7  0036                    add      byte ptr [esi], dh             
  0x00CA35A9  06                      push     es                             
  0x00CA35AA  0000                    add      byte ptr [eax], al             
  0x00CA35AC  00d8                    add      al, bl                         
  0x00CA35AE  57                      push     edi                            
  0x00CA35AF  0090180c0024            add      byte ptr [eax + 0x24000c18], dl 
  0x00CA35B5  2000                    and      byte ptr [eax], al             
  0x00CA35B7  0000                    add      byte ptr [eax], al             
  0x00CA35B9  7050                    jo       0xca360b                       
  0x00CA35BB  0037                    add      byte ptr [edi], dh             
  0x00CA35BD  06                      push     es                             
  0x00CA35BE  0000                    add      byte ptr [eax], al             
  0x00CA35C0  00d8                    add      al, bl                         
  0x00CA35C2  44                      inc      esp                            
                                        ; XREF: 0x00CA3571 (cond_jump)
  0x00CA35C3  0000                    add      byte ptr [eax], al             
  0x00CA35C5  7044                    jo       0xca360b                       
  0x00CA35C7  0038                    add      byte ptr [eax], bh             
  0x00CA35C9  06                      push     es                             
  0x00CA35CA  0000                    add      byte ptr [eax], al             
  0x00CA35CC  0c00                    or       al, 0                          
  0x00CA35CE  0000                    add      byte ptr [eax], al             
  0x00CA35D0  8d95c0000000            lea      edx, [ebp + 0xc0]              
  0x00CA35D6  0000                    add      byte ptr [eax], al             
  0x00CA35D8  e5d4                    in       eax, 0xd4                      
  0x00CA35DA  7e00                    jle      0xca35dc                       
                                        ; XREF: 0x00CA35DA (cond_jump)
  0x00CA35DC  0000                    add      byte ptr [eax], al             
  0x00CA35DE  c00000                  rol      byte ptr [eax], 0              
  0x00CA35E1  0000                    add      byte ptr [eax], al             
  0x00CA35E3  004ae2                  add      byte ptr [edx - 0x1e], cl      
  0x00CA35E6  4f                      dec      edi                            
  0x00CA35E7  00cc                    add      ah, cl                         
  0x00CA35E9  673f                    aas                                     
                                        ; XREF: 0x00CA35A5 (cond_jump)
  0x00CA35EB  00cc                    add      ah, cl                         
  0x00CA35ED  673f                    aas                                     
  0x00CA35EF  004ae2                  add      byte ptr [edx - 0x1e], cl      
  0x00CA35F2  4f                      dec      edi                            
  0x00CA35F3  00ff                    add      bh, bh                         
  0x00CA35F6  7f00                    jg       0xca35f8                       
                                        ; XREF: 0x00CA35F6 (cond_jump)
  0x00CA35F8  e85b850038              call     0x38cabb58                     
  0x00CA35FD  667500                  jne      0xca3600                       
                                        ; XREF: 0x00CA35FD (cond_jump)
  0x00CA3600  386675                  cmp      byte ptr [esi + 0x75], ah      
  0x00CA3603  00e8                    add      al, ch                         
  0x00CA3605  5b                      pop      ebx                            
  0x00CA3606  8500                    test     dword ptr [eax], eax           
  0x00CA360A  7f00                    jg       0xca360c                       
                                        ; XREF: 0x00CA360A (cond_jump)
  0x00CA360C  92                      xchg     edx, eax                       
  0x00CA360D  1f                      pop      ds                             
  0x00CA360E  ea004b40e2004b          ljmp     0x4b00:0xe2404b00              
  0x00CA3615  40                      inc      eax                            
  0x00CA3616  e200                    loop     0xca3618                       
                                        ; XREF: 0x00CA3616 (cond_jump)
  0x00CA3618  92                      xchg     edx, eax                       
  0x00CA3619  1f                      pop      ds                             
  0x00CA361A  ea00ffff7f001b          ljmp     0x1b00:0x7fffff00              
  0x00CA3621  2b810085ac7d            sub      eax, dword ptr [ecx + 0x7dac8500] 
  0x00CA3627  0094d57e006c2a          add      byte ptr [ebp + edx*8 + 0x2a6c007e], dl 
  0x00CA362E  810094d57e00            add      dword ptr [eax], 0x7ed594      
  0x00CA3634  4a                      dec      edx                            
  0x00CA3635  e24f                    loop     0xca3686                       
  0x00CA3637  00cc                    add      ah, cl                         
  0x00CA3639  673f                    aas                                     
  0x00CA363B  0026                    add      byte ptr [esi], ah             
  0x00CA363D  94                      xchg     esp, eax                       
  0x00CA363E  57                      push     edi                            
  0x00CA363F  007fff                  add      byte ptr [edi - 1], bh         
  0x00CA3642  55                      push     ebp                            
  0x00CA3643  0026                    add      byte ptr [esi], ah             
  0x00CA3645  94                      xchg     esp, eax                       
  0x00CA3646  57                      push     edi                            
  0x00CA3647  004ae2                  add      byte ptr [edx - 0x1e], cl      
  0x00CA364A  4f                      dec      edi                            
  0x00CA364B  00cc                    add      ah, cl                         
  0x00CA364D  673f                    aas                                     
  0x00CA364F  0026                    add      byte ptr [esi], ah             
  0x00CA3651  94                      xchg     esp, eax                       
  0x00CA3652  57                      push     edi                            
  0x00CA3653  007fff                  add      byte ptr [edi - 1], bh         
  0x00CA3656  55                      push     ebp                            
  0x00CA3657  0026                    add      byte ptr [esi], ah             
  0x00CA3659  94                      xchg     esp, eax                       
  0x00CA365A  57                      push     edi                            
  0x00CA365B  0022                    add      byte ptr [edx], ah             
  0x00CA365D  3e82006d                add      byte ptr ds:[eax], 0x6d        
  0x00CA3661  877b00                  xchg     dword ptr [ebx], edi           
  0x00CA3664  6d                      insd     dword ptr es:[edi], dx         
  0x00CA3665  877b00                  xchg     dword ptr [ebx], edi           
  0x00CA3668  223e                    and      bh, byte ptr [esi]             
  0x00CA366A  8200ff                  add      byte ptr [eax], 0xff           
  0x00CA366E  7f00                    jg       0xca3670                       
                                        ; XREF: 0x00CA366E (cond_jump)
  0x00CA3670  5e                      pop      esi                            
  0x00CA3671  3bb2004ff727            cmp      esi, dword ptr [edx + 0x27f74f00] 
  0x00CA3677  004ff7                  add      byte ptr [edi - 9], cl         
  0x00CA367A  27                      daa                                     
  0x00CA367B  005e3b                  add      byte ptr [esi + 0x3b], bl      
  0x00CA367E  b200                    mov      dl, 0                          
  0x00CA3682  7f00                    jg       0xca3684                       
                                        ; XREF: 0x00CA3682 (cond_jump)
  0x00CA3684  7489                    je       0xca360f                       
                                        ; XREF: 0x00CA3635 (cond_jump)
  0x00CA3686  c00000                  rol      byte ptr [eax], 0              
  0x00CA3689  0000                    add      byte ptr [eax], al             
  0x00CA368B  0019                    add      byte ptr [ecx], bl             
  0x00CA368D  ed                      in       eax, dx                        
  0x00CA368E  7e00                    jle      0xca3690                       
                                        ; XREF: 0x00CA368E (cond_jump)
  0x00CA3690  0000                    add      byte ptr [eax], al             
  0x00CA3692  c00000                  rol      byte ptr [eax], 0              
  0x00CA3695  0000                    add      byte ptr [eax], al             
  0x00CA3697  00f8                    add      al, bh                         
  0x00CA3699  2a4600                  sub      al, byte ptr [esi]             
  0x00CA369C  208637002086            and      byte ptr [esi - 0x79dfffc9], al 
  0x00CA36A2  37                      aaa                                     
  0x00CA36A3  00f8                    add      al, bh                         
  0x00CA36A5  2a4600                  sub      al, byte ptr [esi]             
  0x00CA36AA  7f00                    jg       0xca36ac                       
                                        ; XREF: 0x00CA36AA (cond_jump)
  0x00CA36AC  9e                      sahf                                    
  0x00CA36AD  ef                      out      dx, eax                        
  0x00CA36AE  8400                    test     byte ptr [eax], al             
  0x00CA36B0  353a760035              xor      eax, 0x3500763a                
  0x00CA36B5  3a7600                  cmp      dh, byte ptr [esi]             
  0x00CA36B8  9e                      sahf                                    
  0x00CA36B9  ef                      out      dx, eax                        
  0x00CA36BA  8400                    test     byte ptr [eax], al             
  0x00CA36BE  7f00                    jg       0xca36c0                       
                                        ; XREF: 0x00CA36BE (cond_jump)
  0x00CA36C0  fe48e6                  dec      byte ptr [eax - 0x1a]          
  0x00CA36C3  008ab7e4008a            add      byte ptr [edx - 0x75ff1b49], cl 
  0x00CA36C9  b7e4                    mov      bh, 0xe4                       
  0x00CA36CB  00fe                    add      dh, bh                         
  0x00CA36CD  48                      dec      eax                            
  0x00CA36CE  e600                    out      0, al                          
  0x00CA36D2  7f00                    jg       0xca36d4                       
                                        ; XREF: 0x00CA36D2 (cond_jump)
  0x00CA36D4  e712                    out      0x12, eax                      
  0x00CA36D6  81007fdc7d00            add      dword ptr [eax], 0x7ddc7f      
  0x00CA36DC  ac                      lodsb    al, byte ptr [esi]             
  0x00CA36DD  ed                      in       eax, dx                        
  0x00CA36DE  7e00                    jle      0xca36e0                       
                                        ; XREF: 0x00CA36DE (cond_jump)
  0x00CA36E0  54                      push     esp                            
  0x00CA36E1  128100aced7e            adc      al, byte ptr [ecx + 0x7eedac00] 
  0x00CA36E7  00f8                    add      al, bh                         
  0x00CA36E9  2a4600                  sub      al, byte ptr [esi]             
  0x00CA36EC  20863700f31d            and      byte ptr [esi + 0x1df30037], al 
  0x00CA36F2  51                      push     ecx                            
  0x00CA36F3  0090f54e00f3            add      byte ptr [eax - 0xcffb10b], dl 
  0x00CA36F9  1d5100f82a              sbb      eax, 0x2af80051                
  0x00CA36FE  46                      inc      esi                            
  0x00CA36FF  0020                    add      byte ptr [eax], ah             
  0x00CA3701  8637                    xchg     byte ptr [edi], dh             
  0x00CA3703  00f3                    add      bl, dh                         
  0x00CA3705  1d510090f5              sbb      eax, 0xf5900051                
  0x00CA370A  4e                      dec      esi                            
  0x00CA370B  00f3                    add      bl, dh                         
  0x00CA370D  1d51001a10              sbb      eax, 0x101a0051                
  0x00CA3712  8200ed                  add      byte ptr [eax], 0xed           
  0x00CA3715  e27b                    loop     0xca3792                       
  0x00CA3717  00ed                    add      ch, ch                         
  0x00CA3719  e27b                    loop     0xca3796                       
  0x00CA371B  001a                    add      byte ptr [edx], bl             
  0x00CA371D  108200ffff7f            adc      byte ptr [edx + 0x7fffff00], al 
  0x00CA3723  00fc                    add      ah, bh                         
  0x00CA3725  2eaf                    scasd    eax, dword ptr es:[edi]        
  0x00CA3727  0000                    add      byte ptr [eax], al             
  0x00CA3729  782c                    js       0xca3757                       
  0x00CA372B  0000                    add      byte ptr [eax], al             
  0x00CA372D  782c                    js       0xca375b                       
  0x00CA372F  00fc                    add      ah, bh                         
  0x00CA3731  2eaf                    scasd    eax, dword ptr es:[edi]        
  0x00CA3733  00ff                    add      bh, bh                         
  0x00CA3736  7f00                    jg       0xca3738                       
                                        ; XREF: 0x00CA3736 (cond_jump)
  0x00CA3738  13f4                    adc      esi, esp                       
  0x00CA373A  61                      popal                                   
  0x00CA373B  0039                    add      byte ptr [ecx], bh             
  0x00CA373D  06                      push     es                             
  0x00CA373E  0000                    add      byte ptr [eax], al             
  0x00CA3740  90                      nop                                     
  0x00CA3741  4e                      dec      esi                            
  0x00CA3742  06                      push     es                             
  0x00CA3743  0002                    add      byte ptr [edx], al             
  0x00CA3745  0000                    add      byte ptr [eax], al             
  0x00CA3747  0000                    add      byte ptr [eax], al             
  0x00CA3749  59                      pop      ecx                            
  0x00CA374A  56                      push     esi                            
  0x00CA374B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA374E  0000                    add      byte ptr [eax], al             
  0x00CA3750  00f4                    add      ah, dh                         
  0x00CA3752  60                      pushal                                  
  0x00CA3753  00f4                    add      ah, dh                         
  0x00CA3755  0400                    add      al, 0                          
                                        ; XREF: 0x00CA3729 (cond_jump)
  0x00CA3757  0000                    add      byte ptr [eax], al             
  0x00CA3759  f4                      hlt                                     
  0x00CA375A  61                      popal                                   
                                        ; XREF: 0x00CA372D (cond_jump)
  0x00CA375B  0000                    add      byte ptr [eax], al             
  0x00CA375D  0000                    add      byte ptr [eax], al             
  0x00CA375F  00905a060003            add      byte ptr [eax + 0x300065a], dl 
  0x00CA3765  0000                    add      byte ptr [eax], al             
  0x00CA3767  0084d807000059          add      byte ptr [eax + ebx*8 + 0x59000007], al 
  0x00CA376E  4c                      dec      esp                            
  0x00CA376F  0000                    add      byte ptr [eax], al             
  0x00CA3772  56                      push     esi                            
  0x00CA3773  0037                    add      byte ptr [edi], dh             
  0x00CA3775  06                      push     es                             
  0x00CA3776  0000                    add      byte ptr [eax], al             
  0x00CA3778  0000                    add      byte ptr [eax], al             
  0x00CA377A  2400                    and      al, 0                          
  0x00CA377C  00f4                    add      ah, dh                         
  0x00CA377E  60                      pushal                                  
  0x00CA377F  002d00000045            add      byte ptr [0x45000000], ch      
  0x00CA3785  f4                      hlt                                     
  0x00CA3786  61                      popal                                   
  0x00CA3787  004100                  add      byte ptr [ecx], al             
  0x00CA378A  0000                    add      byte ptr [eax], al             
  0x00CA378C  05a4050000              add      eax, 0x5a4                     
  0x00CA3791  f4                      hlt                                     
                                        ; XREF: 0x00CA3715 (cond_jump)
  0x00CA3792  60                      pushal                                  
  0x00CA3793  0000                    add      byte ptr [eax], al             
  0x00CA3795  0000                    add      byte ptr [eax], al             
  0x00CA3797  0000                    add      byte ptr [eax], al             
  0x00CA3799  f4                      hlt                                     
  0x00CA379A  61                      popal                                   
  0x00CA379B  001400                  add      byte ptr [eax + eax], dl       
  0x00CA379E  0000                    add      byte ptr [eax], al             
  0x00CA37A0  007060                  add      byte ptr [eax + 0x60], dh      
  0x00CA37A3  008706000000            add      byte ptr [edi + 6], al         
  0x00CA37A9  7061                    jo       0xca380c                       
  0x00CA37AB  00880600000c            add      byte ptr [eax + 0xc000006], cl 
  0x00CA37B1  0000                    add      byte ptr [eax], al             
  0x00CA37B3  0000                    add      byte ptr [eax], al             
  0x00CA37B5  f4                      hlt                                     
  0x00CA37B6  56                      push     esi                            
  0x00CA37B7  000e                    add      byte ptr [esi], cl             
  0x00CA37B9  0000                    add      byte ptr [eax], al             
  0x00CA37BB  0000                    add      byte ptr [eax], al             
  0x00CA37BD  f4                      hlt                                     
  0x00CA37BE  57                      push     edi                            
  0x00CA37BF  0000                    add      byte ptr [eax], al             
  0x00CA37C1  0000                    add      byte ptr [eax], al             
  0x00CA37C3  0000                    add      byte ptr [eax], al             
  0x00CA37C5  4e                      dec      esi                            
  0x00CA37C6  3800                    cmp      byte ptr [eax], al             
  0x00CA37C8  80f00b                  xor      al, 0xb                        
  0x00CA37CB  00800100000c            add      byte ptr [eax + 0xc000001], al 
  0x00CA37D1  0000                    add      byte ptr [eax], al             
  0x00CA37D3  0000                    add      byte ptr [eax], al             
  0x00CA37D5  f4                      hlt                                     
  0x00CA37D6  56                      push     esi                            
  0x00CA37D7  000e                    add      byte ptr [esi], cl             
  0x00CA37D9  0000                    add      byte ptr [eax], al             
  0x00CA37DB  0000                    add      byte ptr [eax], al             
  0x00CA37DD  f4                      hlt                                     
  0x00CA37DE  57                      push     edi                            
  0x00CA37DF  0001                    add      byte ptr [ecx], al             
  0x00CA37E1  0000                    add      byte ptr [eax], al             
  0x00CA37E3  0000                    add      byte ptr [eax], al             
  0x00CA37E5  f4                      hlt                                     
  0x00CA37E6  60                      pushal                                  
  0x00CA37E7  0039                    add      byte ptr [ecx], bh             
  0x00CA37E9  06                      push     es                             
  0x00CA37EA  0000                    add      byte ptr [eax], al             
  0x00CA37EC  004e38                  add      byte ptr [esi + 0x38], cl      
  0x00CA37EF  0000                    add      byte ptr [eax], al             
  0x00CA37F1  0039                    add      byte ptr [ecx], bh             
  0x00CA37F3  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA37F9  0100                    add      dword ptr [eax], eax           
  0x00CA37FB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA37FE  0000                    add      byte ptr [eax], al             
  0x00CA3800  00f4                    add      ah, dh                         
  0x00CA3802  56                      push     esi                            
  0x00CA3803  000e                    add      byte ptr [esi], cl             
  0x00CA3805  0000                    add      byte ptr [eax], al             
  0x00CA3807  0000                    add      byte ptr [eax], al             
  0x00CA3809  f4                      hlt                                     
  0x00CA380A  57                      push     edi                            
  0x00CA380B  0002                    add      byte ptr [edx], al             
  0x00CA380D  0000                    add      byte ptr [eax], al             
  0x00CA380F  0000                    add      byte ptr [eax], al             
  0x00CA3811  f4                      hlt                                     
  0x00CA3812  60                      pushal                                  
  0x00CA3813  0039                    add      byte ptr [ecx], bh             
  0x00CA3815  06                      push     es                             
  0x00CA3816  0000                    add      byte ptr [eax], al             
  0x00CA3818  004e38                  add      byte ptr [esi + 0x38], cl      
  0x00CA381B  0000                    add      byte ptr [eax], al             
  0x00CA381D  0039                    add      byte ptr [ecx], bh             
  0x00CA381F  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA3825  0100                    add      dword ptr [eax], eax           
  0x00CA3827  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA382A  0000                    add      byte ptr [eax], al             
  0x00CA382C  00f4                    add      ah, dh                         
  0x00CA382E  7100                    jno      0xca3830                       
  0x00CA3832  ff00                    inc      dword ptr [eax]                
  0x00CA3834  00f4                    add      ah, dh                         
  0x00CA3836  7500                    jne      0xca3838                       
                                        ; XREF: 0x00CA3836 (cond_jump)
  0x00CA3838  fc                      cld                                     
  0x00CA383A  ff00                    inc      dword ptr [eax]                
  0x00CA383C  0096220010da            add      byte ptr [esi - 0x25efffde], dl 
  0x00CA3842  06                      push     es                             
  0x00CA3843  0021                    add      byte ptr [ecx], ah             
  0x00CA3845  0000                    add      byte ptr [eax], al             
  0x00CA3847  0000                    add      byte ptr [eax], al             
  0x00CA3849  da5700                  ficom    dword ptr [edi]                
  0x00CA384C  00d2                    add      dl, dl                         
  0x00CA384E  51                      push     ecx                            
  0x00CA384F  0000                    add      byte ptr [eax], al             
  0x00CA3851  b9f00010de              mov      ecx, 0xde1000f0                
  0x00CA3856  06                      push     es                             
  0x00CA3857  000b                    add      byte ptr [ebx], cl             
  0x00CA3859  0000                    add      byte ptr [eax], al             
  0x00CA385B  00d4                    add      ah, dl                         
  0x00CA385D  e145                    loope    0xca38a4                       
  0x00CA385F  00d6                    add      dh, dl                         
  0x00CA3861  39f0                    cmp      eax, esi                       
  0x00CA3863  00e6                    add      dh, ah                         
  0x00CA3865  a8f0                    test     al, 0xf0                       
  0x00CA3867  00d2                    add      dl, dl                         
  0x00CA3869  a1f400e259              mov      eax, dword ptr [0x59e200f4]    
  0x00CA386E  44                      inc      esp                            
  0x00CA386F  00e2                    add      dl, ah                         
  0x00CA3871  a1d000d249              mov      eax, dword ptr [0x49d200d0]    
  0x00CA3876  45                      inc      ebp                            
  0x00CA3877  0010                    add      byte ptr [eax], dl             
  0x00CA3879  0020                    add      byte ptr [eax], ah             
  0x00CA387B  0009                    add      byte ptr [ecx], cl             
  0x00CA387D  dd10                    fst      qword ptr [eax]                
  0x00CA387F  004c4c44                add      byte ptr [esp + ecx*2 + 0x44], cl 
  0x00CA3883  00d4                    add      ah, dl                         
  0x00CA3885  e145                    loope    0xca38cc                       
  0x00CA3887  00d6                    add      dh, dl                         
  0x00CA3889  39f0                    cmp      eax, esi                       
  0x00CA388B  00e6                    add      dh, ah                         
  0x00CA388D  a8f0                    test     al, 0xf0                       
  0x00CA388F  00d2                    add      dl, dl                         
  0x00CA3891  a1f400e259              mov      eax, dword ptr [0x59e200f4]    
  0x00CA3896  44                      inc      esp                            
  0x00CA3897  00e2                    add      dl, ah                         
  0x00CA3899  a1f000d259              mov      eax, dword ptr [0x59d200f0]    
  0x00CA389E  45                      inc      ebp                            
  0x00CA389F  0010                    add      byte ptr [eax], dl             
  0x00CA38A1  0020                    add      byte ptr [eax], ah             
  0x00CA38A3  0009                    add      byte ptr [ecx], cl             
  0x00CA38A5  c421                    les      esp, ptr [ecx]                 
  0x00CA38A7  004c4c44                add      byte ptr [esp + ecx*2 + 0x44], cl 
  0x00CA38AB  0084f10300005a          add      byte ptr [ecx + esi*8 + 0x5a000003], al 
  0x00CA38B2  55                      push     ebp                            
  0x00CA38B3  0000                    add      byte ptr [eax], al             
  0x00CA38B5  5a                      pop      edx                            
  0x00CA38B6  51                      push     ecx                            
  0x00CA38B7  0000                    add      byte ptr [eax], al             
  0x00CA38B9  d422                    aam      0x22                           
  0x00CA38BB  0000                    add      byte ptr [eax], al             
  0x00CA38BD  90                      nop                                     
  0x00CA38BE  2200                    and      al, byte ptr [eax]             
  0x00CA38C0  00982300a460            add      byte ptr [eax + 0x60a40023], bl 
  0x00CA38C6  0400                    add      al, 0                          
  0x00CA38C8  0c00                    or       al, 0                          
  0x00CA38CA  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA3885 (cond_jump)
  0x00CA38CC  0000                    add      byte ptr [eax], al             
  0x00CA38CE  0000                    add      byte ptr [eax], al             
  0x00CA38D0  40                      inc      eax                            
  0x00CA38D1  1bd0                    sbb      edx, eax                       
  0x00CA38D3  00440800                add      byte ptr [eax + ecx], al       
  0x00CA38D7  005001                  add      byte ptr [eax + 1], dl         
  0x00CA38DA  0200                    add      al, byte ptr [eax]             
  0x00CA38DC  09e8                    or       eax, ebp                       
  0x00CA38DE  7200                    jb       0xca38e0                       
                                        ; XREF: 0x00CA38DE (cond_jump)
  0x00CA38E0  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA38E3  005f09                  add      byte ptr [edi + 9], bl         
  0x00CA38E6  0000                    add      byte ptr [eax], al             
  0x00CA38E8  007060                  add      byte ptr [eax + 0x60], dh      
  0x00CA38EB  006209                  add      byte ptr [edx + 9], ah         
  0x00CA38EE  0000                    add      byte ptr [eax], al             
  0x00CA38F0  0b00                    or       eax, dword ptr [eax]           
  0x00CA38F2  2000                    and      byte ptr [eax], al             
  0x00CA38F4  07                      pop      es                             
  0x00CA38F5  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA38F6  0500df0805              add      eax, 0x508df00                 
  0x00CA38FB  0080100d00ba            add      byte ptr [eax - 0x45fff2f0], al 
  0x00CA3901  07                      pop      es                             
  0x00CA3902  0000                    add      byte ptr [eax], al             
  0x00CA3904  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA3907  00ed                    add      ch, ch                         
  0x00CA3909  07                      pop      es                             
  0x00CA390A  0000                    add      byte ptr [eax], al             
  0x00CA390C  050c050080              add      eax, 0x8000050c                
  0x00CA3911  100d00c00700            adc      byte ptr [0x7c000], cl         
  0x00CA3917  0080100d00da            add      byte ptr [eax - 0x25fff2f0], al 
  0x00CA391D  07                      pop      es                             
  0x00CA391E  0000                    add      byte ptr [eax], al             
  0x00CA3921  08050000f062            or       byte ptr [0x62f00000], al      
  0x00CA3927  006209                  add      byte ptr [edx + 9], ah         
  0x00CA392A  0000                    add      byte ptr [eax], al             
  0x00CA392C  00f4                    add      ah, dh                         
  0x00CA392E  60                      pushal                                  
  0x00CA392F  00c2                    add      dl, al                         
  0x00CA3931  0f0000                  sldt     word ptr [eax]                 
  0x00CA3934  d8720a                  fdiv     dword ptr [edx + 0xa]          
  0x00CA3937  000500000000            add      byte ptr [0], al               
  0x00CA393D  002400                  add      byte ptr [eax + eax], ah       
  0x00CA3940  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA3943  006009                  add      byte ptr [eax + 9], ah         
  0x00CA3946  0000                    add      byte ptr [eax], al             
  0x00CA3948  00e8                    add      al, ch                         
  0x00CA394A  5e                      pop      esi                            
  0x00CA394B  009f1a02000b            add      byte ptr [edi + 0xb00021a], bl 
  0x00CA3951  0020                    add      byte ptr [eax], ah             
  0x00CA3953  0002                    add      byte ptr [edx], al             
  0x00CA3955  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA3956  0500804101              add      eax, 0x1418000                 
  0x00CA395B  0000                    add      byte ptr [eax], al             
  0x00CA395D  7054                    jo       0xca39b3                       
  0x00CA395F  006109                  add      byte ptr [ecx + 9], ah         
  0x00CA3962  0000                    add      byte ptr [eax], al             
  0x00CA3964  00f0                    add      al, dh                         
  0x00CA3966  44                      inc      esp                            
  0x00CA3967  006009                  add      byte ptr [eax + 9], ah         
  0x00CA396A  0000                    add      byte ptr [eax], al             
  0x00CA396C  52                      push     edx                            
  0x00CA396D  090500110805            or       dword ptr [0x5081100], eax     
  0x00CA3973  0000                    add      byte ptr [eax], al             
  0x00CA3976  44                      inc      esp                            
  0x00CA3977  006009                  add      byte ptr [eax + 9], ah         
  0x00CA397A  0000                    add      byte ptr [eax], al             
  0x00CA397C  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA397F  00e4                    add      ah, ah                         
  0x00CA3981  07                      pop      es                             
  0x00CA3982  0000                    add      byte ptr [eax], al             
  0x00CA3984  00f0                    add      al, dh                         
  0x00CA3986  56                      push     esi                            
  0x00CA3987  006009                  add      byte ptr [eax + 9], ah         
  0x00CA398A  0000                    add      byte ptr [eax], al             
  0x00CA398C  80410100                add      byte ptr [ecx + 1], 0          
  0x00CA3990  00f0                    add      al, dh                         
  0x00CA3992  44                      inc      esp                            
  0x00CA3993  006109                  add      byte ptr [ecx + 9], ah         
  0x00CA3996  0000                    add      byte ptr [eax], al             
  0x00CA3998  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA399B  006009                  add      byte ptr [eax + 9], ah         
  0x00CA399E  0000                    add      byte ptr [eax], al             
  0x00CA39A0  45                      inc      ebp                            
  0x00CA39A1  0020                    add      byte ptr [eax], ah             
  0x00CA39A3  00d0                    add      al, dl                         
  0x00CA39A5  97                      xchg     edi, eax                       
  0x00CA39A6  050080100d              add      eax, 0xd108000                 
  0x00CA39AB  00a80700000c            add      byte ptr [eax + 0xc000007], ch 
  0x00CA39B1  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA395D (cond_jump)
  0x00CA39B3  0000                    add      byte ptr [eax], al             
  0x00CA39B6  56                      push     esi                            
  0x00CA39B7  006009                  add      byte ptr [eax + 9], ah         
  0x00CA39BA  0000                    add      byte ptr [eax], al             
  0x00CA39BC  00f0                    add      al, dh                         
  0x00CA39BE  44                      inc      esp                            
  0x00CA39BF  005f09                  add      byte ptr [edi + 9], bl         
  0x00CA39C2  0000                    add      byte ptr [eax], al             
  0x00CA39C4  40                      inc      eax                            
  0x00CA39C5  190c00                  sbb      dword ptr [eax + eax], ecx     
  0x00CA39C8  208000000070            and      byte ptr [eax + 0x70000000], al 
  0x00CA39CE  54                      push     esp                            
  0x00CA39CF  003d09000000            add      byte ptr [9], bh               
  0x00CA39D6  56                      push     esi                            
  0x00CA39D7  006009                  add      byte ptr [eax + 9], ah         
  0x00CA39DA  0000                    add      byte ptr [eax], al             
  0x00CA39DC  0300                    add      eax, dword ptr [eax]           
  0x00CA39DE  2000                    and      byte ptr [eax], al             
  0x00CA39E0  5a                      pop      edx                            
  0x00CA39E1  2405                    and      al, 5                          
  0x00CA39E3  0000                    add      byte ptr [eax], al             
  0x00CA39E6  6200                    bound    eax, qword ptr [eax]           
  0x00CA39E8  6209                    bound    ecx, qword ptr [ecx]           
  0x00CA39EA  0000                    add      byte ptr [eax], al             
  0x00CA39EC  00f4                    add      ah, dh                         
  0x00CA39EE  60                      pushal                                  
  0x00CA39EF  004d09                  add      byte ptr [ebp + 9], cl         
  0x00CA39F2  0000                    add      byte ptr [eax], al             
  0x00CA39F4  00f4                    add      ah, dh                         
  0x00CA39F6  44                      inc      esp                            
  0x00CA39F7  008000000090            add      byte ptr [eax - 0x70000000], al 
  0x00CA39FD  06                      push     es                             
  0x00CA39FE  06                      push     es                             
  0x00CA39FF  0002                    add      byte ptr [edx], al             
  0x00CA3A01  0000                    add      byte ptr [eax], al             
  0x00CA3A03  0000                    add      byte ptr [eax], al             
  0x00CA3A05  58                      pop      eax                            
  0x00CA3A06  44                      inc      esp                            
  0x00CA3A07  00de                    add      dh, bl                         
  0x00CA3A09  1202                    adc      al, byte ptr [edx]             
  0x00CA3A0B  00941a02004019          add      byte ptr [edx + ebx + 0x19400002], dl 
  0x00CA3A12  0c00                    or       al, 0                          
  0x00CA3A14  1b10                    sbb      edx, dword ptr [eax]           
  0x00CA3A16  0000                    add      byte ptr [eax], al             
  0x00CA3A18  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA3A1B  0030                    add      byte ptr [eax], dh             
  0x00CA3A1D  0900                    or       dword ptr [eax], eax           
  0x00CA3A1F  0013                    add      byte ptr [ebx], dl             
  0x00CA3A21  f4                      hlt                                     
  0x00CA3A22  44                      inc      esp                            
  0x00CA3A23  0012                    add      byte ptr [edx], dl             
  0x00CA3A25  0000                    add      byte ptr [eax], al             
  0x00CA3A27  004019                  add      byte ptr [eax + 0x19], al      
  0x00CA3A2A  0c00                    or       al, 0                          
  0x00CA3A2C  215000                  and      dword ptr [eax], edx           
  0x00CA3A2F  0000                    add      byte ptr [eax], al             
  0x00CA3A31  7054                    jo       0xca3a87                       
  0x00CA3A33  003409                  add      byte ptr [ecx + ecx], dh       
  0x00CA3A36  0000                    add      byte ptr [eax], al             
  0x00CA3A38  9e                      sahf                                    
  0x00CA3A39  2202                    and      al, byte ptr [edx]             
  0x00CA3A3B  00d4                    add      ah, dl                         
  0x00CA3A3D  2a02                    sub      al, byte ptr [edx]             
  0x00CA3A3F  004019                  add      byte ptr [eax + 0x19], al      
  0x00CA3A42  0c00                    or       al, 0                          
  0x00CA3A44  2110                    and      dword ptr [eax], edx           
  0x00CA3A46  0000                    add      byte ptr [eax], al             
  0x00CA3A48  94                      xchg     esp, eax                       
  0x00CA3A49  2a02                    sub      al, byte ptr [edx]             
  0x00CA3A4B  004019                  add      byte ptr [eax + 0x19], al      
  0x00CA3A4E  0c00                    or       al, 0                          
  0x00CA3A50  2210                    and      dl, byte ptr [eax]             
  0x00CA3A52  0000                    add      byte ptr [eax], al             
  0x00CA3A54  d422                    aam      0x22                           
  0x00CA3A56  0200                    add      al, byte ptr [eax]             
  0x00CA3A58  40                      inc      eax                            
  0x00CA3A59  190c00                  sbb      dword ptr [eax + eax], ecx     
  0x00CA3A5C  2310                    and      edx, dword ptr [eax]           
  0x00CA3A5E  0000                    add      byte ptr [eax], al             
  0x00CA3A60  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA3A63  003509000013            add      byte ptr [0x13000009], dh      
  0x00CA3A69  0020                    add      byte ptr [eax], ah             
  0x00CA3A6B  00944a02004019          add      byte ptr [edx + ecx*2 + 0x19400002], dl 
  0x00CA3A72  0c00                    or       al, 0                          
  0x00CA3A74  1a20                    sbb      ah, byte ptr [eax]             
  0x00CA3A76  0000                    add      byte ptr [eax], al             
  0x00CA3A78  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA3A7B  004109                  add      byte ptr [ecx + 9], al         
  0x00CA3A7E  0000                    add      byte ptr [eax], al             
  0x00CA3A80  d41a                    aam      0x1a                           
  0x00CA3A82  0200                    add      al, byte ptr [eax]             
  0x00CA3A84  007044                  add      byte ptr [eax + 0x44], dh      
                                        ; XREF: 0x00CA3A31 (cond_jump)
  0x00CA3A87  004209                  add      byte ptr [edx + 9], al         
  0x00CA3A8A  0000                    add      byte ptr [eax], al             
  0x00CA3A8C  1300                    adc      eax, dword ptr [eax]           
  0x00CA3A8E  2000                    and      byte ptr [eax], al             
  0x00CA3A90  94                      xchg     esp, eax                       
  0x00CA3A91  3a02                    cmp      al, byte ptr [edx]             
  0x00CA3A93  004019                  add      byte ptr [eax + 0x19], al      
  0x00CA3A96  0c00                    or       al, 0                          
  0x00CA3A98  1810                    sbb      byte ptr [eax], dl             
  0x00CA3A9A  0000                    add      byte ptr [eax], al             
  0x00CA3A9C  94                      xchg     esp, eax                       
  0x00CA3A9D  3202                    xor      al, byte ptr [edx]             
  0x00CA3A9F  004019                  add      byte ptr [eax + 0x19], al      
  0x00CA3AA2  0c00                    or       al, 0                          
  0x00CA3AA4  1910                    sbb      dword ptr [eax], edx           
  0x00CA3AA6  0000                    add      byte ptr [eax], al             
  0x00CA3AA8  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA3AAB  003e                    add      byte ptr [esi], bh             
  0x00CA3AAD  0900                    or       dword ptr [eax], eax           
  0x00CA3AAF  00d4                    add      ah, dl                         
  0x00CA3AB1  3a02                    cmp      al, byte ptr [edx]             
  0x00CA3AB3  0000                    add      byte ptr [eax], al             
  0x00CA3AB5  7044                    jo       0xca3afb                       
  0x00CA3AB7  004009                  add      byte ptr [eax + 9], al         
  0x00CA3ABA  0000                    add      byte ptr [eax], al             
  0x00CA3ABC  d432                    aam      0x32                           
  0x00CA3ABE  0200                    add      al, byte ptr [eax]             
  0x00CA3AC0  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA3AC3  003f                    add      byte ptr [edi], bh             
  0x00CA3AC5  0900                    or       dword ptr [eax], eax           
  0x00CA3AC7  0013                    add      byte ptr [ebx], dl             
  0x00CA3AC9  f4                      hlt                                     
  0x00CA3ACA  60                      pushal                                  
  0x00CA3ACB  002c09                  add      byte ptr [ecx + ecx], ch       
  0x00CA3ACE  0000                    add      byte ptr [eax], al             
  0x00CA3AD0  00f4                    add      ah, dh                         
  0x00CA3AD2  57                      push     edi                            
  0x00CA3AD3  0010                    add      byte ptr [eax], dl             
  0x00CA3AD5  0000                    add      byte ptr [eax], al             
  0x00CA3AD7  0080100d00a9            add      byte ptr [eax - 0x56fff2f0], al 
  0x00CA3ADD  0200                    add      al, byte ptr [eax]             
  0x00CA3ADF  0000                    add      byte ptr [eax], al             
  0x00CA3AE1  f4                      hlt                                     
  0x00CA3AE2  44                      inc      esp                            
  0x00CA3AE3  0000                    add      byte ptr [eax], al             
  0x00CA3AE5  0000                    add      byte ptr [eax], al             
  0x00CA3AE7  004500                  add      byte ptr [ebp], al             
  0x00CA3AEA  2000                    and      byte ptr [eax], al             
  0x00CA3AEC  00740500                add      byte ptr [ebp + eax], dh       
  0x00CA3AF0  0c00                    or       al, 0                          
  0x00CA3AF2  0000                    add      byte ptr [eax], al             
  0x00CA3AF4  1b00                    sbb      eax, dword ptr [eax]           
  0x00CA3AF6  3000                    xor      byte ptr [eax], al             
  0x00CA3AF8  80100d                  adc      byte ptr [eax], 0xd            
                                        ; XREF: 0x00CA3AB5 (cond_jump)
  0x00CA3AFB  00a10200000c            add      byte ptr [ecx + 0xc000002], ah 
  0x00CA3B01  0000                    add      byte ptr [eax], al             
  0x00CA3B03  0000                    add      byte ptr [eax], al             
  0x00CA3B05  f4                      hlt                                     
  0x00CA3B06  44                      inc      esp                            
  0x00CA3B07  001500000000            add      byte ptr [0], dl               
  0x00CA3B0D  7044                    jo       0xca3b53                       
  0x00CA3B0F  002c09                  add      byte ptr [ecx + ecx], ch       
  0x00CA3B12  0000                    add      byte ptr [eax], al             
  0x00CA3B14  00f4                    add      ah, dh                         
  0x00CA3B16  44                      inc      esp                            
  0x00CA3B17  004d09                  add      byte ptr [ebp + 9], cl         
  0x00CA3B1A  0000                    add      byte ptr [eax], al             
  0x00CA3B1C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA3B1F  002d09000000            add      byte ptr [9], ch               
  0x00CA3B25  f4                      hlt                                     
  0x00CA3B26  44                      inc      esp                            
  0x00CA3B27  005909                  add      byte ptr [ecx + 9], bl         
  0x00CA3B2A  0000                    add      byte ptr [eax], al             
  0x00CA3B2C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA3B2F  002e                    add      byte ptr [esi], ch             
  0x00CA3B31  0900                    or       dword ptr [eax], eax           
  0x00CA3B33  0000                    add      byte ptr [eax], al             
  0x00CA3B35  f4                      hlt                                     
  0x00CA3B36  44                      inc      esp                            
  0x00CA3B37  005309                  add      byte ptr [ebx + 9], dl         
  0x00CA3B3A  0000                    add      byte ptr [eax], al             
  0x00CA3B3C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA3B3F  002f                    add      byte ptr [edi], ch             
  0x00CA3B41  0900                    or       dword ptr [eax], eax           
  0x00CA3B43  0000                    add      byte ptr [eax], al             
  0x00CA3B45  f4                      hlt                                     
  0x00CA3B46  44                      inc      esp                            
  0x00CA3B47  00ff                    add      bh, bh                         
  0x00CA3B49  ff00                    inc      dword ptr [eax]                
  0x00CA3B4B  0000                    add      byte ptr [eax], al             
  0x00CA3B4D  7044                    jo       0xca3b93                       
  0x00CA3B4F  0033                    add      byte ptr [ebx], dh             
  0x00CA3B51  0900                    or       dword ptr [eax], eax           
                                        ; XREF: 0x00CA3B0D (cond_jump)
  0x00CA3B53  0000                    add      byte ptr [eax], al             
  0x00CA3B55  f4                      hlt                                     
  0x00CA3B56  44                      inc      esp                            
  0x00CA3B57  004109                  add      byte ptr [ecx + 9], al         
  0x00CA3B5A  0000                    add      byte ptr [eax], al             
  0x00CA3B5C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA3B5F  0036                    add      byte ptr [esi], dh             
  0x00CA3B61  0900                    or       dword ptr [eax], eax           
  0x00CA3B63  0000                    add      byte ptr [eax], al             
  0x00CA3B65  002400                  add      byte ptr [eax + eax], ah       
  0x00CA3B68  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA3B6B  0031                    add      byte ptr [ecx], dh             
  0x00CA3B6D  0900                    or       dword ptr [eax], eax           
  0x00CA3B6F  0000                    add      byte ptr [eax], al             
  0x00CA3B71  7044                    jo       0xca3bb7                       
  0x00CA3B73  0032                    add      byte ptr [edx], dh             
  0x00CA3B75  0900                    or       dword ptr [eax], eax           
  0x00CA3B77  0000                    add      byte ptr [eax], al             
  0x00CA3B79  7044                    jo       0xca3bbf                       
  0x00CA3B7B  0037                    add      byte ptr [edi], dh             
  0x00CA3B7D  0900                    or       dword ptr [eax], eax           
  0x00CA3B7F  0000                    add      byte ptr [eax], al             
  0x00CA3B81  7044                    jo       0xca3bc7                       
  0x00CA3B83  0038                    add      byte ptr [eax], bh             
  0x00CA3B85  0900                    or       dword ptr [eax], eax           
  0x00CA3B87  0000                    add      byte ptr [eax], al             
  0x00CA3B89  7044                    jo       0xca3bcf                       
  0x00CA3B8B  0039                    add      byte ptr [ecx], bh             
  0x00CA3B8D  0900                    or       dword ptr [eax], eax           
  0x00CA3B8F  0000                    add      byte ptr [eax], al             
  0x00CA3B91  7044                    jo       0xca3bd7                       
                                        ; XREF: 0x00CA3B4D (cond_jump)
  0x00CA3B93  003a                    add      byte ptr [edx], bh             
  0x00CA3B95  0900                    or       dword ptr [eax], eax           
  0x00CA3B97  0000                    add      byte ptr [eax], al             
  0x00CA3B99  7044                    jo       0xca3bdf                       
  0x00CA3B9B  003b                    add      byte ptr [ebx], bh             
  0x00CA3B9D  0900                    or       dword ptr [eax], eax           
  0x00CA3B9F  0000                    add      byte ptr [eax], al             
  0x00CA3BA1  7044                    jo       0xca3be7                       
  0x00CA3BA3  003c09                  add      byte ptr [ecx + ecx], bh       
  0x00CA3BA6  0000                    add      byte ptr [eax], al             
  0x00CA3BA8  00f4                    add      ah, dh                         
  0x00CA3BAA  60                      pushal                                  
  0x00CA3BAB  004709                  add      byte ptr [edi + 9], al         
  0x00CA3BAE  0000                    add      byte ptr [eax], al             
  0x00CA3BB0  00f4                    add      ah, dh                         
  0x00CA3BB2  44                      inc      esp                            
  0x00CA3BB3  0000                    add      byte ptr [eax], al             
  0x00CA3BB5  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA3B71 (cond_jump)
  0x00CA3BB7  0000                    add      byte ptr [eax], al             
  0x00CA3BB9  58                      pop      eax                            
  0x00CA3BBA  44                      inc      esp                            
  0x00CA3BBB  0000                    add      byte ptr [eax], al             
  0x00CA3BBD  f4                      hlt                                     
  0x00CA3BBE  44                      inc      esp                            
                                        ; XREF: 0x00CA3B79 (cond_jump)
  0x00CA3BBF  0002                    add      byte ptr [edx], al             
  0x00CA3BC1  0000                    add      byte ptr [eax], al             
  0x00CA3BC3  0000                    add      byte ptr [eax], al             
  0x00CA3BC5  58                      pop      eax                            
  0x00CA3BC6  44                      inc      esp                            
                                        ; XREF: 0x00CA3B81 (cond_jump)
  0x00CA3BC7  0000                    add      byte ptr [eax], al             
  0x00CA3BC9  f4                      hlt                                     
  0x00CA3BCA  44                      inc      esp                            
  0x00CA3BCB  0003                    add      byte ptr [ebx], al             
  0x00CA3BCD  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA3B89 (cond_jump)
  0x00CA3BCF  0000                    add      byte ptr [eax], al             
  0x00CA3BD1  58                      pop      eax                            
  0x00CA3BD2  44                      inc      esp                            
  0x00CA3BD3  0000                    add      byte ptr [eax], al             
  0x00CA3BD5  f4                      hlt                                     
  0x00CA3BD6  44                      inc      esp                            
                                        ; XREF: 0x00CA3B91 (cond_jump)
  0x00CA3BD7  000400                  add      byte ptr [eax + eax], al       
  0x00CA3BDA  0000                    add      byte ptr [eax], al             
  0x00CA3BDC  005844                  add      byte ptr [eax + 0x44], bl      
                                        ; XREF: 0x00CA3B99 (cond_jump)
  0x00CA3BDF  0000                    add      byte ptr [eax], al             
  0x00CA3BE1  f4                      hlt                                     
  0x00CA3BE2  44                      inc      esp                            
  0x00CA3BE3  0001                    add      byte ptr [ecx], al             
  0x00CA3BE5  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA3BA1 (cond_jump)
  0x00CA3BE7  0000                    add      byte ptr [eax], al             
  0x00CA3BE9  58                      pop      eax                            
  0x00CA3BEA  44                      inc      esp                            
  0x00CA3BEB  0000                    add      byte ptr [eax], al             
  0x00CA3BED  f4                      hlt                                     
  0x00CA3BEE  44                      inc      esp                            
  0x00CA3BEF  000500000000            add      byte ptr [0], al               
  0x00CA3BF5  58                      pop      eax                            
  0x00CA3BF6  44                      inc      esp                            
  0x00CA3BF7  0000                    add      byte ptr [eax], al             
  0x00CA3BF9  f4                      hlt                                     
  0x00CA3BFA  60                      pushal                                  
  0x00CA3BFB  005909                  add      byte ptr [ecx + 9], bl         
  0x00CA3BFE  0000                    add      byte ptr [eax], al             
  0x00CA3C00  00f4                    add      ah, dh                         
  0x00CA3C02  44                      inc      esp                            
  0x00CA3C03  0001                    add      byte ptr [ecx], al             
  0x00CA3C05  0000                    add      byte ptr [eax], al             
  0x00CA3C07  009006060002            add      byte ptr [eax + 0x2000606], dl 
  0x00CA3C0D  0000                    add      byte ptr [eax], al             
  0x00CA3C0F  0000                    add      byte ptr [eax], al             
  0x00CA3C11  58                      pop      eax                            
  0x00CA3C12  44                      inc      esp                            
  0x00CA3C13  0000                    add      byte ptr [eax], al             
  0x00CA3C15  f4                      hlt                                     
  0x00CA3C16  60                      pushal                                  
  0x00CA3C17  005309                  add      byte ptr [ebx + 9], dl         
  0x00CA3C1A  0000                    add      byte ptr [eax], al             
  0x00CA3C1C  00f4                    add      ah, dh                         
  0x00CA3C1E  44                      inc      esp                            
  0x00CA3C1F  00ff                    add      bh, bh                         
  0x00CA3C21  ff00                    inc      dword ptr [eax]                
  0x00CA3C23  009006060002            add      byte ptr [eax + 0x2000606], dl 
  0x00CA3C29  0000                    add      byte ptr [eax], al             
  0x00CA3C2B  0000                    add      byte ptr [eax], al             
  0x00CA3C2D  58                      pop      eax                            
  0x00CA3C2E  44                      inc      esp                            
  0x00CA3C2F  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA3C32  0000                    add      byte ptr [eax], al             
  0x00CA3C34  00f0                    add      al, dh                         
  0x00CA3C36  6200                    bound    eax, qword ptr [eax]           
  0x00CA3C38  6209                    bound    ecx, qword ptr [ecx]           
  0x00CA3C3A  0000                    add      byte ptr [eax], al             
  0x00CA3C3C  00f4                    add      ah, dh                         
  0x00CA3C3E  45                      inc      ebp                            
  0x00CA3C3F  0003                    add      byte ptr [ebx], al             
  0x00CA3C41  0000                    add      byte ptr [eax], al             
  0x00CA3C43  00d6                    add      dh, dl                         
  0x00CA3C45  1202                    adc      al, byte ptr [edx]             
  0x00CA3C47  00e0                    add      al, ah                         
  0x00CA3C49  0020                    add      byte ptr [eax], ah             
  0x00CA3C4B  0000                    add      byte ptr [eax], al             
  0x00CA3C4D  f4                      hlt                                     
  0x00CA3C4E  6200                    bound    eax, qword ptr [eax]           
  0x00CA3C50  92                      xchg     edx, eax                       
  0x00CA3C51  0f0000                  sldt     word ptr [eax]                 
  0x00CA3C54  000e                    add      byte ptr [esi], cl             
  0x00CA3C56  2100                    and      dword ptr [eax], eax           
  0x00CA3C58  40                      inc      eax                            
  0x00CA3C59  0020                    add      byte ptr [eax], ah             
  0x00CA3C5B  0000                    add      byte ptr [eax], al             
  0x00CA3C5D  f4                      hlt                                     
  0x00CA3C5E  60                      pushal                                  
  0x00CA3C5F  008000000000            add      byte ptr [eax], al             
  0x00CA3C65  9a210000f47000          lcall    0x70, 0xf4000021               
  0x00CA3C6C  0001                    add      byte ptr [ecx], al             
  0x00CA3C6E  0000                    add      byte ptr [eax], al             
  0x00CA3C70  00f4                    add      ah, dh                         
  0x00CA3C72  56                      push     esi                            
  0x00CA3C73  0007                    add      byte ptr [edi], al             
  0x00CA3C75  0000                    add      byte ptr [eax], al             
  0x00CA3C77  0000                    add      byte ptr [eax], al             
  0x00CA3C79  ea790080010d00          ljmp     0xd:0x1800079                  
  0x00CA3C80  0300                    add      eax, dword ptr [eax]           
  0x00CA3C82  2000                    and      byte ptr [eax], al             
  0x00CA3C84  002405000c0000          add      byte ptr [eax + 0xc00], ah     
  0x00CA3C8B  0000                    add      byte ptr [eax], al             
  0x00CA3C8D  0823                    or       byte ptr [ebx], ah             
  0x00CA3C8F  000a                    add      byte ptr [edx], cl             
  0x00CA3C91  0000                    add      byte ptr [eax], al             
  0x00CA3C93  00a0c80400a0            add      byte ptr [eax - 0x5ffffb38], ah 
  0x00CA3C99  61                      popal                                   
  0x00CA3C9A  0400                    add      al, 0                          
  0x00CA3C9C  a0640400a0              mov      al, byte ptr [0xa0000464]      
  0x00CA3CA1  650400                  add      al, 0                          
  0x00CA3CA4  f8                      clc                                     
  0x00CA3CA5  0400                    add      al, 0                          
  0x00CA3CA7  0000                    add      byte ptr [eax], al             
  0x00CA3CA9  0e                      push     cs                             
  0x00CA3CAA  2300                    and      eax, dword ptr [eax]           
  0x00CA3CAC  2200                    and      al, byte ptr [eax]             
  0x00CA3CAE  2000                    and      byte ptr [eax], al             
  0x00CA3CB0  009821000014            add      byte ptr [eax + 0x14000021], bl 
  0x00CA3CB6  2200                    and      al, byte ptr [eax]             
  0x00CA3CB8  114804                  adc      dword ptr [eax + 4], ecx       
  0x00CA3CBB  0000                    add      byte ptr [eax], al             
  0x00CA3CBD  35220000e0              xor      eax, 0xe0000022                
  0x00CA3CC2  5f                      pop      edi                            
  0x00CA3CC3  0000                    add      byte ptr [eax], al             
  0x00CA3CC5  e14f                    loope    0xca3d16                       
  0x00CA3CC7  0078e0                  add      byte ptr [eax - 0x20], bh      
  0x00CA3CCA  5e                      pop      esi                            
  0x00CA3CCB  0010                    add      byte ptr [eax], dl             
  0x00CA3CCD  d806                    fadd     dword ptr [esi]                
  0x00CA3CCF  0009                    add      byte ptr [ecx], cl             
  0x00CA3CD1  0000                    add      byte ptr [eax], al             
  0x00CA3CD3  0019                    add      byte ptr [ecx], bl             
  0x00CA3CD5  d94500                  fld      dword ptr [ebp]                
  0x00CA3CD8  16                      push     ss                             
  0x00CA3CD9  0020                    add      byte ptr [eax], ah             
  0x00CA3CDB  0000                    add      byte ptr [eax], al             
  0x00CA3CDD  808f0068b88a00          or       byte ptr [edi - 0x75479800], 0 
  0x00CA3CE4  19e1                    sbb      ecx, esp                       
  0x00CA3CE6  4f                      dec      edi                            
  0x00CA3CE7  0016                    add      byte ptr [esi], dl             
  0x00CA3CE9  0020                    add      byte ptr [eax], ah             
  0x00CA3CEB  0000                    add      byte ptr [eax], al             
  0x00CA3CED  1ccf                    sbb      al, 0xcf                       
  0x00CA3CEF  00781d                  add      byte ptr [eax + 0x1d], bh      
  0x00CA3CF2  ca0000                  retf     0                              
  0x00CA3CF5  0e                      push     cs                             
  0x00CA3CF6  2300                    and      eax, dword ptr [eax]           
  0x00CA3CF8  2230                    and      dh, byte ptr [eax]             
  0x00CA3CFA  2200                    and      al, byte ptr [eax]             
  0x00CA3CFC  009821000014            add      byte ptr [eax + 0x14000021], bl 
  0x00CA3D02  2200                    and      al, byte ptr [eax]             
  0x00CA3D04  114804                  adc      dword ptr [eax + 4], ecx       
  0x00CA3D07  0000                    add      byte ptr [eax], al             
  0x00CA3D09  35220000e0              xor      eax, 0xe0000022                
  0x00CA3D0E  5f                      pop      edi                            
  0x00CA3D0F  0000                    add      byte ptr [eax], al             
  0x00CA3D11  e14f                    loope    0xca3d62                       
  0x00CA3D13  0078e0                  add      byte ptr [eax - 0x20], bh      
                                        ; XREF: 0x00CA3CC5 (cond_jump)
  0x00CA3D16  5e                      pop      esi                            
  0x00CA3D17  0010                    add      byte ptr [eax], dl             
  0x00CA3D19  d806                    fadd     dword ptr [esi]                
  0x00CA3D1B  0009                    add      byte ptr [ecx], cl             
  0x00CA3D1D  0000                    add      byte ptr [eax], al             
  0x00CA3D1F  0019                    add      byte ptr [ecx], bl             
  0x00CA3D21  d94500                  fld      dword ptr [ebp]                
  0x00CA3D24  16                      push     ss                             
  0x00CA3D25  0020                    add      byte ptr [eax], ah             
  0x00CA3D27  0000                    add      byte ptr [eax], al             
  0x00CA3D29  808f0068b88a00          or       byte ptr [edi - 0x75479800], 0 
  0x00CA3D30  19e1                    sbb      ecx, esp                       
  0x00CA3D32  4f                      dec      edi                            
  0x00CA3D33  0016                    add      byte ptr [esi], dl             
  0x00CA3D35  0020                    add      byte ptr [eax], ah             
  0x00CA3D37  0000                    add      byte ptr [eax], al             
  0x00CA3D39  1ccf                    sbb      al, 0xcf                       
  0x00CA3D3B  00781d                  add      byte ptr [eax + 0x1d], bh      
  0x00CA3D3E  ca0000                  retf     0                              
  0x00CA3D41  3022                    xor      byte ptr [edx], ah             
  0x00CA3D43  0000                    add      byte ptr [eax], al             
  0x00CA3D45  1422                    adc      al, 0x22                       
  0x00CA3D47  0011                    add      byte ptr [ecx], dl             
  0x00CA3D49  48                      dec      eax                            
  0x00CA3D4A  0400                    add      al, 0                          
  0x00CA3D4C  0035220000e0            add      byte ptr [0xe0000022], dh      
  0x00CA3D52  5f                      pop      edi                            
  0x00CA3D53  0000                    add      byte ptr [eax], al             
  0x00CA3D55  e145                    loope    0xca3d9c                       
  0x00CA3D57  006ce05e                add      byte ptr [eax + 0x5e], ch      
  0x00CA3D5B  0010                    add      byte ptr [eax], dl             
  0x00CA3D5D  d806                    fadd     dword ptr [esi]                
  0x00CA3D5F  0009                    add      byte ptr [ecx], cl             
  0x00CA3D61  0000                    add      byte ptr [eax], al             
  0x00CA3D63  0019                    add      byte ptr [ecx], bl             
  0x00CA3D66  4f                      dec      edi                            
  0x00CA3D67  0016                    add      byte ptr [esi], dl             
  0x00CA3D69  0020                    add      byte ptr [eax], ah             
  0x00CA3D6B  0000                    add      byte ptr [eax], al             
  0x00CA3D6D  808f0078b88a00          or       byte ptr [edi - 0x75478800], 0 
  0x00CA3D74  19e1                    sbb      ecx, esp                       
  0x00CA3D76  45                      inc      ebp                            
  0x00CA3D77  0016                    add      byte ptr [esi], dl             
  0x00CA3D79  0020                    add      byte ptr [eax], ah             
  0x00CA3D7B  0000                    add      byte ptr [eax], al             
  0x00CA3D7D  1ccf                    sbb      al, 0xcf                       
  0x00CA3D7F  006c1dca                add      byte ptr [ebp + ebx - 0x36], ch 
  0x00CA3D83  0000                    add      byte ptr [eax], al             
  0x00CA3D85  0e                      push     cs                             
  0x00CA3D86  2300                    and      eax, dword ptr [eax]           
  0x00CA3D88  2202                    and      al, byte ptr [edx]             
  0x00CA3D8A  3a00                    cmp      al, byte ptr [eax]             
  0x00CA3D8C  0030                    add      byte ptr [eax], dh             
  0x00CA3D8E  2200                    and      al, byte ptr [eax]             
  0x00CA3D90  009921000011            add      byte ptr [ecx + 0x11000021], bl 
  0x00CA3D96  2200                    and      al, byte ptr [eax]             
  0x00CA3D98  0032                    add      byte ptr [edx], dh             
  0x00CA3D9A  2300                    and      eax, dword ptr [eax]           
                                        ; XREF: 0x00CA3D55 (cond_jump)
  0x00CA3D9C  001422                  add      byte ptr [edx], dl             
  0x00CA3D9F  0000                    add      byte ptr [eax], al             
  0x00CA3DA1  f4                      hlt                                     
  0x00CA3DA2  6600520f                add      byte ptr [edx + 0xf], dl       
  0x00CA3DA6  0000                    add      byte ptr [eax], al             
  0x00CA3DA8  004920                  add      byte ptr [ecx + 0x20], cl      
  0x00CA3DAB  0000                    add      byte ptr [eax], al             
  0x00CA3DAD  352200185a              xor      eax, 0x5a180022                
  0x00CA3DB2  0400                    add      al, 0                          
  0x00CA3DB4  001c23                  add      byte ptr [ebx], bl             
  0x00CA3DB7  0000                    add      byte ptr [eax], al             
  0x00CA3DB9  1d23000052              sbb      eax, 0x52000023                
  0x00CA3DBE  2000                    and      byte ptr [eax], al             
  0x00CA3DC0  00e0                    add      al, ah                         
  0x00CA3DC2  5f                      pop      edi                            
  0x00CA3DC3  0000                    add      byte ptr [eax], al             
  0x00CA3DC5  c1f400                  sal      esp, 0                         
  0x00CA3DC8  00de                    add      dh, bl                         
  0x00CA3DCA  4c                      dec      esp                            
  0x00CA3DCB  00aed94f00bf            add      byte ptr [esi - 0x40ffb027], ch 
  0x00CA3DD1  e05e                    loopne   0xca3e31                       
  0x00CA3DD3  0010                    add      byte ptr [eax], dl             
  0x00CA3DD5  da06                    fiadd    dword ptr [esi]                
  0x00CA3DD7  0020                    add      byte ptr [eax], ah             
  0x00CA3DD9  0000                    add      byte ptr [eax], al             
  0x00CA3DDB  0010                    add      byte ptr [eax], dl             
  0x00CA3DDD  d206                    rol      byte ptr [esi], cl             
  0x00CA3DDF  0007                    add      byte ptr [edi], al             
  0x00CA3DE1  0000                    add      byte ptr [eax], al             
  0x00CA3DE3  0016                    add      byte ptr [esi], dl             
  0x00CA3DE5  808f00eee14500          or       byte ptr [edi + 0x45e1ee00], 0 
  0x00CA3DEC  cb                      retf                                    
  0x00CA3DED  b88a00161c              mov      eax, 0x1c16008a                
  0x00CA3DF2  cf                      iretd                                   
  0x00CA3DF3  00aed94f00bf            add      byte ptr [esi - 0x40ffb027], ch 
  0x00CA3DF9  1dca000049              sbb      eax, 0x490000ca                
  0x00CA3DFE  2000                    and      byte ptr [eax], al             
  0x00CA3E00  16                      push     ss                             
  0x00CA3E01  808f00eea88a00          or       byte ptr [edi - 0x75571200], 0 
  0x00CA3E08  cb                      retf                                    
  0x00CA3E09  e145                    loope    0xca3e50                       
  0x00CA3E0B  0016                    add      byte ptr [esi], dl             
  0x00CA3E0D  0ccf                    or       al, 0xcf                       
  0x00CA3E0F  00ea                    add      dl, ch                         
  0x00CA3E12  4f                      dec      edi                            
  0x00CA3E13  00cf                    add      bh, cl                         
  0x00CA3E15  0dca0010d2              or       eax, 0xd21000ca                
  0x00CA3E1A  06                      push     es                             
  0x00CA3E1B  0007                    add      byte ptr [edi], al             
  0x00CA3E1D  0000                    add      byte ptr [eax], al             
  0x00CA3E1F  0016                    add      byte ptr [esi], dl             
  0x00CA3E21  808f00aee14500          or       byte ptr [edi + 0x45e1ae00], 0 
  0x00CA3E28  bfb88a0016              mov      edi, 0x16008ab8                
  0x00CA3E2D  1ccf                    sbb      al, 0xcf                       
  0x00CA3E2F  00ea                    add      dl, ch                         
  0x00CA3E32  4f                      dec      edi                            
  0x00CA3E33  00cf                    add      bh, cl                         
  0x00CA3E35  1dca000049              sbb      eax, 0x490000ca                
  0x00CA3E3A  2000                    and      byte ptr [eax], al             
  0x00CA3E3C  16                      push     ss                             
  0x00CA3E3D  808f00aea88a00          or       byte ptr [edi - 0x75575200], 0 
  0x00CA3E44  bfc1f40000              mov      edi, 0xf4c1                    
  0x00CA3E49  de4c0016                fimul    word ptr [eax + eax + 0x16]    
  0x00CA3E4D  0ccf                    or       al, 0xcf                       
  0x00CA3E4F  00aed94f00bf            add      byte ptr [esi - 0x40ffb027], ch 
  0x00CA3E55  0dca00002f              or       eax, 0x2f0000ca                
  0x00CA3E5A  2300                    and      eax, dword ptr [eax]           
  0x00CA3E5C  2a4e23                  sub      cl, byte ptr [esi + 0x23]      
  0x00CA3E5F  0032                    add      byte ptr [edx], dh             
  0x00CA3E61  0020                    add      byte ptr [eax], ah             
  0x00CA3E63  0000                    add      byte ptr [eax], al             
  0x00CA3E65  b92100009a              mov      ecx, 0x9a000021                
  0x00CA3E6A  2100                    and      dword ptr [eax], eax           
  0x00CA3E6C  80cd0c                  or       ch, 0xc                        
  0x00CA3E6F  00ca                    add      dl, cl                         
  0x00CA3E72  ff00                    inc      dword ptr [eax]                
  0x00CA3E74  0002                    add      byte ptr [edx], al             
  0x00CA3E76  3800                    cmp      byte ptr [eax], al             
  0x00CA3E78  001422                  add      byte ptr [edx], dl             
  0x00CA3E7B  0000                    add      byte ptr [eax], al             
  0x00CA3E7D  1c23                    sbb      al, 0x23                       
  0x00CA3E7F  0000                    add      byte ptr [eax], al             
  0x00CA3E81  52                      push     edx                            
  0x00CA3E82  2300                    and      eax, dword ptr [eax]           
  0x00CA3E84  00f4                    add      ah, dh                         
  0x00CA3E86  6600520f                add      byte ptr [edx + 0xf], dl       
  0x00CA3E8A  0000                    add      byte ptr [eax], al             
  0x00CA3E8C  115804                  adc      dword ptr [eax + 4], ebx       
  0x00CA3E8F  0000                    add      byte ptr [eax], al             
  0x00CA3E91  1923                    sbb      dword ptr [ebx], esp           
  0x00CA3E93  0000                    add      byte ptr [eax], al             
  0x00CA3E95  352200001d              xor      eax, 0x1d000022                
  0x00CA3E9A  2300                    and      eax, dword ptr [eax]           
  0x00CA3E9C  005220                  add      byte ptr [edx + 0x20], dl      
  0x00CA3E9F  0000                    add      byte ptr [eax], al             
  0x00CA3EA1  e05f                    loopne   0xca3f02                       
  0x00CA3EA3  0000                    add      byte ptr [eax], al             
  0x00CA3EA5  c1f400                  sal      esp, 0                         
  0x00CA3EA8  00de                    add      dh, bl                         
  0x00CA3EAA  4c                      dec      esp                            
  0x00CA3EAB  00aec94f00bf            add      byte ptr [esi - 0x40ffb037], ch 
  0x00CA3EB1  e05e                    loopne   0xca3f11                       
  0x00CA3EB3  0016                    add      byte ptr [esi], dl             
  0x00CA3EB5  0020                    add      byte ptr [eax], ah             
  0x00CA3EB7  0000                    add      byte ptr [eax], al             
  0x00CA3EB9  808f00eea88a00          or       byte ptr [edi - 0x75571200], 0 
  0x00CA3EC0  cb                      retf                                    
  0x00CA3EC1  e145                    loope    0xca3f08                       
  0x00CA3EC3  0016                    add      byte ptr [esi], dl             
  0x00CA3EC5  0ccf                    or       al, 0xcf                       
  0x00CA3EC7  0010                    add      byte ptr [eax], dl             
  0x00CA3EC9  d206                    rol      byte ptr [esi], cl             
  0x00CA3ECB  0010                    add      byte ptr [eax], dl             
  0x00CA3ECD  0000                    add      byte ptr [eax], al             
  0x00CA3ECF  00ea                    add      dl, ch                         
  0x00CA3ED1  c9                      leave                                   
  0x00CA3ED2  4f                      dec      edi                            
  0x00CA3ED3  00cf                    add      bh, cl                         
  0x00CA3ED5  0dca001600              or       eax, 0x1600ca                  
  0x00CA3EDA  2000                    and      byte ptr [eax], al             
  0x00CA3EDC  00808f00aea8            add      byte ptr [eax - 0x5751ff71], al 
  0x00CA3EE2  8a00                    mov      al, byte ptr [eax]             
  0x00CA3EE4  bfc1f40000              mov      edi, 0xf4c1                    
  0x00CA3EE9  de4c0016                fimul    word ptr [eax + eax + 0x16]    
  0x00CA3EED  0ccf                    or       al, 0xcf                       
  0x00CA3EEF  00aec94f00bf            add      byte ptr [esi - 0x40ffb037], ch 
  0x00CA3EF5  0dca001600              or       eax, 0x1600ca                  
  0x00CA3EFA  2000                    and      byte ptr [eax], al             
  0x00CA3EFC  00808f00eea8            add      byte ptr [eax - 0x5711ff71], al 
                                        ; XREF: 0x00CA3EA1 (cond_jump)
  0x00CA3F02  8a00                    mov      al, byte ptr [eax]             
  0x00CA3F04  cb                      retf                                    
  0x00CA3F05  e145                    loope    0xca3f4c                       
  0x00CA3F07  0016                    add      byte ptr [esi], dl             
  0x00CA3F09  0ccf                    or       al, 0xcf                       
  0x00CA3F0B  00ea                    add      dl, ch                         
  0x00CA3F0D  c9                      leave                                   
  0x00CA3F0E  4f                      dec      edi                            
  0x00CA3F0F  00cf                    add      bh, cl                         
                                        ; XREF: 0x00CA3EB1 (cond_jump)
  0x00CA3F11  0dca001600              or       eax, 0x1600ca                  
  0x00CA3F16  2000                    and      byte ptr [eax], al             
  0x00CA3F18  00808f00aea8            add      byte ptr [eax - 0x5751ff71], al 
  0x00CA3F1E  8a00                    mov      al, byte ptr [eax]             
  0x00CA3F20  bf00200020              mov      edi, 0x20002000                
  0x00CA3F25  f4                      hlt                                     
  0x00CA3F26  0500ffff00              add      eax, 0xffff00                  
  0x00CA3F2B  0016                    add      byte ptr [esi], dl             
  0x00CA3F2D  4c                      dec      esp                            
  0x00CA3F2E  57                      push     edi                            
  0x00CA3F2F  00a061040000            add      byte ptr [eax + 0x461], ah     
  0x00CA3F35  4d                      dec      ebp                            
  0x00CA3F36  56                      push     esi                            
  0x00CA3F37  00a0640400a0            add      byte ptr [eax - 0x5ffffb9c], ah 
  0x00CA3F3D  650400                  add      al, 0                          
  0x00CA3F40  b8f300000c              mov      eax, 0xc0000f3                 
  0x00CA3F45  0000                    add      byte ptr [eax], al             
  0x00CA3F47  0000                    add      byte ptr [eax], al             
  0x00CA3F49  f4                      hlt                                     
  0x00CA3F4A  7100                    jno      0xca3f4c                       
  0x00CA3F4E  ff00                    inc      dword ptr [eax]                
  0x00CA3F50  00f4                    add      ah, dh                         
  0x00CA3F52  7500                    jne      0xca3f54                       
                                        ; XREF: 0x00CA3F52 (cond_jump)
  0x00CA3F54  fc                      cld                                     
  0x00CA3F56  ff00                    inc      dword ptr [eax]                
  0x00CA3F58  0096220010da            add      byte ptr [esi - 0x25efffde], dl 
  0x00CA3F5E  06                      push     es                             
  0x00CA3F5F  001a                    add      byte ptr [edx], bl             
  0x00CA3F61  0000                    add      byte ptr [eax], al             
  0x00CA3F63  0000                    add      byte ptr [eax], al             
  0x00CA3F65  b9f00010de              mov      ecx, 0xde1000f0                
  0x00CA3F6A  06                      push     es                             
  0x00CA3F6B  000a                    add      byte ptr [edx], cl             
  0x00CA3F6D  0000                    add      byte ptr [eax], al             
  0x00CA3F6F  00d4                    add      ah, dl                         
  0x00CA3F71  e145                    loope    0xca3fb8                       
  0x00CA3F73  00d6                    add      dh, dl                         
  0x00CA3F75  39f0                    cmp      eax, esi                       
  0x00CA3F77  00e6                    add      dh, ah                         
  0x00CA3F79  a8f0                    test     al, 0xf0                       
  0x00CA3F7B  00d2                    add      dl, dl                         
  0x00CA3F7D  a1f400e259              mov      eax, dword ptr [0x59e200f4]    
  0x00CA3F82  44                      inc      esp                            
  0x00CA3F83  00e2                    add      dl, ah                         
  0x00CA3F85  a1d000d349              mov      eax, dword ptr [0x49d300d0]    
  0x00CA3F8A  45                      inc      ebp                            
  0x00CA3F8B  0000                    add      byte ptr [eax], al             
  0x00CA3F8D  dd10                    fst      qword ptr [eax]                
  0x00CA3F8F  0000                    add      byte ptr [eax], al             
  0x00CA3F91  4c                      dec      esp                            
  0x00CA3F92  44                      inc      esp                            
  0x00CA3F93  00d4                    add      ah, dl                         
  0x00CA3F95  e145                    loope    0xca3fdc                       
  0x00CA3F97  00d6                    add      dh, dl                         
  0x00CA3F99  39f0                    cmp      eax, esi                       
  0x00CA3F9B  00e6                    add      dh, ah                         
  0x00CA3F9D  a8f0                    test     al, 0xf0                       
  0x00CA3F9F  00d2                    add      dl, dl                         
  0x00CA3FA1  a1f400e259              mov      eax, dword ptr [0x59e200f4]    
  0x00CA3FA6  44                      inc      esp                            
  0x00CA3FA7  00e2                    add      dl, ah                         
  0x00CA3FA9  a1f000d359              mov      eax, dword ptr [0x59d300f0]    
  0x00CA3FAE  45                      inc      ebp                            
  0x00CA3FAF  0000                    add      byte ptr [eax], al             
  0x00CA3FB1  4c                      dec      esp                            
  0x00CA3FB2  56                      push     esi                            
  0x00CA3FB3  008ef1030000            add      byte ptr [esi + 0x3f1], cl     
  0x00CA3FB9  d422                    aam      0x22                           
  0x00CA3FBB  0000                    add      byte ptr [eax], al             
  0x00CA3FBD  90                      nop                                     
  0x00CA3FBE  2200                    and      al, byte ptr [eax]             
  0x00CA3FC0  00982300a460            add      byte ptr [eax + 0x60a40023], bl 
  0x00CA3FC6  0400                    add      al, 0                          
  0x00CA3FC8  0c00                    or       al, 0                          
  0x00CA3FCA  0000                    add      byte ptr [eax], al             
  0x00CA3FCC  00f4                    add      ah, dh                         
  0x00CA3FCE  7100                    jno      0xca3fd0                       
  0x00CA3FD2  ff00                    inc      dword ptr [eax]                
  0x00CA3FD4  00f4                    add      ah, dh                         
  0x00CA3FD6  7500                    jne      0xca3fd8                       
                                        ; XREF: 0x00CA3FD6 (cond_jump)
  0x00CA3FD8  fc                      cld                                     
  0x00CA3FDA  ff00                    inc      dword ptr [eax]                
                                        ; XREF: 0x00CA3F95 (cond_jump)
  0x00CA3FDC  0096220010da            add      byte ptr [esi - 0x25efffde], dl 
  0x00CA3FE2  06                      push     es                             
  0x00CA3FE3  0021                    add      byte ptr [ecx], ah             
  0x00CA3FE5  0000                    add      byte ptr [eax], al             
  0x00CA3FE7  0000                    add      byte ptr [eax], al             
  0x00CA3FE9  da5700                  ficom    dword ptr [edi]                
  0x00CA3FEC  00d2                    add      dl, dl                         
  0x00CA3FEE  51                      push     ecx                            
  0x00CA3FEF  0000                    add      byte ptr [eax], al             
  0x00CA3FF1  b9f00010de              mov      ecx, 0xde1000f0                
  0x00CA3FF6  06                      push     es                             
  0x00CA3FF7  000b                    add      byte ptr [ebx], cl             
  0x00CA3FF9  0000                    add      byte ptr [eax], al             
  0x00CA3FFB  00d4                    add      ah, dl                         
  0x00CA3FFD  e145                    loope    0xca4044                       
  0x00CA3FFF  00d6                    add      dh, dl                         
  0x00CA4001  39f0                    cmp      eax, esi                       
  0x00CA4003  00e6                    add      dh, ah                         
  0x00CA4005  a8f0                    test     al, 0xf0                       
  0x00CA4007  00d2                    add      dl, dl                         
  0x00CA4009  a1f400e259              mov      eax, dword ptr [0x59e200f4]    
  0x00CA400E  44                      inc      esp                            
  0x00CA400F  00e2                    add      dl, ah                         
  0x00CA4011  a1d000d249              mov      eax, dword ptr [0x49d200d0]    
  0x00CA4016  45                      inc      ebp                            
  0x00CA4017  0010                    add      byte ptr [eax], dl             
  0x00CA4019  0020                    add      byte ptr [eax], ah             
  0x00CA401B  0009                    add      byte ptr [ecx], cl             
  0x00CA401D  dd10                    fst      qword ptr [eax]                
  0x00CA401F  004c4c44                add      byte ptr [esp + ecx*2 + 0x44], cl 
  0x00CA4023  00d4                    add      ah, dl                         
  0x00CA4025  e145                    loope    0xca406c                       
  0x00CA4027  00d6                    add      dh, dl                         
  0x00CA4029  39f0                    cmp      eax, esi                       
  0x00CA402B  00e6                    add      dh, ah                         
  0x00CA402D  a8f0                    test     al, 0xf0                       
  0x00CA402F  00d2                    add      dl, dl                         
  0x00CA4031  a1f400e259              mov      eax, dword ptr [0x59e200f4]    
  0x00CA4036  44                      inc      esp                            
  0x00CA4037  00e2                    add      dl, ah                         
  0x00CA4039  a1f000d259              mov      eax, dword ptr [0x59d200f0]    
  0x00CA403E  45                      inc      ebp                            
  0x00CA403F  0010                    add      byte ptr [eax], dl             
  0x00CA4041  0020                    add      byte ptr [eax], ah             
  0x00CA4043  0009                    add      byte ptr [ecx], cl             
  0x00CA4045  c421                    les      esp, ptr [ecx]                 
  0x00CA4047  004c4c44                add      byte ptr [esp + ecx*2 + 0x44], cl 
  0x00CA404B  0084f10300005a          add      byte ptr [ecx + esi*8 + 0x5a000003], al 
  0x00CA4052  55                      push     ebp                            
  0x00CA4053  0000                    add      byte ptr [eax], al             
  0x00CA4055  5a                      pop      edx                            
  0x00CA4056  51                      push     ecx                            
  0x00CA4057  0000                    add      byte ptr [eax], al             
  0x00CA4059  d422                    aam      0x22                           
  0x00CA405B  0000                    add      byte ptr [eax], al             
  0x00CA405D  90                      nop                                     
  0x00CA405E  2200                    and      al, byte ptr [eax]             
  0x00CA4060  00982300a460            add      byte ptr [eax + 0x60a40023], bl 
  0x00CA4066  0400                    add      al, 0                          
  0x00CA4068  0c00                    or       al, 0                          
  0x00CA406A  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA4025 (cond_jump)
  0x00CA406C  00c8                    add      al, cl                         
  0x00CA406E  44                      inc      esp                            
  0x00CA406F  00a000200014            add      byte ptr [eax + 0x14002000], ah 
  0x00CA4075  c8440011                enter    0x44, 0x11                     
  0x00CA4079  0020                    add      byte ptr [eax], ah             
  0x00CA407B  0010                    add      byte ptr [eax], dl             
  0x00CA407D  de06                    fiadd    word ptr [esi]                 
  0x00CA407F  0005000000a0            add      byte ptr [0xa0000000], al      
  0x00CA4085  0c18                    or       al, 0x18                       
  0x00CA4087  00bac8440014            add      byte ptr [edx + 0x140044c8], bh 
  0x00CA408D  0020                    add      byte ptr [eax], ah             
  0x00CA408F  0011                    add      byte ptr [ecx], dl             
  0x00CA4091  0020                    add      byte ptr [eax], ah             
  0x00CA4093  0000                    add      byte ptr [eax], al             
  0x00CA4095  2418                    and      al, 0x18                       
  0x00CA4097  00ba0020000c            add      byte ptr [edx + 0xc002000], bh 
  0x00CA409D  0000                    add      byte ptr [eax], al             
  0x00CA409F  0013                    add      byte ptr [ebx], dl             
  0x00CA40A1  c84600e1                enter    0x46, -0x1f                    
  0x00CA40A5  0020                    add      byte ptr [eax], ah             
  0x00CA40A7  0010                    add      byte ptr [eax], dl             
  0x00CA40A9  de06                    fiadd    word ptr [esi]                 
  0x00CA40AB  0003                    add      byte ptr [ebx], al             
  0x00CA40AD  0000                    add      byte ptr [eax], al             
  0x00CA40AF  0000                    add      byte ptr [eax], al             
  0x00CA40B1  c84600e1                enter    0x46, -0x1f                    
  0x00CA40B5  4c                      dec      esp                            
  0x00CA40B6  56                      push     esi                            
  0x00CA40B7  0000                    add      byte ptr [eax], al             
  0x00CA40B9  6456                    push     esi                            
  0x00CA40BB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA40BE  0000                    add      byte ptr [eax], al             
  0x00CA40C0  004039                  add      byte ptr [eax + 0x39], al      
  0x00CA40C3  0000                    add      byte ptr [eax], al             
  0x00CA40C5  3d23000049              cmp      eax, 0x49000023                
  0x00CA40CA  2000                    and      byte ptr [eax], al             
  0x00CA40CC  004d20                  add      byte ptr [ebp + 0x20], cl      
  0x00CA40CF  0012                    add      byte ptr [edx], dl             
  0x00CA40D1  51                      push     ecx                            
  0x00CA40D2  0400                    add      al, 0                          
  0x00CA40D4  16                      push     ss                             
  0x00CA40D5  55                      push     ebp                            
  0x00CA40D6  0400                    add      al, 0                          
  0x00CA40D8  10d9                    adc      cl, bl                         
  0x00CA40DA  06                      push     es                             
  0x00CA40DB  000400                  add      byte ptr [eax + eax], al       
  0x00CA40DE  0000                    add      byte ptr [eax], al             
  0x00CA40E0  00d9                    add      cl, bl                         
  0x00CA40E2  46                      inc      esi                            
  0x00CA40E3  0000                    add      byte ptr [eax], al             
  0x00CA40E5  b2b0                    mov      dl, 0xb0                       
  0x00CA40E7  0000                    add      byte ptr [eax], al             
  0x00CA40E9  56                      push     esi                            
  0x00CA40EA  44                      inc      esp                            
  0x00CA40EB  0000                    add      byte ptr [eax], al             
  0x00CA40EE  3800                    cmp      byte ptr [eax], al             
  0x00CA40F0  001c23                  add      byte ptr [ebx], bl             
  0x00CA40F3  0000                    add      byte ptr [eax], al             
  0x00CA40F5  41                      inc      ecx                            
  0x00CA40F6  2000                    and      byte ptr [eax], al             
  0x00CA40F8  004520                  add      byte ptr [ebp + 0x20], al      
  0x00CA40FB  0012                    add      byte ptr [edx], dl             
  0x00CA40FD  48                      dec      eax                            
  0x00CA40FE  0400                    add      al, 0                          
  0x00CA4100  16                      push     ss                             
  0x00CA4101  4c                      dec      esp                            
  0x00CA4102  0400                    add      al, 0                          
  0x00CA4104  0002                    add      byte ptr [edx], al             
  0x00CA4106  3800                    cmp      byte ptr [eax], al             
  0x00CA4108  00f4                    add      ah, dh                         
  0x00CA410A  7200                    jb       0xca410c                       
  0x00CA410E  ff00                    inc      dword ptr [eax]                
  0x00CA4110  0002                    add      byte ptr [edx], al             
  0x00CA4112  3c00                    cmp      al, 0                          
  0x00CA4114  00f4                    add      ah, dh                         
  0x00CA4116  7600                    jbe      0xca4118                       
  0x00CA411A  ff00                    inc      dword ptr [eax]                
  0x00CA411C  0088d000d4ca            add      byte ptr [eax - 0x352bff30], cl 
  0x00CA4122  d500                    aad      0                              
  0x00CA4124  f30020                  add      byte ptr [eax], ah             
  0x00CA4127  00c8                    add      al, cl                         
  0x00CA4129  59                      pop      ecx                            
  0x00CA412A  56                      push     esi                            
  0x00CA412B  00eb                    add      bl, ch                         
  0x00CA412D  88d0                    mov      al, dl                         
  0x00CA412F  00903f060005            add      byte ptr [eax + 0x500063f], dl 
  0x00CA4135  0000                    add      byte ptr [eax], al             
  0x00CA4137  00d4                    add      ah, dl                         
  0x00CA4139  cad500                  retf     0xd5                           
  0x00CA413C  f35d                    pop      ebp                            
  0x00CA413E  57                      push     edi                            
  0x00CA413F  00c8                    add      al, cl                         
  0x00CA4141  59                      pop      ecx                            
  0x00CA4142  56                      push     esi                            
  0x00CA4143  00eb                    add      bl, ch                         
  0x00CA4145  88d0                    mov      al, dl                         
  0x00CA4147  0000                    add      byte ptr [eax], al             
  0x00CA4149  5d                      pop      ebp                            
  0x00CA414A  57                      push     edi                            
  0x00CA414B  0000                    add      byte ptr [eax], al             
  0x00CA414D  40                      inc      eax                            
  0x00CA414E  2000                    and      byte ptr [eax], al             
  0x00CA4150  00442000                add      byte ptr [eax], al             
  0x00CA4154  007f38                  add      byte ptr [edi + 0x38], bh      
  0x00CA4157  0000                    add      byte ptr [eax], al             
  0x00CA4159  1a23                    sbb      ah, byte ptr [ebx]             
  0x00CA415B  0000                    add      byte ptr [eax], al             
  0x00CA415D  1c23                    sbb      al, 0x23                       
  0x00CA415F  0000                    add      byte ptr [eax], al             
  0x00CA4161  1e                      push     ds                             
  0x00CA4162  2300                    and      eax, dword ptr [eax]           
  0x00CA4164  004120                  add      byte ptr [ecx + 0x20], al      
  0x00CA4167  0000                    add      byte ptr [eax], al             
  0x00CA4169  45                      inc      ebp                            
  0x00CA416A  2000                    and      byte ptr [eax], al             
  0x00CA416C  004020                  add      byte ptr [eax + 0x20], al      
  0x00CA416F  0000                    add      byte ptr [eax], al             
  0x00CA4171  4a                      dec      edx                            
  0x00CA4172  2000                    and      byte ptr [eax], al             
  0x00CA4174  00442000                add      byte ptr [eax], al             
  0x00CA4178  004e20                  add      byte ptr [esi + 0x20], cl      
  0x00CA417B  0000                    add      byte ptr [eax], al             
  0x00CA417D  0238                    add      bh, byte ptr [eax]             
  0x00CA417F  0000                    add      byte ptr [eax], al             
  0x00CA4181  f4                      hlt                                     
  0x00CA4182  7200                    jb       0xca4184                       
  0x00CA4186  ff00                    inc      dword ptr [eax]                
  0x00CA4188  0002                    add      byte ptr [edx], al             
  0x00CA418A  3c00                    cmp      al, 0                          
  0x00CA418C  00f4                    add      ah, dh                         
  0x00CA418E  7600                    jbe      0xca4190                       
  0x00CA4192  ff00                    inc      dword ptr [eax]                
  0x00CA4194  0088d000d4ca            add      byte ptr [eax - 0x352bff30], cl 
  0x00CA419A  d500                    aad      0                              
  0x00CA419C  f30020                  add      byte ptr [eax], ah             
  0x00CA419F  00c8                    add      al, cl                         
  0x00CA41A1  7956                    jns      0xca41f9                       
  0x00CA41A3  00eb                    add      bl, ch                         
  0x00CA41A5  88d0                    mov      al, dl                         
  0x00CA41A7  00903f060005            add      byte ptr [eax + 0x500063f], dl 
  0x00CA41AD  0000                    add      byte ptr [eax], al             
  0x00CA41AF  00d4                    add      ah, dl                         
  0x00CA41B1  cad500                  retf     0xd5                           
  0x00CA41B4  f37d5f                  jge      0xca4216                       
  0x00CA41B7  00c8                    add      al, cl                         
  0x00CA41B9  7956                    jns      0xca4211                       
  0x00CA41BB  00eb                    add      bl, ch                         
  0x00CA41BD  88d0                    mov      al, dl                         
  0x00CA41BF  0000                    add      byte ptr [eax], al             
  0x00CA41C1  7d5f                    jge      0xca4222                       
  0x00CA41C3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA41C6  0000                    add      byte ptr [eax], al             
  0x00CA41C8  00c0                    add      al, al                         
  0x00CA41CA  f1                      int1                                    
  0x00CA41CB  0000                    add      byte ptr [eax], al             
  0x00CA41CD  da4d00                  fimul    dword ptr [ebp]                
  0x00CA41D0  c8d84e00                enter    0x4ed8, 0                      
  0x00CA41D4  eb00                    jmp      0xca41d6                       
                                        ; XREF: 0x00CA41D4 (jump)
  0x00CA41D6  2000                    and      byte ptr [eax], al             
  0x00CA41D8  b064                    mov      al, 0x64                       
  0x00CA41DA  5f                      pop      edi                            
  0x00CA41DB  0010                    add      byte ptr [eax], dl             
  0x00CA41DD  da06                    fiadd    dword ptr [esi]                
  0x00CA41DF  0006                    add      byte ptr [esi], al             
  0x00CA41E1  0000                    add      byte ptr [eax], al             
  0x00CA41E3  00a7c0f10000            add      byte ptr [edi + 0xf1c0], ah    
  0x00CA41E9  da4d00                  fimul    dword ptr [ebp]                
  0x00CA41EC  c8d84e00                enter    0x4ed8, 0                      
  0x00CA41F0  eb5c                    jmp      0xca424e                       
  0x00CA41F2  56                      push     esi                            
  0x00CA41F3  00b0645f00a7            add      byte ptr [eax - 0x58ffa09c], dh 
                                        ; XREF: 0x00CA41A1 (cond_jump)
  0x00CA41F9  0020                    add      byte ptr [eax], ah             
  0x00CA41FB  0000                    add      byte ptr [eax], al             
  0x00CA41FD  5c                      pop      esp                            
  0x00CA41FE  56                      push     esi                            
  0x00CA41FF  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA4202  0000                    add      byte ptr [eax], al             
  0x00CA4204  00c0                    add      al, al                         
  0x00CA4206  f1                      int1                                    
  0x00CA4207  0000                    add      byte ptr [eax], al             
  0x00CA4209  da4d00                  fimul    dword ptr [ebp]                
  0x00CA420C  c8e14e00                enter    0x4ee1, 0                      
  0x00CA4210  eb00                    jmp      0xca4212                       
                                        ; XREF: 0x00CA4210 (jump)
  0x00CA4212  2000                    and      byte ptr [eax], al             
  0x00CA4214  b0d8                    mov      al, 0xd8                       
                                        ; XREF: 0x00CA41B4 (cond_jump)
  0x00CA4216  4e                      dec      esi                            
  0x00CA4217  00a7d94400c8            add      byte ptr [edi - 0x37ffbb27], ah 
  0x00CA421D  645f                    pop      edi                            
  0x00CA421F  00eb                    add      bl, ch                         
  0x00CA4221  5c                      pop      esp                            
                                        ; XREF: 0x00CA41C1 (cond_jump)
  0x00CA4222  56                      push     esi                            
  0x00CA4223  00b0655f0010            add      byte ptr [eax + 0x10005f65], dh 
  0x00CA4229  da06                    fiadd    dword ptr [esi]                
  0x00CA422B  000a                    add      byte ptr [edx], cl             
  0x00CA422D  0000                    add      byte ptr [eax], al             
  0x00CA422F  00a7c0f10000            add      byte ptr [edi + 0xf1c0], ah    
  0x00CA4235  da4d00                  fimul    dword ptr [ebp]                
  0x00CA4238  c8e14e00                enter    0x4ee1, 0                      
  0x00CA423C  eb5d                    jmp      0xca429b                       
  0x00CA423E  56                      push     esi                            
  0x00CA423F  00b0d84e00a7            add      byte ptr [eax - 0x58ffb128], dh 
  0x00CA4245  d94400c8                fld      dword ptr [eax + eax - 0x38]   
  0x00CA4249  645f                    pop      edi                            
  0x00CA424B  00eb                    add      bl, ch                         
  0x00CA424D  5c                      pop      esp                            
                                        ; XREF: 0x00CA41F0 (jump)
  0x00CA424E  56                      push     esi                            
  0x00CA424F  00b0655f00a7            add      byte ptr [eax - 0x58ffa09b], dh 
  0x00CA4255  0020                    add      byte ptr [eax], ah             
  0x00CA4257  0000                    add      byte ptr [eax], al             
  0x00CA4259  5d                      pop      ebp                            
  0x00CA425A  56                      push     esi                            
  0x00CA425B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA425E  0000                    add      byte ptr [eax], al             
  0x00CA4260  00c0                    add      al, al                         
  0x00CA4262  f1                      int1                                    
  0x00CA4263  0000                    add      byte ptr [eax], al             
  0x00CA4265  da4d00                  fimul    dword ptr [ebp]                
  0x00CA4268  a8c8                    test     al, 0xc8                       
  0x00CA426A  4e                      dec      esi                            
  0x00CA426B  00bb002000e0            add      byte ptr [ebx - 0x1fffe000], bh 
  0x00CA4271  4d                      dec      ebp                            
  0x00CA4272  57                      push     edi                            
  0x00CA4273  0010                    add      byte ptr [eax], dl             
  0x00CA4275  da06                    fiadd    dword ptr [esi]                
  0x00CA4277  0006                    add      byte ptr [esi], al             
  0x00CA4279  0000                    add      byte ptr [eax], al             
  0x00CA427B  00c7                    add      bh, al                         
  0x00CA427D  c0f100                  sal      cl, 0                          
  0x00CA4280  00da                    add      dl, bl                         
  0x00CA4282  4d                      dec      ebp                            
  0x00CA4283  00a8c84e00bb            add      byte ptr [eax - 0x44ffb138], ch 
  0x00CA4289  4c                      dec      esp                            
  0x00CA428A  56                      push     esi                            
  0x00CA428B  00e0                    add      al, ah                         
  0x00CA428D  4d                      dec      ebp                            
  0x00CA428E  57                      push     edi                            
  0x00CA428F  00c7                    add      bh, al                         
  0x00CA4291  0020                    add      byte ptr [eax], ah             
  0x00CA4293  0000                    add      byte ptr [eax], al             
  0x00CA4295  4c                      dec      esp                            
  0x00CA4296  56                      push     esi                            
  0x00CA4297  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA429A  0000                    add      byte ptr [eax], al             
  0x00CA429C  00c1                    add      cl, al                         
  0x00CA429E  f1                      int1                                    
  0x00CA429F  0000                    add      byte ptr [eax], al             
  0x00CA42A1  da4d00                  fimul    dword ptr [ebp]                
  0x00CA42A4  a8e1                    test     al, 0xe1                       
  0x00CA42A6  4e                      dec      esi                            
  0x00CA42A7  00bbe04e00e0            add      byte ptr [ebx - 0x1fffb120], bh 
  0x00CA42AD  c84400c7                enter    0x44, -0x39                    
  0x00CA42B1  55                      push     ebp                            
  0x00CA42B2  57                      push     edi                            
  0x00CA42B3  00b8e14e00ab            add      byte ptr [eax - 0x54ffb11f], bh 
  0x00CA42B9  5c                      pop      esp                            
  0x00CA42BA  56                      push     esi                            
  0x00CA42BB  00e0                    add      al, ah                         
  0x00CA42BD  c9                      leave                                   
  0x00CA42BE  44                      inc      esp                            
  0x00CA42BF  00c7                    add      bh, al                         
  0x00CA42C1  4d                      dec      ebp                            
  0x00CA42C2  57                      push     edi                            
  0x00CA42C3  0010                    add      byte ptr [eax], dl             
  0x00CA42C5  da06                    fiadd    dword ptr [esi]                
  0x00CA42C7  000b                    add      byte ptr [ebx], cl             
  0x00CA42C9  0000                    add      byte ptr [eax], al             
  0x00CA42CB  0000                    add      byte ptr [eax], al             
  0x00CA42CD  c1f100                  sal      ecx, 0                         
  0x00CA42D0  00da                    add      dl, bl                         
  0x00CA42D2  4d                      dec      ebp                            
  0x00CA42D3  00a8e14e00bb            add      byte ptr [eax - 0x44ffb11f], ch 
  0x00CA42D9  0cc8                    or       al, 0xc8                       
  0x00CA42DB  00e0                    add      al, ah                         
  0x00CA42DD  c84400c7                enter    0x44, -0x39                    
  0x00CA42E1  55                      push     ebp                            
  0x00CA42E2  57                      push     edi                            
  0x00CA42E3  00b8e14e00ab            add      byte ptr [eax - 0x54ffb11f], bh 
  0x00CA42E9  5c                      pop      esp                            
  0x00CA42EA  56                      push     esi                            
  0x00CA42EB  00e0                    add      al, ah                         
  0x00CA42ED  c9                      leave                                   
  0x00CA42EE  44                      inc      esp                            
  0x00CA42EF  00c7                    add      bh, al                         
  0x00CA42F1  4d                      dec      ebp                            
  0x00CA42F2  57                      push     edi                            
  0x00CA42F3  0000                    add      byte ptr [eax], al             
  0x00CA42F5  4c                      dec      esp                            
  0x00CA42F6  56                      push     esi                            
  0x00CA42F7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA42FA  0000                    add      byte ptr [eax], al             
  0x00CA42FC  00d8                    add      al, bl                         
  0x00CA42FE  56                      push     esi                            
  0x00CA42FF  0010                    add      byte ptr [eax], dl             
  0x00CA4301  d906                    fld      dword ptr [esi]                
  0x00CA4303  0007                    add      byte ptr [edi], al             
  0x00CA4305  0000                    add      byte ptr [eax], al             
  0x00CA4307  0001                    add      byte ptr [ecx], al             
  0x00CA4309  1e                      push     ds                             
  0x00CA430A  0c00                    or       al, 0                          
  0x00CA430C  3e0020                  add      byte ptr ds:[eax], ah          
  0x00CA430F  0003                    add      byte ptr [ebx], al             
  0x00CA4311  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA4312  2300                    and      eax, dword ptr [eax]           
  0x00CA4314  48                      dec      eax                            
  0x00CA4315  a0020000d8              mov      al, byte ptr [0xd8000002]      
  0x00CA431A  56                      push     esi                            
  0x00CA431B  0000                    add      byte ptr [eax], al             
  0x00CA431D  59                      pop      ecx                            
  0x00CA431E  57                      push     edi                            
  0x00CA431F  0000                    add      byte ptr [eax], al             
  0x00CA4321  50                      push     eax                            
  0x00CA4322  2000                    and      byte ptr [eax], al             
  0x00CA4324  0c00                    or       al, 0                          
  0x00CA4326  0000                    add      byte ptr [eax], al             
  0x00CA4328  00f4                    add      ah, dh                         
  0x00CA432A  46                      inc      esi                            
  0x00CA432B  0001                    add      byte ptr [ecx], al             
  0x00CA432D  0000                    add      byte ptr [eax], al             
  0x00CA432F  0000                    add      byte ptr [eax], al             
  0x00CA4331  ae                      scasb    al, byte ptr es:[edi]          
  0x00CA4332  2300                    and      eax, dword ptr [eax]           
  0x00CA4334  55                      push     ebp                            
  0x00CA4335  3522000da4              xor      eax, 0xa40d0022                
  0x00CA433A  050000b422              add      eax, 0x22b40000                
  0x00CA433F  0010                    add      byte ptr [eax], dl             
  0x00CA4341  dc06                    fadd     qword ptr [esi]                
  0x00CA4343  0009                    add      byte ptr [ecx], cl             
  0x00CA4345  0000                    add      byte ptr [eax], al             
  0x00CA4347  0000                    add      byte ptr [eax], al             
  0x00CA4349  f4                      hlt                                     
  0x00CA434A  56                      push     esi                            
  0x00CA434B  00ff                    add      bh, bh                         
  0x00CA434E  7f00                    jg       0xca4350                       
                                        ; XREF: 0x00CA434E (cond_jump)
  0x00CA4350  10dd                    adc      ch, bl                         
  0x00CA4352  06                      push     es                             
  0x00CA4353  000400                  add      byte ptr [eax + eax], al       
  0x00CA4356  0000                    add      byte ptr [eax], al             
  0x00CA4358  00dc                    add      ah, bl                         
  0x00CA435A  44                      inc      esp                            
  0x00CA435B  004500                  add      byte ptr [ebp], al             
  0x00CA435E  2000                    and      byte ptr [eax], al             
  0x00CA4360  40                      inc      eax                            
  0x00CA4361  7002                    jo       0xca4365                       
  0x00CA4363  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA4361 (cond_jump)
  0x00CA4365  4d                      dec      ebp                            
  0x00CA4366  54                      push     esp                            
  0x00CA4367  0000                    add      byte ptr [eax], al             
  0x00CA4369  352200004f              xor      eax, 0x4f000022                
  0x00CA436E  2300                    and      eax, dword ptr [eax]           
  0x00CA4370  0be2                    or       esp, edx                       
  0x00CA4372  56                      push     esi                            
  0x00CA4373  0006                    add      byte ptr [esi], al             
  0x00CA4375  2405                    and      al, 5                          
  0x00CA4377  0000                    add      byte ptr [eax], al             
  0x00CA4379  f4                      hlt                                     
  0x00CA437A  44                      inc      esp                            
  0x00CA437B  000f                    add      byte ptr [edi], cl             
  0x00CA437D  0000                    add      byte ptr [eax], al             
  0x00CA437F  004500                  add      byte ptr [ebp], al             
  0x00CA4382  2000                    and      byte ptr [eax], al             
  0x00CA4384  40                      inc      eax                            
  0x00CA4385  7002                    jo       0xca4389                       
  0x00CA4387  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA4385 (cond_jump)
  0x00CA4389  62540000                bound    edx, qword ptr [eax + eax]     
  0x00CA438D  8521                    test     dword ptr [ecx], esp           
  0x00CA438F  0010                    add      byte ptr [eax], dl             
  0x00CA4391  dc06                    fadd     qword ptr [esi]                
  0x00CA4393  000400                  add      byte ptr [eax + eax], al       
  0x00CA4396  0000                    add      byte ptr [eax], al             
  0x00CA4398  00e5                    add      ch, ah                         
  0x00CA439A  56                      push     esi                            
  0x00CA439B  00648521                add      byte ptr [ebp + eax*4 + 0x21], ah 
  0x00CA439F  0000                    add      byte ptr [eax], al             
  0x00CA43A1  4d                      dec      ebp                            
  0x00CA43A2  54                      push     esp                            
  0x00CA43A3  0000                    add      byte ptr [eax], al             
  0x00CA43A5  3522005987              xor      eax, 0x87590022                
  0x00CA43AA  2300                    and      eax, dword ptr [eax]           
  0x00CA43AC  00f4                    add      ah, dh                         
  0x00CA43AE  45                      inc      ebp                            
  0x00CA43AF  0002                    add      byte ptr [edx], al             
  0x00CA43B1  0000                    add      byte ptr [eax], al             
  0x00CA43B3  0000                    add      byte ptr [eax], al             
  0x00CA43B5  e556                    in       eax, 0x56                      
  0x00CA43B7  0065f4                  add      byte ptr [ebp - 0xc], ah       
  0x00CA43BA  45                      inc      ebp                            
  0x00CA43BB  00fe                    add      dh, bh                         
  0x00CA43BE  ff00                    inc      dword ptr [eax]                
  0x00CA43C0  17                      pop      ss                             
  0x00CA43C1  7405                    je       0xca43c8                       
  0x00CA43C3  0065f4                  add      byte ptr [ebp - 0xc], ah       
  0x00CA43C6  45                      inc      ebp                            
  0x00CA43C7  0002                    add      byte ptr [edx], al             
  0x00CA43C9  0000                    add      byte ptr [eax], al             
  0x00CA43CB  0011                    add      byte ptr [ecx], dl             
  0x00CA43CD  94                      xchg     esp, eax                       
  0x00CA43CE  0500584d20              add      eax, 0x204d5800                
  0x00CA43D3  007d00                  add      byte ptr [ebp], bh             
  0x00CA43D6  2000                    and      byte ptr [eax], al             
  0x00CA43D8  4d                      dec      ebp                            
  0x00CA43D9  7405                    je       0xca43e0                       
  0x00CA43DB  00d6                    add      dh, dl                         
  0x00CA43DD  97                      xchg     edi, eax                       
  0x00CA43DE  050000e556              add      eax, 0x56e50000                
  0x00CA43E3  0065f4                  add      byte ptr [ebp - 0xc], ah       
  0x00CA43E6  45                      inc      ebp                            
  0x00CA43E7  00fe                    add      dh, bh                         
  0x00CA43EA  ff00                    inc      dword ptr [eax]                
  0x00CA43EC  13740500                adc      esi, dword ptr [ebp + eax]     
  0x00CA43F0  65f4                    hlt                                     
  0x00CA43F2  45                      inc      ebp                            
  0x00CA43F3  0002                    add      byte ptr [edx], al             
  0x00CA43F5  0000                    add      byte ptr [eax], al             
  0x00CA43F7  0006                    add      byte ptr [esi], al             
  0x00CA43F9  94                      xchg     esp, eax                       
  0x00CA43FA  0500584d20              add      eax, 0x204d5800                
  0x00CA43FF  007d00                  add      byte ptr [ebp], bh             
  0x00CA4402  2000                    and      byte ptr [eax], al             
  0x00CA4404  42                      inc      edx                            
  0x00CA4405  7405                    je       0xca440c                       
  0x00CA4407  00cb                    add      bl, cl                         
  0x00CA4409  97                      xchg     edi, eax                       
  0x00CA440A  0500d5a705              add      eax, 0x5a7d500                 
  0x00CA440F  005c4520                add      byte ptr [ebp + eax*2 + 0x20], bl 
  0x00CA4413  000da4050000            add      byte ptr [0x5a4], cl           
  0x00CA4419  e556                    in       eax, 0x56                      
  0x00CA441B  00540020                add      byte ptr [eax + eax + 0x20], dl 
  0x00CA441F  0000                    add      byte ptr [eax], al             
  0x00CA4421  4d                      dec      ebp                            
  0x00CA4422  54                      push     esp                            
  0x00CA4423  0000                    add      byte ptr [eax], al             
  0x00CA4425  e556                    in       eax, 0x56                      
  0x00CA4427  0050f4                  add      byte ptr [eax - 0xc], dl       
  0x00CA442A  45                      inc      ebp                            
  0x00CA442B  0002                    add      byte ptr [edx], al             
  0x00CA442D  0000                    add      byte ptr [eax], al             
  0x00CA442F  0000                    add      byte ptr [eax], al             
  0x00CA4431  45                      inc      ebp                            
  0x00CA4432  54                      push     esp                            
  0x00CA4433  00c0                    add      al, al                         
  0x00CA4435  0f05                    syscall                                 
  0x00CA4437  0054f445                add      byte ptr [esp + esi*8 + 0x45], dl 
  0x00CA443B  0002                    add      byte ptr [edx], al             
  0x00CA443D  0000                    add      byte ptr [eax], al             
  0x00CA443F  0000                    add      byte ptr [eax], al             
  0x00CA4441  6554                    push     esp                            
  0x00CA4443  00c7                    add      bh, al                         
  0x00CA4445  0f05                    syscall                                 
  0x00CA4447  0000                    add      byte ptr [eax], al             
  0x00CA4449  4e                      dec      esi                            
  0x00CA444A  2300                    and      eax, dword ptr [eax]           
  0x00CA444C  034d20                  add      ecx, dword ptr [ebp + 0x20]    
  0x00CA444F  0007                    add      byte ptr [edi], al             
  0x00CA4451  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA4452  050000e256              add      eax, 0x56e20000                
  0x00CA4457  00540020                add      byte ptr [eax + eax + 0x20], dl 
  0x00CA445B  0000                    add      byte ptr [eax], al             
  0x00CA445D  62540000                bound    edx, qword ptr [eax + eax]     
  0x00CA4461  e556                    in       eax, 0x56                      
  0x00CA4463  005000                  add      byte ptr [eax], dl             
  0x00CA4466  2000                    and      byte ptr [eax], al             
  0x00CA4468  006554                  add      byte ptr [ebp + 0x54], ah      
  0x00CA446B  0000                    add      byte ptr [eax], al             
  0x00CA446D  e256                    loop     0xca44c5                       
  0x00CA446F  00540020                add      byte ptr [eax + eax + 0x20], dl 
  0x00CA4473  0000                    add      byte ptr [eax], al             
  0x00CA4475  62540000                bound    edx, qword ptr [eax + eax]     
  0x00CA4479  e556                    in       eax, 0x56                      
  0x00CA447B  0050f4                  add      byte ptr [eax - 0xc], dl       
  0x00CA447E  45                      inc      ebp                            
  0x00CA447F  0002                    add      byte ptr [edx], al             
  0x00CA4481  0000                    add      byte ptr [eax], al             
  0x00CA4483  005865                  add      byte ptr [eax + 0x65], bl      
  0x00CA4486  54                      push     esp                            
  0x00CA4487  008b0f050000            add      byte ptr [ebx + 0x50f], cl     
  0x00CA448D  35220000e2              xor      eax, 0xe2000022                
  0x00CA4492  45                      inc      ebp                            
  0x00CA4493  0010                    add      byte ptr [eax], dl             
  0x00CA4495  dc06                    fadd     qword ptr [esi]                
  0x00CA4497  000500000000            add      byte ptr [0], al               
  0x00CA449D  e556                    in       eax, 0x56                      
  0x00CA449F  006000                  add      byte ptr [eax], ah             
  0x00CA44A2  2000                    and      byte ptr [eax], al             
  0x00CA44A4  00852100004d            add      byte ptr [ebp + 0x4d000021], al 
  0x00CA44AA  54                      push     esp                            
  0x00CA44AB  0000                    add      byte ptr [eax], al             
  0x00CA44AD  ae                      scasb    al, byte ptr es:[edi]          
  0x00CA44AE  2300                    and      eax, dword ptr [eax]           
  0x00CA44B0  55                      push     ebp                            
  0x00CA44B1  35220009a4              xor      eax, 0xa4090022                
  0x00CA44B6  050000b422              add      eax, 0x22b40000                
  0x00CA44BB  0010                    add      byte ptr [eax], dl             
  0x00CA44BD  dc06                    fadd     qword ptr [esi]                
  0x00CA44BF  0006                    add      byte ptr [esi], al             
  0x00CA44C1  0000                    add      byte ptr [eax], al             
  0x00CA44C3  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA446D (cond_jump)
  0x00CA44C5  cd44                    int      0x44                           
  0x00CA44C7  0010                    add      byte ptr [eax], dl             
  0x00CA44C9  dd06                    fld      qword ptr [esi]                
  0x00CA44CB  0002                    add      byte ptr [edx], al             
  0x00CA44CD  0000                    add      byte ptr [eax], al             
  0x00CA44CF  0000                    add      byte ptr [eax], al             
  0x00CA44D1  5c                      pop      esp                            
  0x00CA44D2  44                      inc      esp                            
  0x00CA44D3  0000                    add      byte ptr [eax], al             
  0x00CA44D5  0000                    add      byte ptr [eax], al             
  0x00CA44D7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA44DA  0000                    add      byte ptr [eax], al             
  0x00CA44DC  00f0                    add      al, dh                         
  0x00CA44DE  44                      inc      esp                            
  0x00CA44DF  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA44E2  0000                    add      byte ptr [eax], al             
  0x00CA44E4  00f0                    add      al, dh                         
  0x00CA44E6  56                      push     esi                            
  0x00CA44E7  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA44ED  0020                    add      byte ptr [eax], ah             
  0x00CA44EF  0013                    add      byte ptr [ebx], dl             
  0x00CA44F1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA44F2  0500009620              add      eax, 0x20960000                
  0x00CA44F7  0000                    add      byte ptr [eax], al             
  0x00CA44F9  f4                      hlt                                     
  0x00CA44FA  60                      pushal                                  
  0x00CA44FB  008001000000            add      byte ptr [eax + 1], al         
  0x00CA4501  f4                      hlt                                     
  0x00CA4502  61                      popal                                   
  0x00CA4503  004102                  add      byte ptr [ecx + 2], al         
  0x00CA4506  0000                    add      byte ptr [eax], al             
  0x00CA4508  00f4                    add      ah, dh                         
  0x00CA450A  56                      push     esi                            
  0x00CA450B  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA4511  c422                    les      esp, ptr [edx]                 
  0x00CA4513  004000                  add      byte ptr [eax], al             
  0x00CA4516  2000                    and      byte ptr [eax], al             
  0x00CA4518  0092210000e2            add      byte ptr [edx - 0x1dffffdf], dl 
  0x00CA451E  7100                    jno      0xca4520                       
                                        ; XREF: 0x00CA451E (cond_jump)
  0x00CA4520  10d9                    adc      cl, bl                         
  0x00CA4522  06                      push     es                             
  0x00CA4523  000500000000            add      byte ptr [0], al               
  0x00CA4529  d9440000                fld      dword ptr [eax + eax]          
  0x00CA452D  e056                    loopne   0xca4585                       
  0x00CA452F  00481e                  add      byte ptr [eax + 0x1e], cl      
  0x00CA4532  0c00                    or       al, 0                          
  0x00CA4534  005854                  add      byte ptr [eax + 0x54], bl      
  0x00CA4537  0010                    add      byte ptr [eax], dl             
  0x00CA4539  0c05                    or       al, 5                          
  0x00CA453B  0000                    add      byte ptr [eax], al             
  0x00CA453E  56                      push     esi                            
  0x00CA453F  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA4542  0000                    add      byte ptr [eax], al             
  0x00CA4544  0300                    add      eax, dword ptr [eax]           
  0x00CA4546  2000                    and      byte ptr [eax], al             
  0x00CA4548  0ca4                    or       al, 0xa4                       
  0x00CA454A  050000f460              add      eax, 0x60f40000                
  0x00CA454F  003502000000            add      byte ptr [2], dh               
  0x00CA4555  f4                      hlt                                     
  0x00CA4556  61                      popal                                   
  0x00CA4557  00f6                    add      dh, dh                         
  0x00CA4559  0200                    add      al, byte ptr [eax]             
  0x00CA455B  0000                    add      byte ptr [eax], al             
  0x00CA455D  07                      pop      es                             
  0x00CA455E  3900                    cmp      dword ptr [eax], eax           
  0x00CA4560  10d9                    adc      cl, bl                         
  0x00CA4562  06                      push     es                             
  0x00CA4563  000500000000            add      byte ptr [0], al               
  0x00CA4569  d9440000                fld      dword ptr [eax + eax]          
  0x00CA456D  e056                    loopne   0xca45c5                       
  0x00CA456F  00481e                  add      byte ptr [eax + 0x1e], cl      
  0x00CA4572  0c00                    or       al, 0                          
  0x00CA4574  005854                  add      byte ptr [eax + 0x54], bl      
  0x00CA4577  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA457A  0000                    add      byte ptr [eax], al             
  0x00CA457C  20f4                    and      ah, dh                         
  0x00CA457E  0500ffffff              add      eax, 0xffffff00                
  0x00CA4583  00a0610400a0            add      byte ptr [eax - 0x5ffffb9f], ah 
  0x00CA4589  620400                  bound    eax, qword ptr [eax + eax]     
  0x00CA458C  a0640400a0              mov      al, byte ptr [0xa0000464]      
  0x00CA4591  650400                  add      al, 0                          
  0x00CA4594  a0660400b8              mov      al, byte ptr [0xb8000466]      
  0x00CA4599  f30000                  add      byte ptr [eax], al             
  0x00CA459C  00f4                    add      ah, dh                         
  0x00CA459E  44                      inc      esp                            
  0x00CA459F  0000                    add      byte ptr [eax], al             
  0x00CA45A1  0000                    add      byte ptr [eax], al             
  0x00CA45A3  004d00                  add      byte ptr [ebp], cl             
  0x00CA45A6  2000                    and      byte ptr [eax], al             
  0x00CA45A8  0ca4                    or       al, 0xa4                       
  0x00CA45AA  050000f444              add      eax, 0x44f40000                
  0x00CA45AF  0010                    add      byte ptr [eax], dl             
  0x00CA45B1  0000                    add      byte ptr [eax], al             
  0x00CA45B3  004d00                  add      byte ptr [ebp], cl             
  0x00CA45B6  2000                    and      byte ptr [eax], al             
  0x00CA45B8  4a                      dec      edx                            
  0x00CA45B9  100d00110000            adc      byte ptr [0x1100], cl          
  0x00CA45BF  0000                    add      byte ptr [eax], al             
  0x00CA45C1  0030                    add      byte ptr [eax], dh             
  0x00CA45C3  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA456D (cond_jump)
  0x00CA45C5  f4                      hlt                                     
  0x00CA45C6  56                      push     esi                            
  0x00CA45C7  0000                    add      byte ptr [eax], al             
  0x00CA45C9  0000                    add      byte ptr [eax], al             
  0x00CA45CB  0000                    add      byte ptr [eax], al             
  0x00CA45CD  f4                      hlt                                     
  0x00CA45CE  57                      push     edi                            
  0x00CA45CF  00ff                    add      bh, bh                         
  0x00CA45D2  ff00                    inc      dword ptr [eax]                
  0x00CA45D4  0c00                    or       al, 0                          
  0x00CA45D6  0000                    add      byte ptr [eax], al             
  0x00CA45D8  1300                    adc      eax, dword ptr [eax]           
  0x00CA45DA  2000                    and      byte ptr [eax], al             
  0x00CA45DC  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA45DF  0012                    add      byte ptr [edx], dl             
  0x00CA45E1  0900                    or       dword ptr [eax], eax           
  0x00CA45E3  0000                    add      byte ptr [eax], al             
  0x00CA45E5  0030                    add      byte ptr [eax], dh             
  0x00CA45E7  0000                    add      byte ptr [eax], al             
  0x00CA45E9  f4                      hlt                                     
  0x00CA45EA  56                      push     esi                            
  0x00CA45EB  0000                    add      byte ptr [eax], al             
  0x00CA45ED  0000                    add      byte ptr [eax], al             
  0x00CA45EF  0000                    add      byte ptr [eax], al             
  0x00CA45F1  f4                      hlt                                     
  0x00CA45F2  57                      push     edi                            
  0x00CA45F3  0008                    add      byte ptr [eax], cl             
  0x00CA45F5  06                      push     es                             
  0x00CA45F6  0000                    add      byte ptr [eax], al             
  0x00CA45F8  0c00                    or       al, 0                          
  0x00CA45FA  0000                    add      byte ptr [eax], al             
  0x00CA45FC  5c                      pop      esp                            
  0x00CA45FD  08050080100d            or       byte ptr [0xd108000], al       
  0x00CA4603  009600000000            add      byte ptr [esi], dl             
  0x00CA460A  56                      push     esi                            
  0x00CA460B  00960b000003            add      byte ptr [esi + 0x300000b], dl 
  0x00CA4611  0020                    add      byte ptr [eax], ah             
  0x00CA4613  005374                  add      byte ptr [ebx + 0x74], dl      
  0x00CA4616  050080100d              add      eax, 0xd108000                 
  0x00CA461B  00c4                    add      ah, al                         
  0x00CA461D  0000                    add      byte ptr [eax], al             
  0x00CA461F  0000                    add      byte ptr [eax], al             
  0x00CA4622  56                      push     esi                            
  0x00CA4623  00960b000003            add      byte ptr [esi + 0x300000b], dl 
  0x00CA4629  0020                    add      byte ptr [eax], ah             
  0x00CA462B  004d74                  add      byte ptr [ebp + 0x74], cl      
  0x00CA462E  050080100d              add      eax, 0xd108000                 
  0x00CA4633  006e01                  add      byte ptr [esi + 1], ch         
  0x00CA4636  0000                    add      byte ptr [eax], al             
  0x00CA4638  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA463B  00b101000080            add      byte ptr [ecx - 0x7fffffff], dh 
  0x00CA4641  100d00f40100            adc      byte ptr [0x1f400], cl         
  0x00CA4647  0000                    add      byte ptr [eax], al             
  0x00CA4649  002400                  add      byte ptr [eax + eax], ah       
  0x00CA464C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA464F  002b                    add      byte ptr [ebx], ch             
  0x00CA4651  0900                    or       dword ptr [eax], eax           
  0x00CA4653  0000                    add      byte ptr [eax], al             
  0x00CA4656  56                      push     esi                            
  0x00CA4657  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA465A  0000                    add      byte ptr [eax], al             
  0x00CA465C  00f0                    add      al, dh                         
  0x00CA465E  44                      inc      esp                            
  0x00CA465F  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA4665  0020                    add      byte ptr [eax], ah             
  0x00CA4667  0009                    add      byte ptr [ecx], cl             
  0x00CA4669  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA466A  050080100d              add      eax, 0xd108000                 
  0x00CA466F  004601                  add      byte ptr [esi + 1], al         
  0x00CA4672  0000                    add      byte ptr [eax], al             
  0x00CA4674  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA4677  001b                    add      byte ptr [ebx], bl             
  0x00CA4679  0200                    add      al, byte ptr [eax]             
  0x00CA467B  0000                    add      byte ptr [eax], al             
  0x00CA467D  7055                    jo       0xca46d4                       
  0x00CA467F  002b                    add      byte ptr [ebx], ch             
  0x00CA4681  0900                    or       dword ptr [eax], eax           
  0x00CA4683  0080100d0003            add      byte ptr [eax + 0x3000d10], al 
  0x00CA4689  0300                    add      eax, dword ptr [eax]           
  0x00CA468B  0080100d001b            add      byte ptr [eax + 0x1b000d10], al 
  0x00CA4691  0300                    add      eax, dword ptr [eax]           
  0x00CA4693  0080100d002a            add      byte ptr [eax + 0x2a000d10], al 
  0x00CA4699  0300                    add      eax, dword ptr [eax]           
  0x00CA469B  0080100d0085            add      byte ptr [eax - 0x7afff2f0], al 
  0x00CA46A1  0300                    add      eax, dword ptr [eax]           
  0x00CA46A3  0080100d0041            add      byte ptr [eax + 0x41000d10], al 
  0x00CA46A9  0300                    add      eax, dword ptr [eax]           
  0x00CA46AB  0000                    add      byte ptr [eax], al             
  0x00CA46AE  56                      push     esi                            
  0x00CA46AF  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA46B2  0000                    add      byte ptr [eax], al             
  0x00CA46B4  0300                    add      eax, dword ptr [eax]           
  0x00CA46B6  2000                    and      byte ptr [eax], al             
  0x00CA46B8  0a10                    or       dl, byte ptr [eax]             
  0x00CA46BA  0d002c0400              or       eax, 0x42c00                   
  0x00CA46BF  0080100d008e            add      byte ptr [eax - 0x71fff2f0], al 
  0x00CA46C5  0300                    add      eax, dword ptr [eax]           
  0x00CA46C7  0080100d00bb            add      byte ptr [eax - 0x44fff2f0], al 
  0x00CA46CD  0300                    add      eax, dword ptr [eax]           
  0x00CA46CF  0080100d00d6            add      byte ptr [eax - 0x29fff2f0], al 
  0x00CA46D5  0300                    add      eax, dword ptr [eax]           
  0x00CA46D7  0080100d0081            add      byte ptr [eax - 0x7efff2f0], al 
  0x00CA46DE  ff00                    inc      dword ptr [eax]                
  0x00CA46E0  1300                    adc      eax, dword ptr [eax]           
  0x00CA46E2  2000                    and      byte ptr [eax], al             
  0x00CA46E4  1b10                    sbb      edx, dword ptr [eax]           
  0x00CA46E6  2100                    and      dword ptr [eax], eax           
  0x00CA46E8  0c00                    or       al, 0                          
  0x00CA46EA  0000                    add      byte ptr [eax], al             
  0x00CA46EC  005820                  add      byte ptr [eax + 0x20], bl      
  0x00CA46EF  0000                    add      byte ptr [eax], al             
  0x00CA46F1  d8440000                fadd     dword ptr [eax + eax]          
  0x00CA46F5  7044                    jo       0xca473b                       
  0x00CA46F7  00420b                  add      byte ptr [edx + 0xb], al       
  0x00CA46FA  0000                    add      byte ptr [eax], al             
  0x00CA46FC  00d8                    add      al, bl                         
  0x00CA46FE  44                      inc      esp                            
  0x00CA46FF  0000                    add      byte ptr [eax], al             
  0x00CA4701  7044                    jo       0xca4747                       
  0x00CA4703  00430b                  add      byte ptr [ebx + 0xb], al       
  0x00CA4706  0000                    add      byte ptr [eax], al             
  0x00CA4708  00d8                    add      al, bl                         
  0x00CA470A  44                      inc      esp                            
  0x00CA470B  0000                    add      byte ptr [eax], al             
  0x00CA470D  7044                    jo       0xca4753                       
  0x00CA470F  00440b00                add      byte ptr [ebx + ecx], al       
  0x00CA4713  0000                    add      byte ptr [eax], al             
  0x00CA4715  d85700                  fcom     dword ptr [edi]                
  0x00CA4718  90                      nop                                     
  0x00CA4719  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA471C  2420                    and      al, 0x20                       
  0x00CA471E  0000                    add      byte ptr [eax], al             
  0x00CA4720  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA4723  007b0b                  add      byte ptr [ebx + 0xb], bh       
  0x00CA4726  0000                    add      byte ptr [eax], al             
  0x00CA4728  90                      nop                                     
  0x00CA4729  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA472C  1b10                    sbb      edx, dword ptr [eax]           
  0x00CA472E  0000                    add      byte ptr [eax], al             
  0x00CA4730  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA4733  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA4736  0000                    add      byte ptr [eax], al             
  0x00CA4738  90                      nop                                     
  0x00CA4739  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA473C  1830                    sbb      byte ptr [eax], dh             
  0x00CA473E  0000                    add      byte ptr [eax], al             
  0x00CA4740  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA4743  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA4746  0000                    add      byte ptr [eax], al             
  0x00CA4748  00d8                    add      al, bl                         
  0x00CA474A  44                      inc      esp                            
  0x00CA474B  0000                    add      byte ptr [eax], al             
  0x00CA474D  7044                    jo       0xca4793                       
  0x00CA474F  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA4752  0000                    add      byte ptr [eax], al             
  0x00CA4754  00d8                    add      al, bl                         
  0x00CA4756  44                      inc      esp                            
  0x00CA4757  0000                    add      byte ptr [eax], al             
  0x00CA4759  7044                    jo       0xca479f                       
  0x00CA475B  00460b                  add      byte ptr [esi + 0xb], al       
  0x00CA475E  0000                    add      byte ptr [eax], al             
  0x00CA4760  00d8                    add      al, bl                         
  0x00CA4762  44                      inc      esp                            
  0x00CA4763  0000                    add      byte ptr [eax], al             
  0x00CA4765  7044                    jo       0xca47ab                       
  0x00CA4767  00470b                  add      byte ptr [edi + 0xb], al       
  0x00CA476A  0000                    add      byte ptr [eax], al             
  0x00CA476C  00d8                    add      al, bl                         
  0x00CA476E  57                      push     edi                            
  0x00CA476F  0090180c0020            add      byte ptr [eax + 0x20000c18], dl 
  0x00CA4775  60                      pushal                                  
  0x00CA4776  0000                    add      byte ptr [eax], al             
  0x00CA4778  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA477B  007c0b00                add      byte ptr [ebx + ecx], bh       
  0x00CA477F  0000                    add      byte ptr [eax], al             
  0x00CA4781  d85700                  fcom     dword ptr [edi]                
  0x00CA4784  90                      nop                                     
  0x00CA4785  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA4788  2310                    and      edx, dword ptr [eax]           
  0x00CA478A  0000                    add      byte ptr [eax], al             
  0x00CA478C  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA478F  003d02000090            add      byte ptr [0x90000002], bh      
  0x00CA4795  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA4798  2210                    and      dl, byte ptr [eax]             
  0x00CA479A  0000                    add      byte ptr [eax], al             
  0x00CA479C  007050                  add      byte ptr [eax + 0x50], dh      
                                        ; XREF: 0x00CA4759 (cond_jump)
  0x00CA479F  003e                    add      byte ptr [esi], bh             
  0x00CA47A1  0200                    add      al, byte ptr [eax]             
  0x00CA47A3  0090180c0021            add      byte ptr [eax + 0x21000c18], dl 
  0x00CA47A9  1000                    adc      byte ptr [eax], al             
                                        ; XREF: 0x00CA4765 (cond_jump)
  0x00CA47AB  0000                    add      byte ptr [eax], al             
  0x00CA47AD  7050                    jo       0xca47ff                       
  0x00CA47AF  003f                    add      byte ptr [edi], bh             
  0x00CA47B1  0200                    add      al, byte ptr [eax]             
  0x00CA47B3  0090180c0018            add      byte ptr [eax + 0x18000c18], dl 
  0x00CA47B9  40                      inc      eax                            
  0x00CA47BA  0000                    add      byte ptr [eax], al             
  0x00CA47BC  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA47BF  004002                  add      byte ptr [eax + 2], al         
  0x00CA47C2  0000                    add      byte ptr [eax], al             
  0x00CA47C4  00d8                    add      al, bl                         
  0x00CA47C6  61                      popal                                   
  0x00CA47C7  0000                    add      byte ptr [eax], al             
  0x00CA47C9  06                      push     es                             
  0x00CA47CA  3800                    cmp      byte ptr [eax], al             
  0x00CA47CC  004820                  add      byte ptr [eax + 0x20], cl      
  0x00CA47CF  0000                    add      byte ptr [eax], al             
  0x00CA47D1  d95700                  fst      dword ptr [edi]                
  0x00CA47D4  90                      nop                                     
  0x00CA47D5  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA47D8  1a20                    sbb      ah, byte ptr [eax]             
  0x00CA47DA  0000                    add      byte ptr [eax], al             
  0x00CA47DC  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA47DF  004e0b                  add      byte ptr [esi + 0xb], cl       
  0x00CA47E2  0000                    add      byte ptr [eax], al             
  0x00CA47E4  00d9                    add      cl, bl                         
  0x00CA47E6  57                      push     edi                            
  0x00CA47E7  008e5f010000            add      byte ptr [esi + 0x15f], cl     
  0x00CA47ED  7057                    jo       0xca4846                       
  0x00CA47EF  00490b                  add      byte ptr [ecx + 0xb], cl       
  0x00CA47F2  0000                    add      byte ptr [eax], al             
  0x00CA47F4  00d8                    add      al, bl                         
  0x00CA47F6  57                      push     edi                            
  0x00CA47F7  0090180c0018            add      byte ptr [eax + 0x18000c18], dl 
  0x00CA47FD  800000                  add      byte ptr [eax], 0              
  0x00CA4800  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA4803  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4806  0000                    add      byte ptr [eax], al             
  0x00CA4808  90                      nop                                     
  0x00CA4809  180c00                  sbb      byte ptr [eax + eax], cl       
  0x00CA480C  208000000070            and      byte ptr [eax + 0x70000000], al 
  0x00CA4812  50                      push     eax                            
  0x00CA4813  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA4816  0000                    add      byte ptr [eax], al             
  0x00CA4818  00d8                    add      al, bl                         
  0x00CA481A  57                      push     edi                            
  0x00CA481B  0090180c0018            add      byte ptr [eax + 0x18000c18], dl 
  0x00CA4821  1000                    adc      byte ptr [eax], al             
  0x00CA4823  0000                    add      byte ptr [eax], al             
  0x00CA4825  7050                    jo       0xca4877                       
  0x00CA4827  004c0b00                add      byte ptr [ebx + ecx], cl       
  0x00CA482B  0090180c0019            add      byte ptr [eax + 0x19000c18], dl 
  0x00CA4831  1000                    adc      byte ptr [eax], al             
  0x00CA4833  0000                    add      byte ptr [eax], al             
  0x00CA4835  7050                    jo       0xca4887                       
  0x00CA4837  004a0b                  add      byte ptr [edx + 0xb], cl       
  0x00CA483A  0000                    add      byte ptr [eax], al             
  0x00CA483C  00d8                    add      al, bl                         
  0x00CA483E  57                      push     edi                            
  0x00CA483F  0000                    add      byte ptr [eax], al             
  0x00CA4841  7057                    jo       0xca489a                       
  0x00CA4843  004b0b                  add      byte ptr [ebx + 0xb], cl       
                                        ; XREF: 0x00CA47ED (cond_jump)
  0x00CA4846  0000                    add      byte ptr [eax], al             
  0x00CA4848  00e0                    add      al, ah                         
  0x00CA484A  57                      push     edi                            
  0x00CA484B  0000                    add      byte ptr [eax], al             
  0x00CA484D  7057                    jo       0xca48a6                       
  0x00CA484F  004d0b                  add      byte ptr [ebp + 0xb], cl       
  0x00CA4852  0000                    add      byte ptr [eax], al             
  0x00CA4854  0c00                    or       al, 0                          
  0x00CA4856  0000                    add      byte ptr [eax], al             
  0x00CA4858  00f4                    add      ah, dh                         
  0x00CA485A  44                      inc      esp                            
  0x00CA485B  0000                    add      byte ptr [eax], al             
  0x00CA485D  0000                    add      byte ptr [eax], al             
  0x00CA485F  0000                    add      byte ptr [eax], al             
  0x00CA4861  7044                    jo       0xca48a7                       
  0x00CA4863  00960b000000            add      byte ptr [esi + 0xb], dl       
  0x00CA486A  56                      push     esi                            
  0x00CA486B  004002                  add      byte ptr [eax + 2], al         
  0x00CA486E  0000                    add      byte ptr [eax], al             
  0x00CA4870  00f4                    add      ah, dh                         
  0x00CA4872  44                      inc      esp                            
  0x00CA4873  0009                    add      byte ptr [ecx], cl             
  0x00CA4875  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA4825 (cond_jump)
  0x00CA4877  004500                  add      byte ptr [ebp], al             
  0x00CA487A  2000                    and      byte ptr [eax], al             
  0x00CA487C  41                      inc      ecx                            
  0x00CA487D  27                      daa                                     
  0x00CA487E  2000                    and      byte ptr [eax], al             
  0x00CA4880  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA4883  004002                  add      byte ptr [eax + 2], al         
  0x00CA4886  0000                    add      byte ptr [eax], al             
  0x00CA4888  00f0                    add      al, dh                         
  0x00CA488A  56                      push     esi                            
  0x00CA488B  00490b                  add      byte ptr [ecx + 0xb], cl       
  0x00CA488E  0000                    add      byte ptr [eax], al             
  0x00CA4890  00f4                    add      ah, dh                         
  0x00CA4892  44                      inc      esp                            
  0x00CA4893  001f                    add      byte ptr [edi], bl             
  0x00CA4895  0000                    add      byte ptr [eax], al             
  0x00CA4897  0045f4                  add      byte ptr [ebp - 0xc], al       
                                        ; XREF: 0x00CA4841 (cond_jump)
  0x00CA489A  45                      inc      ebp                            
  0x00CA489B  0000                    add      byte ptr [eax], al             
  0x00CA489D  0000                    add      byte ptr [eax], al             
  0x00CA489F  004127                  add      byte ptr [ecx + 0x27], al      
  0x00CA48A2  2000                    and      byte ptr [eax], al             
  0x00CA48A4  650020                  add      byte ptr gs:[eax], ah          
                                        ; XREF: 0x00CA4861 (cond_jump)
  0x00CA48A7  006129                  add      byte ptr [ecx + 0x29], ah      
  0x00CA48AA  2000                    and      byte ptr [eax], al             
  0x00CA48AC  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA48AF  00490b                  add      byte ptr [ecx + 0xb], cl       
  0x00CA48B2  0000                    add      byte ptr [eax], al             
  0x00CA48B4  00f0                    add      al, dh                         
  0x00CA48B6  56                      push     esi                            
  0x00CA48B7  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA48BA  0000                    add      byte ptr [eax], al             
  0x00CA48BC  00f4                    add      ah, dh                         
  0x00CA48BE  44                      inc      esp                            
  0x00CA48BF  0007                    add      byte ptr [edi], al             
  0x00CA48C1  0000                    add      byte ptr [eax], al             
  0x00CA48C3  0045f4                  add      byte ptr [ebp - 0xc], al       
  0x00CA48C6  45                      inc      ebp                            
  0x00CA48C7  0006                    add      byte ptr [esi], al             
  0x00CA48C9  0000                    add      byte ptr [eax], al             
  0x00CA48CB  0014a4                  add      byte ptr [esp], dl             
  0x00CA48CE  050065f444              add      eax, 0x44f46500                
  0x00CA48D3  0003                    add      byte ptr [ebx], al             
  0x00CA48D5  0000                    add      byte ptr [eax], al             
  0x00CA48D7  0011                    add      byte ptr [ecx], dl             
  0x00CA48D9  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA48DA  050045f445              add      eax, 0x45f44500                
  0x00CA48DF  0002                    add      byte ptr [edx], al             
  0x00CA48E1  0000                    add      byte ptr [eax], al             
  0x00CA48E3  000e                    add      byte ptr [esi], cl             
  0x00CA48E5  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA48E6  050065f444              add      eax, 0x44f46500                
  0x00CA48EB  000400                  add      byte ptr [eax + eax], al       
  0x00CA48EE  0000                    add      byte ptr [eax], al             
  0x00CA48F0  0ba4050045f445          or       esp, dword ptr [ebp + eax + 0x45f44500] 
  0x00CA48F7  000500000008            add      byte ptr [0x8000000], al       
  0x00CA48FD  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA48FE  050065f444              add      eax, 0x44f46500                
  0x00CA4903  0001                    add      byte ptr [ecx], al             
  0x00CA4905  0000                    add      byte ptr [eax], al             
  0x00CA4907  0005a4050000            add      byte ptr [0x5a4], al           
  0x00CA490D  f4                      hlt                                     
  0x00CA490E  44                      inc      esp                            
  0x00CA490F  0002                    add      byte ptr [edx], al             
  0x00CA4911  0000                    add      byte ptr [eax], al             
  0x00CA4913  0000                    add      byte ptr [eax], al             
  0x00CA4915  7044                    jo       0xca495b                       
  0x00CA4917  00960b000000            add      byte ptr [esi + 0xb], dl       
  0x00CA491D  7054                    jo       0xca4973                       
  0x00CA491F  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA4922  0000                    add      byte ptr [eax], al             
  0x00CA4924  0c00                    or       al, 0                          
  0x00CA4926  0000                    add      byte ptr [eax], al             
  0x00CA4928  00f0                    add      al, dh                         
  0x00CA492A  56                      push     esi                            
  0x00CA492B  0012                    add      byte ptr [edx], dl             
  0x00CA492D  0900                    or       dword ptr [eax], eax           
  0x00CA492F  0000                    add      byte ptr [eax], al             
  0x00CA4931  f4                      hlt                                     
  0x00CA4932  44                      inc      esp                            
  0x00CA4933  006507                  add      byte ptr [ebp + 7], ah         
  0x00CA4936  0200                    add      al, byte ptr [eax]             
  0x00CA4938  45                      inc      ebp                            
  0x00CA4939  0020                    add      byte ptr [eax], ah             
  0x00CA493B  0006                    add      byte ptr [esi], al             
  0x00CA493D  2405                    and      al, 5                          
  0x00CA493F  0000                    add      byte ptr [eax], al             
  0x00CA4942  56                      push     esi                            
  0x00CA4943  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA4946  0000                    add      byte ptr [eax], al             
  0x00CA4948  0300                    add      eax, dword ptr [eax]           
  0x00CA494A  2000                    and      byte ptr [eax], al             
  0x00CA494C  5e                      pop      esi                            
  0x00CA494D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA494E  05000c0000              add      eax, 0xc00                     
  0x00CA4953  0013                    add      byte ptr [ebx], dl             
  0x00CA4955  f4                      hlt                                     
  0x00CA4956  60                      pushal                                  
  0x00CA4957  00fd                    add      ch, bh                         
  0x00CA4959  0400                    add      al, 0                          
                                        ; XREF: 0x00CA4915 (cond_jump)
  0x00CA495B  009006060003            add      byte ptr [eax + 0x3000606], dl 
  0x00CA4961  0000                    add      byte ptr [eax], al             
  0x00CA4963  0000                    add      byte ptr [eax], al             
  0x00CA4965  58                      pop      eax                            
  0x00CA4966  54                      push     esp                            
  0x00CA4967  0000                    add      byte ptr [eax], al             
  0x00CA4969  58                      pop      eax                            
  0x00CA496A  54                      push     esp                            
  0x00CA496B  0013                    add      byte ptr [ebx], dl             
  0x00CA496D  f4                      hlt                                     
  0x00CA496E  60                      pushal                                  
  0x00CA496F  00a805000090            add      byte ptr [eax - 0x6ffffffb], ch 
  0x00CA4975  0506000200              add      eax, 0x20006                   
  0x00CA497A  0000                    add      byte ptr [eax], al             
  0x00CA497C  005854                  add      byte ptr [eax + 0x54], bl      
  0x00CA497F  0013                    add      byte ptr [ebx], dl             
  0x00CA4981  f4                      hlt                                     
  0x00CA4982  60                      pushal                                  
  0x00CA4983  007b05                  add      byte ptr [ebx + 5], bh         
  0x00CA4986  0000                    add      byte ptr [eax], al             
  0x00CA4988  90                      nop                                     
  0x00CA4989  2806                    sub      byte ptr [esi], al             
  0x00CA498B  0002                    add      byte ptr [edx], al             
  0x00CA498D  0000                    add      byte ptr [eax], al             
  0x00CA498F  0000                    add      byte ptr [eax], al             
  0x00CA4991  58                      pop      eax                            
  0x00CA4992  54                      push     esp                            
  0x00CA4993  0013                    add      byte ptr [ebx], dl             
  0x00CA4995  f4                      hlt                                     
  0x00CA4996  60                      pushal                                  
  0x00CA4997  00ae05000090            add      byte ptr [esi - 0x6ffffffb], ch 
  0x00CA499D  5a                      pop      edx                            
  0x00CA499E  06                      push     es                             
  0x00CA499F  0002                    add      byte ptr [edx], al             
  0x00CA49A1  0000                    add      byte ptr [eax], al             
  0x00CA49A3  0000                    add      byte ptr [eax], al             
  0x00CA49A5  58                      pop      eax                            
  0x00CA49A6  54                      push     esp                            
  0x00CA49A7  0013                    add      byte ptr [ebx], dl             
  0x00CA49A9  f4                      hlt                                     
  0x00CA49AA  60                      pushal                                  
  0x00CA49AB  0008                    add      byte ptr [eax], cl             
  0x00CA49AD  06                      push     es                             
  0x00CA49AE  0000                    add      byte ptr [eax], al             
  0x00CA49B0  90                      nop                                     
  0x00CA49B1  0506000200              add      eax, 0x20006                   
  0x00CA49B6  0000                    add      byte ptr [eax], al             
  0x00CA49B8  005854                  add      byte ptr [eax + 0x54], bl      
  0x00CA49BB  0013                    add      byte ptr [ebx], dl             
  0x00CA49BD  f4                      hlt                                     
  0x00CA49BE  60                      pushal                                  
  0x00CA49BF  000d06000090            add      byte ptr [0x90000006], cl      
  0x00CA49C5  0506000200              add      eax, 0x20006                   
  0x00CA49CA  0000                    add      byte ptr [eax], al             
  0x00CA49CC  005854                  add      byte ptr [eax + 0x54], bl      
  0x00CA49CF  0013                    add      byte ptr [ebx], dl             
  0x00CA49D1  f4                      hlt                                     
  0x00CA49D2  60                      pushal                                  
  0x00CA49D3  0009                    add      byte ptr [ecx], cl             
  0x00CA49D5  0500009010              add      eax, 0x10900000                
  0x00CA49DA  06                      push     es                             
  0x00CA49DB  0002                    add      byte ptr [edx], al             
  0x00CA49DD  0000                    add      byte ptr [eax], al             
  0x00CA49DF  0000                    add      byte ptr [eax], al             
  0x00CA49E1  58                      pop      eax                            
  0x00CA49E2  54                      push     esp                            
  0x00CA49E3  0013                    add      byte ptr [ebx], dl             
  0x00CA49E5  f4                      hlt                                     
  0x00CA49E6  60                      pushal                                  
  0x00CA49E7  0019                    add      byte ptr [ecx], bl             
  0x00CA49E9  0500009008              add      eax, 0x8900000                 
  0x00CA49EE  06                      push     es                             
  0x00CA49EF  0002                    add      byte ptr [edx], al             
  0x00CA49F1  0000                    add      byte ptr [eax], al             
  0x00CA49F3  0000                    add      byte ptr [eax], al             
  0x00CA49F5  58                      pop      eax                            
  0x00CA49F6  54                      push     esp                            
  0x00CA49F7  0013                    add      byte ptr [ebx], dl             
  0x00CA49F9  f4                      hlt                                     
  0x00CA49FA  60                      pushal                                  
  0x00CA49FB  0021                    add      byte ptr [ecx], ah             
  0x00CA49FD  050000903c              add      eax, 0x3c900000                
  0x00CA4A02  06                      push     es                             
  0x00CA4A03  0002                    add      byte ptr [edx], al             
  0x00CA4A05  0000                    add      byte ptr [eax], al             
  0x00CA4A07  0000                    add      byte ptr [eax], al             
  0x00CA4A09  58                      pop      eax                            
  0x00CA4A0A  54                      push     esp                            
  0x00CA4A0B  0013                    add      byte ptr [ebx], dl             
  0x00CA4A0D  f4                      hlt                                     
  0x00CA4A0E  60                      pushal                                  
  0x00CA4A0F  005d05                  add      byte ptr [ebp + 5], bl         
  0x00CA4A12  0000                    add      byte ptr [eax], al             
  0x00CA4A14  90                      nop                                     
  0x00CA4A15  1e                      push     ds                             
  0x00CA4A16  06                      push     es                             
  0x00CA4A17  0002                    add      byte ptr [edx], al             
  0x00CA4A19  0000                    add      byte ptr [eax], al             
  0x00CA4A1B  0000                    add      byte ptr [eax], al             
  0x00CA4A1D  58                      pop      eax                            
  0x00CA4A1E  54                      push     esp                            
  0x00CA4A1F  0013                    add      byte ptr [ebx], dl             
  0x00CA4A21  f4                      hlt                                     
  0x00CA4A22  60                      pushal                                  
  0x00CA4A23  0012                    add      byte ptr [edx], dl             
  0x00CA4A25  06                      push     es                             
  0x00CA4A26  0000                    add      byte ptr [eax], al             
  0x00CA4A28  93                      xchg     ebx, eax                       
  0x00CA4A29  0006                    add      byte ptr [esi], al             
  0x00CA4A2B  0002                    add      byte ptr [edx], al             
  0x00CA4A2D  0000                    add      byte ptr [eax], al             
  0x00CA4A2F  0000                    add      byte ptr [eax], al             
  0x00CA4A31  58                      pop      eax                            
  0x00CA4A32  54                      push     esp                            
  0x00CA4A33  0000                    add      byte ptr [eax], al             
  0x00CA4A35  f4                      hlt                                     
  0x00CA4A36  44                      inc      esp                            
  0x00CA4A37  006507                  add      byte ptr [ebp + 7], ah         
  0x00CA4A3A  0200                    add      al, byte ptr [eax]             
  0x00CA4A3C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA4A3F  0012                    add      byte ptr [edx], dl             
  0x00CA4A41  0900                    or       dword ptr [eax], eax           
  0x00CA4A43  0000                    add      byte ptr [eax], al             
  0x00CA4A45  f4                      hlt                                     
  0x00CA4A46  44                      inc      esp                            
  0x00CA4A47  0000                    add      byte ptr [eax], al             
  0x00CA4A49  0000                    add      byte ptr [eax], al             
  0x00CA4A4B  0000                    add      byte ptr [eax], al             
  0x00CA4A4D  7044                    jo       0xca4a93                       
  0x00CA4A4F  00960b000000            add      byte ptr [esi + 0xb], dl       
  0x00CA4A55  f4                      hlt                                     
  0x00CA4A56  61                      popal                                   
  0x00CA4A57  00c2                    add      dl, al                         
  0x00CA4A59  0f0000                  sldt     word ptr [eax]                 
  0x00CA4A5C  00f0                    add      al, dh                         
  0x00CA4A5E  7100                    jno      0xca4a60                       
                                        ; XREF: 0x00CA4A5E (cond_jump)
  0x00CA4A60  7d0b                    jge      0xca4a6d                       
  0x00CA4A62  0000                    add      byte ptr [eax], al             
  0x00CA4A64  00f0                    add      al, dh                         
  0x00CA4A66  44                      inc      esp                            
  0x00CA4A67  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA4A6A  0000                    add      byte ptr [eax], al             
  0x00CA4A6C  00e9                    add      cl, ch                         
  0x00CA4A6E  5e                      pop      esi                            
  0x00CA4A6F  004070                  add      byte ptr [eax + 0x70], al      
  0x00CA4A72  54                      push     esp                            
  0x00CA4A73  00970b000000            add      byte ptr [edi + 0xb], dl       
  0x00CA4A79  7054                    jo       0xca4acf                       
  0x00CA4A7B  00980b00001b            add      byte ptr [eax + 0x1b00000b], bl 
  0x00CA4A82  44                      inc      esp                            
  0x00CA4A83  00970b000013            add      byte ptr [edi + 0x1300000b], dl 
  0x00CA4A89  052d004d02              add      eax, 0x24d002d                 
  0x00CA4A8E  2c00                    sub      al, 0                          
  0x00CA4A90  5a                      pop      edx                            
  0x00CA4A91  94                      xchg     esp, eax                       
  0x00CA4A92  05001bf044              add      eax, 0x44f01b00                
  0x00CA4A97  00980b000013            add      byte ptr [eax + 0x1300000b], bl 
  0x00CA4A9D  06                      push     es                             
  0x00CA4A9E  2d004d022c              sub      eax, 0x2c024d00                
  0x00CA4AA3  005594                  add      byte ptr [ebp - 0x6c], dl      
  0x00CA4AA6  050000f056              add      eax, 0x56f00000                
  0x00CA4AAB  007c0b00                add      byte ptr [ebx + ecx], bh       
  0x00CA4AAF  0023                    add      byte ptr [ebx], ah             
  0x00CA4AB1  0020                    add      byte ptr [eax], ah             
  0x00CA4AB3  0000                    add      byte ptr [eax], al             
  0x00CA4AB5  7054                    jo       0xca4b0b                       
  0x00CA4AB7  00990b00001b            add      byte ptr [ecx + 0x1b00000b], bl 
  0x00CA4ABE  44                      inc      esp                            
  0x00CA4ABF  007b0b                  add      byte ptr [ebx + 0xb], bh       
  0x00CA4AC2  0000                    add      byte ptr [eax], al             
  0x00CA4AC4  1303                    adc      eax, dword ptr [ebx]           
  0x00CA4AC6  2d004d042c              sub      eax, 0x2c044d00                
  0x00CA4ACB  004b94                  add      byte ptr [ebx - 0x6c], cl      
  0x00CA4ACE  05001bf044              add      eax, 0x44f01b00                
  0x00CA4AD3  00990b000013            add      byte ptr [ecx + 0x1300000b], bl 
  0x00CA4AD9  132d004d032c            adc      ebp, dword ptr [0x2c034d00]    
  0x00CA4ADF  004694                  add      byte ptr [esi - 0x6c], al      
  0x00CA4AE2  050000f056              add      eax, 0x56f00000                
  0x00CA4AE7  007c0b00                add      byte ptr [ebx + ecx], bh       
  0x00CA4AEB  00c4                    add      ah, al                         
  0x00CA4AED  40                      inc      eax                            
  0x00CA4AEE  0100                    add      dword ptr [eax], eax           
  0x00CA4AF0  2400                    and      al, 0                          
  0x00CA4AF2  0000                    add      byte ptr [eax], al             
  0x00CA4AF4  00da                    add      dl, bl                         
  0x00CA4AF6  2100                    and      dword ptr [eax], eax           
  0x00CA4AF8  00f0                    add      al, dh                         
  0x00CA4AFA  44                      inc      esp                            
  0x00CA4AFB  007b0b                  add      byte ptr [ebx + 0xb], bh       
  0x00CA4AFE  0000                    add      byte ptr [eax], al             
  0x00CA4B00  00f4                    add      ah, dh                         
  0x00CA4B02  46                      inc      esi                            
  0x00CA4B03  0006                    add      byte ptr [esi], al             
  0x00CA4B05  0000                    add      byte ptr [eax], al             
  0x00CA4B07  00d0                    add      al, dl                         
  0x00CA4B09  44                      inc      esp                            
  0x00CA4B0A  2300                    and      eax, dword ptr [eax]           
  0x00CA4B0C  2e1d0c0040f4            sbb      eax, 0xf440000c                
  0x00CA4B12  44                      inc      esp                            
  0x00CA4B13  004c0f00                add      byte ptr [edi + ecx], cl       
  0x00CA4B17  004000                  add      byte ptr [eax], al             
  0x00CA4B1A  2000                    and      byte ptr [eax], al             
  0x00CA4B1C  0091210000e1            add      byte ptr [ecx - 0x1effffdf], dl 
  0x00CA4B22  5e                      pop      esi                            
  0x00CA4B23  0022                    add      byte ptr [edx], ah             
  0x00CA4B25  cf                      iretd                                   
  0x00CA4B26  2100                    and      dword ptr [eax], eax           
  0x00CA4B28  22842100220020          and      al, byte ptr [ecx + 0x20002200] 
  0x00CA4B2F  004070                  add      byte ptr [eax + 0x70], al      
  0x00CA4B32  57                      push     edi                            
  0x00CA4B33  009a0b000000            add      byte ptr [edx + 0xb], bl       
  0x00CA4B39  8521                    test     dword ptr [ecx], esp           
  0x00CA4B3B  006ce421                add      byte ptr [esp + 0x21], ch      
  0x00CA4B3F  0000                    add      byte ptr [eax], al             
  0x00CA4B41  f4                      hlt                                     
  0x00CA4B42  46                      inc      esi                            
  0x00CA4B43  0008                    add      byte ptr [eax], cl             
  0x00CA4B45  0000                    add      byte ptr [eax], al             
  0x00CA4B47  00d0                    add      al, dl                         
  0x00CA4B49  a7                      cmpsd    dword ptr [esi], dword ptr es:[edi] 
  0x00CA4B4A  2100                    and      dword ptr [eax], eax           
  0x00CA4B4C  e87050009d              call     0x9dca9bc1                     
  0x00CA4B51  0b00                    or       eax, dword ptr [eax]           
  0x00CA4B53  0000                    add      byte ptr [eax], al             
  0x00CA4B55  7045                    jo       0xca4b9c                       
  0x00CA4B57  009b0b0000b0            add      byte ptr [ebx - 0x4ffffff5], bl 
  0x00CA4B5D  7051                    jo       0xca4bb0                       
  0x00CA4B5F  009e0b000000            add      byte ptr [esi + 0xb], bl       
  0x00CA4B65  7047                    jo       0xca4bae                       
  0x00CA4B67  009c0b00000070          add      byte ptr [ebx + ecx + 0x70000000], bl 
  0x00CA4B6E  50                      push     eax                            
  0x00CA4B6F  009f0b000003            add      byte ptr [edi + 0x300000b], bl 
  0x00CA4B75  0c05                    or       al, 5                          
  0x00CA4B77  0000                    add      byte ptr [eax], al             
  0x00CA4B79  7054                    jo       0xca4bcf                       
  0x00CA4B7B  00960b00000c            add      byte ptr [esi + 0xc00000b], dl 
  0x00CA4B81  0000                    add      byte ptr [eax], al             
  0x00CA4B83  0000                    add      byte ptr [eax], al             
  0x00CA4B85  f4                      hlt                                     
  0x00CA4B86  56                      push     esi                            
  0x00CA4B87  001409                  add      byte ptr [ecx + ecx], dl       
  0x00CA4B8A  0000                    add      byte ptr [eax], al             
  0x00CA4B8C  00f0                    add      al, dh                         
  0x00CA4B8E  44                      inc      esp                            
  0x00CA4B8F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4B92  0000                    add      byte ptr [eax], al             
  0x00CA4B94  40                      inc      eax                            
  0x00CA4B95  0020                    add      byte ptr [eax], ah             
  0x00CA4B97  0000                    add      byte ptr [eax], al             
  0x00CA4B99  91                      xchg     ecx, eax                       
  0x00CA4B9A  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA4B55 (cond_jump)
  0x00CA4B9C  00e1                    add      cl, ah                         
  0x00CA4B9E  56                      push     esi                            
  0x00CA4B9F  0001                    add      byte ptr [ecx], al             
  0x00CA4BA1  1e                      push     ds                             
  0x00CA4BA2  0c00                    or       al, 0                          
  0x00CA4BA4  3ef4                    hlt                                     
  0x00CA4BA6  44                      inc      esp                            
  0x00CA4BA7  0001                    add      byte ptr [ecx], al             
  0x00CA4BA9  0000                    add      byte ptr [eax], al             
  0x00CA4BAB  004c0020                add      byte ptr [eax + eax + 0x20], cl 
  0x00CA4BAF  001b                    add      byte ptr [ebx], bl             
  0x00CA4BB1  2920                    sub      dword ptr [eax], esp           
  0x00CA4BB3  0003                    add      byte ptr [ebx], al             
  0x00CA4BB5  f4                      hlt                                     
  0x00CA4BB6  45                      inc      ebp                            
  0x00CA4BB7  0003                    add      byte ptr [ebx], al             
  0x00CA4BB9  0000                    add      byte ptr [eax], al             
  0x00CA4BBB  0068a0                  add      byte ptr [eax - 0x60], ch      
  0x00CA4BBE  0200                    add      al, byte ptr [eax]             
  0x00CA4BC0  6d                      insd     dword ptr es:[edi], dx         
  0x00CA4BC1  0020                    add      byte ptr [eax], ah             
  0x00CA4BC3  006870                  add      byte ptr [eax + 0x70], ch      
  0x00CA4BC6  0200                    add      al, byte ptr [eax]             
  0x00CA4BC8  00f4                    add      ah, dh                         
  0x00CA4BCA  56                      push     esi                            
  0x00CA4BCB  00610b                  add      byte ptr [ecx + 0xb], ah       
  0x00CA4BCE  0000                    add      byte ptr [eax], al             
  0x00CA4BD0  00f0                    add      al, dh                         
  0x00CA4BD2  44                      inc      esp                            
  0x00CA4BD3  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4BD6  0000                    add      byte ptr [eax], al             
  0x00CA4BD8  40                      inc      eax                            
  0x00CA4BD9  0020                    add      byte ptr [eax], ah             
  0x00CA4BDB  0000                    add      byte ptr [eax], al             
  0x00CA4BDD  90                      nop                                     
  0x00CA4BDE  2100                    and      dword ptr [eax], eax           
  0x00CA4BE0  006055                  add      byte ptr [eax + 0x55], ah      
  0x00CA4BE3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA4BE6  0000                    add      byte ptr [eax], al             
  0x00CA4BE8  00f0                    add      al, dh                         
  0x00CA4BEA  44                      inc      esp                            
  0x00CA4BEB  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA4BEE  0000                    add      byte ptr [eax], al             
  0x00CA4BF0  00f4                    add      ah, dh                         
  0x00CA4BF2  46                      inc      esi                            
  0x00CA4BF3  0006                    add      byte ptr [esi], al             
  0x00CA4BF5  0000                    add      byte ptr [eax], al             
  0x00CA4BF7  00d0                    add      al, dl                         
  0x00CA4BFA  44                      inc      esp                            
  0x00CA4BFB  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4BFE  0000                    add      byte ptr [eax], al             
  0x00CA4C00  2e1d0c0040f4            sbb      eax, 0xf440000c                
  0x00CA4C06  44                      inc      esp                            
  0x00CA4C07  00920f000040            add      byte ptr [edx + 0x4000000f], dl 
  0x00CA4C0D  0020                    add      byte ptr [eax], ah             
  0x00CA4C0F  0000                    add      byte ptr [eax], al             
  0x00CA4C11  94                      xchg     esp, eax                       
  0x00CA4C12  2100                    and      dword ptr [eax], eax           
  0x00CA4C14  00f0                    add      al, dh                         
  0x00CA4C16  56                      push     esi                            
  0x00CA4C17  00440b00                add      byte ptr [ebx + ecx], al       
  0x00CA4C1B  0000                    add      byte ptr [eax], al             
  0x00CA4C1D  e44c                    in       al, 0x4c                       
  0x00CA4C1F  004000                  add      byte ptr [eax], al             
  0x00CA4C22  2000                    and      byte ptr [eax], al             
  0x00CA4C24  0091210020e1            add      byte ptr [ecx - 0x1edfffdf], dl 
  0x00CA4C2A  050000f056              add      eax, 0x56f00000                
  0x00CA4C2F  00420b                  add      byte ptr [edx + 0xb], al       
  0x00CA4C32  0000                    add      byte ptr [eax], al             
  0x00CA4C34  00e4                    add      ah, ah                         
  0x00CA4C36  4c                      dec      esp                            
  0x00CA4C37  004000                  add      byte ptr [eax], al             
  0x00CA4C3A  2000                    and      byte ptr [eax], al             
  0x00CA4C3C  0091210000f0            add      byte ptr [ecx - 0xfffffdf], dl 
  0x00CA4C42  56                      push     esi                            
  0x00CA4C43  00430b                  add      byte ptr [ebx + 0xb], al       
  0x00CA4C46  0000                    add      byte ptr [eax], al             
  0x00CA4C48  00e4                    add      ah, ah                         
  0x00CA4C4A  4c                      dec      esp                            
  0x00CA4C4B  004000                  add      byte ptr [eax], al             
  0x00CA4C4E  2000                    and      byte ptr [eax], al             
  0x00CA4C50  0092210000f4            add      byte ptr [edx - 0xbffffdf], dl 
  0x00CA4C56  44                      inc      esp                            
  0x00CA4C57  0000                    add      byte ptr [eax], al             
  0x00CA4C59  0100                    add      dword ptr [eax], eax           
  0x00CA4C5B  0000                    add      byte ptr [eax], al             
  0x00CA4C5D  e246                    loop     0xca4ca5                       
  0x00CA4C5F  00d0                    add      al, dl                         
  0x00CA4C61  0020                    add      byte ptr [eax], ah             
  0x00CA4C63  0022                    add      byte ptr [edx], ah             
  0x00CA4C65  002400                  add      byte ptr [eax + eax], ah       
  0x00CA4C68  0006                    add      byte ptr [esi], al             
  0x00CA4C6A  2100                    and      dword ptr [eax], eax           
  0x00CA4C6C  d000                    rol      byte ptr [eax], 1              
  0x00CA4C6E  2400                    and      al, 0                          
  0x00CA4C70  00e2                    add      dl, ah                         
  0x00CA4C72  46                      inc      esi                            
  0x00CA4C73  00d2                    add      dl, dl                         
  0x00CA4C75  002400                  add      byte ptr [eax + eax], ah       
  0x00CA4C78  2e1d0c0040e1            sbb      eax, 0xe140000c                
  0x00CA4C7E  44                      inc      esp                            
  0x00CA4C7F  004000                  add      byte ptr [eax], al             
  0x00CA4C82  2000                    and      byte ptr [eax], al             
  0x00CA4C84  0090210000e2            add      byte ptr [eax - 0x1dffffdf], dl 
  0x00CA4C8A  7000                    jo       0xca4c8c                       
                                        ; XREF: 0x00CA4C8A (cond_jump)
  0x00CA4C8C  00f4                    add      ah, dh                         
  0x00CA4C8E  6400fd                  add      ch, bh                         
  0x00CA4C91  0200                    add      al, byte ptr [eax]             
  0x00CA4C93  0000                    add      byte ptr [eax], al             
  0x00CA4C95  013c00                  add      dword ptr [eax + eax], edi     
  0x00CA4C98  00ff                    add      bh, bh                         
  0x00CA4C9A  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA4C9D  f4                      hlt                                     
  0x00CA4C9E  45                      inc      ebp                            
  0x00CA4C9F  00cf                    add      bh, cl                         
  0x00CA4CA1  f73f                    idiv     dword ptr [edi]                
  0x00CA4CA3  0000                    add      byte ptr [eax], al             
  0x00CA4CA6  56                      push     esi                            
  0x00CA4CA7  003f                    add      byte ptr [edi], bh             
  0x00CA4CA9  0200                    add      al, byte ptr [eax]             
  0x00CA4CAB  0003                    add      byte ptr [ebx], al             
  0x00CA4CAD  0020                    add      byte ptr [eax], ah             
  0x00CA4CAF  000f                    add      byte ptr [edi], cl             
  0x00CA4CB1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA4CB2  050000f056              add      eax, 0x56f00000                
  0x00CA4CB7  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4CBA  0000                    add      byte ptr [eax], al             
  0x00CA4CBC  32f4                    xor      dh, ah                         
  0x00CA4CBE  44                      inc      esp                            
  0x00CA4CBF  00fd                    add      ch, bh                         
  0x00CA4CC1  0400                    add      al, 0                          
  0x00CA4CC3  004000                  add      byte ptr [eax], al             
  0x00CA4CC6  2000                    and      byte ptr [eax], al             
  0x00CA4CC8  0091210000f4            add      byte ptr [ecx - 0xbffffdf], dl 
  0x00CA4CCE  47                      inc      edi                            
  0x00CA4CCF  004703                  add      byte ptr [edi + 3], al         
  0x00CA4CD2  0000                    add      byte ptr [eax], al             
  0x00CA4CD4  00d9                    add      cl, bl                         
  0x00CA4CD6  57                      push     edi                            
  0x00CA4CD7  0000                    add      byte ptr [eax], al             
  0x00CA4CD9  d15100                  rcl      dword ptr [ecx]                
  0x00CA4CDC  e704                    out      4, eax                         
  0x00CA4CDE  0d00005955              or       eax, 0x55590000                
  0x00CA4CE3  0000                    add      byte ptr [eax], al             
  0x00CA4CE5  61                      popal                                   
  0x00CA4CE6  51                      push     ecx                            
  0x00CA4CE7  0002                    add      byte ptr [edx], al             
  0x00CA4CE9  0c05                    or       al, 5                          
  0x00CA4CEB  00f4                    add      ah, dh                         
  0x00CA4CED  040d                    add      al, 0xd                        
  0x00CA4CEF  0020                    add      byte ptr [eax], ah             
  0x00CA4CF1  f4                      hlt                                     
  0x00CA4CF2  0500ffff00              add      eax, 0xffff00                  
  0x00CA4CF7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA4CFA  0000                    add      byte ptr [eax], al             
  0x00CA4CFC  00f0                    add      al, dh                         
  0x00CA4CFE  56                      push     esi                            
  0x00CA4CFF  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4D02  0000                    add      byte ptr [eax], al             
  0x00CA4D04  00f0                    add      al, dh                         
  0x00CA4D06  44                      inc      esp                            
  0x00CA4D07  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA4D0D  0020                    add      byte ptr [eax], ah             
  0x00CA4D0F  004da4                  add      byte ptr [ebp - 0x5c], cl      
  0x00CA4D12  050000f056              add      eax, 0x56f00000                
  0x00CA4D17  003d02000003            add      byte ptr [0x3000002], bh       
  0x00CA4D1D  0020                    add      byte ptr [eax], ah             
  0x00CA4D1F  005ba4                  add      byte ptr [ebx - 0x5c], bl      
  0x00CA4D22  050000f460              add      eax, 0x60f40000                
  0x00CA4D27  00fd                    add      ch, bh                         
  0x00CA4D29  0200                    add      al, byte ptr [eax]             
  0x00CA4D2B  0000                    add      byte ptr [eax], al             
  0x00CA4D2D  1422                    adc      al, 0x22                       
  0x00CA4D2F  0000                    add      byte ptr [eax], al             
  0x00CA4D31  0138                    add      dword ptr [eax], edi           
  0x00CA4D33  0000                    add      byte ptr [eax], al             
  0x00CA4D35  1c23                    sbb      al, 0x23                       
  0x00CA4D37  0000                    add      byte ptr [eax], al             
  0x00CA4D3A  44                      inc      esp                            
  0x00CA4D3B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4D3E  0000                    add      byte ptr [eax], al             
  0x00CA4D40  00f4                    add      ah, dh                         
  0x00CA4D42  46                      inc      esi                            
  0x00CA4D43  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA4D46  0000                    add      byte ptr [eax], al             
  0x00CA4D48  d0f4                    sal      ah, 1                          
  0x00CA4D4A  44                      inc      esp                            
  0x00CA4D4B  0021                    add      byte ptr [ecx], ah             
  0x00CA4D4D  0500002e1d              add      eax, 0x1d2e0000                
  0x00CA4D52  0c00                    or       al, 0                          
  0x00CA4D54  40                      inc      eax                            
  0x00CA4D55  0020                    add      byte ptr [eax], ah             
  0x00CA4D57  0000                    add      byte ptr [eax], al             
  0x00CA4D59  91                      xchg     ecx, eax                       
  0x00CA4D5A  2100                    and      dword ptr [eax], eax           
  0x00CA4D5C  00f0                    add      al, dh                         
  0x00CA4D5E  44                      inc      esp                            
  0x00CA4D5F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4D62  0000                    add      byte ptr [eax], al             
  0x00CA4D64  00f4                    add      ah, dh                         
  0x00CA4D66  46                      inc      esi                            
  0x00CA4D67  0006                    add      byte ptr [esi], al             
  0x00CA4D69  0000                    add      byte ptr [eax], al             
  0x00CA4D6B  00d0                    add      al, dl                         
  0x00CA4D6D  f4                      hlt                                     
  0x00CA4D6E  44                      inc      esp                            
  0x00CA4D6F  005d05                  add      byte ptr [ebp + 5], bl         
  0x00CA4D72  0000                    add      byte ptr [eax], al             
  0x00CA4D74  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA4D7A  2000                    and      byte ptr [eax], al             
  0x00CA4D7C  0092210000f0            add      byte ptr [edx - 0xfffffdf], dl 
  0x00CA4D82  56                      push     esi                            
  0x00CA4D83  004002                  add      byte ptr [eax + 2], al         
  0x00CA4D86  0000                    add      byte ptr [eax], al             
  0x00CA4D88  c44001                  les      eax, ptr [eax + 1]             
  0x00CA4D8B  0007                    add      byte ptr [edi], al             
  0x00CA4D8D  0000                    add      byte ptr [eax], al             
  0x00CA4D8F  0000                    add      byte ptr [eax], al             
  0x00CA4D91  da21                    fisub    dword ptr [ecx]                
  0x00CA4D93  0000                    add      byte ptr [eax], al             
  0x00CA4D95  44                      inc      esp                            
  0x00CA4D96  2300                    and      eax, dword ptr [eax]           
  0x00CA4D98  00f4                    add      ah, dh                         
  0x00CA4D9A  46                      inc      esi                            
  0x00CA4D9B  000f                    add      byte ptr [edi], cl             
  0x00CA4D9D  0000                    add      byte ptr [eax], al             
  0x00CA4D9F  00d0                    add      al, dl                         
  0x00CA4DA1  f4                      hlt                                     
  0x00CA4DA2  44                      inc      esp                            
  0x00CA4DA3  001408                  add      byte ptr [eax + ecx], dl       
  0x00CA4DA6  0000                    add      byte ptr [eax], al             
  0x00CA4DA8  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA4DAE  2000                    and      byte ptr [eax], al             
  0x00CA4DB0  009521000003            add      byte ptr [ebp + 0x3000021], dl 
  0x00CA4DB6  3a00                    cmp      al, byte ptr [eax]             
  0x00CA4DB8  00ff                    add      bh, bh                         
  0x00CA4DBA  3e009e040d0013          add      byte ptr ds:[esi + 0x13000d04], bl 
  0x00CA4DC1  0c05                    or       al, 5                          
  0x00CA4DC3  0000                    add      byte ptr [eax], al             
  0x00CA4DC6  56                      push     esi                            
  0x00CA4DC7  003e                    add      byte ptr [esi], bh             
  0x00CA4DC9  0200                    add      al, byte ptr [eax]             
  0x00CA4DCB  0003                    add      byte ptr [ebx], al             
  0x00CA4DCD  0020                    add      byte ptr [eax], ah             
  0x00CA4DCF  000f                    add      byte ptr [edi], cl             
  0x00CA4DD1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA4DD2  050000f460              add      eax, 0x60f40000                
  0x00CA4DD7  00fd                    add      ch, bh                         
  0x00CA4DD9  0200                    add      al, byte ptr [eax]             
  0x00CA4DDB  0000                    add      byte ptr [eax], al             
  0x00CA4DDD  1422                    adc      al, 0x22                       
  0x00CA4DDF  0000                    add      byte ptr [eax], al             
  0x00CA4DE1  0138                    add      dword ptr [eax], edi           
  0x00CA4DE3  0000                    add      byte ptr [eax], al             
  0x00CA4DE5  1c23                    sbb      al, 0x23                       
  0x00CA4DE7  0000                    add      byte ptr [eax], al             
  0x00CA4DE9  f4                      hlt                                     
  0x00CA4DEA  61                      popal                                   
  0x00CA4DEB  0009                    add      byte ptr [ecx], cl             
  0x00CA4DED  05000000f4              add      eax, 0xf4000000                
  0x00CA4DF2  6200                    bound    eax, qword ptr [eax]           
  0x00CA4DF4  1905000000f4            sbb      dword ptr [0xf4000000], eax    
  0x00CA4DFA  650000                  add      byte ptr gs:[eax], al          
  0x00CA4DFD  0800                    or       byte ptr [eax], al             
  0x00CA4DFF  0000                    add      byte ptr [eax], al             
  0x00CA4E01  043a                    add      al, 0x3a                       
  0x00CA4E03  0000                    add      byte ptr [eax], al             
  0x00CA4E06  3e00bf040d000c          add      byte ptr ds:[edi + 0xc000d04], bh 
  0x00CA4E0D  0000                    add      byte ptr [eax], al             
  0x00CA4E0F  0000                    add      byte ptr [eax], al             
  0x00CA4E12  44                      inc      esp                            
  0x00CA4E13  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA4E16  0000                    add      byte ptr [eax], al             
  0x00CA4E18  00f4                    add      ah, dh                         
  0x00CA4E1A  46                      inc      esi                            
  0x00CA4E1B  0006                    add      byte ptr [esi], al             
  0x00CA4E1D  0000                    add      byte ptr [eax], al             
  0x00CA4E1F  00d0                    add      al, dl                         
  0x00CA4E22  44                      inc      esp                            
  0x00CA4E23  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4E26  0000                    add      byte ptr [eax], al             
  0x00CA4E28  2e1d0c0040f4            sbb      eax, 0xf440000c                
  0x00CA4E2E  44                      inc      esp                            
  0x00CA4E2F  00920f000040            add      byte ptr [edx + 0x4000000f], dl 
  0x00CA4E35  0020                    add      byte ptr [eax], ah             
  0x00CA4E37  0000                    add      byte ptr [eax], al             
  0x00CA4E39  91                      xchg     ecx, eax                       
  0x00CA4E3A  2100                    and      dword ptr [eax], eax           
  0x00CA4E3C  00e1                    add      cl, ah                         
  0x00CA4E3E  4c                      dec      esp                            
  0x00CA4E3F  0000                    add      byte ptr [eax], al             
  0x00CA4E41  7044                    jo       0xca4e87                       
  0x00CA4E43  0013                    add      byte ptr [ebx], dl             
  0x00CA4E45  0900                    or       dword ptr [eax], eax           
  0x00CA4E47  0000                    add      byte ptr [eax], al             
  0x00CA4E49  f4                      hlt                                     
  0x00CA4E4A  61                      popal                                   
  0x00CA4E4B  00fd                    add      ch, bh                         
  0x00CA4E4D  0200                    add      al, byte ptr [eax]             
  0x00CA4E4F  0013                    add      byte ptr [ebx], dl             
  0x00CA4E51  0020                    add      byte ptr [eax], ah             
  0x00CA4E53  001b                    add      byte ptr [ebx], bl             
  0x00CA4E55  d9440091                fld      dword ptr [eax + eax - 0x6f]   
  0x00CA4E59  0006                    add      byte ptr [esi], al             
  0x00CA4E5B  000400                  add      byte ptr [eax + eax], al       
  0x00CA4E5E  0000                    add      byte ptr [eax], al             
  0x00CA4E60  47                      inc      edi                            
  0x00CA4E61  0020                    add      byte ptr [eax], ah             
  0x00CA4E63  004090                  add      byte ptr [eax - 0x70], al      
  0x00CA4E66  0200                    add      al, byte ptr [eax]             
  0x00CA4E68  8ad9                    mov      bl, cl                         
  0x00CA4E6A  44                      inc      esp                            
  0x00CA4E6B  0000                    add      byte ptr [eax], al             
  0x00CA4E6D  f4                      hlt                                     
  0x00CA4E6E  60                      pushal                                  
  0x00CA4E6F  00a805000000            add      byte ptr [eax + 5], ch         
  0x00CA4E76  7000                    jo       0xca4e78                       
                                        ; XREF: 0x00CA4E76 (cond_jump)
  0x00CA4E78  41                      inc      ecx                            
  0x00CA4E79  0b00                    or       eax, dword ptr [eax]           
  0x00CA4E7B  0032                    add      byte ptr [edx], dh             
  0x00CA4E7D  0020                    add      byte ptr [eax], ah             
  0x00CA4E7F  0026                    add      byte ptr [esi], ah             
  0x00CA4E81  e844004768              call     0x69114eca                     
  0x00CA4E86  56                      push     esi                            
                                        ; XREF: 0x00CA4E41 (cond_jump)
  0x00CA4E87  004090                  add      byte ptr [eax - 0x70], al      
  0x00CA4E8A  0200                    add      al, byte ptr [eax]             
  0x00CA4E8C  00c7                    add      bh, al                         
  0x00CA4E8E  2100                    and      dword ptr [eax], eax           
  0x00CA4E90  00f4                    add      ah, dh                         
  0x00CA4E92  56                      push     esi                            
  0x00CA4E93  001409                  add      byte ptr [ecx + ecx], dl       
  0x00CA4E96  0000                    add      byte ptr [eax], al             
  0x00CA4E98  00f0                    add      al, dh                         
  0x00CA4E9A  44                      inc      esp                            
  0x00CA4E9B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4E9E  0000                    add      byte ptr [eax], al             
  0x00CA4EA0  40                      inc      eax                            
  0x00CA4EA1  0020                    add      byte ptr [eax], ah             
  0x00CA4EA3  0000                    add      byte ptr [eax], al             
  0x00CA4EA5  90                      nop                                     
  0x00CA4EA6  2100                    and      dword ptr [eax], eax           
  0x00CA4EA8  006047                  add      byte ptr [eax + 0x47], ah      
  0x00CA4EAB  00911c0c0000            add      byte ptr [ecx + 0xc1c], dl     
  0x00CA4EB2  44                      inc      esp                            
  0x00CA4EB3  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4EB6  0000                    add      byte ptr [eax], al             
  0x00CA4EB8  00f4                    add      ah, dh                         
  0x00CA4EBA  46                      inc      esi                            
  0x00CA4EBB  0002                    add      byte ptr [edx], al             
  0x00CA4EBD  0000                    add      byte ptr [eax], al             
  0x00CA4EBF  00d0                    add      al, dl                         
  0x00CA4EC1  f4                      hlt                                     
  0x00CA4EC2  44                      inc      esp                            
  0x00CA4EC3  001a                    add      byte ptr [edx], bl             
  0x00CA4EC5  0900                    or       dword ptr [eax], eax           
  0x00CA4EC7  002e                    add      byte ptr [esi], ch             
  0x00CA4EC9  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA4ECE  2000                    and      byte ptr [eax], al             
  0x00CA4ED0  009021000058            add      byte ptr [eax + 0x58000021], dl 
  0x00CA4ED6  55                      push     ebp                            
  0x00CA4ED7  0000                    add      byte ptr [eax], al             
  0x00CA4ED9  60                      pushal                                  
  0x00CA4EDA  51                      push     ecx                            
  0x00CA4EDB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA4EDE  0000                    add      byte ptr [eax], al             
  0x00CA4EE0  00f4                    add      ah, dh                         
  0x00CA4EE2  60                      pushal                                  
  0x00CA4EE3  00fd                    add      ch, bh                         
  0x00CA4EE5  0200                    add      al, byte ptr [eax]             
  0x00CA4EE7  0000                    add      byte ptr [eax], al             
  0x00CA4EE9  f4                      hlt                                     
  0x00CA4EEA  6400fd                  add      ch, bh                         
  0x00CA4EED  0300                    add      eax, dword ptr [eax]           
  0x00CA4EEF  0000                    add      byte ptr [eax], al             
  0x00CA4EF1  0138                    add      dword ptr [eax], edi           
  0x00CA4EF3  0000                    add      byte ptr [eax], al             
  0x00CA4EF5  1c23                    sbb      al, 0x23                       
  0x00CA4EF7  0000                    add      byte ptr [eax], al             
  0x00CA4EFA  44                      inc      esp                            
  0x00CA4EFB  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4EFE  0000                    add      byte ptr [eax], al             
  0x00CA4F00  00f4                    add      ah, dh                         
  0x00CA4F02  46                      inc      esi                            
  0x00CA4F03  0008                    add      byte ptr [eax], cl             
  0x00CA4F05  0000                    add      byte ptr [eax], al             
  0x00CA4F07  00d0                    add      al, dl                         
  0x00CA4F09  f4                      hlt                                     
  0x00CA4F0A  44                      inc      esp                            
  0x00CA4F0B  007b05                  add      byte ptr [ebx + 5], bh         
  0x00CA4F0E  0000                    add      byte ptr [eax], al             
  0x00CA4F10  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA4F16  2000                    and      byte ptr [eax], al             
  0x00CA4F18  0091210000f4            add      byte ptr [ecx - 0xbffffdf], dl 
  0x00CA4F1E  6500a208000000          add      byte ptr gs:[edx + 8], ah      
  0x00CA4F25  023a                    add      bh, byte ptr [edx]             
  0x00CA4F27  0000                    add      byte ptr [eax], al             
  0x00CA4F2A  3e009e040d0000          add      byte ptr ds:[esi + 0xd04], bl  
  0x00CA4F32  44                      inc      esp                            
  0x00CA4F33  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4F36  0000                    add      byte ptr [eax], al             
  0x00CA4F38  00f4                    add      ah, dh                         
  0x00CA4F3A  46                      inc      esi                            
  0x00CA4F3B  0012                    add      byte ptr [edx], dl             
  0x00CA4F3D  0000                    add      byte ptr [eax], al             
  0x00CA4F3F  00d0                    add      al, dl                         
  0x00CA4F41  f4                      hlt                                     
  0x00CA4F42  44                      inc      esp                            
  0x00CA4F43  00ae0500002e            add      byte ptr [esi + 0x2e000005], ch 
  0x00CA4F49  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA4F4E  2000                    and      byte ptr [eax], al             
  0x00CA4F50  00902100000e            add      byte ptr [eax + 0xe000021], dl 
  0x00CA4F56  3800                    cmp      byte ptr [eax], al             
  0x00CA4F58  00f4                    add      ah, dh                         
  0x00CA4F5A  61                      popal                                   
  0x00CA4F5B  00fd                    add      ch, bh                         
  0x00CA4F5D  0300                    add      eax, dword ptr [eax]           
  0x00CA4F5F  0000                    add      byte ptr [eax], al             
  0x00CA4F61  48                      dec      eax                            
  0x00CA4F62  2000                    and      byte ptr [eax], al             
  0x00CA4F64  90                      nop                                     
  0x00CA4F65  0406                    add      al, 6                          
  0x00CA4F67  0009                    add      byte ptr [ecx], cl             
  0x00CA4F69  0000                    add      byte ptr [eax], al             
  0x00CA4F6B  0013                    add      byte ptr [ebx], dl             
  0x00CA4F6D  0020                    add      byte ptr [eax], ah             
  0x00CA4F6F  009040060004            add      byte ptr [eax + 0x4000640], dl 
  0x00CA4F75  0000                    add      byte ptr [eax], al             
  0x00CA4F77  0000                    add      byte ptr [eax], al             
  0x00CA4F79  d9440047                fld      dword ptr [eax + eax + 0x47]   
  0x00CA4F7D  0020                    add      byte ptr [eax], ah             
  0x00CA4F7F  004090                  add      byte ptr [eax - 0x70], al      
  0x00CA4F82  0200                    add      al, byte ptr [eax]             
  0x00CA4F84  260020                  add      byte ptr es:[eax], ah          
  0x00CA4F87  0000                    add      byte ptr [eax], al             
  0x00CA4F89  58                      pop      eax                            
  0x00CA4F8A  56                      push     esi                            
  0x00CA4F8B  0000                    add      byte ptr [eax], al             
  0x00CA4F8E  44                      inc      esp                            
  0x00CA4F8F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4F92  0000                    add      byte ptr [eax], al             
  0x00CA4F94  00f4                    add      ah, dh                         
  0x00CA4F96  46                      inc      esi                            
  0x00CA4F97  0012                    add      byte ptr [edx], dl             
  0x00CA4F99  0000                    add      byte ptr [eax], al             
  0x00CA4F9B  00d0                    add      al, dl                         
  0x00CA4F9D  f4                      hlt                                     
  0x00CA4F9E  44                      inc      esp                            
  0x00CA4F9F  00ae0500002e            add      byte ptr [esi + 0x2e000005], ch 
  0x00CA4FA5  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA4FAA  2000                    and      byte ptr [eax], al             
  0x00CA4FAC  009021000008            add      byte ptr [eax + 0x8000021], dl 
  0x00CA4FB2  3800                    cmp      byte ptr [eax], al             
  0x00CA4FB4  00f4                    add      ah, dh                         
  0x00CA4FB6  57                      push     edi                            
  0x00CA4FB7  0002                    add      byte ptr [edx], al             
  0x00CA4FB9  0000                    add      byte ptr [eax], al             
  0x00CA4FBB  0000                    add      byte ptr [eax], al             
  0x00CA4FBD  48                      dec      eax                            
  0x00CA4FBE  2000                    and      byte ptr [eax], al             
  0x00CA4FC0  0006                    add      byte ptr [esi], al             
  0x00CA4FC2  3800                    cmp      byte ptr [eax], al             
  0x00CA4FC4  90                      nop                                     
  0x00CA4FC5  0206                    add      al, byte ptr [esi]             
  0x00CA4FC7  000b                    add      byte ptr [ebx], cl             
  0x00CA4FC9  0000                    add      byte ptr [eax], al             
  0x00CA4FCB  0000                    add      byte ptr [eax], al             
  0x00CA4FCD  1122                    adc      dword ptr [edx], esp           
  0x00CA4FCF  0012                    add      byte ptr [edx], dl             
  0x00CA4FD1  48                      dec      eax                            
  0x00CA4FD2  0400                    add      al, 0                          
  0x00CA4FD4  10cd                    adc      ch, cl                         
  0x00CA4FD6  06                      push     es                             
  0x00CA4FD7  0006                    add      byte ptr [esi], al             
  0x00CA4FD9  0000                    add      byte ptr [eax], al             
  0x00CA4FDB  0000                    add      byte ptr [eax], al             
  0x00CA4FDD  da440000                fiadd    dword ptr [eax + eax]          
  0x00CA4FE1  da5600                  ficom    dword ptr [esi]                
  0x00CA4FE4  45                      inc      ebp                            
  0x00CA4FE5  0020                    add      byte ptr [eax], ah             
  0x00CA4FE7  004090                  add      byte ptr [eax - 0x70], al      
  0x00CA4FEA  0200                    add      al, byte ptr [eax]             
  0x00CA4FEC  005956                  add      byte ptr [ecx + 0x56], bl      
  0x00CA4FEF  002a                    add      byte ptr [edx], ch             
  0x00CA4FF1  40                      inc      eax                            
  0x00CA4FF2  2000                    and      byte ptr [eax], al             
  0x00CA4FF4  00f0                    add      al, dh                         
  0x00CA4FF6  44                      inc      esp                            
  0x00CA4FF7  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA4FFA  0000                    add      byte ptr [eax], al             
  0x00CA4FFC  00f4                    add      ah, dh                         
  0x00CA4FFE  46                      inc      esi                            
  0x00CA4FFF  0012                    add      byte ptr [edx], dl             
  0x00CA5001  0000                    add      byte ptr [eax], al             
  0x00CA5003  00d0                    add      al, dl                         
  0x00CA5005  f4                      hlt                                     
  0x00CA5006  44                      inc      esp                            
  0x00CA5007  00ae0500002e            add      byte ptr [esi + 0x2e000005], ch 
  0x00CA500D  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA5012  2000                    and      byte ptr [eax], al             
  0x00CA5014  009021000006            add      byte ptr [eax + 0x6000021], dl 
  0x00CA501A  3800                    cmp      byte ptr [eax], al             
  0x00CA501C  00f4                    add      ah, dh                         
  0x00CA501E  6200                    bound    eax, qword ptr [eax]           
  0x00CA5020  af                      scasd    eax, dword ptr es:[edi]        
  0x00CA5021  0800                    or       byte ptr [eax], al             
  0x00CA5023  0000                    add      byte ptr [eax], al             
  0x00CA5025  0239                    add      bh, byte ptr [ecx]             
  0x00CA5027  001b                    add      byte ptr [ebx], bl             
  0x00CA5029  f4                      hlt                                     
  0x00CA502A  45                      inc      ebp                            
  0x00CA502B  0001                    add      byte ptr [ecx], al             
  0x00CA502D  0000                    add      byte ptr [eax], al             
  0x00CA502F  0000                    add      byte ptr [eax], al             
  0x00CA5031  a6                      cmpsb    byte ptr [esi], byte ptr es:[edi] 
  0x00CA5032  2000                    and      byte ptr [eax], al             
  0x00CA5034  90                      nop                                     
  0x00CA5035  0306                    add      eax, dword ptr [esi]           
  0x00CA5037  000d00000000            add      byte ptr [0], cl               
  0x00CA503D  1122                    adc      dword ptr [edx], esp           
  0x00CA503F  0000                    add      byte ptr [eax], al             
  0x00CA5041  da4f00                  fimul    dword ptr [edi]                
  0x00CA5044  004920                  add      byte ptr [ecx + 0x20], cl      
  0x00CA5047  0000                    add      byte ptr [eax], al             
  0x00CA5049  d1440010                rol      dword ptr [eax + eax + 0x10], 1 
  0x00CA504D  c60600                  mov      byte ptr [esi], 0              
  0x00CA5050  0400                    add      al, 0                          
  0x00CA5052  0000                    add      byte ptr [eax], al             
  0x00CA5054  c0c944                  ror      cl, 0x44                       
  0x00CA5057  0045d1                  add      byte ptr [ebp - 0x2f], al      
  0x00CA505A  44                      inc      esp                            
  0x00CA505B  006870                  add      byte ptr [eax + 0x70], ch      
  0x00CA505E  0200                    add      al, byte ptr [eax]             
  0x00CA5060  00ce                    add      dh, cl                         
  0x00CA5062  2000                    and      byte ptr [eax], al             
  0x00CA5064  324820                  xor      cl, byte ptr [eax + 0x20]      
  0x00CA5067  0000                    add      byte ptr [eax], al             
  0x00CA5069  8621                    xchg     byte ptr [ecx], ah             
  0x00CA506B  0000                    add      byte ptr [eax], al             
  0x00CA506E  44                      inc      esp                            
  0x00CA506F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA5072  0000                    add      byte ptr [eax], al             
  0x00CA5074  00f4                    add      ah, dh                         
  0x00CA5076  46                      inc      esi                            
  0x00CA5077  0012                    add      byte ptr [edx], dl             
  0x00CA5079  0000                    add      byte ptr [eax], al             
  0x00CA507B  00d0                    add      al, dl                         
  0x00CA507D  f4                      hlt                                     
  0x00CA507E  44                      inc      esp                            
  0x00CA507F  00ae0500002e            add      byte ptr [esi + 0x2e000005], ch 
  0x00CA5085  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA508A  2000                    and      byte ptr [eax], al             
  0x00CA508C  009021000002            add      byte ptr [eax + 0x2000021], dl 
  0x00CA5092  3800                    cmp      byte ptr [eax], al             
  0x00CA5094  00f4                    add      ah, dh                         
  0x00CA5096  44                      inc      esp                            
  0x00CA5097  0000                    add      byte ptr [eax], al             
  0x00CA5099  3200                    xor      al, byte ptr [eax]             
  0x00CA509B  0000                    add      byte ptr [eax], al             
  0x00CA509D  e856004500              call     0x10f50f8                      
  0x00CA50A2  2000                    and      byte ptr [eax], al             
  0x00CA50A4  1b29                    sbb      ebp, dword ptr [ecx]           
  0x00CA50A6  2000                    and      byte ptr [eax], al             
  0x00CA50A8  00f0                    add      al, dh                         
  0x00CA50AA  44                      inc      esp                            
  0x00CA50AB  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA50AE  0000                    add      byte ptr [eax], al             
  0x00CA50B0  00f4                    add      ah, dh                         
  0x00CA50B2  46                      inc      esi                            
  0x00CA50B3  0002                    add      byte ptr [edx], al             
  0x00CA50B5  0000                    add      byte ptr [eax], al             
  0x00CA50B7  00d0                    add      al, dl                         
  0x00CA50B9  f4                      hlt                                     
  0x00CA50BA  44                      inc      esp                            
  0x00CA50BB  001a                    add      byte ptr [edx], bl             
  0x00CA50BD  0900                    or       dword ptr [eax], eax           
  0x00CA50BF  002e                    add      byte ptr [esi], ch             
  0x00CA50C1  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA50C6  2000                    and      byte ptr [eax], al             
  0x00CA50C8  0091210000f4            add      byte ptr [ecx - 0xbffffdf], dl 
  0x00CA50CE  56                      push     esi                            
  0x00CA50CF  0008                    add      byte ptr [eax], cl             
  0x00CA50D1  06                      push     es                             
  0x00CA50D2  0000                    add      byte ptr [eax], al             
  0x00CA50D4  00f0                    add      al, dh                         
  0x00CA50D6  44                      inc      esp                            
  0x00CA50D7  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA50DA  0000                    add      byte ptr [eax], al             
  0x00CA50DC  40                      inc      eax                            
  0x00CA50DD  0020                    add      byte ptr [eax], ah             
  0x00CA50DF  0000                    add      byte ptr [eax], al             
  0x00CA50E1  90                      nop                                     
  0x00CA50E2  2100                    and      dword ptr [eax], eax           
  0x00CA50E4  00e1                    add      cl, ah                         
  0x00CA50E6  44                      inc      esp                            
  0x00CA50E7  0000                    add      byte ptr [eax], al             
  0x00CA50E9  f4                      hlt                                     
  0x00CA50EA  46                      inc      esi                            
  0x00CA50EB  00ff                    add      bh, bh                         
  0x00CA50EE  7f00                    jg       0xca50f0                       
                                        ; XREF: 0x00CA50EE (cond_jump)
  0x00CA50F0  d0e0                    shl      al, 1                          
  0x00CA50F2  44                      inc      esp                            
  0x00CA50F3  004500                  add      byte ptr [ebp], al             
  0x00CA50F6  2000                    and      byte ptr [eax], al             
  0x00CA50F8  1b29                    sbb      ebp, dword ptr [ecx]           
  0x00CA50FA  2000                    and      byte ptr [eax], al             
  0x00CA50FC  00f4                    add      ah, dh                         
  0x00CA50FE  56                      push     esi                            
  0x00CA50FF  0026                    add      byte ptr [esi], ah             
  0x00CA5101  0900                    or       dword ptr [eax], eax           
  0x00CA5103  0000                    add      byte ptr [eax], al             
  0x00CA5106  44                      inc      esp                            
  0x00CA5107  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA510A  0000                    add      byte ptr [eax], al             
  0x00CA510C  40                      inc      eax                            
  0x00CA510D  0020                    add      byte ptr [eax], ah             
  0x00CA510F  0000                    add      byte ptr [eax], al             
  0x00CA5111  90                      nop                                     
  0x00CA5112  2100                    and      dword ptr [eax], eax           
  0x00CA5114  006055                  add      byte ptr [eax + 0x55], ah      
  0x00CA5117  0000                    add      byte ptr [eax], al             
  0x00CA511A  44                      inc      esp                            
  0x00CA511B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA511E  0000                    add      byte ptr [eax], al             
  0x00CA5120  00f4                    add      ah, dh                         
  0x00CA5122  46                      inc      esi                            
  0x00CA5123  0012                    add      byte ptr [edx], dl             
  0x00CA5125  0000                    add      byte ptr [eax], al             
  0x00CA5127  00d0                    add      al, dl                         
  0x00CA5129  f4                      hlt                                     
  0x00CA512A  44                      inc      esp                            
  0x00CA512B  00ae0500002e            add      byte ptr [esi + 0x2e000005], ch 
  0x00CA5131  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA5136  2000                    and      byte ptr [eax], al             
  0x00CA5138  009021000006            add      byte ptr [eax + 0x6000021], dl 
  0x00CA513E  3800                    cmp      byte ptr [eax], al             
  0x00CA5140  00f4                    add      ah, dh                         
  0x00CA5142  6200                    bound    eax, qword ptr [eax]           
  0x00CA5144  ac                      lodsb    al, byte ptr [esi]             
  0x00CA5145  0800                    or       byte ptr [eax], al             
  0x00CA5147  0000                    add      byte ptr [eax], al             
  0x00CA5149  0239                    add      bh, byte ptr [ecx]             
  0x00CA514B  001b                    add      byte ptr [ebx], bl             
  0x00CA514D  a6                      cmpsb    byte ptr [esi], byte ptr es:[edi] 
  0x00CA514E  2000                    and      byte ptr [eax], al             
  0x00CA5150  90                      nop                                     
  0x00CA5151  0306                    add      eax, dword ptr [esi]           
  0x00CA5153  000d00000000            add      byte ptr [0], cl               
  0x00CA5159  1122                    adc      dword ptr [edx], esp           
  0x00CA515B  0000                    add      byte ptr [eax], al             
  0x00CA515D  da4f00                  fimul    dword ptr [edi]                
  0x00CA5160  004920                  add      byte ptr [ecx + 0x20], cl      
  0x00CA5163  0000                    add      byte ptr [eax], al             
  0x00CA5165  d1440010                rol      dword ptr [eax + eax + 0x10], 1 
  0x00CA5169  c60600                  mov      byte ptr [esi], 0              
  0x00CA516C  0400                    add      al, 0                          
  0x00CA516E  0000                    add      byte ptr [eax], al             
  0x00CA5170  c0c944                  ror      cl, 0x44                       
  0x00CA5173  0045d1                  add      byte ptr [ebp - 0x2f], al      
  0x00CA5176  44                      inc      esp                            
  0x00CA5177  006870                  add      byte ptr [eax + 0x70], ch      
  0x00CA517A  0200                    add      al, byte ptr [eax]             
  0x00CA517C  00ce                    add      dh, cl                         
  0x00CA517E  2000                    and      byte ptr [eax], al             
  0x00CA5180  324820                  xor      cl, byte ptr [eax + 0x20]      
  0x00CA5183  0000                    add      byte ptr [eax], al             
  0x00CA5185  8621                    xchg     byte ptr [ecx], ah             
  0x00CA5187  0000                    add      byte ptr [eax], al             
  0x00CA518A  44                      inc      esp                            
  0x00CA518B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA518E  0000                    add      byte ptr [eax], al             
  0x00CA5190  00f4                    add      ah, dh                         
  0x00CA5192  46                      inc      esi                            
  0x00CA5193  0012                    add      byte ptr [edx], dl             
  0x00CA5195  0000                    add      byte ptr [eax], al             
  0x00CA5197  00d0                    add      al, dl                         
  0x00CA5199  f4                      hlt                                     
  0x00CA519A  44                      inc      esp                            
  0x00CA519B  00ae0500002e            add      byte ptr [esi + 0x2e000005], ch 
  0x00CA51A1  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA51A6  2000                    and      byte ptr [eax], al             
  0x00CA51A8  009021000002            add      byte ptr [eax + 0x2000021], dl 
  0x00CA51AE  3800                    cmp      byte ptr [eax], al             
  0x00CA51B0  00f4                    add      ah, dh                         
  0x00CA51B2  44                      inc      esp                            
  0x00CA51B3  0000                    add      byte ptr [eax], al             
  0x00CA51B5  3200                    xor      al, byte ptr [eax]             
  0x00CA51B7  0000                    add      byte ptr [eax], al             
  0x00CA51B9  e856004500              call     0x10f5214                      
  0x00CA51BE  2000                    and      byte ptr [eax], al             
  0x00CA51C0  1b29                    sbb      ebp, dword ptr [ecx]           
  0x00CA51C2  2000                    and      byte ptr [eax], al             
  0x00CA51C4  00f0                    add      al, dh                         
  0x00CA51C6  44                      inc      esp                            
  0x00CA51C7  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA51CA  0000                    add      byte ptr [eax], al             
  0x00CA51CC  00f4                    add      ah, dh                         
  0x00CA51CE  46                      inc      esi                            
  0x00CA51CF  0002                    add      byte ptr [edx], al             
  0x00CA51D1  0000                    add      byte ptr [eax], al             
  0x00CA51D3  00d0                    add      al, dl                         
  0x00CA51D5  f4                      hlt                                     
  0x00CA51D6  44                      inc      esp                            
  0x00CA51D7  001a                    add      byte ptr [edx], bl             
  0x00CA51D9  0900                    or       dword ptr [eax], eax           
  0x00CA51DB  002e                    add      byte ptr [esi], ch             
  0x00CA51DD  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA51E2  2000                    and      byte ptr [eax], al             
  0x00CA51E4  0091210000f4            add      byte ptr [ecx - 0xbffffdf], dl 
  0x00CA51EA  56                      push     esi                            
  0x00CA51EB  0008                    add      byte ptr [eax], cl             
  0x00CA51ED  06                      push     es                             
  0x00CA51EE  0000                    add      byte ptr [eax], al             
  0x00CA51F0  00f0                    add      al, dh                         
  0x00CA51F2  44                      inc      esp                            
  0x00CA51F3  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA51F6  0000                    add      byte ptr [eax], al             
  0x00CA51F8  40                      inc      eax                            
  0x00CA51F9  0020                    add      byte ptr [eax], ah             
  0x00CA51FB  0000                    add      byte ptr [eax], al             
  0x00CA51FD  90                      nop                                     
  0x00CA51FE  2100                    and      dword ptr [eax], eax           
  0x00CA5200  00e1                    add      cl, ah                         
  0x00CA5202  44                      inc      esp                            
  0x00CA5203  0000                    add      byte ptr [eax], al             
  0x00CA5205  f4                      hlt                                     
  0x00CA5206  46                      inc      esi                            
  0x00CA5207  0000                    add      byte ptr [eax], al             
  0x00CA5209  004000                  add      byte ptr [eax], al             
  0x00CA520C  d0e0                    shl      al, 1                          
  0x00CA520E  46                      inc      esi                            
  0x00CA520F  005560                  add      byte ptr [ebp + 0x60], dl      
  0x00CA5212  44                      inc      esp                            
  0x00CA5213  001b                    add      byte ptr [ebx], bl             
  0x00CA5215  2920                    sub      dword ptr [eax], esp           
  0x00CA5217  0000                    add      byte ptr [eax], al             
  0x00CA5219  f4                      hlt                                     
  0x00CA521A  56                      push     esi                            
  0x00CA521B  007f0b                  add      byte ptr [edi + 0xb], bh       
  0x00CA521E  0000                    add      byte ptr [eax], al             
  0x00CA5220  00f0                    add      al, dh                         
  0x00CA5222  44                      inc      esp                            
  0x00CA5223  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA5226  0000                    add      byte ptr [eax], al             
  0x00CA5228  40                      inc      eax                            
  0x00CA5229  0020                    add      byte ptr [eax], ah             
  0x00CA522B  0000                    add      byte ptr [eax], al             
  0x00CA522D  90                      nop                                     
  0x00CA522E  2100                    and      dword ptr [eax], eax           
  0x00CA5230  006055                  add      byte ptr [eax + 0x55], ah      
  0x00CA5233  0000                    add      byte ptr [eax], al             
  0x00CA5236  44                      inc      esp                            
  0x00CA5237  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA523A  0000                    add      byte ptr [eax], al             
  0x00CA523C  00f4                    add      ah, dh                         
  0x00CA523E  46                      inc      esi                            
  0x00CA523F  0012                    add      byte ptr [edx], dl             
  0x00CA5241  0000                    add      byte ptr [eax], al             
  0x00CA5243  00d0                    add      al, dl                         
  0x00CA5245  f4                      hlt                                     
  0x00CA5246  44                      inc      esp                            
  0x00CA5247  00ae0500002e            add      byte ptr [esi + 0x2e000005], ch 
  0x00CA524D  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA5252  2000                    and      byte ptr [eax], al             
  0x00CA5254  009021000006            add      byte ptr [eax + 0x6000021], dl 
  0x00CA525A  3800                    cmp      byte ptr [eax], al             
  0x00CA525C  00ae20009003            add      byte ptr [esi + 0x3900020], ch 
  0x00CA5262  06                      push     es                             
  0x00CA5263  000a                    add      byte ptr [edx], cl             
  0x00CA5265  0000                    add      byte ptr [eax], al             
  0x00CA5267  0000                    add      byte ptr [eax], al             
  0x00CA5269  1122                    adc      dword ptr [edx], esp           
  0x00CA526B  0000                    add      byte ptr [eax], al             
  0x00CA526D  99                      cdq                                     
  0x00CA526E  2100                    and      dword ptr [eax], eax           
  0x00CA5270  0012                    add      byte ptr [edx], dl             
  0x00CA5272  2200                    and      al, byte ptr [eax]             
  0x00CA5274  004920                  add      byte ptr [ecx + 0x20], cl      
  0x00CA5277  0000                    add      byte ptr [eax], al             
  0x00CA5279  d9440000                fld      dword ptr [eax + eax]          
  0x00CA527D  5a                      pop      edx                            
  0x00CA527E  44                      inc      esp                            
  0x00CA527F  0000                    add      byte ptr [eax], al             
  0x00CA5281  d9440000                fld      dword ptr [eax + eax]          
  0x00CA5285  5a                      pop      edx                            
  0x00CA5286  44                      inc      esp                            
  0x00CA5287  0032                    add      byte ptr [edx], dh             
  0x00CA5289  48                      dec      eax                            
  0x00CA528A  2000                    and      byte ptr [eax], al             
  0x00CA528C  0c00                    or       al, 0                          
  0x00CA528E  0000                    add      byte ptr [eax], al             
  0x00CA5290  00f4                    add      ah, dh                         
  0x00CA5292  56                      push     esi                            
  0x00CA5293  007f0b                  add      byte ptr [edi + 0xb], bh       
  0x00CA5296  0000                    add      byte ptr [eax], al             
  0x00CA5298  00f0                    add      al, dh                         
  0x00CA529A  44                      inc      esp                            
  0x00CA529B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA529E  0000                    add      byte ptr [eax], al             
  0x00CA52A0  40                      inc      eax                            
  0x00CA52A1  0020                    add      byte ptr [eax], ah             
  0x00CA52A3  0000                    add      byte ptr [eax], al             
  0x00CA52A5  90                      nop                                     
  0x00CA52A6  2100                    and      dword ptr [eax], eax           
  0x00CA52A8  00f4                    add      ah, dh                         
  0x00CA52AA  56                      push     esi                            
  0x00CA52AB  000d06000000            add      byte ptr [6], cl               
  0x00CA52B2  44                      inc      esp                            
  0x00CA52B3  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA52B6  0000                    add      byte ptr [eax], al             
  0x00CA52B8  40                      inc      eax                            
  0x00CA52B9  0020                    add      byte ptr [eax], ah             
  0x00CA52BB  0000                    add      byte ptr [eax], al             
  0x00CA52BD  91                      xchg     ecx, eax                       
  0x00CA52BE  2100                    and      dword ptr [eax], eax           
  0x00CA52C0  00f4                    add      ah, dh                         
  0x00CA52C2  56                      push     esi                            
  0x00CA52C3  00840b000000f0          add      byte ptr [ebx + ecx - 0x10000000], al 
  0x00CA52CA  44                      inc      esp                            
  0x00CA52CB  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA52CE  0000                    add      byte ptr [eax], al             
  0x00CA52D0  40                      inc      eax                            
  0x00CA52D1  0020                    add      byte ptr [eax], ah             
  0x00CA52D3  0000                    add      byte ptr [eax], al             
  0x00CA52D5  92                      xchg     edx, eax                       
  0x00CA52D6  2100                    and      dword ptr [eax], eax           
  0x00CA52D8  1be0                    sbb      esp, eax                       
  0x00CA52DA  44                      inc      esp                            
  0x00CA52DB  0000                    add      byte ptr [eax], al             
  0x00CA52DD  e156                    loope    0xca5335                       
  0x00CA52DF  0042f4                  add      byte ptr [edx - 0xc], al       
  0x00CA52E2  45                      inc      ebp                            
  0x00CA52E3  0001                    add      byte ptr [ecx], al             
  0x00CA52E5  0000                    add      byte ptr [eax], al             
  0x00CA52E7  0068a0                  add      byte ptr [eax - 0x60], ch      
  0x00CA52EA  0200                    add      al, byte ptr [eax]             
  0x00CA52EC  006257                  add      byte ptr [edx + 0x57], ah      
  0x00CA52EF  0000                    add      byte ptr [eax], al             
  0x00CA52F1  61                      popal                                   
  0x00CA52F2  44                      inc      esp                            
  0x00CA52F3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA52F6  0000                    add      byte ptr [eax], al             
  0x00CA52F8  00f4                    add      ah, dh                         
  0x00CA52FA  60                      pushal                                  
  0x00CA52FB  00fd                    add      ch, bh                         
  0x00CA52FD  0200                    add      al, byte ptr [eax]             
  0x00CA52FF  0000                    add      byte ptr [eax], al             
  0x00CA5302  44                      inc      esp                            
  0x00CA5303  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA5306  0000                    add      byte ptr [eax], al             
  0x00CA5308  00f4                    add      ah, dh                         
  0x00CA530A  46                      inc      esi                            
  0x00CA530B  0080000000d0            add      byte ptr [eax - 0x30000000], al 
  0x00CA5311  f4                      hlt                                     
  0x00CA5312  44                      inc      esp                            
  0x00CA5313  0012                    add      byte ptr [edx], dl             
  0x00CA5315  06                      push     es                             
  0x00CA5316  0000                    add      byte ptr [eax], al             
  0x00CA5318  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA531E  2000                    and      byte ptr [eax], al             
  0x00CA5320  0091210000f4            add      byte ptr [ecx - 0xbffffdf], dl 
  0x00CA5326  6400b208000000          add      byte ptr fs:[edx + 8], dh      
  0x00CA532D  f4                      hlt                                     
  0x00CA532E  650000                  add      byte ptr gs:[eax], al          
  0x00CA5331  0000                    add      byte ptr [eax], al             
  0x00CA5333  00fc                    add      ah, bh                         
                                        ; XREF: 0x00CA52DD (cond_jump)
  0x00CA5335  040d                    add      al, 0xd                        
  0x00CA5337  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA533A  0000                    add      byte ptr [eax], al             
  0x00CA533C  00f0                    add      al, dh                         
  0x00CA533E  56                      push     esi                            
  0x00CA533F  002b                    add      byte ptr [ebx], ch             
  0x00CA5341  0900                    or       dword ptr [eax], eax           
  0x00CA5343  0003                    add      byte ptr [ebx], al             
  0x00CA5345  0020                    add      byte ptr [eax], ah             
  0x00CA5347  000e                    add      byte ptr [esi], cl             
  0x00CA5349  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA534A  050000f460              add      eax, 0x60f40000                
  0x00CA534F  0000                    add      byte ptr [eax], al             
  0x00CA5351  0000                    add      byte ptr [eax], al             
  0x00CA5353  0000                    add      byte ptr [eax], al             
  0x00CA5355  f4                      hlt                                     
  0x00CA5356  61                      popal                                   
  0x00CA5357  004000                  add      byte ptr [eax], al             
  0x00CA535A  0000                    add      byte ptr [eax], al             
  0x00CA535C  001422                  add      byte ptr [edx], dl             
  0x00CA535F  0000                    add      byte ptr [eax], al             
  0x00CA5361  35220000f4              xor      eax, 0xf4000022                
  0x00CA5366  6200                    bound    eax, qword ptr [eax]           
  0x00CA5368  b20a                    mov      dl, 0xa                        
  0x00CA536A  0000                    add      byte ptr [eax], al             
  0x00CA536C  00f4                    add      ah, dh                         
  0x00CA536E  6600f2                  add      dl, dh                         
  0x00CA5371  0a00                    or       al, byte ptr [eax]             
  0x00CA5373  0000                    add      byte ptr [eax], al             
  0x00CA5375  3f                      aas                                     
  0x00CA5376  3a00                    cmp      al, byte ptr [eax]             
  0x00CA5378  4d                      dec      ebp                            
  0x00CA5379  050d000a0c              add      eax, 0xc0a000d                 
  0x00CA537E  050000f460              add      eax, 0x60f40000                
  0x00CA5383  0000                    add      byte ptr [eax], al             
  0x00CA5385  0000                    add      byte ptr [eax], al             
  0x00CA5387  0000                    add      byte ptr [eax], al             
  0x00CA5389  1422                    adc      al, 0x22                       
  0x00CA538B  0000                    add      byte ptr [eax], al             
  0x00CA538D  f4                      hlt                                     
  0x00CA538E  6200                    bound    eax, qword ptr [eax]           
  0x00CA5390  b209                    mov      dl, 9                          
  0x00CA5392  0000                    add      byte ptr [eax], al             
  0x00CA5394  00f4                    add      ah, dh                         
  0x00CA5396  660032                  add      byte ptr [edx], dh             
  0x00CA5399  0a00                    or       al, byte ptr [eax]             
  0x00CA539B  0000                    add      byte ptr [eax], al             
  0x00CA539D  7f3a                    jg       0xca53d9                       
  0x00CA539F  003e                    add      byte ptr [esi], bh             
  0x00CA53A1  050d000c00              add      eax, 0xc000d                   
  0x00CA53A6  0000                    add      byte ptr [eax], al             
  0x00CA53A8  a0000500a0              mov      al, byte ptr [0xa0000500]      
  0x00CA53AD  61                      popal                                   
  0x00CA53AE  0400                    add      al, 0                          
  0x00CA53B0  00f0                    add      al, dh                         
  0x00CA53B2  56                      push     esi                            
  0x00CA53B3  002b                    add      byte ptr [ebx], ch             
  0x00CA53B5  0900                    or       dword ptr [eax], eax           
  0x00CA53B7  0003                    add      byte ptr [ebx], al             
  0x00CA53B9  0020                    add      byte ptr [eax], ah             
  0x00CA53BB  0015a4050000            add      byte ptr [0x5a4], dl           
  0x00CA53C1  f4                      hlt                                     
  0x00CA53C2  60                      pushal                                  
  0x00CA53C3  0000                    add      byte ptr [eax], al             
  0x00CA53C5  0000                    add      byte ptr [eax], al             
  0x00CA53C7  0000                    add      byte ptr [eax], al             
  0x00CA53C9  f4                      hlt                                     
  0x00CA53CA  61                      popal                                   
  0x00CA53CB  004000                  add      byte ptr [eax], al             
  0x00CA53CE  0000                    add      byte ptr [eax], al             
  0x00CA53D0  00f4                    add      ah, dh                         
  0x00CA53D2  6400fd                  add      ch, bh                         
  0x00CA53D5  0300                    add      eax, dword ptr [eax]           
  0x00CA53D7  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA539D (cond_jump)
  0x00CA53D9  f4                      hlt                                     
  0x00CA53DA  6500fc                  add      ah, bh                         
  0x00CA53DD  0400                    add      al, 0                          
  0x00CA53DF  0000                    add      byte ptr [eax], al             
  0x00CA53E1  f4                      hlt                                     
  0x00CA53E2  6200                    bound    eax, qword ptr [eax]           
  0x00CA53E4  b20a                    mov      dl, 0xa                        
  0x00CA53E6  0000                    add      byte ptr [eax], al             
  0x00CA53E8  00f4                    add      ah, dh                         
  0x00CA53EA  6600f2                  add      dl, dh                         
  0x00CA53ED  0a00                    or       al, byte ptr [eax]             
  0x00CA53EF  0000                    add      byte ptr [eax], al             
  0x00CA53F1  2038                    and      byte ptr [eax], bh             
  0x00CA53F3  0000                    add      byte ptr [eax], al             
  0x00CA53F5  1923                    sbb      dword ptr [ebx], esp           
  0x00CA53F7  0000                    add      byte ptr [eax], al             
  0x00CA53F9  3f                      aas                                     
  0x00CA53FA  3a00                    cmp      al, byte ptr [eax]             
  0x00CA53FC  0003                    add      byte ptr [ebx], al             
  0x00CA53FE  3c00                    cmp      al, 0                          
  0x00CA5400  00f4                    add      ah, dh                         
  0x00CA5402  7500                    jne      0xca5404                       
                                        ; XREF: 0x00CA5402 (cond_jump)
  0x00CA5404  fd                      std                                     
  0x00CA5406  ff00                    inc      dword ptr [eax]                
  0x00CA5408  7305                    jae      0xca540f                       
  0x00CA540A  0d00110c05              or       eax, 0x50c1100                 
                                        ; XREF: 0x00CA5408 (cond_jump)
  0x00CA540F  0000                    add      byte ptr [eax], al             
  0x00CA5411  f4                      hlt                                     
  0x00CA5412  60                      pushal                                  
  0x00CA5413  0000                    add      byte ptr [eax], al             
  0x00CA5415  0000                    add      byte ptr [eax], al             
  0x00CA5417  0000                    add      byte ptr [eax], al             
  0x00CA5419  f4                      hlt                                     
  0x00CA541A  6400fd                  add      ch, bh                         
  0x00CA541D  0300                    add      eax, dword ptr [eax]           
  0x00CA541F  0000                    add      byte ptr [eax], al             
  0x00CA5421  f4                      hlt                                     
  0x00CA5422  6500fc                  add      ah, bh                         
  0x00CA5425  0400                    add      al, 0                          
  0x00CA5427  0000                    add      byte ptr [eax], al             
  0x00CA5429  f4                      hlt                                     
  0x00CA542A  6200                    bound    eax, qword ptr [eax]           
  0x00CA542C  b209                    mov      dl, 9                          
  0x00CA542E  0000                    add      byte ptr [eax], al             
  0x00CA5430  00f4                    add      ah, dh                         
  0x00CA5432  660032                  add      byte ptr [edx], dh             
  0x00CA5435  0a00                    or       al, byte ptr [eax]             
  0x00CA5437  0000                    add      byte ptr [eax], al             
  0x00CA5439  40                      inc      eax                            
  0x00CA543A  3800                    cmp      byte ptr [eax], al             
  0x00CA543C  007f3a                  add      byte ptr [edi + 0x3a], bh      
  0x00CA543F  0000                    add      byte ptr [eax], al             
  0x00CA5441  023c00                  add      bh, byte ptr [eax + eax]       
  0x00CA5444  00f4                    add      ah, dh                         
  0x00CA5446  7500                    jne      0xca5448                       
  0x00CA544A  ff00                    inc      dword ptr [eax]                
  0x00CA544C  64050d0020f4            add      eax, 0xf420000d                
  0x00CA5452  0500ffff00              add      eax, 0xffff00                  
  0x00CA5457  00a061040000            add      byte ptr [eax + 0x461], ah     
  0x00CA545E  56                      push     esi                            
  0x00CA545F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA5462  0000                    add      byte ptr [eax], al             
  0x00CA5464  00f0                    add      al, dh                         
  0x00CA5466  44                      inc      esp                            
  0x00CA5467  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA546D  f4                      hlt                                     
  0x00CA546E  60                      pushal                                  
  0x00CA546F  00fd                    add      ch, bh                         
  0x00CA5471  0300                    add      eax, dword ptr [eax]           
  0x00CA5473  0008                    add      byte ptr [eax], cl             
  0x00CA5475  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA5476  050000f461              add      eax, 0x61f40000                
  0x00CA547B  008001000090            add      byte ptr [eax - 0x6fffffff], al 
  0x00CA5481  b506                    mov      ch, 6                          
  0x00CA5483  0003                    add      byte ptr [ebx], al             
  0x00CA5485  0000                    add      byte ptr [eax], al             
  0x00CA5487  0000                    add      byte ptr [eax], al             
  0x00CA5489  d8440000                fadd     dword ptr [eax + eax]          
  0x00CA548D  59                      pop      ecx                            
  0x00CA548E  44                      inc      esp                            
  0x00CA548F  0007                    add      byte ptr [edi], al             
  0x00CA5491  0c05                    or       al, 5                          
  0x00CA5493  0000                    add      byte ptr [eax], al             
  0x00CA5495  f4                      hlt                                     
  0x00CA5496  61                      popal                                   
  0x00CA5497  003502000090            add      byte ptr [0x90000002], dh      
  0x00CA549D  07                      pop      es                             
  0x00CA549E  06                      push     es                             
  0x00CA549F  0003                    add      byte ptr [ebx], al             
  0x00CA54A1  0000                    add      byte ptr [eax], al             
  0x00CA54A3  0000                    add      byte ptr [eax], al             
  0x00CA54A5  d8440000                fadd     dword ptr [eax + eax]          
  0x00CA54A9  59                      pop      ecx                            
  0x00CA54AA  44                      inc      esp                            
  0x00CA54AB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA54AE  0000                    add      byte ptr [eax], al             
  0x00CA54B0  00f0                    add      al, dh                         
  0x00CA54B2  56                      push     esi                            
  0x00CA54B3  002b                    add      byte ptr [ebx], ch             
  0x00CA54B5  0900                    or       dword ptr [eax], eax           
  0x00CA54B7  0003                    add      byte ptr [ebx], al             
  0x00CA54B9  0020                    add      byte ptr [eax], ah             
  0x00CA54BB  000a                    add      byte ptr [edx], cl             
  0x00CA54BD  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA54BE  050000f460              add      eax, 0x60f40000                
  0x00CA54C3  0000                    add      byte ptr [eax], al             
  0x00CA54C5  0000                    add      byte ptr [eax], al             
  0x00CA54C7  0000                    add      byte ptr [eax], al             
  0x00CA54C9  40                      inc      eax                            
  0x00CA54CA  3800                    cmp      byte ptr [eax], al             
  0x00CA54CC  ef                      out      dx, eax                        
  0x00CA54CD  030d0000f460            add      ecx, dword ptr [0x60f40000]    
  0x00CA54D3  004000                  add      byte ptr [eax], al             
  0x00CA54D6  0000                    add      byte ptr [eax], al             
  0x00CA54D8  004038                  add      byte ptr [eax + 0x38], al      
  0x00CA54DB  00ef                    add      bh, ch                         
  0x00CA54DD  030d00050c05            add      ecx, dword ptr [0x50c0500]     
  0x00CA54E3  0000                    add      byte ptr [eax], al             
  0x00CA54E5  f4                      hlt                                     
  0x00CA54E6  60                      pushal                                  
  0x00CA54E7  0000                    add      byte ptr [eax], al             
  0x00CA54E9  0000                    add      byte ptr [eax], al             
  0x00CA54EB  0000                    add      byte ptr [eax], al             
  0x00CA54ED  803800                  cmp      byte ptr [eax], 0              
  0x00CA54F0  ef                      out      dx, eax                        
  0x00CA54F1  030d000c0000            add      ecx, dword ptr [0xc00]         
  0x00CA54F7  0000                    add      byte ptr [eax], al             
  0x00CA54FA  56                      push     esi                            
  0x00CA54FB  00970b000000            add      byte ptr [edi + 0xb], dl       
  0x00CA5502  44                      inc      esp                            
  0x00CA5503  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA5506  0000                    add      byte ptr [eax], al             
  0x00CA5508  45                      inc      ebp                            
  0x00CA5509  f4                      hlt                                     
  0x00CA550A  45                      inc      ebp                            
  0x00CA550B  0001                    add      byte ptr [ecx], al             
  0x00CA550D  0000                    add      byte ptr [eax], al             
  0x00CA550F  0003                    add      byte ptr [ebx], al             
  0x00CA5511  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA5512  05000d0805              add      eax, 0x5080d00                 
  0x00CA5517  0003                    add      byte ptr [ebx], al             
  0x00CA5519  0c05                    or       al, 5                          
  0x00CA551B  0000                    add      byte ptr [eax], al             
  0x00CA551D  7045                    jo       0xca5564                       
  0x00CA551F  008f0b000000            add      byte ptr [edi + 0xb], cl       
  0x00CA5525  f4                      hlt                                     
  0x00CA5526  60                      pushal                                  
  0x00CA5527  007f0b                  add      byte ptr [edi + 0xb], bh       
  0x00CA552A  0000                    add      byte ptr [eax], al             
  0x00CA552C  00f4                    add      ah, dh                         
  0x00CA552E  61                      popal                                   
  0x00CA552F  00a305000090            add      byte ptr [ebx - 0x6ffffffb], ah 
  0x00CA5535  0506000300              add      eax, 0x30006                   
  0x00CA553A  0000                    add      byte ptr [eax], al             
  0x00CA553C  00d8                    add      al, bl                         
  0x00CA553E  44                      inc      esp                            
  0x00CA553F  0000                    add      byte ptr [eax], al             
  0x00CA5541  59                      pop      ecx                            
  0x00CA5542  44                      inc      esp                            
  0x00CA5543  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA5546  0000                    add      byte ptr [eax], al             
  0x00CA5548  0003                    add      byte ptr [ebx], al             
  0x00CA554A  2900                    sub      dword ptr [eax], eax           
  0x00CA554C  00f0                    add      al, dh                         
  0x00CA554E  7000                    jo       0xca5550                       
                                        ; XREF: 0x00CA554E (cond_jump)
  0x00CA5550  41                      inc      ecx                            
  0x00CA5551  0b00                    or       eax, dword ptr [eax]           
  0x00CA5553  0000                    add      byte ptr [eax], al             
  0x00CA5555  f4                      hlt                                     
  0x00CA5556  60                      pushal                                  
  0x00CA5557  007f0b                  add      byte ptr [edi + 0xb], bh       
  0x00CA555A  0000                    add      byte ptr [eax], al             
  0x00CA555C  00e8                    add      al, ch                         
  0x00CA555E  56                      push     esi                            
  0x00CA555F  008541010010            add      byte ptr [ebp + 0x10000141], al 
  0x00CA5565  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA5566  050000f460              add      eax, 0x60f40000                
  0x00CA556B  0026                    add      byte ptr [esi], ah             
  0x00CA556D  0900                    or       dword ptr [eax], eax           
  0x00CA556F  0000                    add      byte ptr [eax], al             
  0x00CA5571  e856008541              call     0x424f55cc                     
  0x00CA5576  0100                    add      dword ptr [eax], eax           
  0x00CA5578  0ba4050000f056          or       esp, dword ptr [ebp + eax + 0x56f00000] 
  0x00CA557F  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA5582  0000                    add      byte ptr [eax], al             
  0x00CA5584  854001                  test     dword ptr [eax + 1], eax       
  0x00CA5587  0006                    add      byte ptr [esi], al             
  0x00CA5589  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA558A  050000f460              add      eax, 0x60f40000                
  0x00CA558F  00a305000000            add      byte ptr [ebx + 5], ah         
  0x00CA5595  e856008541              call     0x424f55f0                     
  0x00CA559A  0100                    add      dword ptr [eax], eax           
  0x00CA559C  02a40500000229          add      ah, byte ptr [ebp + eax + 0x29020000] 
  0x00CA55A3  0000                    add      byte ptr [eax], al             
  0x00CA55A5  f4                      hlt                                     
  0x00CA55A6  60                      pushal                                  
  0x00CA55A7  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA55AD  6851000c00              push     0xc0051                        
  0x00CA55B2  0000                    add      byte ptr [eax], al             
  0x00CA55B4  0018                    add      byte ptr [eax], bl             
  0x00CA55B6  3d0000f044              cmp      eax, 0x44f00000                
  0x00CA55BB  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA55BE  0000                    add      byte ptr [eax], al             
  0x00CA55C0  00f0                    add      al, dh                         
  0x00CA55C2  56                      push     esi                            
  0x00CA55C3  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA55C9  f4                      hlt                                     
  0x00CA55CA  60                      pushal                                  
  0x00CA55CB  00800100000c            add      byte ptr [eax + 0xc000001], al 
  0x00CA55D1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA55D2  0500009620              add      eax, 0x20960000                
  0x00CA55D7  0000                    add      byte ptr [eax], al             
  0x00CA55D9  f4                      hlt                                     
  0x00CA55DA  61                      popal                                   
  0x00CA55DB  004102                  add      byte ptr [ecx + 2], al         
  0x00CA55DE  0000                    add      byte ptr [eax], al             
  0x00CA55E0  00f4                    add      ah, dh                         
  0x00CA55E2  56                      push     esi                            
  0x00CA55E3  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA55E9  c422                    les      esp, ptr [edx]                 
  0x00CA55EB  004000                  add      byte ptr [eax], al             
  0x00CA55EE  2000                    and      byte ptr [eax], al             
  0x00CA55F0  0092210000e2            add      byte ptr [edx - 0x1dffffdf], dl 
  0x00CA55F6  7100                    jno      0xca55f8                       
                                        ; XREF: 0x00CA55F6 (cond_jump)
  0x00CA55F8  8b050d000a0c            mov      eax, dword ptr [0xc0a000d]     
  0x00CA55FE  050000f056              add      eax, 0x56f00000                
  0x00CA5603  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA5606  0000                    add      byte ptr [eax], al             
  0x00CA5608  03f4                    add      esi, esp                       
  0x00CA560A  60                      pushal                                  
  0x00CA560B  003502000005            add      byte ptr [0x5000002], dh       
  0x00CA5611  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA5612  050000f461              add      eax, 0x61f40000                
  0x00CA5617  00f6                    add      dh, dh                         
  0x00CA5619  0200                    add      al, byte ptr [eax]             
  0x00CA561B  0000                    add      byte ptr [eax], al             
  0x00CA561D  07                      pop      es                             
  0x00CA561E  3900                    cmp      dword ptr [eax], eax           
  0x00CA5620  8b050d000c00            mov      eax, dword ptr [0xc000d]       
  0x00CA5626  0000                    add      byte ptr [eax], al             
  0x00CA5628  00f0                    add      al, dh                         
  0x00CA562A  44                      inc      esp                            
  0x00CA562B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA562E  0000                    add      byte ptr [eax], al             
  0x00CA5630  00f0                    add      al, dh                         
  0x00CA5632  56                      push     esi                            
  0x00CA5633  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA5639  f4                      hlt                                     
  0x00CA563A  61                      popal                                   
  0x00CA563B  004102                  add      byte ptr [ecx + 2], al         
  0x00CA563E  0000                    add      byte ptr [eax], al             
  0x00CA5640  59                      pop      ecx                            
  0x00CA5641  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA5642  050000f456              add      eax, 0x56f40000                
  0x00CA5647  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA564E  44                      inc      esp                            
  0x00CA564F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA5652  0000                    add      byte ptr [eax], al             
  0x00CA5654  40                      inc      eax                            
  0x00CA5655  0020                    add      byte ptr [eax], ah             
  0x00CA5657  0000                    add      byte ptr [eax], al             
  0x00CA5659  90                      nop                                     
  0x00CA565A  2100                    and      dword ptr [eax], eax           
  0x00CA565C  00f4                    add      ah, dh                         
  0x00CA565E  56                      push     esi                            
  0x00CA565F  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA5666  44                      inc      esp                            
  0x00CA5667  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA566A  0000                    add      byte ptr [eax], al             
  0x00CA566C  40                      inc      eax                            
  0x00CA566D  0020                    add      byte ptr [eax], ah             
  0x00CA566F  0000                    add      byte ptr [eax], al             
  0x00CA5671  92                      xchg     edx, eax                       
  0x00CA5672  2100                    and      dword ptr [eax], eax           
  0x00CA5674  00e0                    add      al, ah                         
  0x00CA5676  56                      push     esi                            
  0x00CA5677  0000                    add      byte ptr [eax], al             
  0x00CA5679  e271                    loop     0xca56ec                       
  0x00CA567B  0000                    add      byte ptr [eax], al             
  0x00CA567D  94                      xchg     esp, eax                       
  0x00CA567E  2100                    and      dword ptr [eax], eax           
  0x00CA5680  0036                    add      byte ptr [esi], dh             
  0x00CA5682  2200                    and      al, byte ptr [eax]             
  0x00CA5684  00f4                    add      ah, dh                         
  0x00CA5686  56                      push     esi                            
  0x00CA5687  005c0b00                add      byte ptr [ebx + ecx], bl       
  0x00CA568B  0000                    add      byte ptr [eax], al             
  0x00CA568E  44                      inc      esp                            
  0x00CA568F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA5692  0000                    add      byte ptr [eax], al             
  0x00CA5694  40                      inc      eax                            
  0x00CA5695  0020                    add      byte ptr [eax], ah             
  0x00CA5697  0000                    add      byte ptr [eax], al             
  0x00CA5699  90                      nop                                     
  0x00CA569A  2100                    and      dword ptr [eax], eax           
  0x00CA569C  00f4                    add      ah, dh                         
  0x00CA569E  56                      push     esi                            
  0x00CA569F  00910b000000            add      byte ptr [ecx + 0xb], dl       
  0x00CA56A6  44                      inc      esp                            
  0x00CA56A7  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA56AA  0000                    add      byte ptr [eax], al             
  0x00CA56AC  40                      inc      eax                            
  0x00CA56AD  0020                    add      byte ptr [eax], ah             
  0x00CA56AF  0000                    add      byte ptr [eax], al             
  0x00CA56B1  92                      xchg     edx, eax                       
  0x00CA56B2  2100                    and      dword ptr [eax], eax           
  0x00CA56B4  002e                    add      byte ptr [esi], ch             
  0x00CA56B6  2300                    and      eax, dword ptr [eax]           
  0x00CA56B8  844101                  test     byte ptr [ecx + 1], al         
  0x00CA56BB  00c4                    add      ah, al                         
  0x00CA56BD  740b                    je       0xca56ca                       
  0x00CA56BF  0016                    add      byte ptr [esi], dl             
  0x00CA56C1  0f0000                  sldt     word ptr [eax]                 
  0x00CA56C4  00852100adf4            add      byte ptr [ebp - 0xb52ffdf], al 
                                        ; XREF: 0x00CA56BD (cond_jump)
  0x00CA56CA  47                      inc      edi                            
  0x00CA56CB  0001                    add      byte ptr [ecx], al             
  0x00CA56CD  0000                    add      byte ptr [eax], al             
  0x00CA56CF  00c4                    add      ah, al                         
  0x00CA56D1  740b                    je       0xca56de                       
  0x00CA56D3  0012                    add      byte ptr [edx], dl             
  0x00CA56D5  0f0000                  sldt     word ptr [eax]                 
  0x00CA56D8  00e6                    add      dh, ah                         
  0x00CA56DA  2100                    and      dword ptr [eax], eax           
  0x00CA56DC  d09d20002e1d            rcr      byte ptr [ebp + 0x1d2e0020], 1 
  0x00CA56E2  0c00                    or       al, 0                          
  0x00CA56E4  65f4                    hlt                                     
  0x00CA56E6  46                      inc      esi                            
  0x00CA56E7  00abaa2a0078            add      byte ptr [ebx + 0x78002aaa], ch 
  0x00CA56ED  2920                    sub      dword ptr [eax], esp           
  0x00CA56EF  0000                    add      byte ptr [eax], al             
  0x00CA56F1  60                      pushal                                  
  0x00CA56F2  55                      push     ebp                            
  0x00CA56F3  0000                    add      byte ptr [eax], al             
  0x00CA56F5  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00CA56F6  2100                    and      dword ptr [eax], eax           
  0x00CA56F8  e9bc210082              jmp      0x82ca78b9                     
  0x00CA56FD  1d0c001000              sbb      eax, 0x10000c                  
  0x00CA5702  2000                    and      byte ptr [eax], al             
  0x00CA5704  650020                  add      byte ptr gs:[eax], ah          
  0x00CA5707  007829                  add      byte ptr [eax + 0x29], bh      
  0x00CA570A  2000                    and      byte ptr [eax], al             
  0x00CA570C  006255                  add      byte ptr [edx + 0x55], ah      
  0x00CA570F  0000                    add      byte ptr [eax], al             
  0x00CA5711  3222                    xor      ah, byte ptr [edx]             
  0x00CA5713  0000                    add      byte ptr [eax], al             
  0x00CA5715  59                      pop      ecx                            
  0x00CA5716  2000                    and      byte ptr [eax], al             
  0x00CA5718  0000                    add      byte ptr [eax], al             
  0x00CA571A  3a00                    cmp      al, byte ptr [eax]             
  0x00CA571C  96                      xchg     esi, eax                       
  0x00CA571D  050d00110c              add      eax, 0xc11000d                 
  0x00CA5722  050000f056              add      eax, 0x56f00000                
  0x00CA5727  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA572A  0000                    add      byte ptr [eax], al             
  0x00CA572C  03f4                    add      esi, esp                       
  0x00CA572E  60                      pushal                                  
  0x00CA572F  008f0b00000c            add      byte ptr [edi + 0xc00000b], cl 
  0x00CA5735  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA5736  050000f461              add      eax, 0x61f40000                
  0x00CA573B  00f6                    add      dh, dh                         
  0x00CA573D  0200                    add      al, byte ptr [eax]             
  0x00CA573F  0000                    add      byte ptr [eax], al             
  0x00CA5741  07                      pop      es                             
  0x00CA5742  3900                    cmp      dword ptr [eax], eax           
  0x00CA5744  0036                    add      byte ptr [esi], dh             
  0x00CA5746  2200                    and      al, byte ptr [eax]             
  0x00CA5748  0032                    add      byte ptr [edx], dh             
  0x00CA574A  2200                    and      al, byte ptr [eax]             
  0x00CA574C  005920                  add      byte ptr [ecx + 0x20], bl      
  0x00CA574F  0000                    add      byte ptr [eax], al             
  0x00CA5751  003a                    add      byte ptr [edx], bh             
  0x00CA5753  0000                    add      byte ptr [eax], al             
  0x00CA5755  06                      push     es                             
  0x00CA5756  3c00                    cmp      al, 0                          
  0x00CA5758  00f0                    add      al, dh                         
  0x00CA575A  7d00                    jge      0xca575c                       
                                        ; XREF: 0x00CA575A (cond_jump)
  0x00CA575C  130f                    adc      ecx, dword ptr [edi]           
  0x00CA575E  0000                    add      byte ptr [eax], al             
  0x00CA5760  96                      xchg     esi, eax                       
  0x00CA5761  050d000c00              add      eax, 0xc000d                   
  0x00CA5766  0000                    add      byte ptr [eax], al             
  0x00CA5768  00f0                    add      al, dh                         
  0x00CA576A  56                      push     esi                            
  0x00CA576B  004002                  add      byte ptr [eax + 2], al         
  0x00CA576E  0000                    add      byte ptr [eax], al             
  0x00CA5770  041d                    add      al, 0x1d                       
  0x00CA5772  0c00                    or       al, 0                          
  0x00CA5774  00c7                    add      bh, al                         
  0x00CA5776  2100                    and      dword ptr [eax], eax           
  0x00CA5778  00f4                    add      ah, dh                         
  0x00CA577A  46                      inc      esi                            
  0x00CA577B  0003                    add      byte ptr [ebx], al             
  0x00CA577D  0000                    add      byte ptr [eax], al             
  0x00CA577F  00b00020002e            add      byte ptr [eax + 0x2e002000], dh 
  0x00CA5785  1d0c00c040              sbb      eax, 0x40c0000c                
  0x00CA578A  0100                    add      dword ptr [eax], eax           
  0x00CA578C  49                      dec      ecx                            
  0x00CA578D  0000                    add      byte ptr [eax], al             
  0x00CA578F  0000                    add      byte ptr [eax], al             
  0x00CA5792  2100                    and      dword ptr [eax], eax           
  0x00CA5794  00f4                    add      ah, dh                         
  0x00CA5796  61                      popal                                   
  0x00CA5797  00a00b000000            add      byte ptr [eax + 0xb], ah       
  0x00CA579D  f4                      hlt                                     
  0x00CA579E  6200                    bound    eax, qword ptr [eax]           
  0x00CA57A0  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00CA57A1  0b00                    or       eax, dword ptr [eax]           
  0x00CA57A3  0000                    add      byte ptr [eax], al             
  0x00CA57A6  45                      inc      ebp                            
  0x00CA57A7  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA57AD  c506                    lds      eax, ptr [esi]                 
  0x00CA57AF  0003                    add      byte ptr [ebx], al             
  0x00CA57B1  0000                    add      byte ptr [eax], al             
  0x00CA57B3  0000                    add      byte ptr [eax], al             
  0x00CA57B5  59                      pop      ecx                            
  0x00CA57B6  47                      inc      edi                            
  0x00CA57B7  0000                    add      byte ptr [eax], al             
  0x00CA57B9  5a                      pop      edx                            
  0x00CA57BA  46                      inc      esi                            
  0x00CA57BB  0000                    add      byte ptr [eax], al             
  0x00CA57BD  f4                      hlt                                     
  0x00CA57BE  57                      push     edi                            
  0x00CA57BF  0001                    add      byte ptr [ecx], al             
  0x00CA57C1  0000                    add      byte ptr [eax], al             
  0x00CA57C3  0000                    add      byte ptr [eax], al             
  0x00CA57C6  56                      push     esi                            
  0x00CA57C7  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA57CA  0000                    add      byte ptr [eax], al             
  0x00CA57CC  0300                    add      eax, dword ptr [eax]           
  0x00CA57CE  2000                    and      byte ptr [eax], al             
  0x00CA57D0  02a405001b0020          add      ah, byte ptr [ebp + eax + 0x20001b00] 
  0x00CA57D7  0000                    add      byte ptr [eax], al             
  0x00CA57D9  7057                    jo       0xca5832                       
  0x00CA57DB  00900b00000c            add      byte ptr [eax + 0xc00000b], dl 
  0x00CA57E1  0000                    add      byte ptr [eax], al             
  0x00CA57E3  0000                    add      byte ptr [eax], al             
  0x00CA57E5  f4                      hlt                                     
  0x00CA57E6  56                      push     esi                            
  0x00CA57E7  000f                    add      byte ptr [edi], cl             
  0x00CA57E9  0000                    add      byte ptr [eax], al             
  0x00CA57EB  0000                    add      byte ptr [eax], al             
  0x00CA57ED  f4                      hlt                                     
  0x00CA57EE  57                      push     edi                            
  0x00CA57EF  0000                    add      byte ptr [eax], al             
  0x00CA57F1  0000                    add      byte ptr [eax], al             
  0x00CA57F3  0000                    add      byte ptr [eax], al             
  0x00CA57F5  f4                      hlt                                     
  0x00CA57F6  7000                    jo       0xca57f8                       
                                        ; XREF: 0x00CA57F6 (cond_jump)
  0x00CA57F8  16                      push     ss                             
  0x00CA57F9  0400                    add      al, 0                          
  0x00CA57FB  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA5801  0100                    add      dword ptr [eax], eax           
  0x00CA5803  0003                    add      byte ptr [ebx], al             
  0x00CA5805  0020                    add      byte ptr [eax], ah             
  0x00CA5807  0000                    add      byte ptr [eax], al             
  0x00CA5809  2405                    and      al, 5                          
  0x00CA580B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA580E  0000                    add      byte ptr [eax], al             
  0x00CA5810  00f4                    add      ah, dh                         
  0x00CA5812  56                      push     esi                            
  0x00CA5813  000f                    add      byte ptr [edi], cl             
  0x00CA5815  0000                    add      byte ptr [eax], al             
  0x00CA5817  0000                    add      byte ptr [eax], al             
  0x00CA5819  f4                      hlt                                     
  0x00CA581A  57                      push     edi                            
  0x00CA581B  0001                    add      byte ptr [ecx], al             
  0x00CA581D  0000                    add      byte ptr [eax], al             
  0x00CA581F  0000                    add      byte ptr [eax], al             
  0x00CA5821  f4                      hlt                                     
  0x00CA5822  60                      pushal                                  
  0x00CA5823  00fd                    add      ch, bh                         
  0x00CA5825  0400                    add      al, 0                          
  0x00CA5827  0000                    add      byte ptr [eax], al             
  0x00CA5829  f4                      hlt                                     
  0x00CA582A  7000                    jo       0xca582c                       
                                        ; XREF: 0x00CA582A (cond_jump)
  0x00CA582C  16                      push     ss                             
  0x00CA582D  0400                    add      al, 0                          
  0x00CA582F  0000                    add      byte ptr [eax], al             
  0x00CA5831  0039                    add      byte ptr [ecx], bh             
  0x00CA5833  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA5839  0100                    add      dword ptr [eax], eax           
  0x00CA583B  0003                    add      byte ptr [ebx], al             
  0x00CA583D  0020                    add      byte ptr [eax], ah             
  0x00CA583F  0000                    add      byte ptr [eax], al             
  0x00CA5841  2405                    and      al, 5                          
  0x00CA5843  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA5846  0000                    add      byte ptr [eax], al             
  0x00CA5848  00f4                    add      ah, dh                         
  0x00CA584A  56                      push     esi                            
  0x00CA584B  000f                    add      byte ptr [edi], cl             
  0x00CA584D  0000                    add      byte ptr [eax], al             
  0x00CA584F  0000                    add      byte ptr [eax], al             
  0x00CA5851  f4                      hlt                                     
  0x00CA5852  57                      push     edi                            
  0x00CA5853  0002                    add      byte ptr [edx], al             
  0x00CA5855  0000                    add      byte ptr [eax], al             
  0x00CA5857  0000                    add      byte ptr [eax], al             
  0x00CA5859  f4                      hlt                                     
  0x00CA585A  60                      pushal                                  
  0x00CA585B  00fd                    add      ch, bh                         
  0x00CA585D  0400                    add      al, 0                          
  0x00CA585F  0000                    add      byte ptr [eax], al             
  0x00CA5861  f4                      hlt                                     
  0x00CA5862  7000                    jo       0xca5864                       
                                        ; XREF: 0x00CA5862 (cond_jump)
  0x00CA5864  16                      push     ss                             
  0x00CA5865  0400                    add      al, 0                          
  0x00CA5867  0000                    add      byte ptr [eax], al             
  0x00CA5869  0039                    add      byte ptr [ecx], bh             
  0x00CA586B  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA5871  0100                    add      dword ptr [eax], eax           
  0x00CA5873  0003                    add      byte ptr [ebx], al             
  0x00CA5875  0020                    add      byte ptr [eax], ah             
  0x00CA5877  0000                    add      byte ptr [eax], al             
  0x00CA5879  2405                    and      al, 5                          
  0x00CA587B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA587E  0000                    add      byte ptr [eax], al             
  0x00CA5880  00f4                    add      ah, dh                         
  0x00CA5882  56                      push     esi                            
  0x00CA5883  0016                    add      byte ptr [esi], dl             
  0x00CA5885  0000                    add      byte ptr [eax], al             
  0x00CA5887  0000                    add      byte ptr [eax], al             
  0x00CA5889  f4                      hlt                                     
  0x00CA588A  57                      push     edi                            
  0x00CA588B  0001                    add      byte ptr [ecx], al             
  0x00CA588D  0000                    add      byte ptr [eax], al             
  0x00CA588F  0000                    add      byte ptr [eax], al             
  0x00CA5891  0039                    add      byte ptr [ecx], bh             
  0x00CA5893  0000                    add      byte ptr [eax], al             
  0x00CA5895  f4                      hlt                                     
  0x00CA5896  7000                    jo       0xca5898                       
                                        ; XREF: 0x00CA5896 (cond_jump)
  0x00CA5898  800000                  add      byte ptr [eax], 0              
  0x00CA589B  0000                    add      byte ptr [eax], al             
  0x00CA589D  f4                      hlt                                     
  0x00CA589E  60                      pushal                                  
  0x00CA589F  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA58A2  0000                    add      byte ptr [eax], al             
  0x00CA58A4  80f00b                  xor      al, 0xb                        
  0x00CA58A7  008001000003            add      byte ptr [eax + 0x3000001], al 
  0x00CA58AD  0020                    add      byte ptr [eax], ah             
  0x00CA58AF  0000                    add      byte ptr [eax], al             
  0x00CA58B1  2405                    and      al, 5                          
  0x00CA58B3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA58B6  0000                    add      byte ptr [eax], al             
  0x00CA58B8  00f4                    add      ah, dh                         
  0x00CA58BA  56                      push     esi                            
  0x00CA58BB  0012                    add      byte ptr [edx], dl             
  0x00CA58BD  0000                    add      byte ptr [eax], al             
  0x00CA58BF  0000                    add      byte ptr [eax], al             
  0x00CA58C1  f4                      hlt                                     
  0x00CA58C2  57                      push     edi                            
  0x00CA58C3  0000                    add      byte ptr [eax], al             
  0x00CA58C5  0000                    add      byte ptr [eax], al             
  0x00CA58C7  0000                    add      byte ptr [eax], al             
  0x00CA58C9  f4                      hlt                                     
  0x00CA58CA  7000                    jo       0xca58cc                       
                                        ; XREF: 0x00CA58CA (cond_jump)
  0x00CA58CC  90                      nop                                     
  0x00CA58CD  0300                    add      eax, dword ptr [eax]           
  0x00CA58CF  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA58D5  0100                    add      dword ptr [eax], eax           
  0x00CA58D7  0003                    add      byte ptr [ebx], al             
  0x00CA58D9  0020                    add      byte ptr [eax], ah             
  0x00CA58DB  0000                    add      byte ptr [eax], al             
  0x00CA58DD  2405                    and      al, 5                          
  0x00CA58DF  0000                    add      byte ptr [eax], al             
  0x00CA58E1  f4                      hlt                                     
  0x00CA58E2  56                      push     esi                            
  0x00CA58E3  0013                    add      byte ptr [ebx], dl             
  0x00CA58E5  0000                    add      byte ptr [eax], al             
  0x00CA58E7  0000                    add      byte ptr [eax], al             
  0x00CA58E9  f4                      hlt                                     
  0x00CA58EA  57                      push     edi                            
  0x00CA58EB  0000                    add      byte ptr [eax], al             
  0x00CA58ED  0000                    add      byte ptr [eax], al             
  0x00CA58EF  0000                    add      byte ptr [eax], al             
  0x00CA58F1  f4                      hlt                                     
  0x00CA58F2  7000                    jo       0xca58f4                       
                                        ; XREF: 0x00CA58F2 (cond_jump)
  0x00CA58F4  90                      nop                                     
  0x00CA58F5  0300                    add      eax, dword ptr [eax]           
  0x00CA58F7  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA58FD  0100                    add      dword ptr [eax], eax           
  0x00CA58FF  0003                    add      byte ptr [ebx], al             
  0x00CA5901  0020                    add      byte ptr [eax], ah             
  0x00CA5903  0000                    add      byte ptr [eax], al             
  0x00CA5905  2405                    and      al, 5                          
  0x00CA5907  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA590A  0000                    add      byte ptr [eax], al             
  0x00CA590C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA590F  006309                  add      byte ptr [ebx + 9], ah         
  0x00CA5912  0000                    add      byte ptr [eax], al             
  0x00CA5914  00f0                    add      al, dh                         
  0x00CA5916  56                      push     esi                            
  0x00CA5917  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA591D  0020                    add      byte ptr [eax], ah             
  0x00CA591F  0007                    add      byte ptr [edi], al             
  0x00CA5921  2405                    and      al, 5                          
  0x00CA5923  0000                    add      byte ptr [eax], al             
  0x00CA5925  f4                      hlt                                     
  0x00CA5926  7100                    jno      0xca5928                       
                                        ; XREF: 0x00CA5926 (cond_jump)
  0x00CA5928  8903                    mov      dword ptr [ebx], eax           
  0x00CA592A  0000                    add      byte ptr [eax], al             
  0x00CA592C  0007                    add      byte ptr [edi], al             
  0x00CA592E  3800                    cmp      byte ptr [eax], al             
  0x00CA5930  00f4                    add      ah, dh                         
  0x00CA5932  60                      pushal                                  
  0x00CA5933  003502000007            add      byte ptr [0x7000002], dh       
  0x00CA5939  0c05                    or       al, 5                          
  0x00CA593B  0000                    add      byte ptr [eax], al             
  0x00CA593D  f4                      hlt                                     
  0x00CA593E  46                      inc      esi                            
  0x00CA593F  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA5945  d820                    fsub     dword ptr [eax]                
  0x00CA5947  0022                    add      byte ptr [edx], ah             
  0x00CA5949  f4                      hlt                                     
  0x00CA594A  60                      pushal                                  
  0x00CA594B  008001000000            add      byte ptr [eax + 1], al         
  0x00CA5951  1921                    sbb      dword ptr [ecx], esp           
  0x00CA5953  0000                    add      byte ptr [eax], al             
  0x00CA5955  f4                      hlt                                     
  0x00CA5956  56                      push     esi                            
  0x00CA5957  0012                    add      byte ptr [edx], dl             
  0x00CA5959  0000                    add      byte ptr [eax], al             
  0x00CA595B  0000                    add      byte ptr [eax], al             
  0x00CA595D  f4                      hlt                                     
  0x00CA595E  57                      push     edi                            
  0x00CA595F  0002                    add      byte ptr [edx], al             
  0x00CA5961  0000                    add      byte ptr [eax], al             
  0x00CA5963  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA5969  0100                    add      dword ptr [eax], eax           
  0x00CA596B  0003                    add      byte ptr [ebx], al             
  0x00CA596D  0020                    add      byte ptr [eax], ah             
  0x00CA596F  0000                    add      byte ptr [eax], al             
  0x00CA5971  2405                    and      al, 5                          
  0x00CA5973  0000                    add      byte ptr [eax], al             
  0x00CA5976  44                      inc      esp                            
  0x00CA5977  006309                  add      byte ptr [ebx + 9], ah         
  0x00CA597A  0000                    add      byte ptr [eax], al             
  0x00CA597C  00f0                    add      al, dh                         
  0x00CA597E  56                      push     esi                            
  0x00CA597F  00970b000045            add      byte ptr [edi + 0x4500000b], dl 
  0x00CA5985  0020                    add      byte ptr [eax], ah             
  0x00CA5987  0007                    add      byte ptr [edi], al             
  0x00CA5989  2405                    and      al, 5                          
  0x00CA598B  0000                    add      byte ptr [eax], al             
  0x00CA598D  f4                      hlt                                     
  0x00CA598E  7100                    jno      0xca5990                       
                                        ; XREF: 0x00CA598E (cond_jump)
  0x00CA5990  8903                    mov      dword ptr [ebx], eax           
  0x00CA5992  0000                    add      byte ptr [eax], al             
  0x00CA5994  0007                    add      byte ptr [edi], al             
  0x00CA5996  3800                    cmp      byte ptr [eax], al             
  0x00CA5998  00f4                    add      ah, dh                         
  0x00CA599A  60                      pushal                                  
  0x00CA599B  00f6                    add      dh, dh                         
  0x00CA599D  0200                    add      al, byte ptr [eax]             
  0x00CA599F  0007                    add      byte ptr [edi], al             
  0x00CA59A1  0c05                    or       al, 5                          
  0x00CA59A3  0000                    add      byte ptr [eax], al             
  0x00CA59A5  f4                      hlt                                     
  0x00CA59A6  46                      inc      esi                            
  0x00CA59A7  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA59AD  d820                    fsub     dword ptr [eax]                
  0x00CA59AF  0022                    add      byte ptr [edx], ah             
  0x00CA59B1  f4                      hlt                                     
  0x00CA59B2  60                      pushal                                  
  0x00CA59B3  004102                  add      byte ptr [ecx + 2], al         
  0x00CA59B6  0000                    add      byte ptr [eax], al             
  0x00CA59B8  0019                    add      byte ptr [ecx], bl             
  0x00CA59BA  2100                    and      dword ptr [eax], eax           
  0x00CA59BC  00f4                    add      ah, dh                         
  0x00CA59BE  56                      push     esi                            
  0x00CA59BF  0013                    add      byte ptr [ebx], dl             
  0x00CA59C1  0000                    add      byte ptr [eax], al             
  0x00CA59C3  0000                    add      byte ptr [eax], al             
  0x00CA59C5  f4                      hlt                                     
  0x00CA59C6  57                      push     edi                            
  0x00CA59C7  0002                    add      byte ptr [edx], al             
  0x00CA59C9  0000                    add      byte ptr [eax], al             
  0x00CA59CB  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA59D1  0100                    add      dword ptr [eax], eax           
  0x00CA59D3  0003                    add      byte ptr [ebx], al             
  0x00CA59D5  0020                    add      byte ptr [eax], ah             
  0x00CA59D7  0000                    add      byte ptr [eax], al             
  0x00CA59D9  2405                    and      al, 5                          
  0x00CA59DB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA59DE  0000                    add      byte ptr [eax], al             
  0x00CA59E0  40                      inc      eax                            
  0x00CA59E1  1bd0                    sbb      edx, eax                       
  0x00CA59E3  005206                  add      byte ptr [edx + 6], dl         
  0x00CA59E6  0000                    add      byte ptr [eax], al             
  0x00CA59E8  50                      push     eax                            
  0x00CA59E9  0103                    add      dword ptr [ebx], eax           
  0x00CA59EB  00750b                  add      byte ptr [ebp + 0xb], dh       
  0x00CA59EE  8c00                    mov      word ptr [eax], es             
  0x00CA59F0  0b00                    or       eax, dword ptr [eax]           
  0x00CA59F2  2000                    and      byte ptr [eax], al             
  0x00CA59F4  0210                    add      dl, byte ptr [eax]             
  0x00CA59F6  0d004b0600              or       eax, 0x64b00                   
  0x00CA59FB  0080100d003b            add      byte ptr [eax + 0x3b000d10], al 
  0x00CA5A01  06                      push     es                             
  0x00CA5A02  0000                    add      byte ptr [eax], al             
  0x00CA5A04  00f4                    add      ah, dh                         
  0x00CA5A06  57                      push     edi                            
  0x00CA5A07  0010                    add      byte ptr [eax], dl             
  0x00CA5A09  0000                    add      byte ptr [eax], al             
  0x00CA5A0B  0000                    add      byte ptr [eax], al             
  0x00CA5A0D  0030                    add      byte ptr [eax], dh             
  0x00CA5A0F  0080100d0033            add      byte ptr [eax + 0x33000d10], al 
  0x00CA5A15  0200                    add      al, byte ptr [eax]             
  0x00CA5A17  0000                    add      byte ptr [eax], al             
  0x00CA5A19  f4                      hlt                                     
  0x00CA5A1A  44                      inc      esp                            
  0x00CA5A1B  0000                    add      byte ptr [eax], al             
  0x00CA5A1D  0000                    add      byte ptr [eax], al             
  0x00CA5A1F  004500                  add      byte ptr [ebp], al             
  0x00CA5A22  2000                    and      byte ptr [eax], al             
  0x00CA5A24  00740500                add      byte ptr [ebp + eax], dh       
  0x00CA5A28  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA5A2B  003f                    add      byte ptr [edi], bh             
  0x00CA5A2D  06                      push     es                             
  0x00CA5A2E  0000                    add      byte ptr [eax], al             
  0x00CA5A30  0c00                    or       al, 0                          
  0x00CA5A32  0000                    add      byte ptr [eax], al             
  0x00CA5A34  61                      popal                                   
  0x00CA5A35  f4                      hlt                                     
  0x00CA5A36  46                      inc      esi                            
  0x00CA5A37  0010                    add      byte ptr [eax], dl             
  0x00CA5A39  0000                    add      byte ptr [eax], al             
  0x00CA5A3B  0000                    add      byte ptr [eax], al             
  0x00CA5A3D  07                      pop      es                             
  0x00CA5A3E  2300                    and      eax, dword ptr [eax]           
  0x00CA5A40  10d9                    adc      cl, bl                         
  0x00CA5A42  06                      push     es                             
  0x00CA5A43  000a                    add      byte ptr [edx], cl             
  0x00CA5A45  0000                    add      byte ptr [eax], al             
  0x00CA5A47  007cd950                add      byte ptr [ecx + ebx*8 + 0x50], bh 
  0x00CA5A4B  0007                    add      byte ptr [edi], al             
  0x00CA5A4D  7405                    je       0xca5a54                       
  0x00CA5A4F  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA5A52  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA5A4D (cond_jump)
  0x00CA5A54  46                      inc      esi                            
  0x00CA5A55  1e                      push     ds                             
  0x00CA5A56  0c00                    or       al, 0                          
  0x00CA5A58  90                      nop                                     
  0x00CA5A59  1e                      push     ds                             
  0x00CA5A5A  0c00                    or       al, 0                          
  0x00CA5A5C  49                      dec      ecx                            
  0x00CA5A5D  e421                    in       al, 0x21                       
  0x00CA5A5F  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA5A62  54                      push     esp                            
  0x00CA5A63  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA5A66  0c00                    or       al, 0                          
  0x00CA5A68  4e                      dec      esi                            
  0x00CA5A69  1e                      push     ds                             
  0x00CA5A6A  0c00                    or       al, 0                          
  0x00CA5A6C  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA5A72  0000                    add      byte ptr [eax], al             
  0x00CA5A74  61                      popal                                   
  0x00CA5A75  f4                      hlt                                     
                                        ; XREF: 0x00CA5AC8 (cond_jump)
  0x00CA5A76  46                      inc      esi                            
  0x00CA5A77  0010                    add      byte ptr [eax], dl             
  0x00CA5A79  0000                    add      byte ptr [eax], al             
  0x00CA5A7B  0000                    add      byte ptr [eax], al             
  0x00CA5A7D  07                      pop      es                             
  0x00CA5A7E  2300                    and      eax, dword ptr [eax]           
  0x00CA5A80  7cd9                    jl       0xca5a5b                       
  0x00CA5A82  50                      push     eax                            
  0x00CA5A83  0007                    add      byte ptr [edi], al             
  0x00CA5A85  7405                    je       0xca5a8c                       
  0x00CA5A87  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA5A8A  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA5A85 (cond_jump)
  0x00CA5A8C  46                      inc      esi                            
  0x00CA5A8D  1e                      push     ds                             
  0x00CA5A8E  0c00                    or       al, 0                          
  0x00CA5A90  90                      nop                                     
  0x00CA5A91  1e                      push     ds                             
  0x00CA5A92  0c00                    or       al, 0                          
  0x00CA5A94  49                      dec      ecx                            
  0x00CA5A95  e421                    in       al, 0x21                       
  0x00CA5A97  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA5A9A  54                      push     esp                            
  0x00CA5A9B  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA5A9E  0c00                    or       al, 0                          
  0x00CA5AA0  4e                      dec      esi                            
  0x00CA5AA1  1e                      push     ds                             
  0x00CA5AA2  0c00                    or       al, 0                          
  0x00CA5AA4  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA5AAA  0000                    add      byte ptr [eax], al             
  0x00CA5AAC  00f4                    add      ah, dh                         
  0x00CA5AAE  46                      inc      esi                            
  0x00CA5AAF  0010                    add      byte ptr [eax], dl             
  0x00CA5AB1  0000                    add      byte ptr [eax], al             
  0x00CA5AB3  0000                    add      byte ptr [eax], al             
  0x00CA5AB5  07                      pop      es                             
  0x00CA5AB6  2300                    and      eax, dword ptr [eax]           
  0x00CA5AB8  10d9                    adc      cl, bl                         
  0x00CA5ABA  06                      push     es                             
  0x00CA5ABB  000d00000000            add      byte ptr [0], cl               
  0x00CA5AC1  d95600                  fst      dword ptr [esi]                
  0x00CA5AC4  6e                      outsb    dx, byte ptr [esi]             
  0x00CA5AC5  1e                      push     ds                             
  0x00CA5AC6  0c00                    or       al, 0                          
  0x00CA5AC8  7cac                    jl       0xca5a76                       
  0x00CA5ACA  2000                    and      byte ptr [eax], al             
  0x00CA5ACC  07                      pop      es                             
  0x00CA5ACD  7405                    je       0xca5ad4                       
  0x00CA5ACF  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA5AD2  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA5ACD (cond_jump)
  0x00CA5AD4  46                      inc      esi                            
  0x00CA5AD5  1e                      push     ds                             
  0x00CA5AD6  0c00                    or       al, 0                          
  0x00CA5AD8  90                      nop                                     
  0x00CA5AD9  1e                      push     ds                             
  0x00CA5ADA  0c00                    or       al, 0                          
  0x00CA5ADC  49                      dec      ecx                            
  0x00CA5ADD  e421                    in       al, 0x21                       
  0x00CA5ADF  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA5AE2  54                      push     esp                            
  0x00CA5AE3  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA5AE6  0c00                    or       al, 0                          
  0x00CA5AE8  4e                      dec      esi                            
  0x00CA5AE9  1e                      push     ds                             
  0x00CA5AEA  0c00                    or       al, 0                          
  0x00CA5AEC  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA5AF2  0000                    add      byte ptr [eax], al             
  0x00CA5AF4  00f4                    add      ah, dh                         
  0x00CA5AF6  46                      inc      esi                            
  0x00CA5AF7  0010                    add      byte ptr [eax], dl             
  0x00CA5AF9  0000                    add      byte ptr [eax], al             
  0x00CA5AFB  0000                    add      byte ptr [eax], al             
  0x00CA5AFD  07                      pop      es                             
                                        ; XREF: 0x00CA5B50 (cond_jump)
  0x00CA5AFE  2300                    and      eax, dword ptr [eax]           
  0x00CA5B00  10d9                    adc      cl, bl                         
  0x00CA5B02  06                      push     es                             
  0x00CA5B03  000d00000000            add      byte ptr [0], cl               
  0x00CA5B09  d95e00                  fstp     dword ptr [esi]                
  0x00CA5B0C  6e                      outsb    dx, byte ptr [esi]             
  0x00CA5B0D  1e                      push     ds                             
  0x00CA5B0E  0c00                    or       al, 0                          
  0x00CA5B10  7cac                    jl       0xca5abe                       
  0x00CA5B12  2000                    and      byte ptr [eax], al             
  0x00CA5B14  07                      pop      es                             
  0x00CA5B15  7405                    je       0xca5b1c                       
  0x00CA5B17  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA5B1A  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA5B15 (cond_jump)
  0x00CA5B1C  46                      inc      esi                            
  0x00CA5B1D  1e                      push     ds                             
  0x00CA5B1E  0c00                    or       al, 0                          
  0x00CA5B20  90                      nop                                     
  0x00CA5B21  1e                      push     ds                             
  0x00CA5B22  0c00                    or       al, 0                          
  0x00CA5B24  49                      dec      ecx                            
  0x00CA5B25  e421                    in       al, 0x21                       
  0x00CA5B27  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA5B2A  54                      push     esp                            
  0x00CA5B2B  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA5B2E  0c00                    or       al, 0                          
  0x00CA5B30  4e                      dec      esi                            
  0x00CA5B31  1e                      push     ds                             
  0x00CA5B32  0c00                    or       al, 0                          
  0x00CA5B34  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA5B3A  0000                    add      byte ptr [eax], al             
  0x00CA5B3C  00f4                    add      ah, dh                         
                                        ; XREF: 0x00CA5B90 (cond_jump)
  0x00CA5B3E  46                      inc      esi                            
  0x00CA5B3F  0010                    add      byte ptr [eax], dl             
  0x00CA5B41  0000                    add      byte ptr [eax], al             
  0x00CA5B43  0000                    add      byte ptr [eax], al             
  0x00CA5B45  07                      pop      es                             
  0x00CA5B46  2300                    and      eax, dword ptr [eax]           
  0x00CA5B48  00d9                    add      cl, bl                         
  0x00CA5B4A  56                      push     esi                            
  0x00CA5B4B  006e1e                  add      byte ptr [esi + 0x1e], ch      
  0x00CA5B4E  0c00                    or       al, 0                          
  0x00CA5B50  7cac                    jl       0xca5afe                       
  0x00CA5B52  2000                    and      byte ptr [eax], al             
  0x00CA5B54  07                      pop      es                             
  0x00CA5B55  7405                    je       0xca5b5c                       
  0x00CA5B57  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA5B5A  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA5B55 (cond_jump)
  0x00CA5B5C  46                      inc      esi                            
  0x00CA5B5D  1e                      push     ds                             
  0x00CA5B5E  0c00                    or       al, 0                          
  0x00CA5B60  90                      nop                                     
  0x00CA5B61  1e                      push     ds                             
  0x00CA5B62  0c00                    or       al, 0                          
  0x00CA5B64  49                      dec      ecx                            
  0x00CA5B65  e421                    in       al, 0x21                       
  0x00CA5B67  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA5B6A  54                      push     esp                            
  0x00CA5B6B  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA5B6E  0c00                    or       al, 0                          
  0x00CA5B70  4e                      dec      esi                            
  0x00CA5B71  1e                      push     ds                             
  0x00CA5B72  0c00                    or       al, 0                          
  0x00CA5B74  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA5B7A  0000                    add      byte ptr [eax], al             
  0x00CA5B7C  00f4                    add      ah, dh                         
  0x00CA5B7E  46                      inc      esi                            
  0x00CA5B7F  0010                    add      byte ptr [eax], dl             
  0x00CA5B81  0000                    add      byte ptr [eax], al             
  0x00CA5B83  0000                    add      byte ptr [eax], al             
  0x00CA5B85  07                      pop      es                             
  0x00CA5B86  2300                    and      eax, dword ptr [eax]           
  0x00CA5B88  00d9                    add      cl, bl                         
  0x00CA5B8A  5e                      pop      esi                            
  0x00CA5B8B  006e1e                  add      byte ptr [esi + 0x1e], ch      
  0x00CA5B8E  0c00                    or       al, 0                          
  0x00CA5B90  7cac                    jl       0xca5b3e                       
  0x00CA5B92  2000                    and      byte ptr [eax], al             
  0x00CA5B94  07                      pop      es                             
  0x00CA5B95  7405                    je       0xca5b9c                       
  0x00CA5B97  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA5B9A  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA5B95 (cond_jump)
  0x00CA5B9C  46                      inc      esi                            
  0x00CA5B9D  1e                      push     ds                             
  0x00CA5B9E  0c00                    or       al, 0                          
  0x00CA5BA0  90                      nop                                     
  0x00CA5BA1  1e                      push     ds                             
  0x00CA5BA2  0c00                    or       al, 0                          
  0x00CA5BA4  49                      dec      ecx                            
  0x00CA5BA5  e421                    in       al, 0x21                       
  0x00CA5BA7  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA5BAA  54                      push     esp                            
  0x00CA5BAB  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA5BAE  0c00                    or       al, 0                          
  0x00CA5BB0  4e                      dec      esi                            
  0x00CA5BB1  1e                      push     ds                             
  0x00CA5BB2  0c00                    or       al, 0                          
  0x00CA5BB4  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA5BBA  0000                    add      byte ptr [eax], al             
  0x00CA5BBC  00f4                    add      ah, dh                         
  0x00CA5BBE  61                      popal                                   
  0x00CA5BBF  0012                    add      byte ptr [edx], dl             
  0x00CA5BC1  0d000000f4              or       eax, 0xf4000000                
  0x00CA5BC6  46                      inc      esi                            
  0x00CA5BC7  00ff                    add      bh, bh                         
  0x00CA5BC9  0000                    add      byte ptr [eax], al             
  0x00CA5BCB  0010                    add      byte ptr [eax], dl             
  0x00CA5BCD  d806                    fadd     dword ptr [esi]                
  0x00CA5BCF  000e                    add      byte ptr [esi], cl             
  0x00CA5BD1  0000                    add      byte ptr [eax], al             
  0x00CA5BD3  00901c0c0056            add      byte ptr [eax + 0x56000c1c], dl 
  0x00CA5BD9  0020                    add      byte ptr [eax], ah             
  0x00CA5BDB  0000                    add      byte ptr [eax], al             
  0x00CA5BDD  d85100                  fcom     dword ptr [ecx]                
  0x00CA5BE0  00992100911d            add      byte ptr [ecx + 0x1d910021], bl 
  0x00CA5BE6  0c00                    or       al, 0                          
  0x00CA5BE8  00e9                    add      cl, ch                         
  0x00CA5BEA  4c                      dec      esp                            
  0x00CA5BEB  004b00                  add      byte ptr [ebx], cl             
  0x00CA5BEE  2000                    and      byte ptr [eax], al             
  0x00CA5BF0  90                      nop                                     
  0x00CA5BF1  1c0c                    sbb      al, 0xc                        
  0x00CA5BF3  005600                  add      byte ptr [esi], dl             
  0x00CA5BF6  2000                    and      byte ptr [eax], al             
  0x00CA5BF8  00992100911d            add      byte ptr [ecx + 0x1d910021], bl 
  0x00CA5BFE  0c00                    or       al, 0                          
  0x00CA5C00  00e9                    add      cl, ch                         
  0x00CA5C02  4c                      dec      esp                            
  0x00CA5C03  004b00                  add      byte ptr [ebx], cl             
  0x00CA5C06  2000                    and      byte ptr [eax], al             
  0x00CA5C08  91                      xchg     ecx, eax                       
  0x00CA5C09  1e                      push     ds                             
  0x00CA5C0A  0c00                    or       al, 0                          
  0x00CA5C0C  00ae2100111c            add      byte ptr [esi + 0x1c110021], ch 
  0x00CA5C12  0c00                    or       al, 0                          
  0x00CA5C14  0c00                    or       al, 0                          
  0x00CA5C16  0000                    add      byte ptr [eax], al             
  0x00CA5C18  1bf4                    sbb      esi, esp                       
  0x00CA5C1A  61                      popal                                   
  0x00CA5C1B  0012                    add      byte ptr [edx], dl             
  0x00CA5C1D  0e                      push     cs                             
  0x00CA5C1E  0000                    add      byte ptr [eax], al             
  0x00CA5C20  00f4                    add      ah, dh                         
  0x00CA5C22  46                      inc      esi                            
  0x00CA5C23  00ff                    add      bh, bh                         
  0x00CA5C25  0000                    add      byte ptr [eax], al             
  0x00CA5C27  0000                    add      byte ptr [eax], al             
  0x00CA5C29  48                      dec      eax                            
  0x00CA5C2A  2000                    and      byte ptr [eax], al             
  0x00CA5C2C  10d8                    adc      al, bl                         
  0x00CA5C2E  06                      push     es                             
  0x00CA5C2F  000d0000005e            add      byte ptr [0x5e000000], cl      
  0x00CA5C35  ae                      scasb    al, byte ptr es:[edi]          
  0x00CA5C36  2100                    and      dword ptr [eax], eax           
  0x00CA5C38  00f8                    add      al, bh                         
  0x00CA5C3A  44                      inc      esp                            
  0x00CA5C3B  0000                    add      byte ptr [eax], al             
  0x00CA5C3D  b92100d01e              mov      ecx, 0x1ed00021                
  0x00CA5C42  0c00                    or       al, 0                          
  0x00CA5C44  42                      inc      edx                            
  0x00CA5C45  0020                    add      byte ptr [eax], ah             
  0x00CA5C47  0000                    add      byte ptr [eax], al             
  0x00CA5C49  e94c004300              jmp      0x10d5c9a                      
  0x00CA5C4E  2000                    and      byte ptr [eax], al             
  0x00CA5C50  56                      push     esi                            
  0x00CA5C52  2100                    and      dword ptr [eax], eax           
  0x00CA5C54  00992100d11e            add      byte ptr [ecx + 0x1ed10021], bl 
  0x00CA5C5A  0c00                    or       al, 0                          
  0x00CA5C5C  00e9                    add      cl, ch                         
  0x00CA5C5E  4c                      dec      esp                            
  0x00CA5C5F  004b00                  add      byte ptr [ebx], cl             
  0x00CA5C62  2000                    and      byte ptr [eax], al             
  0x00CA5C64  91                      xchg     ecx, eax                       
  0x00CA5C65  1e                      push     ds                             
  0x00CA5C66  0c00                    or       al, 0                          
  0x00CA5C68  00ae2100111c            add      byte ptr [esi + 0x1c110021], ch 
  0x00CA5C6E  0c00                    or       al, 0                          
  0x00CA5C70  0c00                    or       al, 0                          
  0x00CA5C72  0000                    add      byte ptr [eax], al             
  0x00CA5C74  180400                  sbb      byte ptr [eax + eax], al       
  0x00CA5C77  0018                    add      byte ptr [eax], bl             
  0x00CA5C79  0400                    add      al, 0                          
  0x00CA5C7B  0018                    add      byte ptr [eax], bl             
  0x00CA5C7D  0400                    add      al, 0                          
  0x00CA5C7F  002a                    add      byte ptr [edx], ch             
  0x00CA5C81  0400                    add      al, 0                          
  0x00CA5C83  002a                    add      byte ptr [edx], ch             
  0x00CA5C85  0400                    add      al, 0                          
  0x00CA5C87  002a                    add      byte ptr [edx], ch             
  0x00CA5C89  0400                    add      al, 0                          
  0x00CA5C8B  002504000042            add      byte ptr [0x42000004], ah      
  0x00CA5C91  0400                    add      al, 0                          
  0x00CA5C93  004204                  add      byte ptr [edx + 4], al         
  0x00CA5C96  0000                    add      byte ptr [eax], al             
  0x00CA5C98  42                      inc      edx                            
  0x00CA5C99  0400                    add      al, 0                          
  0x00CA5C9B  004204                  add      byte ptr [edx + 4], al         
  0x00CA5C9E  0000                    add      byte ptr [eax], al             
  0x00CA5CA0  42                      inc      edx                            
  0x00CA5CA1  0400                    add      al, 0                          
  0x00CA5CA3  004204                  add      byte ptr [edx + 4], al         
  0x00CA5CA6  0000                    add      byte ptr [eax], al             
  0x00CA5CA8  42                      inc      edx                            
  0x00CA5CA9  0400                    add      al, 0                          
  0x00CA5CAB  004204                  add      byte ptr [edx + 4], al         
  0x00CA5CAE  0000                    add      byte ptr [eax], al             
  0x00CA5CB0  42                      inc      edx                            
  0x00CA5CB1  0400                    add      al, 0                          
  0x00CA5CB3  004204                  add      byte ptr [edx + 4], al         
  0x00CA5CB6  0000                    add      byte ptr [eax], al             
  0x00CA5CB8  42                      inc      edx                            
  0x00CA5CB9  0400                    add      al, 0                          
  0x00CA5CBB  004204                  add      byte ptr [edx + 4], al         
  0x00CA5CBE  0000                    add      byte ptr [eax], al             
  0x00CA5CC0  42                      inc      edx                            
  0x00CA5CC1  0400                    add      al, 0                          
  0x00CA5CC3  004e04                  add      byte ptr [esi + 4], cl         
  0x00CA5CC6  0000                    add      byte ptr [eax], al             
  0x00CA5CC8  4e                      dec      esi                            
  0x00CA5CC9  0400                    add      al, 0                          
  0x00CA5CCB  004e04                  add      byte ptr [esi + 4], cl         
  0x00CA5CCE  0000                    add      byte ptr [eax], al             
  0x00CA5CD0  5f                      pop      edi                            
  0x00CA5CD1  0400                    add      al, 0                          
  0x00CA5CD3  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5CD6  0000                    add      byte ptr [eax], al             
  0x00CA5CD8  5f                      pop      edi                            
  0x00CA5CD9  0400                    add      al, 0                          
  0x00CA5CDB  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5CDE  0000                    add      byte ptr [eax], al             
  0x00CA5CE0  5f                      pop      edi                            
  0x00CA5CE1  0400                    add      al, 0                          
  0x00CA5CE3  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5CE6  0000                    add      byte ptr [eax], al             
  0x00CA5CE8  5f                      pop      edi                            
  0x00CA5CE9  0400                    add      al, 0                          
  0x00CA5CEB  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5CEE  0000                    add      byte ptr [eax], al             
  0x00CA5CF0  5f                      pop      edi                            
  0x00CA5CF1  0400                    add      al, 0                          
  0x00CA5CF3  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5CF6  0000                    add      byte ptr [eax], al             
  0x00CA5CF8  5f                      pop      edi                            
  0x00CA5CF9  0400                    add      al, 0                          
  0x00CA5CFB  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5CFE  0000                    add      byte ptr [eax], al             
  0x00CA5D00  5f                      pop      edi                            
  0x00CA5D01  0400                    add      al, 0                          
  0x00CA5D03  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5D06  0000                    add      byte ptr [eax], al             
  0x00CA5D08  5f                      pop      edi                            
  0x00CA5D09  0400                    add      al, 0                          
  0x00CA5D0B  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5D0E  0000                    add      byte ptr [eax], al             
  0x00CA5D10  5f                      pop      edi                            
  0x00CA5D11  0400                    add      al, 0                          
  0x00CA5D13  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5D16  0000                    add      byte ptr [eax], al             
  0x00CA5D18  5f                      pop      edi                            
  0x00CA5D19  0400                    add      al, 0                          
  0x00CA5D1B  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5D1E  0000                    add      byte ptr [eax], al             
  0x00CA5D20  5f                      pop      edi                            
  0x00CA5D21  0400                    add      al, 0                          
  0x00CA5D23  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5D26  0000                    add      byte ptr [eax], al             
  0x00CA5D28  5f                      pop      edi                            
  0x00CA5D29  0400                    add      al, 0                          
  0x00CA5D2B  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5D2E  0000                    add      byte ptr [eax], al             
  0x00CA5D30  5f                      pop      edi                            
  0x00CA5D31  0400                    add      al, 0                          
  0x00CA5D33  005f04                  add      byte ptr [edi + 4], bl         
  0x00CA5D36  0000                    add      byte ptr [eax], al             
  0x00CA5D38  5f                      pop      edi                            
  0x00CA5D39  0400                    add      al, 0                          
  0x00CA5D3B  0000                    add      byte ptr [eax], al             
  0x00CA5D3D  b122                    mov      cl, 0x22                       
  0x00CA5D3F  0000                    add      byte ptr [eax], al             
  0x00CA5D41  1923                    sbb      dword ptr [ebx], esp           
  0x00CA5D43  0000                    add      byte ptr [eax], al             
  0x00CA5D45  48                      dec      eax                            
  0x00CA5D46  2000                    and      byte ptr [eax], al             
  0x00CA5D48  004920                  add      byte ptr [ecx + 0x20], cl      
  0x00CA5D4B  001b                    add      byte ptr [ebx], bl             
  0x00CA5D4D  f4                      hlt                                     
  0x00CA5D4E  45                      inc      ebp                            
  0x00CA5D4F  004000                  add      byte ptr [eax], al             
  0x00CA5D52  0000                    add      byte ptr [eax], al             
  0x00CA5D54  00f4                    add      ah, dh                         
  0x00CA5D56  51                      push     ecx                            
  0x00CA5D57  0000                    add      byte ptr [eax], al             
  0x00CA5D59  0c00                    or       al, 0                          
  0x00CA5D5B  0001                    add      byte ptr [ecx], al             
  0x00CA5D5D  d8440010                fadd     dword ptr [eax + eax + 0x10]   
  0x00CA5D61  dc06                    fadd     qword ptr [esi]                
  0x00CA5D63  0003                    add      byte ptr [ebx], al             
  0x00CA5D65  0000                    add      byte ptr [eax], al             
  0x00CA5D67  00a6d8440001            add      byte ptr [esi + 0x10044d8], ah 
  0x00CA5D6D  59                      pop      ecx                            
  0x00CA5D6E  50                      push     eax                            
  0x00CA5D6F  0000                    add      byte ptr [eax], al             
  0x00CA5D71  002400                  add      byte ptr [eax + eax], ah       
  0x00CA5D74  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA5D77  009204000000            add      byte ptr [edx + 4], dl         
  0x00CA5D7D  002400                  add      byte ptr [eax + eax], ah       
  0x00CA5D80  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA5D83  009104000000            add      byte ptr [ecx + 4], dl         
  0x00CA5D89  1023                    adc      byte ptr [ebx], ah             
  0x00CA5D8B  0000                    add      byte ptr [eax], al             
  0x00CA5D8D  b8220000f4              mov      eax, 0xf4000022                
  0x00CA5D92  7400                    je       0xca5d94                       
                                        ; XREF: 0x00CA5D92 (cond_jump)
  0x00CA5D94  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00CA5D95  0300                    add      eax, dword ptr [eax]           
  0x00CA5D97  0000                    add      byte ptr [eax], al             
  0x00CA5D99  f4                      hlt                                     
  0x00CA5D9A  650032                  add      byte ptr gs:[edx], dh          
  0x00CA5D9D  0b00                    or       eax, dword ptr [eax]           
  0x00CA5D9F  0000                    add      byte ptr [eax], al             
  0x00CA5DA2  44                      inc      esp                            
  0x00CA5DA3  007b0b                  add      byte ptr [ebx + 0xb], bh       
  0x00CA5DA6  0000                    add      byte ptr [eax], al             
  0x00CA5DA8  00f4                    add      ah, dh                         
  0x00CA5DAA  46                      inc      esi                            
  0x00CA5DAB  0032                    add      byte ptr [edx], dh             
  0x00CA5DAD  0000                    add      byte ptr [eax], al             
  0x00CA5DAF  00d0                    add      al, dl                         
  0x00CA5DB1  44                      inc      esp                            
  0x00CA5DB2  2200                    and      al, byte ptr [eax]             
  0x00CA5DB4  2e1d0c0040f4            sbb      eax, 0xf440000c                
  0x00CA5DBA  44                      inc      esp                            
  0x00CA5DBB  001c0c                  add      byte ptr [esp + ecx], bl       
  0x00CA5DBE  0000                    add      byte ptr [eax], al             
  0x00CA5DC0  40                      inc      eax                            
  0x00CA5DC1  0020                    add      byte ptr [eax], ah             
  0x00CA5DC3  0000                    add      byte ptr [eax], al             
  0x00CA5DC5  96                      xchg     esi, eax                       
  0x00CA5DC6  2100                    and      dword ptr [eax], eax           
  0x00CA5DC8  0001                    add      byte ptr [ecx], al             
  0x00CA5DCA  3900                    cmp      dword ptr [eax], eax           
  0x00CA5DCC  ce                      into                                    
  0x00CA5DCD  720b                    jb       0xca5dda                       
  0x00CA5DCF  001a                    add      byte ptr [edx], bl             
  0x00CA5DD1  0f0000                  sldt     word ptr [eax]                 
  0x00CA5DD4  00c4                    add      ah, al                         
  0x00CA5DD6  2300                    and      eax, dword ptr [eax]           
  0x00CA5DD8  45                      inc      ebp                            
  0x00CA5DD9  07                      pop      es                             
                                        ; XREF: 0x00CA5DCD (cond_jump)
  0x00CA5DDA  2200                    and      al, byte ptr [eax]             
  0x00CA5DDC  40                      inc      eax                            
  0x00CA5DDD  7002                    jo       0xca5de1                       
  0x00CA5DDF  00742423                add      byte ptr [esp + 0x23], dh      
  0x00CA5DE3  0044e857                add      byte ptr [eax + ebp*8 + 0x57], al 
  0x00CA5DE7  0000                    add      byte ptr [eax], al             
  0x00CA5DE9  58                      pop      eax                            
  0x00CA5DEA  2000                    and      byte ptr [eax], al             
  0x00CA5DEC  00e8                    add      al, ch                         
  0x00CA5DEE  45                      inc      ebp                            
  0x00CA5DEF  000da4050000            add      byte ptr [0x5a4], cl           
  0x00CA5DF5  f4                      hlt                                     
  0x00CA5DF6  47                      inc      edi                            
  0x00CA5DF7  00d1                    add      cl, dl                         
  0x00CA5DF9  0000                    add      byte ptr [eax], al             
  0x00CA5DFB  0010                    add      byte ptr [eax], dl             
  0x00CA5DFD  cc                      int3                                    
  0x00CA5DFE  06                      push     es                             
  0x00CA5DFF  0009                    add      byte ptr [ecx], cl             
  0x00CA5E01  0000                    add      byte ptr [eax], al             
  0x00CA5E03  006cee21                add      byte ptr [esi + ebp*8 + 0x21], ch 
  0x00CA5E07  006090                  add      byte ptr [eax - 0x70], ah      
  0x00CA5E0A  0200                    add      al, byte ptr [eax]             
  0x00CA5E0C  2e58                    pop      eax                            
  0x00CA5E0E  2000                    and      byte ptr [eax], al             
  0x00CA5E10  2be8                    sub      ebp, eax                       
  0x00CA5E12  45                      inc      ebp                            
  0x00CA5E13  007dbd                  add      byte ptr [ebp - 0x43], bh      
  0x00CA5E16  2100                    and      dword ptr [eax], eax           
  0x00CA5E18  00ed                    add      ch, ch                         
  0x00CA5E1A  4c                      dec      esp                            
  0x00CA5E1B  00402f                  add      byte ptr [eax + 0x2f], al      
  0x00CA5E1E  2000                    and      byte ptr [eax], al             
  0x00CA5E20  00cf                    add      bh, cl                         
  0x00CA5E22  2100                    and      dword ptr [eax], eax           
  0x00CA5E24  00542200                add      byte ptr [edx], dl             
  0x00CA5E28  61                      popal                                   
  0x00CA5E29  f4                      hlt                                     
  0x00CA5E2A  44                      inc      esp                            
  0x00CA5E2B  0000                    add      byte ptr [eax], al             
  0x00CA5E2D  0100                    add      dword ptr [eax], eax           
  0x00CA5E2F  0094ec070044f0          add      byte ptr [esp + ebp*8 - 0xfbbfff9], dl 
  0x00CA5E36  47                      inc      edi                            
  0x00CA5E37  009104000080            add      byte ptr [ecx - 0x7ffffffc], dl 
  0x00CA5E3D  e40a                    in       al, 0xa                        
  0x00CA5E3F  000d70570093            add      byte ptr [0x93005770], cl      
  0x00CA5E45  0400                    add      al, 0                          
  0x00CA5E47  0008                    add      byte ptr [eax], cl             
  0x00CA5E49  f4                      hlt                                     
  0x00CA5E4A  05006dee20              add      eax, 0x20ee6d00                
  0x00CA5E4F  0058f4                  add      byte ptr [eax - 0xc], bl       
  0x00CA5E52  0500c44001              add      eax, 0x140c400                 
  0x00CA5E57  004000                  add      byte ptr [eax], al             
  0x00CA5E5A  0000                    add      byte ptr [eax], al             
  0x00CA5E5C  1329                    adc      ebp, dword ptr [ecx]           
  0x00CA5E5E  2000                    and      byte ptr [eax], al             
  0x00CA5E60  00872100530c            add      byte ptr [edi + 0xc530021], al 
  0x00CA5E66  050000f447              add      eax, 0x47f40000                
  0x00CA5E6B  008001000050            add      byte ptr [eax + 0x50000001], al 
  0x00CA5E71  0c05                    or       al, 5                          
  0x00CA5E73  0000                    add      byte ptr [eax], al             
  0x00CA5E75  8621                    xchg     byte ptr [ecx], ah             
  0x00CA5E77  0000                    add      byte ptr [eax], al             
  0x00CA5E79  ce                      into                                    
  0x00CA5E7A  2300                    and      eax, dword ptr [eax]           
  0x00CA5E7C  854701                  test     dword ptr [edi + 1], eax       
  0x00CA5E7F  000da4050000            add      byte ptr [0x5a4], cl           
  0x00CA5E85  ce                      into                                    
  0x00CA5E86  2000                    and      byte ptr [eax], al             
  0x00CA5E88  0d00200008              or       eax, 0x8002000                 
  0x00CA5E8D  f4                      hlt                                     
  0x00CA5E8E  05006dee20              add      eax, 0x20ee6d00                
  0x00CA5E93  0008                    add      byte ptr [eax], cl             
  0x00CA5E95  f4                      hlt                                     
  0x00CA5E96  0500c44001              add      eax, 0x140c400                 
  0x00CA5E9B  004000                  add      byte ptr [eax], al             
  0x00CA5E9E  0000                    add      byte ptr [eax], al             
  0x00CA5EA0  1329                    adc      ebp, dword ptr [ecx]           
  0x00CA5EA2  2000                    and      byte ptr [eax], al             
  0x00CA5EA4  00872100030c            add      byte ptr [edi + 0xc030021], al 
  0x00CA5EAA  050000f447              add      eax, 0x47f40000                
  0x00CA5EAF  008001000000            add      byte ptr [eax + 1], al         
  0x00CA5EB6  44                      inc      esp                            
  0x00CA5EB7  00930400004d            add      byte ptr [ebx + 0x4d000004], dl 
  0x00CA5EBE  56                      push     esi                            
  0x00CA5EBF  009204000000            add      byte ptr [edx + 4], dl         
  0x00CA5EC5  7055                    jo       0xca5f1c                       
  0x00CA5EC7  009304000004            add      byte ptr [ebx + 0x4000004], dl 
  0x00CA5ECD  94                      xchg     esp, eax                       
  0x00CA5ECE  0500002e23              add      eax, 0x232e0000                
  0x00CA5ED3  0000                    add      byte ptr [eax], al             
  0x00CA5ED5  7071                    jo       0xca5f48                       
  0x00CA5ED7  009204000003            add      byte ptr [edx + 0x3000004], dl 
  0x00CA5EDD  0020                    add      byte ptr [eax], ah             
  0x00CA5EDF  0014a4                  add      byte ptr [esp], dl             
  0x00CA5EE2  05001e0c05              add      eax, 0x50c1e00                 
  0x00CA5EE7  000d00200008            add      byte ptr [0x8002000], cl       
  0x00CA5EED  f4                      hlt                                     
  0x00CA5EEE  05006dee20              add      eax, 0x20ee6d00                
  0x00CA5EF3  001a                    add      byte ptr [edx], bl             
  0x00CA5EF5  f4                      hlt                                     
  0x00CA5EF6  0500c44001              add      eax, 0x140c400                 
  0x00CA5EFB  004000                  add      byte ptr [eax], al             
  0x00CA5EFE  0000                    add      byte ptr [eax], al             
  0x00CA5F00  1329                    adc      ebp, dword ptr [ecx]           
  0x00CA5F02  2000                    and      byte ptr [eax], al             
  0x00CA5F04  00872100150c            add      byte ptr [edi + 0xc150021], al 
  0x00CA5F0A  050000f447              add      eax, 0x47f40000                
  0x00CA5F0F  004001                  add      byte ptr [eax + 1], al         
  0x00CA5F12  0000                    add      byte ptr [eax], al             
  0x00CA5F14  120c0500710020          adc      cl, byte ptr [eax + 0x20007100] 
  0x00CA5F1B  00c4                    add      ah, al                         
  0x00CA5F1D  40                      inc      eax                            
  0x00CA5F1E  0100                    add      dword ptr [eax], eax           
  0x00CA5F20  800000                  add      byte ptr [eax], 0              
  0x00CA5F23  0013                    add      byte ptr [ebx], dl             
  0x00CA5F25  2920                    sub      dword ptr [eax], esp           
  0x00CA5F27  0000                    add      byte ptr [eax], al             
  0x00CA5F29  8721                    xchg     dword ptr [ecx], esp           
  0x00CA5F2B  000c0c                  add      byte ptr [esp + ecx], cl       
  0x00CA5F2E  050001f044              add      eax, 0x44f00100                
  0x00CA5F33  008f04000044            add      byte ptr [edi + 0x44000004], cl 
  0x00CA5F3A  44                      inc      esp                            
  0x00CA5F3B  008e04000001            add      byte ptr [esi + 0x1000004], cl 
  0x00CA5F41  7054                    jo       0xca5f97                       
  0x00CA5F43  008b04000044            add      byte ptr [ebx + 0x44000004], cl 
  0x00CA5F49  7047                    jo       0xca5f92                       
  0x00CA5F4B  009104000074            add      byte ptr [ecx + 0x74000004], dl 
  0x00CA5F51  7054                    jo       0xca5fa7                       
  0x00CA5F53  008a0400001b            add      byte ptr [edx + 0x1b000004], cl 
  0x00CA5F59  0c05                    or       al, 5                          
  0x00CA5F5B  0000                    add      byte ptr [eax], al             
  0x00CA5F5E  56                      push     esi                            
  0x00CA5F5F  008b04000000            add      byte ptr [ebx + 4], cl         
  0x00CA5F66  44                      inc      esp                            
  0x00CA5F67  008d04000044            add      byte ptr [ebp + 0x44000004], cl 
  0x00CA5F6E  44                      inc      esp                            
  0x00CA5F6F  008f04000001            add      byte ptr [edi + 0x1000004], cl 
  0x00CA5F75  8621                    xchg     byte ptr [ecx], ah             
  0x00CA5F77  0044f045                add      byte ptr [eax + esi*8 + 0x45], al 
  0x00CA5F7B  008a04000055            add      byte ptr [edx + 0x55000004], cl 
  0x00CA5F82  44                      inc      esp                            
  0x00CA5F83  008c0400005090          add      byte ptr [esp + eax - 0x6fb00000], cl 
  0x00CA5F8A  0200                    add      al, byte ptr [eax]             
  0x00CA5F8C  61                      popal                                   
  0x00CA5F8D  7054                    jo       0xca5fe3                       
  0x00CA5F8F  008b04000044            add      byte ptr [ebx + 0x44000004], cl 
  0x00CA5F96  44                      inc      esp                            
                                        ; XREF: 0x00CA5F41 (cond_jump)
  0x00CA5F97  008e04000001            add      byte ptr [esi + 0x1000004], cl 
  0x00CA5F9D  8621                    xchg     byte ptr [ecx], ah             
  0x00CA5F9F  00447047                add      byte ptr [eax + esi*2 + 0x47], al 
  0x00CA5FA3  009104000055            add      byte ptr [ecx + 0x55000004], dl 
  0x00CA5FAA  44                      inc      esp                            
  0x00CA5FAB  008b04000050            add      byte ptr [ebx + 0x50000004], cl 
  0x00CA5FB1  90                      nop                                     
  0x00CA5FB2  0200                    add      al, byte ptr [eax]             
  0x00CA5FB4  7470                    je       0xca6026                       
  0x00CA5FB6  54                      push     esp                            
  0x00CA5FB7  008a04000045            add      byte ptr [edx + 0x45000004], cl 
  0x00CA5FBD  0020                    add      byte ptr [eax], ah             
  0x00CA5FBF  004090                  add      byte ptr [eax - 0x70], al      
  0x00CA5FC2  0200                    add      al, byte ptr [eax]             
  0x00CA5FC4  00f0                    add      al, dh                         
  0x00CA5FC6  44                      inc      esp                            
  0x00CA5FC7  00900400004c            add      byte ptr [eax + 0x4c000004], dl 
  0x00CA5FCD  de4e00                  fimul    word ptr [esi]                 
  0x00CA5FD0  851c0c                  test     dword ptr [esp + ecx], ebx     
  0x00CA5FD3  001429                  add      byte ptr [ecx + ebp], dl       
  0x00CA5FD6  2000                    and      byte ptr [eax], al             
  0x00CA5FD8  55                      push     ebp                            
  0x00CA5FD9  0020                    add      byte ptr [eax], ah             
  0x00CA5FDB  005090                  add      byte ptr [eax - 0x70], dl      
  0x00CA5FDE  0200                    add      al, byte ptr [eax]             
  0x00CA5FE0  006a54                  add      byte ptr [edx + 0x54], ch      
                                        ; XREF: 0x00CA5F8D (cond_jump)
  0x00CA5FE3  0000                    add      byte ptr [eax], al             
  0x00CA5FE5  0e                      push     cs                             
  0x00CA5FE6  2200                    and      al, byte ptr [eax]             
  0x00CA5FE8  00c4                    add      ah, al                         
  0x00CA5FEA  2300                    and      eax, dword ptr [eax]           
  0x00CA5FEC  45                      inc      ebp                            
  0x00CA5FED  5a                      pop      edx                            
  0x00CA5FEE  2000                    and      byte ptr [eax], al             
  0x00CA5FF0  d7                      xlatb                                   
  0x00CA5FF1  96                      xchg     esi, eax                       
  0x00CA5FF2  05000c0000              add      eax, 0xc00                     
  0x00CA5FF7  0000                    add      byte ptr [eax], al             
  0x00CA5FFA  56                      push     esi                            
  0x00CA5FFB  00b704000003            add      byte ptr [edi + 0x3000004], dh 
  0x00CA6002  44                      inc      esp                            
  0x00CA6003  00a204000007            add      byte ptr [edx + 0x7000004], ah 
  0x00CA6009  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA600A  050000ee20              add      eax, 0x20ee0000                
  0x00CA600F  00640024                add      byte ptr [eax + eax + 0x24], ah 
  0x00CA6013  0010                    add      byte ptr [eax], dl             
  0x00CA6015  cc                      int3                                    
  0x00CA6016  06                      push     es                             
  0x00CA6017  0002                    add      byte ptr [edx], al             
  0x00CA6019  0000                    add      byte ptr [eax], al             
  0x00CA601B  0000                    add      byte ptr [eax], al             
  0x00CA601D  59                      pop      ecx                            
  0x00CA601E  44                      inc      esp                            
  0x00CA601F  004d0c                  add      byte ptr [ebp + 0xc], cl       
  0x00CA6022  050000f464              add      eax, 0x64f40000                
  0x00CA6027  00ba0c000000            add      byte ptr [edx + 0xc], bh       
  0x00CA602D  f4                      hlt                                     
  0x00CA602E  6600a504000000          add      byte ptr [ebp + 4], ah         
  0x00CA6035  da5700                  ficom    dword ptr [edi]                
  0x00CA6038  4c                      dec      esp                            
  0x00CA603A  46                      inc      esi                            
  0x00CA603B  00bf0400005c            add      byte ptr [edi + 0x5c000004], bh 
  0x00CA6041  0020                    add      byte ptr [eax], ah             
  0x00CA6043  001b                    add      byte ptr [ebx], bl             
  0x00CA6045  2920                    sub      dword ptr [eax], esp           
  0x00CA6047  00ce                    add      dh, cl                         
  0x00CA6049  40                      inc      eax                            
  0x00CA604A  0100                    add      dword ptr [eax], eax           
  0x00CA604C  e01f                    loopne   0xca606d                       
  0x00CA604E  0000                    add      byte ptr [eax], al             
  0x00CA6050  58                      pop      eax                            
  0x00CA6051  dd5e00                  fstp     qword ptr [esi]                
  0x00CA6054  7500                    jne      0xca6056                       
                                        ; XREF: 0x00CA6054 (cond_jump)
  0x00CA6056  2000                    and      byte ptr [eax], al             
  0x00CA6058  7070                    jo       0xca60ca                       
  0x00CA605A  0200                    add      al, byte ptr [eax]             
  0x00CA605C  64c521                  lds      esp, ptr fs:[ecx]              
  0x00CA605F  0084410100009e          add      byte ptr [ecx + eax*2 - 0x61ffffff], al 
  0x00CA6066  2100                    and      dword ptr [eax], eax           
  0x00CA6068  00d8                    add      al, bl                         
  0x00CA606A  56                      push     esi                            
  0x00CA606B  0014f4                  add      byte ptr [esp + esi*8], dl     
  0x00CA606E  46                      inc      esi                            
  0x00CA606F  003f                    add      byte ptr [edi], bh             
  0x00CA6071  0000                    add      byte ptr [eax], al             
  0x00CA6073  0013                    add      byte ptr [ebx], dl             
  0x00CA6075  2920                    sub      dword ptr [eax], esp           
  0x00CA6077  00ca                    add      dl, cl                         
  0x00CA6079  1e                      push     ds                             
  0x00CA607A  0c00                    or       al, 0                          
  0x00CA607C  55                      push     ebp                            
  0x00CA607D  0020                    add      byte ptr [eax], ah             
  0x00CA607F  005070                  add      byte ptr [eax + 0x70], dl      
  0x00CA6082  0200                    add      al, byte ptr [eax]             
  0x00CA6084  009c210010de06          add      byte ptr [ecx + 0x6de1000], bl 
  0x00CA608B  000b                    add      byte ptr [ebx], cl             
  0x00CA608D  0000                    add      byte ptr [eax], al             
  0x00CA608F  0000                    add      byte ptr [eax], al             
  0x00CA6091  d85600                  fcom     dword ptr [esi]                
  0x00CA6094  14ec                    adc      al, 0xec                       
  0x00CA6096  7e00                    jle      0xca6098                       
                                        ; XREF: 0x00CA6096 (cond_jump)
  0x00CA6098  1329                    adc      ebp, dword ptr [ecx]           
  0x00CA609A  2000                    and      byte ptr [eax], al             
  0x00CA609C  ca1e0c                  retf     0xc1e                          
  0x00CA609F  005559                  add      byte ptr [ebp + 0x59], dl      
  0x00CA60A2  7600                    jbe      0xca60a4                       
                                        ; XREF: 0x00CA60A2 (cond_jump)
  0x00CA60A4  50                      push     eax                            
  0x00CA60A5  7002                    jo       0xca60a9                       
  0x00CA60A7  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA60A5 (cond_jump)
  0x00CA60A9  9c                      pushfd                                  
  0x00CA60AA  2100                    and      dword ptr [eax], eax           
  0x00CA60AC  00ee                    add      dh, ch                         
  0x00CA60AE  56                      push     esi                            
  0x00CA60AF  008041010000            add      byte ptr [eax + 0x141], al     
  0x00CA60B5  6e                      outsb    dx, byte ptr [esi]             
  0x00CA60B6  54                      push     esp                            
  0x00CA60B7  0000                    add      byte ptr [eax], al             
  0x00CA60B9  ec                      in       al, dx                         
  0x00CA60BA  7e00                    jle      0xca60bc                       
                                        ; XREF: 0x00CA60BA (cond_jump)
  0x00CA60BC  005976                  add      byte ptr [ecx + 0x76], bl      
  0x00CA60BF  0000                    add      byte ptr [eax], al             
  0x00CA60C1  ee                      out      dx, al                         
  0x00CA60C2  56                      push     esi                            
  0x00CA60C3  008041010071            add      byte ptr [eax + 0x71000141], al 
  0x00CA60C9  6e                      outsb    dx, byte ptr [esi]             
                                        ; XREF: 0x00CA6058 (cond_jump)
  0x00CA60CA  54                      push     esp                            
  0x00CA60CB  006500                  add      byte ptr [ebp], ah             
  0x00CA60CE  2000                    and      byte ptr [eax], al             
  0x00CA60D0  99                      cdq                                     
  0x00CA60D1  7705                    ja       0xca60d8                       
  0x00CA60D3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA60D6  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA60D1 (cond_jump)
  0x00CA60D8  00f4                    add      ah, dh                         
  0x00CA60DA  60                      pushal                                  
  0x00CA60DB  00680b                  add      byte ptr [eax + 0xb], ch       
  0x00CA60DE  0000                    add      byte ptr [eax], al             
  0x00CA60E0  00f0                    add      al, dh                         
  0x00CA60E2  7000                    jo       0xca60e4                       
                                        ; XREF: 0x00CA60E2 (cond_jump)
  0x00CA60E4  40                      inc      eax                            
  0x00CA60E5  0b00                    or       eax, dword ptr [eax]           
  0x00CA60E7  0000                    add      byte ptr [eax], al             
  0x00CA60E9  e8570000f0              call     0xf0ca6145                     
  0x00CA60EE  44                      inc      esp                            
  0x00CA60EF  007a0b                  add      byte ptr [edx + 0xb], bh       
  0x00CA60F2  0000                    add      byte ptr [eax], al             
  0x00CA60F4  4c                      dec      esp                            
  0x00CA60F5  0020                    add      byte ptr [eax], ah             
  0x00CA60F7  000b                    add      byte ptr [ebx], cl             
  0x00CA60F9  0020                    add      byte ptr [eax], ah             
  0x00CA60FB  000c14                  add      byte ptr [esp + edx], cl       
  0x00CA60FE  0500130020              add      eax, 0x20001300                
  0x00CA6103  0000                    add      byte ptr [eax], al             
  0x00CA6105  7056                    jo       0xca615d                       
  0x00CA6107  00660b                  add      byte ptr [esi + 0xb], ah       
  0x00CA610A  0000                    add      byte ptr [eax], al             
  0x00CA610C  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA610F  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA6112  0000                    add      byte ptr [eax], al             
  0x00CA6114  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA6117  00a40400000070          add      byte ptr [esp + eax + 0x70000000], ah 
  0x00CA611E  56                      push     esi                            
  0x00CA611F  006e0b                  add      byte ptr [esi + 0xb], ch       
  0x00CA6122  0000                    add      byte ptr [eax], al             
  0x00CA6124  c0100d                  rcl      byte ptr [eax], 0xd            
  0x00CA6127  0027                    add      byte ptr [edi], ah             
  0x00CA6129  0000                    add      byte ptr [eax], al             
  0x00CA612B  0013                    add      byte ptr [ebx], dl             
  0x00CA612D  0020                    add      byte ptr [eax], ah             
  0x00CA612F  0000                    add      byte ptr [eax], al             
  0x00CA6131  d821                    fsub     dword ptr [ecx]                
  0x00CA6133  0000                    add      byte ptr [eax], al             
  0x00CA6136  44                      inc      esp                            
  0x00CA6137  00700b                  add      byte ptr [eax + 0xb], dh       
  0x00CA613A  0000                    add      byte ptr [eax], al             
  0x00CA613C  45                      inc      ebp                            
  0x00CA613D  0020                    add      byte ptr [eax], ah             
  0x00CA613F  000494                  add      byte ptr [esp + edx*4], al     
  0x00CA6142  050000f456              add      eax, 0x56f40000                
  0x00CA6147  0009                    add      byte ptr [ecx], cl             
  0x00CA6149  0000                    add      byte ptr [eax], al             
  0x00CA614B  0000                    add      byte ptr [eax], al             
  0x00CA614D  d821                    fsub     dword ptr [ecx]                
  0x00CA614F  000500200009            add      byte ptr [0x9002000], al       
  0x00CA6155  f4                      hlt                                     
  0x00CA6156  0500130020              add      eax, 0x20001300                
  0x00CA615B  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA6105 (cond_jump)
  0x00CA615D  7056                    jo       0xca61b5                       
  0x00CA615F  00660b                  add      byte ptr [esi + 0xb], ah       
  0x00CA6162  0000                    add      byte ptr [eax], al             
  0x00CA6164  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA6167  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA616A  0000                    add      byte ptr [eax], al             
  0x00CA616C  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA616F  00a4040000130c          add      byte ptr [esp + eax + 0xc130000], ah 
  0x00CA6176  050000f456              add      eax, 0x56f40000                
  0x00CA617B  0001                    add      byte ptr [ecx], al             
  0x00CA617D  0000                    add      byte ptr [eax], al             
  0x00CA617F  0000                    add      byte ptr [eax], al             
  0x00CA6181  7056                    jo       0xca61d9                       
  0x00CA6183  00660b                  add      byte ptr [esi + 0xb], ah       
  0x00CA6186  0000                    add      byte ptr [eax], al             
  0x00CA6188  00ee                    add      dh, ch                         
  0x00CA618A  2100                    and      dword ptr [eax], eax           
  0x00CA618C  000423                  add      byte ptr [ebx], al             
  0x00CA618F  00440020                add      byte ptr [eax + eax + 0x20], al 
  0x00CA6193  0006                    add      byte ptr [esi], al             
  0x00CA6195  1c0c                    sbb      al, 0xc                        
  0x00CA6197  0000                    add      byte ptr [eax], al             
  0x00CA6199  0028                    add      byte ptr [eax], ch             
  0x00CA619B  0000                    add      byte ptr [eax], al             
  0x00CA619D  7056                    jo       0xca61f5                       
  0x00CA619F  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA61A2  0000                    add      byte ptr [eax], al             
  0x00CA61A4  06                      push     es                             
  0x00CA61A5  1d0c000004              sbb      eax, 0x400000c                 
  0x00CA61AA  2300                    and      eax, dword ptr [eax]           
  0x00CA61AC  40                      inc      eax                            
  0x00CA61AD  0020                    add      byte ptr [eax], ah             
  0x00CA61AF  0000                    add      byte ptr [eax], al             
  0x00CA61B1  7056                    jo       0xca6209                       
  0x00CA61B3  00a404000000c4          add      byte ptr [esp + eax - 0x3c000000], ah 
  0x00CA61BA  2100                    and      dword ptr [eax], eax           
  0x00CA61BC  4c                      dec      esp                            
  0x00CA61BD  0020                    add      byte ptr [eax], ah             
  0x00CA61BF  0000                    add      byte ptr [eax], al             
  0x00CA61C2  56                      push     esi                            
  0x00CA61C3  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA61C6  0000                    add      byte ptr [eax], al             
  0x00CA61C8  00f4                    add      ah, dh                         
  0x00CA61CA  44                      inc      esp                            
  0x00CA61CB  000500000045            add      byte ptr [0x45000000], al      
  0x00CA61D1  0020                    add      byte ptr [eax], ah             
  0x00CA61D3  0013                    add      byte ptr [ebx], dl             
  0x00CA61D5  2405                    and      al, 5                          
  0x00CA61D7  0000                    add      byte ptr [eax], al             
  0x00CA61DA  56                      push     esi                            
  0x00CA61DB  009f0b000000            add      byte ptr [edi + 0xb], bl       
  0x00CA61E2  44                      inc      esp                            
  0x00CA61E3  007a0b                  add      byte ptr [edx + 0xb], bh       
  0x00CA61E6  0000                    add      byte ptr [eax], al             
  0x00CA61E8  44                      inc      esp                            
  0x00CA61E9  0020                    add      byte ptr [eax], ah             
  0x00CA61EB  0000                    add      byte ptr [eax], al             
  0x00CA61EE  44                      inc      esp                            
  0x00CA61EF  00530b                  add      byte ptr [ebx + 0xb], dl       
  0x00CA61F2  0000                    add      byte ptr [eax], al             
  0x00CA61F4  44                      inc      esp                            
                                        ; XREF: 0x00CA619D (cond_jump)
  0x00CA61F5  0020                    add      byte ptr [eax], ah             
  0x00CA61F7  0000                    add      byte ptr [eax], al             
  0x00CA61F9  d921                    fldenv   [ecx]                          
  0x00CA61FB  0000                    add      byte ptr [eax], al             
  0x00CA61FD  7057                    jo       0xca6256                       
  0x00CA61FF  006e0b                  add      byte ptr [esi + 0xb], ch       
  0x00CA6202  0000                    add      byte ptr [eax], al             
  0x00CA6204  0300                    add      eax, dword ptr [eax]           
  0x00CA6206  2000                    and      byte ptr [eax], al             
  0x00CA6208  0494                    add      al, 0x94                       
  0x00CA620A  0500050020              add      eax, 0x20000500                
  0x00CA620F  0002                    add      byte ptr [edx], al             
  0x00CA6211  f4                      hlt                                     
  0x00CA6212  0500030c05              add      eax, 0x50c0300                 
  0x00CA6217  0000                    add      byte ptr [eax], al             
  0x00CA6219  2423                    and      al, 0x23                       
  0x00CA621B  0000                    add      byte ptr [eax], al             
  0x00CA621E  2000                    and      byte ptr [eax], al             
  0x00CA6220  00f0                    add      al, dh                         
  0x00CA6222  56                      push     esi                            
  0x00CA6223  00700b                  add      byte ptr [eax + 0xb], dh       
  0x00CA6226  0000                    add      byte ptr [eax], al             
  0x00CA6228  0300                    add      eax, dword ptr [eax]           
  0x00CA622A  2000                    and      byte ptr [eax], al             
  0x00CA622C  0af4                    or       dh, ah                         
  0x00CA622E  050000f444              add      eax, 0x44f40000                
  0x00CA6233  0001                    add      byte ptr [ecx], al             
  0x00CA6235  0000                    add      byte ptr [eax], al             
  0x00CA6237  0000                    add      byte ptr [eax], al             
  0x00CA6239  7044                    jo       0xca627f                       
  0x00CA623B  00660b                  add      byte ptr [esi + 0xb], ah       
  0x00CA623E  0000                    add      byte ptr [eax], al             
  0x00CA6240  00f0                    add      al, dh                         
  0x00CA6242  44                      inc      esp                            
  0x00CA6243  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA6246  0000                    add      byte ptr [eax], al             
  0x00CA6248  40                      inc      eax                            
  0x00CA6249  0020                    add      byte ptr [eax], ah             
  0x00CA624B  0000                    add      byte ptr [eax], al             
  0x00CA624D  7056                    jo       0xca62a5                       
  0x00CA624F  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA6252  0000                    add      byte ptr [eax], al             
  0x00CA6254  0c00                    or       al, 0                          
                                        ; XREF: 0x00CA61FD (cond_jump)
  0x00CA6256  0000                    add      byte ptr [eax], al             
  0x00CA6258  0011                    add      byte ptr [ecx], dl             
  0x00CA625A  2200                    and      al, byte ptr [eax]             
  0x00CA625C  00b2220069f4            add      byte ptr [edx - 0xb96ffde], dh 
  0x00CA6262  46                      inc      esi                            
  0x00CA6263  0002                    add      byte ptr [edx], al             
  0x00CA6265  0000                    add      byte ptr [eax], al             
  0x00CA6267  0010                    add      byte ptr [eax], dl             
  0x00CA6269  d806                    fadd     dword ptr [esi]                
  0x00CA626B  000500000000            add      byte ptr [0], al               
  0x00CA6271  c9                      leave                                   
  0x00CA6272  56                      push     esi                            
  0x00CA6273  00148f                  add      byte ptr [edi + ecx*4], dl     
  0x00CA6276  2100                    and      dword ptr [eax], eax           
  0x00CA6278  50                      push     eax                            
  0x00CA6279  0020                    add      byte ptr [eax], ah             
  0x00CA627B  0000                    add      byte ptr [eax], al             
  0x00CA627D  5a                      pop      edx                            
  0x00CA627E  54                      push     esp                            
                                        ; XREF: 0x00CA6239 (cond_jump)
  0x00CA627F  0000                    add      byte ptr [eax], al             
  0x00CA6281  4e                      dec      esi                            
  0x00CA6282  2300                    and      eax, dword ptr [eax]           
  0x00CA6284  32442300                xor      al, byte ptr [ebx]             
  0x00CA6288  40                      inc      eax                            
  0x00CA6289  0423                    add      al, 0x23                       
  0x00CA628B  00440024                add      byte ptr [eax + eax + 0x24], al 
  0x00CA628F  0004a4                  add      byte ptr [esp], al             
  0x00CA6292  050010cc06              add      eax, 0x6cc1000                 
  0x00CA6297  0002                    add      byte ptr [edx], al             
  0x00CA6299  0000                    add      byte ptr [eax], al             
  0x00CA629B  0000                    add      byte ptr [eax], al             
  0x00CA629D  5a                      pop      edx                            
  0x00CA629E  44                      inc      esp                            
  0x00CA629F  0000                    add      byte ptr [eax], al             
  0x00CA62A1  b022                    mov      al, 0x22                       
  0x00CA62A3  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA624D (cond_jump)
  0x00CA62A5  91                      xchg     ecx, eax                       
  0x00CA62A6  2200                    and      al, byte ptr [eax]             
  0x00CA62A8  00f4                    add      ah, dh                         
  0x00CA62AA  65000d0d000000          add      byte ptr gs:[0xd], cl          
  0x00CA62B1  f4                      hlt                                     
  0x00CA62B2  7500                    jne      0xca62b4                       
  0x00CA62B6  ff00                    inc      dword ptr [eax]                
  0x00CA62B8  10da                    adc      dl, bl                         
  0x00CA62BA  06                      push     es                             
  0x00CA62BB  0007                    add      byte ptr [edi], al             
  0x00CA62BD  0000                    add      byte ptr [eax], al             
  0x00CA62BF  0000                    add      byte ptr [eax], al             
  0x00CA62C1  b8f000d0b8              mov      eax, 0xb8d000f0                
  0x00CA62C7  00d2                    add      dl, dl                         
  0x00CA62C9  b8d000d200              mov      eax, 0xd200d0                  
  0x00CA62CE  2000                    and      byte ptr [eax], al             
  0x00CA62D0  2200                    and      al, byte ptr [eax]             
  0x00CA62D2  2000                    and      byte ptr [eax], al             
  0x00CA62D4  005958                  add      byte ptr [ecx + 0x58], bl      
  0x00CA62D7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA62DA  0000                    add      byte ptr [eax], al             
  0x00CA62DC  20f4                    and      ah, dh                         
  0x00CA62DE  0500ffffff              add      eax, 0xffffff00                
  0x00CA62E3  00a0610400a0            add      byte ptr [eax - 0x5ffffb9f], ah 
  0x00CA62E9  620400                  bound    eax, qword ptr [eax + eax]     
  0x00CA62EC  a0640400a0              mov      al, byte ptr [0xa0000464]      
  0x00CA62F1  650400                  add      al, 0                          
  0x00CA62F4  a0660400b8              mov      al, byte ptr [0xb8000466]      
  0x00CA62F9  f30000                  add      byte ptr [eax], al             
  0x00CA62FC  00f4                    add      ah, dh                         
  0x00CA62FE  44                      inc      esp                            
  0x00CA62FF  0000                    add      byte ptr [eax], al             
  0x00CA6301  0000                    add      byte ptr [eax], al             
  0x00CA6303  004d00                  add      byte ptr [ebp], cl             
  0x00CA6306  2000                    and      byte ptr [eax], al             
  0x00CA6308  0ca4                    or       al, 0xa4                       
  0x00CA630A  050000f444              add      eax, 0x44f40000                
  0x00CA630F  0010                    add      byte ptr [eax], dl             
  0x00CA6311  0000                    add      byte ptr [eax], al             
  0x00CA6313  004d00                  add      byte ptr [ebp], cl             
  0x00CA6316  2000                    and      byte ptr [eax], al             
  0x00CA6318  4a                      dec      edx                            
  0x00CA6319  100d000f0000            adc      byte ptr [0xf00], cl           
  0x00CA631F  0000                    add      byte ptr [eax], al             
  0x00CA6321  0030                    add      byte ptr [eax], dh             
  0x00CA6323  0000                    add      byte ptr [eax], al             
  0x00CA6325  f4                      hlt                                     
  0x00CA6326  56                      push     esi                            
  0x00CA6327  0000                    add      byte ptr [eax], al             
  0x00CA6329  0000                    add      byte ptr [eax], al             
  0x00CA632B  0000                    add      byte ptr [eax], al             
  0x00CA632D  f4                      hlt                                     
  0x00CA632E  57                      push     edi                            
  0x00CA632F  00ff                    add      bh, bh                         
  0x00CA6332  ff00                    inc      dword ptr [eax]                
  0x00CA6334  0c00                    or       al, 0                          
  0x00CA6336  0000                    add      byte ptr [eax], al             
  0x00CA6338  1300                    adc      eax, dword ptr [eax]           
  0x00CA633A  2000                    and      byte ptr [eax], al             
  0x00CA633C  0000                    add      byte ptr [eax], al             
  0x00CA633E  3000                    xor      byte ptr [eax], al             
  0x00CA6340  00f4                    add      ah, dh                         
  0x00CA6342  56                      push     esi                            
  0x00CA6343  0000                    add      byte ptr [eax], al             
  0x00CA6345  0000                    add      byte ptr [eax], al             
  0x00CA6347  0000                    add      byte ptr [eax], al             
  0x00CA6349  f4                      hlt                                     
  0x00CA634A  57                      push     edi                            
  0x00CA634B  0008                    add      byte ptr [eax], cl             
  0x00CA634D  06                      push     es                             
  0x00CA634E  0000                    add      byte ptr [eax], al             
  0x00CA6350  0c00                    or       al, 0                          
  0x00CA6352  0000                    add      byte ptr [eax], al             
  0x00CA6354  00f0                    add      al, dh                         
  0x00CA6356  56                      push     esi                            
  0x00CA6357  00960b000003            add      byte ptr [esi + 0x300000b], dl 
  0x00CA635D  0020                    add      byte ptr [eax], ah             
  0x00CA635F  001e                    add      byte ptr [esi], bl             
  0x00CA6361  7405                    je       0xca6368                       
  0x00CA6363  0000                    add      byte ptr [eax], al             
  0x00CA6366  56                      push     esi                            
  0x00CA6367  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA636A  0000                    add      byte ptr [eax], al             
  0x00CA636C  0300                    add      eax, dword ptr [eax]           
  0x00CA636E  2e0002                  add      byte ptr cs:[edx], al          
  0x00CA6371  2405                    and      al, 5                          
  0x00CA6373  008041010000            add      byte ptr [eax + 0x141], al     
  0x00CA6379  7056                    jo       0xca63d1                       
  0x00CA637B  00890b000080            add      byte ptr [ecx - 0x7ffffff5], cl 
  0x00CA6381  100d008b0300            adc      byte ptr [0x38b00], cl         
  0x00CA6387  0000                    add      byte ptr [eax], al             
  0x00CA6389  f4                      hlt                                     
  0x00CA638A  44                      inc      esp                            
  0x00CA638B  00b007000000            add      byte ptr [eax + 7], dh         
  0x00CA6391  7044                    jo       0xca63d7                       
  0x00CA6393  00720b                  add      byte ptr [edx + 0xb], dh       
  0x00CA6396  0000                    add      byte ptr [eax], al             
  0x00CA6398  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA639B  001400                  add      byte ptr [eax + eax], dl       
  0x00CA639E  0000                    add      byte ptr [eax], al             
  0x00CA63A0  00f0                    add      al, dh                         
  0x00CA63A2  56                      push     esi                            
  0x00CA63A3  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA63A6  0000                    add      byte ptr [eax], al             
  0x00CA63A8  0300                    add      eax, dword ptr [eax]           
  0x00CA63AA  2e0003                  add      byte ptr cs:[ebx], al          
  0x00CA63AD  2405                    and      al, 5                          
  0x00CA63AF  0080100d0053            add      byte ptr [eax + 0x53000d10], al 
  0x00CA63B5  0300                    add      eax, dword ptr [eax]           
  0x00CA63B7  0080100d009d            add      byte ptr [eax - 0x62fff2f0], al 
  0x00CA63BD  0000                    add      byte ptr [eax], al             
  0x00CA63BF  0000                    add      byte ptr [eax], al             
  0x00CA63C2  56                      push     esi                            
  0x00CA63C3  00960b000003            add      byte ptr [esi + 0x300000b], dl 
  0x00CA63C9  0020                    add      byte ptr [eax], ah             
  0x00CA63CB  0003                    add      byte ptr [ebx], al             
  0x00CA63CD  2405                    and      al, 5                          
  0x00CA63CF  0080100d009c            add      byte ptr [eax - 0x63fff2f0], al 
  0x00CA63D5  0100                    add      dword ptr [eax], eax           
                                        ; XREF: 0x00CA6391 (cond_jump)
  0x00CA63D7  0013                    add      byte ptr [ebx], dl             
  0x00CA63D9  0020                    add      byte ptr [eax], ah             
  0x00CA63DB  001b                    add      byte ptr [ebx], bl             
  0x00CA63DD  1021                    adc      byte ptr [ecx], ah             
  0x00CA63DF  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA63E2  0000                    add      byte ptr [eax], al             
  0x00CA63E4  0c00                    or       al, 0                          
  0x00CA63E6  0000                    add      byte ptr [eax], al             
  0x00CA63E8  00f4                    add      ah, dh                         
  0x00CA63EA  44                      inc      esp                            
  0x00CA63EB  0001                    add      byte ptr [ecx], al             
  0x00CA63ED  0000                    add      byte ptr [eax], al             
  0x00CA63EF  0000                    add      byte ptr [eax], al             
  0x00CA63F1  7044                    jo       0xca6437                       
  0x00CA63F3  006f0b                  add      byte ptr [edi + 0xb], ch       
  0x00CA63F6  0000                    add      byte ptr [eax], al             
  0x00CA63F8  1bf0                    sbb      esi, eax                       
  0x00CA63FA  56                      push     esi                            
  0x00CA63FB  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA63FE  0000                    add      byte ptr [eax], al             
  0x00CA6400  0300                    add      eax, dword ptr [eax]           
  0x00CA6402  2000                    and      byte ptr [eax], al             
  0x00CA6404  02240500090000          add      ah, byte ptr [eax + 0x900]     
  0x00CA640B  0000                    add      byte ptr [eax], al             
  0x00CA640D  7051                    jo       0xca6460                       
  0x00CA640F  00c0                    add      al, al                         
  0x00CA6411  0400                    add      al, 0                          
  0x00CA6413  0000                    add      byte ptr [eax], al             
  0x00CA6415  0230                    add      dh, byte ptr [eax]             
  0x00CA6417  0000                    add      byte ptr [eax], al             
  0x00CA6419  0131                    add      dword ptr [ecx], esi           
  0x00CA641B  0000                    add      byte ptr [eax], al             
  0x00CA641D  0132                    add      dword ptr [edx], esi           
  0x00CA641F  0000                    add      byte ptr [eax], al             
  0x00CA6421  023500c4700b            add      dh, byte ptr [0xb70c400]       
  0x00CA6427  0008                    add      byte ptr [eax], cl             
  0x00CA6429  0c00                    or       al, 0                          
  0x00CA642B  0000                    add      byte ptr [eax], al             
  0x00CA642D  7044                    jo       0xca6473                       
  0x00CA642F  008d040000c4            add      byte ptr [ebp - 0x3bfffffc], cl 
  0x00CA6435  710b                    jno      0xca6442                       
                                        ; XREF: 0x00CA63F1 (cond_jump)
  0x00CA6437  00040c                  add      byte ptr [esp + ecx], al       
  0x00CA643A  0000                    add      byte ptr [eax], al             
  0x00CA643C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA643F  008c040000c472          add      byte ptr [esp + eax + 0x72c40000], cl 
  0x00CA6446  0b00                    or       eax, dword ptr [eax]           
  0x00CA6448  140c                    adc      al, 0xc                        
  0x00CA644A  0000                    add      byte ptr [eax], al             
  0x00CA644C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA644F  008f040000c4            add      byte ptr [edi - 0x3bfffffc], cl 
  0x00CA6455  750b                    jne      0xca6462                       
  0x00CA6457  0018                    add      byte ptr [eax], bl             
  0x00CA6459  0c00                    or       al, 0                          
  0x00CA645B  0000                    add      byte ptr [eax], al             
  0x00CA645D  7044                    jo       0xca64a3                       
  0x00CA645F  009004000000            add      byte ptr [eax + 4], dl         
  0x00CA6465  0036                    add      byte ptr [esi], dh             
  0x00CA6467  0000                    add      byte ptr [eax], al             
  0x00CA646A  44                      inc      esp                            
  0x00CA646B  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA6471  c406                    les      eax, ptr [esi]                 
                                        ; XREF: 0x00CA642D (cond_jump)
  0x00CA6473  004e00                  add      byte ptr [esi], cl             
  0x00CA6476  0000                    add      byte ptr [eax], al             
  0x00CA6478  0001                    add      byte ptr [ecx], al             
  0x00CA647A  2800                    sub      byte ptr [eax], al             
  0x00CA647C  007050                  add      byte ptr [eax + 0x50], dh      
  0x00CA647F  00c0                    add      al, al                         
  0x00CA6481  0400                    add      al, 0                          
  0x00CA6483  0000                    add      byte ptr [eax], al             
  0x00CA6485  0430                    add      al, 0x30                       
  0x00CA6487  00c4                    add      ah, al                         
  0x00CA6489  700b                    jo       0xca6496                       
  0x00CA648B  000c0c                  add      byte ptr [esp + ecx], cl       
  0x00CA648E  0000                    add      byte ptr [eax], al             
  0x00CA6490  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA6493  008e04000000            add      byte ptr [esi + 4], cl         
  0x00CA6499  f4                      hlt                                     
  0x00CA649A  44                      inc      esp                            
  0x00CA649B  0000                    add      byte ptr [eax], al             
  0x00CA649D  80ff00                  cmp      bh, 0                          
  0x00CA64A0  007044                  add      byte ptr [eax + 0x44], dh      
                                        ; XREF: 0x00CA645D (cond_jump)
  0x00CA64A3  008a04000000            add      byte ptr [edx + 4], cl         
  0x00CA64A9  f4                      hlt                                     
  0x00CA64AA  44                      inc      esp                            
  0x00CA64AB  0000                    add      byte ptr [eax], al             
  0x00CA64AD  80ff00                  cmp      bh, 0                          
  0x00CA64B0  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA64B3  008b04000000            add      byte ptr [ebx + 4], cl         
  0x00CA64B9  c422                    les      esp, ptr [edx]                 
  0x00CA64BB  0000                    add      byte ptr [eax], al             
  0x00CA64BD  f4                      hlt                                     
  0x00CA64BE  46                      inc      esi                            
  0x00CA64BF  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA64C5  f4                      hlt                                     
  0x00CA64C6  44                      inc      esp                            
  0x00CA64C7  00fa                    add      dl, bh                         
  0x00CA64C9  0000                    add      byte ptr [eax], al             
  0x00CA64CB  002e                    add      byte ptr [esi], ch             
  0x00CA64CD  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA64D2  2000                    and      byte ptr [eax], al             
  0x00CA64D4  0090210000c4            add      byte ptr [eax - 0x3bffffdf], dl 
  0x00CA64DA  2200                    and      al, byte ptr [eax]             
  0x00CA64DC  00f4                    add      ah, dh                         
  0x00CA64DE  46                      inc      esi                            
  0x00CA64DF  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA64E5  f4                      hlt                                     
  0x00CA64E6  44                      inc      esp                            
  0x00CA64E7  00fa                    add      dl, bh                         
  0x00CA64E9  0000                    add      byte ptr [eax], al             
  0x00CA64EB  002e                    add      byte ptr [esi], ch             
  0x00CA64ED  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA64F2  2000                    and      byte ptr [eax], al             
  0x00CA64F4  0095210000c4            add      byte ptr [ebp - 0x3bffffdf], dl 
  0x00CA64FA  2200                    and      al, byte ptr [eax]             
  0x00CA64FC  00f4                    add      ah, dh                         
  0x00CA64FE  46                      inc      esi                            
  0x00CA64FF  0032                    add      byte ptr [edx], dh             
  0x00CA6501  0000                    add      byte ptr [eax], al             
  0x00CA6503  00d0                    add      al, dl                         
  0x00CA6505  f4                      hlt                                     
  0x00CA6506  44                      inc      esp                            
  0x00CA6507  0000                    add      byte ptr [eax], al             
  0x00CA6509  0000                    add      byte ptr [eax], al             
  0x00CA650B  002e                    add      byte ptr [esi], ch             
  0x00CA650D  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA6512  2000                    and      byte ptr [eax], al             
  0x00CA6514  009a21000000            add      byte ptr [edx + 0x21], bl      
  0x00CA651A  3800                    cmp      byte ptr [eax], al             
  0x00CA651C  00f4                    add      ah, dh                         
  0x00CA651E  56                      push     esi                            
  0x00CA651F  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA6525  c422                    les      esp, ptr [edx]                 
  0x00CA6527  004000                  add      byte ptr [eax], al             
  0x00CA652A  2000                    and      byte ptr [eax], al             
  0x00CA652C  0091210000e1            add      byte ptr [ecx - 0x1effffdf], dl 
  0x00CA6532  7400                    je       0xca6534                       
                                        ; XREF: 0x00CA6532 (cond_jump)
  0x00CA6534  00e1                    add      cl, ah                         
  0x00CA6536  7600                    jbe      0xca6538                       
                                        ; XREF: 0x00CA6536 (cond_jump)
  0x00CA6538  0000                    add      byte ptr [eax], al             
  0x00CA653A  3200                    xor      al, byte ptr [eax]             
  0x00CA653C  007066                  add      byte ptr [eax + 0x66], dh      
  0x00CA653F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA6542  0000                    add      byte ptr [eax], al             
  0x00CA6544  d7                      xlatb                                   
  0x00CA6545  030d0000f066            add      ecx, dword ptr [0x66f00000]    
  0x00CA654B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA654E  0000                    add      byte ptr [eax], al             
  0x00CA6550  00f4                    add      ah, dh                         
  0x00CA6552  56                      push     esi                            
  0x00CA6553  00c1                    add      cl, al                         
  0x00CA6555  0400                    add      al, 0                          
  0x00CA6557  0000                    add      byte ptr [eax], al             
  0x00CA6559  c422                    les      esp, ptr [edx]                 
  0x00CA655B  004000                  add      byte ptr [eax], al             
  0x00CA655E  2000                    and      byte ptr [eax], al             
  0x00CA6560  009021000060            add      byte ptr [eax + 0x60000021], dl 
  0x00CA6566  6200                    bound    eax, qword ptr [eax]           
  0x00CA6568  00f4                    add      ah, dh                         
  0x00CA656A  56                      push     esi                            
  0x00CA656B  009404000000c4          add      byte ptr [esp + eax - 0x3c000000], dl 
  0x00CA6572  2200                    and      al, byte ptr [eax]             
  0x00CA6574  40                      inc      eax                            
  0x00CA6575  0020                    add      byte ptr [eax], ah             
  0x00CA6577  0000                    add      byte ptr [eax], al             
  0x00CA6579  90                      nop                                     
  0x00CA657A  2100                    and      dword ptr [eax], eax           
  0x00CA657C  00f0                    add      al, dh                         
  0x00CA657E  44                      inc      esp                            
  0x00CA657F  008a04000000            add      byte ptr [edx + 4], cl         
  0x00CA6585  60                      pushal                                  
  0x00CA6586  44                      inc      esp                            
  0x00CA6587  0000                    add      byte ptr [eax], al             
  0x00CA6589  f4                      hlt                                     
  0x00CA658A  56                      push     esi                            
  0x00CA658B  009904000000            add      byte ptr [ecx + 4], bl         
  0x00CA6591  c422                    les      esp, ptr [edx]                 
  0x00CA6593  004000                  add      byte ptr [eax], al             
  0x00CA6596  2000                    and      byte ptr [eax], al             
  0x00CA6598  0090210000f0            add      byte ptr [eax - 0xfffffdf], dl 
  0x00CA659E  44                      inc      esp                            
  0x00CA659F  008b04000000            add      byte ptr [ebx + 4], cl         
  0x00CA65A5  60                      pushal                                  
  0x00CA65A6  44                      inc      esp                            
  0x00CA65A7  0000                    add      byte ptr [eax], al             
  0x00CA65A9  5e                      pop      esi                            
  0x00CA65AA  2000                    and      byte ptr [eax], al             
  0x00CA65AC  00f0                    add      al, dh                         
  0x00CA65AE  56                      push     esi                            
  0x00CA65AF  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA65B2  0000                    add      byte ptr [eax], al             
  0x00CA65B4  0300                    add      eax, dword ptr [eax]           
  0x00CA65B6  2000                    and      byte ptr [eax], al             
  0x00CA65B8  1ca4                    sbb      al, 0xa4                       
  0x00CA65BA  0500000128              add      eax, 0x28010000                
  0x00CA65BF  0000                    add      byte ptr [eax], al             
  0x00CA65C1  7050                    jo       0xca6613                       
  0x00CA65C3  00c0                    add      al, al                         
  0x00CA65C5  0400                    add      al, 0                          
  0x00CA65C7  0000                    add      byte ptr [eax], al             
  0x00CA65C9  0430                    add      al, 0x30                       
  0x00CA65CB  00c4                    add      ah, al                         
  0x00CA65CD  700b                    jo       0xca65da                       
  0x00CA65CF  000c0c                  add      byte ptr [esp + ecx], cl       
  0x00CA65D2  0000                    add      byte ptr [eax], al             
  0x00CA65D4  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA65D7  008e04000000            add      byte ptr [esi + 4], cl         
  0x00CA65DD  f4                      hlt                                     
  0x00CA65DE  44                      inc      esp                            
  0x00CA65DF  0000                    add      byte ptr [eax], al             
  0x00CA65E1  80ff00                  cmp      bh, 0                          
  0x00CA65E4  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA65E7  008a04000000            add      byte ptr [edx + 4], cl         
  0x00CA65ED  f4                      hlt                                     
  0x00CA65EE  44                      inc      esp                            
  0x00CA65EF  0000                    add      byte ptr [eax], al             
  0x00CA65F1  80ff00                  cmp      bh, 0                          
  0x00CA65F4  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA65F7  008b04000000            add      byte ptr [ebx + 4], cl         
  0x00CA65FD  f4                      hlt                                     
  0x00CA65FE  60                      pushal                                  
  0x00CA65FF  008304000000            add      byte ptr [ebx + 4], al         
  0x00CA6605  f4                      hlt                                     
  0x00CA6606  65008304000000          add      byte ptr gs:[ebx + 4], al      
  0x00CA660D  f4                      hlt                                     
  0x00CA660E  7200                    jb       0xca6610                       
                                        ; XREF: 0x00CA660E (cond_jump)
  0x00CA6610  b804000000              mov      eax, 4                         
  0x00CA6615  0038                    add      byte ptr [eax], bh             
  0x00CA6617  0000                    add      byte ptr [eax], al             
  0x00CA6619  07                      pop      es                             
  0x00CA661A  3c00                    cmp      al, 0                          
  0x00CA661C  0007                    add      byte ptr [edi], al             
  0x00CA661E  3e0000                  add      byte ptr ds:[eax], al          
  0x00CA6621  0032                    add      byte ptr [edx], dh             
  0x00CA6623  00d7                    add      bh, dl                         
  0x00CA6625  030d000c0000            add      ecx, dword ptr [0xc00]         
  0x00CA662B  0000                    add      byte ptr [eax], al             
  0x00CA662E  56                      push     esi                            
  0x00CA662F  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA6632  0000                    add      byte ptr [eax], al             
  0x00CA6634  0300                    add      eax, dword ptr [eax]           
  0x00CA6636  2000                    and      byte ptr [eax], al             
  0x00CA6638  0424                    add      al, 0x24                       
  0x00CA663A  0500000024              add      eax, 0x24000000                
  0x00CA663F  0000                    add      byte ptr [eax], al             
  0x00CA6641  7044                    jo       0xca6687                       
  0x00CA6643  006e0b                  add      byte ptr [esi + 0xb], ch       
  0x00CA6646  0000                    add      byte ptr [eax], al             
  0x00CA6648  0000                    add      byte ptr [eax], al             
  0x00CA664A  3400                    xor      al, 0                          
  0x00CA664C  1b00                    sbb      eax, dword ptr [eax]           
  0x00CA664E  2000                    and      byte ptr [eax], al             
  0x00CA6650  00f0                    add      al, dh                         
  0x00CA6652  44                      inc      esp                            
  0x00CA6653  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA6659  c406                    les      eax, ptr [esi]                 
  0x00CA665B  0003                    add      byte ptr [ebx], al             
  0x00CA665D  0000                    add      byte ptr [eax], al             
  0x00CA665F  008841010088            add      byte ptr [eax - 0x77fffebf], cl 
  0x00CA6665  41                      inc      ecx                            
  0x00CA6666  0100                    add      dword ptr [eax], eax           
  0x00CA6668  884101                  mov      byte ptr [ecx + 1], al         
  0x00CA666B  0000                    add      byte ptr [eax], al             
  0x00CA666E  56                      push     esi                            
  0x00CA666F  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA6672  0000                    add      byte ptr [eax], al             
  0x00CA6674  0300                    add      eax, dword ptr [eax]           
  0x00CA6676  2000                    and      byte ptr [eax], al             
  0x00CA6678  02240500884101          add      ah, byte ptr [eax + 0x1418800] 
  0x00CA667F  0000                    add      byte ptr [eax], al             
  0x00CA6682  56                      push     esi                            
  0x00CA6683  004a0b                  add      byte ptr [edx + 0xb], cl       
  0x00CA6686  0000                    add      byte ptr [eax], al             
  0x00CA6688  03f4                    add      esi, esp                       
  0x00CA668A  44                      inc      esp                            
  0x00CA668B  0008                    add      byte ptr [eax], cl             
  0x00CA668D  0000                    add      byte ptr [eax], al             
  0x00CA668F  0002                    add      byte ptr [edx], al             
  0x00CA6691  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA6692  0500480020              add      eax, 0x20004800                
  0x00CA6697  008841010000            add      byte ptr [eax + 0x141], cl     
  0x00CA669E  56                      push     esi                            
  0x00CA669F  00890b000003            add      byte ptr [ecx + 0x300000b], cl 
  0x00CA66A5  0020                    add      byte ptr [eax], ah             
  0x00CA66A7  0002                    add      byte ptr [edx], al             
  0x00CA66A9  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA66AA  0500884101              add      eax, 0x1418800                 
  0x00CA66AF  0000                    add      byte ptr [eax], al             
  0x00CA66B2  56                      push     esi                            
  0x00CA66B3  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA66B6  0000                    add      byte ptr [eax], al             
  0x00CA66B8  854201                  test     dword ptr [edx + 1], eax       
  0x00CA66BB  0007                    add      byte ptr [edi], al             
  0x00CA66BD  2405                    and      al, 5                          
  0x00CA66BF  008841010000            add      byte ptr [eax + 0x141], cl     
  0x00CA66C6  56                      push     esi                            
  0x00CA66C7  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA66CA  0000                    add      byte ptr [eax], al             
  0x00CA66CC  03f4                    add      esi, esp                       
  0x00CA66CE  44                      inc      esp                            
  0x00CA66CF  000400                  add      byte ptr [eax + eax], al       
  0x00CA66D2  0000                    add      byte ptr [eax], al             
  0x00CA66D4  48                      dec      eax                            
  0x00CA66D5  2a20                    sub      ah, byte ptr [eax]             
  0x00CA66D7  0000                    add      byte ptr [eax], al             
  0x00CA66DA  44                      inc      esp                            
  0x00CA66DB  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA66E1  c406                    les      eax, ptr [esi]                 
  0x00CA66E3  0002                    add      byte ptr [edx], al             
  0x00CA66E5  0000                    add      byte ptr [eax], al             
  0x00CA66E7  008842010000            add      byte ptr [eax + 0x142], cl     
  0x00CA66EE  56                      push     esi                            
  0x00CA66EF  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA66F2  0000                    add      byte ptr [eax], al             
  0x00CA66F4  0300                    add      eax, dword ptr [eax]           
  0x00CA66F6  2000                    and      byte ptr [eax], al             
  0x00CA66F8  02a40500884101          add      ah, byte ptr [ebp + eax + 0x1418800] 
  0x00CA66FF  0000                    add      byte ptr [eax], al             
  0x00CA6701  f4                      hlt                                     
  0x00CA6702  56                      push     esi                            
  0x00CA6703  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA6709  002400                  add      byte ptr [eax + eax], ah       
  0x00CA670C  40                      inc      eax                            
  0x00CA670D  0020                    add      byte ptr [eax], ah             
  0x00CA670F  0000                    add      byte ptr [eax], al             
  0x00CA6711  90                      nop                                     
  0x00CA6712  2100                    and      dword ptr [eax], eax           
  0x00CA6714  00f0                    add      al, dh                         
  0x00CA6716  44                      inc      esp                            
  0x00CA6717  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA671D  c406                    les      eax, ptr [esi]                 
  0x00CA671F  0006                    add      byte ptr [esi], al             
  0x00CA6721  0000                    add      byte ptr [eax], al             
  0x00CA6723  0000                    add      byte ptr [eax], al             
  0x00CA6725  d85600                  fcom     dword ptr [esi]                
  0x00CA6728  0300                    add      eax, dword ptr [eax]           
  0x00CA672A  2000                    and      byte ptr [eax], al             
  0x00CA672C  02a40500884601          add      ah, byte ptr [ebp + eax + 0x1468800] 
  0x00CA6733  0000                    add      byte ptr [eax], al             
  0x00CA6735  0000                    add      byte ptr [eax], al             
  0x00CA6737  0000                    add      byte ptr [eax], al             
  0x00CA6739  f4                      hlt                                     
  0x00CA673A  60                      pushal                                  
  0x00CA673B  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA6741  f4                      hlt                                     
  0x00CA6742  61                      popal                                   
  0x00CA6743  00910b000000            add      byte ptr [ecx + 0xb], dl       
  0x00CA6749  f4                      hlt                                     
  0x00CA674A  46                      inc      esi                            
  0x00CA674B  0007                    add      byte ptr [edi], al             
  0x00CA674D  0000                    add      byte ptr [eax], al             
  0x00CA674F  0000                    add      byte ptr [eax], al             
  0x00CA6752  44                      inc      esp                            
  0x00CA6753  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA6759  c406                    les      eax, ptr [esi]                 
  0x00CA675B  000a                    add      byte ptr [edx], cl             
  0x00CA675D  0000                    add      byte ptr [eax], al             
  0x00CA675F  0000                    add      byte ptr [eax], al             
  0x00CA6761  d85600                  fcom     dword ptr [esi]                
  0x00CA6764  03d9                    add      ebx, ecx                       
  0x00CA6766  44                      inc      esp                            
  0x00CA6767  0006                    add      byte ptr [esi], al             
  0x00CA6769  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA676A  0500884401              add      eax, 0x1448800                 
  0x00CA676F  00d0                    add      al, dl                         
  0x00CA6771  0020                    add      byte ptr [eax], ah             
  0x00CA6773  002e                    add      byte ptr [esi], ch             
  0x00CA6775  1d0c001800              sbb      eax, 0x18000c                  
  0x00CA677A  2000                    and      byte ptr [eax], al             
  0x00CA677C  884201                  mov      byte ptr [edx + 1], al         
  0x00CA677F  0000                    add      byte ptr [eax], al             
  0x00CA6781  0000                    add      byte ptr [eax], al             
  0x00CA6783  0000                    add      byte ptr [eax], al             
  0x00CA6786  56                      push     esi                            
  0x00CA6787  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA678A  0000                    add      byte ptr [eax], al             
  0x00CA678C  0300                    add      eax, dword ptr [eax]           
  0x00CA678E  2000                    and      byte ptr [eax], al             
  0x00CA6790  08a4050000f056          or       byte ptr [ebp + eax + 0x56f00000], ah 
  0x00CA6797  008f0b000003            add      byte ptr [edi + 0x300000b], cl 
  0x00CA679D  f4                      hlt                                     
  0x00CA679E  44                      inc      esp                            
  0x00CA679F  000e                    add      byte ptr [esi], cl             
  0x00CA67A1  0000                    add      byte ptr [eax], al             
  0x00CA67A3  0003                    add      byte ptr [ebx], al             
  0x00CA67A5  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA67A6  0500884401              add      eax, 0x1448800                 
  0x00CA67AB  004800                  add      byte ptr [eax], cl             
  0x00CA67AE  2000                    and      byte ptr [eax], al             
  0x00CA67B0  884101                  mov      byte ptr [ecx + 1], al         
  0x00CA67B3  0000                    add      byte ptr [eax], al             
  0x00CA67B6  56                      push     esi                            
  0x00CA67B7  00900b000003            add      byte ptr [eax + 0x300000b], dl 
  0x00CA67BD  0020                    add      byte ptr [eax], ah             
  0x00CA67BF  0006                    add      byte ptr [esi], al             
  0x00CA67C1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA67C2  0500884201              add      eax, 0x1428800                 
  0x00CA67C7  008842010088            add      byte ptr [eax - 0x77fffebe], cl 
  0x00CA67CD  42                      inc      edx                            
  0x00CA67CE  0100                    add      dword ptr [eax], eax           
  0x00CA67D0  884201                  mov      byte ptr [edx + 1], al         
  0x00CA67D3  008843010088            add      byte ptr [eax - 0x77fffebd], cl 
  0x00CA67D9  41                      inc      ecx                            
  0x00CA67DA  0100                    add      dword ptr [eax], eax           
  0x00CA67DC  00f0                    add      al, dh                         
  0x00CA67DE  56                      push     esi                            
  0x00CA67DF  006f0b                  add      byte ptr [edi + 0xb], ch       
  0x00CA67E2  0000                    add      byte ptr [eax], al             
  0x00CA67E4  0300                    add      eax, dword ptr [eax]           
  0x00CA67E6  2000                    and      byte ptr [eax], al             
  0x00CA67E8  0e                      push     cs                             
  0x00CA67E9  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA67EA  0500884601              add      eax, 0x1468800                 
  0x00CA67EF  0000                    add      byte ptr [eax], al             
  0x00CA67F2  44                      inc      esp                            
  0x00CA67F3  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA67F9  c406                    les      eax, ptr [esi]                 
  0x00CA67FB  0003                    add      byte ptr [ebx], al             
  0x00CA67FD  0000                    add      byte ptr [eax], al             
  0x00CA67FF  008844010088            add      byte ptr [eax - 0x77fffebc], cl 
  0x00CA6805  43                      inc      ebx                            
  0x00CA6806  0100                    add      dword ptr [eax], eax           
  0x00CA6808  00f0                    add      al, dh                         
  0x00CA680A  56                      push     esi                            
  0x00CA680B  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA680E  0000                    add      byte ptr [eax], al             
  0x00CA6810  0300                    add      eax, dword ptr [eax]           
  0x00CA6812  2000                    and      byte ptr [eax], al             
  0x00CA6814  03a40500884401          add      esp, dword ptr [ebp + eax + 0x1448800] 
  0x00CA681B  008843010088            add      byte ptr [eax - 0x77fffebd], cl 
  0x00CA6821  41                      inc      ecx                            
  0x00CA6822  0100                    add      dword ptr [eax], eax           
  0x00CA6824  884101                  mov      byte ptr [ecx + 1], al         
  0x00CA6827  001b                    add      byte ptr [ebx], bl             
  0x00CA6829  e721                    out      0x21, eax                      
  0x00CA682B  0000                    add      byte ptr [eax], al             
  0x00CA682E  56                      push     esi                            
  0x00CA682F  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA6832  0000                    add      byte ptr [eax], al             
  0x00CA6834  c54001                  lds      eax, ptr [eax + 1]             
  0x00CA6837  0003                    add      byte ptr [ebx], al             
  0x00CA6839  0000                    add      byte ptr [eax], al             
  0x00CA683B  004210                  add      byte ptr [edx + 0x10], al      
  0x00CA683E  0d000d0000              or       eax, 0xd00                     
  0x00CA6843  0079e7                  add      byte ptr [ecx - 0x19], bh      
  0x00CA6846  2100                    and      dword ptr [eax], eax           
  0x00CA6848  884901                  mov      byte ptr [ecx + 1], cl         
  0x00CA684B  0079e7                  add      byte ptr [ecx - 0x19], bh      
  0x00CA684E  2100                    and      dword ptr [eax], eax           
  0x00CA6850  884101                  mov      byte ptr [ecx + 1], al         
  0x00CA6853  008841010000            add      byte ptr [eax + 0x141], cl     
  0x00CA685A  56                      push     esi                            
  0x00CA685B  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA685E  0000                    add      byte ptr [eax], al             
  0x00CA6860  c54001                  lds      eax, ptr [eax + 1]             
  0x00CA6863  0003                    add      byte ptr [ebx], al             
  0x00CA6865  0000                    add      byte ptr [eax], al             
  0x00CA6867  0002                    add      byte ptr [edx], al             
  0x00CA6869  2405                    and      al, 5                          
  0x00CA686B  00886f010088            add      byte ptr [eax - 0x77fffe91], cl 
  0x00CA6871  47                      inc      edi                            
  0x00CA6872  0100                    add      dword ptr [eax], eax           
  0x00CA6875  1e                      push     ds                             
  0x00CA6876  0c00                    or       al, 0                          
  0x00CA6878  007055                  add      byte ptr [eax + 0x55], dh      
  0x00CA687B  00700b                  add      byte ptr [eax + 0xb], dh       
  0x00CA687E  0000                    add      byte ptr [eax], al             
  0x00CA6880  871e                    xchg     dword ptr [esi], ebx           
  0x00CA6882  0c00                    or       al, 0                          
  0x00CA6884  79e4                    jns      0xca686a                       
  0x00CA6886  2100                    and      dword ptr [eax], eax           
  0x00CA6888  48                      dec      eax                            
  0x00CA6889  0020                    add      byte ptr [eax], ah             
  0x00CA688B  0000                    add      byte ptr [eax], al             
  0x00CA688D  7055                    jo       0xca68e4                       
  0x00CA688F  00710b                  add      byte ptr [ecx + 0xb], dh       
  0x00CA6892  0000                    add      byte ptr [eax], al             
  0x00CA6894  00f0                    add      al, dh                         
  0x00CA6896  56                      push     esi                            
  0x00CA6897  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA689A  0000                    add      byte ptr [eax], al             
  0x00CA689C  0300                    add      eax, dword ptr [eax]           
  0x00CA689E  2000                    and      byte ptr [eax], al             
  0x00CA68A0  07                      pop      es                             
  0x00CA68A1  2405                    and      al, 5                          
  0x00CA68A3  001b                    add      byte ptr [ebx], bl             
  0x00CA68A5  0020                    add      byte ptr [eax], ah             
  0x00CA68A7  008841010088            add      byte ptr [eax - 0x77fffebf], cl 
  0x00CA68AD  41                      inc      ecx                            
  0x00CA68AE  0100                    add      dword ptr [eax], eax           
  0x00CA68B0  885001                  mov      byte ptr [eax + 1], dl         
  0x00CA68B3  0000                    add      byte ptr [eax], al             
  0x00CA68B5  7055                    jo       0xca690c                       
  0x00CA68B7  00530b                  add      byte ptr [ebx + 0xb], dl       
  0x00CA68BA  0000                    add      byte ptr [eax], al             
  0x00CA68BC  00f0                    add      al, dh                         
  0x00CA68BE  56                      push     esi                            
  0x00CA68BF  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA68C2  0000                    add      byte ptr [eax], al             
  0x00CA68C4  0300                    add      eax, dword ptr [eax]           
  0x00CA68C6  2000                    and      byte ptr [eax], al             
  0x00CA68C8  8c24050000f057          mov      word ptr [eax + 0x57f00000], fs 
  0x00CA68CF  00510b                  add      byte ptr [ecx + 0xb], dl       
  0x00CA68D2  0000                    add      byte ptr [eax], al             
  0x00CA68D4  00f0                    add      al, dh                         
  0x00CA68D6  44                      inc      esp                            
  0x00CA68D7  00530b                  add      byte ptr [ebx + 0xb], dl       
  0x00CA68DA  0000                    add      byte ptr [eax], al             
  0x00CA68DC  48                      dec      eax                            
  0x00CA68DD  0020                    add      byte ptr [eax], ah             
  0x00CA68DF  0000                    add      byte ptr [eax], al             
  0x00CA68E2  44                      inc      esp                            
  0x00CA68E3  009a0b000000            add      byte ptr [edx + 0xb], bl       
  0x00CA68E9  f4                      hlt                                     
  0x00CA68EA  46                      inc      esi                            
  0x00CA68EB  0008                    add      byte ptr [eax], cl             
  0x00CA68ED  0000                    add      byte ptr [eax], al             
  0x00CA68EF  00d0                    add      al, dl                         
  0x00CA68F1  0020                    add      byte ptr [eax], ah             
  0x00CA68F3  0000                    add      byte ptr [eax], al             
  0x00CA68F5  0e                      push     cs                             
  0x00CA68F6  2100                    and      dword ptr [eax], eax           
  0x00CA68F8  1400                    adc      al, 0                          
  0x00CA68FA  2000                    and      byte ptr [eax], al             
  0x00CA68FC  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA68FF  00520b                  add      byte ptr [edx + 0xb], dl       
  0x00CA6902  0000                    add      byte ptr [eax], al             
  0x00CA6904  00f0                    add      al, dh                         
  0x00CA6906  56                      push     esi                            
  0x00CA6907  00530b                  add      byte ptr [ebx + 0xb], dl       
  0x00CA690A  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA68B5 (cond_jump)
  0x00CA690C  00f0                    add      al, dh                         
  0x00CA690E  44                      inc      esp                            
  0x00CA690F  009f0b000045            add      byte ptr [edi + 0x4500000b], bl 
  0x00CA6915  0020                    add      byte ptr [eax], ah             
  0x00CA6917  00857405001b            add      byte ptr [ebp + 0x1b000574], al 
  0x00CA691D  f4                      hlt                                     
  0x00CA691E  44                      inc      esp                            
  0x00CA691F  005555                  add      byte ptr [ebp + 0x55], dl      
  0x00CA6922  150000f056              adc      eax, 0x56f00000                
  0x00CA6927  00520b                  add      byte ptr [edx + 0xb], dl       
  0x00CA692A  0000                    add      byte ptr [eax], al             
  0x00CA692C  c44001                  les      eax, ptr [eax + 1]             
  0x00CA692F  002f                    add      byte ptr [edi], ch             
  0x00CA6931  0000                    add      byte ptr [eax], al             
  0x00CA6933  0000                    add      byte ptr [eax], al             
  0x00CA6935  8521                    test     dword ptr [ecx], esp           
  0x00CA6937  00a800200000            add      byte ptr [eax + 0x2000], ch    
  0x00CA693D  af                      scasd    eax, dword ptr es:[edi]        
  0x00CA693E  2100                    and      dword ptr [eax], eax           
  0x00CA6940  00f0                    add      al, dh                         
  0x00CA6942  44                      inc      esp                            
  0x00CA6943  00710b                  add      byte ptr [ecx + 0xb], dh       
  0x00CA6946  0000                    add      byte ptr [eax], al             
  0x00CA6948  4d                      dec      ebp                            
  0x00CA6949  0020                    add      byte ptr [eax], ah             
  0x00CA694B  0058f4                  add      byte ptr [eax - 0xc], bl       
  0x00CA694E  050000a521              add      eax, 0x21a50000                
  0x00CA6953  0000                    add      byte ptr [eax], al             
  0x00CA6955  f4                      hlt                                     
  0x00CA6956  44                      inc      esp                            
  0x00CA6957  0006                    add      byte ptr [esi], al             
  0x00CA6959  0000                    add      byte ptr [eax], al             
  0x00CA695B  00a00020002e            add      byte ptr [eax + 0x2e002000], ah 
  0x00CA6961  1d0c0036f0              sbb      eax, 0xf036000c                
  0x00CA6966  44                      inc      esp                            
  0x00CA6967  00520b                  add      byte ptr [edx + 0xb], dl       
  0x00CA696A  0000                    add      byte ptr [eax], al             
  0x00CA696C  40                      inc      eax                            
  0x00CA696D  0020                    add      byte ptr [eax], ah             
  0x00CA696F  00c4                    add      ah, al                         
  0x00CA6971  40                      inc      eax                            
  0x00CA6972  0100                    add      dword ptr [eax], eax           
  0x00CA6974  2f                      das                                     
  0x00CA6975  0000                    add      byte ptr [eax], al             
  0x00CA6977  0000                    add      byte ptr [eax], al             
  0x00CA697A  2100                    and      dword ptr [eax], eax           
  0x00CA697C  c8400100                enter    0x140, 0                       
  0x00CA6980  0100                    add      dword ptr [eax], eax           
  0x00CA6982  0000                    add      byte ptr [eax], al             
  0x00CA6984  00f4                    add      ah, dh                         
  0x00CA6986  56                      push     esi                            
  0x00CA6987  00680b                  add      byte ptr [eax + 0xb], ch       
  0x00CA698A  0000                    add      byte ptr [eax], al             
  0x00CA698C  0000                    add      byte ptr [eax], al             
  0x00CA698E  2400                    and      al, 0                          
  0x00CA6990  40                      inc      eax                            
  0x00CA6991  0020                    add      byte ptr [eax], ah             
  0x00CA6993  0000                    add      byte ptr [eax], al             
  0x00CA6995  90                      nop                                     
  0x00CA6996  2100                    and      dword ptr [eax], eax           
  0x00CA6998  00f8                    add      al, bh                         
  0x00CA699A  2000                    and      byte ptr [eax], al             
  0x00CA699C  10d8                    adc      al, bl                         
  0x00CA699E  06                      push     es                             
  0x00CA699F  0002                    add      byte ptr [edx], al             
  0x00CA69A1  0000                    add      byte ptr [eax], al             
  0x00CA69A3  0000                    add      byte ptr [eax], al             
  0x00CA69A5  58                      pop      eax                            
  0x00CA69A6  57                      push     edi                            
  0x00CA69A7  00cc                    add      ah, cl                         
  0x00CA69A9  40                      inc      eax                            
  0x00CA69AA  0100                    add      dword ptr [eax], eax           
  0x00CA69AC  0100                    add      dword ptr [eax], eax           
  0x00CA69AE  0000                    add      byte ptr [eax], al             
  0x00CA69B0  00f4                    add      ah, dh                         
  0x00CA69B2  56                      push     esi                            
  0x00CA69B3  0006                    add      byte ptr [esi], al             
  0x00CA69B5  0000                    add      byte ptr [eax], al             
  0x00CA69B7  00740020                add      byte ptr [eax + eax + 0x20], dh 
  0x00CA69BB  0003                    add      byte ptr [ebx], al             
  0x00CA69BD  0020                    add      byte ptr [eax], ah             
  0x00CA69BF  0005f4050000            add      byte ptr [0x5f4], al           
  0x00CA69C5  d821                    fsub     dword ptr [ecx]                
  0x00CA69C7  0010                    add      byte ptr [eax], dl             
  0x00CA69C9  d806                    fadd     dword ptr [esi]                
  0x00CA69CB  0002                    add      byte ptr [edx], al             
  0x00CA69CD  0000                    add      byte ptr [eax], al             
  0x00CA69CF  0000                    add      byte ptr [eax], al             
  0x00CA69D1  58                      pop      eax                            
  0x00CA69D2  57                      push     edi                            
  0x00CA69D3  0000                    add      byte ptr [eax], al             
  0x00CA69D5  f4                      hlt                                     
  0x00CA69D6  56                      push     esi                            
  0x00CA69D7  00680b                  add      byte ptr [eax + 0xb], ch       
  0x00CA69DA  0000                    add      byte ptr [eax], al             
  0x00CA69DC  00f4                    add      ah, dh                         
  0x00CA69DE  44                      inc      esp                            
  0x00CA69DF  0003                    add      byte ptr [ebx], al             
  0x00CA69E1  0000                    add      byte ptr [eax], al             
  0x00CA69E3  004000                  add      byte ptr [eax], al             
  0x00CA69E6  2000                    and      byte ptr [eax], al             
  0x00CA69E8  0090210000e0            add      byte ptr [eax - 0x1fffffdf], dl 
  0x00CA69EE  56                      push     esi                            
  0x00CA69EF  00806f010000            add      byte ptr [eax + 0x16f], al     
  0x00CA69F5  60                      pushal                                  
  0x00CA69F6  56                      push     esi                            
  0x00CA69F7  0000                    add      byte ptr [eax], al             
  0x00CA69F9  f4                      hlt                                     
  0x00CA69FA  56                      push     esi                            
  0x00CA69FB  00680b                  add      byte ptr [eax + 0xb], ch       
  0x00CA69FE  0000                    add      byte ptr [eax], al             
  0x00CA6A00  0000                    add      byte ptr [eax], al             
  0x00CA6A02  2400                    and      al, 0                          
  0x00CA6A04  40                      inc      eax                            
  0x00CA6A05  0020                    add      byte ptr [eax], ah             
  0x00CA6A07  0000                    add      byte ptr [eax], al             
  0x00CA6A09  90                      nop                                     
  0x00CA6A0A  2100                    and      dword ptr [eax], eax           
  0x00CA6A0C  00f0                    add      al, dh                         
  0x00CA6A0E  7000                    jo       0xca6a10                       
                                        ; XREF: 0x00CA6A0E (cond_jump)
  0x00CA6A10  40                      inc      eax                            
  0x00CA6A11  0b00                    or       eax, dword ptr [eax]           
  0x00CA6A13  0000                    add      byte ptr [eax], al             
  0x00CA6A15  e8560000f0              call     0xf0ca6a70                     
  0x00CA6A1A  44                      inc      esp                            
  0x00CA6A1B  00710b                  add      byte ptr [ecx + 0xb], dh       
  0x00CA6A1E  0000                    add      byte ptr [eax], al             
  0x00CA6A20  44                      inc      esp                            
  0x00CA6A21  0020                    add      byte ptr [eax], ah             
  0x00CA6A23  0000                    add      byte ptr [eax], al             
  0x00CA6A25  6856000c00              push     0xc0056                        
  0x00CA6A2A  0000                    add      byte ptr [eax], al             
  0x00CA6A2C  00f4                    add      ah, dh                         
  0x00CA6A2E  44                      inc      esp                            
  0x00CA6A2F  0001                    add      byte ptr [ecx], al             
  0x00CA6A31  0000                    add      byte ptr [eax], al             
  0x00CA6A33  0000                    add      byte ptr [eax], al             
  0x00CA6A35  7044                    jo       0xca6a7b                       
  0x00CA6A37  00960b00000c            add      byte ptr [esi + 0xc00000b], dl 
  0x00CA6A3D  0000                    add      byte ptr [eax], al             
  0x00CA6A3F  0080100d0023            add      byte ptr [eax + 0x23000d10], al 
  0x00CA6A45  0000                    add      byte ptr [eax], al             
  0x00CA6A47  0003                    add      byte ptr [ebx], al             
  0x00CA6A49  0020                    add      byte ptr [eax], ah             
  0x00CA6A4B  001b                    add      byte ptr [ebx], bl             
  0x00CA6A4D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA6A4E  050080100d              add      eax, 0xd108000                 
  0x00CA6A53  00a2fdff0000            add      byte ptr [edx + 0xfffd], ah    
  0x00CA6A59  f4                      hlt                                     
  0x00CA6A5A  56                      push     esi                            
  0x00CA6A5B  000500000000            add      byte ptr [0], al               
  0x00CA6A62  44                      inc      esp                            
  0x00CA6A63  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA6A66  0000                    add      byte ptr [eax], al             
  0x00CA6A68  45                      inc      ebp                            
  0x00CA6A69  0020                    add      byte ptr [eax], ah             
  0x00CA6A6B  0017                    add      byte ptr [edi], dl             
  0x00CA6A6D  f4                      hlt                                     
  0x00CA6A6E  050000f456              add      eax, 0x56f40000                
  0x00CA6A73  00680b                  add      byte ptr [eax + 0xb], ch       
  0x00CA6A76  0000                    add      byte ptr [eax], al             
  0x00CA6A78  00f0                    add      al, dh                         
  0x00CA6A7A  44                      inc      esp                            
                                        ; XREF: 0x00CA6A35 (cond_jump)
  0x00CA6A7B  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA6A7E  0000                    add      byte ptr [eax], al             
  0x00CA6A80  40                      inc      eax                            
  0x00CA6A81  0020                    add      byte ptr [eax], ah             
  0x00CA6A83  00c0                    add      al, al                         
  0x00CA6A85  40                      inc      eax                            
  0x00CA6A86  0100                    add      dword ptr [eax], eax           
  0x00CA6A88  0100                    add      dword ptr [eax], eax           
  0x00CA6A8A  0000                    add      byte ptr [eax], al             
  0x00CA6A8C  00d0                    add      al, dl                         
  0x00CA6A8E  2100                    and      dword ptr [eax], eax           
  0x00CA6A90  00d0                    add      al, dl                         
  0x00CA6A92  56                      push     esi                            
  0x00CA6A93  0000                    add      byte ptr [eax], al             
  0x00CA6A95  d8440040                fadd     dword ptr [eax + eax + 0x40]   
  0x00CA6A99  0020                    add      byte ptr [eax], ah             
  0x00CA6A9B  0000                    add      byte ptr [eax], al             
  0x00CA6A9E  44                      inc      esp                            
  0x00CA6A9F  007a0b                  add      byte ptr [edx + 0xb], bh       
  0x00CA6AA2  0000                    add      byte ptr [eax], al             
  0x00CA6AA4  44                      inc      esp                            
  0x00CA6AA6  45                      inc      ebp                            
  0x00CA6AA7  00a40400006400          add      byte ptr [esp + eax + 0x640000], ah 
  0x00CA6AAE  2000                    and      byte ptr [eax], al             
  0x00CA6AB0  006056                  add      byte ptr [eax + 0x56], ah      
  0x00CA6AB3  00050c050080            add      byte ptr [0x8000050c], al      
  0x00CA6AB9  100d00580100            adc      byte ptr [0x15800], cl         
  0x00CA6ABF  0080100d0086            add      byte ptr [eax - 0x79fff2f0], al 
  0x00CA6AC5  fd                      std                                     
  0x00CA6AC6  ff00                    inc      dword ptr [eax]                
  0x00CA6AC8  0c00                    or       al, 0                          
  0x00CA6ACA  0000                    add      byte ptr [eax], al             
  0x00CA6ACC  00f4                    add      ah, dh                         
  0x00CA6ACE  45                      inc      ebp                            
  0x00CA6ACF  0090ffff0000            add      byte ptr [eax + 0xffff], dl    
  0x00CA6AD5  7045                    jo       0xca6b1c                       
  0x00CA6AD7  009f04000080            add      byte ptr [edi - 0x7ffffffc], bl 
  0x00CA6ADD  100d007c0000            adc      byte ptr [0x7c00], cl          
  0x00CA6AE3  0000                    add      byte ptr [eax], al             
  0x00CA6AE5  f4                      hlt                                     
  0x00CA6AE6  44                      inc      esp                            
  0x00CA6AE7  0008                    add      byte ptr [eax], cl             
  0x00CA6AE9  0000                    add      byte ptr [eax], al             
  0x00CA6AEB  0000                    add      byte ptr [eax], al             
  0x00CA6AED  7044                    jo       0xca6b33                       
  0x00CA6AEF  009e04000000            add      byte ptr [esi + 4], bl         
  0x00CA6AF5  002400                  add      byte ptr [eax + eax], ah       
  0x00CA6AF8  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA6AFB  00b504000000            add      byte ptr [ebp + 4], dh         
  0x00CA6B02  50                      push     eax                            
  0x00CA6B03  009e0400000a            add      byte ptr [esi + 0xa000004], bl 
  0x00CA6B09  0000                    add      byte ptr [eax], al             
  0x00CA6B0B  0000                    add      byte ptr [eax], al             
  0x00CA6B0D  7050                    jo       0xca6b5f                       
  0x00CA6B0F  009e04000080            add      byte ptr [esi - 0x7ffffffc], bl 
  0x00CA6B15  100d00870000            adc      byte ptr [0x8700], cl          
  0x00CA6B1B  000b                    add      byte ptr [ebx], cl             
  0x00CA6B1D  0020                    add      byte ptr [eax], ah             
  0x00CA6B1F  0009                    add      byte ptr [ecx], cl             
  0x00CA6B21  94                      xchg     esp, eax                       
  0x00CA6B22  050000f444              add      eax, 0x44f40000                
  0x00CA6B27  0001                    add      byte ptr [ecx], al             
  0x00CA6B29  0000                    add      byte ptr [eax], al             
  0x00CA6B2B  0000                    add      byte ptr [eax], al             
  0x00CA6B2D  7044                    jo       0xca6b73                       
  0x00CA6B2F  00b504000000            add      byte ptr [ebp + 4], dh         
  0x00CA6B36  44                      inc      esp                            
  0x00CA6B37  009f04000000            add      byte ptr [edi + 4], bl         
  0x00CA6B3D  7044                    jo       0xca6b83                       
  0x00CA6B3F  00b604000000            add      byte ptr [esi + 4], dh         
  0x00CA6B46  56                      push     esi                            
  0x00CA6B47  00b504000003            add      byte ptr [ebp + 0x3000004], dh 
  0x00CA6B4D  0020                    add      byte ptr [eax], ah             
  0x00CA6B4F  000f                    add      byte ptr [edi], cl             
  0x00CA6B51  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA6B52  050000f456              add      eax, 0x56f40000                
  0x00CA6B57  0010                    add      byte ptr [eax], dl             
  0x00CA6B59  0000                    add      byte ptr [eax], al             
  0x00CA6B5B  0000                    add      byte ptr [eax], al             
  0x00CA6B5D  c421                    les      esp, ptr [ecx]                 
                                        ; XREF: 0x00CA6B0D (cond_jump)
  0x00CA6B5F  0000                    add      byte ptr [eax], al             
  0x00CA6B61  7056                    jo       0xca6bb9                       
  0x00CA6B63  00a004000000            add      byte ptr [eax + 4], ah         
  0x00CA6B6A  56                      push     esi                            
  0x00CA6B6B  009f04000000            add      byte ptr [edi + 4], bl         
  0x00CA6B71  7056                    jo       0xca6bc9                       
                                        ; XREF: 0x00CA6B2D (cond_jump)
  0x00CA6B73  00a104000040            add      byte ptr [ecx + 0x40000004], ah 
  0x00CA6B79  0020                    add      byte ptr [eax], ah             
  0x00CA6B7B  0022                    add      byte ptr [edx], ah             
  0x00CA6B7D  0020                    add      byte ptr [eax], ah             
  0x00CA6B7F  0000                    add      byte ptr [eax], al             
  0x00CA6B81  7056                    jo       0xca6bd9                       
                                        ; XREF: 0x00CA6B3D (cond_jump)
  0x00CA6B83  009f0400000e            add      byte ptr [edi + 0xe000004], bl 
  0x00CA6B89  0c05                    or       al, 5                          
  0x00CA6B8B  0000                    add      byte ptr [eax], al             
  0x00CA6B8D  f4                      hlt                                     
  0x00CA6B8E  56                      push     esi                            
  0x00CA6B8F  0010                    add      byte ptr [eax], dl             
  0x00CA6B92  ff00                    inc      dword ptr [eax]                
  0x00CA6B94  00c4                    add      ah, al                         
  0x00CA6B96  2100                    and      dword ptr [eax], eax           
  0x00CA6B98  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA6B9B  00a104000000            add      byte ptr [ecx + 4], ah         
  0x00CA6BA2  56                      push     esi                            
  0x00CA6BA3  009f04000000            add      byte ptr [edi + 4], bl         
  0x00CA6BA9  7056                    jo       0xca6c01                       
  0x00CA6BAB  00a004000040            add      byte ptr [eax + 0x40000004], ah 
  0x00CA6BB1  0020                    add      byte ptr [eax], ah             
  0x00CA6BB3  0022                    add      byte ptr [edx], ah             
  0x00CA6BB5  0020                    add      byte ptr [eax], ah             
  0x00CA6BB7  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA6B61 (cond_jump)
  0x00CA6BB9  7056                    jo       0xca6c11                       
  0x00CA6BBB  009f04000080            add      byte ptr [edi - 0x7ffffffc], bl 
  0x00CA6BC1  100d00430000            adc      byte ptr [0x4300], cl          
  0x00CA6BC7  0080100d005a            add      byte ptr [eax + 0x5a000d10], al 
  0x00CA6BCD  0000                    add      byte ptr [eax], al             
  0x00CA6BCF  000b                    add      byte ptr [ebx], cl             
  0x00CA6BD1  0020                    add      byte ptr [eax], ah             
  0x00CA6BD3  000e                    add      byte ptr [esi], cl             
  0x00CA6BD5  94                      xchg     esp, eax                       
  0x00CA6BD6  050000f044              add      eax, 0x44f00000                
  0x00CA6BDB  009f04000000            add      byte ptr [edi + 4], bl         
  0x00CA6BE1  7044                    jo       0xca6c27                       
  0x00CA6BE3  00a104000000            add      byte ptr [ecx + 4], ah         
  0x00CA6BE9  f4                      hlt                                     
  0x00CA6BEA  44                      inc      esp                            
  0x00CA6BEB  0001                    add      byte ptr [ecx], al             
  0x00CA6BED  0000                    add      byte ptr [eax], al             
  0x00CA6BEF  0000                    add      byte ptr [eax], al             
  0x00CA6BF1  7044                    jo       0xca6c37                       
  0x00CA6BF3  00b504000000            add      byte ptr [ebp + 4], dh         
  0x00CA6BFA  44                      inc      esp                            
  0x00CA6BFB  009f04000000            add      byte ptr [edi + 4], bl         
                                        ; XREF: 0x00CA6BA9 (cond_jump)
  0x00CA6C01  7044                    jo       0xca6c47                       
  0x00CA6C03  00b604000005            add      byte ptr [esi + 0x5000004], dh 
  0x00CA6C09  0c05                    or       al, 5                          
  0x00CA6C0B  0000                    add      byte ptr [eax], al             
  0x00CA6C0E  44                      inc      esp                            
  0x00CA6C0F  009f04000000            add      byte ptr [edi + 4], bl         
  0x00CA6C15  7044                    jo       0xca6c5b                       
  0x00CA6C17  00a004000000            add      byte ptr [eax + 4], ah         
  0x00CA6C1E  56                      push     esi                            
  0x00CA6C1F  00a004000000            add      byte ptr [eax + 4], ah         
  0x00CA6C26  44                      inc      esp                            
                                        ; XREF: 0x00CA6BE1 (cond_jump)
  0x00CA6C27  00a104000044            add      byte ptr [ecx + 0x44000004], ah 
  0x00CA6C2E  2100                    and      dword ptr [eax], eax           
  0x00CA6C30  c54001                  lds      eax, ptr [eax + 1]             
  0x00CA6C33  0001                    add      byte ptr [ecx], al             
  0x00CA6C35  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA6BF1 (cond_jump)
  0x00CA6C37  0008                    add      byte ptr [eax], cl             
  0x00CA6C39  2405                    and      al, 5                          
  0x00CA6C3B  0080100d0028            add      byte ptr [eax + 0x28000d10], al 
  0x00CA6C41  0100                    add      dword ptr [eax], eax           
  0x00CA6C43  0000                    add      byte ptr [eax], al             
  0x00CA6C45  f4                      hlt                                     
  0x00CA6C46  44                      inc      esp                            
                                        ; XREF: 0x00CA6C01 (cond_jump)
  0x00CA6C47  0001                    add      byte ptr [ecx], al             
  0x00CA6C49  0000                    add      byte ptr [eax], al             
  0x00CA6C4B  0000                    add      byte ptr [eax], al             
  0x00CA6C4D  7044                    jo       0xca6c93                       
  0x00CA6C4F  00b50400001b            add      byte ptr [ebp + 0x1b000004], dh 
  0x00CA6C55  0c05                    or       al, 5                          
  0x00CA6C57  005100                  add      byte ptr [ecx], dl             
  0x00CA6C5A  2000                    and      byte ptr [eax], al             
  0x00CA6C5C  40                      inc      eax                            
  0x00CA6C5D  0020                    add      byte ptr [eax], ah             
  0x00CA6C5F  0022                    add      byte ptr [edx], ah             
  0x00CA6C61  0020                    add      byte ptr [eax], ah             
  0x00CA6C63  004500                  add      byte ptr [ebp], al             
  0x00CA6C66  2000                    and      byte ptr [eax], al             
  0x00CA6C68  0474                    add      al, 0x74                       
  0x00CA6C6A  0500008e20              add      eax, 0x208e0000                
  0x00CA6C6F  008041010005            add      byte ptr [eax + 0x5000141], al 
  0x00CA6C75  0c05                    or       al, 5                          
  0x00CA6C77  005500                  add      byte ptr [ebp], dl             
  0x00CA6C7A  2000                    and      byte ptr [eax], al             
  0x00CA6C7C  0394050000ce20          add      edx, dword ptr [ebp + eax + 0x20ce0000] 
  0x00CA6C83  00844101000070          add      byte ptr [ecx + eax*2 + 0x70000001], al 
  0x00CA6C8A  54                      push     esp                            
  0x00CA6C8B  009f04000000            add      byte ptr [edi + 4], bl         
  0x00CA6C92  56                      push     esi                            
                                        ; XREF: 0x00CA6C4D (cond_jump)
  0x00CA6C93  009e04000084            add      byte ptr [esi - 0x7bfffffc], bl 
  0x00CA6C99  41                      inc      ecx                            
  0x00CA6C9A  0100                    add      dword ptr [eax], eax           
  0x00CA6C9C  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA6C9F  009e04000087            add      byte ptr [esi - 0x78fffffc], bl 
  0x00CA6CA5  7705                    ja       0xca6cac                       
  0x00CA6CA7  0000                    add      byte ptr [eax], al             
  0x00CA6CAA  56                      push     esi                            
  0x00CA6CAB  00b504000085            add      byte ptr [ebp - 0x7afffffc], dh 
  0x00CA6CB1  41                      inc      ecx                            
  0x00CA6CB2  0100                    add      dword ptr [eax], eax           
  0x00CA6CB4  0324050080100d          add      esp, dword ptr [eax + 0xd108000] 
  0x00CA6CBB  0009                    add      byte ptr [ecx], cl             
  0x00CA6CBD  0100                    add      dword ptr [eax], eax           
  0x00CA6CBF  0000                    add      byte ptr [eax], al             
  0x00CA6CC2  56                      push     esi                            
  0x00CA6CC3  00b50400000c            add      byte ptr [ebp + 0xc000004], dh 
  0x00CA6CC9  0000                    add      byte ptr [eax], al             
  0x00CA6CCB  0000                    add      byte ptr [eax], al             
  0x00CA6CCE  56                      push     esi                            
  0x00CA6CCF  009f040000c0            add      byte ptr [edi - 0x3ffffffc], bl 
  0x00CA6CD5  40                      inc      eax                            
  0x00CA6CD6  0100                    add      dword ptr [eax], eax           
  0x00CA6CD8  f00000                  lock add byte ptr [eax], al             
  0x00CA6CDB  0008                    add      byte ptr [eax], cl             
  0x00CA6CDD  1c0c                    sbb      al, 0xc                        
  0x00CA6CDF  0000                    add      byte ptr [eax], al             
  0x00CA6CE1  8421                    test     byte ptr [ecx], ah             
  0x00CA6CE3  0000                    add      byte ptr [eax], al             
  0x00CA6CE5  002c00                  add      byte ptr [eax + eax], ch       
  0x00CA6CE8  081d0c000086            or       byte ptr [0x8600000c], bl      
  0x00CA6CEE  2100                    and      dword ptr [eax], eax           
  0x00CA6CF0  00f4                    add      ah, dh                         
  0x00CA6CF2  60                      pushal                                  
  0x00CA6CF3  00730b                  add      byte ptr [ebx + 0xb], dh       
  0x00CA6CF6  0000                    add      byte ptr [eax], al             
  0x00CA6CF8  00f4                    add      ah, dh                         
  0x00CA6CFA  6200                    bound    eax, qword ptr [eax]           
  0x00CA6CFC  790b                    jns      0xca6d09                       
  0x00CA6CFE  0000                    add      byte ptr [eax], al             
  0x00CA6D00  00f4                    add      ah, dh                         
  0x00CA6D02  6400740b00              add      byte ptr fs:[ebx + ecx], dh    
  0x00CA6D07  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA6CFC (cond_jump)
  0x00CA6D09  053c0000f0              add      eax, 0xf000003c                
  0x00CA6D0E  7000                    jo       0xca6d10                       
                                        ; XREF: 0x00CA6D0E (cond_jump)
  0x00CA6D10  97                      xchg     edi, eax                       
  0x00CA6D11  0b00                    or       eax, dword ptr [eax]           
  0x00CA6D13  0000                    add      byte ptr [eax], al             
  0x00CA6D15  95                      xchg     ebp, eax                       
  0x00CA6D16  2200                    and      al, byte ptr [eax]             
  0x00CA6D18  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA6D1B  0000                    add      byte ptr [eax], al             
  0x00CA6D1D  5a                      pop      edx                            
  0x00CA6D1E  46                      inc      esi                            
  0x00CA6D1F  0010                    add      byte ptr [eax], dl             
  0x00CA6D21  d806                    fadd     dword ptr [esi]                
  0x00CA6D23  0002                    add      byte ptr [edx], al             
  0x00CA6D25  0000                    add      byte ptr [eax], al             
  0x00CA6D27  0000                    add      byte ptr [eax], al             
  0x00CA6D29  5d                      pop      ebp                            
  0x00CA6D2A  46                      inc      esi                            
  0x00CA6D2B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA6D2E  0000                    add      byte ptr [eax], al             
  0x00CA6D30  00f0                    add      al, dh                         
  0x00CA6D32  7000                    jo       0xca6d34                       
                                        ; XREF: 0x00CA6D32 (cond_jump)
  0x00CA6D34  40                      inc      eax                            
  0x00CA6D35  0b00                    or       eax, dword ptr [eax]           
  0x00CA6D37  0000                    add      byte ptr [eax], al             
  0x00CA6D39  f4                      hlt                                     
  0x00CA6D3A  60                      pushal                                  
  0x00CA6D3B  00680b                  add      byte ptr [eax + 0xb], ch       
  0x00CA6D3E  0000                    add      byte ptr [eax], al             
  0x00CA6D40  00e8                    add      al, ch                         
  0x00CA6D42  57                      push     edi                            
  0x00CA6D43  0000                    add      byte ptr [eax], al             
  0x00CA6D45  fa                      cli                                     
  0x00CA6D46  2100                    and      dword ptr [eax], eax           
  0x00CA6D48  00f0                    add      al, dh                         
  0x00CA6D4A  56                      push     esi                            
  0x00CA6D4B  00c0                    add      al, al                         
  0x00CA6D4D  0400                    add      al, 0                          
  0x00CA6D4F  0003                    add      byte ptr [ebx], al             
  0x00CA6D51  0020                    add      byte ptr [eax], ah             
  0x00CA6D53  0045a5                  add      byte ptr [ebp - 0x5b], al      
  0x00CA6D56  050000f456              add      eax, 0x56f40000                
  0x00CA6D5B  0001                    add      byte ptr [ecx], al             
  0x00CA6D5D  0000                    add      byte ptr [eax], al             
  0x00CA6D5F  0000                    add      byte ptr [eax], al             
  0x00CA6D61  7056                    jo       0xca6db9                       
  0x00CA6D63  00b704000000            add      byte ptr [edi + 4], dh         
  0x00CA6D69  f4                      hlt                                     
  0x00CA6D6A  56                      push     esi                            
  0x00CA6D6B  0000                    add      byte ptr [eax], al             
  0x00CA6D6D  0000                    add      byte ptr [eax], al             
  0x00CA6D6F  0000                    add      byte ptr [eax], al             
  0x00CA6D72  44                      inc      esp                            
  0x00CA6D73  00730b                  add      byte ptr [ebx + 0xb], dh       
  0x00CA6D76  0000                    add      byte ptr [eax], al             
  0x00CA6D78  45                      inc      ebp                            
  0x00CA6D79  0020                    add      byte ptr [eax], ah             
  0x00CA6D7B  0004a4                  add      byte ptr [esp], al             
  0x00CA6D7E  0500000024              add      eax, 0x24000000                
  0x00CA6D83  0000                    add      byte ptr [eax], al             
  0x00CA6D85  7044                    jo       0xca6dcb                       
  0x00CA6D87  00b704000000            add      byte ptr [edi + 4], dh         
  0x00CA6D8D  f4                      hlt                                     
  0x00CA6D8E  46                      inc      esi                            
  0x00CA6D8F  0000                    add      byte ptr [eax], al             
  0x00CA6D91  0000                    add      byte ptr [eax], al             
  0x00CA6D93  0000                    add      byte ptr [eax], al             
  0x00CA6D95  f4                      hlt                                     
  0x00CA6D96  60                      pushal                                  
  0x00CA6D97  00740b00                add      byte ptr [ebx + ecx], dh       
  0x00CA6D9B  0000                    add      byte ptr [eax], al             
  0x00CA6D9E  44                      inc      esp                            
  0x00CA6D9F  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA6DA5  c406                    les      eax, ptr [esi]                 
  0x00CA6DA7  0008                    add      byte ptr [eax], cl             
  0x00CA6DA9  0000                    add      byte ptr [eax], al             
  0x00CA6DAB  0000                    add      byte ptr [eax], al             
  0x00CA6DAD  d85600                  fcom     dword ptr [esi]                
  0x00CA6DB0  55                      push     ebp                            
  0x00CA6DB1  0020                    add      byte ptr [eax], ah             
  0x00CA6DB3  0004a4                  add      byte ptr [esp], al             
  0x00CA6DB6  0500130020              add      eax, 0x20001300                
  0x00CA6DBB  0000                    add      byte ptr [eax], al             
  0x00CA6DBD  7056                    jo       0xca6e15                       
  0x00CA6DBF  00b704000000            add      byte ptr [edi + 4], dh         
  0x00CA6DC5  0000                    add      byte ptr [eax], al             
  0x00CA6DC7  0000                    add      byte ptr [eax], al             
  0x00CA6DCA  56                      push     esi                            
                                        ; XREF: 0x00CA6D85 (cond_jump)
  0x00CA6DCB  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA6DCE  0000                    add      byte ptr [eax], al             
  0x00CA6DD0  0300                    add      eax, dword ptr [eax]           
  0x00CA6DD2  2000                    and      byte ptr [eax], al             
  0x00CA6DD4  08a4050000f056          or       byte ptr [ebp + eax + 0x56f00000], ah 
  0x00CA6DDB  00790b                  add      byte ptr [ecx + 0xb], bh       
  0x00CA6DDE  0000                    add      byte ptr [eax], al             
  0x00CA6DE0  55                      push     ebp                            
  0x00CA6DE1  0020                    add      byte ptr [eax], ah             
  0x00CA6DE3  0004a4                  add      byte ptr [esp], al             
  0x00CA6DE6  0500130020              add      eax, 0x20001300                
  0x00CA6DEB  0000                    add      byte ptr [eax], al             
  0x00CA6DED  7056                    jo       0xca6e45                       
  0x00CA6DEF  00b704000000            add      byte ptr [edi + 4], dh         
  0x00CA6DF5  07                      pop      es                             
  0x00CA6DF6  3000                    xor      byte ptr [eax], al             
  0x00CA6DF8  c4700b                  les      esi, ptr [eax + 0xb]           
  0x00CA6DFB  00b20c000000            add      byte ptr [edx + 0xc], dh       
  0x00CA6E01  7044                    jo       0xca6e47                       
  0x00CA6E03  00bf04000013            add      byte ptr [edi + 0x13000004], bh 
  0x00CA6E09  f4                      hlt                                     
  0x00CA6E0A  60                      pushal                                  
  0x00CA6E0B  00a504000090            add      byte ptr [ebp - 0x6ffffffc], ah 
  0x00CA6E11  1006                    adc      byte ptr [esi], al             
  0x00CA6E13  0002                    add      byte ptr [edx], al             
                                        ; XREF: 0x00CA6DBD (cond_jump)
  0x00CA6E15  0000                    add      byte ptr [eax], al             
  0x00CA6E17  0000                    add      byte ptr [eax], al             
  0x00CA6E19  58                      pop      eax                            
  0x00CA6E1A  56                      push     esi                            
  0x00CA6E1B  0000                    add      byte ptr [eax], al             
  0x00CA6E1D  0036                    add      byte ptr [esi], dh             
  0x00CA6E1F  0000                    add      byte ptr [eax], al             
  0x00CA6E22  44                      inc      esp                            
  0x00CA6E23  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA6E29  c406                    les      eax, ptr [esi]                 
  0x00CA6E2B  0036                    add      byte ptr [esi], dh             
  0x00CA6E2D  0000                    add      byte ptr [eax], al             
  0x00CA6E2F  0000                    add      byte ptr [eax], al             
  0x00CA6E31  f4                      hlt                                     
  0x00CA6E32  56                      push     esi                            
  0x00CA6E33  00740b00                add      byte ptr [ebx + ecx], dh       
  0x00CA6E37  0000                    add      byte ptr [eax], al             
  0x00CA6E39  c422                    les      esp, ptr [edx]                 
  0x00CA6E3B  004000                  add      byte ptr [eax], al             
  0x00CA6E3E  2000                    and      byte ptr [eax], al             
  0x00CA6E40  0090210000f0            add      byte ptr [eax - 0xfffffdf], dl 
  0x00CA6E46  56                      push     esi                            
                                        ; XREF: 0x00CA6E01 (cond_jump)
  0x00CA6E47  00730b                  add      byte ptr [ebx + 0xb], dh       
  0x00CA6E4A  0000                    add      byte ptr [eax], al             
  0x00CA6E4C  00e0                    add      al, ah                         
  0x00CA6E4E  44                      inc      esp                            
  0x00CA6E4F  00844f0100081d          add      byte ptr [edi + ecx*2 + 0x1d080001], al 
  0x00CA6E56  0c00                    or       al, 0                          
  0x00CA6E58  40                      inc      eax                            
  0x00CA6E59  0020                    add      byte ptr [eax], ah             
  0x00CA6E5B  00041d0c000070          add      byte ptr [ebx + 0x7000000c], al 
  0x00CA6E62  54                      push     esp                            
  0x00CA6E63  00a204000000            add      byte ptr [edx + 4], ah         
  0x00CA6E69  f4                      hlt                                     
  0x00CA6E6A  56                      push     esi                            
  0x00CA6E6B  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA6E71  c422                    les      esp, ptr [edx]                 
  0x00CA6E73  004000                  add      byte ptr [eax], al             
  0x00CA6E76  2000                    and      byte ptr [eax], al             
  0x00CA6E78  009021000000            add      byte ptr [eax + 0x21], dl      
  0x00CA6E7E  250000e047              and      eax, 0x47e00000                
  0x00CA6E83  0000                    add      byte ptr [eax], al             
  0x00CA6E85  c422                    les      esp, ptr [edx]                 
  0x00CA6E87  0000                    add      byte ptr [eax], al             
  0x00CA6E89  f4                      hlt                                     
  0x00CA6E8A  46                      inc      esi                            
  0x00CA6E8B  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA6E91  f4                      hlt                                     
  0x00CA6E92  44                      inc      esp                            
  0x00CA6E93  00fa                    add      dl, bh                         
  0x00CA6E95  0000                    add      byte ptr [eax], al             
  0x00CA6E97  002e                    add      byte ptr [esi], ch             
  0x00CA6E99  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA6E9E  2000                    and      byte ptr [eax], al             
  0x00CA6EA0  0090210000c4            add      byte ptr [eax - 0x3bffffdf], dl 
  0x00CA6EA6  2200                    and      al, byte ptr [eax]             
  0x00CA6EA8  00f4                    add      ah, dh                         
  0x00CA6EAA  46                      inc      esi                            
  0x00CA6EAB  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA6EB2  44                      inc      esp                            
  0x00CA6EB3  00720b                  add      byte ptr [edx + 0xb], dh       
  0x00CA6EB6  0000                    add      byte ptr [eax], al             
  0x00CA6EB8  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA6EBE  2000                    and      byte ptr [eax], al             
  0x00CA6EC0  0091210000c4            add      byte ptr [ecx - 0x3bffffdf], dl 
  0x00CA6EC6  2200                    and      al, byte ptr [eax]             
  0x00CA6EC8  00f4                    add      ah, dh                         
  0x00CA6ECA  46                      inc      esi                            
  0x00CA6ECB  0032                    add      byte ptr [edx], dh             
  0x00CA6ECD  0000                    add      byte ptr [eax], al             
  0x00CA6ECF  00d0                    add      al, dl                         
  0x00CA6ED1  f4                      hlt                                     
  0x00CA6ED2  44                      inc      esp                            
  0x00CA6ED3  0000                    add      byte ptr [eax], al             
  0x00CA6ED5  0000                    add      byte ptr [eax], al             
  0x00CA6ED7  002e                    add      byte ptr [esi], ch             
  0x00CA6ED9  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA6EDE  2000                    and      byte ptr [eax], al             
  0x00CA6EE0  0092210000f4            add      byte ptr [edx - 0xbffffdf], dl 
  0x00CA6EE6  65001a                  add      byte ptr gs:[edx], bl          
  0x00CA6EE9  0f0000                  sldt     word ptr [eax]                 
  0x00CA6EEC  007066                  add      byte ptr [eax + 0x66], dh      
  0x00CA6EEF  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA6EF2  0000                    add      byte ptr [eax], al             
  0x00CA6EF4  86040d0000f066          xchg     byte ptr [ecx + 0x66f00000], al 
  0x00CA6EFB  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA6EFE  0000                    add      byte ptr [eax], al             
  0x00CA6F00  005e20                  add      byte ptr [esi + 0x20], bl      
  0x00CA6F03  0000                    add      byte ptr [eax], al             
  0x00CA6F06  56                      push     esi                            
  0x00CA6F07  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA6F0A  0000                    add      byte ptr [eax], al             
  0x00CA6F0C  0300                    add      eax, dword ptr [eax]           
  0x00CA6F0E  2000                    and      byte ptr [eax], al             
  0x00CA6F10  17                      pop      ss                             
  0x00CA6F11  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA6F12  050000f056              add      eax, 0x56f00000                
  0x00CA6F17  00730b                  add      byte ptr [ebx + 0xb], dh       
  0x00CA6F1A  0000                    add      byte ptr [eax], al             
  0x00CA6F1C  00f0                    add      al, dh                         
  0x00CA6F1E  44                      inc      esp                            
  0x00CA6F1F  00790b                  add      byte ptr [ecx + 0xb], bh       
  0x00CA6F22  0000                    add      byte ptr [eax], al             
  0x00CA6F24  844f01                  test     byte ptr [edi + 1], cl         
  0x00CA6F27  0008                    add      byte ptr [eax], cl             
  0x00CA6F29  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA6F2E  2000                    and      byte ptr [eax], al             
  0x00CA6F30  041d                    add      al, 0x1d                       
  0x00CA6F32  0c00                    or       al, 0                          
  0x00CA6F34  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA6F37  00a204000000            add      byte ptr [edx + 4], ah         
  0x00CA6F3D  00250000f447            add      byte ptr [0x47f40000], ah      
  0x00CA6F43  0007                    add      byte ptr [edi], al             
  0x00CA6F45  0000                    add      byte ptr [eax], al             
  0x00CA6F47  0000                    add      byte ptr [eax], al             
  0x00CA6F49  f4                      hlt                                     
  0x00CA6F4A  60                      pushal                                  
  0x00CA6F4B  008304000000            add      byte ptr [ebx + 4], al         
  0x00CA6F51  f4                      hlt                                     
  0x00CA6F52  61                      popal                                   
  0x00CA6F53  0039                    add      byte ptr [ecx], bh             
  0x00CA6F55  0b00                    or       eax, dword ptr [eax]           
  0x00CA6F57  0000                    add      byte ptr [eax], al             
  0x00CA6F59  f4                      hlt                                     
  0x00CA6F5A  6200                    bound    eax, qword ptr [eax]           
  0x00CA6F5C  b804000000              mov      eax, 4                         
  0x00CA6F61  f4                      hlt                                     
  0x00CA6F62  65001a                  add      byte ptr gs:[edx], bl          
  0x00CA6F65  0f0000                  sldt     word ptr [eax]                 
  0x00CA6F68  86040d0000f460          xchg     byte ptr [ecx + 0x60f40000], al 
  0x00CA6F6F  00a60400001b            add      byte ptr [esi + 0x1b000004], ah 
  0x00CA6F75  f4                      hlt                                     
  0x00CA6F76  6600fb                  add      bl, bh                         
  0x00CA6F79  0c00                    or       al, 0                          
  0x00CA6F7B  0000                    add      byte ptr [eax], al             
  0x00CA6F7D  d8440013                fadd     dword ptr [eax + eax + 0x13]   
  0x00CA6F81  f4                      hlt                                     
  0x00CA6F82  47                      inc      edi                            
  0x00CA6F83  005555                  add      byte ptr [ebp + 0x55], dl      
  0x00CA6F86  d500                    aad      0                              
  0x00CA6F88  00e8                    add      al, ch                         
  0x00CA6F8A  2000                    and      byte ptr [eax], al             
  0x00CA6F8D  de4e00                  fimul    word ptr [esi]                 
  0x00CA6F90  13842100dad844          adc      eax, dword ptr [ecx + 0x44d8da00] 
  0x00CA6F97  0000                    add      byte ptr [eax], al             
  0x00CA6F99  e82000c6de              call     0xdf906fbe                     
  0x00CA6F9E  4e                      dec      esi                            
  0x00CA6F9F  0000                    add      byte ptr [eax], al             
  0x00CA6FA1  8421                    test     byte ptr [ecx], ah             
  0x00CA6FA3  00da                    add      dl, bl                         
  0x00CA6FA5  d8f0                    fdiv     st(0)                          
  0x00CA6FA7  00da                    add      dl, bl                         
  0x00CA6FA9  d8440013                fadd     dword ptr [eax + eax + 0x13]   
  0x00CA6FAD  f4                      hlt                                     
  0x00CA6FAE  47                      inc      edi                            
  0x00CA6FAF  0000                    add      byte ptr [eax], al             
  0x00CA6FB1  00c0                    add      al, al                         
  0x00CA6FB3  0000                    add      byte ptr [eax], al             
  0x00CA6FB5  e82000c6de              call     0xdf906fda                     
  0x00CA6FBA  4e                      dec      esi                            
  0x00CA6FBB  0000                    add      byte ptr [eax], al             
  0x00CA6FBD  8421                    test     byte ptr [ecx], ah             
  0x00CA6FBF  00da                    add      dl, bl                         
  0x00CA6FC1  0020                    add      byte ptr [eax], ah             
  0x00CA6FC3  0000                    add      byte ptr [eax], al             
  0x00CA6FC5  d8f0                    fdiv     st(0)                          
  0x00CA6FC7  00900a060002            add      byte ptr [eax + 0x200060a], dl 
  0x00CA6FCD  0000                    add      byte ptr [eax], al             
  0x00CA6FCF  00da                    add      dl, bl                         
  0x00CA6FD1  d8f0                    fdiv     st(0)                          
  0x00CA6FD3  00da                    add      dl, bl                         
  0x00CA6FD5  0020                    add      byte ptr [eax], ah             
  0x00CA6FD7  00ae1d0c0000            add      byte ptr [esi + 0xc1d], ch     
  0x00CA6FDD  7056                    jo       0xca7035                       
  0x00CA6FDF  007a0b                  add      byte ptr [edx + 0xb], bh       
  0x00CA6FE2  0000                    add      byte ptr [eax], al             
  0x00CA6FE4  020c0500000c05          add      cl, byte ptr [eax + 0x50c0000] 
  0x00CA6FEB  001b                    add      byte ptr [ebx], bl             
  0x00CA6FEE  44                      inc      esp                            
  0x00CA6FEF  007a0b                  add      byte ptr [edx + 0xb], bh       
  0x00CA6FF2  0000                    add      byte ptr [eax], al             
  0x00CA6FF4  004f23                  add      byte ptr [edi + 0x23], cl      
  0x00CA6FF7  004c0020                add      byte ptr [eax + eax + 0x20], cl 
  0x00CA6FFB  0000                    add      byte ptr [eax], al             
  0x00CA6FFD  fa                      cli                                     
  0x00CA6FFE  2100                    and      dword ptr [eax], eax           
  0x00CA7000  0b00                    or       eax, dword ptr [eax]           
  0x00CA7002  2000                    and      byte ptr [eax], al             
  0x00CA7004  02140500030c05          add      dl, byte ptr [eax + 0x50c0300] 
  0x00CA700B  0080100d0033            add      byte ptr [eax + 0x33000d10], al 
  0x00CA7011  fc                      cld                                     
  0x00CA7012  ff00                    inc      dword ptr [eax]                
  0x00CA7014  0c00                    or       al, 0                          
  0x00CA7016  0000                    add      byte ptr [eax], al             
  0x00CA7018  1bf4                    sbb      esi, esp                       
  0x00CA701A  60                      pushal                                  
  0x00CA701B  007a0b                  add      byte ptr [edx + 0xb], bh       
  0x00CA701E  0000                    add      byte ptr [eax], al             
  0x00CA7020  006057                  add      byte ptr [eax + 0x57], ah      
  0x00CA7023  0000                    add      byte ptr [eax], al             
  0x00CA7025  f4                      hlt                                     
  0x00CA7026  60                      pushal                                  
  0x00CA7027  00730b                  add      byte ptr [ebx + 0xb], dh       
  0x00CA702A  0000                    add      byte ptr [eax], al             
  0x00CA702C  00f4                    add      ah, dh                         
  0x00CA702E  6200                    bound    eax, qword ptr [eax]           
  0x00CA7030  790b                    jns      0xca703d                       
  0x00CA7032  0000                    add      byte ptr [eax], al             
  0x00CA7034  00f4                    add      ah, dh                         
  0x00CA7036  6400740b00              add      byte ptr fs:[ebx + ecx], dh    
  0x00CA703B  0000                    add      byte ptr [eax], al             
  0x00CA703E  7000                    jo       0xca7040                       
                                        ; XREF: 0x00CA703E (cond_jump)
  0x00CA7040  97                      xchg     edi, eax                       
  0x00CA7041  0b00                    or       eax, dword ptr [eax]           
  0x00CA7043  0000                    add      byte ptr [eax], al             
  0x00CA7045  95                      xchg     ebp, eax                       
  0x00CA7046  2200                    and      al, byte ptr [eax]             
  0x00CA7048  006057                  add      byte ptr [eax + 0x57], ah      
  0x00CA704B  0000                    add      byte ptr [eax], al             
  0x00CA704D  625700                  bound    edx, qword ptr [edi]           
  0x00CA7050  10d8                    adc      al, bl                         
  0x00CA7052  06                      push     es                             
  0x00CA7053  0002                    add      byte ptr [edx], al             
  0x00CA7055  0000                    add      byte ptr [eax], al             
  0x00CA7057  0000                    add      byte ptr [eax], al             
  0x00CA7059  5d                      pop      ebp                            
  0x00CA705A  57                      push     edi                            
  0x00CA705B  0000                    add      byte ptr [eax], al             
  0x00CA705D  0036                    add      byte ptr [esi], dh             
  0x00CA705F  0000                    add      byte ptr [eax], al             
  0x00CA7062  44                      inc      esp                            
  0x00CA7063  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA7069  c406                    les      eax, ptr [esi]                 
  0x00CA706B  0012                    add      byte ptr [edx], dl             
  0x00CA706D  0000                    add      byte ptr [eax], al             
  0x00CA706F  0000                    add      byte ptr [eax], al             
  0x00CA7071  c422                    les      esp, ptr [edx]                 
  0x00CA7073  0000                    add      byte ptr [eax], al             
  0x00CA7075  f4                      hlt                                     
  0x00CA7076  46                      inc      esi                            
  0x00CA7077  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA707E  44                      inc      esp                            
  0x00CA707F  00720b                  add      byte ptr [edx + 0xb], dh       
  0x00CA7082  0000                    add      byte ptr [eax], al             
  0x00CA7084  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA708A  2000                    and      byte ptr [eax], al             
  0x00CA708C  0090210000f4            add      byte ptr [eax - 0xbffffdf], dl 
  0x00CA7092  56                      push     esi                            
  0x00CA7093  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA7099  c422                    les      esp, ptr [edx]                 
  0x00CA709B  004000                  add      byte ptr [eax], al             
  0x00CA709E  2000                    and      byte ptr [eax], al             
  0x00CA70A0  009221001062            add      byte ptr [edx + 0x62100021], dl 
  0x00CA70A6  06                      push     es                             
  0x00CA70A7  0002                    add      byte ptr [edx], al             
  0x00CA70A9  0000                    add      byte ptr [eax], al             
  0x00CA70AB  0000                    add      byte ptr [eax], al             
  0x00CA70AD  58                      pop      eax                            
  0x00CA70AE  57                      push     edi                            
  0x00CA70AF  0000                    add      byte ptr [eax], al             
  0x00CA70B1  5e                      pop      esi                            
  0x00CA70B2  2000                    and      byte ptr [eax], al             
  0x00CA70B4  00f0                    add      al, dh                         
  0x00CA70B6  56                      push     esi                            
  0x00CA70B7  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA70BA  0000                    add      byte ptr [eax], al             
  0x00CA70BC  0300                    add      eax, dword ptr [eax]           
  0x00CA70BE  2000                    and      byte ptr [eax], al             
  0x00CA70C0  06                      push     es                             
  0x00CA70C1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA70C2  050000f460              add      eax, 0x60f40000                
  0x00CA70C7  0039                    add      byte ptr [ecx], bh             
  0x00CA70C9  0b00                    or       eax, dword ptr [eax]           
  0x00CA70CB  009007060002            add      byte ptr [eax + 0x2000607], dl 
  0x00CA70D1  0000                    add      byte ptr [eax], al             
  0x00CA70D3  0000                    add      byte ptr [eax], al             
  0x00CA70D5  58                      pop      eax                            
  0x00CA70D6  57                      push     edi                            
  0x00CA70D7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA70DA  0000                    add      byte ptr [eax], al             
  0x00CA70DC  00f0                    add      al, dh                         
  0x00CA70DE  44                      inc      esp                            
  0x00CA70DF  00b604000000            add      byte ptr [esi + 4], dh         
  0x00CA70E5  7044                    jo       0xca712b                       
  0x00CA70E7  009f04000080            add      byte ptr [edi - 0x7ffffffc], bl 
  0x00CA70ED  100d00f8feff            adc      byte ptr [0xfffef800], cl      
  0x00CA70F3  000f                    add      byte ptr [edi], cl             
  0x00CA70F5  0a05000c0000            or       al, byte ptr [0xc00]           
  0x00CA70FB  001b                    add      byte ptr [ebx], bl             
  0x00CA70FD  0020                    add      byte ptr [eax], ah             
  0x00CA70FF  008850010088            add      byte ptr [eax - 0x77fffeb0], cl 
  0x00CA7105  50                      push     eax                            
  0x00CA7106  0100                    add      dword ptr [eax], eax           
  0x00CA7108  884201                  mov      byte ptr [edx + 1], al         
  0x00CA710B  008846010088            add      byte ptr [eax - 0x77fffeba], cl 
  0x00CA7111  45                      inc      ebp                            
  0x00CA7112  0100                    add      dword ptr [eax], eax           
  0x00CA7114  884301                  mov      byte ptr [ebx + 1], al         
  0x00CA7117  008843010000            add      byte ptr [eax + 0x143], cl     
  0x00CA711E  56                      push     esi                            
  0x00CA711F  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7122  0000                    add      byte ptr [eax], al             
  0x00CA7124  854101                  test     dword ptr [ecx + 1], eax       
  0x00CA7127  0004a4                  add      byte ptr [esp], al             
  0x00CA712A  0500864101              add      eax, 0x1418600                 
  0x00CA712F  0002                    add      byte ptr [edx], al             
  0x00CA7131  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7132  0500884201              add      eax, 0x1428800                 
  0x00CA7137  0000                    add      byte ptr [eax], al             
  0x00CA713A  56                      push     esi                            
  0x00CA713B  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA713E  0000                    add      byte ptr [eax], al             
  0x00CA7140  86440100                xchg     byte ptr [ecx + eax], al       
  0x00CA7144  02a40500884201          add      ah, byte ptr [ebp + eax + 0x1428800] 
  0x00CA714B  0000                    add      byte ptr [eax], al             
  0x00CA714E  56                      push     esi                            
  0x00CA714F  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7152  0000                    add      byte ptr [eax], al             
  0x00CA7154  854201                  test     dword ptr [edx + 1], eax       
  0x00CA7157  0002                    add      byte ptr [edx], al             
  0x00CA7159  2405                    and      al, 5                          
  0x00CA715B  008842010088            add      byte ptr [eax - 0x77fffebe], cl 
  0x00CA7161  41                      inc      ecx                            
  0x00CA7162  0100                    add      dword ptr [eax], eax           
  0x00CA7164  884501                  mov      byte ptr [ebp + 1], al         
  0x00CA7167  008841010000            add      byte ptr [eax + 0x141], cl     
  0x00CA716E  56                      push     esi                            
  0x00CA716F  004c0b00                add      byte ptr [ebx + ecx], cl       
  0x00CA7173  0003                    add      byte ptr [ebx], al             
  0x00CA7175  f4                      hlt                                     
  0x00CA7176  44                      inc      esp                            
  0x00CA7177  0008                    add      byte ptr [eax], cl             
  0x00CA7179  0000                    add      byte ptr [eax], al             
  0x00CA717B  0002                    add      byte ptr [edx], al             
  0x00CA717D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA717E  0500480020              add      eax, 0x20004800                
  0x00CA7183  008841010088            add      byte ptr [eax - 0x77fffebf], cl 
  0x00CA7189  41                      inc      ecx                            
  0x00CA718A  0100                    add      dword ptr [eax], eax           
  0x00CA718C  884101                  mov      byte ptr [ecx + 1], al         
  0x00CA718F  008841010088            add      byte ptr [eax - 0x77fffebf], cl 
  0x00CA7195  41                      inc      ecx                            
  0x00CA7196  0100                    add      dword ptr [eax], eax           
  0x00CA7198  884101                  mov      byte ptr [ecx + 1], al         
  0x00CA719B  008841010000            add      byte ptr [eax + 0x141], cl     
  0x00CA71A1  7057                    jo       0xca71fa                       
  0x00CA71A3  00510b                  add      byte ptr [ecx + 0xb], dl       
  0x00CA71A6  0000                    add      byte ptr [eax], al             
  0x00CA71A8  0c00                    or       al, 0                          
  0x00CA71AA  0000                    add      byte ptr [eax], al             
  0x00CA71AC  0000                    add      byte ptr [eax], al             
  0x00CA71AE  360000                  add      byte ptr ss:[eax], al          
  0x00CA71B2  44                      inc      esp                            
  0x00CA71B3  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA71B9  c406                    les      eax, ptr [esi]                 
  0x00CA71BB  003400                  add      byte ptr [eax + eax], dh       
  0x00CA71BE  0000                    add      byte ptr [eax], al             
  0x00CA71C0  00f4                    add      ah, dh                         
  0x00CA71C2  56                      push     esi                            
  0x00CA71C3  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA71C9  c422                    les      esp, ptr [edx]                 
  0x00CA71CB  004000                  add      byte ptr [eax], al             
  0x00CA71CE  2000                    and      byte ptr [eax], al             
  0x00CA71D0  0090210000e0            add      byte ptr [eax - 0x1fffffdf], dl 
  0x00CA71D6  56                      push     esi                            
  0x00CA71D7  0003                    add      byte ptr [ebx], al             
  0x00CA71D9  92                      xchg     edx, eax                       
  0x00CA71DA  2100                    and      dword ptr [eax], eax           
  0x00CA71DC  4b                      dec      ebx                            
  0x00CA71DD  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA71DE  050000c422              add      eax, 0x22c40000                
  0x00CA71E3  0000                    add      byte ptr [eax], al             
  0x00CA71E5  f4                      hlt                                     
  0x00CA71E6  46                      inc      esi                            
  0x00CA71E7  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA71ED  f4                      hlt                                     
  0x00CA71EE  44                      inc      esp                            
  0x00CA71EF  00fa                    add      dl, bh                         
  0x00CA71F1  0000                    add      byte ptr [eax], al             
  0x00CA71F3  002e                    add      byte ptr [esi], ch             
  0x00CA71F5  1d0c004000              sbb      eax, 0x40000c                  
                                        ; XREF: 0x00CA71A1 (cond_jump)
  0x00CA71FA  2000                    and      byte ptr [eax], al             
  0x00CA71FC  0090210000f4            add      byte ptr [eax - 0xbffffdf], dl 
  0x00CA7202  56                      push     esi                            
  0x00CA7203  005c0b00                add      byte ptr [ebx + ecx], bl       
  0x00CA7207  0000                    add      byte ptr [eax], al             
  0x00CA7209  c422                    les      esp, ptr [edx]                 
  0x00CA720B  004000                  add      byte ptr [eax], al             
  0x00CA720E  2000                    and      byte ptr [eax], al             
  0x00CA7210  0091210000e1            add      byte ptr [ecx - 0x1effffdf], dl 
  0x00CA7216  7000                    jo       0xca7218                       
                                        ; XREF: 0x00CA7216 (cond_jump)
  0x00CA7218  d9720b                  fnstenv  [edx + 0xb]                    
  0x00CA721B  0012                    add      byte ptr [edx], dl             
  0x00CA721D  0f0000                  sldt     word ptr [eax]                 
  0x00CA7220  00f4                    add      ah, dh                         
  0x00CA7222  56                      push     esi                            
  0x00CA7223  00910b000000            add      byte ptr [ecx + 0xb], dl       
  0x00CA7229  c422                    les      esp, ptr [edx]                 
  0x00CA722B  004000                  add      byte ptr [eax], al             
  0x00CA722E  2000                    and      byte ptr [eax], al             
  0x00CA7230  0091210000e1            add      byte ptr [ecx - 0x1effffdf], dl 
  0x00CA7236  7200                    jb       0xca7238                       
                                        ; XREF: 0x00CA7236 (cond_jump)
  0x00CA7238  00d8                    add      al, bl                         
  0x00CA723A  45                      inc      ebp                            
  0x00CA723B  0000                    add      byte ptr [eax], al             
  0x00CA723D  c422                    les      esp, ptr [edx]                 
  0x00CA723F  0000                    add      byte ptr [eax], al             
  0x00CA7241  f4                      hlt                                     
  0x00CA7242  46                      inc      esi                            
  0x00CA7243  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA7249  f4                      hlt                                     
  0x00CA724A  44                      inc      esp                            
  0x00CA724B  00b00700002e            add      byte ptr [eax + 0x2e000007], dh 
  0x00CA7251  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA7256  2000                    and      byte ptr [eax], al             
  0x00CA7258  00952100005d            add      byte ptr [ebp + 0x5d000021], dl 
  0x00CA725E  45                      inc      ebp                            
  0x00CA725F  0000                    add      byte ptr [eax], al             
  0x00CA7261  c422                    les      esp, ptr [edx]                 
  0x00CA7263  0000                    add      byte ptr [eax], al             
  0x00CA7265  f4                      hlt                                     
  0x00CA7266  46                      inc      esi                            
  0x00CA7267  001f                    add      byte ptr [edi], bl             
  0x00CA7269  0000                    add      byte ptr [eax], al             
  0x00CA726B  00d0                    add      al, dl                         
  0x00CA726D  f4                      hlt                                     
  0x00CA726E  44                      inc      esp                            
  0x00CA726F  0000                    add      byte ptr [eax], al             
  0x00CA7271  0000                    add      byte ptr [eax], al             
  0x00CA7273  002e                    add      byte ptr [esi], ch             
  0x00CA7275  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA727A  2000                    and      byte ptr [eax], al             
  0x00CA727C  00942100005c4d          add      byte ptr [ecx + 0x4d5c0000], dl 
  0x00CA7283  001e                    add      byte ptr [esi], bl             
  0x00CA7285  050d00005e              add      eax, 0x5e00000d                
  0x00CA728A  2000                    and      byte ptr [eax], al             
  0x00CA728C  00f0                    add      al, dh                         
  0x00CA728E  56                      push     esi                            
  0x00CA728F  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA7292  0000                    add      byte ptr [eax], al             
  0x00CA7294  0300                    add      eax, dword ptr [eax]           
  0x00CA7296  2000                    and      byte ptr [eax], al             
  0x00CA7298  13a4050000f056          adc      esp, dword ptr [ebp + eax + 0x56f00000] 
  0x00CA729F  008f0b000003            add      byte ptr [edi + 0x300000b], cl 
  0x00CA72A5  0020                    add      byte ptr [eax], ah             
  0x00CA72A7  000f                    add      byte ptr [edi], cl             
  0x00CA72A9  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA72AA  050000f460              add      eax, 0x60f40000                
  0x00CA72AF  008304000000            add      byte ptr [ebx + 4], al         
  0x00CA72B5  06                      push     es                             
  0x00CA72B6  3800                    cmp      byte ptr [eax], al             
  0x00CA72B8  00f0                    add      al, dh                         
  0x00CA72BA  7900                    jns      0xca72bc                       
                                        ; XREF: 0x00CA72BA (cond_jump)
  0x00CA72BC  130f                    adc      ecx, dword ptr [edi]           
  0x00CA72BE  0000                    add      byte ptr [eax], al             
  0x00CA72C0  0002                    add      byte ptr [edx], al             
  0x00CA72C2  3a00                    cmp      al, byte ptr [eax]             
  0x00CA72C4  00d8                    add      al, bl                         
  0x00CA72C6  45                      inc      ebp                            
  0x00CA72C7  0000                    add      byte ptr [eax], al             
  0x00CA72C9  f4                      hlt                                     
  0x00CA72CA  650039                  add      byte ptr gs:[ecx], bh          
  0x00CA72CD  0b00                    or       eax, dword ptr [eax]           
  0x00CA72CF  0000                    add      byte ptr [eax], al             
  0x00CA72D1  5d                      pop      ebp                            
  0x00CA72D2  45                      inc      ebp                            
  0x00CA72D3  0000                    add      byte ptr [eax], al             
  0x00CA72D5  f4                      hlt                                     
  0x00CA72D6  64009b00000000          add      byte ptr fs:[ebx], bl          
  0x00CA72DD  5c                      pop      esp                            
  0x00CA72DE  4d                      dec      ebp                            
  0x00CA72DF  001e                    add      byte ptr [esi], bl             
  0x00CA72E1  050d000c00              add      eax, 0xc000d                   
  0x00CA72E6  0000                    add      byte ptr [eax], al             
  0x00CA72E8  00f4                    add      ah, dh                         
  0x00CA72EA  56                      push     esi                            
  0x00CA72EB  0013                    add      byte ptr [ebx], dl             
  0x00CA72ED  0000                    add      byte ptr [eax], al             
  0x00CA72EF  0000                    add      byte ptr [eax], al             
  0x00CA72F1  f4                      hlt                                     
  0x00CA72F2  57                      push     edi                            
  0x00CA72F3  0001                    add      byte ptr [ecx], al             
  0x00CA72F5  0000                    add      byte ptr [eax], al             
  0x00CA72F7  0000                    add      byte ptr [eax], al             
  0x00CA72F9  f4                      hlt                                     
  0x00CA72FA  7000                    jo       0xca72fc                       
                                        ; XREF: 0x00CA72FA (cond_jump)
  0x00CA72FC  90                      nop                                     
  0x00CA72FD  0300                    add      eax, dword ptr [eax]           
  0x00CA72FF  0000                    add      byte ptr [eax], al             
  0x00CA7301  0039                    add      byte ptr [ecx], bh             
  0x00CA7303  0000                    add      byte ptr [eax], al             
  0x00CA7305  f4                      hlt                                     
  0x00CA7306  60                      pushal                                  
  0x00CA7307  00fa                    add      dl, bh                         
  0x00CA7309  0000                    add      byte ptr [eax], al             
  0x00CA730B  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA7311  0100                    add      dword ptr [eax], eax           
  0x00CA7313  0003                    add      byte ptr [ebx], al             
  0x00CA7315  0020                    add      byte ptr [eax], ah             
  0x00CA7317  0000                    add      byte ptr [eax], al             
  0x00CA7319  2405                    and      al, 5                          
  0x00CA731B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA731E  0000                    add      byte ptr [eax], al             
  0x00CA7320  0c00                    or       al, 0                          
  0x00CA7322  0000                    add      byte ptr [eax], al             
  0x00CA7324  0c00                    or       al, 0                          
  0x00CA7326  0000                    add      byte ptr [eax], al             
  0x00CA7328  40                      inc      eax                            
  0x00CA7329  1bd0                    sbb      edx, eax                       
  0x00CA732B  00d3                    add      bl, dl                         
  0x00CA732D  06                      push     es                             
  0x00CA732E  0000                    add      byte ptr [eax], al             
  0x00CA7330  50                      push     eax                            
  0x00CA7331  010400                  add      dword ptr [eax + eax], eax     
  0x00CA7334  ed                      in       eax, dx                        
  0x00CA7336  0f0000                  sldt     word ptr [eax]                 
  0x00CA7339  7044                    jo       0xca737f                       
  0x00CA733B  00fb                    add      bl, bh                         
  0x00CA733D  0000                    add      byte ptr [eax], al             
  0x00CA733F  000b                    add      byte ptr [ebx], cl             
  0x00CA7341  0020                    add      byte ptr [eax], ah             
  0x00CA7343  0003                    add      byte ptr [ebx], al             
  0x00CA7345  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7346  050080100d              add      eax, 0xd108000                 
  0x00CA734B  00c0                    add      al, al                         
  0x00CA734D  06                      push     es                             
  0x00CA734E  0000                    add      byte ptr [eax], al             
  0x00CA7350  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA7353  006c0600                add      byte ptr [esi + eax], ch       
  0x00CA7357  0000                    add      byte ptr [eax], al             
  0x00CA735A  57                      push     edi                            
  0x00CA735B  00fb                    add      bl, bh                         
  0x00CA735D  0000                    add      byte ptr [eax], al             
  0x00CA735F  000b                    add      byte ptr [ebx], cl             
  0x00CA7361  f4                      hlt                                     
  0x00CA7362  60                      pushal                                  
  0x00CA7363  001f                    add      byte ptr [edi], bl             
  0x00CA7365  0000                    add      byte ptr [eax], al             
  0x00CA7367  0012                    add      byte ptr [edx], dl             
  0x00CA7369  2405                    and      al, 5                          
  0x00CA736B  0000                    add      byte ptr [eax], al             
  0x00CA736D  002400                  add      byte ptr [eax + eax], ah       
  0x00CA7370  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA7373  004f0b                  add      byte ptr [edi + 0xb], cl       
  0x00CA7376  0000                    add      byte ptr [eax], al             
  0x00CA7378  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA737B  00500b                  add      byte ptr [eax + 0xb], dl       
  0x00CA737E  0000                    add      byte ptr [eax], al             
  0x00CA7380  00f4                    add      ah, dh                         
  0x00CA7382  44                      inc      esp                            
  0x00CA7383  0000                    add      byte ptr [eax], al             
  0x00CA7385  72f8                    jb       0xca737f                       
  0x00CA7387  0000                    add      byte ptr [eax], al             
  0x00CA7389  58                      pop      eax                            
  0x00CA738A  44                      inc      esp                            
  0x00CA738B  0000                    add      byte ptr [eax], al             
  0x00CA738D  f4                      hlt                                     
  0x00CA738E  44                      inc      esp                            
  0x00CA738F  0000                    add      byte ptr [eax], al             
  0x00CA7391  1f                      pop      ds                             
  0x00CA7392  4e                      dec      esi                            
  0x00CA7393  0000                    add      byte ptr [eax], al             
  0x00CA7395  58                      pop      eax                            
  0x00CA7396  44                      inc      esp                            
  0x00CA7397  0000                    add      byte ptr [eax], al             
  0x00CA7399  f4                      hlt                                     
  0x00CA739A  44                      inc      esp                            
  0x00CA739B  0000                    add      byte ptr [eax], al             
  0x00CA739D  0100                    add      dword ptr [eax], eax           
  0x00CA739F  0000                    add      byte ptr [eax], al             
  0x00CA73A1  58                      pop      eax                            
  0x00CA73A2  44                      inc      esp                            
  0x00CA73A3  0000                    add      byte ptr [eax], al             
  0x00CA73A5  f4                      hlt                                     
  0x00CA73A6  44                      inc      esp                            
  0x00CA73A7  0000                    add      byte ptr [eax], al             
  0x00CA73A9  005000                  add      byte ptr [eax], dl             
  0x00CA73AC  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA73AF  0000                    add      byte ptr [eax], al             
  0x00CA73B2  56                      push     esi                            
  0x00CA73B3  004f0b                  add      byte ptr [edi + 0xb], cl       
  0x00CA73B6  0000                    add      byte ptr [eax], al             
  0x00CA73B8  03f0                    add      esi, eax                       
  0x00CA73BA  44                      inc      esp                            
  0x00CA73BB  00500b                  add      byte ptr [eax + 0xb], dl       
  0x00CA73BE  0000                    add      byte ptr [eax], al             
  0x00CA73C0  02a40500005844          add      ah, byte ptr [ebp + eax + 0x44580000] 
  0x00CA73C7  0000                    add      byte ptr [eax], al             
  0x00CA73C9  f4                      hlt                                     
  0x00CA73CA  57                      push     edi                            
  0x00CA73CB  0010                    add      byte ptr [eax], dl             
  0x00CA73CD  0000                    add      byte ptr [eax], al             
  0x00CA73CF  0080100d00a6            add      byte ptr [eax - 0x59fff2f0], al 
  0x00CA73D5  0100                    add      dword ptr [eax], eax           
  0x00CA73D7  0000                    add      byte ptr [eax], al             
  0x00CA73D9  f4                      hlt                                     
  0x00CA73DA  44                      inc      esp                            
  0x00CA73DB  0000                    add      byte ptr [eax], al             
  0x00CA73DD  0000                    add      byte ptr [eax], al             
  0x00CA73DF  004500                  add      byte ptr [ebp], al             
  0x00CA73E2  2000                    and      byte ptr [eax], al             
  0x00CA73E4  00740500                add      byte ptr [ebp + eax], dh       
  0x00CA73E8  005820                  add      byte ptr [eax + 0x20], bl      
  0x00CA73EB  0000                    add      byte ptr [eax], al             
  0x00CA73ED  d85600                  fcom     dword ptr [esi]                
  0x00CA73F0  00f0                    add      al, dh                         
  0x00CA73F2  57                      push     edi                            
  0x00CA73F3  00fb                    add      bl, bh                         
  0x00CA73F5  0000                    add      byte ptr [eax], al             
  0x00CA73F7  000b                    add      byte ptr [ebx], cl             
  0x00CA73F9  f4                      hlt                                     
  0x00CA73FA  44                      inc      esp                            
  0x00CA73FB  000400                  add      byte ptr [eax + eax], al       
  0x00CA73FE  0000                    add      byte ptr [eax], al             
  0x00CA7400  40                      inc      eax                            
  0x00CA7401  2a20                    sub      ah, byte ptr [eax]             
  0x00CA7403  0000                    add      byte ptr [eax], al             
  0x00CA7406  57                      push     edi                            
  0x00CA7407  004f0b                  add      byte ptr [edi + 0xb], cl       
  0x00CA740A  0000                    add      byte ptr [eax], al             
  0x00CA740C  0bf4                    or       esi, esp                       
  0x00CA740E  44                      inc      esp                            
  0x00CA740F  0001                    add      byte ptr [ecx], al             
  0x00CA7411  0000                    add      byte ptr [eax], al             
  0x00CA7413  004022                  add      byte ptr [eax + 0x22], al      
  0x00CA7416  2000                    and      byte ptr [eax], al             
  0x00CA7418  0000                    add      byte ptr [eax], al             
  0x00CA741A  2400                    and      al, 0                          
  0x00CA741C  0000                    add      byte ptr [eax], al             
  0x00CA741E  250000f460              and      eax, 0x60f40000                
  0x00CA7423  001f                    add      byte ptr [edi], bl             
  0x00CA7425  0000                    add      byte ptr [eax], al             
  0x00CA7427  0080cc0c0007            add      byte ptr [eax + 0x7000ccc], al 
  0x00CA742D  0000                    add      byte ptr [eax], al             
  0x00CA742F  0040cc                  add      byte ptr [eax - 0x34], al      
  0x00CA7432  0a00                    or       al, byte ptr [eax]             
  0x00CA7434  0098210000f4            add      byte ptr [eax - 0xbffffdf], bl 
  0x00CA743A  44                      inc      esp                            
  0x00CA743B  0001                    add      byte ptr [ecx], al             
  0x00CA743D  0000                    add      byte ptr [eax], al             
  0x00CA743F  0000                    add      byte ptr [eax], al             
  0x00CA7441  e845000070              call     0x70ca748b                     
  0x00CA7446  44                      inc      esp                            
  0x00CA7447  004f0b                  add      byte ptr [edi + 0xb], cl       
  0x00CA744A  0000                    add      byte ptr [eax], al             
  0x00CA744C  007045                  add      byte ptr [eax + 0x45], dh      
  0x00CA744F  00500b                  add      byte ptr [eax + 0xb], dl       
  0x00CA7452  0000                    add      byte ptr [eax], al             
  0x00CA7454  0084210000f056          add      byte ptr [ecx + 0x56f00000], al 
  0x00CA745B  00fb                    add      bl, bh                         
  0x00CA745D  0000                    add      byte ptr [eax], al             
  0x00CA745F  0080100d0036            add      byte ptr [eax + 0x36000d10], al 
  0x00CA7465  06                      push     es                             
  0x00CA7466  0000                    add      byte ptr [eax], al             
  0x00CA7468  0c00                    or       al, 0                          
  0x00CA746A  0000                    add      byte ptr [eax], al             
  0x00CA746C  61                      popal                                   
  0x00CA746D  f4                      hlt                                     
  0x00CA746E  46                      inc      esi                            
  0x00CA746F  0010                    add      byte ptr [eax], dl             
  0x00CA7471  0000                    add      byte ptr [eax], al             
  0x00CA7473  0000                    add      byte ptr [eax], al             
  0x00CA7475  07                      pop      es                             
  0x00CA7476  2300                    and      eax, dword ptr [eax]           
  0x00CA7478  10d9                    adc      cl, bl                         
  0x00CA747A  06                      push     es                             
  0x00CA747B  000a                    add      byte ptr [edx], cl             
  0x00CA747D  0000                    add      byte ptr [eax], al             
  0x00CA747F  007cd950                add      byte ptr [ecx + ebx*8 + 0x50], bh 
  0x00CA7483  0007                    add      byte ptr [edi], al             
  0x00CA7485  7405                    je       0xca748c                       
  0x00CA7487  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA748A  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA7485 (cond_jump)
  0x00CA748C  46                      inc      esi                            
  0x00CA748D  1e                      push     ds                             
  0x00CA748E  0c00                    or       al, 0                          
  0x00CA7490  90                      nop                                     
  0x00CA7491  1e                      push     ds                             
  0x00CA7492  0c00                    or       al, 0                          
  0x00CA7494  49                      dec      ecx                            
  0x00CA7495  e421                    in       al, 0x21                       
  0x00CA7497  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA749A  54                      push     esp                            
  0x00CA749B  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA749E  0c00                    or       al, 0                          
  0x00CA74A0  4e                      dec      esi                            
  0x00CA74A1  1e                      push     ds                             
  0x00CA74A2  0c00                    or       al, 0                          
  0x00CA74A4  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA74AA  0000                    add      byte ptr [eax], al             
  0x00CA74AC  61                      popal                                   
  0x00CA74AD  f4                      hlt                                     
                                        ; XREF: 0x00CA7500 (cond_jump)
  0x00CA74AE  46                      inc      esi                            
  0x00CA74AF  0010                    add      byte ptr [eax], dl             
  0x00CA74B1  0000                    add      byte ptr [eax], al             
  0x00CA74B3  0000                    add      byte ptr [eax], al             
  0x00CA74B5  07                      pop      es                             
  0x00CA74B6  2300                    and      eax, dword ptr [eax]           
  0x00CA74B8  7cd9                    jl       0xca7493                       
  0x00CA74BA  50                      push     eax                            
  0x00CA74BB  0007                    add      byte ptr [edi], al             
  0x00CA74BD  7405                    je       0xca74c4                       
  0x00CA74BF  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA74C2  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA74BD (cond_jump)
  0x00CA74C4  46                      inc      esi                            
  0x00CA74C5  1e                      push     ds                             
  0x00CA74C6  0c00                    or       al, 0                          
  0x00CA74C8  90                      nop                                     
  0x00CA74C9  1e                      push     ds                             
  0x00CA74CA  0c00                    or       al, 0                          
  0x00CA74CC  49                      dec      ecx                            
  0x00CA74CD  e421                    in       al, 0x21                       
  0x00CA74CF  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA74D2  54                      push     esp                            
  0x00CA74D3  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA74D6  0c00                    or       al, 0                          
  0x00CA74D8  4e                      dec      esi                            
  0x00CA74D9  1e                      push     ds                             
  0x00CA74DA  0c00                    or       al, 0                          
  0x00CA74DC  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA74E2  0000                    add      byte ptr [eax], al             
  0x00CA74E4  00f4                    add      ah, dh                         
  0x00CA74E6  46                      inc      esi                            
  0x00CA74E7  0010                    add      byte ptr [eax], dl             
  0x00CA74E9  0000                    add      byte ptr [eax], al             
  0x00CA74EB  0000                    add      byte ptr [eax], al             
  0x00CA74ED  07                      pop      es                             
  0x00CA74EE  2300                    and      eax, dword ptr [eax]           
  0x00CA74F0  10d9                    adc      cl, bl                         
  0x00CA74F2  06                      push     es                             
  0x00CA74F3  000d00000000            add      byte ptr [0], cl               
  0x00CA74F9  d95600                  fst      dword ptr [esi]                
  0x00CA74FC  6e                      outsb    dx, byte ptr [esi]             
  0x00CA74FD  1e                      push     ds                             
  0x00CA74FE  0c00                    or       al, 0                          
  0x00CA7500  7cac                    jl       0xca74ae                       
  0x00CA7502  2000                    and      byte ptr [eax], al             
  0x00CA7504  07                      pop      es                             
  0x00CA7505  7405                    je       0xca750c                       
  0x00CA7507  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA750A  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA7505 (cond_jump)
  0x00CA750C  46                      inc      esi                            
  0x00CA750D  1e                      push     ds                             
  0x00CA750E  0c00                    or       al, 0                          
  0x00CA7510  90                      nop                                     
  0x00CA7511  1e                      push     ds                             
  0x00CA7512  0c00                    or       al, 0                          
  0x00CA7514  49                      dec      ecx                            
  0x00CA7515  e421                    in       al, 0x21                       
  0x00CA7517  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA751A  54                      push     esp                            
  0x00CA751B  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA751E  0c00                    or       al, 0                          
  0x00CA7520  4e                      dec      esi                            
  0x00CA7521  1e                      push     ds                             
  0x00CA7522  0c00                    or       al, 0                          
  0x00CA7524  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA752A  0000                    add      byte ptr [eax], al             
  0x00CA752C  00f4                    add      ah, dh                         
  0x00CA752E  46                      inc      esi                            
  0x00CA752F  0010                    add      byte ptr [eax], dl             
  0x00CA7531  0000                    add      byte ptr [eax], al             
  0x00CA7533  0000                    add      byte ptr [eax], al             
  0x00CA7535  07                      pop      es                             
                                        ; XREF: 0x00CA7588 (cond_jump)
  0x00CA7536  2300                    and      eax, dword ptr [eax]           
  0x00CA7538  10d9                    adc      cl, bl                         
  0x00CA753A  06                      push     es                             
  0x00CA753B  000d00000000            add      byte ptr [0], cl               
  0x00CA7541  d95e00                  fstp     dword ptr [esi]                
  0x00CA7544  6e                      outsb    dx, byte ptr [esi]             
  0x00CA7545  1e                      push     ds                             
  0x00CA7546  0c00                    or       al, 0                          
  0x00CA7548  7cac                    jl       0xca74f6                       
  0x00CA754A  2000                    and      byte ptr [eax], al             
  0x00CA754C  07                      pop      es                             
  0x00CA754D  7405                    je       0xca7554                       
  0x00CA754F  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA7552  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA754D (cond_jump)
  0x00CA7554  46                      inc      esi                            
  0x00CA7555  1e                      push     ds                             
  0x00CA7556  0c00                    or       al, 0                          
  0x00CA7558  90                      nop                                     
  0x00CA7559  1e                      push     ds                             
  0x00CA755A  0c00                    or       al, 0                          
  0x00CA755C  49                      dec      ecx                            
  0x00CA755D  e421                    in       al, 0x21                       
  0x00CA755F  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA7562  54                      push     esp                            
  0x00CA7563  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA7566  0c00                    or       al, 0                          
  0x00CA7568  4e                      dec      esi                            
  0x00CA7569  1e                      push     ds                             
  0x00CA756A  0c00                    or       al, 0                          
  0x00CA756C  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA7572  0000                    add      byte ptr [eax], al             
  0x00CA7574  00f4                    add      ah, dh                         
                                        ; XREF: 0x00CA75C8 (cond_jump)
  0x00CA7576  46                      inc      esi                            
  0x00CA7577  0010                    add      byte ptr [eax], dl             
  0x00CA7579  0000                    add      byte ptr [eax], al             
  0x00CA757B  0000                    add      byte ptr [eax], al             
  0x00CA757D  07                      pop      es                             
  0x00CA757E  2300                    and      eax, dword ptr [eax]           
  0x00CA7580  00d9                    add      cl, bl                         
  0x00CA7582  56                      push     esi                            
  0x00CA7583  006e1e                  add      byte ptr [esi + 0x1e], ch      
  0x00CA7586  0c00                    or       al, 0                          
  0x00CA7588  7cac                    jl       0xca7536                       
  0x00CA758A  2000                    and      byte ptr [eax], al             
  0x00CA758C  07                      pop      es                             
  0x00CA758D  7405                    je       0xca7594                       
  0x00CA758F  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA7592  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA758D (cond_jump)
  0x00CA7594  46                      inc      esi                            
  0x00CA7595  1e                      push     ds                             
  0x00CA7596  0c00                    or       al, 0                          
  0x00CA7598  90                      nop                                     
  0x00CA7599  1e                      push     ds                             
  0x00CA759A  0c00                    or       al, 0                          
  0x00CA759C  49                      dec      ecx                            
  0x00CA759D  e421                    in       al, 0x21                       
  0x00CA759F  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA75A2  54                      push     esp                            
  0x00CA75A3  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA75A6  0c00                    or       al, 0                          
  0x00CA75A8  4e                      dec      esi                            
  0x00CA75A9  1e                      push     ds                             
  0x00CA75AA  0c00                    or       al, 0                          
  0x00CA75AC  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA75B2  0000                    add      byte ptr [eax], al             
  0x00CA75B4  00f4                    add      ah, dh                         
  0x00CA75B6  46                      inc      esi                            
  0x00CA75B7  0010                    add      byte ptr [eax], dl             
  0x00CA75B9  0000                    add      byte ptr [eax], al             
  0x00CA75BB  0000                    add      byte ptr [eax], al             
  0x00CA75BD  07                      pop      es                             
  0x00CA75BE  2300                    and      eax, dword ptr [eax]           
  0x00CA75C0  00d9                    add      cl, bl                         
  0x00CA75C2  5e                      pop      esi                            
  0x00CA75C3  006e1e                  add      byte ptr [esi + 0x1e], ch      
  0x00CA75C6  0c00                    or       al, 0                          
  0x00CA75C8  7cac                    jl       0xca7576                       
  0x00CA75CA  2000                    and      byte ptr [eax], al             
  0x00CA75CC  07                      pop      es                             
  0x00CA75CD  7405                    je       0xca75d4                       
  0x00CA75CF  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA75D2  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA75CD (cond_jump)
  0x00CA75D4  46                      inc      esi                            
  0x00CA75D5  1e                      push     ds                             
  0x00CA75D6  0c00                    or       al, 0                          
  0x00CA75D8  90                      nop                                     
  0x00CA75D9  1e                      push     ds                             
  0x00CA75DA  0c00                    or       al, 0                          
  0x00CA75DC  49                      dec      ecx                            
  0x00CA75DD  e421                    in       al, 0x21                       
  0x00CA75DF  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA75E2  54                      push     esp                            
  0x00CA75E3  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA75E6  0c00                    or       al, 0                          
  0x00CA75E8  4e                      dec      esi                            
  0x00CA75E9  1e                      push     ds                             
  0x00CA75EA  0c00                    or       al, 0                          
  0x00CA75EC  008521000c00            add      byte ptr [ebp + 0xc0021], al   
  0x00CA75F2  0000                    add      byte ptr [eax], al             
  0x00CA75F4  00f4                    add      ah, dh                         
  0x00CA75F6  61                      popal                                   
  0x00CA75F7  0012                    add      byte ptr [edx], dl             
  0x00CA75F9  0d000000f4              or       eax, 0xf4000000                
  0x00CA75FE  46                      inc      esi                            
  0x00CA75FF  00ff                    add      bh, bh                         
  0x00CA7601  0000                    add      byte ptr [eax], al             
  0x00CA7603  0010                    add      byte ptr [eax], dl             
  0x00CA7605  d806                    fadd     dword ptr [esi]                
  0x00CA7607  000e                    add      byte ptr [esi], cl             
  0x00CA7609  0000                    add      byte ptr [eax], al             
  0x00CA760B  00901c0c0056            add      byte ptr [eax + 0x56000c1c], dl 
  0x00CA7611  0020                    add      byte ptr [eax], ah             
  0x00CA7613  0000                    add      byte ptr [eax], al             
  0x00CA7615  d85100                  fcom     dword ptr [ecx]                
  0x00CA7618  00992100911d            add      byte ptr [ecx + 0x1d910021], bl 
  0x00CA761E  0c00                    or       al, 0                          
  0x00CA7620  00e9                    add      cl, ch                         
  0x00CA7622  4c                      dec      esp                            
  0x00CA7623  004b00                  add      byte ptr [ebx], cl             
  0x00CA7626  2000                    and      byte ptr [eax], al             
  0x00CA7628  90                      nop                                     
  0x00CA7629  1c0c                    sbb      al, 0xc                        
  0x00CA762B  005600                  add      byte ptr [esi], dl             
  0x00CA762E  2000                    and      byte ptr [eax], al             
  0x00CA7630  00992100911d            add      byte ptr [ecx + 0x1d910021], bl 
  0x00CA7636  0c00                    or       al, 0                          
  0x00CA7638  00e9                    add      cl, ch                         
  0x00CA763A  4c                      dec      esp                            
  0x00CA763B  004b00                  add      byte ptr [ebx], cl             
  0x00CA763E  2000                    and      byte ptr [eax], al             
  0x00CA7640  91                      xchg     ecx, eax                       
  0x00CA7641  1e                      push     ds                             
  0x00CA7642  0c00                    or       al, 0                          
  0x00CA7644  00ae2100111c            add      byte ptr [esi + 0x1c110021], ch 
  0x00CA764A  0c00                    or       al, 0                          
  0x00CA764C  0c00                    or       al, 0                          
  0x00CA764E  0000                    add      byte ptr [eax], al             
  0x00CA7650  1bf4                    sbb      esi, esp                       
  0x00CA7652  61                      popal                                   
  0x00CA7653  0012                    add      byte ptr [edx], dl             
  0x00CA7655  0e                      push     cs                             
  0x00CA7656  0000                    add      byte ptr [eax], al             
  0x00CA7658  00f4                    add      ah, dh                         
  0x00CA765A  46                      inc      esi                            
  0x00CA765B  00ff                    add      bh, bh                         
  0x00CA765D  0000                    add      byte ptr [eax], al             
  0x00CA765F  0000                    add      byte ptr [eax], al             
  0x00CA7661  48                      dec      eax                            
  0x00CA7662  2000                    and      byte ptr [eax], al             
  0x00CA7664  10d8                    adc      al, bl                         
  0x00CA7666  06                      push     es                             
  0x00CA7667  000d0000005e            add      byte ptr [0x5e000000], cl      
  0x00CA766D  ae                      scasb    al, byte ptr es:[edi]          
  0x00CA766E  2100                    and      dword ptr [eax], eax           
  0x00CA7670  00f8                    add      al, bh                         
  0x00CA7672  44                      inc      esp                            
  0x00CA7673  0000                    add      byte ptr [eax], al             
  0x00CA7675  b92100d01e              mov      ecx, 0x1ed00021                
  0x00CA767A  0c00                    or       al, 0                          
  0x00CA767C  42                      inc      edx                            
  0x00CA767D  0020                    add      byte ptr [eax], ah             
  0x00CA767F  0000                    add      byte ptr [eax], al             
  0x00CA7681  e94c004300              jmp      0x10d76d2                      
  0x00CA7686  2000                    and      byte ptr [eax], al             
  0x00CA7688  56                      push     esi                            
  0x00CA768A  2100                    and      dword ptr [eax], eax           
  0x00CA768C  00992100d11e            add      byte ptr [ecx + 0x1ed10021], bl 
  0x00CA7692  0c00                    or       al, 0                          
  0x00CA7694  00e9                    add      cl, ch                         
  0x00CA7696  4c                      dec      esp                            
  0x00CA7697  004b00                  add      byte ptr [ebx], cl             
  0x00CA769A  2000                    and      byte ptr [eax], al             
  0x00CA769C  91                      xchg     ecx, eax                       
  0x00CA769D  1e                      push     ds                             
  0x00CA769E  0c00                    or       al, 0                          
  0x00CA76A0  00ae2100111c            add      byte ptr [esi + 0x1c110021], ch 
  0x00CA76A6  0c00                    or       al, 0                          
  0x00CA76A8  0c00                    or       al, 0                          
  0x00CA76AA  0000                    add      byte ptr [eax], al             
  0x00CA76AC  7904                    jns      0xca76b2                       
  0x00CA76AE  0000                    add      byte ptr [eax], al             
  0x00CA76B0  fa                      cli                                     
  0x00CA76B1  0300                    add      eax, dword ptr [eax]           
  0x00CA76B3  0017                    add      byte ptr [edi], dl             
  0x00CA76B5  0400                    add      al, 0                          
  0x00CA76B7  003404                  add      byte ptr [esp + eax], dh       
  0x00CA76BA  0000                    add      byte ptr [eax], al             
  0x00CA76BC  3b0400                  cmp      eax, dword ptr [eax + eax]     
  0x00CA76BF  00540400                add      byte ptr [esp + eax], dl       
  0x00CA76C3  005b04                  add      byte ptr [ebx + 4], bl         
  0x00CA76C6  0000                    add      byte ptr [eax], al             
  0x00CA76C8  5e                      pop      esi                            
  0x00CA76C9  0400                    add      al, 0                          
  0x00CA76CB  006104                  add      byte ptr [ecx + 4], ah         
  0x00CA76CE  0000                    add      byte ptr [eax], al             
  0x00CA76D0  640400                  add      al, 0                          
  0x00CA76D3  006704                  add      byte ptr [edi + 4], ah         
  0x00CA76D6  0000                    add      byte ptr [eax], al             
  0x00CA76D8  6a04                    push     4                              
  0x00CA76DA  0000                    add      byte ptr [eax], al             
  0x00CA76DC  6d                      insd     dword ptr es:[edi], dx         
  0x00CA76DD  0400                    add      al, 0                          
  0x00CA76DF  007004                  add      byte ptr [eax + 4], dh         
  0x00CA76E2  0000                    add      byte ptr [eax], al             
  0x00CA76E4  7304                    jae      0xca76ea                       
  0x00CA76E6  0000                    add      byte ptr [eax], al             
  0x00CA76E8  7604                    jbe      0xca76ee                       
                                        ; XREF: 0x00CA76E4 (cond_jump)
  0x00CA76EA  0000                    add      byte ptr [eax], al             
  0x00CA76EC  00f4                    add      ah, dh                         
                                        ; XREF: 0x00CA76E8 (cond_jump)
  0x00CA76EE  7400                    je       0xca76f0                       
                                        ; XREF: 0x00CA76EE (cond_jump)
  0x00CA76F0  e103                    loope    0xca76f5                       
  0x00CA76F2  0000                    add      byte ptr [eax], al             
  0x00CA76F4  10d8                    adc      al, bl                         
  0x00CA76F6  06                      push     es                             
  0x00CA76F7  008600000000            add      byte ptr [esi], al             
  0x00CA76FD  dd640000                frstor   dword ptr [eax + eax]          
  0x00CA7701  e056                    loopne   0xca7759                       
  0x00CA7703  0096ec070000            add      byte ptr [esi + 0x7ec], dl     
  0x00CA7709  8521                    test     dword ptr [ecx], esp           
  0x00CA770B  0080e60a0000            add      byte ptr [eax + 0xae6], al     
  0x00CA7711  f4                      hlt                                     
  0x00CA7712  44                      inc      esp                            
  0x00CA7713  0003                    add      byte ptr [ebx], al             
  0x00CA7715  0000                    add      byte ptr [eax], al             
  0x00CA7717  00a0f4620005            add      byte ptr [eax + 0x50062f4], ah 
  0x00CA771D  0000                    add      byte ptr [eax], al             
  0x00CA771F  0040f0                  add      byte ptr [eax - 0x10], al      
  0x00CA7722  7200                    jb       0xca7724                       
                                        ; XREF: 0x00CA7722 (cond_jump)
  0x00CA7724  0200                    add      al, byte ptr [eax]             
  0x00CA7726  0000                    add      byte ptr [eax], al             
  0x00CA7728  224f23                  and      cl, byte ptr [edi + 0x23]      
  0x00CA772B  000b                    add      byte ptr [ebx], cl             
  0x00CA772D  0139                    add      dword ptr [ecx], edi           
  0x00CA772F  0000                    add      byte ptr [eax], al             
  0x00CA7731  6a54                    push     0x54                           
  0x00CA7733  000424                  add      byte ptr [esp], al             
  0x00CA7736  0500007060              add      eax, 0x60700000                
  0x00CA773B  000d0000000e            add      byte ptr [0xe000000], cl       
  0x00CA7741  0c05                    or       al, 5                          
  0x00CA7743  0000                    add      byte ptr [eax], al             
  0x00CA7745  f4                      hlt                                     
  0x00CA7746  66000a                  add      byte ptr [edx], cl             
  0x00CA7749  0d00000024              or       eax, 0x24000000                
  0x00CA774E  2300                    and      eax, dword ptr [eax]           
  0x00CA7750  4d                      dec      ebp                            
  0x00CA7751  0239                    add      bh, byte ptr [ecx]             
  0x00CA7753  0009                    add      byte ptr [ecx], cl             
  0x00CA7755  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7756  050000daf0              add      eax, 0xf0da0000                
  0x00CA775B  00d0                    add      al, dl                         
  0x00CA775F  00d2                    add      dl, dl                         
  0x00CA7763  00d2                    add      dl, dl                         
  0x00CA7765  f066000d00000024        lock add byte ptr [0x24000000], cl      
  0x00CA776D  1d0c000000              sbb      eax, 0xc                       
  0x00CA7772  3900                    cmp      dword ptr [eax], eax           
  0x00CA7774  006650                  add      byte ptr [esi + 0x50], ah      
  0x00CA7777  0013                    add      byte ptr [ebx], dl             
  0x00CA7779  7071                    jo       0xca77ec                       
  0x00CA777B  0002                    add      byte ptr [edx], al             
  0x00CA777D  0000                    add      byte ptr [eax], al             
  0x00CA777F  00c3                    add      bl, al                         
  0x00CA7781  0c05                    or       al, 5                          
  0x00CA7783  0000                    add      byte ptr [eax], al             
  0x00CA7785  f4                      hlt                                     
  0x00CA7786  44                      inc      esp                            
  0x00CA7787  0005000000a0            add      byte ptr [0xa0000000], al      
  0x00CA778D  f4                      hlt                                     
  0x00CA778E  6200                    bound    eax, qword ptr [eax]           
  0x00CA7790  0800                    or       byte ptr [eax], al             
  0x00CA7792  0000                    add      byte ptr [eax], al             
  0x00CA7794  40                      inc      eax                            
  0x00CA7796  7200                    jb       0xca7798                       
                                        ; XREF: 0x00CA7796 (cond_jump)
  0x00CA7798  0300                    add      eax, dword ptr [eax]           
  0x00CA779A  0000                    add      byte ptr [eax], al             
  0x00CA779C  224f23                  and      cl, byte ptr [edi + 0x23]      
  0x00CA779F  000b                    add      byte ptr [ebx], cl             
  0x00CA77A1  0139                    add      dword ptr [ecx], edi           
  0x00CA77A3  0000                    add      byte ptr [eax], al             
  0x00CA77A5  6a54                    push     0x54                           
  0x00CA77A7  000424                  add      byte ptr [esp], al             
  0x00CA77AA  0500007060              add      eax, 0x60700000                
  0x00CA77AF  000e                    add      byte ptr [esi], cl             
  0x00CA77B1  0000                    add      byte ptr [eax], al             
  0x00CA77B3  000e                    add      byte ptr [esi], cl             
  0x00CA77B5  0c05                    or       al, 5                          
  0x00CA77B7  0000                    add      byte ptr [eax], al             
  0x00CA77B9  f4                      hlt                                     
  0x00CA77BA  66000d0d000000          add      byte ptr [0xd], cl             
  0x00CA77C1  2423                    and      al, 0x23                       
  0x00CA77C3  004d02                  add      byte ptr [ebp + 2], cl         
  0x00CA77C6  3900                    cmp      dword ptr [eax], eax           
  0x00CA77C8  09a4050000daf0          or       dword ptr [ebp + eax - 0xf260000], esp 
  0x00CA77CF  00d0                    add      al, dl                         
  0x00CA77D3  00d2                    add      dl, dl                         
  0x00CA77D7  00d2                    add      dl, dl                         
  0x00CA77D9  f066000e                lock add byte ptr [esi], cl             
  0x00CA77DD  0000                    add      byte ptr [eax], al             
  0x00CA77DF  0020                    add      byte ptr [eax], ah             
  0x00CA77E1  1d0c000000              sbb      eax, 0xc                       
  0x00CA77E6  3900                    cmp      dword ptr [eax], eax           
  0x00CA77E8  006650                  add      byte ptr [esi + 0x50], ah      
  0x00CA77EB  0013                    add      byte ptr [ebx], dl             
  0x00CA77ED  7071                    jo       0xca7860                       
  0x00CA77EF  0003                    add      byte ptr [ebx], al             
  0x00CA77F1  0000                    add      byte ptr [eax], al             
  0x00CA77F3  00860c050000            add      byte ptr [esi + 0x50c], al     
  0x00CA77F9  f4                      hlt                                     
  0x00CA77FA  44                      inc      esp                            
  0x00CA77FB  0007                    add      byte ptr [edi], al             
  0x00CA77FD  0000                    add      byte ptr [eax], al             
  0x00CA77FF  00a000200040            add      byte ptr [eax + 0x40002000], ah 
  0x00CA7805  0020                    add      byte ptr [eax], ah             
  0x00CA7807  0038                    add      byte ptr [eax], bh             
  0x00CA7809  1d0c00101c              sbb      eax, 0x1c10000c                
  0x00CA780E  0c00                    or       al, 0                          
  0x00CA7810  5f                      pop      edi                            
  0x00CA7811  0c05                    or       al, 5                          
  0x00CA7813  0000                    add      byte ptr [eax], al             
  0x00CA7815  f4                      hlt                                     
  0x00CA7816  44                      inc      esp                            
  0x00CA7817  000b                    add      byte ptr [ebx], cl             
  0x00CA7819  0000                    add      byte ptr [eax], al             
  0x00CA781B  00a0f462000b            add      byte ptr [eax + 0xb0062f4], ah 
  0x00CA7821  0000                    add      byte ptr [eax], al             
  0x00CA7823  0040f0                  add      byte ptr [eax - 0x10], al      
  0x00CA7826  7200                    jb       0xca7828                       
                                        ; XREF: 0x00CA7826 (cond_jump)
  0x00CA7828  0400                    add      al, 0                          
  0x00CA782A  0000                    add      byte ptr [eax], al             
  0x00CA782C  224f23                  and      cl, byte ptr [edi + 0x23]      
  0x00CA782F  000b                    add      byte ptr [ebx], cl             
  0x00CA7831  0139                    add      dword ptr [ecx], edi           
  0x00CA7833  0000                    add      byte ptr [eax], al             
  0x00CA7835  f4                      hlt                                     
  0x00CA7836  660010                  add      byte ptr [eax], dl             
  0x00CA7839  0d0000006a              or       eax, 0x6a000000                
  0x00CA783E  54                      push     esp                            
  0x00CA783F  000424                  add      byte ptr [esp], al             
  0x00CA7842  0500007060              add      eax, 0x60700000                
  0x00CA7847  000f                    add      byte ptr [edi], cl             
  0x00CA7849  0000                    add      byte ptr [eax], al             
  0x00CA784B  0008                    add      byte ptr [eax], cl             
  0x00CA784D  0c05                    or       al, 5                          
  0x00CA784F  0000                    add      byte ptr [eax], al             
  0x00CA7853  00d0                    add      al, dl                         
  0x00CA7857  00d2                    add      dl, dl                         
  0x00CA7859  f066000f                lock add byte ptr [edi], cl             
  0x00CA785D  0000                    add      byte ptr [eax], al             
  0x00CA785F  0020                    add      byte ptr [eax], ah             
  0x00CA7861  1d0c000000              sbb      eax, 0xc                       
  0x00CA7866  3900                    cmp      dword ptr [eax], eax           
  0x00CA7868  006650                  add      byte ptr [esi + 0x50], ah      
  0x00CA786B  0013                    add      byte ptr [ebx], dl             
  0x00CA786D  7071                    jo       0xca78e0                       
  0x00CA786F  000400                  add      byte ptr [eax + eax], al       
  0x00CA7872  0000                    add      byte ptr [eax], al             
  0x00CA7874  46                      inc      esi                            
  0x00CA7875  0c05                    or       al, 5                          
  0x00CA7877  0000                    add      byte ptr [eax], al             
  0x00CA7879  f4                      hlt                                     
  0x00CA787A  44                      inc      esp                            
  0x00CA787B  000f                    add      byte ptr [edi], cl             
  0x00CA787D  0000                    add      byte ptr [eax], al             
  0x00CA787F  00a000200040            add      byte ptr [eax + 0x40002000], ah 
  0x00CA7885  0020                    add      byte ptr [eax], ah             
  0x00CA7887  0036                    add      byte ptr [esi], dh             
  0x00CA7889  1d0c00101c              sbb      eax, 0x1c10000c                
  0x00CA788E  0c00                    or       al, 0                          
  0x00CA7890  1f                      pop      ds                             
  0x00CA7891  0c05                    or       al, 5                          
  0x00CA7893  0000                    add      byte ptr [eax], al             
  0x00CA7895  f4                      hlt                                     
  0x00CA7896  56                      push     esi                            
  0x00CA7897  0000                    add      byte ptr [eax], al             
  0x00CA7899  000400                  add      byte ptr [eax + eax], al       
  0x00CA789C  1b0c050000f456          sbb      ecx, dword ptr [eax + 0x56f40000] 
  0x00CA78A3  0000                    add      byte ptr [eax], al             
  0x00CA78A5  0002                    add      byte ptr [edx], al             
  0x00CA78A7  0018                    add      byte ptr [eax], bl             
  0x00CA78A9  0c05                    or       al, 5                          
  0x00CA78AB  0000                    add      byte ptr [eax], al             
  0x00CA78AD  f4                      hlt                                     
  0x00CA78AE  56                      push     esi                            
  0x00CA78AF  0000                    add      byte ptr [eax], al             
  0x00CA78B1  0001                    add      byte ptr [ecx], al             
  0x00CA78B3  00150c050000            add      byte ptr [0x50c], dl           
  0x00CA78B9  f4                      hlt                                     
  0x00CA78BA  56                      push     esi                            
  0x00CA78BB  0000                    add      byte ptr [eax], al             
  0x00CA78BD  800000                  add      byte ptr [eax], 0              
  0x00CA78C0  120c050000f456          adc      cl, byte ptr [eax + 0x56f40000] 
  0x00CA78C7  0000                    add      byte ptr [eax], al             
  0x00CA78C9  40                      inc      eax                            
  0x00CA78CA  0000                    add      byte ptr [eax], al             
  0x00CA78CD  0c05                    or       al, 5                          
  0x00CA78CF  0000                    add      byte ptr [eax], al             
  0x00CA78D1  f4                      hlt                                     
  0x00CA78D2  56                      push     esi                            
  0x00CA78D3  0000                    add      byte ptr [eax], al             
  0x00CA78D5  2000                    and      byte ptr [eax], al             
  0x00CA78D7  000c0c                  add      byte ptr [esp + ecx], cl       
  0x00CA78DA  050000f456              add      eax, 0x56f40000                
  0x00CA78DF  0000                    add      byte ptr [eax], al             
  0x00CA78E1  1000                    adc      byte ptr [eax], al             
  0x00CA78E3  0009                    add      byte ptr [ecx], cl             
  0x00CA78E5  0c05                    or       al, 5                          
  0x00CA78E7  0000                    add      byte ptr [eax], al             
  0x00CA78E9  f4                      hlt                                     
  0x00CA78EA  56                      push     esi                            
  0x00CA78EB  0000                    add      byte ptr [eax], al             
  0x00CA78ED  0800                    or       byte ptr [eax], al             
  0x00CA78EF  0006                    add      byte ptr [esi], al             
  0x00CA78F1  0c05                    or       al, 5                          
  0x00CA78F3  0000                    add      byte ptr [eax], al             
  0x00CA78F5  f4                      hlt                                     
  0x00CA78F6  56                      push     esi                            
  0x00CA78F7  0000                    add      byte ptr [eax], al             
  0x00CA78F9  0400                    add      al, 0                          
  0x00CA78FB  0003                    add      byte ptr [ebx], al             
  0x00CA78FD  0c05                    or       al, 5                          
  0x00CA78FF  0000                    add      byte ptr [eax], al             
  0x00CA7901  f4                      hlt                                     
  0x00CA7902  56                      push     esi                            
  0x00CA7903  0000                    add      byte ptr [eax], al             
  0x00CA7905  0100                    add      dword ptr [eax], eax           
  0x00CA7907  006000                  add      byte ptr [eax], ah             
  0x00CA790A  2000                    and      byte ptr [eax], al             
  0x00CA790C  005856                  add      byte ptr [eax + 0x56], bl      
  0x00CA790F  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA7912  0000                    add      byte ptr [eax], al             
  0x00CA7914  c6040000                mov      byte ptr [eax + eax], 0        
  0x00CA7918  b204                    mov      dl, 4                          
  0x00CA791A  0000                    add      byte ptr [eax], al             
  0x00CA791C  a804                    test     al, 4                          
  0x00CA791E  0000                    add      byte ptr [eax], al             
  0x00CA7920  bb0400009e              mov      ebx, 0x9e000004                
  0x00CA7925  0400                    add      al, 0                          
  0x00CA7927  00bb040000bb            add      byte ptr [ebx - 0x44fffffc], bh 
  0x00CA792D  0400                    add      al, 0                          
  0x00CA792F  00bb040000bb            add      byte ptr [ebx - 0x44fffffc], bh 
  0x00CA7935  0400                    add      al, 0                          
  0x00CA7937  00bb040000bb            add      byte ptr [ebx - 0x44fffffc], bh 
  0x00CA793D  0400                    add      al, 0                          
  0x00CA793F  00bb040000bb            add      byte ptr [ebx - 0x44fffffc], bh 
  0x00CA7945  0400                    add      al, 0                          
  0x00CA7947  00bb040000bb            add      byte ptr [ebx - 0x44fffffc], bh 
  0x00CA794D  0400                    add      al, 0                          
  0x00CA794F  00bb04000000            add      byte ptr [ebx + 4], bh         
  0x00CA7956  6200                    bound    eax, qword ptr [eax]           
  0x00CA7958  55                      push     ebp                            
  0x00CA7959  0b00                    or       eax, dword ptr [eax]           
  0x00CA795B  0022                    add      byte ptr [edx], ah             
  0x00CA795E  0500470b00              add      eax, 0xb4700                   
  0x00CA7963  0000                    add      byte ptr [eax], al             
  0x00CA7966  56                      push     esi                            
  0x00CA7967  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA796A  0000                    add      byte ptr [eax], al             
  0x00CA796C  00f0                    add      al, dh                         
  0x00CA796E  45                      inc      ebp                            
  0x00CA796F  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA7972  0000                    add      byte ptr [eax], al             
  0x00CA7974  00f4                    add      ah, dh                         
  0x00CA7976  46                      inc      esi                            
  0x00CA7977  0010                    add      byte ptr [eax], dl             
  0x00CA7979  0000                    add      byte ptr [eax], al             
  0x00CA797B  0000                    add      byte ptr [eax], al             
  0x00CA797D  f4                      hlt                                     
  0x00CA797E  7400                    je       0xca7980                       
                                        ; XREF: 0x00CA797E (cond_jump)
  0x00CA7980  7b04                    jnp      0xca7986                       
  0x00CA7982  0000                    add      byte ptr [eax], al             
  0x00CA7984  10d8                    adc      al, bl                         
                                        ; XREF: 0x00CA7980 (cond_jump)
  0x00CA7986  06                      push     es                             
  0x00CA7987  002f                    add      byte ptr [edi], ch             
  0x00CA7989  0000                    add      byte ptr [eax], al             
  0x00CA798B  0000                    add      byte ptr [eax], al             
  0x00CA798D  dd640096                frstor   dword ptr [eax + eax - 0x6a]   
  0x00CA7991  ec                      in       al, dx                         
  0x00CA7992  07                      pop      es                             
  0x00CA7993  00c7                    add      bh, al                         
  0x00CA7995  740b                    je       0xca79a2                       
  0x00CA7997  00fa                    add      dl, bh                         
  0x00CA7999  0c00                    or       al, 0                          
  0x00CA799B  0080e60a0000            add      byte ptr [eax + 0xae6], al     
                                        ; XREF: 0x00CA7995 (cond_jump)
  0x00CA79A2  57                      push     edi                            
  0x00CA79A3  000400                  add      byte ptr [eax + eax], al       
  0x00CA79A6  0000                    add      byte ptr [eax], al             
  0x00CA79A8  8c4101                  mov      word ptr [ecx + 1], es         
  0x00CA79AB  0000                    add      byte ptr [eax], al             
  0x00CA79AD  7055                    jo       0xca7a04                       
  0x00CA79AF  000400                  add      byte ptr [eax + eax], al       
  0x00CA79B2  0000                    add      byte ptr [eax], al             
  0x00CA79B4  43                      inc      ebx                            
  0x00CA79B5  2405                    and      al, 5                          
  0x00CA79B7  0000                    add      byte ptr [eax], al             
  0x00CA79B9  0239                    add      bh, byte ptr [ecx]             
  0x00CA79BB  0000                    add      byte ptr [eax], al             
  0x00CA79BD  7071                    jo       0xca7a30                       
  0x00CA79BF  000400                  add      byte ptr [eax + eax], al       
  0x00CA79C2  0000                    add      byte ptr [eax], al             
  0x00CA79C4  140c                    adc      al, 0xc                        
  0x00CA79C6  050000f057              add      eax, 0x57f00000                
  0x00CA79CB  0003                    add      byte ptr [ebx], al             
  0x00CA79CD  0000                    add      byte ptr [eax], al             
  0x00CA79CF  008c4101000070          add      byte ptr [ecx + eax*2 + 0x70000001], cl 
  0x00CA79D6  55                      push     ebp                            
  0x00CA79D7  0003                    add      byte ptr [ebx], al             
  0x00CA79D9  0000                    add      byte ptr [eax], al             
  0x00CA79DB  0019                    add      byte ptr [ecx], bl             
  0x00CA79DD  2405                    and      al, 5                          
  0x00CA79DF  0000                    add      byte ptr [eax], al             
  0x00CA79E1  0339                    add      edi, dword ptr [ecx]           
  0x00CA79E3  0000                    add      byte ptr [eax], al             
  0x00CA79E5  7071                    jo       0xca7a58                       
  0x00CA79E7  0003                    add      byte ptr [ebx], al             
  0x00CA79E9  0000                    add      byte ptr [eax], al             
  0x00CA79EB  000a                    add      byte ptr [edx], cl             
  0x00CA79ED  0c05                    or       al, 5                          
  0x00CA79EF  0000                    add      byte ptr [eax], al             
  0x00CA79F2  57                      push     edi                            
  0x00CA79F3  0002                    add      byte ptr [edx], al             
  0x00CA79F5  0000                    add      byte ptr [eax], al             
  0x00CA79F7  008c4101000070          add      byte ptr [ecx + eax*2 + 0x70000001], cl 
  0x00CA79FE  55                      push     ebp                            
  0x00CA79FF  0002                    add      byte ptr [edx], al             
  0x00CA7A01  0000                    add      byte ptr [eax], al             
  0x00CA7A03  000f                    add      byte ptr [edi], cl             
  0x00CA7A05  2405                    and      al, 5                          
  0x00CA7A07  0000                    add      byte ptr [eax], al             
  0x00CA7A09  0339                    add      edi, dword ptr [ecx]           
  0x00CA7A0B  0000                    add      byte ptr [eax], al             
  0x00CA7A0D  7071                    jo       0xca7a80                       
  0x00CA7A0F  0002                    add      byte ptr [edx], al             
  0x00CA7A11  0000                    add      byte ptr [eax], al             
  0x00CA7A13  006900                  add      byte ptr [ecx], ch             
  0x00CA7A16  2000                    and      byte ptr [eax], al             
  0x00CA7A18  7ce0                    jl       0xca79fa                       
  0x00CA7A1A  50                      push     eax                            
  0x00CA7A1B  0007                    add      byte ptr [edi], al             
  0x00CA7A1D  7405                    je       0xca7a24                       
  0x00CA7A1F  0078e4                  add      byte ptr [eax - 0x1c], bh      
  0x00CA7A22  2100                    and      dword ptr [eax], eax           
                                        ; XREF: 0x00CA7A1D (cond_jump)
  0x00CA7A24  46                      inc      esi                            
  0x00CA7A25  1e                      push     ds                             
  0x00CA7A26  0c00                    or       al, 0                          
  0x00CA7A28  90                      nop                                     
  0x00CA7A29  1e                      push     ds                             
  0x00CA7A2A  0c00                    or       al, 0                          
  0x00CA7A2C  49                      dec      ecx                            
  0x00CA7A2D  e421                    in       al, 0x21                       
  0x00CA7A2F  00585a                  add      byte ptr [eax + 0x5a], bl      
  0x00CA7A32  54                      push     esp                            
  0x00CA7A33  00681e                  add      byte ptr [eax + 0x1e], ch      
  0x00CA7A36  0c00                    or       al, 0                          
  0x00CA7A38  4e                      dec      esi                            
  0x00CA7A39  1e                      push     ds                             
  0x00CA7A3A  0c00                    or       al, 0                          
  0x00CA7A3C  00a521000058            add      byte ptr [ebp + 0x58000021], ah 
  0x00CA7A42  2000                    and      byte ptr [eax], al             
  0x00CA7A44  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA7A47  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA7A4A  0000                    add      byte ptr [eax], al             
  0x00CA7A4C  007045                  add      byte ptr [eax + 0x45], dh      
  0x00CA7A4F  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA7A52  0000                    add      byte ptr [eax], al             
  0x00CA7A54  007062                  add      byte ptr [eax + 0x62], dh      
  0x00CA7A57  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA7A5A  0000                    add      byte ptr [eax], al             
  0x00CA7A5C  22f4                    and      dh, ah                         
  0x00CA7A5E  0500ffff00              add      eax, 0xffff00                  
  0x00CA7A63  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA7A66  0000                    add      byte ptr [eax], al             
  0x00CA7A68  20f4                    and      ah, dh                         
  0x00CA7A6A  0500ffffff              add      eax, 0xffffff00                
  0x00CA7A6F  00a0610400a0            add      byte ptr [eax - 0x5ffffb9f], ah 
  0x00CA7A75  620400                  bound    eax, qword ptr [eax + eax]     
  0x00CA7A78  a0640400a0              mov      al, byte ptr [0xa0000464]      
  0x00CA7A7D  650400                  add      al, 0                          
                                        ; XREF: 0x00CA7A0D (cond_jump)
  0x00CA7A80  a0660400b8              mov      al, byte ptr [0xb8000466]      
  0x00CA7A85  f30000                  add      byte ptr [eax], al             
  0x00CA7A88  00f4                    add      ah, dh                         
  0x00CA7A8A  44                      inc      esp                            
  0x00CA7A8B  0000                    add      byte ptr [eax], al             
  0x00CA7A8D  0000                    add      byte ptr [eax], al             
  0x00CA7A8F  004d00                  add      byte ptr [ebp], cl             
  0x00CA7A92  2000                    and      byte ptr [eax], al             
  0x00CA7A94  0ca4                    or       al, 0xa4                       
  0x00CA7A96  050000f444              add      eax, 0x44f40000                
  0x00CA7A9B  0010                    add      byte ptr [eax], dl             
  0x00CA7A9D  0000                    add      byte ptr [eax], al             
  0x00CA7A9F  004d00                  add      byte ptr [ebp], cl             
  0x00CA7AA2  2000                    and      byte ptr [eax], al             
  0x00CA7AA4  4a                      dec      edx                            
  0x00CA7AA5  100d000f0000            adc      byte ptr [0xf00], cl           
  0x00CA7AAB  0000                    add      byte ptr [eax], al             
  0x00CA7AAD  0030                    add      byte ptr [eax], dh             
  0x00CA7AAF  0000                    add      byte ptr [eax], al             
  0x00CA7AB1  f4                      hlt                                     
  0x00CA7AB2  56                      push     esi                            
  0x00CA7AB3  0000                    add      byte ptr [eax], al             
  0x00CA7AB5  0000                    add      byte ptr [eax], al             
  0x00CA7AB7  0000                    add      byte ptr [eax], al             
  0x00CA7AB9  f4                      hlt                                     
  0x00CA7ABA  57                      push     edi                            
  0x00CA7ABB  00ff                    add      bh, bh                         
  0x00CA7ABE  ff00                    inc      dword ptr [eax]                
  0x00CA7AC0  0c00                    or       al, 0                          
  0x00CA7AC2  0000                    add      byte ptr [eax], al             
  0x00CA7AC4  1300                    adc      eax, dword ptr [eax]           
  0x00CA7AC6  2000                    and      byte ptr [eax], al             
  0x00CA7AC8  0000                    add      byte ptr [eax], al             
  0x00CA7ACA  3000                    xor      byte ptr [eax], al             
  0x00CA7ACC  00f4                    add      ah, dh                         
  0x00CA7ACE  56                      push     esi                            
  0x00CA7ACF  0000                    add      byte ptr [eax], al             
  0x00CA7AD1  0000                    add      byte ptr [eax], al             
  0x00CA7AD3  0000                    add      byte ptr [eax], al             
  0x00CA7AD5  f4                      hlt                                     
  0x00CA7AD6  57                      push     edi                            
  0x00CA7AD7  0008                    add      byte ptr [eax], cl             
  0x00CA7AD9  06                      push     es                             
  0x00CA7ADA  0000                    add      byte ptr [eax], al             
  0x00CA7ADC  0c00                    or       al, 0                          
  0x00CA7ADE  0000                    add      byte ptr [eax], al             
  0x00CA7AE0  00f0                    add      al, dh                         
  0x00CA7AE2  56                      push     esi                            
  0x00CA7AE3  00960b000003            add      byte ptr [esi + 0x300000b], dl 
  0x00CA7AE9  0020                    add      byte ptr [eax], ah             
  0x00CA7AEB  005874                  add      byte ptr [eax + 0x74], bl      
  0x00CA7AEE  0500007060              add      eax, 0x60700000                
  0x00CA7AF3  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA7AF6  0000                    add      byte ptr [eax], al             
  0x00CA7AF8  007060                  add      byte ptr [eax + 0x60], dh      
  0x00CA7AFB  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA7AFE  0000                    add      byte ptr [eax], al             
  0x00CA7B00  00f4                    add      ah, dh                         
  0x00CA7B02  45                      inc      ebp                            
  0x00CA7B03  00b007000000            add      byte ptr [eax + 7], dh         
  0x00CA7B09  7045                    jo       0xca7b50                       
  0x00CA7B0B  00720b                  add      byte ptr [edx + 0xb], dh       
  0x00CA7B0E  0000                    add      byte ptr [eax], al             
  0x00CA7B10  00f0                    add      al, dh                         
  0x00CA7B12  56                      push     esi                            
  0x00CA7B13  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA7B16  0000                    add      byte ptr [eax], al             
  0x00CA7B18  0300                    add      eax, dword ptr [eax]           
  0x00CA7B1A  2400                    and      al, 0                          
  0x00CA7B1C  09240500007044          or       dword ptr [eax + 0x44700000], esp 
  0x00CA7B23  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA7B26  0000                    add      byte ptr [eax], al             
  0x00CA7B28  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA7B2B  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA7B2E  0000                    add      byte ptr [eax], al             
  0x00CA7B30  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA7B33  009d0b000080            add      byte ptr [ebp - 0x7ffffff5], bl 
  0x00CA7B39  100d008c0000            adc      byte ptr [0x8c00], cl          
  0x00CA7B3F  0080100d0029            add      byte ptr [eax + 0x29000d10], al 
  0x00CA7B45  0100                    add      dword ptr [eax], eax           
  0x00CA7B47  0080100d009e            add      byte ptr [eax - 0x61fff2f0], al 
  0x00CA7B4D  0100                    add      dword ptr [eax], eax           
  0x00CA7B4F  0080100d0066            add      byte ptr [eax + 0x66000d10], al 
  0x00CA7B55  0300                    add      eax, dword ptr [eax]           
  0x00CA7B57  0000                    add      byte ptr [eax], al             
  0x00CA7B5A  56                      push     esi                            
  0x00CA7B5B  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA7B5E  0000                    add      byte ptr [eax], al             
  0x00CA7B60  854501                  test     dword ptr [ebp + 1], eax       
  0x00CA7B63  0003                    add      byte ptr [ebx], al             
  0x00CA7B65  2405                    and      al, 5                          
  0x00CA7B67  0080100d0093            add      byte ptr [eax - 0x6cfff2f0], al 
  0x00CA7B6D  0300                    add      eax, dword ptr [eax]           
  0x00CA7B6F  0000                    add      byte ptr [eax], al             
  0x00CA7B72  56                      push     esi                            
  0x00CA7B73  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA7B76  0000                    add      byte ptr [eax], al             
  0x00CA7B78  00f0                    add      al, dh                         
  0x00CA7B7A  44                      inc      esp                            
  0x00CA7B7B  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA7B7E  0000                    add      byte ptr [eax], al             
  0x00CA7B80  44                      inc      esp                            
  0x00CA7B81  0020                    add      byte ptr [eax], ah             
  0x00CA7B83  0000                    add      byte ptr [eax], al             
  0x00CA7B85  7054                    jo       0xca7bdb                       
  0x00CA7B87  00580b                  add      byte ptr [eax + 0xb], bl       
  0x00CA7B8A  0000                    add      byte ptr [eax], al             
  0x00CA7B8C  80100d                  adc      byte ptr [eax], 0xd            
  0x00CA7B8F  00d0                    add      al, dl                         
  0x00CA7B91  0300                    add      eax, dword ptr [eax]           
  0x00CA7B93  0000                    add      byte ptr [eax], al             
  0x00CA7B96  56                      push     esi                            
  0x00CA7B97  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA7B9A  0000                    add      byte ptr [eax], al             
  0x00CA7B9C  854501                  test     dword ptr [ebp + 1], eax       
  0x00CA7B9F  000b                    add      byte ptr [ebx], cl             
  0x00CA7BA1  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7BA2  050000f044              add      eax, 0x44f00000                
  0x00CA7BA7  00580b                  add      byte ptr [eax + 0xb], bl       
  0x00CA7BAA  0000                    add      byte ptr [eax], al             
  0x00CA7BAC  00f4                    add      ah, dh                         
  0x00CA7BAE  45                      inc      ebp                            
  0x00CA7BAF  0010                    add      byte ptr [eax], dl             
  0x00CA7BB1  0000                    add      byte ptr [eax], al             
  0x00CA7BB3  00a0f044009d            add      byte ptr [eax - 0x62ffbb10], ah 
  0x00CA7BB9  0b00                    or       eax, dword ptr [eax]           
  0x00CA7BBB  002e                    add      byte ptr [esi], ch             
  0x00CA7BBD  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA7BC2  2000                    and      byte ptr [eax], al             
  0x00CA7BC4  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA7BC7  009d0b000080            add      byte ptr [ebp - 0x7ffffff5], bl 
  0x00CA7BCD  100d00080000            adc      byte ptr [0x800], cl           
  0x00CA7BD3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA7BD6  0000                    add      byte ptr [eax], al             
  0x00CA7BD8  00f4                    add      ah, dh                         
  0x00CA7BDA  44                      inc      esp                            
                                        ; XREF: 0x00CA7B85 (cond_jump)
  0x00CA7BDB  0000                    add      byte ptr [eax], al             
  0x00CA7BDD  0100                    add      dword ptr [eax], eax           
  0x00CA7BDF  0000                    add      byte ptr [eax], al             
  0x00CA7BE1  7044                    jo       0xca7c27                       
  0x00CA7BE3  0010                    add      byte ptr [eax], dl             
  0x00CA7BE5  0000                    add      byte ptr [eax], al             
  0x00CA7BE7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA7BEA  0000                    add      byte ptr [eax], al             
  0x00CA7BEC  00f0                    add      al, dh                         
  0x00CA7BEE  56                      push     esi                            
  0x00CA7BEF  00960b000003            add      byte ptr [esi + 0x300000b], dl 
  0x00CA7BF5  0020                    add      byte ptr [eax], ah             
  0x00CA7BF7  0007                    add      byte ptr [edi], al             
  0x00CA7BF9  f4                      hlt                                     
  0x00CA7BFA  050013f044              add      eax, 0x44f01300                
  0x00CA7BFF  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA7C02  0000                    add      byte ptr [eax], al             
  0x00CA7C04  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA7C07  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA7C0A  0000                    add      byte ptr [eax], al             
  0x00CA7C0C  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA7C0F  009a0b000000            add      byte ptr [edx + 0xb], bl       
  0x00CA7C15  f4                      hlt                                     
  0x00CA7C16  60                      pushal                                  
  0x00CA7C17  0011                    add      byte ptr [ecx], dl             
  0x00CA7C19  0000                    add      byte ptr [eax], al             
  0x00CA7C1B  0000                    add      byte ptr [eax], al             
  0x00CA7C1D  f4                      hlt                                     
  0x00CA7C1E  44                      inc      esp                            
  0x00CA7C1F  000d00000000            add      byte ptr [0], cl               
  0x00CA7C25  58                      pop      eax                            
  0x00CA7C26  44                      inc      esp                            
                                        ; XREF: 0x00CA7BE1 (cond_jump)
  0x00CA7C27  0000                    add      byte ptr [eax], al             
  0x00CA7C2A  56                      push     esi                            
  0x00CA7C2B  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA7C2E  0000                    add      byte ptr [eax], al             
  0x00CA7C30  00f0                    add      al, dh                         
  0x00CA7C32  44                      inc      esp                            
  0x00CA7C33  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA7C36  0000                    add      byte ptr [eax], al             
  0x00CA7C38  44                      inc      esp                            
  0x00CA7C39  0020                    add      byte ptr [eax], ah             
  0x00CA7C3B  0000                    add      byte ptr [eax], al             
  0x00CA7C3D  58                      pop      eax                            
  0x00CA7C3E  54                      push     esp                            
  0x00CA7C3F  0000                    add      byte ptr [eax], al             
  0x00CA7C42  44                      inc      esp                            
  0x00CA7C43  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA7C46  0000                    add      byte ptr [eax], al             
  0x00CA7C48  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA7C4B  0000                    add      byte ptr [eax], al             
  0x00CA7C4D  002400                  add      byte ptr [eax + eax], ah       
  0x00CA7C50  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA7C53  0000                    add      byte ptr [eax], al             
  0x00CA7C55  58                      pop      eax                            
  0x00CA7C56  44                      inc      esp                            
  0x00CA7C57  0000                    add      byte ptr [eax], al             
  0x00CA7C59  58                      pop      eax                            
  0x00CA7C5A  44                      inc      esp                            
  0x00CA7C5B  0000                    add      byte ptr [eax], al             
  0x00CA7C5D  58                      pop      eax                            
  0x00CA7C5E  44                      inc      esp                            
  0x00CA7C5F  0013                    add      byte ptr [ebx], dl             
  0x00CA7C61  f4                      hlt                                     
  0x00CA7C62  44                      inc      esp                            
  0x00CA7C63  0009                    add      byte ptr [ecx], cl             
  0x00CA7C65  0000                    add      byte ptr [eax], al             
  0x00CA7C67  0000                    add      byte ptr [eax], al             
  0x00CA7C69  7044                    jo       0xca7caf                       
  0x00CA7C6B  001e                    add      byte ptr [esi], bl             
  0x00CA7C6D  0000                    add      byte ptr [eax], al             
  0x00CA7C6F  0000                    add      byte ptr [eax], al             
  0x00CA7C72  44                      inc      esp                            
  0x00CA7C73  001e                    add      byte ptr [esi], bl             
  0x00CA7C75  0000                    add      byte ptr [eax], al             
  0x00CA7C77  004019                  add      byte ptr [eax + 0x19], al      
  0x00CA7C7A  0c00                    or       al, 0                          
  0x00CA7C7C  184000                  sbb      byte ptr [eax], al             
  0x00CA7C7F  0000                    add      byte ptr [eax], al             
  0x00CA7C81  58                      pop      eax                            
  0x00CA7C82  54                      push     esp                            
  0x00CA7C83  0000                    add      byte ptr [eax], al             
  0x00CA7C85  002400                  add      byte ptr [eax + eax], ah       
  0x00CA7C88  005844                  add      byte ptr [eax + 0x44], bl      
  0x00CA7C8B  001b                    add      byte ptr [ebx], bl             
  0x00CA7C8D  0020                    add      byte ptr [eax], ah             
  0x00CA7C8F  0013                    add      byte ptr [ebx], dl             
  0x00CA7C91  0020                    add      byte ptr [eax], ah             
  0x00CA7C93  00df                    add      bh, bl                         
  0x00CA7C95  1e                      push     ds                             
  0x00CA7C96  0c00                    or       al, 0                          
  0x00CA7C98  00a4210040190c          add      byte ptr [ecx + 0xc194000], ah 
  0x00CA7C9F  0020                    add      byte ptr [eax], ah             
  0x00CA7CA1  800000                  add      byte ptr [eax], 0              
  0x00CA7CA4  1b00                    sbb      eax, dword ptr [eax]           
  0x00CA7CA6  2000                    and      byte ptr [eax], al             
  0x00CA7CA8  df1e                    fistp    word ptr [esi]                 
  0x00CA7CAA  0c00                    or       al, 0                          
  0x00CA7CAC  00a4210040190c          add      byte ptr [ecx + 0xc194000], ah 
  0x00CA7CB3  0018                    add      byte ptr [eax], bl             
  0x00CA7CB5  800000                  add      byte ptr [eax], 0              
  0x00CA7CB8  005854                  add      byte ptr [eax + 0x54], bl      
  0x00CA7CBB  0013                    add      byte ptr [ebx], dl             
  0x00CA7CBE  57                      push     edi                            
  0x00CA7CBF  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7CC2  0000                    add      byte ptr [eax], al             
  0x00CA7CC4  0bf4                    or       esi, esp                       
  0x00CA7CC6  45                      inc      ebp                            
  0x00CA7CC7  008000000007            add      byte ptr [eax + 0x7000000], al 
  0x00CA7CCD  2405                    and      al, 5                          
  0x00CA7CCF  001b                    add      byte ptr [ebx], bl             
  0x00CA7CD1  0020                    add      byte ptr [eax], ah             
  0x00CA7CD3  00a11c0c0068            add      byte ptr [ecx + 0x68000c1c], ah 
  0x00CA7CD9  0020                    add      byte ptr [eax], ah             
  0x00CA7CDB  0000                    add      byte ptr [eax], al             
  0x00CA7CDD  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7CDE  2100                    and      dword ptr [eax], eax           
  0x00CA7CE0  40                      inc      eax                            
  0x00CA7CE1  190c00                  sbb      dword ptr [eax + eax], ecx     
  0x00CA7CE4  208000001b00            and      byte ptr [eax + 0x1b0000], al  
  0x00CA7CEA  2000                    and      byte ptr [eax], al             
  0x00CA7CEC  a11c0c0068              mov      eax, dword ptr [0x68000c1c]    
  0x00CA7CF1  0020                    add      byte ptr [eax], ah             
  0x00CA7CF3  0000                    add      byte ptr [eax], al             
  0x00CA7CF5  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7CF6  2100                    and      dword ptr [eax], eax           
  0x00CA7CF8  40                      inc      eax                            
  0x00CA7CF9  190c00                  sbb      dword ptr [eax + eax], ecx     
  0x00CA7CFC  188000000058            sbb      byte ptr [eax + 0x58000000], al 
  0x00CA7D02  54                      push     esp                            
  0x00CA7D03  0013                    add      byte ptr [ebx], dl             
  0x00CA7D06  57                      push     edi                            
  0x00CA7D07  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7D0A  0000                    add      byte ptr [eax], al             
  0x00CA7D0C  0b00                    or       eax, dword ptr [eax]           
  0x00CA7D0E  2000                    and      byte ptr [eax], al             
  0x00CA7D10  07                      pop      es                             
  0x00CA7D11  2405                    and      al, 5                          
  0x00CA7D13  001b                    add      byte ptr [ebx], bl             
  0x00CA7D15  0020                    add      byte ptr [eax], ah             
  0x00CA7D17  00a11c0c0068            add      byte ptr [ecx + 0x68000c1c], ah 
  0x00CA7D1D  0020                    add      byte ptr [eax], ah             
  0x00CA7D1F  0000                    add      byte ptr [eax], al             
  0x00CA7D21  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7D22  2100                    and      dword ptr [eax], eax           
  0x00CA7D24  40                      inc      eax                            
  0x00CA7D25  190c00                  sbb      dword ptr [eax + eax], ecx     
  0x00CA7D28  20800000a11c            and      byte ptr [eax + 0x1ca10000], al 
  0x00CA7D2E  0c00                    or       al, 0                          
  0x00CA7D30  6800200000              push     0x2000                         
  0x00CA7D35  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7D36  2100                    and      dword ptr [eax], eax           
  0x00CA7D38  40                      inc      eax                            
  0x00CA7D39  190c00                  sbb      dword ptr [eax + eax], ecx     
  0x00CA7D3C  188000000058            sbb      byte ptr [eax + 0x58000000], al 
  0x00CA7D42  54                      push     esp                            
  0x00CA7D43  0013                    add      byte ptr [ebx], dl             
  0x00CA7D45  0020                    add      byte ptr [eax], ah             
  0x00CA7D47  0000                    add      byte ptr [eax], al             
  0x00CA7D49  58                      pop      eax                            
  0x00CA7D4A  54                      push     esp                            
  0x00CA7D4B  0000                    add      byte ptr [eax], al             
  0x00CA7D4D  f4                      hlt                                     
  0x00CA7D4E  60                      pushal                                  
  0x00CA7D4F  0011                    add      byte ptr [ecx], dl             
  0x00CA7D51  0000                    add      byte ptr [eax], al             
  0x00CA7D53  0000                    add      byte ptr [eax], al             
  0x00CA7D56  56                      push     esi                            
  0x00CA7D57  00960b000000            add      byte ptr [esi + 0xb], dl       
  0x00CA7D5D  f4                      hlt                                     
  0x00CA7D5E  57                      push     edi                            
  0x00CA7D5F  0008                    add      byte ptr [eax], cl             
  0x00CA7D61  06                      push     es                             
  0x00CA7D62  0000                    add      byte ptr [eax], al             
  0x00CA7D64  0c00                    or       al, 0                          
  0x00CA7D66  0000                    add      byte ptr [eax], al             
  0x00CA7D68  00f0                    add      al, dh                         
  0x00CA7D6A  6200                    bound    eax, qword ptr [eax]           
  0x00CA7D6C  45                      inc      ebp                            
  0x00CA7D6D  0b00                    or       eax, dword ptr [eax]           
  0x00CA7D6F  0022                    add      byte ptr [edx], ah             
  0x00CA7D72  0500470b00              add      eax, 0xb4700                   
  0x00CA7D77  0000                    add      byte ptr [eax], al             
  0x00CA7D79  f4                      hlt                                     
  0x00CA7D7A  57                      push     edi                            
  0x00CA7D7B  0010                    add      byte ptr [eax], dl             
  0x00CA7D7D  0000                    add      byte ptr [eax], al             
  0x00CA7D7F  0000                    add      byte ptr [eax], al             
  0x00CA7D81  00250000f444            add      byte ptr [0x44f40000], ah      
  0x00CA7D87  00770b                  add      byte ptr [edi + 0xb], dh       
  0x00CA7D8A  0000                    add      byte ptr [eax], al             
  0x00CA7D8C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA7D8F  0000                    add      byte ptr [eax], al             
  0x00CA7D91  0000                    add      byte ptr [eax], al             
  0x00CA7D93  0000                    add      byte ptr [eax], al             
  0x00CA7D95  f4                      hlt                                     
  0x00CA7D96  44                      inc      esp                            
  0x00CA7D97  0000                    add      byte ptr [eax], al             
  0x00CA7D99  0000                    add      byte ptr [eax], al             
  0x00CA7D9B  0000                    add      byte ptr [eax], al             
  0x00CA7D9D  7044                    jo       0xca7de3                       
  0x00CA7D9F  00540b00                add      byte ptr [ebx + ecx], dl       
  0x00CA7DA3  0000                    add      byte ptr [eax], al             
  0x00CA7DA5  f4                      hlt                                     
  0x00CA7DA6  61                      popal                                   
  0x00CA7DA7  0000                    add      byte ptr [eax], al             
  0x00CA7DA9  0000                    add      byte ptr [eax], al             
  0x00CA7DAB  0000                    add      byte ptr [eax], al             
  0x00CA7DAD  1038                    adc      byte ptr [eax], bh             
  0x00CA7DAF  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7DB5  f4                      hlt                                     
  0x00CA7DB6  61                      popal                                   
  0x00CA7DB7  00540b00                add      byte ptr [ebx + ecx], dl       
  0x00CA7DBB  0000                    add      byte ptr [eax], al             
  0x00CA7DBD  1038                    adc      byte ptr [eax], bh             
  0x00CA7DBF  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7DC5  f4                      hlt                                     
  0x00CA7DC6  61                      popal                                   
  0x00CA7DC7  007b0b                  add      byte ptr [ebx + 0xb], bh       
  0x00CA7DCA  0000                    add      byte ptr [eax], al             
  0x00CA7DCC  0002                    add      byte ptr [edx], al             
  0x00CA7DCE  3800                    cmp      byte ptr [eax], al             
  0x00CA7DD0  93                      xchg     ebx, eax                       
  0x00CA7DD1  030d0000f461            add      ecx, dword ptr [0x61f40000]    
  0x00CA7DD7  007c0b00                add      byte ptr [ebx + ecx], bh       
  0x00CA7DDB  0000                    add      byte ptr [eax], al             
  0x00CA7DDD  06                      push     es                             
  0x00CA7DDE  3800                    cmp      byte ptr [eax], al             
  0x00CA7DE0  93                      xchg     ebx, eax                       
  0x00CA7DE1  030d00000428            add      ecx, dword ptr [0x28040000]    
  0x00CA7DE7  0000                    add      byte ptr [eax], al             
  0x00CA7DE9  7050                    jo       0xca7e3b                       
  0x00CA7DEB  001e                    add      byte ptr [esi], bl             
  0x00CA7DED  0000                    add      byte ptr [eax], al             
  0x00CA7DEF  0000                    add      byte ptr [eax], al             
  0x00CA7DF1  f4                      hlt                                     
  0x00CA7DF2  61                      popal                                   
  0x00CA7DF3  001e                    add      byte ptr [esi], bl             
  0x00CA7DF5  0000                    add      byte ptr [eax], al             
  0x00CA7DF7  0000                    add      byte ptr [eax], al             
  0x00CA7DF9  0538009303              add      eax, 0x3930038                 
  0x00CA7DFE  0d00000028              or       eax, 0x28000000                
  0x00CA7E03  0000                    add      byte ptr [eax], al             
  0x00CA7E05  7050                    jo       0xca7e57                       
  0x00CA7E07  001e                    add      byte ptr [esi], bl             
  0x00CA7E09  0000                    add      byte ptr [eax], al             
  0x00CA7E0B  0000                    add      byte ptr [eax], al             
  0x00CA7E0D  f4                      hlt                                     
  0x00CA7E0E  61                      popal                                   
  0x00CA7E0F  001e                    add      byte ptr [esi], bl             
  0x00CA7E11  0000                    add      byte ptr [eax], al             
  0x00CA7E13  0000                    add      byte ptr [eax], al             
  0x00CA7E15  0338                    add      edi, dword ptr [eax]           
  0x00CA7E17  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7E1D  f4                      hlt                                     
  0x00CA7E1E  61                      popal                                   
  0x00CA7E1F  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7E22  0000                    add      byte ptr [eax], al             
  0x00CA7E24  0003                    add      byte ptr [ebx], al             
  0x00CA7E26  3800                    cmp      byte ptr [eax], al             
  0x00CA7E28  93                      xchg     ebx, eax                       
  0x00CA7E29  030d0000f056            add      ecx, dword ptr [0x56f00000]    
  0x00CA7E2F  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7E32  0000                    add      byte ptr [eax], al             
  0x00CA7E34  854101                  test     dword ptr [ecx + 1], eax       
  0x00CA7E37  000a                    add      byte ptr [edx], cl             
  0x00CA7E39  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7E3A  0500864101              add      eax, 0x1418600                 
  0x00CA7E3F  0008                    add      byte ptr [eax], cl             
  0x00CA7E41  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA7E42  0500000028              add      eax, 0x28000000                
  0x00CA7E47  0000                    add      byte ptr [eax], al             
  0x00CA7E49  7050                    jo       0xca7e9b                       
  0x00CA7E4B  001e                    add      byte ptr [esi], bl             
  0x00CA7E4D  0000                    add      byte ptr [eax], al             
  0x00CA7E4F  0000                    add      byte ptr [eax], al             
  0x00CA7E51  f4                      hlt                                     
  0x00CA7E52  61                      popal                                   
  0x00CA7E53  001e                    add      byte ptr [esi], bl             
  0x00CA7E55  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA7E05 (cond_jump)
  0x00CA7E57  0000                    add      byte ptr [eax], al             
  0x00CA7E59  0238                    add      bh, byte ptr [eax]             
  0x00CA7E5B  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7E62  56                      push     esi                            
  0x00CA7E63  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7E66  0000                    add      byte ptr [eax], al             
  0x00CA7E68  86440100                xchg     byte ptr [ecx + eax], al       
  0x00CA7E6C  08a40500000028          or       byte ptr [ebp + eax + 0x28000000], ah 
  0x00CA7E73  0000                    add      byte ptr [eax], al             
  0x00CA7E75  7050                    jo       0xca7ec7                       
  0x00CA7E77  001e                    add      byte ptr [esi], bl             
  0x00CA7E79  0000                    add      byte ptr [eax], al             
  0x00CA7E7B  0000                    add      byte ptr [eax], al             
  0x00CA7E7D  f4                      hlt                                     
  0x00CA7E7E  61                      popal                                   
  0x00CA7E7F  001e                    add      byte ptr [esi], bl             
  0x00CA7E81  0000                    add      byte ptr [eax], al             
  0x00CA7E83  0000                    add      byte ptr [eax], al             
  0x00CA7E85  0238                    add      bh, byte ptr [eax]             
  0x00CA7E87  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7E8E  56                      push     esi                            
  0x00CA7E8F  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA7E92  0000                    add      byte ptr [eax], al             
  0x00CA7E94  854201                  test     dword ptr [edx + 1], eax       
  0x00CA7E97  000524050000            add      byte ptr [0x524], al           
  0x00CA7E9D  f4                      hlt                                     
  0x00CA7E9E  61                      popal                                   
  0x00CA7E9F  004e0b                  add      byte ptr [esi + 0xb], cl       
  0x00CA7EA2  0000                    add      byte ptr [eax], al             
  0x00CA7EA4  0002                    add      byte ptr [edx], al             
  0x00CA7EA6  3800                    cmp      byte ptr [eax], al             
  0x00CA7EA8  93                      xchg     ebx, eax                       
  0x00CA7EA9  030d0000f461            add      ecx, dword ptr [0x61f40000]    
  0x00CA7EAF  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA7EB2  0000                    add      byte ptr [eax], al             
  0x00CA7EB4  0001                    add      byte ptr [ecx], al             
  0x00CA7EB6  3800                    cmp      byte ptr [eax], al             
  0x00CA7EB8  93                      xchg     ebx, eax                       
  0x00CA7EB9  030d0000f461            add      ecx, dword ptr [0x61f40000]    
  0x00CA7EBF  00490b                  add      byte ptr [ecx + 0xb], cl       
  0x00CA7EC2  0000                    add      byte ptr [eax], al             
  0x00CA7EC4  000538009303            add      byte ptr [0x3930038], al       
  0x00CA7ECA  0d0000f461              or       eax, 0x61f40000                
  0x00CA7ECF  004c0b00                add      byte ptr [ebx + ecx], cl       
  0x00CA7ED3  0000                    add      byte ptr [eax], al             
  0x00CA7ED5  0138                    add      dword ptr [eax], edi           
  0x00CA7ED7  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7EDE  56                      push     esi                            
  0x00CA7EDF  004c0b00                add      byte ptr [ebx + ecx], cl       
  0x00CA7EE3  0003                    add      byte ptr [ebx], al             
  0x00CA7EE5  0020                    add      byte ptr [eax], ah             
  0x00CA7EE7  0005a4050000            add      byte ptr [0x5a4], al           
  0x00CA7EED  f4                      hlt                                     
  0x00CA7EEE  61                      popal                                   
  0x00CA7EEF  004d0b                  add      byte ptr [ebp + 0xb], cl       
  0x00CA7EF2  0000                    add      byte ptr [eax], al             
  0x00CA7EF4  0008                    add      byte ptr [eax], cl             
  0x00CA7EF6  3800                    cmp      byte ptr [eax], al             
  0x00CA7EF8  93                      xchg     ebx, eax                       
  0x00CA7EF9  030d00000028            add      ecx, dword ptr [0x28000000]    
  0x00CA7EFF  0000                    add      byte ptr [eax], al             
  0x00CA7F01  7050                    jo       0xca7f53                       
  0x00CA7F03  001e                    add      byte ptr [esi], bl             
  0x00CA7F05  0000                    add      byte ptr [eax], al             
  0x00CA7F07  0000                    add      byte ptr [eax], al             
  0x00CA7F09  f4                      hlt                                     
  0x00CA7F0A  61                      popal                                   
  0x00CA7F0B  001e                    add      byte ptr [esi], bl             
  0x00CA7F0D  0000                    add      byte ptr [eax], al             
  0x00CA7F0F  0000                    add      byte ptr [eax], al             
  0x00CA7F11  0138                    add      dword ptr [eax], edi           
  0x00CA7F13  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7F19  0028                    add      byte ptr [eax], ch             
  0x00CA7F1B  0000                    add      byte ptr [eax], al             
  0x00CA7F1D  7050                    jo       0xca7f6f                       
  0x00CA7F1F  001e                    add      byte ptr [esi], bl             
  0x00CA7F21  0000                    add      byte ptr [eax], al             
  0x00CA7F23  0000                    add      byte ptr [eax], al             
  0x00CA7F25  f4                      hlt                                     
  0x00CA7F26  61                      popal                                   
  0x00CA7F27  001e                    add      byte ptr [esi], bl             
  0x00CA7F29  0000                    add      byte ptr [eax], al             
  0x00CA7F2B  0000                    add      byte ptr [eax], al             
  0x00CA7F2D  0138                    add      dword ptr [eax], edi           
  0x00CA7F2F  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7F35  0028                    add      byte ptr [eax], ch             
  0x00CA7F37  0000                    add      byte ptr [eax], al             
  0x00CA7F39  7050                    jo       0xca7f8b                       
  0x00CA7F3B  001e                    add      byte ptr [esi], bl             
  0x00CA7F3D  0000                    add      byte ptr [eax], al             
  0x00CA7F3F  0000                    add      byte ptr [eax], al             
  0x00CA7F41  f4                      hlt                                     
  0x00CA7F42  61                      popal                                   
  0x00CA7F43  001e                    add      byte ptr [esi], bl             
  0x00CA7F45  0000                    add      byte ptr [eax], al             
  0x00CA7F47  0000                    add      byte ptr [eax], al             
  0x00CA7F49  0138                    add      dword ptr [eax], edi           
  0x00CA7F4B  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7F51  0128                    add      dword ptr [eax], ebp           
                                        ; XREF: 0x00CA7F01 (cond_jump)
  0x00CA7F53  0000                    add      byte ptr [eax], al             
  0x00CA7F55  7050                    jo       0xca7fa7                       
  0x00CA7F57  001e                    add      byte ptr [esi], bl             
  0x00CA7F59  0000                    add      byte ptr [eax], al             
  0x00CA7F5B  0000                    add      byte ptr [eax], al             
  0x00CA7F5D  f4                      hlt                                     
  0x00CA7F5E  61                      popal                                   
  0x00CA7F5F  001e                    add      byte ptr [esi], bl             
  0x00CA7F61  0000                    add      byte ptr [eax], al             
  0x00CA7F63  0000                    add      byte ptr [eax], al             
  0x00CA7F65  0138                    add      dword ptr [eax], edi           
  0x00CA7F67  0093030d0013            add      byte ptr [ebx + 0x13000d03], dl 
  0x00CA7F6D  0020                    add      byte ptr [eax], ah             
                                        ; XREF: 0x00CA7F1D (cond_jump)
  0x00CA7F6F  0000                    add      byte ptr [eax], al             
  0x00CA7F71  7056                    jo       0xca7fc9                       
  0x00CA7F73  001e                    add      byte ptr [esi], bl             
  0x00CA7F75  0000                    add      byte ptr [eax], al             
  0x00CA7F77  0000                    add      byte ptr [eax], al             
  0x00CA7F79  f4                      hlt                                     
  0x00CA7F7A  61                      popal                                   
  0x00CA7F7B  001e                    add      byte ptr [esi], bl             
  0x00CA7F7D  0000                    add      byte ptr [eax], al             
  0x00CA7F7F  0000                    add      byte ptr [eax], al             
  0x00CA7F81  0138                    add      dword ptr [eax], edi           
  0x00CA7F83  0093030d0013            add      byte ptr [ebx + 0x13000d03], dl 
  0x00CA7F89  0020                    add      byte ptr [eax], ah             
                                        ; XREF: 0x00CA7F39 (cond_jump)
  0x00CA7F8B  0000                    add      byte ptr [eax], al             
  0x00CA7F8D  7056                    jo       0xca7fe5                       
  0x00CA7F8F  001e                    add      byte ptr [esi], bl             
  0x00CA7F91  0000                    add      byte ptr [eax], al             
  0x00CA7F93  0000                    add      byte ptr [eax], al             
  0x00CA7F95  f4                      hlt                                     
  0x00CA7F96  61                      popal                                   
  0x00CA7F97  001e                    add      byte ptr [esi], bl             
  0x00CA7F99  0000                    add      byte ptr [eax], al             
  0x00CA7F9B  0000                    add      byte ptr [eax], al             
  0x00CA7F9D  0138                    add      dword ptr [eax], edi           
  0x00CA7F9F  0093030d0013            add      byte ptr [ebx + 0x13000d03], dl 
  0x00CA7FA5  0020                    add      byte ptr [eax], ah             
                                        ; XREF: 0x00CA7F55 (cond_jump)
  0x00CA7FA7  0000                    add      byte ptr [eax], al             
  0x00CA7FA9  7050                    jo       0xca7ffb                       
  0x00CA7FAB  001e                    add      byte ptr [esi], bl             
  0x00CA7FAD  0000                    add      byte ptr [eax], al             
  0x00CA7FAF  0000                    add      byte ptr [eax], al             
  0x00CA7FB1  f4                      hlt                                     
  0x00CA7FB2  61                      popal                                   
  0x00CA7FB3  001e                    add      byte ptr [esi], bl             
  0x00CA7FB5  0000                    add      byte ptr [eax], al             
  0x00CA7FB7  0000                    add      byte ptr [eax], al             
  0x00CA7FB9  0138                    add      dword ptr [eax], edi           
  0x00CA7FBB  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA7FC1  7045                    jo       0xca8008                       
  0x00CA7FC3  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA7FC6  0000                    add      byte ptr [eax], al             
  0x00CA7FC8  007057                  add      byte ptr [eax + 0x57], dh      
  0x00CA7FCB  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA7FCE  0000                    add      byte ptr [eax], al             
  0x00CA7FD0  007062                  add      byte ptr [eax + 0x62], dh      
  0x00CA7FD3  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA7FD6  0000                    add      byte ptr [eax], al             
  0x00CA7FD8  22f4                    and      dh, ah                         
  0x00CA7FDA  0500ffff00              add      eax, 0xffff00                  
  0x00CA7FDF  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA7FE2  0000                    add      byte ptr [eax], al             
  0x00CA7FE4  1300                    adc      eax, dword ptr [eax]           
  0x00CA7FE6  2000                    and      byte ptr [eax], al             
  0x00CA7FE8  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA7FEB  0002                    add      byte ptr [edx], al             
  0x00CA7FED  0000                    add      byte ptr [eax], al             
  0x00CA7FEF  0000                    add      byte ptr [eax], al             
  0x00CA7FF1  7056                    jo       0xca8049                       
  0x00CA7FF3  0003                    add      byte ptr [ebx], al             
  0x00CA7FF5  0000                    add      byte ptr [eax], al             
  0x00CA7FF7  0000                    add      byte ptr [eax], al             
  0x00CA7FF9  7056                    jo       0xca8051                       
                                        ; XREF: 0x00CA7FA9 (cond_jump)
  0x00CA7FFB  000400                  add      byte ptr [eax + eax], al       
  0x00CA7FFE  0000                    add      byte ptr [eax], al             
  0x00CA8000  0000                    add      byte ptr [eax], al             
  0x00CA8002  360000                  add      byte ptr ss:[eax], al          
  0x00CA8006  44                      inc      esp                            
  0x00CA8007  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA800D  c406                    les      eax, ptr [esi]                 
  0x00CA800F  001d00000000            add      byte ptr [0], bl               
  0x00CA8015  f4                      hlt                                     
  0x00CA8016  56                      push     esi                            
  0x00CA8017  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA801D  c422                    les      esp, ptr [edx]                 
  0x00CA801F  004000                  add      byte ptr [eax], al             
  0x00CA8022  2000                    and      byte ptr [eax], al             
  0x00CA8024  0090210000e0            add      byte ptr [eax - 0x1fffffdf], dl 
  0x00CA802A  7000                    jo       0xca802c                       
                                        ; XREF: 0x00CA802A (cond_jump)
  0x00CA802C  00c4                    add      ah, al                         
  0x00CA802E  2200                    and      al, byte ptr [eax]             
  0x00CA8030  00f4                    add      ah, dh                         
  0x00CA8032  46                      inc      esi                            
  0x00CA8033  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA8039  f4                      hlt                                     
  0x00CA803A  44                      inc      esp                            
  0x00CA803B  0000                    add      byte ptr [eax], al             
  0x00CA803D  0100                    add      dword ptr [eax], eax           
  0x00CA803F  002e                    add      byte ptr [esi], ch             
  0x00CA8041  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA8046  2000                    and      byte ptr [eax], al             
  0x00CA8048  0090210000c4            add      byte ptr [eax - 0x3bffffdf], dl 
  0x00CA804E  2200                    and      al, byte ptr [eax]             
  0x00CA8050  00f4                    add      ah, dh                         
  0x00CA8052  46                      inc      esi                            
  0x00CA8053  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA805A  44                      inc      esp                            
  0x00CA805B  00720b                  add      byte ptr [edx + 0xb], dh       
  0x00CA805E  0000                    add      byte ptr [eax], al             
  0x00CA8060  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA8066  2000                    and      byte ptr [eax], al             
  0x00CA8068  009521000070            add      byte ptr [ebp + 0x70000021], dl 
  0x00CA806E  6600410b                add      byte ptr [ecx + 0xb], al       
  0x00CA8072  0000                    add      byte ptr [eax], al             
  0x00CA8074  f1                      int1                                    
  0x00CA8075  030d0000f066            add      ecx, dword ptr [0x66f00000]    
  0x00CA807B  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA807E  0000                    add      byte ptr [eax], al             
  0x00CA8080  005e20                  add      byte ptr [esi + 0x20], bl      
  0x00CA8083  0000                    add      byte ptr [eax], al             
  0x00CA8086  56                      push     esi                            
  0x00CA8087  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA808A  0000                    add      byte ptr [eax], al             
  0x00CA808C  0300                    add      eax, dword ptr [eax]           
  0x00CA808E  2000                    and      byte ptr [eax], al             
  0x00CA8090  07                      pop      es                             
  0x00CA8091  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA8092  0500000738              add      eax, 0x38070000                
  0x00CA8097  0000                    add      byte ptr [eax], al             
  0x00CA8099  f4                      hlt                                     
  0x00CA809A  60                      pushal                                  
  0x00CA809B  008904000000            add      byte ptr [ecx + 4], cl         
  0x00CA80A1  f4                      hlt                                     
  0x00CA80A2  650039                  add      byte ptr gs:[ecx], bh          
  0x00CA80A5  0b00                    or       eax, dword ptr [eax]           
  0x00CA80A7  00f1                    add      cl, dh                         
  0x00CA80A9  030d0000f057            add      ecx, dword ptr [0x57f00000]    
  0x00CA80AF  0002                    add      byte ptr [edx], al             
  0x00CA80B1  0000                    add      byte ptr [eax], al             
  0x00CA80B3  000b                    add      byte ptr [ebx], cl             
  0x00CA80B5  0020                    add      byte ptr [eax], ah             
  0x00CA80B7  0014a4                  add      byte ptr [esp], dl             
  0x00CA80BA  050000f462              add      eax, 0x62f40000                
  0x00CA80BF  000500000000            add      byte ptr [0], al               
  0x00CA80C5  f4                      hlt                                     
  0x00CA80C6  66000a                  add      byte ptr [edx], cl             
  0x00CA80C9  0d0000004e              or       eax, 0x4e000000                
  0x00CA80CE  2200                    and      al, byte ptr [eax]             
  0x00CA80D0  10f4                    adc      ah, dh                         
  0x00CA80D2  44                      inc      esp                            
  0x00CA80D3  0001                    add      byte ptr [ecx], al             
  0x00CA80D5  0000                    add      byte ptr [eax], al             
  0x00CA80D7  008c4301003ed0          add      byte ptr [ebx + eax*2 - 0x2fc1ffff], cl 
  0x00CA80DE  2100                    and      dword ptr [eax], eax           
  0x00CA80E0  10cd                    adc      ch, cl                         
  0x00CA80E2  06                      push     es                             
  0x00CA80E3  0002                    add      byte ptr [edx], al             
  0x00CA80E5  0000                    add      byte ptr [eax], al             
  0x00CA80E7  0000                    add      byte ptr [eax], al             
  0x00CA80E9  58                      pop      eax                            
  0x00CA80EA  44                      inc      esp                            
  0x00CA80EB  0000                    add      byte ptr [eax], al             
  0x00CA80EF  00d0                    add      al, dl                         
  0x00CA80F3  00d2                    add      dl, dl                         
  0x00CA80F7  00d2                    add      dl, dl                         
  0x00CA80F9  f066000d00000024        lock add byte ptr [0x24000000], cl      
  0x00CA8101  1d0c000066              sbb      eax, 0x6600000c                
  0x00CA8106  50                      push     eax                            
  0x00CA8107  0000                    add      byte ptr [eax], al             
  0x00CA810A  57                      push     edi                            
  0x00CA810B  0003                    add      byte ptr [ebx], al             
  0x00CA810D  0000                    add      byte ptr [eax], al             
  0x00CA810F  000b                    add      byte ptr [ebx], cl             
  0x00CA8111  0020                    add      byte ptr [eax], ah             
  0x00CA8113  0014a4                  add      byte ptr [esp], dl             
  0x00CA8116  050000f462              add      eax, 0x62f40000                
  0x00CA811B  0008                    add      byte ptr [eax], cl             
  0x00CA811D  0000                    add      byte ptr [eax], al             
  0x00CA811F  0000                    add      byte ptr [eax], al             
  0x00CA8121  f4                      hlt                                     
  0x00CA8122  66000d0d000000          add      byte ptr [0xd], cl             
  0x00CA8129  4e                      dec      esi                            
  0x00CA812A  2200                    and      al, byte ptr [eax]             
  0x00CA812C  10f4                    adc      ah, dh                         
  0x00CA812E  44                      inc      esp                            
  0x00CA812F  0002                    add      byte ptr [edx], al             
  0x00CA8131  0000                    add      byte ptr [eax], al             
  0x00CA8133  008c4301003ed0          add      byte ptr [ebx + eax*2 - 0x2fc1ffff], cl 
  0x00CA813A  2100                    and      dword ptr [eax], eax           
  0x00CA813C  10cd                    adc      ch, cl                         
  0x00CA813E  06                      push     es                             
  0x00CA813F  0002                    add      byte ptr [edx], al             
  0x00CA8141  0000                    add      byte ptr [eax], al             
  0x00CA8143  0000                    add      byte ptr [eax], al             
  0x00CA8145  58                      pop      eax                            
  0x00CA8146  44                      inc      esp                            
  0x00CA8147  0000                    add      byte ptr [eax], al             
  0x00CA814B  00d0                    add      al, dl                         
  0x00CA814F  00d2                    add      dl, dl                         
  0x00CA8153  00d2                    add      dl, dl                         
  0x00CA8155  f066000e                lock add byte ptr [esi], cl             
  0x00CA8159  0000                    add      byte ptr [eax], al             
  0x00CA815B  0020                    add      byte ptr [eax], ah             
  0x00CA815D  1d0c000066              sbb      eax, 0x6600000c                
  0x00CA8162  50                      push     eax                            
  0x00CA8163  0000                    add      byte ptr [eax], al             
  0x00CA8166  57                      push     edi                            
  0x00CA8167  000400                  add      byte ptr [eax + eax], al       
  0x00CA816A  0000                    add      byte ptr [eax], al             
  0x00CA816C  0b00                    or       eax, dword ptr [eax]           
  0x00CA816E  2000                    and      byte ptr [eax], al             
  0x00CA8170  13a4050000f462          adc      esp, dword ptr [ebp + eax + 0x62f40000] 
  0x00CA8177  000b                    add      byte ptr [ebx], cl             
  0x00CA8179  0000                    add      byte ptr [eax], al             
  0x00CA817B  0000                    add      byte ptr [eax], al             
  0x00CA817D  f4                      hlt                                     
  0x00CA817E  660010                  add      byte ptr [eax], dl             
  0x00CA8181  0d0000004e              or       eax, 0x4e000000                
  0x00CA8186  2200                    and      al, byte ptr [eax]             
  0x00CA8188  10f4                    adc      ah, dh                         
  0x00CA818A  44                      inc      esp                            
  0x00CA818B  00050000008c            add      byte ptr [0x8c000000], al      
  0x00CA8191  42                      inc      edx                            
  0x00CA8192  0100                    add      dword ptr [eax], eax           
  0x00CA8194  3ed021                  shl      byte ptr ds:[ecx], 1           
  0x00CA8197  0010                    add      byte ptr [eax], dl             
  0x00CA8199  cd06                    int      6                              
  0x00CA819B  0002                    add      byte ptr [edx], al             
  0x00CA819D  0000                    add      byte ptr [eax], al             
  0x00CA819F  0000                    add      byte ptr [eax], al             
  0x00CA81A1  58                      pop      eax                            
  0x00CA81A2  44                      inc      esp                            
  0x00CA81A3  0000                    add      byte ptr [eax], al             
  0x00CA81A7  00d0                    add      al, dl                         
  0x00CA81AB  00d2                    add      dl, dl                         
  0x00CA81AD  f066000f                lock add byte ptr [edi], cl             
  0x00CA81B1  0000                    add      byte ptr [eax], al             
  0x00CA81B3  0020                    add      byte ptr [eax], ah             
  0x00CA81B5  1d0c000066              sbb      eax, 0x6600000c                
  0x00CA81BA  50                      push     eax                            
  0x00CA81BB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA81BE  0000                    add      byte ptr [eax], al             
  0x00CA81C0  00f0                    add      al, dh                         
  0x00CA81C2  6200                    bound    eax, qword ptr [eax]           
  0x00CA81C4  55                      push     ebp                            
  0x00CA81C5  0b00                    or       eax, dword ptr [eax]           
  0x00CA81C7  0022                    add      byte ptr [edx], ah             
  0x00CA81CA  0500470b00              add      eax, 0xb4700                   
  0x00CA81CF  0000                    add      byte ptr [eax], al             
  0x00CA81D2  57                      push     edi                            
  0x00CA81D3  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA81D6  0000                    add      byte ptr [eax], al             
  0x00CA81D8  00f0                    add      al, dh                         
  0x00CA81DA  45                      inc      ebp                            
  0x00CA81DB  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA81DE  0000                    add      byte ptr [eax], al             
  0x00CA81E0  00f4                    add      ah, dh                         
  0x00CA81E2  61                      popal                                   
  0x00CA81E3  007f0b                  add      byte ptr [edi + 0xb], bh       
  0x00CA81E6  0000                    add      byte ptr [eax], al             
  0x00CA81E8  00f0                    add      al, dh                         
  0x00CA81EA  7100                    jno      0xca81ec                       
                                        ; XREF: 0x00CA81EA (cond_jump)
  0x00CA81EC  97                      xchg     edi, eax                       
  0x00CA81ED  0b00                    or       eax, dword ptr [eax]           
  0x00CA81EF  0000                    add      byte ptr [eax], al             
  0x00CA81F1  0138                    add      dword ptr [eax], edi           
  0x00CA81F3  006f03                  add      byte ptr [edi + 3], ch         
  0x00CA81F6  0d0000f461              or       eax, 0x61f40000                
  0x00CA81FB  00840b000000f0          add      byte ptr [ebx + ecx - 0x10000000], al 
  0x00CA8202  7100                    jno      0xca8204                       
                                        ; XREF: 0x00CA8202 (cond_jump)
  0x00CA8204  97                      xchg     edi, eax                       
  0x00CA8205  0b00                    or       eax, dword ptr [eax]           
  0x00CA8207  0000                    add      byte ptr [eax], al             
  0x00CA8209  0138                    add      dword ptr [eax], edi           
  0x00CA820B  006f03                  add      byte ptr [edi + 3], ch         
  0x00CA820E  0d0000f461              or       eax, 0x61f40000                
  0x00CA8213  004a0b                  add      byte ptr [edx + 0xb], cl       
  0x00CA8216  0000                    add      byte ptr [eax], al             
  0x00CA8218  0001                    add      byte ptr [ecx], al             
  0x00CA821A  3800                    cmp      byte ptr [eax], al             
  0x00CA821C  93                      xchg     ebx, eax                       
  0x00CA821D  030d0000f056            add      ecx, dword ptr [0x56f00000]    
  0x00CA8223  004a0b                  add      byte ptr [edx + 0xb], cl       
  0x00CA8226  0000                    add      byte ptr [eax], al             
  0x00CA8228  0300                    add      eax, dword ptr [eax]           
  0x00CA822A  2000                    and      byte ptr [eax], al             
  0x00CA822C  05a4050000              add      eax, 0x5a4                     
  0x00CA8231  f4                      hlt                                     
  0x00CA8232  61                      popal                                   
  0x00CA8233  004b0b                  add      byte ptr [ebx + 0xb], cl       
  0x00CA8236  0000                    add      byte ptr [eax], al             
  0x00CA8238  0008                    add      byte ptr [eax], cl             
  0x00CA823A  3800                    cmp      byte ptr [eax], al             
  0x00CA823C  93                      xchg     ebx, eax                       
  0x00CA823D  030d00130020            add      ecx, dword ptr [0x20001300]    
  0x00CA8243  0000                    add      byte ptr [eax], al             
  0x00CA8245  7056                    jo       0xca829d                       
  0x00CA8247  001e                    add      byte ptr [esi], bl             
  0x00CA8249  0000                    add      byte ptr [eax], al             
  0x00CA824B  0000                    add      byte ptr [eax], al             
  0x00CA824D  f4                      hlt                                     
  0x00CA824E  61                      popal                                   
  0x00CA824F  00890b000000            add      byte ptr [ecx + 0xb], cl       
  0x00CA8255  0138                    add      dword ptr [eax], edi           
  0x00CA8257  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA825D  f9                      stc                                     
  0x00CA825E  56                      push     esi                            
  0x00CA825F  0003                    add      byte ptr [ebx], al             
  0x00CA8261  0020                    add      byte ptr [eax], ah             
  0x00CA8263  0005a4050000            add      byte ptr [0x5a4], al           
  0x00CA8269  f4                      hlt                                     
  0x00CA826A  61                      popal                                   
  0x00CA826B  001e                    add      byte ptr [esi], bl             
  0x00CA826D  0000                    add      byte ptr [eax], al             
  0x00CA826F  0000                    add      byte ptr [eax], al             
  0x00CA8271  0138                    add      dword ptr [eax], edi           
  0x00CA8273  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA827A  56                      push     esi                            
  0x00CA827B  007d0b                  add      byte ptr [ebp + 0xb], bh       
  0x00CA827E  0000                    add      byte ptr [eax], al             
  0x00CA8280  c54001                  lds      eax, ptr [eax + 1]             
  0x00CA8283  0002                    add      byte ptr [edx], al             
  0x00CA8285  0000                    add      byte ptr [eax], al             
  0x00CA8287  0012                    add      byte ptr [edx], dl             
  0x00CA8289  2405                    and      al, 5                          
  0x00CA828B  0000                    add      byte ptr [eax], al             
  0x00CA828D  f4                      hlt                                     
  0x00CA828E  44                      inc      esp                            
  0x00CA828F  0010                    add      byte ptr [eax], dl             
  0x00CA8291  0000                    add      byte ptr [eax], al             
  0x00CA8293  0000                    add      byte ptr [eax], al             
  0x00CA8295  7044                    jo       0xca82db                       
  0x00CA8297  001e                    add      byte ptr [esi], bl             
  0x00CA8299  0000                    add      byte ptr [eax], al             
  0x00CA829B  0000                    add      byte ptr [eax], al             
  0x00CA829E  56                      push     esi                            
  0x00CA829F  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA82A2  0000                    add      byte ptr [eax], al             
  0x00CA82A4  0300                    add      eax, dword ptr [eax]           
  0x00CA82A6  2000                    and      byte ptr [eax], al             
  0x00CA82A8  06                      push     es                             
  0x00CA82A9  2405                    and      al, 5                          
  0x00CA82AB  0000                    add      byte ptr [eax], al             
  0x00CA82AD  f4                      hlt                                     
  0x00CA82AE  61                      popal                                   
  0x00CA82AF  001e                    add      byte ptr [esi], bl             
  0x00CA82B1  0000                    add      byte ptr [eax], al             
  0x00CA82B3  0000                    add      byte ptr [eax], al             
  0x00CA82B5  0538009303              add      eax, 0x3930038                 
  0x00CA82BA  0d00050c05              or       eax, 0x50c0500                 
  0x00CA82BF  0000                    add      byte ptr [eax], al             
  0x00CA82C1  f4                      hlt                                     
  0x00CA82C2  61                      popal                                   
  0x00CA82C3  001e                    add      byte ptr [esi], bl             
  0x00CA82C5  0000                    add      byte ptr [eax], al             
  0x00CA82C7  0000                    add      byte ptr [eax], al             
  0x00CA82C9  0138                    add      dword ptr [eax], edi           
  0x00CA82CB  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA82D1  f4                      hlt                                     
  0x00CA82D2  61                      popal                                   
  0x00CA82D3  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA82DA  7100                    jno      0xca82dc                       
                                        ; XREF: 0x00CA82DA (cond_jump)
  0x00CA82DC  97                      xchg     edi, eax                       
  0x00CA82DD  0b00                    or       eax, dword ptr [eax]           
  0x00CA82DF  0000                    add      byte ptr [eax], al             
  0x00CA82E1  0238                    add      bh, byte ptr [eax]             
  0x00CA82E3  006f03                  add      byte ptr [edi + 3], ch         
  0x00CA82E6  0d0000f056              or       eax, 0x56f00000                
  0x00CA82EB  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA82EE  0000                    add      byte ptr [eax], al             
  0x00CA82F0  0300                    add      eax, dword ptr [eax]           
  0x00CA82F2  2000                    and      byte ptr [eax], al             
  0x00CA82F4  05a4050000              add      eax, 0x5a4                     
  0x00CA82F9  f4                      hlt                                     
  0x00CA82FA  61                      popal                                   
  0x00CA82FB  008f0b000000            add      byte ptr [edi + 0xb], cl       
  0x00CA8301  0138                    add      dword ptr [eax], edi           
  0x00CA8303  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8309  0036                    add      byte ptr [esi], dh             
  0x00CA830B  0000                    add      byte ptr [eax], al             
  0x00CA830E  44                      inc      esp                            
  0x00CA830F  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA8315  c406                    les      eax, ptr [esi]                 
  0x00CA8317  0011                    add      byte ptr [ecx], dl             
  0x00CA8319  0000                    add      byte ptr [eax], al             
  0x00CA831B  0000                    add      byte ptr [eax], al             
  0x00CA831D  f4                      hlt                                     
  0x00CA831E  56                      push     esi                            
  0x00CA831F  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA8325  c422                    les      esp, ptr [edx]                 
  0x00CA8327  004000                  add      byte ptr [eax], al             
  0x00CA832A  2000                    and      byte ptr [eax], al             
  0x00CA832C  0091210000e1            add      byte ptr [ecx - 0x1effffdf], dl 
  0x00CA8332  56                      push     esi                            
  0x00CA8333  0003                    add      byte ptr [ebx], al             
  0x00CA8335  0020                    add      byte ptr [eax], ah             
  0x00CA8337  0008                    add      byte ptr [eax], cl             
  0x00CA8339  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA833A  050000f456              add      eax, 0x56f40000                
  0x00CA833F  00a00b000000            add      byte ptr [eax + 0xb], ah       
  0x00CA8345  c422                    les      esp, ptr [edx]                 
  0x00CA8347  004000                  add      byte ptr [eax], al             
  0x00CA834A  2000                    and      byte ptr [eax], al             
  0x00CA834C  009121000006            add      byte ptr [ecx + 0x6000021], dl 
  0x00CA8352  3800                    cmp      byte ptr [eax], al             
  0x00CA8354  93                      xchg     ebx, eax                       
  0x00CA8355  030d00005e20            add      ecx, dword ptr [0x205e0000]    
  0x00CA835B  0000                    add      byte ptr [eax], al             
  0x00CA835D  0036                    add      byte ptr [esi], dh             
  0x00CA835F  0000                    add      byte ptr [eax], al             
  0x00CA8362  44                      inc      esp                            
  0x00CA8363  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA8369  c406                    les      eax, ptr [esi]                 
  0x00CA836B  0023                    add      byte ptr [ebx], ah             
  0x00CA836D  0000                    add      byte ptr [eax], al             
  0x00CA836F  0000                    add      byte ptr [eax], al             
  0x00CA8371  f4                      hlt                                     
  0x00CA8372  56                      push     esi                            
  0x00CA8373  008a0b000000            add      byte ptr [edx + 0xb], cl       
  0x00CA8379  c422                    les      esp, ptr [edx]                 
  0x00CA837B  004000                  add      byte ptr [eax], al             
  0x00CA837E  2000                    and      byte ptr [eax], al             
  0x00CA8380  0091210000e1            add      byte ptr [ecx - 0x1effffdf], dl 
  0x00CA8386  56                      push     esi                            
  0x00CA8387  0003                    add      byte ptr [ebx], al             
  0x00CA8389  0020                    add      byte ptr [eax], ah             
  0x00CA838B  001a                    add      byte ptr [edx], bl             
  0x00CA838D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA838E  050000c422              add      eax, 0x22c40000                
  0x00CA8393  0000                    add      byte ptr [eax], al             
  0x00CA8395  f4                      hlt                                     
  0x00CA8396  46                      inc      esi                            
  0x00CA8397  001f                    add      byte ptr [edi], bl             
  0x00CA8399  0000                    add      byte ptr [eax], al             
  0x00CA839B  00d0                    add      al, dl                         
  0x00CA839D  f4                      hlt                                     
  0x00CA839E  44                      inc      esp                            
  0x00CA839F  0000                    add      byte ptr [eax], al             
  0x00CA83A1  0000                    add      byte ptr [eax], al             
  0x00CA83A3  002e                    add      byte ptr [esi], ch             
  0x00CA83A5  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA83AA  2000                    and      byte ptr [eax], al             
  0x00CA83AC  009121000004            add      byte ptr [ecx + 0x4000021], dl 
  0x00CA83B2  3800                    cmp      byte ptr [eax], al             
  0x00CA83B4  a3030d0000              mov      dword ptr [0xd03], eax         
  0x00CA83B9  f4                      hlt                                     
  0x00CA83BA  56                      push     esi                            
  0x00CA83BB  00910b000000            add      byte ptr [ecx + 0xb], dl       
  0x00CA83C1  c422                    les      esp, ptr [edx]                 
  0x00CA83C3  004000                  add      byte ptr [eax], al             
  0x00CA83C6  2000                    and      byte ptr [eax], al             
  0x00CA83C8  0090210000e0            add      byte ptr [eax - 0x1fffffdf], dl 
  0x00CA83CE  7100                    jno      0xca83d0                       
                                        ; XREF: 0x00CA83CE (cond_jump)
  0x00CA83D0  0007                    add      byte ptr [edi], al             
  0x00CA83D2  3800                    cmp      byte ptr [eax], al             
  0x00CA83D4  81030d0000f4            add      dword ptr [ebx], 0xf400000d    
  0x00CA83DA  56                      push     esi                            
  0x00CA83DB  00610b                  add      byte ptr [ecx + 0xb], ah       
  0x00CA83DE  0000                    add      byte ptr [eax], al             
  0x00CA83E0  00c4                    add      ah, al                         
  0x00CA83E2  2200                    and      al, byte ptr [eax]             
  0x00CA83E4  40                      inc      eax                            
  0x00CA83E5  0020                    add      byte ptr [eax], ah             
  0x00CA83E7  0000                    add      byte ptr [eax], al             
  0x00CA83E9  91                      xchg     ecx, eax                       
  0x00CA83EA  2100                    and      dword ptr [eax], eax           
  0x00CA83EC  0002                    add      byte ptr [edx], al             
  0x00CA83EE  3800                    cmp      byte ptr [eax], al             
  0x00CA83F0  93                      xchg     ebx, eax                       
  0x00CA83F1  030d00005e20            add      ecx, dword ptr [0x205e0000]    
  0x00CA83F7  0000                    add      byte ptr [eax], al             
  0x00CA83FA  56                      push     esi                            
  0x00CA83FB  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA83FE  0000                    add      byte ptr [eax], al             
  0x00CA8400  0300                    add      eax, dword ptr [eax]           
  0x00CA8402  2000                    and      byte ptr [eax], al             
  0x00CA8404  0ca4                    or       al, 0xa4                       
  0x00CA8406  050000f056              add      eax, 0x56f00000                
  0x00CA840B  008f0b000003            add      byte ptr [edi + 0x300000b], cl 
  0x00CA8411  0020                    add      byte ptr [eax], ah             
  0x00CA8413  0008                    add      byte ptr [eax], cl             
  0x00CA8415  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA8416  050000f461              add      eax, 0x61f40000                
  0x00CA841B  009b00000000            add      byte ptr [ebx], bl             
  0x00CA8421  0438                    add      al, 0x38                       
  0x00CA8423  00a3030d0000            add      byte ptr [ebx + 0xd03], ah     
  0x00CA8429  0239                    add      bh, byte ptr [ecx]             
  0x00CA842B  0000                    add      byte ptr [eax], al             
  0x00CA842D  07                      pop      es                             
  0x00CA842E  3800                    cmp      byte ptr [eax], al             
  0x00CA8430  81030d0000f4            add      dword ptr [ebx], 0xf400000d    
  0x00CA8436  61                      popal                                   
  0x00CA8437  00900b000000            add      byte ptr [eax + 0xb], dl       
  0x00CA843D  0138                    add      dword ptr [eax], edi           
  0x00CA843F  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8445  f9                      stc                                     
  0x00CA8446  56                      push     esi                            
  0x00CA8447  0003                    add      byte ptr [ebx], al             
  0x00CA8449  0020                    add      byte ptr [eax], ah             
  0x00CA844B  0049a4                  add      byte ptr [ecx - 0x5c], cl      
  0x00CA844E  050000f456              add      eax, 0x56f40000                
  0x00CA8453  0002                    add      byte ptr [edx], al             
  0x00CA8455  0000                    add      byte ptr [eax], al             
  0x00CA8457  0000                    add      byte ptr [eax], al             
  0x00CA8459  7056                    jo       0xca84b1                       
  0x00CA845B  001e                    add      byte ptr [esi], bl             
  0x00CA845D  0000                    add      byte ptr [eax], al             
  0x00CA845F  0000                    add      byte ptr [eax], al             
  0x00CA8461  f4                      hlt                                     
  0x00CA8462  61                      popal                                   
  0x00CA8463  001e                    add      byte ptr [esi], bl             
  0x00CA8465  0000                    add      byte ptr [eax], al             
  0x00CA8467  0000                    add      byte ptr [eax], al             
  0x00CA8469  0238                    add      bh, byte ptr [eax]             
  0x00CA846B  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8471  f4                      hlt                                     
  0x00CA8472  56                      push     esi                            
  0x00CA8473  0001                    add      byte ptr [ecx], al             
  0x00CA8475  0000                    add      byte ptr [eax], al             
  0x00CA8477  0000                    add      byte ptr [eax], al             
  0x00CA8479  7056                    jo       0xca84d1                       
  0x00CA847B  001e                    add      byte ptr [esi], bl             
  0x00CA847D  0000                    add      byte ptr [eax], al             
  0x00CA847F  0000                    add      byte ptr [eax], al             
  0x00CA8481  f4                      hlt                                     
  0x00CA8482  61                      popal                                   
  0x00CA8483  001e                    add      byte ptr [esi], bl             
  0x00CA8485  0000                    add      byte ptr [eax], al             
  0x00CA8487  0000                    add      byte ptr [eax], al             
  0x00CA8489  0238                    add      bh, byte ptr [eax]             
  0x00CA848B  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8491  f4                      hlt                                     
  0x00CA8492  56                      push     esi                            
  0x00CA8493  0001                    add      byte ptr [ecx], al             
  0x00CA8495  0000                    add      byte ptr [eax], al             
  0x00CA8497  0000                    add      byte ptr [eax], al             
  0x00CA8499  7056                    jo       0xca84f1                       
  0x00CA849B  001e                    add      byte ptr [esi], bl             
  0x00CA849D  0000                    add      byte ptr [eax], al             
  0x00CA849F  0000                    add      byte ptr [eax], al             
  0x00CA84A1  f4                      hlt                                     
  0x00CA84A2  61                      popal                                   
  0x00CA84A3  001e                    add      byte ptr [esi], bl             
  0x00CA84A5  0000                    add      byte ptr [eax], al             
  0x00CA84A7  0000                    add      byte ptr [eax], al             
  0x00CA84A9  0238                    add      bh, byte ptr [eax]             
  0x00CA84AB  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
                                        ; XREF: 0x00CA8459 (cond_jump)
  0x00CA84B1  f4                      hlt                                     
  0x00CA84B2  56                      push     esi                            
  0x00CA84B3  0002                    add      byte ptr [edx], al             
  0x00CA84B5  0000                    add      byte ptr [eax], al             
  0x00CA84B7  0000                    add      byte ptr [eax], al             
  0x00CA84B9  7056                    jo       0xca8511                       
  0x00CA84BB  001e                    add      byte ptr [esi], bl             
  0x00CA84BD  0000                    add      byte ptr [eax], al             
  0x00CA84BF  0000                    add      byte ptr [eax], al             
  0x00CA84C1  f4                      hlt                                     
  0x00CA84C2  61                      popal                                   
  0x00CA84C3  001e                    add      byte ptr [esi], bl             
  0x00CA84C5  0000                    add      byte ptr [eax], al             
  0x00CA84C7  0000                    add      byte ptr [eax], al             
  0x00CA84C9  0238                    add      bh, byte ptr [eax]             
  0x00CA84CB  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
                                        ; XREF: 0x00CA8479 (cond_jump)
  0x00CA84D1  f4                      hlt                                     
  0x00CA84D2  56                      push     esi                            
  0x00CA84D3  0007                    add      byte ptr [edi], al             
  0x00CA84D5  0000                    add      byte ptr [eax], al             
  0x00CA84D7  0000                    add      byte ptr [eax], al             
  0x00CA84D9  7056                    jo       0xca8531                       
  0x00CA84DB  001e                    add      byte ptr [esi], bl             
  0x00CA84DD  0000                    add      byte ptr [eax], al             
  0x00CA84DF  0000                    add      byte ptr [eax], al             
  0x00CA84E1  f4                      hlt                                     
  0x00CA84E2  61                      popal                                   
  0x00CA84E3  001e                    add      byte ptr [esi], bl             
  0x00CA84E5  0000                    add      byte ptr [eax], al             
  0x00CA84E7  0000                    add      byte ptr [eax], al             
  0x00CA84E9  0338                    add      edi, dword ptr [eax]           
  0x00CA84EB  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
                                        ; XREF: 0x00CA8499 (cond_jump)
  0x00CA84F1  f4                      hlt                                     
  0x00CA84F2  61                      popal                                   
  0x00CA84F3  006f0b                  add      byte ptr [edi + 0xb], ch       
  0x00CA84F6  0000                    add      byte ptr [eax], al             
  0x00CA84F8  0001                    add      byte ptr [ecx], al             
  0x00CA84FA  3800                    cmp      byte ptr [eax], al             
  0x00CA84FC  93                      xchg     ebx, eax                       
  0x00CA84FD  030d0000f956            add      ecx, dword ptr [0x56f90000]    
  0x00CA8503  0003                    add      byte ptr [ebx], al             
  0x00CA8505  0020                    add      byte ptr [eax], ah             
  0x00CA8507  004aa4                  add      byte ptr [edx - 0x5c], cl      
  0x00CA850A  050000f461              add      eax, 0x61f40000                
  0x00CA850F  00730b                  add      byte ptr [ebx + 0xb], dh       
  0x00CA8512  0000                    add      byte ptr [eax], al             
  0x00CA8514  0006                    add      byte ptr [esi], al             
  0x00CA8516  3800                    cmp      byte ptr [eax], al             
  0x00CA8518  93                      xchg     ebx, eax                       
  0x00CA8519  030d00000036            add      ecx, dword ptr [0x36000000]    
  0x00CA851F  0000                    add      byte ptr [eax], al             
  0x00CA8522  44                      inc      esp                            
  0x00CA8523  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA8529  c406                    les      eax, ptr [esi]                 
  0x00CA852B  0011                    add      byte ptr [ecx], dl             
  0x00CA852D  0000                    add      byte ptr [eax], al             
  0x00CA852F  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA84D9 (cond_jump)
  0x00CA8531  f4                      hlt                                     
  0x00CA8532  56                      push     esi                            
  0x00CA8533  00740b00                add      byte ptr [ebx + ecx], dh       
  0x00CA8537  0000                    add      byte ptr [eax], al             
  0x00CA8539  c422                    les      esp, ptr [edx]                 
  0x00CA853B  004000                  add      byte ptr [eax], al             
  0x00CA853E  2000                    and      byte ptr [eax], al             
  0x00CA8540  009121000004            add      byte ptr [ecx + 0x4000021], dl 
  0x00CA8546  3800                    cmp      byte ptr [eax], al             
  0x00CA8548  93                      xchg     ebx, eax                       
  0x00CA8549  030d0000f456            add      ecx, dword ptr [0x56f40000]    
  0x00CA854F  000400                  add      byte ptr [eax + eax], al       
  0x00CA8552  0000                    add      byte ptr [eax], al             
  0x00CA8554  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA8557  001e                    add      byte ptr [esi], bl             
  0x00CA8559  0000                    add      byte ptr [eax], al             
  0x00CA855B  0000                    add      byte ptr [eax], al             
  0x00CA855D  f4                      hlt                                     
  0x00CA855E  61                      popal                                   
  0x00CA855F  001e                    add      byte ptr [esi], bl             
  0x00CA8561  0000                    add      byte ptr [eax], al             
  0x00CA8563  0000                    add      byte ptr [eax], al             
  0x00CA8565  0338                    add      edi, dword ptr [eax]           
  0x00CA8567  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA856D  5e                      pop      esi                            
  0x00CA856E  2000                    and      byte ptr [eax], al             
  0x00CA8570  00f0                    add      al, dh                         
  0x00CA8572  56                      push     esi                            
  0x00CA8573  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA8576  0000                    add      byte ptr [eax], al             
  0x00CA8578  0300                    add      eax, dword ptr [eax]           
  0x00CA857A  2000                    and      byte ptr [eax], al             
  0x00CA857C  0da4050000              or       eax, 0x5a4                     
  0x00CA8581  f4                      hlt                                     
  0x00CA8582  61                      popal                                   
  0x00CA8583  00790b                  add      byte ptr [ecx + 0xb], bh       
  0x00CA8586  0000                    add      byte ptr [eax], al             
  0x00CA8588  000438                  add      byte ptr [eax + edi], al       
  0x00CA858B  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8591  f4                      hlt                                     
  0x00CA8592  56                      push     esi                            
  0x00CA8593  000400                  add      byte ptr [eax + eax], al       
  0x00CA8596  0000                    add      byte ptr [eax], al             
  0x00CA8598  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA859B  001e                    add      byte ptr [esi], bl             
  0x00CA859D  0000                    add      byte ptr [eax], al             
  0x00CA859F  0000                    add      byte ptr [eax], al             
  0x00CA85A1  f4                      hlt                                     
  0x00CA85A2  61                      popal                                   
  0x00CA85A3  001e                    add      byte ptr [esi], bl             
  0x00CA85A5  0000                    add      byte ptr [eax], al             
  0x00CA85A7  0000                    add      byte ptr [eax], al             
  0x00CA85A9  0338                    add      edi, dword ptr [eax]           
  0x00CA85AB  0093030d0013            add      byte ptr [ebx + 0x13000d03], dl 
  0x00CA85B1  f4                      hlt                                     
  0x00CA85B2  61                      popal                                   
  0x00CA85B3  001e                    add      byte ptr [esi], bl             
  0x00CA85B5  0000                    add      byte ptr [eax], al             
  0x00CA85B7  0000                    add      byte ptr [eax], al             
  0x00CA85B9  61                      popal                                   
  0x00CA85BA  56                      push     esi                            
  0x00CA85BB  0000                    add      byte ptr [eax], al             
  0x00CA85BD  0138                    add      dword ptr [eax], edi           
  0x00CA85BF  0093030d0001            add      byte ptr [ebx + 0x1000d03], dl 
  0x00CA85C5  0c05                    or       al, 5                          
  0x00CA85C7  0000                    add      byte ptr [eax], al             
  0x00CA85CA  56                      push     esi                            
  0x00CA85CB  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA85CE  0000                    add      byte ptr [eax], al             
  0x00CA85D0  854301                  test     dword ptr [ebx + 1], eax       
  0x00CA85D3  005524                  add      byte ptr [ebp + 0x24], dl      
  0x00CA85D6  0500004e22              add      eax, 0x224e0000                
  0x00CA85DB  0000                    add      byte ptr [eax], al             
  0x00CA85DE  44                      inc      esp                            
  0x00CA85DF  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA85E2  0000                    add      byte ptr [eax], al             
  0x00CA85E4  44                      inc      esp                            
  0x00CA85E5  f4                      hlt                                     
  0x00CA85E6  46                      inc      esi                            
  0x00CA85E7  0010                    add      byte ptr [eax], dl             
  0x00CA85E9  0000                    add      byte ptr [eax], al             
  0x00CA85EB  0000                    add      byte ptr [eax], al             
  0x00CA85EE  2100                    and      dword ptr [eax], eax           
  0x00CA85F0  00ee                    add      dh, ch                         
  0x00CA85F2  2100                    and      dword ptr [eax], eax           
  0x00CA85F4  36f4                    hlt                                     
  0x00CA85F6  44                      inc      esp                            
  0x00CA85F7  0010                    add      byte ptr [eax], dl             
  0x00CA85F9  0000                    add      byte ptr [eax], al             
  0x00CA85FB  004000                  add      byte ptr [eax], al             
  0x00CA85FE  2000                    and      byte ptr [eax], al             
  0x00CA8600  00c4                    add      ah, al                         
  0x00CA8602  2100                    and      dword ptr [eax], eax           
  0x00CA8604  b0f0                    mov      al, 0xf0                       
                                        ; XREF: 0x00CA8614 (cond_jump)
  0x00CA8606  47                      inc      edi                            
  0x00CA8607  009d0b00002e            add      byte ptr [ebp + 0x2e00000b], bl 
  0x00CA860D  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA8612  2000                    and      byte ptr [eax], al             
  0x00CA8614  70f0                    jo       0xca8606                       
  0x00CA8616  44                      inc      esp                            
  0x00CA8617  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA861A  0000                    add      byte ptr [eax], al             
  0x00CA861C  41                      inc      ecx                            
  0x00CA861E  2100                    and      dword ptr [eax], eax           
  0x00CA8620  06                      push     es                             
  0x00CA8621  1d0c0000b0              sbb      eax, 0xb000000c                
  0x00CA8626  1800                    sbb      byte ptr [eax], al             
  0x00CA8628  9e                      sahf                                    
  0x00CA8629  0b00                    or       eax, dword ptr [eax]           
  0x00CA862B  0054f044                add      byte ptr [eax + esi*8 + 0x44], dl 
  0x00CA862F  007a0b                  add      byte ptr [edx + 0xb], bh       
  0x00CA8632  0000                    add      byte ptr [eax], al             
  0x00CA8634  44                      inc      esp                            
  0x00CA8635  0020                    add      byte ptr [eax], ah             
  0x00CA8637  00740020                add      byte ptr [eax + eax + 0x20], dh 
  0x00CA863B  0003                    add      byte ptr [ebx], al             
  0x00CA863D  0020                    add      byte ptr [eax], ah             
  0x00CA863F  001a                    add      byte ptr [edx], bl             
  0x00CA8641  f4                      hlt                                     
  0x00CA8642  0500804701              add      eax, 0x1478000                 
  0x00CA8647  0000                    add      byte ptr [eax], al             
  0x00CA864A  44                      inc      esp                            
  0x00CA864B  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA864E  0000                    add      byte ptr [eax], al             
  0x00CA8650  06                      push     es                             
  0x00CA8651  1c0c                    sbb      al, 0xc                        
  0x00CA8653  0041c4                  add      byte ptr [ecx - 0x3c], al      
  0x00CA8656  2100                    and      dword ptr [eax], eax           
  0x00CA8658  40                      inc      eax                            
  0x00CA8659  0020                    add      byte ptr [eax], ah             
  0x00CA865B  0000                    add      byte ptr [eax], al             
  0x00CA865D  7056                    jo       0xca86b5                       
  0x00CA865F  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA8662  0000                    add      byte ptr [eax], al             
  0x00CA8664  c54001                  lds      eax, ptr [eax + 1]             
  0x00CA8667  00ff                    add      bh, bh                         
  0x00CA8669  0100                    add      dword ptr [eax], eax           
  0x00CA866B  0002                    add      byte ptr [edx], al             
  0x00CA866D  f4                      hlt                                     
  0x00CA866E  05000c0000              add      eax, 0xc00                     
  0x00CA8673  0000                    add      byte ptr [eax], al             
  0x00CA8676  56                      push     esi                            
  0x00CA8677  00710b                  add      byte ptr [ecx + 0xb], dh       
  0x00CA867A  0000                    add      byte ptr [eax], al             
  0x00CA867C  41                      inc      ecx                            
  0x00CA867D  c421                    les      esp, ptr [ecx]                 
  0x00CA867F  0006                    add      byte ptr [esi], al             
  0x00CA8681  1d0c0041c4              sbb      eax, 0xc441000c                
  0x00CA8686  2100                    and      dword ptr [eax], eax           
  0x00CA8688  40                      inc      eax                            
  0x00CA8689  0020                    add      byte ptr [eax], ah             
  0x00CA868B  0000                    add      byte ptr [eax], al             
  0x00CA868D  7056                    jo       0xca86e5                       
  0x00CA868F  00710b                  add      byte ptr [ecx + 0xb], dh       
  0x00CA8692  0000                    add      byte ptr [eax], al             
  0x00CA8694  00f4                    add      ah, dh                         
  0x00CA8696  60                      pushal                                  
  0x00CA8697  006c0b00                add      byte ptr [ebx + ecx], ch       
  0x00CA869B  0000                    add      byte ptr [eax], al             
  0x00CA869D  e056                    loopne   0xca86f5                       
  0x00CA869F  00440020                add      byte ptr [eax + eax + 0x20], al 
  0x00CA86A3  0000                    add      byte ptr [eax], al             
  0x00CA86A5  60                      pushal                                  
  0x00CA86A6  56                      push     esi                            
  0x00CA86A7  0000                    add      byte ptr [eax], al             
  0x00CA86A9  f4                      hlt                                     
  0x00CA86AA  61                      popal                                   
  0x00CA86AB  00660b                  add      byte ptr [esi + 0xb], ah       
  0x00CA86AE  0000                    add      byte ptr [eax], al             
  0x00CA86B0  0001                    add      byte ptr [ecx], al             
  0x00CA86B2  3800                    cmp      byte ptr [eax], al             
  0x00CA86B4  93                      xchg     ebx, eax                       
                                        ; XREF: 0x00CA865D (cond_jump)
  0x00CA86B5  030d0000f956            add      ecx, dword ptr [0x56f90000]    
  0x00CA86BB  0003                    add      byte ptr [ebx], al             
  0x00CA86BD  0020                    add      byte ptr [eax], ah             
  0x00CA86BF  0001                    add      byte ptr [ecx], al             
  0x00CA86C1  a5                      movsd    dword ptr es:[edi], dword ptr [esi] 
  0x00CA86C2  050000f461              add      eax, 0x61f40000                
  0x00CA86C7  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA86CA  0000                    add      byte ptr [eax], al             
  0x00CA86CC  0009                    add      byte ptr [ecx], cl             
  0x00CA86CE  3800                    cmp      byte ptr [eax], al             
  0x00CA86D0  93                      xchg     ebx, eax                       
  0x00CA86D1  030d0000f056            add      ecx, dword ptr [0x56f00000]    
  0x00CA86D7  00700b                  add      byte ptr [eax + 0xb], dh       
  0x00CA86DA  0000                    add      byte ptr [eax], al             
  0x00CA86DC  0300                    add      eax, dword ptr [eax]           
  0x00CA86DE  2000                    and      byte ptr [eax], al             
  0x00CA86E0  9f                      lahf                                    
  0x00CA86E1  f4                      hlt                                     
  0x00CA86E2  050000f444              add      eax, 0x44f40000                
  0x00CA86E7  0001                    add      byte ptr [ecx], al             
  0x00CA86E9  0000                    add      byte ptr [eax], al             
  0x00CA86EB  0000                    add      byte ptr [eax], al             
  0x00CA86ED  7044                    jo       0xca8733                       
  0x00CA86EF  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA86F2  0000                    add      byte ptr [eax], al             
  0x00CA86F4  13f4                    adc      esi, esp                       
  0x00CA86F6  61                      popal                                   
  0x00CA86F7  001e                    add      byte ptr [esi], bl             
  0x00CA86F9  0000                    add      byte ptr [eax], al             
  0x00CA86FB  0000                    add      byte ptr [eax], al             
  0x00CA86FD  61                      popal                                   
  0x00CA86FE  56                      push     esi                            
  0x00CA86FF  0000                    add      byte ptr [eax], al             
  0x00CA8701  0138                    add      dword ptr [eax], edi           
  0x00CA8703  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8709  002400                  add      byte ptr [eax + eax], ah       
  0x00CA870C  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA870F  001e                    add      byte ptr [esi], bl             
  0x00CA8711  0000                    add      byte ptr [eax], al             
  0x00CA8713  0000                    add      byte ptr [eax], al             
  0x00CA8715  f4                      hlt                                     
  0x00CA8716  61                      popal                                   
  0x00CA8717  001e                    add      byte ptr [esi], bl             
  0x00CA8719  0000                    add      byte ptr [eax], al             
  0x00CA871B  0000                    add      byte ptr [eax], al             
  0x00CA871D  0138                    add      dword ptr [eax], edi           
  0x00CA871F  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8726  56                      push     esi                            
  0x00CA8727  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA872A  0000                    add      byte ptr [eax], al             
  0x00CA872C  80410100                add      byte ptr [ecx + 1], 0          
  0x00CA8730  007056                  add      byte ptr [eax + 0x56], dh      
                                        ; XREF: 0x00CA86ED (cond_jump)
  0x00CA8733  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA8736  0000                    add      byte ptr [eax], al             
  0x00CA8738  00f0                    add      al, dh                         
  0x00CA873A  56                      push     esi                            
  0x00CA873B  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA873E  0000                    add      byte ptr [eax], al             
  0x00CA8740  854301                  test     dword ptr [ebx + 1], eax       
  0x00CA8743  004124                  add      byte ptr [ecx + 0x24], al      
  0x00CA8746  0500000024              add      eax, 0x24000000                
  0x00CA874B  0000                    add      byte ptr [eax], al             
  0x00CA874D  7044                    jo       0xca8793                       
  0x00CA874F  001e                    add      byte ptr [esi], bl             
  0x00CA8751  0000                    add      byte ptr [eax], al             
  0x00CA8753  0000                    add      byte ptr [eax], al             
  0x00CA8755  ee                      out      dx, al                         
  0x00CA8756  2100                    and      dword ptr [eax], eax           
  0x00CA8758  855001                  test     dword ptr [eax + 1], edx       
  0x00CA875B  000b                    add      byte ptr [ebx], cl             
  0x00CA875D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA875E  050000f044              add      eax, 0x44f00000                
  0x00CA8763  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA8766  0000                    add      byte ptr [eax], al             
  0x00CA8768  40                      inc      eax                            
  0x00CA8769  0020                    add      byte ptr [eax], ah             
  0x00CA876B  0000                    add      byte ptr [eax], al             
  0x00CA876D  e421                    in       al, 0x21                       
  0x00CA876F  0000                    add      byte ptr [eax], al             
  0x00CA8771  7056                    jo       0xca87c9                       
  0x00CA8773  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA8776  0000                    add      byte ptr [eax], al             
  0x00CA8778  00f4                    add      ah, dh                         
  0x00CA877A  61                      popal                                   
  0x00CA877B  001e                    add      byte ptr [esi], bl             
  0x00CA877D  0000                    add      byte ptr [eax], al             
  0x00CA877F  0000                    add      byte ptr [eax], al             
  0x00CA8781  98                      cwde                                    
  0x00CA8782  2000                    and      byte ptr [eax], al             
  0x00CA8784  93                      xchg     ebx, eax                       
  0x00CA8785  030d00004e22            add      ecx, dword ptr [0x224e0000]    
  0x00CA878B  0000                    add      byte ptr [eax], al             
  0x00CA878D  7056                    jo       0xca87e5                       
  0x00CA878F  00590b                  add      byte ptr [ecx + 0xb], bl       
  0x00CA8792  0000                    add      byte ptr [eax], al             
  0x00CA8794  00f4                    add      ah, dh                         
  0x00CA8796  61                      popal                                   
  0x00CA8797  001e                    add      byte ptr [esi], bl             
  0x00CA8799  0000                    add      byte ptr [eax], al             
  0x00CA879B  0000                    add      byte ptr [eax], al             
  0x00CA879D  1038                    adc      byte ptr [eax], bh             
  0x00CA879F  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA87A5  f4                      hlt                                     
  0x00CA87A6  61                      popal                                   
  0x00CA87A7  001e                    add      byte ptr [esi], bl             
  0x00CA87A9  0000                    add      byte ptr [eax], al             
  0x00CA87AB  0000                    add      byte ptr [eax], al             
  0x00CA87AD  1038                    adc      byte ptr [eax], bh             
  0x00CA87AF  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA87B6  56                      push     esi                            
  0x00CA87B7  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA87BA  0000                    add      byte ptr [eax], al             
  0x00CA87BC  80600100                and      byte ptr [eax + 1], 0          
  0x00CA87C0  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA87C3  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA87C6  0000                    add      byte ptr [eax], al             
  0x00CA87C8  0000                    add      byte ptr [eax], al             
  0x00CA87CA  2400                    and      al, 0                          
  0x00CA87CC  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA87CF  001e                    add      byte ptr [esi], bl             
  0x00CA87D1  0000                    add      byte ptr [eax], al             
  0x00CA87D3  0000                    add      byte ptr [eax], al             
  0x00CA87D6  56                      push     esi                            
  0x00CA87D7  00700b                  add      byte ptr [eax + 0xb], dh       
  0x00CA87DA  0000                    add      byte ptr [eax], al             
  0x00CA87DC  06                      push     es                             
  0x00CA87DD  1d0c0000f0              sbb      eax, 0xf000000c                
  0x00CA87E2  44                      inc      esp                            
  0x00CA87E3  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA87E6  0000                    add      byte ptr [eax], al             
  0x00CA87E8  44                      inc      esp                            
  0x00CA87E9  0020                    add      byte ptr [eax], ah             
  0x00CA87EB  0006                    add      byte ptr [esi], al             
  0x00CA87ED  1c0c                    sbb      al, 0xc                        
  0x00CA87EF  0010                    add      byte ptr [eax], dl             
  0x00CA87F1  cc                      int3                                    
  0x00CA87F2  06                      push     es                             
  0x00CA87F3  000a                    add      byte ptr [edx], cl             
  0x00CA87F5  0000                    add      byte ptr [eax], al             
  0x00CA87F7  0000                    add      byte ptr [eax], al             
  0x00CA87F9  f4                      hlt                                     
  0x00CA87FA  61                      popal                                   
  0x00CA87FB  001e                    add      byte ptr [esi], bl             
  0x00CA87FD  0000                    add      byte ptr [eax], al             
  0x00CA87FF  0000                    add      byte ptr [eax], al             
  0x00CA8801  0838                    or       byte ptr [eax], bh             
  0x00CA8803  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA880A  56                      push     esi                            
  0x00CA880B  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA880E  0000                    add      byte ptr [eax], al             
  0x00CA8810  80480100                or       byte ptr [eax + 1], 0          
  0x00CA8814  007056                  add      byte ptr [eax + 0x56], dh      
  0x00CA8817  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA881A  0000                    add      byte ptr [eax], al             
  0x00CA881C  00f0                    add      al, dh                         
  0x00CA881E  56                      push     esi                            
  0x00CA881F  00700b                  add      byte ptr [eax + 0xb], dh       
  0x00CA8822  0000                    add      byte ptr [eax], al             
  0x00CA8824  06                      push     es                             
  0x00CA8825  1d0c0000f0              sbb      eax, 0xf000000c                
  0x00CA882A  44                      inc      esp                            
  0x00CA882B  005b0b                  add      byte ptr [ebx + 0xb], bl       
  0x00CA882E  0000                    add      byte ptr [eax], al             
  0x00CA8830  44                      inc      esp                            
  0x00CA8831  0020                    add      byte ptr [eax], ah             
  0x00CA8833  0000                    add      byte ptr [eax], al             
  0x00CA8835  f4                      hlt                                     
  0x00CA8836  61                      popal                                   
  0x00CA8837  001e                    add      byte ptr [esi], bl             
  0x00CA8839  0000                    add      byte ptr [eax], al             
  0x00CA883B  0000                    add      byte ptr [eax], al             
  0x00CA883D  98                      cwde                                    
  0x00CA883E  2100                    and      dword ptr [eax], eax           
  0x00CA8840  93                      xchg     ebx, eax                       
  0x00CA8841  030d0000f056            add      ecx, dword ptr [0x56f00000]    
  0x00CA8847  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA884A  0000                    add      byte ptr [eax], al             
  0x00CA884C  00f0                    add      al, dh                         
  0x00CA884E  44                      inc      esp                            
  0x00CA884F  00700b                  add      byte ptr [eax + 0xb], dh       
  0x00CA8852  0000                    add      byte ptr [eax], al             
  0x00CA8854  44                      inc      esp                            
  0x00CA8855  0020                    add      byte ptr [eax], ah             
  0x00CA8857  000f                    add      byte ptr [edi], cl             
  0x00CA8859  0c05                    or       al, 5                          
  0x00CA885B  0000                    add      byte ptr [eax], al             
  0x00CA885E  56                      push     esi                            
  0x00CA885F  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA8862  0000                    add      byte ptr [eax], al             
  0x00CA8864  0300                    add      eax, dword ptr [eax]           
  0x00CA8866  2e000b                  add      byte ptr cs:[ebx], cl          
  0x00CA8869  f4                      hlt                                     
  0x00CA886A  0500000024              add      eax, 0x24000000                
  0x00CA886F  0000                    add      byte ptr [eax], al             
  0x00CA8871  7044                    jo       0xca88b7                       
  0x00CA8873  001e                    add      byte ptr [esi], bl             
  0x00CA8875  0000                    add      byte ptr [eax], al             
  0x00CA8877  0000                    add      byte ptr [eax], al             
  0x00CA8879  f4                      hlt                                     
  0x00CA887A  61                      popal                                   
  0x00CA887B  001e                    add      byte ptr [esi], bl             
  0x00CA887D  0000                    add      byte ptr [eax], al             
  0x00CA887F  0000                    add      byte ptr [eax], al             
  0x00CA8881  0838                    or       byte ptr [eax], bh             
  0x00CA8883  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA888A  56                      push     esi                            
  0x00CA888B  00670b                  add      byte ptr [edi + 0xb], ah       
  0x00CA888E  0000                    add      byte ptr [eax], al             
  0x00CA8890  844101                  test     byte ptr [ecx + 1], al         
  0x00CA8893  0003                    add      byte ptr [ebx], al             
  0x00CA8895  0020                    add      byte ptr [eax], ah             
  0x00CA8897  000b                    add      byte ptr [ebx], cl             
  0x00CA8899  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA889A  050010cc06              add      eax, 0x6cc1000                 
  0x00CA889F  0009                    add      byte ptr [ecx], cl             
  0x00CA88A1  0000                    add      byte ptr [eax], al             
  0x00CA88A3  0013                    add      byte ptr [ebx], dl             
  0x00CA88A5  0020                    add      byte ptr [eax], ah             
  0x00CA88A7  0000                    add      byte ptr [eax], al             
  0x00CA88A9  7056                    jo       0xca8901                       
  0x00CA88AB  0010                    add      byte ptr [eax], dl             
  0x00CA88AD  0000                    add      byte ptr [eax], al             
  0x00CA88AF  0000                    add      byte ptr [eax], al             
  0x00CA88B1  f4                      hlt                                     
  0x00CA88B2  61                      popal                                   
  0x00CA88B3  0010                    add      byte ptr [eax], dl             
  0x00CA88B5  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA8871 (cond_jump)
  0x00CA88B7  0000                    add      byte ptr [eax], al             
  0x00CA88B9  0838                    or       byte ptr [eax], bh             
  0x00CA88BB  006103                  add      byte ptr [ecx + 3], ah         
  0x00CA88BE  0d00000000              or       eax, 0                         
  0x00CA88C3  0000                    add      byte ptr [eax], al             
  0x00CA88C5  7045                    jo       0xca890c                       
  0x00CA88C7  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA88CA  0000                    add      byte ptr [eax], al             
  0x00CA88CC  007057                  add      byte ptr [eax + 0x57], dh      
  0x00CA88CF  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA88D2  0000                    add      byte ptr [eax], al             
  0x00CA88D4  007062                  add      byte ptr [eax + 0x62], dh      
  0x00CA88D7  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA88DA  0000                    add      byte ptr [eax], al             
  0x00CA88DC  22f4                    and      dh, ah                         
  0x00CA88DE  0500ffff00              add      eax, 0xffff00                  
  0x00CA88E3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA88E6  0000                    add      byte ptr [eax], al             
  0x00CA88E8  0001                    add      byte ptr [ecx], al             
  0x00CA88EA  3900                    cmp      dword ptr [eax], eax           
  0x00CA88EC  007071                  add      byte ptr [eax + 0x71], dh      
  0x00CA88EF  0002                    add      byte ptr [edx], al             
  0x00CA88F1  0000                    add      byte ptr [eax], al             
  0x00CA88F3  0000                    add      byte ptr [eax], al             
  0x00CA88F5  7071                    jo       0xca8968                       
  0x00CA88F7  0003                    add      byte ptr [ebx], al             
  0x00CA88F9  0000                    add      byte ptr [eax], al             
  0x00CA88FB  0000                    add      byte ptr [eax], al             
  0x00CA88FD  7071                    jo       0xca8970                       
  0x00CA88FF  000400                  add      byte ptr [eax + eax], al       
  0x00CA8902  0000                    add      byte ptr [eax], al             
  0x00CA8904  0000                    add      byte ptr [eax], al             
  0x00CA8906  360000                  add      byte ptr ss:[eax], al          
  0x00CA890A  44                      inc      esp                            
  0x00CA890B  00970b000010            add      byte ptr [edi + 0x1000000b], dl 
  0x00CA8911  c406                    les      eax, ptr [esi]                 
  0x00CA8913  001d00000000            add      byte ptr [0], bl               
  0x00CA8919  f4                      hlt                                     
  0x00CA891A  56                      push     esi                            
  0x00CA891B  00a50b000000            add      byte ptr [ebp + 0xb], ah       
  0x00CA8921  c422                    les      esp, ptr [edx]                 
  0x00CA8923  004000                  add      byte ptr [eax], al             
  0x00CA8926  2000                    and      byte ptr [eax], al             
  0x00CA8928  0090210000e0            add      byte ptr [eax - 0x1fffffdf], dl 
  0x00CA892E  7000                    jo       0xca8930                       
                                        ; XREF: 0x00CA892E (cond_jump)
  0x00CA8930  00c4                    add      ah, al                         
  0x00CA8932  2200                    and      al, byte ptr [eax]             
  0x00CA8934  00f4                    add      ah, dh                         
  0x00CA8936  46                      inc      esi                            
  0x00CA8937  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA893D  f4                      hlt                                     
  0x00CA893E  44                      inc      esp                            
  0x00CA893F  0000                    add      byte ptr [eax], al             
  0x00CA8941  0100                    add      dword ptr [eax], eax           
  0x00CA8943  002e                    add      byte ptr [esi], ch             
  0x00CA8945  1d0c004000              sbb      eax, 0x40000c                  
  0x00CA894A  2000                    and      byte ptr [eax], al             
  0x00CA894C  0090210000c4            add      byte ptr [eax - 0x3bffffdf], dl 
  0x00CA8952  2200                    and      al, byte ptr [eax]             
  0x00CA8954  00f4                    add      ah, dh                         
  0x00CA8956  46                      inc      esi                            
  0x00CA8957  00b5000000d0            add      byte ptr [ebp - 0x30000000], dh 
  0x00CA895E  44                      inc      esp                            
  0x00CA895F  00720b                  add      byte ptr [edx + 0xb], dh       
  0x00CA8962  0000                    add      byte ptr [eax], al             
  0x00CA8964  2e1d0c004000            sbb      eax, 0x40000c                  
  0x00CA896A  2000                    and      byte ptr [eax], al             
  0x00CA896C  009521000070            add      byte ptr [ebp + 0x70000021], dl 
  0x00CA8972  6600410b                add      byte ptr [ecx + 0xb], al       
  0x00CA8976  0000                    add      byte ptr [eax], al             
  0x00CA8978  8b040d0000f066          mov      eax, dword ptr [ecx + 0x66f00000] 
  0x00CA897F  00410b                  add      byte ptr [ecx + 0xb], al       
  0x00CA8982  0000                    add      byte ptr [eax], al             
  0x00CA8984  005e20                  add      byte ptr [esi + 0x20], bl      
  0x00CA8987  0000                    add      byte ptr [eax], al             
  0x00CA898A  56                      push     esi                            
  0x00CA898B  007e0b                  add      byte ptr [esi + 0xb], bh       
  0x00CA898E  0000                    add      byte ptr [eax], al             
  0x00CA8990  0300                    add      eax, dword ptr [eax]           
  0x00CA8992  2000                    and      byte ptr [eax], al             
  0x00CA8994  07                      pop      es                             
  0x00CA8995  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA8996  0500000738              add      eax, 0x38070000                
  0x00CA899B  0000                    add      byte ptr [eax], al             
  0x00CA899D  f4                      hlt                                     
  0x00CA899E  60                      pushal                                  
  0x00CA899F  008904000000            add      byte ptr [ecx + 4], cl         
  0x00CA89A5  f4                      hlt                                     
  0x00CA89A6  650039                  add      byte ptr gs:[ecx], bh          
  0x00CA89A9  0b00                    or       eax, dword ptr [eax]           
  0x00CA89AB  008b040d000c            add      byte ptr [ebx + 0xc000d04], cl 
  0x00CA89B1  0000                    add      byte ptr [eax], al             
  0x00CA89B3  0000                    add      byte ptr [eax], al             
  0x00CA89B6  6200                    bound    eax, qword ptr [eax]           
  0x00CA89B8  55                      push     ebp                            
  0x00CA89B9  0b00                    or       eax, dword ptr [eax]           
  0x00CA89BB  0022                    add      byte ptr [edx], ah             
  0x00CA89BE  0500470b00              add      eax, 0xb4700                   
  0x00CA89C3  0000                    add      byte ptr [eax], al             
  0x00CA89C6  57                      push     edi                            
  0x00CA89C7  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA89CA  0000                    add      byte ptr [eax], al             
  0x00CA89CC  00f0                    add      al, dh                         
  0x00CA89CE  45                      inc      ebp                            
  0x00CA89CF  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA89D2  0000                    add      byte ptr [eax], al             
  0x00CA89D4  0000                    add      byte ptr [eax], al             
  0x00CA89D6  2400                    and      al, 0                          
  0x00CA89D8  007044                  add      byte ptr [eax + 0x44], dh      
  0x00CA89DB  001e                    add      byte ptr [esi], bl             
  0x00CA89DD  0000                    add      byte ptr [eax], al             
  0x00CA89DF  0000                    add      byte ptr [eax], al             
  0x00CA89E2  56                      push     esi                            
  0x00CA89E3  006e0b                  add      byte ptr [esi + 0xb], ch       
  0x00CA89E6  0000                    add      byte ptr [eax], al             
  0x00CA89E8  855001                  test     dword ptr [eax + 1], edx       
  0x00CA89EB  0009                    add      byte ptr [ecx], cl             
  0x00CA89ED  94                      xchg     esp, eax                       
  0x00CA89EE  0500845001              add      eax, 0x1508400                 
  0x00CA89F3  0000                    add      byte ptr [eax], al             
  0x00CA89F5  7054                    jo       0xca8a4b                       
  0x00CA89F7  006e0b                  add      byte ptr [esi + 0xb], ch       
  0x00CA89FA  0000                    add      byte ptr [eax], al             
  0x00CA89FC  00f4                    add      ah, dh                         
  0x00CA89FE  61                      popal                                   
  0x00CA89FF  001e                    add      byte ptr [esi], bl             
  0x00CA8A01  0000                    add      byte ptr [eax], al             
  0x00CA8A03  0000                    add      byte ptr [eax], al             
  0x00CA8A05  1038                    adc      byte ptr [eax], bh             
  0x00CA8A07  0093030d00d5            add      byte ptr [ebx - 0x2afff2fd], dl 
  0x00CA8A0D  0f05                    syscall                                 
  0x00CA8A0F  0003                    add      byte ptr [ebx], al             
  0x00CA8A11  0020                    add      byte ptr [eax], ah             
  0x00CA8A13  0005a4050000            add      byte ptr [0x5a4], al           
  0x00CA8A19  f4                      hlt                                     
  0x00CA8A1A  61                      popal                                   
  0x00CA8A1B  001e                    add      byte ptr [esi], bl             
  0x00CA8A1D  0000                    add      byte ptr [eax], al             
  0x00CA8A1F  0000                    add      byte ptr [eax], al             
  0x00CA8A21  98                      cwde                                    
  0x00CA8A22  2100                    and      dword ptr [eax], eax           
  0x00CA8A24  93                      xchg     ebx, eax                       
  0x00CA8A25  030d00130020            add      ecx, dword ptr [0x20001300]    
  0x00CA8A2B  0000                    add      byte ptr [eax], al             
  0x00CA8A2D  7056                    jo       0xca8a85                       
  0x00CA8A2F  0001                    add      byte ptr [ecx], al             
  0x00CA8A31  0000                    add      byte ptr [eax], al             
  0x00CA8A33  0000                    add      byte ptr [eax], al             
  0x00CA8A35  7056                    jo       0xca8a8d                       
  0x00CA8A37  001e                    add      byte ptr [esi], bl             
  0x00CA8A39  0000                    add      byte ptr [eax], al             
  0x00CA8A3B  0000                    add      byte ptr [eax], al             
  0x00CA8A3D  f4                      hlt                                     
  0x00CA8A3E  61                      popal                                   
  0x00CA8A3F  001e                    add      byte ptr [esi], bl             
  0x00CA8A41  0000                    add      byte ptr [eax], al             
  0x00CA8A43  0000                    add      byte ptr [eax], al             
  0x00CA8A45  0138                    add      dword ptr [eax], edi           
  0x00CA8A47  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8A4D  f4                      hlt                                     
  0x00CA8A4E  61                      popal                                   
  0x00CA8A4F  0001                    add      byte ptr [ecx], al             
  0x00CA8A51  0000                    add      byte ptr [eax], al             
  0x00CA8A53  0000                    add      byte ptr [eax], al             
  0x00CA8A55  0138                    add      dword ptr [eax], edi           
  0x00CA8A57  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8A5D  f4                      hlt                                     
  0x00CA8A5E  61                      popal                                   
  0x00CA8A5F  001e                    add      byte ptr [esi], bl             
  0x00CA8A61  0000                    add      byte ptr [eax], al             
  0x00CA8A63  0000                    add      byte ptr [eax], al             
  0x00CA8A65  1038                    adc      byte ptr [eax], bh             
  0x00CA8A67  0093030d0000            add      byte ptr [ebx + 0xd03], dl     
  0x00CA8A6D  7045                    jo       0xca8ab4                       
  0x00CA8A6F  00560b                  add      byte ptr [esi + 0xb], dl       
  0x00CA8A72  0000                    add      byte ptr [eax], al             
  0x00CA8A74  007057                  add      byte ptr [eax + 0x57], dh      
  0x00CA8A77  00570b                  add      byte ptr [edi + 0xb], dl       
  0x00CA8A7A  0000                    add      byte ptr [eax], al             
  0x00CA8A7C  007062                  add      byte ptr [eax + 0x62], dh      
  0x00CA8A7F  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA8A82  0000                    add      byte ptr [eax], al             
  0x00CA8A84  22f4                    and      dh, ah                         
  0x00CA8A86  0500ffff00              add      eax, 0xffff00                  
  0x00CA8A8B  0000                    add      byte ptr [eax], al             
  0x00CA8A8E  56                      push     esi                            
  0x00CA8A8F  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA8A92  0000                    add      byte ptr [eax], al             
  0x00CA8A94  00f0                    add      al, dh                         
  0x00CA8A96  44                      inc      esp                            
  0x00CA8A97  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8A9A  0000                    add      byte ptr [eax], al             
  0x00CA8A9C  44                      inc      esp                            
  0x00CA8A9D  0020                    add      byte ptr [eax], ah             
  0x00CA8A9F  0000                    add      byte ptr [eax], al             
  0x00CA8AA1  c421                    les      esp, ptr [ecx]                 
  0x00CA8AA3  0000                    add      byte ptr [eax], al             
  0x00CA8AA5  f4                      hlt                                     
  0x00CA8AA6  45                      inc      ebp                            
  0x00CA8AA7  0010                    add      byte ptr [eax], dl             
  0x00CA8AA9  0000                    add      byte ptr [eax], al             
  0x00CA8AAB  00a00020002e            add      byte ptr [eax + 0x2e002000], ah 
  0x00CA8AB1  1d0c0000f0              sbb      eax, 0xf000000c                
  0x00CA8AB6  44                      inc      esp                            
  0x00CA8AB7  009d0b000040            add      byte ptr [ebp + 0x4000000b], bl 
  0x00CA8ABD  0020                    add      byte ptr [eax], ah             
  0x00CA8ABF  0000                    add      byte ptr [eax], al             
  0x00CA8AC1  7056                    jo       0xca8b19                       
  0x00CA8AC3  009d0b00000c            add      byte ptr [ebp + 0xc00000b], bl 
  0x00CA8AC9  0000                    add      byte ptr [eax], al             
  0x00CA8ACB  0000                    add      byte ptr [eax], al             
  0x00CA8ACE  56                      push     esi                            
  0x00CA8ACF  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA8AD2  0000                    add      byte ptr [eax], al             
  0x00CA8AD4  854001                  test     dword ptr [eax + 1], eax       
  0x00CA8AD7  0010                    add      byte ptr [eax], dl             
  0x00CA8AD9  2405                    and      al, 5                          
  0x00CA8ADB  0000                    add      byte ptr [eax], al             
  0x00CA8ADE  60                      pushal                                  
  0x00CA8ADF  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8AE2  0000                    add      byte ptr [eax], al             
  0x00CA8AE4  005820                  add      byte ptr [eax + 0x20], bl      
  0x00CA8AE7  0020                    add      byte ptr [eax], ah             
  0x00CA8AEA  0500470b00              add      eax, 0xb4700                   
  0x00CA8AEF  0000                    add      byte ptr [eax], al             
  0x00CA8AF2  56                      push     esi                            
  0x00CA8AF3  00580b                  add      byte ptr [eax + 0xb], bl       
  0x00CA8AF6  0000                    add      byte ptr [eax], al             
  0x00CA8AF8  844101                  test     byte ptr [ecx + 1], al         
  0x00CA8AFB  001b                    add      byte ptr [ebx], bl             
  0x00CA8AFD  d821                    fsub     dword ptr [ecx]                
  0x00CA8AFF  00b3030d0000            add      byte ptr [ebx + 0xd03], dh     
  0x00CA8B05  7055                    jo       0xca8b5c                       
  0x00CA8B07  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8B0A  0000                    add      byte ptr [eax], al             
  0x00CA8B0C  20f4                    and      ah, dh                         
  0x00CA8B0E  0500ffff00              add      eax, 0xffff00                  
  0x00CA8B13  00da                    add      dl, bl                         
  0x00CA8B15  0c05                    or       al, 5                          
  0x00CA8B17  008541010003            add      byte ptr [ebp + 0x3000141], al 
  0x00CA8B1D  a4                      movsb    byte ptr es:[edi], byte ptr [esi] 
  0x00CA8B1E  0500854201              add      eax, 0x1428500                 
  0x00CA8B23  000f                    add      byte ptr [edi], cl             
  0x00CA8B25  2405                    and      al, 5                          
  0x00CA8B27  0000                    add      byte ptr [eax], al             
  0x00CA8B2A  60                      pushal                                  
  0x00CA8B2B  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8B2E  0000                    add      byte ptr [eax], al             
  0x00CA8B30  20f0                    and      al, dh                         
  0x00CA8B32  0500470b00              add      eax, 0xb4700                   
  0x00CA8B37  001b                    add      byte ptr [ebx], bl             
  0x00CA8B3A  7000                    jo       0xca8b3c                       
                                        ; XREF: 0x00CA8B3A (cond_jump)
  0x00CA8B3C  58                      pop      eax                            
  0x00CA8B3D  0b00                    or       eax, dword ptr [eax]           
  0x00CA8B3F  0000                    add      byte ptr [eax], al             
  0x00CA8B42  55                      push     ebp                            
  0x00CA8B43  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8B46  0000                    add      byte ptr [eax], al             
  0x00CA8B48  b303                    mov      bl, 3                          
  0x00CA8B4A  0d00007055              or       eax, 0x55700000                
  0x00CA8B4F  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8B52  0000                    add      byte ptr [eax], al             
  0x00CA8B54  20f4                    and      ah, dh                         
  0x00CA8B56  0500ffff00              add      eax, 0xffff00                  
  0x00CA8B5B  00c8                    add      al, cl                         
  0x00CA8B5D  0c05                    or       al, 5                          
  0x00CA8B5F  008543010082            add      byte ptr [ebp - 0x7dfffebd], al 
  0x00CA8B65  2405                    and      al, 5                          
  0x00CA8B67  0000                    add      byte ptr [eax], al             
  0x00CA8B6A  56                      push     esi                            
  0x00CA8B6B  00590b                  add      byte ptr [ecx + 0xb], bl       
  0x00CA8B6E  0000                    add      byte ptr [eax], al             
  0x00CA8B70  00f0                    add      al, dh                         
  0x00CA8B72  44                      inc      esp                            
  0x00CA8B73  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8B76  0000                    add      byte ptr [eax], al             
  0x00CA8B78  44                      inc      esp                            
  0x00CA8B79  0020                    add      byte ptr [eax], ah             
  0x00CA8B7B  008041010000            add      byte ptr [eax + 0x141], al     
  0x00CA8B81  d821                    fsub     dword ptr [ecx]                
  0x00CA8B83  0000                    add      byte ptr [eax], al             
  0x00CA8B85  90                      nop                                     
  0x00CA8B86  2000                    and      byte ptr [eax], al             
  0x00CA8B88  20f0                    and      al, dh                         
  0x00CA8B8A  0500470b00              add      eax, 0xb4700                   
  0x00CA8B8F  0000                    add      byte ptr [eax], al             
  0x00CA8B92  55                      push     ebp                            
  0x00CA8B93  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8B96  0000                    add      byte ptr [eax], al             
  0x00CA8B98  b303                    mov      bl, 3                          
  0x00CA8B9A  0d00911e0c              or       eax, 0xc1e9100                 
  0x00CA8B9F  0000                    add      byte ptr [eax], al             
  0x00CA8BA2  61                      popal                                   
  0x00CA8BA3  00590b                  add      byte ptr [ecx + 0xb], bl       
  0x00CA8BA6  0000                    add      byte ptr [eax], al             
  0x00CA8BA8  006155                  add      byte ptr [ecx + 0x55], ah      
  0x00CA8BAB  00911c0c0000            add      byte ptr [ecx + 0xc1c], dl     
  0x00CA8BB2  44                      inc      esp                            
  0x00CA8BB3  009d0b000000            add      byte ptr [ebp + 0xb], bl       
  0x00CA8BB9  082500a00020            or       byte ptr [0x2000a000], ah      
  0x00CA8BBF  0000                    add      byte ptr [eax], al             
  0x00CA8BC2  44                      inc      esp                            
  0x00CA8BC3  009b0b000041            add      byte ptr [ebx + 0x4100000b], bl 
  0x00CA8BC9  c421                    les      esp, ptr [ecx]                 
  0x00CA8BCB  00440020                add      byte ptr [eax + eax + 0x20], al 
  0x00CA8BCF  0000                    add      byte ptr [eax], al             
  0x00CA8BD1  0423                    add      al, 0x23                       
  0x00CA8BD3  00440020                add      byte ptr [eax + eax + 0x20], al 
  0x00CA8BD7  0000                    add      byte ptr [eax], al             
  0x00CA8BD9  9a200000d82100          lcall    0x21, 0xd8000020               
  0x00CA8BE0  00f0                    add      al, dh                         
  0x00CA8BE2  56                      push     esi                            
  0x00CA8BE3  00590b                  add      byte ptr [ecx + 0xb], bl       
  0x00CA8BE6  0000                    add      byte ptr [eax], al             
  0x00CA8BE8  80410100                add      byte ptr [ecx + 1], 0          
  0x00CA8BEC  00d0                    add      al, dl                         
  0x00CA8BEE  2100                    and      dword ptr [eax], eax           
  0x00CA8BF0  ca030d                  retf     0xd03                          
  0x00CA8BF3  0000                    add      byte ptr [eax], al             
  0x00CA8BF6  56                      push     esi                            
  0x00CA8BF7  00590b                  add      byte ptr [ecx + 0xb], bl       
  0x00CA8BFA  0000                    add      byte ptr [eax], al             
  0x00CA8BFC  80410100                add      byte ptr [ecx + 1], 0          
  0x00CA8C00  00d0                    add      al, dl                         
  0x00CA8C02  2100                    and      dword ptr [eax], eax           
  0x00CA8C04  91                      xchg     ecx, eax                       
  0x00CA8C05  1e                      push     ds                             
  0x00CA8C06  0c00                    or       al, 0                          
  0x00CA8C08  006055                  add      byte ptr [eax + 0x55], ah      
  0x00CA8C0B  00911c0c0000            add      byte ptr [ecx + 0xc1c], dl     
  0x00CA8C12  56                      push     esi                            
  0x00CA8C13  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA8C16  0000                    add      byte ptr [eax], al             
  0x00CA8C18  00f0                    add      al, dh                         
  0x00CA8C1A  44                      inc      esp                            
  0x00CA8C1B  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8C1E  0000                    add      byte ptr [eax], al             
  0x00CA8C20  44                      inc      esp                            
  0x00CA8C21  0020                    add      byte ptr [eax], ah             
  0x00CA8C23  0000                    add      byte ptr [eax], al             
  0x00CA8C25  44                      inc      esp                            
  0x00CA8C26  2300                    and      eax, dword ptr [eax]           
  0x00CA8C28  44                      inc      esp                            
  0x00CA8C29  0020                    add      byte ptr [eax], ah             
  0x00CA8C2B  0000                    add      byte ptr [eax], al             
  0x00CA8C2D  0423                    add      al, 0x23                       
  0x00CA8C2F  00440020                add      byte ptr [eax + eax + 0x20], al 
  0x00CA8C33  0000                    add      byte ptr [eax], al             
  0x00CA8C35  d821                    fsub     dword ptr [ecx]                
  0x00CA8C37  0000                    add      byte ptr [eax], al             
  0x00CA8C3A  56                      push     esi                            
  0x00CA8C3B  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8C3E  0000                    add      byte ptr [eax], al             
  0x00CA8C40  40                      inc      eax                            
  0x00CA8C41  0020                    add      byte ptr [eax], ah             
  0x00CA8C43  0000                    add      byte ptr [eax], al             
  0x00CA8C45  44                      inc      esp                            
  0x00CA8C46  2300                    and      eax, dword ptr [eax]           
  0x00CA8C48  40                      inc      eax                            
  0x00CA8C49  0020                    add      byte ptr [eax], ah             
  0x00CA8C4B  0000                    add      byte ptr [eax], al             
  0x00CA8C4D  d021                    shl      byte ptr [ecx], 1              
  0x00CA8C4F  001b                    add      byte ptr [ebx], bl             
  0x00CA8C51  0020                    add      byte ptr [eax], ah             
  0x00CA8C53  00b3030d0000            add      byte ptr [ebx + 0xd03], dh     
  0x00CA8C59  7055                    jo       0xca8cb0                       
  0x00CA8C5B  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8C5E  0000                    add      byte ptr [eax], al             
  0x00CA8C60  20f4                    and      ah, dh                         
  0x00CA8C62  0500ffff00              add      eax, 0xffff00                  
  0x00CA8C67  00450c                  add      byte ptr [ebp + 0xc], al       
  0x00CA8C6A  0500854401              add      eax, 0x1448500                 
  0x00CA8C6F  000f                    add      byte ptr [edi], cl             
  0x00CA8C71  2405                    and      al, 5                          
  0x00CA8C73  0020                    add      byte ptr [eax], ah             
  0x00CA8C76  0500470b00              add      eax, 0xb4700                   
  0x00CA8C7B  0000                    add      byte ptr [eax], al             
  0x00CA8C7E  60                      pushal                                  
  0x00CA8C7F  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8C82  0000                    add      byte ptr [eax], al             
  0x00CA8C84  00f0                    add      al, dh                         
  0x00CA8C86  7000                    jo       0xca8c88                       
                                        ; XREF: 0x00CA8C86 (cond_jump)
  0x00CA8C88  58                      pop      eax                            
  0x00CA8C89  0b00                    or       eax, dword ptr [eax]           
  0x00CA8C8B  0000                    add      byte ptr [eax], al             
  0x00CA8C8E  57                      push     edi                            
  0x00CA8C8F  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8C92  0000                    add      byte ptr [eax], al             
  0x00CA8C94  b303                    mov      bl, 3                          
  0x00CA8C96  0d00007055              or       eax, 0x55700000                
  0x00CA8C9B  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8C9E  0000                    add      byte ptr [eax], al             
  0x00CA8CA0  20f4                    and      ah, dh                         
  0x00CA8CA2  0500ffff00              add      eax, 0xffff00                  
  0x00CA8CA7  00150c050085            add      byte ptr [0x8500050c], dl      
  0x00CA8CAD  45                      inc      ebp                            
  0x00CA8CAE  0100                    add      dword ptr [eax], eax           
                                        ; XREF: 0x00CA8C59 (cond_jump)
  0x00CA8CB0  1324050020f005          adc      esp, dword ptr [eax + 0x5f02000] 
  0x00CA8CB7  00470b                  add      byte ptr [edi + 0xb], al       
  0x00CA8CBA  0000                    add      byte ptr [eax], al             
  0x00CA8CBC  00f0                    add      al, dh                         
  0x00CA8CBE  60                      pushal                                  
  0x00CA8CBF  00450b                  add      byte ptr [ebp + 0xb], al       
  0x00CA8CC2  0000                    add      byte ptr [eax], al             
  0x00CA8CC4  00f0                    add      al, dh                         
  0x00CA8CC6  7000                    jo       0xca8cc8                       
                                        ; XREF: 0x00CA8CC6 (cond_jump)
  0x00CA8CC8  58                      pop      eax                            
  0x00CA8CC9  0b00                    or       eax, dword ptr [eax]           
  0x00CA8CCB  0000                    add      byte ptr [eax], al             
  0x00CA8CCE  57                      push     edi                            
  0x00CA8CCF  005a0b                  add      byte ptr [edx + 0xb], bl       
  0x00CA8CD2  0000                    add      byte ptr [eax], al             
  0x00CA8CD4  b303                    mov      bl, 3                          
  0x00CA8CD6  0d0000f056              or       eax, 0x56f00000                
  0x00CA8CDB  00550b                  add      byte ptr [ebp + 0xb], dl       
  0x00CA8CDE  0000                    add      byte ptr [eax], al             
  0x00CA8CE0  844101                  test     byte ptr [ecx + 1], al         
  0x00CA8CE3  0000                    add      byte ptr [eax], al             
  0x00CA8CE5  d021                    shl      byte ptr [ecx], 1              
  0x00CA8CE7  00911e0c0000            add      byte ptr [ecx + 0xc1e], dl     
  0x00CA8CED  60                      pushal                                  
  0x00CA8CEE  55                      push     ebp                            
  0x00CA8CEF  00911c0c0020            add      byte ptr [ecx + 0x20000c1c], dl 
  0x00CA8CF5  f4                      hlt                                     
  0x00CA8CF6  0500ffff00              add      eax, 0xffff00                  
  0x00CA8CFB  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA8CFE  0000                    add      byte ptr [eax], al             
  0x00CA8D00  00f4                    add      ah, dh                         
  0x00CA8D02  56                      push     esi                            
  0x00CA8D03  0012                    add      byte ptr [edx], dl             
  0x00CA8D05  0000                    add      byte ptr [eax], al             
  0x00CA8D07  0000                    add      byte ptr [eax], al             
  0x00CA8D09  f4                      hlt                                     
  0x00CA8D0A  57                      push     edi                            
  0x00CA8D0B  0001                    add      byte ptr [ecx], al             
  0x00CA8D0D  0000                    add      byte ptr [eax], al             
  0x00CA8D0F  0000                    add      byte ptr [eax], al             
  0x00CA8D11  f4                      hlt                                     
  0x00CA8D12  7000                    jo       0xca8d14                       
                                        ; XREF: 0x00CA8D12 (cond_jump)
  0x00CA8D14  90                      nop                                     
  0x00CA8D15  0300                    add      eax, dword ptr [eax]           
  0x00CA8D17  0000                    add      byte ptr [eax], al             
  0x00CA8D19  0039                    add      byte ptr [ecx], bh             
  0x00CA8D1B  0000                    add      byte ptr [eax], al             
  0x00CA8D1D  f4                      hlt                                     
  0x00CA8D1E  60                      pushal                                  
  0x00CA8D1F  0000                    add      byte ptr [eax], al             
  0x00CA8D21  0100                    add      dword ptr [eax], eax           
  0x00CA8D23  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA8D29  0100                    add      dword ptr [eax], eax           
  0x00CA8D2B  0003                    add      byte ptr [ebx], al             
  0x00CA8D2D  0020                    add      byte ptr [eax], ah             
  0x00CA8D2F  0000                    add      byte ptr [eax], al             
  0x00CA8D31  2405                    and      al, 5                          
  0x00CA8D33  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA8D36  0000                    add      byte ptr [eax], al             
  0x00CA8D38  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA8D3B  00fd                    add      ch, bh                         
  0x00CA8D3D  0000                    add      byte ptr [eax], al             
  0x00CA8D3F  0000                    add      byte ptr [eax], al             
  0x00CA8D41  7044                    jo       0xca8d87                       
  0x00CA8D43  00fe                    add      dh, bh                         
  0x00CA8D45  0000                    add      byte ptr [eax], al             
  0x00CA8D47  0000                    add      byte ptr [eax], al             
  0x00CA8D49  7060                    jo       0xca8dab                       
  0x00CA8D4B  00ff                    add      bh, bh                         
  0x00CA8D4D  0000                    add      byte ptr [eax], al             
  0x00CA8D4F  0003                    add      byte ptr [ebx], al             
  0x00CA8D51  0020                    add      byte ptr [eax], ah             
  0x00CA8D53  0010                    add      byte ptr [eax], dl             
  0x00CA8D55  2405                    and      al, 5                          
  0x00CA8D57  0000                    add      byte ptr [eax], al             
  0x00CA8D59  f4                      hlt                                     
  0x00CA8D5A  56                      push     esi                            
  0x00CA8D5B  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA8D5E  0000                    add      byte ptr [eax], al             
  0x00CA8D60  00f4                    add      ah, dh                         
  0x00CA8D62  7000                    jo       0xca8d64                       
                                        ; XREF: 0x00CA8D62 (cond_jump)
  0x00CA8D64  da0500000000            fiadd    dword ptr [0]                  
  0x00CA8D6A  3900                    cmp      dword ptr [eax], eax           
  0x00CA8D6C  80f00b                  xor      al, 0xb                        
  0x00CA8D6F  008001000003            add      byte ptr [eax + 0x3000001], al 
  0x00CA8D75  0020                    add      byte ptr [eax], ah             
  0x00CA8D77  0000                    add      byte ptr [eax], al             
  0x00CA8D79  2405                    and      al, 5                          
  0x00CA8D7B  0000                    add      byte ptr [eax], al             
  0x00CA8D7E  56                      push     esi                            
  0x00CA8D7F  00fd                    add      ch, bh                         
  0x00CA8D81  0000                    add      byte ptr [eax], al             
  0x00CA8D83  0000                    add      byte ptr [eax], al             
  0x00CA8D86  60                      pushal                                  
                                        ; XREF: 0x00CA8D41 (cond_jump)
  0x00CA8D87  00ff                    add      bh, bh                         
  0x00CA8D89  0000                    add      byte ptr [eax], al             
  0x00CA8D8B  0000                    add      byte ptr [eax], al             
  0x00CA8D8E  44                      inc      esp                            
  0x00CA8D8F  00fe                    add      dh, bh                         
  0x00CA8D91  0000                    add      byte ptr [eax], al             
  0x00CA8D93  0003                    add      byte ptr [ebx], al             
  0x00CA8D95  f4                      hlt                                     
  0x00CA8D96  45                      inc      ebp                            
  0x00CA8D97  00da                    add      dl, bl                         
  0x00CA8D99  0500000324              add      eax, 0x24030000                
  0x00CA8D9E  0500007045              add      eax, 0x45700000                
  0x00CA8DA3  00480b                  add      byte ptr [eax + 0xb], cl       
  0x00CA8DA6  0000                    add      byte ptr [eax], al             
  0x00CA8DA8  0098200000f0            add      byte ptr [eax - 0xfffffe0], bl 
  0x00CA8DAE  56                      push     esi                            
  0x00CA8DAF  00480b                  add      byte ptr [eax + 0xb], cl       
  0x00CA8DB2  0000                    add      byte ptr [eax], al             
  0x00CA8DB4  40                      inc      eax                            
  0x00CA8DB5  99                      cdq                                     
  0x00CA8DB6  2100                    and      dword ptr [eax], eax           
  0x00CA8DB8  007054                  add      byte ptr [eax + 0x54], dh      
  0x00CA8DBB  00480b                  add      byte ptr [eax + 0xb], cl       
  0x00CA8DBE  0000                    add      byte ptr [eax], al             
  0x00CA8DC0  00f4                    add      ah, dh                         
  0x00CA8DC2  56                      push     esi                            
  0x00CA8DC3  0009                    add      byte ptr [ecx], cl             
  0x00CA8DC5  0000                    add      byte ptr [eax], al             
  0x00CA8DC7  0000                    add      byte ptr [eax], al             
  0x00CA8DC9  f4                      hlt                                     
  0x00CA8DCA  57                      push     edi                            
  0x00CA8DCB  0002                    add      byte ptr [edx], al             
  0x00CA8DCD  0000                    add      byte ptr [eax], al             
  0x00CA8DCF  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA8DD5  0100                    add      dword ptr [eax], eax           
  0x00CA8DD7  0003                    add      byte ptr [ebx], al             
  0x00CA8DD9  0020                    add      byte ptr [eax], ah             
  0x00CA8DDB  0000                    add      byte ptr [eax], al             
  0x00CA8DDD  2405                    and      al, 5                          
  0x00CA8DDF  0000                    add      byte ptr [eax], al             
  0x00CA8DE2  56                      push     esi                            
  0x00CA8DE3  00fd                    add      ch, bh                         
  0x00CA8DE5  0000                    add      byte ptr [eax], al             
  0x00CA8DE7  008545010009            add      byte ptr [ebp + 0x9000145], al 
  0x00CA8DED  2405                    and      al, 5                          
  0x00CA8DEF  0000                    add      byte ptr [eax], al             
  0x00CA8DF1  f4                      hlt                                     
  0x00CA8DF2  56                      push     esi                            
  0x00CA8DF3  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA8DF6  0000                    add      byte ptr [eax], al             
  0x00CA8DF8  00f4                    add      ah, dh                         
  0x00CA8DFA  7000                    jo       0xca8dfc                       
                                        ; XREF: 0x00CA8DFA (cond_jump)
  0x00CA8DFC  2201                    and      al, byte ptr [ecx]             
  0x00CA8DFE  0000                    add      byte ptr [eax], al             
  0x00CA8E00  00f4                    add      ah, dh                         
  0x00CA8E02  7100                    jno      0xca8e04                       
                                        ; XREF: 0x00CA8E02 (cond_jump)
  0x00CA8E04  de0a                    fimul    word ptr [edx]                 
  0x00CA8E06  0000                    add      byte ptr [eax], al             
  0x00CA8E08  80f00b                  xor      al, 0xb                        
  0x00CA8E0B  008001000000            add      byte ptr [eax + 1], al         
  0x00CA8E11  f4                      hlt                                     
  0x00CA8E12  56                      push     esi                            
  0x00CA8E13  0016                    add      byte ptr [esi], dl             
  0x00CA8E15  0000                    add      byte ptr [eax], al             
  0x00CA8E17  0000                    add      byte ptr [eax], al             
  0x00CA8E19  f4                      hlt                                     
  0x00CA8E1A  57                      push     edi                            
  0x00CA8E1B  0002                    add      byte ptr [edx], al             
  0x00CA8E1D  0000                    add      byte ptr [eax], al             
  0x00CA8E1F  0000                    add      byte ptr [eax], al             
  0x00CA8E21  0039                    add      byte ptr [ecx], bh             
  0x00CA8E23  0000                    add      byte ptr [eax], al             
  0x00CA8E25  f4                      hlt                                     
  0x00CA8E26  7000                    jo       0xca8e28                       
                                        ; XREF: 0x00CA8E26 (cond_jump)
  0x00CA8E28  800000                  add      byte ptr [eax], 0              
  0x00CA8E2B  0000                    add      byte ptr [eax], al             
  0x00CA8E2D  f4                      hlt                                     
  0x00CA8E2E  60                      pushal                                  
  0x00CA8E2F  00400b                  add      byte ptr [eax + 0xb], al       
  0x00CA8E32  0000                    add      byte ptr [eax], al             
  0x00CA8E34  80f00b                  xor      al, 0xb                        
  0x00CA8E37  008001000003            add      byte ptr [eax + 0x3000001], al 
  0x00CA8E3D  0020                    add      byte ptr [eax], ah             
  0x00CA8E3F  0000                    add      byte ptr [eax], al             
  0x00CA8E41  2405                    and      al, 5                          
  0x00CA8E43  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA8E46  0000                    add      byte ptr [eax], al             
  0x00CA8E48  00f4                    add      ah, dh                         
  0x00CA8E4A  56                      push     esi                            
  0x00CA8E4B  0016                    add      byte ptr [esi], dl             
  0x00CA8E4D  0000                    add      byte ptr [eax], al             
  0x00CA8E4F  0000                    add      byte ptr [eax], al             
  0x00CA8E51  f4                      hlt                                     
  0x00CA8E52  57                      push     edi                            
  0x00CA8E53  0000                    add      byte ptr [eax], al             
  0x00CA8E55  0000                    add      byte ptr [eax], al             
  0x00CA8E57  0000                    add      byte ptr [eax], al             
  0x00CA8E59  f4                      hlt                                     
  0x00CA8E5A  7000                    jo       0xca8e5c                       
                                        ; XREF: 0x00CA8E5A (cond_jump)
  0x00CA8E5C  800000                  add      byte ptr [eax], 0              
  0x00CA8E5F  0080f00b0080            add      byte ptr [eax - 0x7ffff410], al 
  0x00CA8E65  0100                    add      dword ptr [eax], eax           
  0x00CA8E67  0003                    add      byte ptr [ebx], al             
  0x00CA8E69  0020                    add      byte ptr [eax], ah             
  0x00CA8E6B  0000                    add      byte ptr [eax], al             
  0x00CA8E6D  2405                    and      al, 5                          
  0x00CA8E6F  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA8E72  0000                    add      byte ptr [eax], al             
  0x00CA8E74  0000                    add      byte ptr [eax], al             
  0x00CA8E76  0000                    add      byte ptr [eax], al             
  0x00CA8E78  40                      inc      eax                            
  0x00CA8E79  1bd0                    sbb      edx, eax                       
  0x00CA8E7B  007d00                  add      byte ptr [ebp], bh             
  0x00CA8E7E  0000                    add      byte ptr [eax], al             
  0x00CA8E80  50                      push     eax                            
  0x00CA8E81  0105008d2e3f            add      dword ptr [0x3f2e8d00], eax    
  0x00CA8E87  0000                    add      byte ptr [eax], al             
  0x00CA8E89  7060                    jo       0xca8eeb                       
  0x00CA8E8B  0000                    add      byte ptr [eax], al             
  0x00CA8E8D  07                      pop      es                             
  0x00CA8E8E  0000                    add      byte ptr [eax], al             
  0x00CA8E90  00f4                    add      ah, dh                         
  0x00CA8E92  56                      push     esi                            
  0x00CA8E93  0007                    add      byte ptr [edi], al             
  0x00CA8E95  0000                    add      byte ptr [eax], al             
  0x00CA8E97  0000                    add      byte ptr [eax], al             
  0x00CA8E99  f4                      hlt                                     
  0x00CA8E9A  60                      pushal                                  
  0x00CA8E9B  0000                    add      byte ptr [eax], al             
  0x00CA8E9D  0000                    add      byte ptr [eax], al             
  0x00CA8E9F  0000                    add      byte ptr [eax], al             
  0x00CA8EA1  f4                      hlt                                     
  0x00CA8EA2  7000                    jo       0xca8ea4                       
                                        ; XREF: 0x00CA8EA2 (cond_jump)
  0x00CA8EA4  0001                    add      byte ptr [ecx], al             
  0x00CA8EA6  0000                    add      byte ptr [eax], al             
  0x00CA8EA8  0000                    add      byte ptr [eax], al             
  0x00CA8EAA  3900                    cmp      dword ptr [eax], eax           
  0x00CA8EAC  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA8EAF  0000                    add      byte ptr [eax], al             
  0x00CA8EB1  f4                      hlt                                     
  0x00CA8EB2  56                      push     esi                            
  0x00CA8EB3  0007                    add      byte ptr [edi], al             
  0x00CA8EB5  0000                    add      byte ptr [eax], al             
  0x00CA8EB7  0000                    add      byte ptr [eax], al             
  0x00CA8EB9  f4                      hlt                                     
  0x00CA8EBA  60                      pushal                                  
  0x00CA8EBB  0000                    add      byte ptr [eax], al             
  0x00CA8EBD  0100                    add      dword ptr [eax], eax           
  0x00CA8EBF  0000                    add      byte ptr [eax], al             
  0x00CA8EC1  f4                      hlt                                     
  0x00CA8EC2  7000                    jo       0xca8ec4                       
                                        ; XREF: 0x00CA8EC2 (cond_jump)
  0x00CA8EC4  0001                    add      byte ptr [ecx], al             
  0x00CA8EC6  0000                    add      byte ptr [eax], al             
  0x00CA8EC8  0001                    add      byte ptr [ecx], al             
  0x00CA8ECA  3900                    cmp      dword ptr [eax], eax           
  0x00CA8ECC  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA8ECF  0000                    add      byte ptr [eax], al             
  0x00CA8ED1  f4                      hlt                                     
  0x00CA8ED2  56                      push     esi                            
  0x00CA8ED3  0007                    add      byte ptr [edi], al             
  0x00CA8ED5  0000                    add      byte ptr [eax], al             
  0x00CA8ED7  0000                    add      byte ptr [eax], al             
  0x00CA8ED9  f4                      hlt                                     
  0x00CA8EDA  60                      pushal                                  
  0x00CA8EDB  0000                    add      byte ptr [eax], al             
  0x00CA8EDD  0200                    add      al, byte ptr [eax]             
  0x00CA8EDF  0000                    add      byte ptr [eax], al             
  0x00CA8EE1  f4                      hlt                                     
  0x00CA8EE2  7000                    jo       0xca8ee4                       
                                        ; XREF: 0x00CA8EE2 (cond_jump)
  0x00CA8EE4  0001                    add      byte ptr [ecx], al             
  0x00CA8EE6  0000                    add      byte ptr [eax], al             
  0x00CA8EE8  0002                    add      byte ptr [edx], al             
  0x00CA8EEA  3900                    cmp      dword ptr [eax], eax           
  0x00CA8EEC  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA8EEF  0000                    add      byte ptr [eax], al             
  0x00CA8EF1  f4                      hlt                                     
  0x00CA8EF2  56                      push     esi                            
  0x00CA8EF3  0007                    add      byte ptr [edi], al             
  0x00CA8EF5  0000                    add      byte ptr [eax], al             
  0x00CA8EF7  0000                    add      byte ptr [eax], al             
  0x00CA8EF9  f4                      hlt                                     
  0x00CA8EFA  60                      pushal                                  
  0x00CA8EFB  0000                    add      byte ptr [eax], al             
  0x00CA8EFD  0300                    add      eax, dword ptr [eax]           
  0x00CA8EFF  0000                    add      byte ptr [eax], al             
  0x00CA8F01  f4                      hlt                                     
  0x00CA8F02  7000                    jo       0xca8f04                       
                                        ; XREF: 0x00CA8F02 (cond_jump)
  0x00CA8F04  0001                    add      byte ptr [ecx], al             
  0x00CA8F06  0000                    add      byte ptr [eax], al             
  0x00CA8F08  0003                    add      byte ptr [ebx], al             
  0x00CA8F0A  3900                    cmp      dword ptr [eax], eax           
  0x00CA8F0C  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA8F0F  0000                    add      byte ptr [eax], al             
  0x00CA8F11  f4                      hlt                                     
  0x00CA8F12  56                      push     esi                            
  0x00CA8F13  0007                    add      byte ptr [edi], al             
  0x00CA8F15  0000                    add      byte ptr [eax], al             
  0x00CA8F17  0000                    add      byte ptr [eax], al             
  0x00CA8F19  f4                      hlt                                     
  0x00CA8F1A  60                      pushal                                  
  0x00CA8F1B  0000                    add      byte ptr [eax], al             
  0x00CA8F1D  0400                    add      al, 0                          
  0x00CA8F1F  0000                    add      byte ptr [eax], al             
  0x00CA8F21  f4                      hlt                                     
  0x00CA8F22  7000                    jo       0xca8f24                       
                                        ; XREF: 0x00CA8F22 (cond_jump)
  0x00CA8F24  0001                    add      byte ptr [ecx], al             
  0x00CA8F26  0000                    add      byte ptr [eax], al             
  0x00CA8F28  000439                  add      byte ptr [ecx + edi], al       
  0x00CA8F2B  0080010d0000            add      byte ptr [eax + 0xd01], al     
  0x00CA8F31  f4                      hlt                                     
  0x00CA8F32  44                      inc      esp                            
  0x00CA8F33  0000                    add      byte ptr [eax], al             
  0x00CA8F35  004000                  add      byte ptr [eax], al             
  0x00CA8F38  00704c                  add      byte ptr [eax + 0x4c], dh      
  0x00CA8F3B  0000                    add      byte ptr [eax], al             
  0x00CA8F3D  0000                    add      byte ptr [eax], al             
  0x00CA8F3F  0000                    add      byte ptr [eax], al             
  0x00CA8F41  f4                      hlt                                     
  0x00CA8F42  44                      inc      esp                            
  0x00CA8F43  007982                  add      byte ptr [ecx - 0x7e], bh      
  0x00CA8F46  5a                      pop      edx                            
  0x00CA8F47  0000                    add      byte ptr [eax], al             
  0x00CA8F49  704c                    jo       0xca8f97                       
  0x00CA8F4B  0001                    add      byte ptr [ecx], al             
  0x00CA8F4D  0000                    add      byte ptr [eax], al             
  0x00CA8F4F  0000                    add      byte ptr [eax], al             
  0x00CA8F51  f4                      hlt                                     
  0x00CA8F52  44                      inc      esp                            
  0x00CA8F53  0000                    add      byte ptr [eax], al             
  0x00CA8F55  004000                  add      byte ptr [eax], al             
  0x00CA8F58  00704c                  add      byte ptr [eax + 0x4c], dh      
  0x00CA8F5B  0002                    add      byte ptr [edx], al             
  0x00CA8F5D  0000                    add      byte ptr [eax], al             
  0x00CA8F5F  0000                    add      byte ptr [eax], al             
  0x00CA8F61  f4                      hlt                                     
  0x00CA8F62  44                      inc      esp                            
  0x00CA8F63  003c41                  add      byte ptr [ecx + eax*2], bh     
  0x00CA8F66  2d0000704c              sub      eax, 0x4c700000                
  0x00CA8F6B  0003                    add      byte ptr [ebx], al             
  0x00CA8F6D  0000                    add      byte ptr [eax], al             
  0x00CA8F6F  0000                    add      byte ptr [eax], al             
  0x00CA8F71  f4                      hlt                                     
  0x00CA8F72  44                      inc      esp                            
  0x00CA8F73  003c41                  add      byte ptr [ecx + eax*2], bh     
  0x00CA8F76  2d0000704c              sub      eax, 0x4c700000                
  0x00CA8F7B  000400                  add      byte ptr [eax + eax], al       
  0x00CA8F7E  0000                    add      byte ptr [eax], al             
  0x00CA8F80  00f0                    add      al, dh                         
  0x00CA8F82  6200                    bound    eax, qword ptr [eax]           
  0x00CA8F84  0007                    add      byte ptr [edi], al             
  0x00CA8F86  0000                    add      byte ptr [eax], al             
  0x00CA8F88  0000                    add      byte ptr [eax], al             
  0x00CA8F8A  0000                    add      byte ptr [eax], al             
  0x00CA8F8C  0000                    add      byte ptr [eax], al             
  0x00CA8F8E  0000                    add      byte ptr [eax], al             
  0x00CA8F90  d542                    aad      0x42                           
  0x00CA8F92  0200                    add      al, byte ptr [eax]             
  0x00CA8F94  9e                      sahf                                    
  0x00CA8F95  42                      inc      edx                            
  0x00CA8F96  0200                    add      al, byte ptr [eax]             
  0x00CA8F98  0300                    add      eax, dword ptr [eax]           
  0x00CA8F9A  2000                    and      byte ptr [eax], al             
  0x00CA8F9C  0ba4050000f460          or       esp, dword ptr [ebp + eax + 0x60f40000] 
  0x00CA8FA3  0000                    add      byte ptr [eax], al             
  0x00CA8FA5  0000                    add      byte ptr [eax], al             
  0x00CA8FA7  0000                    add      byte ptr [eax], al             
  0x00CA8FA9  0138                    add      dword ptr [eax], edi           
  0x00CA8FAB  0000                    add      byte ptr [eax], al             
  0x00CA8FAE  4e                      dec      esi                            
  0x00CA8FAF  0000                    add      byte ptr [eax], al             
  0x00CA8FB1  0000                    add      byte ptr [eax], al             
  0x00CA8FB3  009005060004            add      byte ptr [eax + 0x4000605], dl 
  0x00CA8FB9  0000                    add      byte ptr [eax], al             
  0x00CA8FBB  00e1                    add      cl, ah                         
  0x00CA8FBD  0020                    add      byte ptr [eax], ah             
  0x00CA8FBF  0000                    add      byte ptr [eax], al             
  0x00CA8FC1  e84e000058              call     0x58ca9014                     
  0x00CA8FC6  5e                      pop      esi                            
  0x00CA8FC7  0000                    add      byte ptr [eax], al             
  0x00CA8FC9  f4                      hlt                                     
  0x00CA8FCA  6200                    bound    eax, qword ptr [eax]           
  0x00CA8FCC  0005000000f4            add      byte ptr [0xf4000000], al      
  0x00CA8FD2  660000                  add      byte ptr [eax], al             
  0x00CA8FD5  06                      push     es                             
  0x00CA8FD6  0000                    add      byte ptr [eax], al             
  0x00CA8FD8  00f4                    add      ah, dh                         
  0x00CA8FDA  7000                    jo       0xca8fdc                       
                                        ; XREF: 0x00CA8FDA (cond_jump)
  0x00CA8FDC  0001                    add      byte ptr [ecx], al             
  0x00CA8FDE  0000                    add      byte ptr [eax], al             
  0x00CA8FE0  00f4                    add      ah, dh                         
  0x00CA8FE2  60                      pushal                                  
  0x00CA8FE3  0000                    add      byte ptr [eax], al             
  0x00CA8FE5  0000                    add      byte ptr [eax], al             
  0x00CA8FE7  0000                    add      byte ptr [eax], al             
  0x00CA8FE9  f4                      hlt                                     
  0x00CA8FEA  640000                  add      byte ptr fs:[eax], al          
  0x00CA8FED  0000                    add      byte ptr [eax], al             
  0x00CA8FEF  0000                    add      byte ptr [eax], al             
  0x00CA8FF1  1522009100              adc      eax, 0x910022                  
  0x00CA8FF6  06                      push     es                             
  0x00CA8FF7  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA8FFA  0000                    add      byte ptr [eax], al             
  0x00CA8FFC  0088f000d088            add      byte ptr [eax - 0x772fff10], cl 
  0x00CA9003  00d2                    add      dl, dl                         
  0x00CA9005  88f0                    mov      al, dh                         
  0x00CA9007  00d2                    add      dl, dl                         
  0x00CA9009  88f0                    mov      al, dh                         
  0x00CA900B  00d2                    add      dl, dl                         
  0x00CA900D  80c000                  add      al, 0                          
  0x00CA9010  d3dd                    rcr      ebp, cl                        
  0x00CA9012  4e                      dec      esi                            
  0x00CA9013  0000                    add      byte ptr [eax], al             
  0x00CA9015  b022                    mov      al, 0x22                       
  0x00CA9017  0000                    add      byte ptr [eax], al             
  0x00CA9019  f4                      hlt                                     
  0x00CA901A  640000                  add      byte ptr fs:[eax], al          
  0x00CA901D  0000                    add      byte ptr [eax], al             
  0x00CA901F  0000                    add      byte ptr [eax], al             
  0x00CA9021  5a                      pop      edx                            
  0x00CA9022  56                      push     esi                            
  0x00CA9023  0000                    add      byte ptr [eax], al             
  0x00CA9025  5e                      pop      esi                            
  0x00CA9026  56                      push     esi                            
  0x00CA9027  0000                    add      byte ptr [eax], al             
  0x00CA9029  f4                      hlt                                     
  0x00CA902A  56                      push     esi                            
  0x00CA902B  0008                    add      byte ptr [eax], cl             
  0x00CA902D  0000                    add      byte ptr [eax], al             
  0x00CA902F  0000                    add      byte ptr [eax], al             
  0x00CA9031  f4                      hlt                                     
  0x00CA9032  60                      pushal                                  
  0x00CA9033  0000                    add      byte ptr [eax], al             
  0x00CA9035  05000000f4              add      eax, 0xf4000000                
  0x00CA903A  7000                    jo       0xca903c                       
                                        ; XREF: 0x00CA903A (cond_jump)
  0x00CA903C  0001                    add      byte ptr [ecx], al             
  0x00CA903E  0000                    add      byte ptr [eax], al             
  0x00CA9040  0000                    add      byte ptr [eax], al             
  0x00CA9042  3900                    cmp      dword ptr [eax], eax           
  0x00CA9044  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA9047  0000                    add      byte ptr [eax], al             
  0x00CA9049  f4                      hlt                                     
  0x00CA904A  56                      push     esi                            
  0x00CA904B  0008                    add      byte ptr [eax], cl             
  0x00CA904D  0000                    add      byte ptr [eax], al             
  0x00CA904F  0000                    add      byte ptr [eax], al             
  0x00CA9051  f4                      hlt                                     
  0x00CA9052  60                      pushal                                  
  0x00CA9053  0000                    add      byte ptr [eax], al             
  0x00CA9055  06                      push     es                             
  0x00CA9056  0000                    add      byte ptr [eax], al             
  0x00CA9058  00f4                    add      ah, dh                         
  0x00CA905A  7000                    jo       0xca905c                       
                                        ; XREF: 0x00CA905A (cond_jump)
  0x00CA905C  0001                    add      byte ptr [ecx], al             
  0x00CA905E  0000                    add      byte ptr [eax], al             
  0x00CA9060  0001                    add      byte ptr [ecx], al             
  0x00CA9062  3900                    cmp      dword ptr [eax], eax           
  0x00CA9064  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA9067  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA906A  0000                    add      byte ptr [eax], al             
  0x00CA906C  0000                    add      byte ptr [eax], al             
  0x00CA906E  0000                    add      byte ptr [eax], al             
  0x00CA9070  40                      inc      eax                            
  0x00CA9071  1bd0                    sbb      edx, eax                       
  0x00CA9073  008a00000050            add      byte ptr [edx + 0x50000000], cl 
  0x00CA9079  0106                    add      dword ptr [esi], eax           
  0x00CA907B  0017                    add      byte ptr [edi], dl             
  0x00CA907D  95                      xchg     ebp, eax                       
  0x00CA907E  49                      dec      ecx                            
  0x00CA907F  0000                    add      byte ptr [eax], al             
  0x00CA9081  7060                    jo       0xca90e3                       
  0x00CA9083  0000                    add      byte ptr [eax], al             
  0x00CA9085  07                      pop      es                             
  0x00CA9086  0000                    add      byte ptr [eax], al             
  0x00CA9088  00f4                    add      ah, dh                         
  0x00CA908A  56                      push     esi                            
  0x00CA908B  0007                    add      byte ptr [edi], al             
  0x00CA908D  0000                    add      byte ptr [eax], al             
  0x00CA908F  0000                    add      byte ptr [eax], al             
  0x00CA9091  f4                      hlt                                     
  0x00CA9092  60                      pushal                                  
  0x00CA9093  0000                    add      byte ptr [eax], al             
  0x00CA9095  0000                    add      byte ptr [eax], al             
  0x00CA9097  0000                    add      byte ptr [eax], al             
  0x00CA9099  f4                      hlt                                     
  0x00CA909A  7000                    jo       0xca909c                       
                                        ; XREF: 0x00CA909A (cond_jump)
  0x00CA909C  0001                    add      byte ptr [ecx], al             
  0x00CA909E  0000                    add      byte ptr [eax], al             
  0x00CA90A0  0000                    add      byte ptr [eax], al             
  0x00CA90A2  3900                    cmp      dword ptr [eax], eax           
  0x00CA90A4  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA90A7  0000                    add      byte ptr [eax], al             
  0x00CA90A9  f4                      hlt                                     
  0x00CA90AA  56                      push     esi                            
  0x00CA90AB  0007                    add      byte ptr [edi], al             
  0x00CA90AD  0000                    add      byte ptr [eax], al             
  0x00CA90AF  0000                    add      byte ptr [eax], al             
  0x00CA90B1  f4                      hlt                                     
  0x00CA90B2  60                      pushal                                  
  0x00CA90B3  0000                    add      byte ptr [eax], al             
  0x00CA90B5  0100                    add      dword ptr [eax], eax           
  0x00CA90B7  0000                    add      byte ptr [eax], al             
  0x00CA90B9  f4                      hlt                                     
  0x00CA90BA  7000                    jo       0xca90bc                       
                                        ; XREF: 0x00CA90BA (cond_jump)
  0x00CA90BC  0001                    add      byte ptr [ecx], al             
  0x00CA90BE  0000                    add      byte ptr [eax], al             
  0x00CA90C0  0001                    add      byte ptr [ecx], al             
  0x00CA90C2  3900                    cmp      dword ptr [eax], eax           
  0x00CA90C4  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA90C7  0000                    add      byte ptr [eax], al             
  0x00CA90C9  f4                      hlt                                     
  0x00CA90CA  56                      push     esi                            
  0x00CA90CB  0007                    add      byte ptr [edi], al             
  0x00CA90CD  0000                    add      byte ptr [eax], al             
  0x00CA90CF  0000                    add      byte ptr [eax], al             
  0x00CA90D1  f4                      hlt                                     
  0x00CA90D2  60                      pushal                                  
  0x00CA90D3  0000                    add      byte ptr [eax], al             
  0x00CA90D5  0200                    add      al, byte ptr [eax]             
  0x00CA90D7  0000                    add      byte ptr [eax], al             
  0x00CA90D9  f4                      hlt                                     
  0x00CA90DA  7000                    jo       0xca90dc                       
                                        ; XREF: 0x00CA90DA (cond_jump)
  0x00CA90DC  0001                    add      byte ptr [ecx], al             
  0x00CA90DE  0000                    add      byte ptr [eax], al             
  0x00CA90E0  0002                    add      byte ptr [edx], al             
  0x00CA90E2  3900                    cmp      dword ptr [eax], eax           
  0x00CA90E4  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA90E7  0000                    add      byte ptr [eax], al             
  0x00CA90E9  f4                      hlt                                     
  0x00CA90EA  56                      push     esi                            
  0x00CA90EB  0007                    add      byte ptr [edi], al             
  0x00CA90ED  0000                    add      byte ptr [eax], al             
  0x00CA90EF  0000                    add      byte ptr [eax], al             
  0x00CA90F1  f4                      hlt                                     
  0x00CA90F2  60                      pushal                                  
  0x00CA90F3  0000                    add      byte ptr [eax], al             
  0x00CA90F5  0300                    add      eax, dword ptr [eax]           
  0x00CA90F7  0000                    add      byte ptr [eax], al             
  0x00CA90F9  f4                      hlt                                     
  0x00CA90FA  7000                    jo       0xca90fc                       
                                        ; XREF: 0x00CA90FA (cond_jump)
  0x00CA90FC  0001                    add      byte ptr [ecx], al             
  0x00CA90FE  0000                    add      byte ptr [eax], al             
  0x00CA9100  0003                    add      byte ptr [ebx], al             
  0x00CA9102  3900                    cmp      dword ptr [eax], eax           
  0x00CA9104  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA9107  0000                    add      byte ptr [eax], al             
  0x00CA9109  f4                      hlt                                     
  0x00CA910A  56                      push     esi                            
  0x00CA910B  0007                    add      byte ptr [edi], al             
  0x00CA910D  0000                    add      byte ptr [eax], al             
  0x00CA910F  0000                    add      byte ptr [eax], al             
  0x00CA9111  f4                      hlt                                     
  0x00CA9112  60                      pushal                                  
  0x00CA9113  0000                    add      byte ptr [eax], al             
  0x00CA9115  0400                    add      al, 0                          
  0x00CA9117  0000                    add      byte ptr [eax], al             
  0x00CA9119  f4                      hlt                                     
  0x00CA911A  7000                    jo       0xca911c                       
                                        ; XREF: 0x00CA911A (cond_jump)
  0x00CA911C  0001                    add      byte ptr [ecx], al             
  0x00CA911E  0000                    add      byte ptr [eax], al             
  0x00CA9120  000439                  add      byte ptr [ecx + edi], al       
  0x00CA9123  0080010d0000            add      byte ptr [eax + 0xd01], al     
  0x00CA9129  f4                      hlt                                     
  0x00CA912A  44                      inc      esp                            
  0x00CA912B  00ff                    add      bh, bh                         
  0x00CA912E  7f00                    jg       0xca9130                       
                                        ; XREF: 0x00CA912E (cond_jump)
  0x00CA9130  00704c                  add      byte ptr [eax + 0x4c], dh      
  0x00CA9133  0000                    add      byte ptr [eax], al             
  0x00CA9135  0000                    add      byte ptr [eax], al             
  0x00CA9137  0000                    add      byte ptr [eax], al             
  0x00CA9139  704c                    jo       0xca9187                       
  0x00CA913B  000500000000            add      byte ptr [0], al               
  0x00CA9141  f4                      hlt                                     
  0x00CA9142  44                      inc      esp                            
  0x00CA9143  007982                  add      byte ptr [ecx - 0x7e], bh      
  0x00CA9146  5a                      pop      edx                            
  0x00CA9147  0000                    add      byte ptr [eax], al             
  0x00CA9149  704c                    jo       0xca9197                       
  0x00CA914B  0002                    add      byte ptr [edx], al             
  0x00CA914D  0000                    add      byte ptr [eax], al             
  0x00CA914F  0000                    add      byte ptr [eax], al             
  0x00CA9151  704c                    jo       0xca919f                       
  0x00CA9153  0003                    add      byte ptr [ebx], al             
  0x00CA9155  0000                    add      byte ptr [eax], al             
  0x00CA9157  0000                    add      byte ptr [eax], al             
  0x00CA9159  f4                      hlt                                     
  0x00CA915A  44                      inc      esp                            
  0x00CA915B  007982                  add      byte ptr [ecx - 0x7e], bh      
  0x00CA915E  5a                      pop      edx                            
  0x00CA915F  0000                    add      byte ptr [eax], al             
  0x00CA9161  704c                    jo       0xca91af                       
  0x00CA9163  0006                    add      byte ptr [esi], al             
  0x00CA9165  0000                    add      byte ptr [eax], al             
  0x00CA9167  0000                    add      byte ptr [eax], al             
  0x00CA9169  704c                    jo       0xca91b7                       
  0x00CA916B  0009                    add      byte ptr [ecx], cl             
  0x00CA916D  0000                    add      byte ptr [eax], al             
  0x00CA916F  0000                    add      byte ptr [eax], al             
  0x00CA9171  f4                      hlt                                     
  0x00CA9172  44                      inc      esp                            
  0x00CA9173  0000                    add      byte ptr [eax], al             
  0x00CA9175  0000                    add      byte ptr [eax], al             
  0x00CA9177  0000                    add      byte ptr [eax], al             
  0x00CA9179  704c                    jo       0xca91c7                       
  0x00CA917B  000400                  add      byte ptr [eax + eax], al       
  0x00CA917E  0000                    add      byte ptr [eax], al             
  0x00CA9180  00704c                  add      byte ptr [eax + 0x4c], dh      
  0x00CA9183  0008                    add      byte ptr [eax], cl             
  0x00CA9185  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA9139 (cond_jump)
  0x00CA9187  0000                    add      byte ptr [eax], al             
  0x00CA9189  704c                    jo       0xca91d7                       
  0x00CA918B  0001                    add      byte ptr [ecx], al             
  0x00CA918D  0000                    add      byte ptr [eax], al             
  0x00CA918F  0000                    add      byte ptr [eax], al             
  0x00CA9191  704c                    jo       0xca91df                       
  0x00CA9193  0007                    add      byte ptr [edi], al             
  0x00CA9195  0000                    add      byte ptr [eax], al             
                                        ; XREF: 0x00CA9149 (cond_jump)
  0x00CA9197  0000                    add      byte ptr [eax], al             
  0x00CA919A  6200                    bound    eax, qword ptr [eax]           
  0x00CA919C  0007                    add      byte ptr [edi], al             
  0x00CA919E  0000                    add      byte ptr [eax], al             
  0x00CA91A0  0000                    add      byte ptr [eax], al             
  0x00CA91A2  0000                    add      byte ptr [eax], al             
  0x00CA91A4  0000                    add      byte ptr [eax], al             
  0x00CA91A6  0000                    add      byte ptr [eax], al             
  0x00CA91A8  d542                    aad      0x42                           
  0x00CA91AA  0200                    add      al, byte ptr [eax]             
  0x00CA91AC  9e                      sahf                                    
  0x00CA91AD  42                      inc      edx                            
  0x00CA91AE  0200                    add      al, byte ptr [eax]             
  0x00CA91B0  0300                    add      eax, dword ptr [eax]           
  0x00CA91B2  2000                    and      byte ptr [eax], al             
  0x00CA91B4  0ba4050000f460          or       esp, dword ptr [ebp + eax + 0x60f40000] 
  0x00CA91BB  0000                    add      byte ptr [eax], al             
  0x00CA91BD  0000                    add      byte ptr [eax], al             
  0x00CA91BF  0000                    add      byte ptr [eax], al             
  0x00CA91C1  0138                    add      dword ptr [eax], edi           
  0x00CA91C3  0000                    add      byte ptr [eax], al             
  0x00CA91C6  4e                      dec      esi                            
                                        ; XREF: 0x00CA9179 (cond_jump)
  0x00CA91C7  0000                    add      byte ptr [eax], al             
  0x00CA91C9  0000                    add      byte ptr [eax], al             
  0x00CA91CB  00900a060004            add      byte ptr [eax + 0x400060a], dl 
  0x00CA91D1  0000                    add      byte ptr [eax], al             
  0x00CA91D3  00e1                    add      cl, ah                         
  0x00CA91D5  0020                    add      byte ptr [eax], ah             
                                        ; XREF: 0x00CA9189 (cond_jump)
  0x00CA91D7  0000                    add      byte ptr [eax], al             
  0x00CA91D9  e84e000058              call     0x58ca922c                     
  0x00CA91DE  5e                      pop      esi                            
                                        ; XREF: 0x00CA9191 (cond_jump)
  0x00CA91DF  0000                    add      byte ptr [eax], al             
  0x00CA91E1  f4                      hlt                                     
  0x00CA91E2  6200                    bound    eax, qword ptr [eax]           
  0x00CA91E4  0005000000f4            add      byte ptr [0xf4000000], al      
  0x00CA91EA  660000                  add      byte ptr [eax], al             
  0x00CA91ED  06                      push     es                             
  0x00CA91EE  0000                    add      byte ptr [eax], al             
  0x00CA91F0  00f4                    add      ah, dh                         
  0x00CA91F2  7000                    jo       0xca91f4                       
                                        ; XREF: 0x00CA91F2 (cond_jump)
  0x00CA91F4  0001                    add      byte ptr [ecx], al             
  0x00CA91F6  0000                    add      byte ptr [eax], al             
  0x00CA91F8  00f4                    add      ah, dh                         
  0x00CA91FA  60                      pushal                                  
  0x00CA91FB  0000                    add      byte ptr [eax], al             
  0x00CA91FD  0000                    add      byte ptr [eax], al             
  0x00CA91FF  0000                    add      byte ptr [eax], al             
  0x00CA9201  f4                      hlt                                     
  0x00CA9202  640000                  add      byte ptr fs:[eax], al          
  0x00CA9205  0000                    add      byte ptr [eax], al             
  0x00CA9207  0000                    add      byte ptr [eax], al             
  0x00CA9209  1522009100              adc      eax, 0x910022                  
  0x00CA920E  06                      push     es                             
  0x00CA920F  0011                    add      byte ptr [ecx], dl             
  0x00CA9211  0000                    add      byte ptr [eax], al             
  0x00CA9213  0000                    add      byte ptr [eax], al             
  0x00CA9215  88f0                    mov      al, dh                         
  0x00CA9217  00d0                    add      al, dl                         
  0x00CA9219  dc4e00                  fmul     qword ptr [esi]                
  0x00CA921C  d888f000d2dc            fmul     dword ptr [eax - 0x232dff10]   
  0x00CA9222  4e                      dec      esi                            
  0x00CA9223  00da                    add      dl, bl                         
  0x00CA9225  88f0                    mov      al, dh                         
  0x00CA9227  00d2                    add      dl, dl                         
  0x00CA9229  dc4e00                  fmul     qword ptr [esi]                
  0x00CA922C  da88f000d2dc            fimul    dword ptr [eax - 0x232dff10]   
  0x00CA9232  4e                      dec      esi                            
  0x00CA9233  00da                    add      dl, bl                         
  0x00CA9235  88f0                    mov      al, dh                         
  0x00CA9237  00d3                    add      bl, dl                         
  0x00CA9239  dc4e00                  fmul     qword ptr [esi]                
  0x00CA923C  dbdd                    fcmovnu  st(0), st(5)                   
  0x00CA923E  4e                      dec      esi                            
  0x00CA923F  0000                    add      byte ptr [eax], al             
  0x00CA9241  b022                    mov      al, 0x22                       
  0x00CA9243  0000                    add      byte ptr [eax], al             
  0x00CA9245  f4                      hlt                                     
  0x00CA9246  640000                  add      byte ptr fs:[eax], al          
  0x00CA9249  0000                    add      byte ptr [eax], al             
  0x00CA924B  0000                    add      byte ptr [eax], al             
  0x00CA924D  5a                      pop      edx                            
  0x00CA924E  56                      push     esi                            
  0x00CA924F  0000                    add      byte ptr [eax], al             
  0x00CA9251  5e                      pop      esi                            
  0x00CA9252  57                      push     edi                            
  0x00CA9253  0000                    add      byte ptr [eax], al             
  0x00CA9255  f4                      hlt                                     
  0x00CA9256  56                      push     esi                            
  0x00CA9257  0008                    add      byte ptr [eax], cl             
  0x00CA9259  0000                    add      byte ptr [eax], al             
  0x00CA925B  0000                    add      byte ptr [eax], al             
  0x00CA925D  f4                      hlt                                     
  0x00CA925E  60                      pushal                                  
  0x00CA925F  0000                    add      byte ptr [eax], al             
  0x00CA9261  05000000f4              add      eax, 0xf4000000                
  0x00CA9266  7000                    jo       0xca9268                       
                                        ; XREF: 0x00CA9266 (cond_jump)
  0x00CA9268  0001                    add      byte ptr [ecx], al             
  0x00CA926A  0000                    add      byte ptr [eax], al             
  0x00CA926C  0000                    add      byte ptr [eax], al             
  0x00CA926E  3900                    cmp      dword ptr [eax], eax           
  0x00CA9270  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA9273  0000                    add      byte ptr [eax], al             
  0x00CA9275  f4                      hlt                                     
  0x00CA9276  56                      push     esi                            
  0x00CA9277  0008                    add      byte ptr [eax], cl             
  0x00CA9279  0000                    add      byte ptr [eax], al             
  0x00CA927B  0000                    add      byte ptr [eax], al             
  0x00CA927D  f4                      hlt                                     
  0x00CA927E  60                      pushal                                  
  0x00CA927F  0000                    add      byte ptr [eax], al             
  0x00CA9281  06                      push     es                             
  0x00CA9282  0000                    add      byte ptr [eax], al             
  0x00CA9284  00f4                    add      ah, dh                         
  0x00CA9286  7000                    jo       0xca9288                       
                                        ; XREF: 0x00CA9286 (cond_jump)
  0x00CA9288  0001                    add      byte ptr [ecx], al             
  0x00CA928A  0000                    add      byte ptr [eax], al             
  0x00CA928C  0001                    add      byte ptr [ecx], al             
  0x00CA928E  3900                    cmp      dword ptr [eax], eax           
  0x00CA9290  80010d                  add      byte ptr [ecx], 0xd            
  0x00CA9293  000c00                  add      byte ptr [eax + eax], cl       
  0x00CA9296  0000                    add      byte ptr [eax], al             
  0x00CA9298  1800                    sbb      byte ptr [eax], al             
  0x00CA929A  0000                    add      byte ptr [eax], al             
  0x00CA929C  0100                    add      dword ptr [eax], eax           
  0x00CA929E  0000                    add      byte ptr [eax], al             
  0x00CA92A0  0100                    add      dword ptr [eax], eax           
  0x00CA92A2  0000                    add      byte ptr [eax], al             
  0x00CA92A4  0000                    add      byte ptr [eax], al             
  0x00CA92A6  0000                    add      byte ptr [eax], al             
  0x00CA92A8  0000                    add      byte ptr [eax], al             
  0x00CA92AA  0000                    add      byte ptr [eax], al             
  0x00CA92AC  07                      pop      es                             
  0x00CA92AD  0000                    add      byte ptr [eax], al             
  0x00CA92AF  0001                    add      byte ptr [ecx], al             
  0x00CA92B1  0000                    add      byte ptr [eax], al             
  0x00CA92B3  001f                    add      byte ptr [edi], bl             
  0x00CA92B5  0000                    add      byte ptr [eax], al             
  0x00CA92B7  0009                    add      byte ptr [ecx], cl             
  0x00CA92B9  0000                    add      byte ptr [eax], al             
  0x00CA92BB  0000                    add      byte ptr [eax], al             
  0x00CA92BD  0000                    add      byte ptr [eax], al             
  0x00CA92BF  0001                    add      byte ptr [ecx], al             
  0x00CA92C1  0000                    add      byte ptr [eax], al             
  0x00CA92C3  0001                    add      byte ptr [ecx], al             
  0x00CA92C5  0000                    add      byte ptr [eax], al             
  0x00CA92C7  0000                    add      byte ptr [eax], al             
  0x00CA92C9  0000                    add      byte ptr [eax], al             
  0x00CA92CB  0000                    add      byte ptr [eax], al             
  0x00CA92CD  0000                    add      byte ptr [eax], al             
  0x00CA92CF  0001                    add      byte ptr [ecx], al             
  0x00CA92D1  0000                    add      byte ptr [eax], al             
  0x00CA92D3  00ef                    add      bh, ch                         
  0x00CA92D5  0000                    add      byte ptr [eax], al             
  0x00CA92D7  0000                    add      byte ptr [eax], al             
  0x00CA92D9  0000                    add      byte ptr [eax], al             
  0x00CA92DB  008cac65000200          add      byte ptr [esp + ebp*4 + 0x20065], cl 
  0x00CA92E2  0000                    add      byte ptr [eax], al             
