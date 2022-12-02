// #include "TetraSand.h"
#include "Oscillator.h"
#include <thread>
#include <iostream>
#include <string>
#include "IniParser.h"

int main(int argc, char** argv){
    IniParser prsr("config.ini");

    auto samples = prsr.get<int>("config.samples");
    auto analog = prsr.get_array<double>("expected.analog");
    auto binary = prsr.get_array<int>("expected.binary");

    for(const auto i : analog){
        std::cout << i << "\n";
    }

    for(const auto i : binary){
        std::cout << i << "\n";
    }

    Oscillator osc("tcp://*:5556");
    OscConf osccfg = {
        .amp = prsr.get<double>("osc.amp"),
        .freq = prsr.get<double>("osc.freq"),
        .function = prsr.get<long>("osc.function"),
        .buffer_size = prsr.get<uint>("osc.buffer_size")
    };

    osc.set_conf(osccfg);

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