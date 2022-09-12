#include <memory>
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(customlibrary) {
    @function
    $-function.function("$N", &$Z::$N);
    @

    @class
    class_<$Z>("$Z")
        $-constructor.smart_ptr_constructor("$Z", &std::make_shared<$Z>)
        // $CONST.constructor<$ARGSC>()
        $-function.function("$N", &$Z::$N)
        $-member.property("$N", &$Z::get$N, &$Z::set$N)
        // $-cfunction.class_function("$N", &$Z::$N)
        ;
    @
}