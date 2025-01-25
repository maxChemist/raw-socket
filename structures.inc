struc START_CONSOLE_READ_THREAD
{
  .stdin dq ?
  .stdout dq ?
  .handler_address dq ?
  .buffer dq ?
  .buffer_size dq ?
  .prompt dq ?  
  .prompt_length dq ?  
}

struc SOCKADDR_IN
{
  .sin_family dw ?
  .sin_port dw ?
  .sin_addr dd ?
  .sin_zero db 8 dup(?)
}