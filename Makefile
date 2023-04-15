all: pipelinesimulator main.cpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp requirements.hpp

compile : main.cpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp requirements.hpp
	g++ requirements.hpp pipeline.hpp parser.hpp simulator.hpp configuration.hpp main.cpp -o pipelinesimulator

clean:
	rm pipelinesimulator

run_5stage: pipelinesimulator
	./pipelinesimulator 1 input.asm

run_5stage_bypass: pipelinesimulator
	./pipelinesimulator 2 input.asm

run_79stage: pipelinesimulator
	./pipelinesimulator 3 input.asm

run_79stage_bypass: pipelinesimulator
	./pipelinesimulator 4 input.asm

