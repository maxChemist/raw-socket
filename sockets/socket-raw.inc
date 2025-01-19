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
    invoke socket, AF_INET, SOCK_RAW, IPPROTO_ICMP
    test rax, rax
    js exit_program
    mov [rawSocketHandler], rax  ; Store socket handle

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
    invoke recvfrom, rbx, rdx, 1024, 0, 0, 0
    test eax, eax
    jle receive_data  ; If no data, continue receiving

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