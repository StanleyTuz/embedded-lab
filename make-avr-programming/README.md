Following the book "Make: AVR Programming" by Elliot Williams.

The book assumes that I am directly programming an AVR microprocessor chip, but I only have an Arduino. This isn't a real problem --- I can just use the Arduino IDE and write "AVR C" instead of relying on the Arduino API. I will still be writing my code using `.ino` files in order to accommodate the Arduino's toolchain.

Note: the Arduino and AVR are under
`C:\Users\<me>\AppData\Local\Arduino15\` so I'm adding it to `.vscode/c_cpp_properties.json` 

Once the Arduino CLI is installed and on the system PATH, I should be able to do 
```
C/C++ file -> avr-gcc (via arduino-cli) -> upload via bootloader
```

Try this:
```bash
arduino-cli compile --fqbn arduino:avr:mega my_project.ino
arduino-cli upload -p COM3 --fqbn arduino:avr:mega my_project.ino
```