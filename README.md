# CS334: Module 5

## Development

The codebase for this project is written for the Arduino IDE, using its Sketchbook-based setup.

### 1. Installing ESP32 Board Manager

Before beginning, make sure you have the ESP32 Board Manager addon by adding the following url too your additional board manager URLs:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Then, install the `esp32` board from `Espressif` as a board manager. You can now begin working on the Arduino Code.

### 2. Cloning the repository

Clone the repository somewhere on your machine with the following:

```bash
$ git clone git@github.com:sienacizdziel/CS334-Module5.git
```

### 2. Locating the Sketchbook

To allow the Arduino IDE to access all the files correctly, you'll have to open the `module5/` directory (where the `.ino` file is located) as a Sketchbook. In the Arduino IDE preferences, change your `Sketchbook location` to point to the `module5/` folder, wherever you have cloned it on your machine.

You should now be fully setup to be able to compile the code and flash your board.
