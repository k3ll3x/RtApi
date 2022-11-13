// #include "TetraSand.h"
#include "Oscillator.h"
#include <thread>

int main(int argc, char** argv){
    // zmq::context_t ctx;
    // zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    // publisher.bind("tcp://localhost:5556");
    // return 0;

    Oscillator osc("tcp://*:5556");
    std::thread osc_th(&Oscillator::run, &osc);
    
    std::cout << "Oscillator Running...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    osc.stop();
    std::cout << "Stop Oscillator\n";
    osc_th.join();

    // const char * title = "Die Lawaaierige Planeet";
	// int w, h;

	// if(argc > 2){
	// 	w = atoi(argv[1]);
	// 	h = atoi(argv[2]);
	// }else{
    //     #ifdef __EMSCRIPTEN__
    //         w = windowGetWidth();
    //         h = windowGetHeight();
    //     #else
    //         w = 640;
    //         h = 480;
    //     #endif
	// }

    // SceneManager::start(title, w, h);
    return 0;
}