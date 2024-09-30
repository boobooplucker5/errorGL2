#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <vector>

#include "classes/shader.h"
#include "classes/vec3.h"
#include "classes/mesh.h"
#include "classes/texture.h"
#include "classes/drawobj.h"
#include "classes/matt3.h"
#include "classes/ui.h"
#include "classes/text.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <thread>
#include <omp.h>
#include <algorithm>
#include <fstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include "ft2build.h"
#include "classes/character.h"

#include FT_FREETYPE_H
#define STB_IMAGE_IMPLEMENTATION

int drawcalls = 0;
int drawinc = 0;
text* typingtext;
vector<unsigned int> keyholds = {};
float camradx = 0;
float camrady = 0;
float t = 0;
glm::vec3 campos = glm::vec3(0, 0, 0);
glm::vec3 camfront = glm::vec3(0, 0, 1);
glm::vec3 camup = glm::vec3(0, 1, 0);
glm::vec3 camright = glm::vec3(1, 0, 0);
glm::mat4 projmat;
glm::mat4 transmat;
glm::mat4 viewmat;

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
extern const char* uivertex;
extern const char* uifrag;

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

void makedrawobj2(text& bruh)
{

    for (int i = 0; i < bruh.textstring.length(); i++)
    {
        drawobj obj;
        glGenBuffers(1, &obj.vbo);
        glGenVertexArrays(1, &obj.vao);
        glGenBuffers(1, &obj.ebo);
        glBindVertexArray(obj.vao);
        glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
        glBufferData(GL_ARRAY_BUFFER, 64, bruh.vertices[i], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24, bruh.indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        bruh.drawobjects[i] = obj;


    }

}
void drawtext(text bruh, shader uiShader)
{
    for (int i = 0; i < bruh.textstring.length(); i++)
    {
        glBindVertexArray(bruh.drawobjects[i].vao);
        glUniform3f(glGetUniformLocation(uiShader.program, "color"), bruh.textcolor.x, bruh.textcolor.y, bruh.textcolor.z);
        glBindTexture(GL_TEXTURE_2D, bruh.characters[bruh.textstring.at(i)].glyphid);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
void updatetext(text& bruh,string newtext)
{
    bruh.textstring = newtext;
    delete[] bruh.vertices;
    bruh.settext();
    delete[] bruh.drawobjects;
    bruh.drawobjects = new drawobj[bruh.textstring.length()];
    makedrawobj2(bruh);
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
void adddraw(unsigned int vao, unsigned int isize, unsigned int textur)
{
    draws[drawcalls] = vao;
    draws[drawcalls + 1] = isize;
    draws[drawcalls + 2] = textur;
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
void cameramanage()
{

    camfront.x = cos(camradx) * cos(camrady);
    camfront.y = sin(camrady);
    camfront.z = sin(camradx) * cos(camrady);

    camfront = normalize(camfront);
    camright = normalize(cross(glm::vec3(0, 1, 0), camfront));
    camup = normalize(cross(camfront, camright));

    viewmat = lookAt(campos, campos + camfront, camup);
    projmat = glm::perspective(glm::radians(90.0f), 1.0f, 0.001f, 1000.0f);
    viewmat = glm::lookAt(campos, campos + camfront, camup);
    transmat = glm::translate(glm::mat4(1.0f), campos);
}
using namespace ImGui;
int main()
{
    
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    mesh booboo2;
    mesh booboo;

    FT_Library ft;
    FT_Face face;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }
    if (FT_New_Face(ft, "fonts/comic.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 100);
    if (FT_Load_Char(face, 'f', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return -1;
    }

    double start = omp_get_wtime();

    booboo.objload("meshes/cubi.txt");
    // booboo.objload("meshes/hqsphere.txt");
      //booboo.convert("meshes/spere.bin");as
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

    text bruh;
    bruh.centered = true;

    GLFWwindow* window = glfwCreateWindow(800, 800, "window", NULL, NULL);


    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, ifpressed);
    glfwSetMouseButtonCallback(window, ifclicked);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char i = 0; i < 128; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int txt;
        glGenTextures(1, &txt);

        glBindTexture(GL_TEXTURE_2D, txt);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        character chter;
        chter.glyphid = txt;
        chter.size = vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        chter.bearing = vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        chter.advance = face->glyph->advance.x;
        bruh.characters[i] = chter;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& inout = GetIO();
    StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");


    shader defaultShader;
    shader frameShader;
    shader uiShader;
    defaultShader.createShader(vertex, frag);
    frameShader.createShader(fvertex, ffrag);
    uiShader.createShader(uivertex, uifrag);
    glUseProgram(uiShader.program);
    glUniform1i(glGetUniformLocation(uiShader.program, "text"), 0);
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
    bruh.position = vec2(0, 0);
    bruh.textstring = "hello world.";
    glUseProgram(uiShader.program);
    bruh.settext();

    bruh.textcolor = vec3(1, 1, 1);
    makedrawobj2(bruh);
    glUseProgram(defaultShader.program);

    drawobj sobj = makedrawobj(vertices, indices, 144, 24);
    drawobj obj = makedrawobj(booboo.verts, booboo.index, booboo.tvsize, booboo.tisize);
 


    

    texture textu("Textures/NULL.png");
    texture textu2("Textures/scrap.png");
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
    glUniform3f(glGetUniformLocation(defaultShader.program, "lightpos"), 1.2, 1.4, 1.2);
    adddraw(obj.vao, booboo.tisize, textu.id);
    
    while (!glfwWindowShouldClose(window))
    {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        NewFrame();


        t += 0.001;
        
        for (int i = 0; i < keyholds.size(); i++)
        {
            keystuff(keyholds[i]);
        }

        unsigned int rendersize;
        unsigned int* buffarray2 = new unsigned int[drawcalls * 3];
        unsigned int renderinc = 0;
        renderinc += 2;
        float** renders = new float* [4];
        renderinc = 0;
        addrender(buffarray2, renderinc, obj.vao, obj.vbo, booboo.tvsize, renders, booboo.verts, booboo.tverts);


        ifs(window);

        cameramanage();


        glUniform1f(tlocation, 1 * 0.00125);


        glBindFramebuffer(GL_FRAMEBUFFER, fbo);


        glClearColor(0.2f, 0.1f, 0.4f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(defaultShader.program);


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);



        glUniformMatrix4fv(glGetUniformLocation(defaultShader.program, "projmat"), 1, GL_FALSE, &projmat[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(defaultShader.program, "viewmat"), 1, GL_FALSE, &viewmat[0][0]);
        glUniform3f(glGetUniformLocation(defaultShader.program, "campos"), campos.x, campos.y, campos.z);


        drawobjects(draws, drawcalls);



        glBindTexture(GL_TEXTURE_2D, textu.id);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glUseProgram(frameShader.program);

        glBindVertexArray(sobj.vao);

        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(uiShader.program);
        
        drawtext(bruh, uiShader);


        glUseProgram(frameShader.program);

        pxpos = xpos;
        pypos = ypos;
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();

    }



    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();
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
