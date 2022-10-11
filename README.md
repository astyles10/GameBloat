# GBC_Emulator_C

TODO:

Read these (unrelated):
http://inside.mines.edu/~coulston/courses/EENG383/lecture/lecture10.html
https://www.microcontrollertips.com/difference-between-von-neumann-and-harvard-architectures/
https://blog.acolyer.org/2016/04/26/the-linux-scheduler-a-decade-of-wasted-cores/

Compare my registers etc with https://github.com/DanB91/GBEmu

Current priority issues:
1. Finish remaning CPU functions
2. Using https://github.com/DanB91/GBEmu ensure registers and instructions are correct. Register A, F and E were incorrect on last debug session.

Found that at 324 CPU cycles, the correct instruction was 0xCB translating to RES 0,A but GameBloat executes SET 1,A

Could write first x instructions to a file to compare

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
* Catch SIGSEGV, SIGKILL, anything that unexpectedly kills the running process and save the current state
https://stackoverflow.com/questions/10202941/segmentation-fault-handling