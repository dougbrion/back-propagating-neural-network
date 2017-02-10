CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

bin/test: src/test.cpp src/neuron.cpp src/network.cpp src/datainput.cpp
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/test $^

bin/creation: src/creation
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/creation $^

bin/change_char: src/change_char
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/change_char $^

clean:
	rm bin/*
