#include <GL/glew.h>
#include <GL/gl.h>
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

#define WIDTH 1024
#define HEIGHT 768

#define PI 3.14159265f

int obj_length;
float xAngle = 0.0f;
float yAngle = 0.0f;
float radius = 3.0f;

GLuint obj_vao, obj_vbo;
GLuint blankShaderProgram;
GLFWwindow* window;

float backdrop_vert[] = {
//  Position             Texture       Normal
     1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f, // Bottom Right
     1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f, // Top Right
     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // Top Left
     
     1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // Bottom Right
     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // Bottom Left
     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // Top Left
};

void key_callback(GLFWwindow* localwindow, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS ){
      // w - Screenshot
      if(key == GLFW_KEY_W) { 
        char filename[] = "out.bmp";        
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        SOIL_save_screenshot
        (
                filename,
                SOIL_SAVE_TYPE_BMP,
                0, 0, width, height
        );
      // k - Increase object xRotation
      } else if(key == GLFW_KEY_K) {
        xAngle += 15.0f;
        xAngle = xAngle > 360 ? xAngle - 360 : xAngle;
      // j - Decrease object xRotation
      } else if(key == GLFW_KEY_J) {
        xAngle -= 15.0f;
        xAngle = xAngle < 0 ? 360 - xAngle : xAngle;
      // s - Decrease object distance 
      } else if(key == GLFW_KEY_S) {
        radius -= 0.5f;
        radius = radius < 1 ? 1 : radius;
      // d - Increase object distance
      } else if(key == GLFW_KEY_D) {
        radius += 0.5f;
        radius = radius > 15 ? 15 : radius;
      // ESC - Exit program
      } else if ( key == GLFW_KEY_ESCAPE ) {
        exit(0);
      }
    }
}

void update(glm::mat4 view, glm::mat4 proj, glm::mat4 model) {
    // Setup Shader
    glUseProgram(blankShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(blankShaderProgram, "view"),
            1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(blankShaderProgram, "proj"),
            1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(blankShaderProgram, "model"),
            1, GL_FALSE, glm::value_ptr(model));
    glm::vec3 lighting = glm::vec3(radius*1.1,0.5,0.5);
    glUniform3fv(glGetUniformLocation(blankShaderProgram, "lighting"),
            1, glm::value_ptr(lighting));
    
    // Draw Object
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
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);

    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(mode->width, mode->height,
                              "OpenGL", glfwGetPrimaryMonitor() , nullptr); // Windowed
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

    Vert<float, 8> max;
    maximum(obj_vertices, max);
    Vert<float, 8> min;
    minimum(obj_vertices, min);
    
    float dim[3] = {max[0]-min[0], max[1]-min[1], max[2]-min[2]};
    float center[3] = {(dim[0]/2)+min[0], (dim[1]/2)+min[1], (dim[2]/2)+min[2]};

    float maxdim;
    if(dim[0] > dim[1] && dim[0] > dim[2]) {
      maxdim = dim[0];
    } else if(dim[1] > dim[2]) {
      maxdim = dim[1];
    } else {
      maxdim = dim[2];
    }
    
    cout << max[0] << ',' << max[1] << ',' << max[2] << endl;
    cout << min[0] << ',' << min[1] << ',' << min[2] << endl;
    cout << dim[0] << ',' << dim[1] << ',' << dim[2] << endl;
    cout << center[0] << ',' << center[1] << ',' << center[2] << endl;
    cout << maxdim << endl;
    
    obj_length = obj_vertices.size();

    // Read Object 0 texture
//    int obj_tex_width, obj_tex_height;
//    unsigned char* obj_texture;
//    if(argc >= 3) {
//      obj_texture =
//        SOIL_load_image(argv[2], &obj_tex_width, &obj_tex_height, 0, SOIL_LOAD_RGB);
//    } else {
//      obj_texture =
//        SOIL_load_image("tex.png", &obj_tex_width, &obj_tex_height, 0, SOIL_LOAD_RGB);
//    }

    // Make Object shader program.
    makeShader(SHADER_VERT3D, SHADER_FRAG_LIGHTING, blankShaderProgram);

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
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glGenerateMipmap(GL_TEXTURE_2D);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
    //              obj_tex_width, obj_tex_height, 0, GL_RGB,
    //              GL_UNSIGNED_BYTE, obj_texture);
    //glGenerateMipmap(GL_TEXTURE_2D);


    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    // --- Main Loop ---
    while(!glfwWindowShouldClose(window)) {
        // Check for Keypress
        glfwPollEvents();

        // Reset
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        //Display Views
        // Screen Layout:
        //   -----
        //  |  T  |
        //  | L R |
        //  |  B  |
        //   -----
        glm::mat4 proj = 
          glm::perspective(glm::radians(45.0f), 1.0f, 0.5f, 20.0f);
        glm::mat4 model = glm::mat4();
        model = glm::scale(model, glm::vec3(1.0f/maxdim,1.0f/maxdim,1.0f/maxdim));
        model = glm::translate(model,glm::vec3(-center[0],-center[1],-center[2]));
        model = glm::rotate(model, (GLfloat)fmod((glfwGetTime()),2.0f*PI),glm::vec3(0.0f,0.0f,1.0f));
        //model = glm::rotate(model, (GLfloat)(yAngle*PI/180),glm::vec3(0.0f,1.0f,0.0f));
        glm::mat4 view = glm::lookAt(
                          glm::vec3(radius, 0.0f, 0.0f),
                          glm::vec3( 0.0f,  0.0f, 0.0f),
                          glm::vec3( 0.0f,  0.0f, 1.0f)
                        );
        
        // Top View
        glViewport(((width-height)/2)+(height/3), 2*height/3, height/3, height/3);
        update(view, proj, model);

        // Left View
        view = glm::rotate(view, (GLfloat)(90.0f*PI/180),glm::normalize(glm::vec3(1.0f,0.0f,0.0f)));
        model = glm::rotate(model, (GLfloat)(90.0f*PI/180),glm::vec3(0.0f,0.0f,1.0f));

        glViewport(((width-height)/2), height/3, height/3, height/3);
        update(view, proj, model);

        // Bottom View
        view = glm::rotate(view, (GLfloat)(90.0f*PI/180),glm::normalize(glm::vec3(1.0f,0.0f,0.0f)));
        model = glm::rotate(model, (GLfloat)(90.0f*PI/180),glm::vec3(0.0f,0.0f,1.0f));

        glViewport(((width-height)/2)+(height/3), 0, height/3, height/3);
        update(view, proj, model);

        // Right View
        view = glm::rotate(view, (GLfloat)(90.0f*PI/180),glm::normalize(glm::vec3(1.0f,0.0f,0.0f)));
        model = glm::rotate(model, (GLfloat)(90.0f*PI/180),glm::vec3(0.0f,0.0f,1.0f));

        glViewport(((width-height)/2)+(2*(height/3)), height/3, height/3, height/3);
        update(view, proj, model);

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

