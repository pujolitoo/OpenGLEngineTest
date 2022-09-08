#include "main.h"

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "RenderCommand.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void imguiUpdate(int &count);

void oglLogCallback(GLenum source​, GLenum type​, GLuint id​, GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​)
{
    std::cout << message​ << std::endl;
}

enum ShaderType
{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

struct ShaderProgram
{
    std::string fragmentShader;
    std::string vertexShader;
};

static ShaderProgram LoadShaderFromFile(std::string path)
{
    std::string shader;
    std::ifstream is;
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    ShaderProgram pg;

    is.open(path);
    if(is.is_open())
    {
        while(std::getline(is, line))
        {
            if(line.find("#vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }else if(line.find("#fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
            else{
                ss[(int)type] << line << "\n";
            }
        }
    }
    is.close();

    pg.fragmentShader = ss[1].str();
    pg.vertexShader = ss[0].str();
    return pg;
}

static unsigned int CompileShader(GLenum type, std::string& shader)
{
    unsigned int shaderId = glCreateShader(type);
    const char* source = shader.c_str();
    glShaderSource(shaderId, 1, &source, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)alloca(lenght*sizeof(char));
        glGetShaderInfoLog(shaderId, lenght, &lenght, message);
        LOG("Failed to compile shader!");
        E_ASSERT(false, "Could not compile the shader");
        std::cout << message << std::endl;

        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

static unsigned int CreateShader(std::string& vertShader, std::string& fragShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    LOG("----------------------");
    LOG("SCROLL X: " << xoffset);
    LOG("SCROLL Y: " << yoffset);
    LOG("----------------------");
}

#ifdef __cplusplus
extern "C"{
#endif
    int APIEXPORT mainfunc()
    {
        int count = 0;

        if(!glfwInit())
        {
            return 1;
        }

        GLFWwindow *window = glfwCreateWindow(1200, 800, "The Game", NULL, NULL);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            MessageBox(NULL, TEXT("Error while loading GLAD"), TEXT("ERROR"), MB_ICONERROR);
            return -1;
        }

        glfwSetScrollCallback(window, scroll_callback);

        glfwSwapInterval(1);

        LOG("OpenGL Version: " << glGetString(GL_VERSION));

        //Setup log callback

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC)oglLogCallback, nullptr);

        //Init buffers
        unsigned int vaoId;
        glGenVertexArrays(1, &vaoId);
        glBindVertexArray(vaoId);
        LOG("VAOID: " << vaoId);

        float pos[] = {
                        /* Coords */                /*Colors*/              /*Texture Coordinates*/
            /* Vertex 1 */ -0.5f, 0.5f, 0.0f,        1.0f, 0.0f, 0.0f,    0.0f, 1.0f, // top left
                                            
            /* Vertex 2 */ -0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  // bottom left  

            /* Vertex 3 */ 0.5f, -0.5f, 0.0f,       0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  // bottom right

            /* Vertex 4 */ 0.5f, 0.5f, 0.0f,         1.0f, 1.0f, 0.0f,   1.0f, 1.0f   // top right

        };

        unsigned int indices[] = {
            0, 1, 3, 3, 1, 2
        };

        VertexBuffer vb(pos, sizeof(pos));

        IndexBuffer ib(indices, sizeof(indices));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
        glEnableVertexAttribArray(2);

        //Init textures

        unsigned int tex0;
        glGenTextures(1, &tex0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        const char* textureName = "container.jpg";
        int width, height, comp;
        uint8_t* tex1 = stbi_load(textureName, &width, &height, &comp, 0);
        if(tex1)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
            LOG("Texture " << textureName << " loaded succesfully.");
        }
        else
        {
            LOG("Couldn't load texture " << textureName << ".");
        }
        stbi_image_free(tex1);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Init shaders

        ShaderProgram shader1 = LoadShaderFromFile("shdr1.glsl");

        LOG(shader1.vertexShader << shader1.fragmentShader);

        glm::mat4 proj = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f);

        unsigned int fShader = CreateShader(shader1.vertexShader, shader1.fragmentShader);
        glUseProgram(fShader);

        glUniformMatrix4fv(glGetUniformLocation(fShader, "u_MVP"), 1, GL_FALSE, &(proj[0][0]));

        glUniform1i(glGetUniformLocation(fShader, "ourTexture"), 0);

        glfwShowWindow(window);

        while(!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            RenderCommand::Clear();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex0);

            glBindVertexArray(vaoId);

            glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);

            glfwSwapBuffers(window);
        }

        glDeleteProgram(fShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }
#ifdef __cplusplus
}
#endif
