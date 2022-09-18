#include "TetraSand.h"

int main(int argc, char** argv){
    const char * title = "Die Lawaaierige Planeet";
	int w, h;

	if(argc > 2){
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	}else{
        #ifdef __EMSCRIPTEN__
            w = windowGetWidth();
            h = windowGetHeight();
        #else
            w = 640;
            h = 480;
        #endif
	}

    SceneManager::start(title, w, h);
    return 0;
}