test: test.cpp
	g++ -o test test.cpp neuron.cpp network.cpp datainput.cpp -W -Wall

all: test;

clean:
	rm -f test test.o neuron neuron.o network network.o datainput datainput.o
