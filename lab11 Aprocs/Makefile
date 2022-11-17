all: main.o
	@g++ object/main.o -o bin/main -lsfml-graphics -lsfml-window -lsfml-system
	@./bin/main
main.o: main.cpp
	@g++ main.cpp -c -o object/main.o

clear:
	@rm -f bin/* object/*