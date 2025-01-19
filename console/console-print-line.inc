proc ConsolePrintLine 
  mov [rbp-10h], rcx ; stdout handler
  mov [rbp-20h], rdx ; pointer to string
  mov [rbp-30h], r8 ; string length

  sub     rsp, 40
  and     rsp, -16

  mov     rcx, [rbp-10h]
  mov     rdx, [rbp-20h]
  mov     r8,  [rbp-30h]
  lea     r9,  [numberOfCharsWritten]
  push    0
  call    [WriteConsoleA]

      ; Write newline
  mov     rcx, [rbp-10h]
  lea     rdx, [newline]
  mov     r8d, 2
  lea     r9, [numberOfCharsWritten]
  push    0
  call    [WriteConsoleA]
  ret
endp