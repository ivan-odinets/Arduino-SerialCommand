Library for standartization of communication between arduino and other devices.<br>

* void addCommand(const char* name,void (*f)()) - register handler for command name. Must be called once for each command.<br>
* void handleChanges(Stream* stream = &Serial) - check if stream has new data, if yes - parse it and execute correct command. Must be called in loop() function.<br>
* char* next() - get data after command itself. Returns NULL if cmd line is completely parsed.<br>

Common responses.
* printError(const char* message) - prints message with error prefix.<br>
* printInfo(const char* message) - prints message with info prefix.<br>
* printResponse(const char* message) - prints message with response prefix.<br>
* printOk() - prints simple confirmation message.<br>

Responses are printed to stream from which command was recieved.
