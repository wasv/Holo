#include <GL/glew.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
#include <cstring>
#include <fstream>
#include <streambuf>

#include "glhelper.hpp"
#include "verts.hpp"
#include "shaders.h"

#define STATIC_IMAGES

#define WIDTH 1024
#define HEIGHT 768

int obj_length;
GLuint obj_vao, obj_vbo;
GLuint blankShaderProgram;

float backdrop_vert[] = {
//  Position             Texture       Normal
     1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f, // Bottom Right
     1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f, // Top Right
     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // Top Left
     
     1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // Bottom Right
     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // Bottom Left
     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // Top Left
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS ){
      if(key == GLFW_KEY_W) { 
        char filename[] = "out.bmp";
        SOIL_save_screenshot
        (
                filename,
                SOIL_SAVE_TYPE_BMP,
                0, 0, WIDTH, HEIGHT
        );
      }
    }
}

void update(glm::vec3 camera, glm::vec3 target) {
    glm::mat4 view     = glm::lookAt(
                      camera,
                      target,
                      glm::vec3( 0.0f,  0.0f,  1.0f)
                    );
    glm::mat4 proj = 
      glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 1.0f, 10.0f);
    
    
    // Move camera
    //GLfloat radius = 2.0f;
    //GLfloat camX = sin(glfwGetTime()) * radius;
    //GLfloat camY = cos(glfwGetTime()) * radius;
    //view = glm::lookAt(glm::vec3(camX,camY,1.5f),
    //        glm::vec3(0.0f, 0.0f, 0.0f),
    //        glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Draw Object
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    
    glUseProgram(blankShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(blankShaderProgram, "view"),
            1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(blankShaderProgram, "proj"),
            1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(blankShaderProgram, "model"),
            1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(obj_vao);
      glDrawArrays(GL_TRIANGLES, 0, obj_length);
    glBindVertexArray(0);
}

int main( int argc, char *argv[] ) {
    
    // --- GLFW Init ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // --- Create window ---
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
                                      "OpenGL", nullptr, nullptr); // Windowed
    if(window == NULL) return 2;
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    // --- GLEW/GL Init ---
    glewExperimental = GL_TRUE;
    glewInit();

    // --- Application Specific Setup ---
    glBindVertexArray(0);
    
    vector<Vert<float, 8>> obj_vertices;
    // Create Object Vertex Arrays
    glGenVertexArrays(1, &obj_vao);

    glGenBuffers(1, &obj_vbo);

    // Create Object 0 Vertex Buffer
    glBindVertexArray(obj_vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj_vbo);
    
    if(argc >= 2) {
      loadOBJ(argv[1], obj_vertices);
    } else {
      loadOBJ("bb8.obj", obj_vertices);
    }
    glBufferData(GL_ARRAY_BUFFER, obj_vertices.size() * sizeof(Vert<float, 8>),
                                  &obj_vertices[0], GL_STATIC_DRAW);
    obj_length = obj_vertices.size();

    // Read Object 0 texture
    int obj_tex_width, obj_tex_height;
    unsigned char* obj_texture =
      SOIL_load_image("tex.png", &obj_tex_width, &obj_tex_height, 0, SOIL_LOAD_RGB);


    // Make Object shader program.
    makeShader(SHADER_VERT3D, SHADER_FRAG_SIMPLE, blankShaderProgram);

    glBindVertexArray(obj_vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj_vbo);

    GLint bvpAttrib = glGetAttribLocation(blankShaderProgram, "position");
    glEnableVertexAttribArray(bvpAttrib);
    glVertexAttribPointer(bvpAttrib, 3, GL_FLOAT, GL_FALSE,
               8*sizeof(float), 0);

    GLint bvtAttrib = glGetAttribLocation(blankShaderProgram, "texcoord");
    glEnableVertexAttribArray(bvtAttrib);
    glVertexAttribPointer(bvtAttrib, 2, GL_FLOAT, GL_FALSE,
               8*sizeof(float), (void*)(3*sizeof(float)));

    GLint bvnAttrib = glGetAttribLocation(blankShaderProgram, "normal");
    glEnableVertexAttribArray(bvnAttrib);
    glVertexAttribPointer(bvnAttrib, 3, GL_FLOAT, GL_FALSE,
               8*sizeof(float), (void*)(5*sizeof(float)));

    // Create texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                  obj_tex_width, obj_tex_height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, obj_texture);
    glGenerateMipmap(GL_TEXTURE_2D);


    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    // --- Main Loop ---
    while(!glfwWindowShouldClose(window))
    {
        // Check for Keypress
        glfwPollEvents();

        // Reset
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Display Views
        glViewport(0, 0, WIDTH/2, HEIGHT/2);
        update(glm::vec3(-2.0f, -2.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.5f));

        glViewport(WIDTH/2, 0, WIDTH/2, HEIGHT/2);
        update(glm::vec3( 2.0f, -2.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.5f));

        glViewport(0, HEIGHT/2, WIDTH/2, HEIGHT/2);
        update(glm::vec3(-2.0f,  2.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.5f));

        glViewport(WIDTH/2, HEIGHT/2, WIDTH/2, HEIGHT/2);
        update(glm::vec3( 2.0f,  2.0f,  1.0f), glm::vec3( 0.0f,  0.0f,  0.5f));

        glfwSwapBuffers(window);
    }

    // --- Cleanup/Shutdown ---
    glDeleteProgram(blankShaderProgram);
    
    glDeleteBuffers(1, &obj_vbo);
    glDeleteVertexArrays(1, &obj_vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

