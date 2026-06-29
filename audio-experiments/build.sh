# compile the code
arduino-cli compile \
    --output-dir build \
    --fqbn arduino:avr:mega \
    ${1}

# flash to board
avrdude \
    -p atmega2560 \
    -c wiring \
    -P /dev/ttyACM0 \
    -b 115200 \
    -D \
    -U flash:w:build/${1}.ino.hex:i