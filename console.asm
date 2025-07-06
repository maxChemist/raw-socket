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
                setsockopt,'setsockopt',\
                htons,'htons',\
                htonl,'htonl'
        import  user32,\
                MessageBoxA,'MessageBoxA'

section '.edata' export data readable
        export 'console.exe',\
               start,'start',\
               MenuHandler,'MenuHandler',\
               SocketRaw,'SocketRaw',\
               ReadConsoleCycle,'ReadConsoleCycle'
