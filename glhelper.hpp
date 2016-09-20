#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

#include "verts.hpp"

using namespace std;

// Format: 8 wide - [position xyz, texture xy, normal xyz]
void loadOBJ(string path, vector<Vert<float, 8>> & out_vert) {
  cout << "Loading OBJ file " << path << "..." << endl;
  
  ifstream in_file;
  in_file.open (path);

  if (in_file.is_open()) {
    vector<Vert<float, 3>> tmp_vert;
    vector<Vert<float, 2>> tmp_uv;
    vector<Vert<float, 3>> tmp_norm;
    vector<Vert<int, 9>> tmp_index;
    string line;

    while(!in_file.eof()) {
      getline(in_file, line);

      istringstream line_s (line);
      string header;
      line_s >> header;

      if(header[0] != '#') {
        if(header.compare("v") == 0) {
          Vert<float, 3> data;
          sscanf(line.c_str(), "v %f %f %f", &data[0], &data[1], &data[2]);

          tmp_vert.push_back(data);

        } else if(header.compare("vt") == 0) {
          Vert<float, 2> data;
          sscanf(line.c_str(), "vt %f %f", &data[0], &data[1]);

          tmp_uv.push_back(data);

        } else if(header.compare("vn") == 0) {
          Vert<float, 3> data;
          sscanf(line.c_str(), "vn %f %f %f", &data[0], &data[1], &data[2]);

          tmp_norm.push_back(data);

        } else if(header.compare("f") == 0) {
          Vert<int, 9> index;

          char skip = '\0';

          int ret = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
              &index[0], &index[1], &index[2],
              &index[3], &index[4], &index[5],
              &index[6], &index[7], &index[8]);
          if(ret!=9) {
            ret = sscanf(line.c_str(), "f %d//%d %d//%d %d//%d\n",
                &index[0], &index[2],
                &index[3], &index[5],
                &index[6], &index[8]);
            index[1] = -1;
            index[4] = -1;
            index[7] = -1;
            if(ret!=6) {
              ret = sscanf(line.c_str(), "f %d %d %d\n",
                  &index[0], &index[3], &index[6]);
              index[2] = -1;
              index[5] = -1;
              index[8] = -1;
              if(ret!=3) {
                cerr << "Parsing failed." << endl;
                return;
              }
            }
          }

          tmp_index.push_back(index);
        }
      }
    }

    for(int i = 0; i < tmp_index.size(); i++) {
      for(int p = 0; p < 3; p++) {
        Vert<float, 8> vertex;
        if(tmp_index[i][(p*3)] != -1) {
          vertex[0] = tmp_vert[ tmp_index[i][(p*3)]-1 ][0];
          vertex[1] = tmp_vert[ tmp_index[i][(p*3)]-1 ][1];
          vertex[2] = tmp_vert[ tmp_index[i][(p*3)]-1 ][2];
        } else {
          cerr << "Indexing failed" << endl;
          return;
        }
        if(tmp_index[i][(p*3)+1] != -1) {
          vertex[3] = tmp_uv[ tmp_index[i][(p*3)+1]-1 ][0];
          vertex[4] = tmp_uv[ tmp_index[i][(p*3)+1]-1 ][1];
        } else {
          vertex[3] = 0.0f;
          vertex[4] = 0.0f;
        }
        if(tmp_index[i][(p*3)+1] != -1) {
          vertex[5] = tmp_norm[ tmp_index[i][(p*3)+2]-1 ][0];
          vertex[6] = tmp_norm[ tmp_index[i][(p*3)+2]-1 ][1];
          vertex[7] = tmp_norm[ tmp_index[i][(p*3)+2]-1 ][2];
        } else {
          vertex[5] = 0.0f;
          vertex[6] = 0.0f;
          vertex[7] = 0.0f;
        }
        out_vert.push_back(vertex);
      }
    }
    in_file.close();
  }
  else cerr << "Error opening the input or output file" << endl;
}

int makeShader(const char *vSource, const char *fSource, GLuint & shaderId) {
    // Compile vertex shader
    GLuint vertId = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertId, 1, &vSource, NULL);
    glCompileShader(vertId);
    GLint vStatus;
    glGetShaderiv(vertId, GL_COMPILE_STATUS, &vStatus);
    if(vStatus != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertId, 512, NULL, buffer);
        std::cerr << buffer << std::endl;
        return -2;
    }

    // Compile fragment shader
    GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragId, 1, &fSource, NULL);
    glCompileShader(fragId);
    GLint fStatus;
    glGetShaderiv(fragId, GL_COMPILE_STATUS, &fStatus);
    if(fStatus != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(fragId, 512, NULL, buffer);
        std::cerr << buffer << std::endl;
        return -3;
    }

    // Make shader program.
    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertId);
    glAttachShader(shaderId, fragId);
    glBindFragDataLocation(shaderId, 0, "outColor");
    glLinkProgram(shaderId);
    glUseProgram(shaderId);
    return 0;
}
