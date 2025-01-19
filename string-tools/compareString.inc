
proc CompareString ; rcx - pointer to str1, rdx - pointer to str2, r8 - length
    mov r9, rcx
    mov rcx, r8

    .compare_loop:
        mov al , byte [r9]
        cmp al, byte [rdx]
        jne .compare_end
        inc r9
        inc rdx
        loop .compare_loop
        mov rax, 1 ; strings are equal
        ret 
    .compare_end:
        mov rax, 0 ; strings are not equal
        ret
endp