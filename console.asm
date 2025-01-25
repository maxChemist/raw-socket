format PE64 GUI
entry start

include 'fasmw17332/INCLUDE/win64a.inc'

  include 'constants.inc'
  include 'structures.inc'
section '.text' code readable executable

proc start uses rbp rsi rdi
  sub     rsp, 32
  
  ; Show startup message box
  invoke  MessageBoxA, 0, run_msg, run_caption, MB_OK

  ; ; Allocate console
  mov     rcx, mainConsoleStdout
  mov     rdx, mainConsoleStdin
  call    ConsoleAlloc
  
  ; prepeare startConsoleListenStruct for ReadConsoleInputTread
  push [mainConsoleStdin]
  pop qword [startConsoleListenStruct.stdin]

  push [mainConsoleStdout]
  pop qword [startConsoleListenStruct.stdout]

  lea rax, [MenuHandler]
  mov     [startConsoleListenStruct.handler_address], rax

  lea rax, [mainConsoleStdinBuffer]
  mov     [startConsoleListenStruct.buffer], rax

  mov rax, mainConsoleStdinBufferSize
  mov     [startConsoleListenStruct.buffer_size], rax

  lea rax, [mainConsoleStdinPrompt]
  mov     [startConsoleListenStruct.prompt], rax

  mov rax, mainConsoleStdinPromptLen
  mov     [startConsoleListenStruct.prompt_length], rax

  call    ReadConsoleInputTread

.console_loop:
        invoke  Sleep, 1000
        jmp     .console_loop

.exit_prog:
        add     rsp, 32
        xor     eax, eax
        ret
endp


; Socket handling thread
; proc socket_thread uses rbx rsi rdi
;         sub     rsp, 32

;         ; Start socket on port 7123
;         mov     rcx, PORT_NUM
;         stdcall CreateSocket
;         test    rax, rax
;         jz      .thread_exit

;         ; Prepare sockaddr structures for later use
;         mov     word [sockaddr_7001], AF_INET
;         mov     word [sockaddr_7001+2], 0x1B1B     ; Port 7001 in network byte order
;         mov     dword [sockaddr_7001+4], 0x0100007F ; 127.0.0.1
;         mov     qword [sockaddr_7001+8], 0

;         mov     word [sockaddr_7002], AF_INET
;         mov     word [sockaddr_7002+2], 0x1C1B     ; Port 7002 in network byte order
;         mov     dword [sockaddr_7002+4], 0x0100007F ; 127.0.0.1
;         mov     qword [sockaddr_7002+8], 0

; .accept_loop:
;         ; Print ready message
;         invoke  WriteConsoleA, [hStdOut], ready_msg, ready_len, written, 0

;         ; Accept connection
;         invoke  accept, [sock], 0, 0
;         mov     [client], rax
;         cmp     rax, INVALID_SOCKET
;         je      .cleanup_socket

;         ; Print connection accepted message
;         invoke  WriteConsoleA, [hStdOut], accept_msg, accept_len, written, 0

;         ; Receive data first to ensure there's something to forward
;         invoke  recv, [client], buffer, buffer_size-1, 0
;         test    eax, eax
;         jle     .close_client

;         ; Store received size
;         mov     ebx, eax

;         ; Print received data
;         invoke  WriteConsoleA, [hStdOut], recv_msg, recv_len, written, 0
;         invoke  WriteConsoleA, [hStdOut], buffer, rbx, written, 0
;         invoke  WriteConsoleA, [hStdOut], newline, 2, written, 0

;         ; Now create and connect sockets for forwarding
;         invoke  socket, AF_INET, SOCK_STREAM, IPPROTO_TCP
;         mov     [sock_7001], rax
;         cmp     rax, INVALID_SOCKET
;         je      .close_client

;         invoke  socket, AF_INET, SOCK_STREAM, IPPROTO_TCP
;         mov     [sock_7002], rax
;         cmp     rax, INVALID_SOCKET
;         je      .close_7001

;         ; Try connect to port 7001
;         invoke  connect, [sock_7001], sockaddr_7001, SOCKADDR_SIZE
;         test    eax, eax
;         jnz     .close_7002

;         ; Try connect to port 7002
;         invoke  connect, [sock_7002], sockaddr_7002, SOCKADDR_SIZE
;         test    eax, eax
;         jnz     .close_7002

;         ; Forward to port 7001
;         invoke  send, [sock_7001], buffer, ebx, 0
;         invoke  WriteConsoleA, [hStdOut], fwd_7001_msg, fwd_7001_len, written, 0

;         ; Forward to port 7002
;         invoke  send, [sock_7002], buffer, ebx, 0
;         invoke  WriteConsoleA, [hStdOut], fwd_7002_msg, fwd_7002_len, written, 0

; .close_7002:
;         invoke  closesocket, [sock_7002]
; .close_7001:
;         invoke  closesocket, [sock_7001]
; .close_client:
;         invoke  closesocket, [client]
;         jmp     .accept_loop

; .cleanup_socket:
;         invoke  closesocket, [sock]
;         invoke  WSACleanup
; .thread_exit:
;         add     rsp, 32
;         xor     eax, eax
;         ret
; endp

  include 'main-console-processng/menu-handler.inc'
  include 'string-tools/compareString.inc'
  include 'console/console-alloc.inc'
  include 'console/console-print.inc'
  include 'console/console-print-line.inc'
  include 'console/console-read-input-thread.inc'
  include 'sockets/create-socket.inc'
  include 'sockets/socket-raw.inc'
  include 'converter/convertQwordToA.inc'

  ;****************************************
  include 'variables.inc'
  include 'main-console-processng/menu-variables.inc'
  include 'sockets/socket.variables.inc'

section '.idata' import data readable writeable
        library kernel32,'KERNEL32.DLL',\
                ws2_32,'WS2_32.DLL',\
                user32,'USER32.DLL'
        import  kernel32,\
                AllocConsole,'AllocConsole',\
                GetStdHandle,'GetStdHandle',\
                SetConsoleMode,'SetConsoleMode',\
                ReadConsoleA,'ReadConsoleA',\
                WriteConsoleA,'WriteConsoleA',\
                CreateThread,'CreateThread',\
                Sleep,'Sleep',\
                ExitProcess,'ExitProcess'
        import  ws2_32,\
                WSAStartup,'WSAStartup',\
                WSACleanup,'WSACleanup',\
                socket,'socket',\
                bind,'bind',\
                listen,'listen',\
                accept,'accept',\
                recv,'recv',\
                recvfrom,'recvfrom',\
                send,'send',\
                connect,'connect',\
                closesocket,'closesocket',\
                WSAGetLastError,'WSAGetLastError',\
                setsockopt,'setsockopt'
        import  user32,\
                MessageBoxA,'MessageBoxA'

section '.edata' export data readable
        export 'console.exe',\
               start,'start',\
               MenuHandler,'MenuHandler',\
               SocketRaw,'SocketRaw',\
               ReadConsoleCycle,'ReadConsoleCycle'
