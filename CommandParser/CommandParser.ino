class CommandHandler {
private:
  struct Command {
    String name;
    void (*function)();
  };

  Command *commands;
  int commandCount;

public:
  CommandHandler() : commands(nullptr), commandCount(0) {}

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

  ~CommandHandler() {
    delete[] commands;
  }
};

// Example functions for commands
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
void rmRf(){
  Serial.println("Removing Root Directory...");
  // Add code to delete root directory
}

// Instantiate the command handler
CommandHandler commandHandler;

void setup() {
  Serial.begin(9600);
  // Register commands with the command handler
  commandHandler.addCommand("LED ON", &turnLedOn);
  commandHandler.addCommand("LED OFF", &turnLedOff);
  commandHandler.addCommand("STATUS", &reportStatus);
  commandHandler.addCommand("sudo rm -rf --no-preserve-root", &rmRf);
}

void loop() {
  if (Serial.available() > 0) {
    String command = readSerialCommand();
    if (command.length() > 0) {
      commandHandler.executeCommand(command);
    }
  }
}

String readSerialCommand() {
  static String inputString = ""; // 'static' keeps the variable value between function calls
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') { // Also check for carriage return for robustness
      if (inputString.length() > 0) {
        inputString.trim(); // Trim the inputString in place
        String command = inputString; // Copy the trimmed command
        inputString = ""; // Clear the inputString for new input
        return command; // Return the trimmed command we just read
      }
    } else {
      inputString += inChar; // Append the read character to our string
    }
  }
  return ""; // If no newline character, return an empty string
}
