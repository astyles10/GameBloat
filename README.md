# GBC_Emulator_C

TODO:
* Fix compiler warnings regarding casting pointers to integers
* Replace pointers with copies in MMU read/write operations, makes code harder to read
* Write RAM switching for CGB
* Update use of CPU tick counting, should be internal to CPU only

Clean up code:

* Change #define to enums, currently mix of both littered in code
* Set uniform casing throughout code (camel case with upper case FunctionNames, lowercase variableNames)
