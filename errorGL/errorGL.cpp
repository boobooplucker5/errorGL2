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
#include <omp.h>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
int drawcalls = 0;
int drawinc = 0;
float camx = 0;
float camy = 0;
float camz = 0;
int keyholds[] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
int keynums[] = { 87,65,83,68,81,69,82,84,70,71,32,340 };
float camradx = 0;
float camrady = 0;
float t = 1;
glm::vec3 campos = glm::vec3(0, 0, 0);
glm::vec3 camfront = glm::vec3(0,0,1);
glm::vec3 camup = glm::vec3(0, 1, 0);
glm::vec3 camright = glm::vec3(1, 0, 0);
float inc= 1;
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

unsigned int* draws = new unsigned int[256];
void makedrawobj(float *vertices, unsigned int* indices, int svert, int sind, unsigned int& vbo, unsigned int& vao, unsigned int& ebo)
{
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, svert, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sind, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
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
        campos.y -= 0.001f*inc;
    break;
    case 340:
        campos.y += 0.001f*inc;
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
    if (action == GLFW_PRESS)
    {
      //  cout << key << "\n";
            for (int i = 0; i < sizeof(keynums) / 4; i++)
            {
                if (key == keynums[i])
                {
                    keyholds[i] = 1;
                }
            }
        
    }
   
    if (action == GLFW_RELEASE)
    {

        for (int i = 0; i < sizeof(keynums) / 4; i++)
        {
            if (key == keynums[i])
            {
                keyholds[i] = 0;
            }
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
void keystuff()
{
    for (int i = 0; i < sizeof(keynums) / 4; i++)
    {
        if (keyholds[i] == 1)
        {

            move(keynums[i]);
        }
    }

};
void drawobjects(unsigned int* buffarray, unsigned int drawcalls)
{
    for (int i = 0; i < drawcalls; i+=3)
    {
      
        glBindVertexArray(buffarray[i]);
        glBindTexture(GL_TEXTURE_2D, buffarray[i+2]);
        glDrawElements(GL_TRIANGLES,buffarray[i+1], GL_UNSIGNED_INT, 0);
        
    }

};

void renderobjects(unsigned int* buffarray, unsigned int rendercalls, float** totalverts)
{
    for (unsigned int i = 0; i < rendercalls; i++)
    {
        matt3 boi;



        boi = { float(sin(t)),0,float(-cos(t)),0,1,0,float(cos(t)),0,float(sin(t)) };
        boi.vertmult(totalverts[i*2], totalverts[i*2+1], buffarray[i*3+2]/4);
        glBindVertexArray(buffarray[i*3]);
        glBindBuffer(GL_ARRAY_BUFFER, buffarray[i*3+1]);
        glBufferData(GL_ARRAY_BUFFER, buffarray[i*3+2], totalverts[i*2+1], GL_STATIC_DRAW);
 
    }
   // 0 0 1 1
   // 0 1 2 3
   // 0 2 4 6
}
void adddraw(unsigned int vao, unsigned int isize, unsigned int text)
{
    draws[drawcalls] = vao;
    draws[drawcalls+1] = isize;
    draws[drawcalls+2] = text;
    drawcalls+=3;
}
void addrender()
{

}

int main()                                        
{

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); 
    mesh booboo2;
    mesh booboo;
    double start = omp_get_wtime();
 
//booboo.objload("meshes/gunno.txt");
//booboo.convert("meshes/sample.bin");
 booboo.load("meshes/sample.bin");
 for (int i = 0; i < booboo.tvsize / 4; i+=8)
 {
     booboo.verts[i] /= 100;
     booboo.verts[i+1] /= 100;
     booboo.verts[i+2] /= 100;
 }

  //  booboo2.objload("meshes/crash.txt");
   // booboo2.convert("meshes/crash.bin");
 booboo2.load("meshes/crash.bin");
    double finish = omp_get_wtime();
   cout << finish - start << "\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glm::mat4 projmat = glm::perspective(glm::radians(120.0f), 1.0f, 0.001f, 1000.0f);
    glm::mat4 transmat = glm::translate(glm::mat4(1.0f), glm::vec3(0,-20,0));
    glm::mat4 viewmat = glm::lookAt(campos,campos+camfront,camup);
  
  
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
    glUniform1i(glGetUniformLocation(frameShader.program, "ourTexture"), 0);


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
    

    unsigned int svbo, svao, sebo;
    makedrawobj(vertices,indices,144,24,svbo,svao,sebo);
    unsigned int vbo, vao, ebo;
    makedrawobj(booboo.verts, booboo.index,booboo.tvsize,booboo.tisize, vbo, vao, ebo);
    unsigned int vbo2, vao2, ebo2;
    makedrawobj(booboo2.verts, booboo2.index, booboo2.tvsize, booboo2.tisize, vbo2, vao2, ebo2);
  

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

    matt3 tran;
    tran.empty();




    float s = 0.0f;
    GLuint tlocation = glGetUniformLocation(frameShader.program, "t");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_CLAMP);
  
    glCullFace(GL_BACK);
    adddraw(vao,booboo.tisize,text.id);
    adddraw(vao2, booboo2.tisize, text.id);
    while (!glfwWindowShouldClose(window))
    {

        t += 0.001;
        matt3 rotmatx = {sin(t),0,-cos(t),0,1,0,cos(t),0,sin(t)};
        unsigned int rendersize = (booboo.tisize+booboo2.tisize)/4;
        unsigned int* buffarray2 = new unsigned int[drawcalls * 3];
        unsigned int renderinc = 0;
        float** renders = new float* [4];
        renders[0] = booboo.verts;
        renders[1] = booboo.tverts;
        renders[2] = booboo2.verts;
        renders[3] = booboo2.tverts;

        buffarray2[renderinc] = vao;
        buffarray2[renderinc + 1] = vbo;
        buffarray2[renderinc+2] = booboo.tvsize;
        renderinc+=3;
        buffarray2[renderinc] = vao2;
        buffarray2[renderinc + 1] = vbo2;
        buffarray2[renderinc + 2] = booboo2.tvsize;
        renderinc += 3;

 
    
          
        


        camfront.x = cos(camradx) * cos(camrady);
        camfront.y = sin(camrady);
        camfront.z = sin(camradx) * cos(camrady);
        camfront = normalize(camfront);
        camright = normalize(cross(glm::vec3(0,1,0),camfront));
        camup = normalize(cross(camfront,camright));
        viewmat = lookAt(campos, campos + camfront, camup);

        ifs(window);
        keystuff();

       

     //   drawinc+=3;
       // draws[drawinc] = vao2;
      //  draws[drawinc+1] = booboo2.tisize / 4;
     //   draws[drawinc+2] = text.id;
     //   drawinc+=3;
        glUniform1f(tlocation, 3 * 0.00125);
       

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);


        glClearColor(0.2f, 0.1f, 0.4f, 1.0f);         

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transmat = glm::translate(glm::mat4(1.0f), glm::vec3(camx,camy,camz));

        glUseProgram(defaultShader.program);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
 
        glUniformMatrix4fv(glGetUniformLocation(defaultShader.program, "projmat"), 1, GL_FALSE, &projmat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(defaultShader.program, "transmat"), 1, GL_FALSE, &transmat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(defaultShader.program, "viewmat"), 1, GL_FALSE, &viewmat[0][0]);
    

       
        drawobjects(draws, drawcalls);


        renderobjects(buffarray2, 2, renders);
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
     
      glBindVertexArray(svao);

      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


      pxpos = xpos;
      pypos = ypos;
  
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &svao);
    glDeleteBuffers(1, &svbo);
    glDeleteBuffers(1, &sebo);

 
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
