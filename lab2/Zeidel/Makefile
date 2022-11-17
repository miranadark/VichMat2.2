all: main.o
	@g++ object/main.o -o bin/main
	@./bin/main
main.o: main.cpp
	@g++ main.cpp -c -o object/main.o

clear:
	@rm -f bin/* object/*