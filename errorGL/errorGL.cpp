#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>
#include <vector>
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "classes/shader.h"
#include "classes/vec3.h"
#include "classes/mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "classes/matt3.h"
#include <thread>
#include "classes/texture.h"
#include "classes/drawobj.h"
#include <omp.h>
#include <algorithm>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
int drawcalls = 0;
int drawinc = 0;
vector<unsigned int> keyholds = {};
//int keynums[] = { 87,65,83,68,81,69,82,84,70,71,32,340 };

float camradx = 0;
float camrady = 0;
float t = 5;
glm::vec3 campos = glm::vec3(0, 0, 0);
glm::vec3 camfront = glm::vec3(0, 0, 1);
glm::vec3 camup = glm::vec3(0, 1, 0);
glm::vec3 camright = glm::vec3(1, 0, 0);
float inc = 1;
int mouseholdr = 0;
double xpos = 800;
double ypos = 800;
double pxpos = 800;
double pypos = 800;
using namespace std;
//using namespace glm;
extern const char* vertex;
extern const char* frag;
extern const char* fvertex;
extern const char* ffrag;

unsigned int* draws = new unsigned int[32176];
drawobj makedrawobj(float* vertices, unsigned int* indices, int svert, int sind)
{
    drawobj obj;
    glGenBuffers(1, &obj.vbo);
    glGenVertexArrays(1, &obj.vao);
    glGenBuffers(1, &obj.ebo);
    glBindVertexArray(obj.vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
    glBufferData(GL_ARRAY_BUFFER, svert, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sind, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    return obj;

}
void move(int key)
{
    switch (key)
    {
    case 87:
        campos.x = campos.x + camfront.x / 1000 * inc;
        campos.y = campos.y + camfront.y / 1000 * inc;
        campos.z = campos.z + camfront.z / 1000 * inc;
        break;
    case 65:
        campos.x = campos.x + camright.x / 1000 * inc;
        campos.y = campos.y + camright.y / 1000 * inc;
        campos.z = campos.z + camright.z / 1000 * inc;
        break;
    case 83:
        campos.x = campos.x - camfront.x / 1000 * inc;
        campos.y = campos.y - camfront.y / 1000 * inc;
        campos.z = campos.z - camfront.z / 1000 * inc;
        break;
    case 68:
        campos.x = campos.x - camright.x / 1000 * inc;
        campos.y = campos.y - camright.y / 1000 * inc;
        campos.z = campos.z - camright.z / 1000 * inc;
        break;
    case 32:
        campos.y -= 0.001f * inc;
        break;
    case 340:
        campos.y += 0.001f * inc;
        break;
    case  82:
        camradx += 0.0005f;
        break;
    case 84:
        camradx -= 0.0005f;
        break;
    case 70:
        camrady += 0.0005f;
        break;
    case 71:
        camrady -= 0.0005f;
        break;
    default:
        break;
    }
}
void ifpressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   
    if (find(keyholds.begin(), keyholds.end(), key) != keyholds.end())
    {
       
        if (action == GLFW_RELEASE)
        {
            keyholds.erase(find(keyholds.begin(), keyholds.end(), key));
        }
    }
    else
    {
        if (action == GLFW_PRESS)
        {

            keyholds.push_back(key);
        }
    }

}
void ifclicked(GLFWwindow* window, int button, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            mouseholdr = 1;

        }
        break;

    case GLFW_RELEASE:
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {

            mouseholdr = 0;
        }
        break;

    default:
        break;
    }
}

void ifs(GLFWwindow* window)
{
    glfwGetCursorPos(window, &xpos, &ypos);

    if (xpos > 800 && mouseholdr == 1)
    {
        glfwSetCursorPos(window, 0, ypos);
        pxpos = 0;
        xpos = 0;
    }
    if (xpos < 0 && mouseholdr == 1)
    {
        glfwSetCursorPos(window, 800, ypos);
        pxpos = 800;
        xpos = 800;
    }
    if (ypos > 800 && mouseholdr == 1)
    {
        glfwSetCursorPos(window, xpos, 0);
        pypos = 0;
        ypos = 0;
    }
    if (ypos < 0 && mouseholdr == 1)
    {
        glfwSetCursorPos(window, xpos, 800);
        pypos = 800;
        ypos = 800;
    }
    if (pxpos - xpos != 0 && mouseholdr == 1)
    {

        camradx += (xpos - pxpos) / 300;
    }
    if (pypos - ypos != 0 && mouseholdr == 1)
    {

        camrady -= (ypos - pypos) / 300;
    }

    if (camrady < -1.57)
    {
        camrady = -1.57;
    }
    if (camrady > 1.57)
    {
        camrady = 1.57;
    }
};
void keystuff(int key)
{
    if (key == 87 | key == 65 | key == 83 | key == 68)
    {
        move(key);
    }
         
};
void drawobjects(unsigned int* buffarray, unsigned int drawcalls)
{
    for (int i = 0; i < drawcalls; i += 3)
    {

        glBindVertexArray(buffarray[i]);
        glBindTexture(GL_TEXTURE_2D, buffarray[i + 2]);
        glDrawElements(GL_TRIANGLES, buffarray[i + 1], GL_UNSIGNED_INT, 0);

    }

};
void renderobjects(unsigned int* buffarray, unsigned int rendercalls, float** totalverts, matt3 mat)
{
    for (unsigned int i = 0; i < rendercalls; i++)
    {





        mat.vertmult(totalverts[i * 2], totalverts[i * 2 + 1], buffarray[i * 3 + 2] / 4);
        glBindVertexArray(buffarray[i * 3]);
        glBindBuffer(GL_ARRAY_BUFFER, buffarray[i * 3 + 1]);
        glBufferData(GL_ARRAY_BUFFER, buffarray[i * 3 + 2], totalverts[i * 2 + 1], GL_STATIC_DRAW);

    }
    // 0 0 1 1
    // 0 1 2 3
    // 0 2 4 6
}
void adddraw(unsigned int vao, unsigned int isize, unsigned int text)
{
    draws[drawcalls] = vao;
    draws[drawcalls + 1] = isize;
    draws[drawcalls + 2] = text;
    drawcalls += 3;
}
void addrender(unsigned int* buffarr, unsigned int& inc, unsigned int vao, unsigned int vbo, unsigned int vsize, float** totalverts, float* verts, float* tverts)
{
    totalverts[inc / 3 * 2] = verts;
    totalverts[inc / 3 * 2 + 1] = tverts;
    buffarr[inc] = vao;
    buffarr[inc + 1] = vbo;
    buffarr[inc + 2] = vsize;
    inc += 3;
}

int main()
{

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    mesh booboo2;
    mesh booboo;
    double start = omp_get_wtime();

      booboo.objload("meshes/cubi.txt");
      // booboo.objload("meshes/hqsphere.txt");
        //booboo.convert("meshes/spere.bin");
    //booboo.load("meshes/sample.bin");

    //  booboo2.objload("meshes/crash.txt");
     // booboo2.convert("meshes/crash.bin");
    //booboo2.load("meshes/crash.bin");
    double finish = omp_get_wtime();
    cout << finish - start << "\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glm::mat4 projmat = glm::perspective(glm::radians(120.0f), 1.0f, 0.001f, 1000.0f);
    glm::mat4 transmat = glm::translate(glm::mat4(1.0f), glm::vec3(0, -20, 0));
    glm::mat4 viewmat = glm::lookAt(campos, campos + camfront, camup);


    GLFWwindow* window = glfwCreateWindow(800, 800, " i always have the hardest bugs ;-;", NULL, NULL);



    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, ifpressed);
    glfwSetMouseButtonCallback(window, ifclicked);



    shader defaultShader;
    shader frameShader;
    defaultShader.createShader(vertex, frag);
    frameShader.createShader(fvertex, ffrag);

    glUseProgram(frameShader.program);
    glUniform1i(glGetUniformLocation(frameShader.program, "text"), 0);


    float vertices[] = {

   1.0f,  1.0f, 1,      1.0f, 1.0f, 1.0f, 1.0f,  1.0f,
   1.0f, -1.0f, 1,     1.0f, 0.0f,  1.0f, 1.0f,  1.0f,
  -1.0f, -1.0f,  1,     0.0f, 0.0f,  1.0f, 1.0f,  1.0f,
  -1.0f,  1.0f, 1,   0.0f, 1.0f,   1.0f, 1.0f,  1.0f
    };



    unsigned int indices[] = {
        1, 0, 3,
        1, 2, 3
    };

    // unsigned int svbo, svao, sebo;

    drawobj sobj = makedrawobj(vertices, indices, 144, 24);
    //  unsigned int vbo, vao, ebo;
    drawobj obj = makedrawobj(booboo.verts, booboo.index, booboo.tvsize, booboo.tisize);
    // unsigned int vbo2, vao2, ebo2;
     //makedrawobj(booboo2.verts, booboo2.index, booboo2.tvsize, booboo2.tisize, vbo2, vao2, ebo2);


    texture text("Textures/white.png");
    texture text2("Textures/scrap.png");
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);



    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

   

    float s = 0.0f;
    GLuint tlocation = glGetUniformLocation(frameShader.program, "t");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_CLAMP);

    glCullFace(GL_BACK);

    adddraw(obj.vao, booboo.tisize, text.id);
    // adddraw(vao2, booboo2.tisize, text.id);
    while (!glfwWindowShouldClose(window))
    {

        t += 0.001;
        //keyholds.clear();
        for (int i = 0; i < keyholds.size(); i++)
        {
            keystuff(keyholds[i]);
        }

        unsigned int rendersize;
        unsigned int* buffarray2 = new unsigned int[drawcalls * 3];
        unsigned int renderinc = 0;
        float** renders = new float* [4];
       
        addrender(buffarray2, renderinc, obj.vao, obj.vbo, booboo.tvsize, renders, booboo.verts, booboo.tverts);
        //addrender(buffarray2, renderinc, vao2, vbo2, booboo2.tvsize, renders, booboo2.verts, booboo2.tverts);
        //
        camfront.x = cos(camradx) * cos(camrady);
        camfront.y = sin(camrady);
        camfront.z = sin(camradx) * cos(camrady);

        camfront = normalize(camfront);
        camright = normalize(cross(glm::vec3(0, 1, 0), camfront));
        camup = normalize(cross(camfront, camright));

        viewmat = lookAt(campos, campos + camfront, camup);

        ifs(window);
       



        glUniform1f(tlocation, 1 * 0.00125);


        glBindFramebuffer(GL_FRAMEBUFFER, fbo);


        glClearColor(0.2f, 0.1f, 0.4f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transmat = glm::translate(glm::mat4(1.0f), campos);

        glUseProgram(defaultShader.program);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glUniformMatrix4fv(glGetUniformLocation(defaultShader.program, "projmat"), 1, GL_FALSE, &projmat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(defaultShader.program, "viewmat"), 1, GL_FALSE, &viewmat[0][0]);
        glUniform3f(glGetUniformLocation(defaultShader.program, "campos"), campos.x, campos.y, campos.z);
        booboo.transmat = { float(sin(t)),0,float(-cos(t)),0,1,0,float(cos(t)),0,float(sin(t)) };

        drawobjects(draws, drawcalls);


        //renderobjects(buffarray2, 1, renders,booboo.transmat);
        //   glBindVertexArray(vao);
        //  glBindBuffer(GL_ARRAY_BUFFER, vbo);
         //glBufferData(GL_ARRAY_BUFFER, booboo.tvsize, booboo.verts, GL_STATIC_DRAW);
          //glBindBuffer(GL_ARRAY_BUFFER, 0);
          //glBindVertexArray(vao2);
         // glBindBuffer(GL_ARRAY_BUFFER, vbo2);
          //glBufferData(GL_ARRAY_BUFFER, booboo2.tvsize, booboo2.verts, GL_STATIC_DRAW);
          //glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, text.id);
        // glDrawElements(GL_TRIANGLES, booboo.tisize/4, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glUseProgram(frameShader.program);

        glBindVertexArray(sobj.vao);

        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        pxpos = xpos;
        pypos = ypos;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }




    glfwTerminate();


    // delete booboo.index;
     //delete booboo.vertex;
    // delete booboo.uvindex;
    // delete booboo.uvcord;
     //delete booboo.nmindex;
    // delete booboo.normal;
     //delete booboo.verts;
    return 0;
}