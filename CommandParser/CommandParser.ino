#include <Arduino.h>

class CommandHandler {
private:
  struct Command {
    String name;
    void (*function)();
  };

  Command *commands;
  int commandCount;
  String inputString;

public:
  CommandHandler() : commands(nullptr), commandCount(0), inputString("") {}

  void addCommand(const String &name, void (*function)()) {
    Command *newCommands = new Command[commandCount + 1];
    for (int i = 0; i < commandCount; i++) {
      newCommands[i] = commands[i];
    }
    newCommands[commandCount].name = name;
    newCommands[commandCount].function = function;
    delete[] commands;
    commands = newCommands;
    commandCount++;
  }

  void executeCommand(const String &name) {
    for (int i = 0; i < commandCount; i++) {
      if (commands[i].name.equalsIgnoreCase(name)) {
        Serial.print("Executing command: ");
        Serial.println(name);
        (*commands[i].function)();
        return;
      }
    }
    Serial.print("Unknown command: ");
    Serial.println(name);
  }

  String readSerialCommand() {
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      if (inChar == '\n' || inChar == '\r') {
        if (inputString.length() > 0) {
          inputString.trim();
          String command = inputString;
          inputString = "";
          return command;
        }
      } else {
        inputString += inChar;
      }
    }
    return "";
  }

  ~CommandHandler() {
    delete[] commands;
  }
};

void turnLedOn() {
  Serial.println("LED is now ON");
  // Add code to turn LED on
}

void turnLedOff() {
  Serial.println("LED is now OFF");
  // Add code to turn LED off
}

void reportStatus() {
  Serial.println("Reporting status");
  // Add code to report status
}

void rmRf() {
  Serial.println("Removing Root Directory...");
  // Add code to delete root directory
}

CommandHandler commandHandler;

void setup() {
  Serial.begin(9600);
  commandHandler.addCommand("LED ON", &turnLedOn);
  commandHandler.addCommand("LED OFF", &turnLedOff);
  commandHandler.addCommand("STATUS", &reportStatus);
  commandHandler.addCommand("sudo rm -rf --no-preserve-root", &rmRf);
}

void loop() {
  if (Serial.available() > 0) {
    String command = commandHandler.readSerialCommand();
    if (command.length() > 0) {
      commandHandler.executeCommand(command);
    }
  }
}
