all: satb combine
	g++ exact_synthesis.cpp -o sat -lz3 -O3 --std=c++17
satb:
	g++ bunch_sat.cpp -o bunch_sat -O3 --std=c++17 -lstdc++fs -fopenmp
combine:
	g++ combine.cpp -o combine -O3 --std=c++17 -lstdc++fs
clean:
	rm -f sat bunch_sat combine
