turing: turing.cpp simulate.cpp
	g++ -w -o turing turing.cpp simulate.cpp

clean:
	rm -f turing