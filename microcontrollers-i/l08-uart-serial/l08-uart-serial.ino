/* Lab 8A (sort of): read a character from the PC and echo it back. */

// int incoming_byte = 0;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (Serial.available() > 0) {
//     // Read the incoming byte
//     incoming_byte = Serial.read();

//     if (incoming_byte != 10) {
//       // Echo it back
//       Serial.print("Received: ");
//       Serial.println(incoming_byte, DEC); // prints the ASCII value of the character
//     }
//   }
// }


/* Lab 8B: Read user commands.

  Note that I'm using String here for simplicity, but I understand that this dynamically allocates
  data. I think a more efficient version would be a fixed-size character buffer. Then we could
  use C string function like "strcmp(buffer, "LED ON") == 0" to compare the input.
*/
// constexpr uint8_t PIN_LED = 13;
// bool led_on = false;


// void print_status() {
//   if (led_on) {
//     Serial.println("Status of LED: ON");
//   }
//   else {
//     Serial.println("Status of LED: OFF");
//   }
// }

// void turn_on() {
//   if (led_on) {
//     Serial.println("LED is already ON.");
//   } else {
//     Serial.println("Turning on the LED");
//     led_on = true;
//     digitalWrite(PIN_LED, HIGH);
//   }
// }
// void turn_off() {
//   if (!led_on) {
//     Serial.println("LED is already OFF.");
//   } else {
//     Serial.println("Turning off the LED");
//     led_on = false;
//     digitalWrite(PIN_LED, LOW);
//   }
// }


// void setup() {
//   pinMode(PIN_LED, OUTPUT);
//   digitalWrite(PIN_LED, LOW);

//   Serial.begin(9600);
//   Serial.println("Enter data:");
// }

// void loop() {
//   // while (Serial.available() == 0) {}  // do nothing
//   // This is from the Arduino docs, but it's blocking and stops the loop from
//   // continuing to iterate! So I replaced it with an if-block
//   if (Serial.available() > 0) {
//     String teststr = Serial.readString();
//     teststr.trim(); // remove trailing whitespace
    
//     if (teststr == "LED ON") {
//       turn_on();
//     } else if (teststr == "LED OFF") {
//       turn_off();
//     } else if (teststr == "STATUS") {
//       print_status();
//     }

//     Serial.println("Enter data:");
//   }
// }

/* Lab 8C: Serial configuration console.

The user should be able to inspect and modify a device's configuration.
This is basically an extension of the previous lab, but I will introduce
configuration (parameters) that the user can adjust. To keep things focused,
I'll stick with the simple on-board LED. It will have ON, OFF, and BLINK states.
The user can control which state the LED is in, as well as the blinking rate if
the LED is in the blink state.
*/

// constexpr uint8_t LED_PIN = 13;
// enum class LedMode {
//   ON,
//   OFF,
//   BLINK
// };

// struct DeviceState {
//   LedMode led_mode;
//   long blink_rate;
// };

// float blink_rate_to_half_period_ms(long blink_rate) {
//   /* Convert a blink rate in Hz to a half-period in ms. */
//   return 0.5 * 1000 / (float)blink_rate;
// }

// DeviceState device_state = { LedMode::OFF, 10 };

// bool led_blink_state = false; // true = ON, false = OFF
// unsigned long time_last_flip_ms;
// unsigned long now;
// unsigned long blink_half_period_ms;

// void setup() {
//   Serial.begin(9600);

//   pinMode(LED_PIN, OUTPUT);
//   digitalWrite(LED_PIN, LOW);
// }

// void print_status() {
//   switch (device_state.led_mode) {
//     case LedMode::OFF:
//       Serial.println("LED is OFF");
//       break;
//     case LedMode::ON:
//       Serial.println("LED is ON");
//       break;
//     case LedMode::BLINK:
//       Serial.println("LED is BLINKING");
//       break;
//   }
// }

// void turn_on() {
//   if (device_state.led_mode == LedMode::ON) {
//     Serial.println("LED is already ON.");
//   } else {
//     Serial.println("Turning on LED");
//     device_state.led_mode = LedMode::ON;
//     digitalWrite(LED_PIN, HIGH);
//   }
// }
// void turn_off() {
//   if (device_state.led_mode == LedMode::OFF) {
//     Serial.println("LED is already OFF.");
//   } else {
//     Serial.println("Turning off LED");
//     device_state.led_mode = LedMode::OFF;
//     digitalWrite(LED_PIN, LOW);
//   }
// }

// void turn_blink() {
//   if (device_state.led_mode == LedMode::BLINK) {
//     Serial.println("LED is already BLINKING.");
//   } else {
//     Serial.println("Blinking the LED");
//     device_state.led_mode = LedMode::BLINK;
//     // initialize the blinking variables
//     time_last_flip_ms = millis();
//     led_blink_state = true;

//     // pre-calculate the blink half-period
//     blink_half_period_ms = blink_rate_to_half_period_ms(device_state.blink_rate);
//   }
// }

// void blink() {
//   if (led_blink_state) {
//     digitalWrite(LED_PIN, HIGH);
//   } else {
//     digitalWrite(LED_PIN, LOW);
//   }
// }

// void get_rate() {
//   if (device_state.led_mode != LedMode::BLINK) {
//     Serial.println("Error: LED is not in BLINK mode.");
//   } else {
//     Serial.print("Blink rate is ");
//     Serial.print(device_state.blink_rate);
//     Serial.println(" blinks per second.");
//     Serial.print("Blink half-period is ");
//     Serial.print(blink_half_period_ms);
//     Serial.println(" ms.");
//   }
// }

// void set_rate(String rate_str) {
//   long rate = rate_str.toInt();
//   device_state.blink_rate = rate;

//   // update the blink half-period
//   blink_half_period_ms = blink_rate_to_half_period_ms(device_state.blink_rate);
// }

// void loop() {
//   /* This is very inelegant, but it works.
//     Does (1) I/O parsing, (2) dispatch to command functions, and (3) state mutation (via those functions).
//   */
//   if (Serial.available() > 0) {
//     String cmd = Serial.readString();
//     cmd.trim(); // remove trailing whitespace

//     // Switch through available commands
//     if (cmd == "STATUS") print_status();
//     else if (cmd == "SET MODE ON") turn_on();
//     else if (cmd == "SET MODE OFF") turn_off();
//     else if (cmd == "SET MODE BLINK") turn_blink();
//     else if (cmd == "GET RATE") get_rate();
//     else if (cmd.startsWith("SET RATE ")) set_rate(cmd.substring(9));
//   }

//   // If we are in blink mode, make sure we're blinking
//   if (device_state.led_mode == LedMode::BLINK) {
//     now = millis();
//     if (now - time_last_flip_ms > blink_half_period_ms) {
//       time_last_flip_ms = now;
//       led_blink_state = !led_blink_state;
//       blink();
//     }
//   }
// }


/* Lab 8C: Serial configuration console, redux.

I want to practice C/C++ programming by making the code a bit more elegant.
The loop() function should extract a specific command --- with arguments ---
from the user input. Then it should dispatch one of the command handers,
and I think these should follow the same interface.
*/

#include <stdlib.h> // for atol
constexpr uint8_t LED_PIN = 13;
enum class LedMode {
  ON,
  OFF,
  BLINK
};

struct DeviceState {
  LedMode led_mode;
  long blink_rate;
};

enum class CommandType {
  STATUS,
  GET_RATE,
  SET_RATE,
  SET_MODE_ON,
  SET_MODE_OFF,
  SET_MODE_BLINK
};

// Struct to represent user input commands
struct UserCommand {
  const CommandType cmd_type;
  bool has_arg;
  long arg; // just assume it's an int, for simplicity
};

// Struct to represent possible commands and handlers
using CommandHandler = void (*)(const UserCommand&);
struct CommandEntry {
  const CommandType cmd_type;
  CommandHandler handler; // handler is a pointer to a function that takes a const UserCommand& and returns void
};

void print_status(const UserCommand& command);
void get_rate(const UserCommand& command);
void set_rate(const UserCommand& command);
void turn_on(const UserCommand& command);
void turn_off(const UserCommand& command);
void turn_blink(const UserCommand& command);

CommandEntry command_table[] = {
  {CommandType::STATUS, print_status}, // don't need to dereference!
  {CommandType::GET_RATE, get_rate},
  {CommandType::SET_RATE, set_rate},
  {CommandType::SET_MODE_ON, turn_on},
  {CommandType::SET_MODE_OFF, turn_off},
  {CommandType::SET_MODE_BLINK, turn_blink}
};

float blink_rate_to_half_period_ms(long blink_rate) {
  /* Convert a blink rate in Hz to a half-period in ms. */
  return 0.5 * 1000 / (float)blink_rate;
}

DeviceState device_state = { LedMode::OFF, 10 };

bool led_blink_state = false; // true = ON, false = OFF
unsigned long time_last_flip_ms;
unsigned long now;
unsigned long blink_half_period_ms;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void print_status(const UserCommand& command) {
  switch (device_state.led_mode) {
    case LedMode::OFF:
      Serial.println("LED is OFF");
      break;
    case LedMode::ON:
      Serial.println("LED is ON");
      break;
    case LedMode::BLINK:
      Serial.println("LED is BLINKING");
      break;
  }
}

void turn_on(const UserCommand& command) {
  if (device_state.led_mode == LedMode::ON) {
    Serial.println("LED is already ON.");
  } else {
    Serial.println("Turning on LED");
    device_state.led_mode = LedMode::ON;
    digitalWrite(LED_PIN, HIGH);
  }
}
void turn_off(const UserCommand& command) {
  if (device_state.led_mode == LedMode::OFF) {
    Serial.println("LED is already OFF.");
  } else {
    Serial.println("Turning off LED");
    device_state.led_mode = LedMode::OFF;
    digitalWrite(LED_PIN, LOW);
  }
}

void turn_blink(const UserCommand& command) {
  if (device_state.led_mode == LedMode::BLINK) {
    Serial.println("LED is already BLINKING.");
  } else {
    Serial.println("Blinking the LED");
    device_state.led_mode = LedMode::BLINK;
    // initialize the blinking variables
    time_last_flip_ms = millis();
    led_blink_state = true;

    // pre-calculate the blink half-period
    blink_half_period_ms = blink_rate_to_half_period_ms(device_state.blink_rate);
  }
}

void blink() {
  if (led_blink_state) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void get_rate(const UserCommand& command) {
  if (device_state.led_mode != LedMode::BLINK) {
    Serial.println("Error: LED is not in BLINK mode.");
  } else {
    Serial.print("Blink rate is ");
    Serial.print(device_state.blink_rate);
    Serial.println(" blinks per second.");
    Serial.print("Blink half-period is ");
    Serial.print(blink_half_period_ms);
    Serial.println(" ms.");
  }
}

void set_rate(const UserCommand& command) {
  device_state.blink_rate = command.arg;

  // update the blink half-period
  blink_half_period_ms = blink_rate_to_half_period_ms(device_state.blink_rate);
}


UserCommand parse_user_command(String cmd_str) {
  cmd_str.trim(); // remove trailing whitespace
  if (cmd_str == "STATUS") {
    return UserCommand{CommandType::STATUS, false, 0};
  } else if (cmd_str == "SET MODE ON") {
    return UserCommand{CommandType::SET_MODE_ON, false, 0};
  } else if (cmd_str == "SET MODE OFF") {
    return UserCommand{CommandType::SET_MODE_OFF, false, 0};
  } else if (cmd_str == "SET MODE BLINK") {
    return UserCommand{CommandType::SET_MODE_BLINK, false, 0};
  } else if (cmd_str == "GET RATE") {
    return UserCommand{CommandType::GET_RATE, false, 0};
  } else if (cmd_str.startsWith("SET RATE ")) {
    return UserCommand{CommandType::SET_RATE, true, atol(cmd_str.substring(9).c_str())};
  }
}

void dispatch_command(UserCommand cmd) {
  /* Given a user command and any arguments, invoke the correct handler. */
  for (int i=0; i< sizeof(command_table)/sizeof(command_table[0]); i++) {
    CommandEntry cmd_table_entry = command_table[i];

    if (cmd.cmd_type == cmd_table_entry.cmd_type) {
      // Invoke the handler
      cmd_table_entry.handler(cmd);
    }
  }
}

void loop() {
  /* This is very inelegant, but it works. Does
  
  (1) Parses input into a UserCommand object.
  (2) Dispatches to the appropriate command handler.
  (3) Updates blinking variables if in BLINK mode.
  */
  if (Serial.available() > 0) {  
    UserCommand cmd = parse_user_command(Serial.readString());
    dispatch_command(cmd);
  }

  // If we are in blink mode, make sure we're blinking
  if (device_state.led_mode == LedMode::BLINK) {
    now = millis();
    if (now - time_last_flip_ms > blink_half_period_ms) {
      time_last_flip_ms = now;
      led_blink_state = !led_blink_state;
      blink();
    }
  }
}
