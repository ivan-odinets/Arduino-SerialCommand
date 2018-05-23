StreamCommand
=============
Library for standartization of communication between arduino and other devices.<br>
Tested with Serial and WiFiClient objects. In theory it should also work with all subclasses of Stream class.

Usage
=====

* void addCommand(const char* name,void (*f)()) - register handler for command "name". Must be called once for each command in setup() function.<br>
* void handleChanges(Stream* stream = &Serial) - check if stream has new data, if yes - parse it and execute correct command. Must be called in loop() function.<br>
* char* next() - get arguments for command. Returns NULL if cmd line is completely parsed. Should be called from command handlers<br>

Reponses with built-in prefixes.
* printError(const char* message) - prints message with error prefix.<br>
* printInfo(const char* message) - prints message with info prefix.<br>
* printResponse(const char* message) - prints message with response prefix.<br>
* printOk() - prints simple confirmation message.<br>

Moreover StreamCommand supports usual print() and println() functions.

Responses are printed to stream from which command was recieved. SO it is possible to setup common command line interface for Serial and Telnet communication.

Installation:
=============
* Download library from https://github.com/ivan-odinets/StreamCommand/archive/master.zip
* On your arduino ide go to Sketch -> Include Library -> Add .ZIP library and select previously downloaded file.
* Check File -> Examples – > StreamCommand-master for examples of usage
