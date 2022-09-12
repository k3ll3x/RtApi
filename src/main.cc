// #include "ExampleClass.h"

#include "Graphics.h"
#include "ifile.h"
#include <cmath>

Graphics* graphics;

int psize, width = 2;

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


void loop(){
    float r,g,b = 0.69f;
    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(r, g, b, 1.0f);
    while(!glfwWindowShouldClose(graphics->window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
        glUseProgram(graphics->shader_program);
        GLuint time_location = glGetUniformLocation(graphics->shader_program, "time");
        glUniform1f(time_location, 3.14159f);
        GLuint p_size = glGetUniformLocation(graphics->shader_program, "psize");
        glUniform1f(p_size, psize);
        GLuint _width = glGetUniformLocation(graphics->shader_program, "width");
        glUniform1f(_width, width);
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, 100);	
        glDrawArrays(GL_POINTS, 0, 100);
        glUseProgram(0);

        glfwPollEvents();
        glfwSwapBuffers(graphics->window);
	}
}


int main(){
    graphics = new Graphics();

    std::vector<Shader> shaders;

    std::string vertex = ifile("/src/shaders/grid.vert").get_contents();
    std::string fragment = ifile("/src/shaders/solid_color.frag").get_contents();

    std::cout << vertex << '\n';
    std::cout << fragment << '\n';

    shaders.push_back({"positions", vertex, GL_VERTEX_SHADER});
    shaders.push_back({"colors", fragment, GL_FRAGMENT_SHADER});
    // shaders.push_back({"positions", ifile("/src/shaders/grid.vert").get_contents(), GL_VERTEX_SHADER});
    // shaders.push_back({"colors", ifile("/src/shaders/solid_color.frag ").get_contents(), GL_FRAGMENT_SHADER});
    graphics->setupShaders(shaders);

    #ifdef EMSCRIPTEN
        emscripten_set_main_loop(loop, 60, 1);
    #else
        std::cout << "Native\n";
        loop();
    #endif

    return 0;
}