all: sample

sample: sample.cpp pipeline.hpp parser.hpp simulator.hpp requirements.hpp
	g++ requirements.hpp pipeline.hpp parser.hpp simulator.hpp sample.cpp -o sample

clean:
	rm sample

run: sample
	./sample $(arg1)
