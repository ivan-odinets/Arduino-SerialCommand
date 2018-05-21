/*
 * ----------------------------------------------------------------------------
 *
 *  This file is part of StreamCommand library
 * 
 *  StreamCommand library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  StreamCommand library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with StreamCommand library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef STREAMCOMMAND_H
#define STREAMCOMMAND_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <Print.h>
#ifdef ESP8266
  #include <functional>
#endif
#include "./Tokens.h"


#ifndef MAX_COMMANDS
  #define MAX_COMMANDS 10
#endif

#ifndef COMMANDBUFFER_SIZE
  #define COMMANDBUFFER_SIZE 128
#endif

#define MAXDELIMETER 2

#define SERIAL_DEBUG


class StreamCommand : public Print  {
public:
  #ifdef ESP8266
  typedef std::function<void()> Handler;
  #else
  typedef void (*Handler)();
  #endif
    
  StreamCommand(Stream* defaultStream = &Serial);
  void handleChanges(Stream* stream = p_defaultStream);
  void setDefaultStream(Stream* stream) {p_defaultStream = stream;}
  
  //Managing commands
  void addCommand(const char* name,Handler handler);
  void addDefaultHandler(Handler h) {p_defaultHandler = h;}

  void clearBuffer();
  char* next();
  
  //Responses
  void waitForUserInput();
  void printError(const char* message);
  void printInfo(const char* message);
  void printInfo(const String& message);
  void printResponse(const char* message);
  void printResponse(const Printable& p);
  void printResponse(const String& s);
  void printResponse(const char* prefix,const String& s);
  void printResponse(const char* prefix, const Printable& p);
  void printOk();
  
protected:
  //Managing commands
  typedef struct _callback {
    char command[32];
    Handler handler;
  } CommandHandler;
  Handler p_defaultHandler;
  CommandHandler m_commandList[MAX_COMMANDS];
  uint8_t m_numCommands;
  
  char m_buffer[COMMANDBUFFER_SIZE];   // Buffer of stored characters while waiting for terminator character
  uint8_t  m_bufPos;              // Current position in the buffer
  char m_delim[MAXDELIMETER];     // null-terminated list of character to be used as delimeters for tokenizing (default " ")
  char m_term;                    // Character that signals end of command (default '\r')
  char* p_token;                  // Returned token from the command buffer as returned by strtok_r
  char* p_last;                   // State variable used by strtok_r during processing
  
  Stream* p_currentStream;
  Stream* p_defaultStream;
  
  size_t write(uint8_t byte);
};

#endif //PC_H
