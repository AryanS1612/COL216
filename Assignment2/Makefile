all: sample

sample: sample.cpp MIPS_Processor.hpp
	g++-12 -I /opt/homebrew/Cellar/boost/1.81.0_1/include sample.cpp MIPS_Processor.hpp -o sample

debug: 5stage.cpp MIPS_Pipelining.hpp
	g++-12 -g -I /opt/homebrew/Cellar/boost/1.81.0_1/include 5stage.cpp MIPS_Pipelining.hpp -o 5stage.o
	gdb 5stage.o

clean:
	rm sample debug
