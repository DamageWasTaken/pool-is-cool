all:
	g++ -g *.cpp -o main -I src/include/ `pkg-config --cflags --libs sdl3`

clean:
	rm -f main
	g++ -g *.cpp -o main -I src/include/ `pkg-config --cflags --libs sdl3`