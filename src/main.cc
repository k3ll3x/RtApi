// #include "ExampleClass.h"

#include "Graphics.h"
#include "ifile.h"
#include <cmath>
#include <memory>
#include <chrono>
#include <thread>

auto graphics = std::make_unique<Graphics>();

int psize, width = 200;

void setPsize(int n){
    psize = n;
}

void setWidth(int n){
    width = n;
}

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#include <emscripten.h>

using namespace emscripten;

extern "C" {
    extern void another_loop();
}

EMSCRIPTEN_BINDINGS(customlibrary) {
    function("setPsize",&setPsize);
    function("setWidth",&setWidth);
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

float magic(float x, int64_t y){
    return std::abs(std::sin((float)(((int64_t)x)^y)));
}

std::chrono::time_point<std::chrono::system_clock> start;

void loop(){
    #ifdef __EMSCRIPTEN__
        std::cout << "_E_\n";
        // another_loop();
    #endif
    float   r = 0.2f,
            g = 0.0f,
            b = 0.4f;
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glClearColor(r, g, b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto end = std::chrono::system_clock::now();
    auto tcount = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    glClearColor(magic(r,tcount), magic(g,tcount), magic(b,tcount), 1.0f);

    // glUseProgram(graphics->shader_program);
    // GLuint time_location = glGetUniformLocation(graphics->shader_program, "time");
    // glUniform1f(time_location, tcount);
    // GLuint p_size = glGetUniformLocation(graphics->shader_program, "psize");
    // glUniform1f(p_size, psize);
    // GLuint _width = glGetUniformLocation(graphics->shader_program, "width");
    // glUniform1f(_width, width);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, 100);	
    // glDrawArrays(GL_POINTS, 0, 100);
    // glUseProgram(0);

    std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "Fuckin A\n";

    glfwPollEvents();
    glfwSwapBuffers(graphics->window);

    // int x;
    // std::cin.get();
    // std::cin >> x;
}


int main(){
    // graphics = new Graphics();
    start = std::chrono::system_clock::now();
    std::vector<Shader> shaders;

    #ifdef EMSCRIPTEN
        std::string vertex = "#version 330\n\nuniform float time;\nuniform float psize;\nuniform float width;\n\nout vec4 InterpolatedColor;\n\nvoid main(){\n  //float width = 10.0f;\n\n  float x = mod(gl_VertexID, width);\n  float y = floor(gl_VertexID / width);\n\n  float u = x / (width - 1.0f);\n  float v = y / (width - 1.0f);\n\n  float xOffset = cos(time + y * 0.2f) * 0.1f;\n  float yOffset = cos(time + x * 0.3f) * 0.2f;\n\n  float ux = u * 2.0f - 1.0f + xOffset;\n  float uy = v * 2.0f - 1.0f + yOffset;\n\n  vec2 xy = vec2(ux, uy) * 1.2f;\n\n  gl_Position = vec4(xy, 0.0f, 1.0f);\n  gl_PointSize = psize;\n  InterpolatedColor = vec4(sin(time*x), cos(time*y), cos(x*y), 1.0);\n}";
        std::string fragment = "#version 330\n\nout vec4 FragColor;\n\nin vec4 InterpolatedColor;\n\nvoid main(){\n    FragColor = InterpolatedColor;\n}";
    #else
        std::string vertex = ifile("/src/shaders/grid.vert").get_contents();
        std::string fragment = ifile("/src/shaders/solid_color.frag").get_contents();
    #endif

    // std::cout << vertex << '\n';
    // std::cout << fragment << '\n';

    shaders.push_back({"positions", vertex, GL_VERTEX_SHADER});
    shaders.push_back({"colors", fragment, GL_FRAGMENT_SHADER});
    // shaders.push_back({"positions", ifile("/src/shaders/grid.vert").get_contents(), GL_VERTEX_SHADER});
    // shaders.push_back({"colors", ifile("/src/shaders/solid_color.frag ").get_contents(), GL_FRAGMENT_SHADER});

    // graphics->setupShaders(shaders);

    #ifdef EMSCRIPTEN
        // emscripten_set_main_loop(loop, 60, 1);
        loop();
    #else
        std::cout << "Native\n";
        loop();
    #endif

    return 0;
}