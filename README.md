# COL216-Assignment2

## Files included in Submission:

1. requirements.hpp
2. pipeline.hpp
3. parser.hpp
4. simulator.hpp
5. configuration.hpp
5. branchpredictor.hpp
6. branchpredictertester.cpp
7. 5stage.cpp
8. 5stage_bypass.cpp
9. 79stage.cpp
10. 79stage_bypass.cpp
11. Makefile
12. README.md

#### Instructions: write the MIPS assembly code in a file and save it as input.asm. Then use the following make commands to simulate a pipeline.

1. all - to update all dependencies
2. compile - to compile and generate the executable for all the 4 simulator parts
3. run_5stage - to run the simulator for the 5-stage pipeline
4. run_5stage_bypass - to run the simulator for the 5-stage pipeline with forwarding and bypassing
5. run_79stage - to run the simulator for the 7 and 9-stage stage pipeline
6. run_79stage_bypass - to run the simulator for the 7 and 9-stage stage pipeline with forwarding and bypassing

####Structures:

- **configuration.hpp:**
  1.Configuration=> Generates the configuration parameters for the simulator based on question number.
- **requirements.hpp:**
  1. Command=> Defines the structure of a command.
  2. QueueNode=> A node with three integer values.
  3. UpdateQueue=> A queue to keep track of update history in memory and register file.
  4. Registerfile=> Defines a register file and also takes over the functionality of latches.
  5. Runtimedata=> Stores the start and endtime of each stage of an instruction.
  
  
