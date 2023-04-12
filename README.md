# COL216-Assignment2

## Files included in Submission:

1. requirements.hpp
2. pipeline.hpp
3. parser.hpp
4. simulator.hpp
5. main.cpp
6. makefile
7. README.md

### To run testcases enter "make test" command.

1. **requirements.hpp: Describes structures "Command"(methods= "print_command"), "Runtimedata"(methods= "print_runtimedata") and "Registerfile"(methods= "copy_vector", "copy_file").**
2. **pipeline.hpp: Describes structure "Pipeline"(methods= "run_command", "save", "restore", "insert_halt", "print_pipeline").**
3. **parser.hpp: Describes structure "Parser"(methods= "parseCommand", "constructCommands").**
4. **simulator.hpp: Describes structure "Simulator"(methods= "add", "sub", "mul", "op", "beq", "bne", "bOP", "slt", "j", "lw", "sw", "locateAddress", "addi", "checkLabel", "checkRegister", "checkRegisters", "handleExit", "executeCommands", "printRegistersAndMemoryDelta").**
5. **main.cpp: functions for each question.**
