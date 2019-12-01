INC=-I./include

default: main
	
	
main: mkdir BestSchemasDict.o mig.o mig_generator.o permutation.o SearchMutation.o
	g++ --std=c++17 ${INC} -o ./build/MIG_generator ./src/main.cpp ./build/BestSchemasDict.o ./build/mig.o ./build/mig_generator.o ./build/permutation.o ./build/SearchMutation.o -lpthread
	
BestSchemasDict.o: mkdir
	g++ -c ${INC} -o ./build/BestSchemasDict.o ./src/BestSchemasDict.cpp 
	
mig.o: mkdir
	g++ -c ${INC} -o ./build/mig.o ./src/mig.cpp 
	
mig_generator.o: mkdir
	g++ -c ${INC} -o ./build/mig_generator.o ./src/mig_generator.cpp
	
permutation.o: mkdir
	g++ -c ${INC} -o ./build/permutation.o ./src/permutation.cpp

SearchMutation.o: mkdir
	g++ -c ${INC} -o ./build/SearchMutation.o ./src/SearchMutation.cpp
	
mkdir:
	mkdir -p build
	
clean:
	rm ./build/*.o ./build/MIG_generator
	
clear:
	rm ./build/*.o ./build/MIG_generator