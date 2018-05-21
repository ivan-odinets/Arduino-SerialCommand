Library for standartization of communication between arduino and other devices<br>

void addCommand(const char* name,void (*f)()) - register handler f for command name. Must be called once.<br>
void handleChanges(Stream* stream = &Serial) - check if stream has new data, if yes - parse it and execute correct command. Must be called in loop() function.<br>
char* next() - get data after command itself. Returns NULL if cmd line is completely parsed.<br>

Common responses:
printError(const char* message) - prints message with error prefix<br>
printInfo(const char* message) - prints message with info prefix<br>
printResponse(const char* message) - prints message with response prefix<br>
printOk() - prints simple confirmation message<br>

If responses are called from registered handler, it will print data to stream. If no - it will print data to default stream.<br>
