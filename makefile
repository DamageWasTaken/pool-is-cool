SDL_FLAGS := $(shell pkg-config --cflags --libs sdl3 sdl3-image sdl3-mixer sdl3-ttf)

all:
	mkdir -p build
	g++ -g src/*.cpp -o build/main -I src/include/ $(SDL_FLAGS)

clean:
	rm -rf build/
	mkdir -p build
	g++ -g src/*.cpp -o build/main -I src/include/ $(SDL_FLAGS)