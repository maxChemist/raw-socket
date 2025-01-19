section '.data' data readable writeable
  newline db 13,10
  startConsoleListenStruct START_CONSOLE_READ_THREAD
  startConsoleListenStructLength = $ - startConsoleListenStruct
  mainConsoleStdin dq ?
  mainConsoleStdout dq ?
  numberOfCharsWritten  dq ?
  mainConsoleStdinBuffer rb 256 
  mainConsoleStdinBufferSize = $ - mainConsoleStdinBuffer
  mainConsoleStdinPrompt db ':> ', 0
  mainConsoleStdinPromptLen = $ - mainConsoleStdinPrompt - 1



;   hStdOut         dq ?
;   written         dq ?
;   sock            dq ?
;   client          dq ?
;   sock_7001       dq ?    ; Socket for port 7001
;   sock_7002       dq ?    ; Socket for port 7002
;   hThread         dq ?
  thread_id       dq ?
;   buffer_size     = 256
;   buffer          db buffer_size dup(?)
;   message         db 'Console window is open.',13,10
;   message_length  = $ - message
;   listen_msg      db 'Listening on port 7123...',13,10
;   listen_len      = $ - listen_msg
;   ready_msg       db 'Ready to accept connections on port 7123...',13,10
;   ready_len      = $ - ready_msg
;   accept_msg      db 'Connection accepted!',13,10
;   accept_len     = $ - accept_msg
;   recv_msg        db 'Received data: '
;   recv_len        = $ - recv_msg
;   fwd_7001_msg    db 'Forwarded to port 7001',13,10
;   fwd_7001_len    = $ - fwd_7001_msg
;   fwd_7002_msg    db 'Forwarded to port 7002',13,10
;   fwd_7002_len    = $ - fwd_7002_msg
;   sockaddr        rb SOCKADDR_SIZE
;   sockaddr_7001   rb SOCKADDR_SIZE    ; Address structure for port 7001
;   sockaddr_7002   rb SOCKADDR_SIZE    ; Address structure for port 7002
;   ; Error handling messages
;   init_wsa_msg     db 'Initializing Winsock...',13,10
;   init_wsa_len     = $ - init_wsa_msg
;   create_sock_msg  db 'Creating socket...',13,10
;   create_sock_len  = $ - create_sock_msg
;   bind_msg        db 'Binding socket...',13,10
;   bind_len        = $ - bind_msg
;   listen_start_msg db 'Starting to listen...',13,10
;   listen_start_len = $ - listen_start_msg
;   success_msg     db 'Socket initialization successful!',13,10
;   success_len     = $ - success_msg
;   wsa_fail_msg    db 'WSAStartup failed! Error code: ',13,10
;   wsa_fail_len    = $ - wsa_fail_msg
;   socket_fail_msg db 'Socket creation failed! Error code: ',13,10
;   socket_fail_len = $ - socket_fail_msg
;   bind_fail_msg   db 'Bind failed! Error code: ',13,10
;   bind_fail_len   = $ - bind_fail_msg
;   listen_fail_msg db 'Listen failed! Error code: ',13,10
;   listen_fail_len = $ - listen_fail_msg
;   setsockopt_fail_msg db 'SetSockOpt failed! Error code: ',13,10
;   setsockopt_fail_len = $ - setsockopt_fail_msg
;   error_code_msg  db 'Error code: '
;   error_code_len  = $ - error_code_msg
;   error_buffer    db 16 dup(0)  ; Buffer for error code conversion
;   last_error      dd 0
  run_msg        db 'Proxy program started',0
  run_caption    db 'Runed',0

; ;  CreateSocket 
;   createSocketHandler             dq ?
;   createSocketPrintedHandler      rb 64
