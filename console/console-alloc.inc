proc ConsoleAlloc
  mov     [rbp-10h], rcx ; Save rcx - address of console stdout handler
  mov     [rbp-20h], rdx ; Save rdx - address of console stdin handle
  sub     rsp, 30h
  ; Create console
  call    [AllocConsole]
  
  ; Get stdout handle
  mov     ecx, -11                    ; STD_OUTPUT_HANDLE
  call    [GetStdHandle]
  mov     rcx, [rbp-10h]
  mov     [rcx], rax
  
  ; Get stdin handle
  mov     ecx, -10                    ; STD_INPUT_HANDLE
  call    [GetStdHandle]
  mov     rcx, [rbp-20h]
  mov     [rcx], rax
  
  ; Set console mode
  mov     rcx, [rbp-20h]
  mov     edx, ENABLE_LINE_INPUT or ENABLE_ECHO_INPUT or ENABLE_PROCESSED_INPUT
  call    [SetConsoleMode]

  ret
endp