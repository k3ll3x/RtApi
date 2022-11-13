#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <zmq.hpp>
#include <memory>
#include <atomic>
#include <cmath>
#include <vector>
#include "Time.h"

class Oscillator {
public:
    // Oscillator(std::shared_ptr<zmq::context_t> _ctx, std::string addr){
    Oscillator(std::string addr){
        publisher = new zmq::socket_t(ctx, zmq::socket_type::pub);
        publisher->bind(addr);
    }

    void run(){
        std::vector<double> data;
        Time::init();
        while(keepRunning){
            for(int i=0;i<buffer_size;i++){
                auto angle = amp*freq*pi*i*Time::elapsed_time().count();
                data.push_back(std::sin(angle));
            }
            zmq::message_t message(data.size());
            memcpy(message.data(), data.data(), data.size());
            data.clear();
            publisher->send(message, zmq::send_flags::none);
            Time::tick();
        }
    }

    void stop(){
        keepRunning = false;
    }

    double amp = 2.0;
    double freq = 440;
    double pi =  3.14159;
private:
    std::atomic<bool> keepRunning = true;
    uint buffer_size = 200;
    // std::shared_ptr<zmq::context_t> ctx;
    // zmq::context_t* ctx;
    // std::unique_ptr<zmq::socket_t> publisher;
    zmq::context_t ctx;
    zmq::socket_t* publisher;
};