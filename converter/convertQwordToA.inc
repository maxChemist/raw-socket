proc convertQwordToA uses rcx rdx
    mov rax, rcx
    mov r8, rdx
    mov rcx, 16
    
    @@convertQwordToAloop:
        rol rax, 4
        mov dl, al
        and dl, 0Fh
        add dl, '0' 
        cmp dl, '9'
        jle @@less
        add dl, 7
    @@less:
        mov [r8], dl
        inc r8
        loop @@convertQwordToAloop

    mov byte [r8], 0
    ret
endp