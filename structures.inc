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