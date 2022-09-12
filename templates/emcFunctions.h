#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(customLib) {
    $FUNCSfunction("$FNAME", &$FNAME);
}