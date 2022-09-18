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

#include "linmath.h"
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
	virtual void resize(GLFWwindow* window, int width, int height) = 0;
	virtual void error_callback(int error, const char* desc) = 0;
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;

    GLuint shader_program;

    void setupShaders(std::vector<Shader>& shaders){
		GLuint shader[shaders.size()];
        GLchar* shader_source[shaders.size()];
		GLint shader_compiled[shaders.size()];
		GLint log_length;
		std::vector<GLchar> log;
		for(int i = 0; i < shaders.size(); i++){
			//load shader and compile
			shader_source[i] = (GLchar*)shaders[i].text.c_str();
			shader[i] = glCreateShader(shaders[i].type);
			glShaderSource(shader[i], 1, &shader_source[i], nullptr);
			glCompileShader(shader[i]);

			//check for errors
			glGetShaderiv(shader[i], GL_COMPILE_STATUS, &shader_compiled[i]);
			if (shader_compiled[i] != GL_TRUE) {
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
    float mul = 1.0;
	float psize = 10.0;
	float width = 10.0f;

	//destructors
	~SceneStart(){
        glDeleteProgram(shader_program);
    }

	void init(){
        #ifdef __EMSCRIPTEN__
            std::string vertex =
                "#version 300 es\n"
                "uniform float time;\n"
                "uniform float psize;\n"
                "uniform float width;\n"
                "out vec4 InterpolatedColor;\n"
                "void main(){\n"
                "    //float width = 10.0f;\n"
                "    float x = float(gl_VertexID)/ width;\n"
                "    float y = float(gl_VertexID)/ width;\n"
                "    float u = x / (width - 1.0f);\n"
                "    float v = y / (width - 1.0f);\n"
                "    float xOffset = cos(time + y * 0.2f) * 0.1f;\n"
                "    float yOffset = cos(time + x * 0.3f) * 0.2f;\n"
                "    float ux = u * 2.0f - 1.0f + xOffset;\n"
                "    float uy = v * 2.0f - 1.0f + yOffset;\n"
                "    vec2 xy = vec2(ux, uy) * 1.2f;\n"
                "    gl_Position = vec4(xy, 0.0f, 1.0f);\n"
                "    gl_PointSize = psize;\n"
                "    InterpolatedColor = vec4(sin(time*x), cos(time*y), cos(x*y), 1.0);\n"
                "}\n"
                "";

            std::string fragment =
                "#version 300 es\n"
                "precision highp float;\n"
                "out vec4 FragColor;\n"
                "in vec4 InterpolatedColor;\n"
                "void main(){\n"
                "    FragColor = InterpolatedColor;\n"
                "}\n"
                "";
        #else
            std::string vertex = ifile("/src/shaders/grid.vert").get_contents();
            std::string fragment = ifile("/src/shaders/solid_color.frag").get_contents();
        #endif

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

	void reset() {}
	void resize(GLFWwindow* window, int width, int height){}
	void error_callback(int error, const char* desc){}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){}

	void mainLoop(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);
        GLuint time_location = glGetUniformLocation(shader_program, "time");
        glUniform1f(time_location, Time::elapsed_time().count() * mul);
        GLuint p_size = glGetUniformLocation(shader_program, "psize");
        glUniform1f(p_size, psize);
        GLuint _width = glGetUniformLocation(shader_program, "width");
        glUniform1f(_width, width);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 100);	
        // glDrawArrays(GL_POINTS, 0, 100);
        glUseProgram(0);
    }
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
            sceneList.at(currentScene)->key_callback(window, key, scancode, action, mods);
    }

    inline static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
        mxpos = xpos;
        mypos = ypos;
        //std::cout << xpos << '\t' << ypos << '\n';
    }

    inline static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            std::cout << mxpos << '\t' << mypos << '\n';
        }
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
        sceneList.push_back(std::move(scene0));

        //vao
        GLuint vao;
        GLuint positionsVBO;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        for (auto& s : sceneList)
            s->init();

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
