INC=-I./include
LIBS=-lGLEW -lglfw -lGL -lm -lX11 -std=c++17
# LIBS=-std=c++11 -s WASM=1 -s FULL_ES2=1
# FLAGS=-D__EMSCRIPTEN__
FLAGS=-D__EMSCRIPTEN__ -s WASM=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s EXPORT_ALL=1 -s OFFSCREEN_FRAMEBUFFER=1 -s ASSERTIONS=1 -s USE_WEBGL2=1 -s USE_GLFW=3
all:
	mkdir -p generated
	mkdir -p build
	cp index.html build/
	emcc -lembind -o ./build/out.js ./src/main.cc $(INC) $(LIBS) $(FLAGS)
nat:
	g++ ./src/main.cc $(INC) $(LIBS) -o ./build/native.out
clean:
	rm -fr build/*
	rm -fr generated/*
	rm -fr sink/*
