SDL_FLAGS := $(shell pkg-config --cflags --libs sdl3 sdl3-image sdl3-ttf sdl3-mixer)

all:
	mkdir -p build
	g++ -g src/*.cpp -o build/main -I src/include/ $(SDL_FLAGS)

clean:
	rm -rf build/
	mkdir -p build
	g++ -g src/*.cpp -o build/main -I src/include/ $(SDL_FLAGS)
stable:
	mkdir -p build
	g++ -g src/ball.cpp src/math.cpp src/renderwindow.cpp src/texturemanager.cpp src/main.cpp -o build/main -I src/include/ $(SDL_FLAGS)
