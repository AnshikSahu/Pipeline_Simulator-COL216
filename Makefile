all: pipelinesimulator main.cpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp requirements.hpp 5stage.cpp 5stage_bypass.cpp 79stage.cpp 79stage_bypass.cpp

compile : main.cpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp requirements.hpp 5stage.cpp 5stage_bypass.cpp 79stage.cpp 79stage_bypass.cpp
	g++ requirements.hpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp main.cpp -o pipelinesimulator
	g++ requirements.hpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp 5stage.cpp -o pipelinesimulator5
	g++ requirements.hpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp 5stage_bypass.cpp -o pipelinesimulator5b
	g++ requirements.hpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp 79stage.cpp -o pipelinesimulator79
	g++ requirements.hpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp 79stage_bypass.cpp -o pipelinesimulator79b

clean:
	rm pipelinesimulator
	rm pipelinesimulator5
	rm pipelinesimulator5b
	rm pipelinesimulator79
	rm pipelinesimulator79b

run_5stage: pipelinesimulator5
	./pipelinesimulator5 1 input.asm

run_5stage_bypass: pipelinesimulator5b
	./pipelinesimulator5b 2 input.asm

run_79stage: pipelinesimulator79
	./pipelinesimulator79 3 input.asm

run_79stage_bypass: pipelinesimulator79b
	./pipelinesimulator79b 4 input.asm
	
main: pipelinesimulator
	./pipelinesimulator 1 input.asm
	./pipelinesimulator 2 input.asm
	./pipelinesimulator 3 input.asm
	./pipelinesimulator 4 input.asm
