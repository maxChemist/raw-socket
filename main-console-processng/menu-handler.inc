proc MenuHandler 
  mov [rbp-10h], rcx ; Save rcx - address of console command
  sub rsp, 20h

  .isHelpCommand:
    mov rcx, [rbp-10h]
    mov rdx, helpCmd
    mov r8, helpCmdLength
    call CompareString

    test rax, rax
    jz .isRawSocketCommand

    mov rcx, [mainConsoleStdout]
    mov rdx, helpList
    mov r8, helpListLength
    call ConsolePrintLine
    jmp .MenuHandlerEnd

  .isRawSocketCommand:
    mov rcx, [rbp-10h]
    mov rdx, rawSocketCmd
    mov r8, rawSocketCmdLength
    call CompareString

    test rax, rax
    jz .MenuHandlerEnd

    call SocketRaw
    jmp .MenuHandlerEnd

  .MenuHandlerEnd:
  ret
endp