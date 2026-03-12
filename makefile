SDL_FLAGS := $(shell pkg-config --cflags --libs sdl3)

all:
	mkdir -p build
	g++ -g *.cpp -o build/main -I src/include/ $(SDL_FLAGS)

clean:
	rm -rf build/
	mkdir -p build
	g++ -g *.cpp -o build/main -I src/include/ $(SDL_FLAGS)