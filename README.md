# GBC_Emulator_C

TODO:

Brain/Back-end:
* Ensure tick counting & timing is correct (M and T Clocks?) - Ticks differ in jump/ret opcodes
* Finish TODO CPU functions
* Have GPU fill array of data to send over websocket
* Begin reading instructions in CPU
* Handle Interrupts
* Update MBC3 code to save/load RAM files
* Write RAM switching for CGB

GUI:
* Create window with GTK 4.0 (Desktop application)
* Alternatively expose Gameboy brain over TCP websockets, implement GUI with Aurelia
* Test with React/Angular

Project Structure:
* Replace makefile with CMake
* Compile on Windows

Clean up code:
* Set uniform casing throughout code (camel case with upper case FunctionNames, lowercase variableNames)
* Update function declarations to have only data type

Nice to haves:
* Logging system