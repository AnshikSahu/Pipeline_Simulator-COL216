test:
	g++ -std=c++11 requirements.hpp pipeline.hpp parser.hpp test.cpp -o test.o
	chmod 777 test.o
	./test.o