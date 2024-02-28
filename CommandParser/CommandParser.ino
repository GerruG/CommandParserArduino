#include <Arduino.h>

// Class to handle commands received via Serial
class CommandHandler {
private:
  // Struct to store command name and corresponding function
  struct Command {
    String name;
    void (*function)();
  };

  Command *commands;  // Dynamic array to store commands
  int commandCount;   // Count of added commands
  String inputString; // Buffer to store incoming serial data

public:
  // Constructor initializes an empty command handler
  CommandHandler() : commands(nullptr), commandCount(0), inputString("") {}

  // Adds a new command to the dynamic array
  void addCommand(const String &name, void (*function)()) {
    Command *newCommands = new Command[commandCount + 1]; // Create new array
    for (int i = 0; i < commandCount; i++) { // Copy existing commands
      newCommands[i] = commands[i];
    }
    newCommands[commandCount].name = name; // Add new command
    newCommands[commandCount].function = function;
    delete[] commands; // Delete old array
    commands = newCommands; // Update pointer
    commandCount++; // Increment command count
  }

  // Executes a command if it is found in the list
  void executeCommand(const String &name) {
    for (int i = 0; i < commandCount; i++) {
      if (commands[i].name.equalsIgnoreCase(name)) {
        Serial.print("Executing command: ");
        Serial.println(name);
        (*commands[i].function)(); // Execute command function
        return;
      }
    }
    Serial.print("Unknown command: ");
    Serial.println(name);
  }

  // Reads incoming serial data and constructs command strings
  String readSerialCommand() {
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      if (inChar == '\n' || inChar == '\r') {
        if (inputString.length() > 0) {
          inputString.trim(); // Remove whitespace
          String command = inputString; // Save command
          inputString = ""; // Reset buffer
          return command;
        }
      } else {
        inputString += inChar; // Append character
      }
    }
    return ""; // Return empty if no command is complete
  }

  // Destructor
  ~CommandHandler() {
    delete[] commands;
  }
};

// Functions
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
  // Code to remove root directory
}

CommandHandler commandHandler;

void setup() {
  Serial.begin(9600); // baud
  // Commands
  commandHandler.addCommand("LED ON", &turnLedOn);
  commandHandler.addCommand("LED OFF", &turnLedOff);
  commandHandler.addCommand("STATUS", &reportStatus);
  commandHandler.addCommand("sudo rm -rf --no-preserve-root", &rmRf);
}

void loop() {
  // Check for and execute commands received via serial
  if (Serial.available() > 0) {
    String command = commandHandler.readSerialCommand();
    if (command.length() > 0) {
      commandHandler.executeCommand(command);
    }
  }
}
