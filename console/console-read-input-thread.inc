proc ReadConsoleInputTread
  ; Create read thread
  invoke  CreateThread, 0, 0, ReadConsoleCycle, 0, 0, thread_id
  ret
endp
proc ReadConsoleCycle 
  ;rcx - address of startConsoleListenStruct structure
  sub rsp, 50h
  and rsp, -16

  push [startConsoleListenStruct.stdin]
  pop qword [rbp-8h]

  push [startConsoleListenStruct.stdout]
  pop qword [rbp-10h]

  push [startConsoleListenStruct.handler_address]
  pop qword [rbp-18h]

  push [startConsoleListenStruct.buffer]
  pop qword [rbp-20h]

  push [startConsoleListenStruct.buffer_size]
  pop qword [rbp-28h]

  push [startConsoleListenStruct.prompt]
  pop qword [rbp-30h]

  push [startConsoleListenStruct.prompt_length]  
  pop qword [rbp-38h]

  ; [pbp-40h] - number of chars written
.loopReadConsoleCycle:

; print prompt line 
  mov rcx, [rbp-10h]
  mov rdx, [rbp-30h]
  mov r8,  [rbp-38h]
  lea r9, [numberOfCharsWritten]
  push 0
  call ConsolePrint

; Read input
  mov     rcx, [rbp-8h]
  mov     rdx, [rbp-20h]
  mov     r8d, 255
  lea     r9, [numberOfCharsWritten]
  sub     rsp, 40                     ; Shadow space + parameter
  push    0
  call    [ReadConsoleA]
  add     rsp, 48                     ; Cleanup stack
  
; Process input buffer
  mov rcx, [rbp-20h]
  call qword [rbp-18h]


  jmp .loopReadConsoleCycle
ret
endp
