proc OpenRawSocketThread
  ; Create read thread
  invoke  CreateThread, 0, 0, SocketRaw, 0, 0, rawSocketThreadId
  ret
endp
proc SocketRaw
    sub rsp, 20h
    and rsp, -16
     
    ; Initialize Winsock
    sub rsp, 40h                             ; Allocate shadow space
    xor rcx, rcx
    mov cx, [wsaversion]                     ; Version 2.2
    lea rdx, [wsadata]                      ; Address of WSAData buffer
    call [WSAStartup]                       ; Call WSAStartup
    add rsp, 40h                            ; Clean shadow space
    test eax, eax                           ; Check for errors
    
    ; Create raw socket
    invoke socket, AF_INET, SOCK_RAW, IPPROTO_ICMP ; IPPROTO_TCP ; IPPROTO_ICMP
    test rax, rax
    
    js exit_program
    mov [rawSocketHandler], rax  ; Store socket handle

    ; Set the IP_HDRINCL option to include the IP header manually
    ; sub rsp, 40h
    ; mov rcx, [rawSocketHandler]    ; Socket handle
    ; mov rdx, IPPROTO_IP           ; Level
    ; mov r8, IP_HDRINCL            ; Option name
    ; lea r9, [optval]              ; Option value pointer
    ; mov dword [optval], 1         ; Set option value to TRUE
    ; mov qword [rsp+20h], 4        ; Option length
    ; call [setsockopt]
    ; add rsp, 40h
    
    test rax, rax                 ; Check for errors
    js exit_program

    ; Bind socket to port
    mov word [rawSocketStructureIn.sin_family], AF_INET
    
    ; Convert port to network byte order
    mov rcx, 7111                 ; Port number
    call [htons]                  ; Convert to network byte order
    mov word [rawSocketStructureIn.sin_port], ax
    
    ; Set address to INADDR_ANY in network byte order
    xor rcx, rcx                  ; INADDR_ANY = 0
    call [htonl]                  ; Convert to network byte order
    mov dword [rawSocketStructureIn.sin_addr], eax
    
    ; Zero out sin_zero field
    mov dword [rawSocketStructureIn.sin_zero], 0
    
    mov rcx, [rawSocketHandler]
    lea rdx, [rawSocketStructureIn]
    mov r8, rawSocketStructureInLength
    call [bind]
    
    test rax, rax                 ; Check bind result
    js .bind_error               ; Jump if error

    jmp .continue_after_bind

.bind_error:
    call [WSAGetLastError]
    ; Handle error here if needed
    jmp exit_program

.continue_after_bind:
    ; Open file to write received data
    ; invoke CreateFile, addr filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
    ; test rax, rax
    ; js exit_program
    ; mov rcx, rax  ; File handle

    ; Allocate buffer for received data
    sub rsp, 1024  ; Allocate 1024 bytes buffer
    lea rdx, [rsp] ; Set buffer address in rdx

receive_data:
    ; Receive data from raw socket
    sub rsp, 30h
    mov rcx, [rawSocketHandler]
    mov r8, 1024
    xor r9, r9
    mov qword [rsp+20h],00000000 
    mov qword [rsp+28h],00000000 
    call [recvfrom]
    add rsp, 30h
    call [WSAGetLastError]


    ; invoke recvfrom, rbx, rdx, 1024, 0, 0, 0
    test eax, eax
    jle receive_data  ; If no data, continue receiving

    nop
    nop
    ; Write received data to file
    ; invoke WriteFile, rcx, rdx, eax, addr bytes_written, 0
    ; test eax, eax
    ; jz receive_data   ; If unable to write, continue receiving

    ; Forward the received data (optional)
    ; You can add additional forwarding code here
    ; For example, create another socket and send the data to a remote host

    jmp receive_data  ; Continue the loop

exit_program:
    ; Clean up
    invoke closesocket, rbx
    invoke WSACleanup
    invoke ExitProcess, 0

endp