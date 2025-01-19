section '.data' data readable writeable
  helpCmd db '\help',0
  helpCmdLength = $ - helpCmd - 1
  rawSocketCmd db '\raw-socket', 0
  rawSocketCmdLength = $ - rawSocketCmd - 1

  helpList db '\help - Displays this message', 13, 10, \
  '\help - Displays this message.', 13, 10, \
  '\exit - Exits the console.', 13, 10, 0
  helpListLength = $ - helpList - 1
