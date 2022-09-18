#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#include <emscripten.h>

using namespace emscripten;

extern "C" {
    extern void loop();
}

EMSCRIPTEN_BINDINGS(customlibrary) {
    function("",&);
}

// EMSCRIPTEN_BINDINGS(customlibrary) {
    
//     function("lfun",&lfun);

//     class_<ExampleClass>("ExampleClass")
//         .constructor<int>()

//         .function("getY", &ExampleClass::getY, allow_raw_pointers())
//         .function("getX", &ExampleClass::getX)

//                         ;

// }
#endif