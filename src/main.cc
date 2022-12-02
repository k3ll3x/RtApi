// #include "TetraSand.h"
// #include "Oscillator.h"
// #include <thread>
#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>

template<typename T>
std::vector<T> to_array(const std::string& s){
    std::vector<T> result;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, ',')){
        boost::algorithm::trim(item);
        result.push_back(boost::lexical_cast<T>(item));
    }
    return result;
}

int main(int argc, char** argv){
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("config.ini", pt);

    std::cout << pt.get<int>("config.samples") << std::endl;
    auto analog = to_array<double>(pt.get<std::string>("expected.analog"));
    for(const auto i : analog){
        std::cout << i << "\n";
    }

    auto binary = to_array<bool>(pt.get<std::string>("expected.binary"));
    for(const auto i : binary){
        std::cout << i << "\n";
    }
    // zmq::context_t ctx;
    // zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    // publisher.bind("tcp://localhost:5556");
    // return 0;

    // Oscillator osc("tcp://*:5556");
    // std::thread osc_th(&Oscillator::run, &osc);
    
    // std::cout << "Oscillator Running...\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // osc.stop();
    // std::cout << "Stop Oscillator\n";
    // osc_th.join();

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