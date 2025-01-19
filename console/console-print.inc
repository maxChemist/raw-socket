proc ConsolePrint ;rcx - stout handler, rdx - pointer to string, r8 - length
    sub     rsp, 40
    and     rsp, -16
    lea     r9, [numberOfCharsWritten]
    push    0
    call    [WriteConsoleA]
    ret
endp