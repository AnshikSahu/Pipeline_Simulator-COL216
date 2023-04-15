# COL216-Assignment2

### Files included in Submission:

1. requirements.hpp
2. pipeline.hpp
3. parser.hpp
4. simulator.hpp
5. configuration.hpp
5. branchpredictor.hpp
6. branchpredictertester.cpp
7. main.cpp
8. 5stage.cpp
9. 5stage_bypass.cpp
10. 79stage.cpp
11. 79stage_bypass.cpp
12. Makefile
13. README.md
14. report.pdf

**Active instructions: add, and, or, sub, mul, slt, sll, srl, bne, beq, j, addi, lw, sw.**

#### Instructions: write the MIPS assembly code in a file and save it as input.asm. Then use the following make commands to simulate a pipeline.

1. all - to update all dependencies
2. compile - to compile and generate the executable for all the 4 simulator parts
3. run_5stage - to run the simulator for the 5-stage pipeline
4. run_5stage_bypass - to run the simulator for the 5-stage pipeline with forwarding and bypassing
5. run_79stage - to run the simulator for the 7 and 9-stage stage pipeline
6. run_79stage_bypass - to run the simulator for the 7 and 9-stage stage pipeline with forwarding and bypassing

**Note:** The print_table() function of pipeline can be used to make the output more human readable.

#### Structures:

- **configuration.hpp:**
  1. Configuration=> Generates the configuration parameters for the simulator based on question number.
- **requirements.hpp:**
  1. Command=> Defines the structure of a command.
  2. QueueNode=> A node with three integer values.
  3. UpdateQueue=> A queue to keep track of update history in memory and register file.
  4. Registerfile=> Defines a register file and also takes over the functionality of latches.
  5. Runtimedata=> Stores the start and endtime of each stage of an instruction.
- **parser.hpp:**
  1. Parser=> Parses the input file and converts each instruction to an object of the Command struct according to configuration parameters.
- **pipeline.hpp:**
  1. Pipeline=> Defines the structure of a general pipeline. An object of it can be made by specifing the type of pipeline and can be used to run instructions.
- **simulator.hpp:**
  1. Simulator=> Simulates mips architectue pipeline according to the configurations.
- **simulator.hpp:**
  1. SaturatingBranchPredictor=> Makes predictions based on the history of that particular branch. 
  2. BHRBranchPredictor=> Makes predictions based on the history of the entire file. 
  3. SaturatingBHRBranchPredictor=> Predicts which of the above two strategies should be used for making predictions and then uses it for prediction. 
  
  
##### Other files:

- main.cpp, 5stage.cpp, 5stage_bypass.cpp, 79stage.cpp, 79stage_bypass.cpp : Are copies of the same file. Create an instance of Simulator.
- Makefile: To define the make commands.
- Readme.md: The readme file for the code. 

### Design Decesions:
  
- For first in first out, the stalls are inserted only before WB stage.
- There can be two WB stages in the same cycle for a 7-9 pipeline if one of them is non operational.
- For 7-9 stage pipeline, the stages in which the stalls occur are sometimes interchanged for simplicity of the design. This does not affect the overall pipeline.
- In 7-9 stage pipeline without byassing, regsiters can only be read at RR stage. 
- In 5 stage pipeline, the registers in Regsiter File can be read anywhere. 
- **There are no latches or cycles in the code and the stages are just represented as a block of delay and have no functionality of their own. The functionality of the code is executed separately from the pipeline.**
