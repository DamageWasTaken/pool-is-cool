all:
	g++ -g *.cpp -o main -I src/include/ -lSDL3

clean:
	rm -f main
	g++ -g *.cpp -o main -I src/include/ -lSDL3