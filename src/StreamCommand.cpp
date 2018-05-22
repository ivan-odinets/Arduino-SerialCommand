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
 *  Part of the code is based on Arduino-SerialCommand from kroimon
 *  https://github.com/kroimon/Arduino-SerialCommand
 * 
 */

#include "StreamCommand.h"

StreamCommand::StreamCommand(Stream* defaultStream) :
 p_defaultHandler(0x00),m_numCommands(0),m_term('\r'),p_defaultStream(defaultStream)
{
  strncpy(m_delim," ",MAXDELIMETER);
  clearBuffer();
}

/*
 * ----------------------------------------------------------------------------
 *  Managing commands
 * 
 */

void StreamCommand::addCommand(const char* name,Handler handler)
{
  if (m_numCommands == MAX_COMMANDS) {
    #ifdef SERIAL_DEBUG
    Serial.print("[DEBUG] Max command count!");
    #endif
    return;
  }
  
  #ifdef SERIAL_DEBUG
  Serial.print("[DEBUG] addCommand(");Serial.print(name);Serial.println(");");
  #endif
  strcpy(m_commandList[m_numCommands].command,name);
  m_commandList[m_numCommands].handler = handler;
  m_numCommands++;
}
char* StreamCommand::next() 
{
  char *nextToken;
  nextToken = strtok_r(NULL, m_delim, &p_last); 
  return nextToken; 
}
void StreamCommand::clearBuffer()
{
  memset(m_buffer,'\0',COMMANDBUFFER_SIZE);
  m_bufPos = 0;
}
void StreamCommand::handleChanges(Stream* stream)
{
  p_currentStream = stream;
    
  char inChar;
  if (p_currentStream->available() > 0)
    delay(250);
  
  while (p_currentStream->available() > 0) {
    inChar = p_currentStream->read();
    if (isprint(inChar)) {
      m_buffer[m_bufPos++]=inChar;   // Put character into buffer
      if (m_bufPos > COMMANDBUFFER_SIZE-1) 
        break; // wrap buffer around if full
    }
  };
  
  if (strlen(m_buffer) == 0)
    return;

  #ifdef SERIAL_DEBUG
  Serial.print("[DEBUG] Received: ");Serial.println(m_buffer);
  #endif
  m_bufPos=0;           // Reset to start of buffer
  p_token = strtok_r(m_buffer,m_delim,&p_last);   // Search for command at start of buffer
  if (p_token == NULL) 
    return;
  
  for (uint8_t i=0; i<m_numCommands; i++) {
    #ifdef SERIAL_DEBUG
    Serial.print("[DEBUG] Comparing ["); 
    Serial.print(p_token); 
    Serial.print("] to [");
    Serial.print(m_commandList[i].command);
    Serial.println("]");
    #endif
    // Compare the found command against the list of known commands for a match
    if (strncmp(p_token,m_commandList[i].command,COMMANDBUFFER_SIZE) == 0) {
      #ifdef SERIAL_DEBUG
      Serial.print("[DEBUG] Matched Command: ");
      Serial.println(p_token);
      #endif
      // Execute the stored handler function for the command
      #ifdef ESP8266
      m_commandList[i].handler();
      #else
      (*m_commandList[i].handler)();
      #endif
      clearBuffer(); 
      return;
    }
  }

  if (p_defaultHandler)
    #ifdef ESP8266
    p_defaultHandler();
    #else
    (*p_defaultHandler)();
    #endif

  clearBuffer();
}

/*
 * ----------------------------------------------------------------------------
 * Printing responses
 * 
 */

void StreamCommand::waitForUserInput()
{
  while (p_currentStream->available() == 0) {}
  while (p_currentStream->available() != 0)
    p_currentStream->read();
}

void StreamCommand::printError(const char* message)
{
  print(ERROR_PREFIX);
  println(message);
}
void StreamCommand::printInfo(const char* message)
{
  print(INFO_PREFIX);
  println(message);
}
void StreamCommand::printInfo(const String& message)
{
  print(INFO_PREFIX);
  println(message);
}
void StreamCommand::printResponse(const char* message)
{
  print(OK_PREFIX);
  println(message);
}
void StreamCommand::printResponse(const String& message)
{
  print(OK_PREFIX);
  println(message);
}
void StreamCommand::printResponse(const Printable& p)
{
  print(OK_PREFIX);
  println(p);
}
void StreamCommand::printResponse(const char* prefix,const String& s)
{
  print(OK_PREFIX);
  print(prefix);println(s);
}
void StreamCommand::printResponse(const char* prefix,const Printable& p)
{
  print(OK_PREFIX);print(prefix);
  println(p);
}
void StreamCommand::printOk()
{
  println(OK_RESPONSE);
}

/*
 * ----------------------------------------------------------------------------
 * Write function.
 * 
 */

size_t StreamCommand::write(uint8_t byte)
{
  if (p_currentStream != NULL)
    return p_currentStream->write(byte);
  
  if (p_defaultStream != NULL)
    return p_defaultStream->write(byte);
  
  return -1;
}
