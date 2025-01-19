; Procedure to create and bind socket
; Input: RCX = port number
; Returns: RAX = 1 if successful, 0 if failed
proc CreateSocket uses rbx rsi rdi, port_num:QWORD
        sub     rsp, 32
        
        ; Initialize Winsock
        invoke  WriteConsoleA, [hStdOut], init_wsa_msg, init_wsa_len, written, 0
        invoke  WSAStartup, 0202h, wsadata
        test    eax, eax
        jnz     .fail_wsa

        ; Create socket
        invoke  WriteConsoleA, [hStdOut], create_sock_msg, create_sock_len, written, 0
        invoke  socket, AF_INET, SOCK_STREAM, IPPROTO_TCP
        mov     [sock], rax
        cmp     rax, INVALID_SOCKET
        je      .fail_socket

        ; Setup address structure
        mov     word [sockaddr], AF_INET           ; sin_family
        mov     ax, PORT_NUM
        xchg    ah, al                            ; Convert to network byte order
        mov     word [sockaddr+2], ax             ; sin_port
        mov     dword [sockaddr+4], 0x0100007F    ; sin_addr (127.0.0.1)
        mov     qword [sockaddr+8], 0             ; sin_zero

        ; Enable socket reuse
        mov     ebx, 1                            ; SO_REUSEADDR value = 1
        lea     rsi, [ebx]                        ; pointer to value
        invoke  setsockopt, [sock], SOL_SOCKET, SO_REUSEADDR, rsi, 4
        test    eax, eax
        jnz     .fail_setsockopt

        ; Bind socket
        invoke  WriteConsoleA, [hStdOut], bind_msg, bind_len, written, 0
        invoke  bind, [sock], sockaddr, SOCKADDR_SIZE
        test    eax, eax
        jnz     .fail_bind

        ; Listen
        invoke  WriteConsoleA, [hStdOut], listen_start_msg, listen_start_len, written, 0
        invoke  listen, [sock], SOMAXCONN
        test    eax, eax
        jnz     .fail_listen

        invoke  WriteConsoleA, [hStdOut], success_msg, success_len, written, 0
        mov     eax, 1          ; Return success
        add     rsp, 32
        ret

.fail_wsa:
        invoke  WriteConsoleA, [hStdOut], wsa_fail_msg, wsa_fail_len, written, 0
        invoke  WSAGetLastError
        mov     [last_error], eax
        jmp     .print_error

.fail_socket:
        invoke  WriteConsoleA, [hStdOut], socket_fail_msg, socket_fail_len, written, 0
        invoke  WSAGetLastError
        mov     [last_error], eax
        invoke  WSACleanup
        jmp     .print_error

.fail_setsockopt:
        invoke  WriteConsoleA, [hStdOut], setsockopt_fail_msg, setsockopt_fail_len, written, 0
        invoke  WSAGetLastError
        mov     [last_error], eax
        invoke  closesocket, [sock]
        invoke  WSACleanup
        jmp     .print_error

.fail_bind:
        invoke  WriteConsoleA, [hStdOut], bind_fail_msg, bind_fail_len, written, 0
        invoke  WSAGetLastError
        mov     [last_error], eax
        invoke  closesocket, [sock]
        invoke  WSACleanup
        jmp     .print_error

.fail_listen:
        invoke  WriteConsoleA, [hStdOut], listen_fail_msg, listen_fail_len, written, 0
        invoke  WSAGetLastError
        mov     [last_error], eax
        invoke  closesocket, [sock]
        invoke  WSACleanup

.print_error:
        ; Convert last_error to string and print it
        invoke  WriteConsoleA, [hStdOut], error_code_msg, error_code_len, written, 0
        mov     eax, [last_error]
        mov     ecx, 10         ; base 10
        lea     rdi, [error_buffer]
        add     rdi, 15         ; end of buffer
        mov     byte [rdi], 0   ; null terminator
        dec     rdi
        mov     byte [rdi], 10  ; newline
        dec     rdi
        mov     byte [rdi], 13  ; carriage return
.convert_loop:
        dec     rdi
        xor     edx, edx
        div     ecx
        add     dl, '0'
        mov     [rdi], dl
        test    eax, eax
        jnz     .convert_loop

        ; Calculate length
        lea     rcx, [error_buffer]
        add     rcx, 15
        sub     rcx, rdi        ; rcx = length
        
        invoke  WriteConsoleA, [hStdOut], rdi, rcx, written, 0
        xor     eax, eax        ; Return failure
        add     rsp, 32
        ret
endp
