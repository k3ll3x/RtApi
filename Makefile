INC=-I/src/include
# LIBS=-lGLESv2 -lEGL -lm -lX11
LIBS=-lGLEW -lglfw -lGL -lm -lX11 -std=c++17
# LIBS=-lGL -lGLU -lglut
# FLAGS=-s WASM=1 -s LEGACY_GL_EMULATION=1 -s EXPORT_ALL=1 -s OFFSCREEN_FRAMEBUFFER=1
# -s USE_WEBGL2=1
FLAGS=-DEMSCRIPTEN -s WASM=1 -s EXPORT_ALL=1 -s OFFSCREEN_FRAMEBUFFER=1 -s ASSERTIONS=1 -s USE_GLFW=3
all:
	mkdir -p generated
	mkdir -p build
	cp index.html build/
	emcc -lembind -o /src/build/out.js /src/src/main.cc $(INC) $(LIBS) $(FLAGS)
	# emcc /src/src/main.cc $(LIBS) $(FLAGS) -O3 -o /src/build/out.js
nat:
	g++ /src/src/main.cc $(INC) $(LIBS) -o /src/build/native.out
clean:
	rm -fr build/*
	rm -fr generated/*
