proc StringLengthA ; rcx - pointer to str1
    mov r8, rcx
    mov rcx, r8
    xor rax, rax

    .count_loop:
        cmp byte [rcx], 0
        je .count_end
        inc eax
        inc rcx
        jmp .count_loop
    .count_end:
        mov rcx, r8
        ret

endp