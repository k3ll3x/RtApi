#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <regex>

#include <cmath>
#include <memory>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vec3.h"

// #include "linmath.h"
#include "ifile.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EM_JS(int, windowGetWidth, (), {
  return window.innerWidth;
});

EM_JS(int, windowGetHeight, (), {
  return window.innerHeight;
});
#endif

struct Shader {
    std::string name;
    std::string text;
    int type;
};

typedef std::chrono::steady_clock hr_clock;
typedef std::chrono::duration<float> time_in_seconds;

class Time {
private:
    Time();
    inline static hr_clock::time_point _start_point;
    inline static hr_clock::time_point _last_time;
    inline static time_in_seconds _delta_time;
public:
    inline static void init(){
        _start_point = hr_clock::now();
        _last_time = _start_point;
        _delta_time = time_in_seconds(0);
    }

    inline static void tick(){
        auto now = hr_clock::now();
        _delta_time = std::chrono::duration_cast<time_in_seconds>(now - _last_time);
        _last_time = now;
    }

    inline static time_in_seconds elapsed_time(){
        return std::chrono::duration_cast<time_in_seconds>(hr_clock::now() - _start_point);
    }

    inline static time_in_seconds delta_time(){
        return _delta_time;
    }
};

class Scene {
public:	
	// virtual ~Scene() = 0;
	virtual void init() = 0;
	virtual void awake() = 0;
	virtual void sleep() = 0;
	virtual void reset() = 0;
	virtual void mainLoop() = 0;
	virtual void error_callback(int error, const char* desc) = 0;
	virtual void key_callback(int key, int scancode, int action, int mods) = 0;
    virtual void cursor_position_callback(double xpos, double ypos) = 0;
    virtual void mouse_button_callback(int button, int action, int mods) = 0;

    GLuint shader_program;
    GLint primitiveType;
    static inline GLenum primitiveTypes[] = {GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY };

    static void loadVertex(std::vector<cgmath::vec2> &positions, int i, GLuint* vao, GLuint* positionsVBO) {
        //glGenVertexArrays(vaoSize, &vao[i]);
        glGenVertexArrays(1, &vao[i]);
        glBindVertexArray(vao[i]);
        //glGenBuffers(vaoSize, &positionsVBO[i]);
        glGenBuffers(1, &positionsVBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, positionsVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, positionsVBO[i]);
        glBindVertexArray(vao[i]);
    }

    void setupShaders(std::vector<Shader>& shaders){
		GLuint shader[shaders.size()];
		for(int i = 0; i < shaders.size(); i++){
		    std::vector<GLchar> log;
			//load shader and compile
            shaders[i].text += "\n\n\0";
			const GLchar* source = (const GLchar*)shaders[i].text.c_str();
			shader[i] = glCreateShader(shaders[i].type);
			glShaderSource(shader[i], 1, &source, nullptr);
			glCompileShader(shader[i]);

			//check for errors
            GLint compiled;
            GLint log_length;
			glGetShaderiv(shader[i], GL_COMPILE_STATUS, &compiled);
			if (compiled != GL_TRUE) {
				glGetShaderiv(shader[i], GL_INFO_LOG_LENGTH, &log_length);
				log.resize(log_length);
				glGetShaderInfoLog(shader[i], log_length, &log_length, &log[0]);
				for (int j = 0; j < log.size(); j++) {
					std::cout << log[j];
				}
			}
            std::cout << "Shader " << shaders[i].name << " processed!\n";
		}

		//manager
		shader_program = glCreateProgram();
		for(int i = 0; i < shaders.size(); i++){
			glAttachShader(shader_program, shader[i]);
		}
		for(int i = 0; i < shaders.size(); i++){
			glBindAttribLocation(shader_program, i, shaders[i].name.c_str());
		}
		glLinkProgram(shader_program);
		for(int i = 0; i < shaders.size(); i++){
			glDeleteShader(shader[i]);
		}
	}
};

class SceneStart : public Scene {
    float mul = 23.0;
	float psize = 22.0;
	float pwidth = 33.0f;

    float py = 0.1;

    uint16_t points = 666;

    std::string vertex = "";

    std::string fragment;

	//destructors
	~SceneStart(){
        glDeleteProgram(shader_program);
    }

	void init(){    
        ifile shvertex;
        shvertex.read("../shaders/grid.vert");
        // std::string vertex = shvertex.get_contents();
        vertex = shvertex.get_contents();

        ifile shfragment;
        shfragment.read("../shaders/solid_color.frag");
        // std::string fragment = shfragment.get_contents();
        fragment = shfragment.get_contents();

        std::vector<Shader> shaders;
        shaders.push_back({"positions", vertex, GL_VERTEX_SHADER});
        shaders.push_back({"colors", fragment, GL_FRAGMENT_SHADER});
	    setupShaders(shaders);
    }

	void awake(){
        glClearColor(0.42f,0.2f,0.33f,1.0f);
        //diferent sizes of points for each shader program
        glEnable(GL_PROGRAM_POINT_SIZE);
    }

	void sleep(){
        glClearColor(0.2f,1.0f,0.5f,1.0f);
	    glDisable(GL_PROGRAM_POINT_SIZE);
    }

	void reset(){}
    void error_callback(int error, const char* desc){}
	void resize(int width, int height){}
	void key_callback(int key, int scancode, int action, int mods){
        if(key == 'A'){
            py += 0.1;
        }
        if(key == 'S'){
            py -= 0.1;
        }
        if (key == '1') {
            primitiveType = primitiveTypes[0];
        }

        if (key == '2') {
            primitiveType = primitiveTypes[1];
        }

        if (key == '3') {
            primitiveType = primitiveTypes[2];
        }

        if (key == '4') {
            primitiveType = primitiveTypes[3];
        }

        if (key == '5') {
            primitiveType = primitiveTypes[4];
        }

        if (key == '6') {
            primitiveType = primitiveTypes[5];
        }

        if (key == '7') {
            primitiveType = primitiveTypes[6];
        }

        if (key == '8') {
            primitiveType = primitiveTypes[7];
        }

        if (key == '9') {
            primitiveType = primitiveTypes[8];
        }

        if (key == '0') {
            primitiveType = primitiveTypes[9];
        }
    }
    void cursor_position_callback(double xpos, double ypos){
        psize = xpos;
        pwidth = ypos/mul;
        py = std::sin(xpos * ypos);
    }
    void mouse_button_callback(int button, int action, int mods){
        printf("%d\t%d\t%d\n", button, action, mods);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
            
        }        
    }

    double magic_rgb(double seed){
        return std::abs(std::cos(std::sin(seed)+Time::elapsed_time().count()));
    }

	void mainLoop(){
        auto tcount = Time::elapsed_time().count();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);
        GLuint time_location = glGetUniformLocation(shader_program, "time");
        glUniform1f(time_location, tcount);
        GLuint p_size = glGetUniformLocation(shader_program, "psize");
        glUniform1f(p_size, psize);
        GLuint _width = glGetUniformLocation(shader_program, "width");
        glUniform1f(_width, pwidth);
        GLuint _py = glGetUniformLocation(shader_program, "py");
        glUniform1f(_py, py);
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, 100);
        points += std::cos(tcount)*((int)tcount%33);
        glDrawArrays(primitiveType, 0, points);
        double r = magic_rgb(33.3);
        double g = magic_rgb(r);
        double b = magic_rgb(g);
        double a = magic_rgb(b);
        glClearColor(r,g,b,a);
        glUseProgram(0);
    }
};

class SceneConchoid : public Scene {
    static const int vaoSize = 300;
	GLuint vao[vaoSize];
	GLuint _positionsVBO[vaoSize];
	GLenum primitiveType;
	GLuint positionsSize[vaoSize];

	cgmath::vec2 getConcoidUp(cgmath::vec2 p, cgmath::vec2 &o, float k) {
        cgmath::vec2 d = k * d.normalize(o - p);
        return p - d;
    }

    cgmath::vec2 getConcoidDown(cgmath::vec2 p, cgmath::vec2 &o, float k) {
        cgmath::vec2 d = k * d.normalize(o - p);
        return p + d;
    }
    void init() {
        //getpoints
        float k = 0.1f;
        cgmath::vec2 o(0.0f,0.4f);

        float interval = 0.01f;
        float wsize = 2.0f;

        float range = 1.0f / 1600;

        for (int j = 0; j < vaoSize; j++) {
            if (j % 2 == 0) {
                std::vector<cgmath::vec2> positionsUp;
                for (float i = -wsize; i < wsize; i += interval) {
                    cgmath::vec2 q = getConcoidUp(cgmath::vec2(i, 0), o, k);
                    if (!(q.x > -range && q.y > -range && q.x < range && q.y < range)) {
                        positionsUp.push_back(q);
                    }
                }

                positionsSize[j] = positionsUp.size();
                loadVertex(positionsUp, j, vao, _positionsVBO);
            }
            else {
                std::vector<cgmath::vec2> positionsDown;
                for (float i = -wsize; i < wsize; i += interval) {
                    cgmath::vec2 q = getConcoidDown(cgmath::vec2(i, 0), o, k);
                    if (!(q.x > -range && q.y > -range && q.x < range && q.y < range)) {
                        positionsDown.push_back(q);
                    }
                }

                positionsSize[j] = positionsDown.size();
                loadVertex(positionsDown, j, vao, _positionsVBO);
                k += 0.1f;
            }
        }

        primitiveType = GL_LINE_STRIP;
        // primitiveType = GL_POINT;
    }

    void awake() {
        glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
        glPointSize(1.0f);
    }

    void sleep() {
        glPointSize(1.0f);
    }

    void mainLoop() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < vaoSize; i++) {
            glBindVertexArray(vao[i]);
            glDrawArrays(primitiveType, 0, positionsSize[i]);
            glBindVertexArray(vao[i]);
        }
    }

    void reset(){}
    void error_callback(int error, const char* desc){}
    void cursor_position_callback(double xpos, double ypos){}
    void mouse_button_callback(int button, int action, int mods){}
    void key_callback(int key, int scancode, int action, int mods){}
};

class SceneSphere : public Scene {
    float gypos, gxpos, div = 33.131f;
    int np = 1030;
    GLint primitiveType = GL_POINTS;
    void key_callback(int key, int scancode, int action, int mods){
        if(key == 'A')div += 1.323;
        if(key == 'S')div -= 1.323;
        if(key == 'Q')np += 33;
        if(key == 'W')np -= 33;
        if (key == '1') {
            primitiveType = primitiveTypes[0];
        }

        if (key == '2') {
            primitiveType = primitiveTypes[1];
        }

        if (key == '3') {
            primitiveType = primitiveTypes[2];
        }

        if (key == '4') {
            primitiveType = primitiveTypes[3];
        }

        if (key == '5') {
            primitiveType = primitiveTypes[4];
        }

        if (key == '6') {
            primitiveType = primitiveTypes[5];
        }

        if (key == '7') {
            primitiveType = primitiveTypes[6];
        }

        if (key == '8') {
            primitiveType = primitiveTypes[7];
        }

        if (key == '9') {
            primitiveType = primitiveTypes[8];
        }

        if (key == '0') {
            primitiveType = primitiveTypes[9];
        }
    }
    void error_callback(int error, const char* desc){};
	void cursor_position_callback(double xpos, double ypos){
        gypos = ypos;
        gxpos = xpos;
    };
    void mouse_button_callback(int button, int action, int mods){};
    ~SceneSphere()
    {
        //Delete executable from memory when scene fades,.
        glDeleteProgram(shader_program);
    }

    void init()
    {
        ifile shvertex;
        shvertex.read("../shaders/sphere.vert");
        std::string vertex = shvertex.get_contents();

        ifile shfragment;
        shfragment.read("../shaders/solid_color.frag");
        std::string fragment = shfragment.get_contents();

        std::vector<Shader> shaders;
        shaders.push_back({"positions", vertex, GL_VERTEX_SHADER});
        shaders.push_back({"colors", fragment, GL_FRAGMENT_SHADER});
	    setupShaders(shaders);
    }

    void awake()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_PROGRAM_POINT_SIZE);//diferentes tamanios de puntos para cada shaders
    }

    void sleep()
    {
        glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
        glDisable(GL_PROGRAM_POINT_SIZE);
    }

    double magic_rgb(double seed){
        return std::abs(std::sin(std::cos(seed)+Time::elapsed_time().count()));
    }

    void mainLoop()
    {
        auto tcount = Time::elapsed_time().count();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);
        GLuint time_location = glGetUniformLocation(shader_program, "time");
        glUniform1f(time_location, tcount);
        GLuint _xpos = glGetUniformLocation(shader_program, "mox");
        glUniform1f(_xpos, gxpos);
        GLuint _ypos = glGetUniformLocation(shader_program, "moy");
        glUniform1f(_ypos, gypos);
        GLuint _div = glGetUniformLocation(shader_program, "div");
        glUniform1f(_div, div);
        glDrawArrays(primitiveType, 0, np);
        double r = magic_rgb(std::sin(tcount));
        double g = magic_rgb(std::cos(tcount));
        double b = magic_rgb(r*g);
        double a = magic_rgb(b);
        glClearColor(r,g,b,a);
        glUseProgram(0);
    }

    void resize(int width, int height)
    {
    }

    void reset(){};
};

class SceneManager{
public:
    inline static void start(const char *name, int w, int h){
        Time::init();
        if (!glfwInit()){
            std::cout << "Could not create window!\n";
            cleanup();
            exit(EXIT_FAILURE);
        }

        // glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        // const char* glsl_version = "#version 330";
        #ifdef __EMSCRIPTEN__
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
        #else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        #endif

        width = w;
        height = h;

        window = glfwCreateWindow(width, height, name, nullptr, nullptr);

        if (!window){
            std::cout << "Could not create window!\n";
            cleanup();
            exit(EXIT_FAILURE);
        }

        // GLFWimage icon[1];
        // icon[0].width = xek::width;
        // icon[0].height = xek::height;
        // icon[0].pixels = (unsigned char*)xek::xek_icon_w;
        // glfwSetWindowIcon(window, 1, icon);

        glfwGetFramebufferSize(window, &width, &height);

        // moved to scenes init in Scene Manager
        glfwSetErrorCallback(error_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetWindowSizeCallback(window, resize);

        // glfwSetWindowCloseCallback(window, GL_FALSE);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        // glewExperimental = GL_TRUE;
        if (GLEW_OK != glewInit()){
            std::cout << "Error init glew\n";
            cleanup();
            exit(EXIT_FAILURE);
        }

        // win_mgr.setup(glsl_version, window);

        glViewport(0, 0, width, height);

        // OpenGL init
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        // GL version
        std::cout << glGetString(GL_VERSION) << std::endl;

        //maximize window
        glfwMaximizeWindow(window);

        // Scene init
        initialize();

        // Run main loop
        mainLoop();
    }
private:
    inline static GLFWwindow* window;
    inline static std::vector<std::unique_ptr<Scene>> sceneList;
    inline static int currentScene = -1;
    inline static int width, height;
    inline static double mxpos, mypos;
private:
    inline static void error_callback(int error, const char *description){
        fprintf(stderr, "Error: %s\n", description);
    }

    inline static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
        // std::cout << "key:\t" << key << "\t" << (char)key << "\tscancode:\t" << scancode << "\taction:\t" << action << "\tmods:\t" << mods << '\n';
        if(key == 'T') Time::tick();
        if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS){
            next();
        }
        if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS){
            prev();
        }
        if (key == GLFW_KEY_R && action == GLFW_PRESS){
            if (currentScene >= 0)
                sceneList.at(currentScene)->reset();
        }
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key == GLFW_KEY_F11 && action == GLFW_PRESS){
            glfwMaximizeWindow(window);
        }
        if (currentScene >= 0)
            sceneList.at(currentScene)->key_callback(key, scancode, action, mods);
    }

    inline static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
        if (currentScene >= 0)
            sceneList.at(currentScene)->cursor_position_callback(xpos, ypos);
    }

    inline static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
        if (currentScene >= 0)
            sceneList.at(currentScene)->mouse_button_callback(button, action, mods);
    }

    inline static void next(){
        int nScenes = (int)sceneList.size();
        if (nScenes > 0){
            sceneList.at(currentScene)->sleep();
            currentScene = (currentScene + 1) % nScenes;
            sceneList.at(currentScene)->awake();
        }
    }

    inline static void prev(){
        int nScenes = (int)sceneList.size();
        if (nScenes > 0){
            sceneList.at(currentScene)->sleep();
            currentScene--;
            if (currentScene < 0)
                currentScene = (nScenes - 1);
            sceneList.at(currentScene)->awake();
        }
    }

    inline static void initialize(){
        std::unique_ptr<Scene> scene0(new SceneStart);
        std::unique_ptr<Scene> scene1(new SceneConchoid);
        std::unique_ptr<Scene> scene2(new SceneSphere);
        sceneList.push_back(std::move(scene0));
        sceneList.push_back(std::move(scene1));
        sceneList.push_back(std::move(scene2));

        //vao
        GLuint vao;
        GLuint positionsVBO;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        for (auto& s : sceneList){
            s->init();
        }


        if (sceneList.size() >= 0){
            currentScene = 0;
            sceneList.at(currentScene)->awake();
        }
    }

    inline static void loop(){
        if (currentScene >= 0)
            sceneList.at(currentScene)->mainLoop();

        glfwPollEvents();

        // if (currentScene >= 0)
        //     sceneList.at(currentScene)->sceneWindowHandler();

        glfwSwapBuffers(window);

        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // std::cout << "Hey\n";
    }

    inline static void mainLoop(){
        Time::tick();
        #ifdef __EMSCRIPTEN__
            emscripten_set_main_loop(loop, 0, 1);
        #else
            while (!glfwWindowShouldClose(window)){
                loop(); 
            }
        #endif

        cleanup();
    }

    inline static void idle(){
        // glutPostRedisplay();
    }

    inline static void cleanup(){
        glfwDestroyWindow(window);
        glfwTerminate();
        sceneList.clear();
        currentScene = -1;
    }

    inline static void resize(GLFWwindow *window, int width, int height){
        glViewport(0, 0, width, height);
    }
};
