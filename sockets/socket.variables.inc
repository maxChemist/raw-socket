section '.data' data readable writeable
    wsadata db 512 dup(0)                   ; WSAData structure buffer
    wsaversion dw 0x0202                    ; Winsock version 2.2
    sock dd 0                               ; Socket descriptor
    protocol dd 1                           ; Protocol: IPPROTO_ICMP (1)
    errmsg db "Failed to create socket", 0  ; Error message
    successmsg db "Raw socket created!", 0  ; Success message
    rawSocketHandler dq 0                   ; Raw socket handler
    rawSocketThreadId dq 0
    rawSocketStructureIn SOCKADDR_IN
    rawSocketStructureInLength = $ - rawSocketStructureIn