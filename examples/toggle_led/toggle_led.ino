#include <StreamCommand.h>

#define LED_PIN 13

StreamCommand cmd;

void setLedCmd()
{
  char* token = cmd.next();
  if (token == NULL) {
    cmd.printError("No argument error");
    return;
  }

  digitalWrite(LED_PIN,strcmp(token,"on") == 0);
  cmd.printOk();
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LOW);
  cmd.addCommand("setLed",setLedCmd);
}

void loop() {
  cmd.handleChanges(&Serial);
}
