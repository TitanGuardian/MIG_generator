all: satb
	g++ exact_synthesis.cpp -o sat -lz3 -O3 --std=c++17
satb:
	g++ bunch_sat.cpp -o bunch_sat -O3 --std=c++17 -fopenmp
clean:
	rm -f sat bunch_sat 
