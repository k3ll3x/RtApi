#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Shader {
    std::string name;
    std::string text;
    int type;
};

class Graphics {
public:
    Graphics(){
        setup();
    }

    ~Graphics(){
        cleanup();
    }

    GLFWwindow* window;
    GLuint shader_program;

    void setup(int width = 800, int height = 600){
        if (!glfwInit()){
            // log += "Could not create window!\n";
            std::cout << "Could not create window!\n";
            cleanup();
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        window = glfwCreateWindow(width, height, "Satanas", nullptr, nullptr);

        if (!window){
            // log += "Could not create window!\n";
            std::cout << "Could not create window!\n";
            cleanup();
            exit(EXIT_FAILURE);
        }

        glfwGetFramebufferSize(window, &width, &height);

        // glfwSetErrorCallback(error_callback);
        // glfwSetKeyCallback(window, key_callback);
        // glfwSetCursorPosCallback(window, cursor_position_callback);
        // glfwSetMouseButtonCallback(window, mouse_button_callback);
        // glfwSetWindowSizeCallback(window, resize);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (GLEW_OK != glewInit()){
            // log += "Error init glew\n";
            std::cout << "Error init glew\n";
            cleanup();
            exit(EXIT_FAILURE);
        }

        glViewport(0, 0, width, height);

        // OpenGL init
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // GL version
        std::cout << glGetString(GL_VERSION) << '\n';

        //maximize window
        glfwMaximizeWindow(window);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

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
private:
    const char* glsl_version = "#version 330";
    GLuint vao;
    GLuint positionsVBO;

    void resize(GLFWwindow *window, int width, int height){
        glViewport(0, 0, width, height);
    }

    void cleanup(){
        std::cout << "Cleaning...\n";
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};