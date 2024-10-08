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
#include "classes/object.h"


#include "classes/texture.h"

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

vector<unsigned int> keyholds = {};
vector<text*> typingtexts;
bool caps = true;
bool shift = false;
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

float inc = 30;
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
extern const char* textvertex;
extern const char* textfrag;

unsigned int* draws = new unsigned int[32176];
void objdrawobj(object& obj)
{
  
    glGenBuffers(1, &obj.drawobject.vbo);
    glGenVertexArrays(1, &obj.drawobject.vao);
    glGenBuffers(1, &obj.drawobject.ebo);
    glBindVertexArray(obj.drawobject.vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj.drawobject.vbo);
    glBufferData(GL_ARRAY_BUFFER, obj.objectmesh.tvsize, obj.objectmesh.verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.drawobject.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.objectmesh.tisize, obj.objectmesh.index, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


}
void uidrawobj(ui& obj)
{

    glGenBuffers(1, &obj.drawobject.vbo);
    glGenVertexArrays(1, &obj.drawobject.vao);
    glGenBuffers(1, &obj.drawobject.ebo);
    glBindVertexArray(obj.drawobject.vao);
    glBindBuffer(GL_ARRAY_BUFFER, obj.drawobject.vbo);
    glBufferData(GL_ARRAY_BUFFER, 80, obj.vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.drawobject.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24, obj.indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);



}
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
void updatetext(text& bruh)
{
    if (bruh.textstring.length() > 0)
    {
        delete[] bruh.vertices;
        bruh.settext();
        delete[] bruh.drawobjects;
        bruh.drawobjects = new drawobj[bruh.textstring.length()];
        makedrawobj2(bruh);
    }

}
void updatetype(text& bruh, int key)
{
    if (bruh.textstring.length() > 0)
    {
        delete[] bruh.vertices;
        bruh.settext();
        for (int i = 0; i < bruh.textstring.length(); i++)
        {
            glDeleteBuffers(1, &bruh.drawobjects->ebo);
            glDeleteBuffers(1, &bruh.drawobjects->vbo);
            glDeleteBuffers(1, &bruh.drawobjects->vao);
        }
        delete[] bruh.drawobjects;
        bruh.drawobjects = new drawobj[bruh.textstring.length()];
        makedrawobj2(bruh);
    }

}
void type(vector<text*> vec, int key)
{

    char typechar = (char)key;
    for (int i = 0; i < vec.size(); i++)
    {
        
        if (key != 259)
        {
            vec[i]->textstring += key;
            updatetext(*vec[i]);
        }
        else
        {
            if (vec[i]->textstring.length() > 0)
            {
                vec[i]->textstring.erase(vec[i]->textstring.length() - 1); 
            }
           
        }
      
     
    }
  
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
int keytype(int key)
{
    int newkey = key;
    char charkey = (char)key;
    if (shift == true)
    {
        switch (key)
        {
        case 96:
            newkey = 126;
            break;
        case 49:
            newkey = 33;
            break;
        case 50:
            newkey = 64;
            break;
        case 51:
            newkey = 35;
            break;
        case 52:
            newkey = 36;
            break;
        case 53:
            newkey = 37;
            break;
        case 54:
            newkey = 37;
            break;
        case 55:
            newkey = 94;
        case 56:
            newkey = 42;
            break;
        case 57:
            newkey = 40;
            break;
        case 48:
            newkey = 41;
            break;
        case 45:
            newkey = 95;
            break;
        case 61:
            newkey = 43;
            break;
        case 91:
            newkey = 123;
            break;
        case 93:
            newkey = 125;
            break;
        case 92:
            newkey = 124;
            break;
        case 59:
            newkey = 58;
            break;
        case 39:
            newkey = 34;
            break;
        case 44:
            newkey = 60;
            break;
        case 46:
            newkey = 62;
            break;
        case 47:
            newkey = 63;
            break;
        default:
            break;
        }
    }
    if (caps == true)
    {
        switch (charkey)
        {
        case 'Q':
            newkey = int('q');
        break;
        case 'W':
            newkey = int('w');
            break;
        case 'E':
            newkey = int('e');
            break;
        case 'R':
            newkey = int('r');
            break;
        case 'T':
            newkey = int('t');
            break;
        case 'Y':
            newkey = int('y');
            break;
        case 'U':
            newkey = int('u');
            break;
        case 'I':
            newkey = int('i');
            break;
        case 'O':
            newkey = int('o');
            break;
        case 'P':
            newkey = int('p');
            break;
        case 'A':
            newkey = int('a');
            break;
        case 'S':
            newkey = int('s');
            break;
        case 'D':
            newkey = int('d');
            break;
        case 'F':
            newkey = int('f');
            break;
        case 'G':
            newkey = int('g');
            break;
        case 'H':
            newkey = int('h');
            break;
        case 'J':
            newkey = int('j');
            break;
        case 'K':
            newkey = int('k');
            break;
        case 'L':
            newkey = int('l');
            break;
        case 'Z':
            newkey = int('z');
            break;
        case 'X':
            newkey = int('x');
            break;
        case 'C':
            newkey = int('c');
            break;
        case 'V':
            newkey = int('v');
            break;
        case 'B':
            newkey = int('b');
            break;
        case 'N':
            newkey = int('n');
            break;
        case 'M':
            newkey = int('m');
            break;
        }
    }
    
    return newkey;
}
void ifpressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (find(keyholds.begin(), keyholds.end(), key) != keyholds.end())
    {

        if (action == GLFW_RELEASE)
        {
   
            if (key == 344 || key == 340)
            {
                shift = false;
            }
            keyholds.erase(find(keyholds.begin(), keyholds.end(), key));
        }
    }
    else
    {
        if (action == GLFW_PRESS)
        {
            if (key == 280)
            {
                caps =  abs(caps-1);
            }
            if (key == 344 || key == 340)
            {
                shift = true;
            }
            if (key < 280)
            {
                type(typingtexts, keytype(key));
            }
           
            cout << (char)key << "\n";
            //cout << typingtexts.at(0).textstring << "\n";
          
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
FT_Face makefont(FT_Library& library, const char* fontdirectory, int size)
{
    FT_Face face;
    if (FT_Init_FreeType(&library))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << "\n";

    }
    if (FT_New_Face(library, fontdirectory, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << "\n";
      
    }
    FT_Set_Pixel_Sizes(face, 0, 100);
    if (FT_Load_Char(face, 'f', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << "\n";
     
    }
 
    return face;
}
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
void objadddraw(object obj, unsigned int textur)
{
    draws[drawcalls] = obj.drawobject.vao;
    draws[drawcalls + 1] = obj.objectmesh.tisize;
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
void objaddrender(unsigned int* buffarr, unsigned int& inc, float** totalverts, object obj)
{
    totalverts[inc / 3 * 2] = obj.objectmesh.verts;
    totalverts[inc / 3 * 2 + 1] = obj.objectmesh.verts;
    buffarr[inc] = obj.drawobject.vao;
    buffarr[inc + 1] = obj.drawobject.vbo;
    buffarr[inc + 2] = obj.drawobject.vsize;
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
    FT_Face face = makefont(ft, "fonts/comic.ttf",100);



    

    double start = omp_get_wtime();

  //  booboo.load("meshes/permaid.txt");
     //booboo.objload("meshes/sqrpyro.txt");
  //    booboo.convert("meshes/simple.bin");
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
    shader textShader;
    shader uiShader;
    defaultShader.createShader(vertex, frag);
    frameShader.createShader(fvertex, ffrag);
    textShader.createShader(textvertex, textfrag);
    uiShader.createShader(uivertex, uifrag);
    glUseProgram(textShader.program);
    glUniform1i(glGetUniformLocation(textShader.program, "text"), 0);
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
    glUseProgram(textShader.program);
    bruh.settext();

    bruh.textcolor = vec3(1, 1, 1);
    makedrawobj2(bruh);
    glUseProgram(defaultShader.program);

    object cubeobj;
    cubeobj.size = vec3(12, 12,12);
    cubeobj.position = vec3(24, 24, 24);
    cubeobj.create("meshes/cube.bin");
    objdrawobj(cubeobj);
    object sphereobj;
    sphereobj.size = vec3(12, 12, 12);
    sphereobj.position = vec3(0, 24, 48);
    sphereobj.create("meshes/cube.bin");
    objdrawobj(sphereobj);
    object pyrobj;
    pyrobj.size = vec3(12, 12, 12);
    pyrobj.position = vec3(0, 0, 24);
    pyrobj.create("meshes/cube.bin");
    objdrawobj(pyrobj);



    drawobj sobj = makedrawobj(vertices, indices, 144, 24);

   // cubeobj.drawobject = makedrawobj(cubeobj.objectmesh.verts, cubeobj.objectmesh.index, cubeobj.objectmesh.tvsize, cubeobj.objectmesh.tisize);




    texture textu("Textures/white.png");
    texture textu2("Textures/NULL.png");
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
//    glUniform3f(glGetUniformLocation(defaultShader.program, "lightpos"), 1.2, 1.4, 1.2);
  //  adddraw(obj.vao, booboo.tisize, textu.id);
   // objadddraw(cubeobj,textu2.id);
   // objadddraw(sphereobj, textu2.id);
   // objadddraw(pyrobj, textu2.id);
    //adddraw(cubeobj.drawobject.vao, cubeobj.objectmesh.tisize, textu.id);
    typingtexts.push_back(&bruh);
    vec3 lightpos(0,30,0);

    ui gol;
    gol.pos = vec2(0, 0);
    gol.size = vec2(0.5, 0.25);
    gol.color = vec3(1, 0, 0);
    gol.create();
    uidrawobj(gol);

    while (!glfwWindowShouldClose(window))
    {
        lightpos.z = 0;
        lightpos.y = (cos(t) - sin(t))*48;
        lightpos.x = (sin(t) + cos(t))*48;
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
        renderinc += 4;
        float** renders = new float* [8];
        renderinc = 0;
        //addrender(buffarray2, renderinc, obj.vao, obj.vbo, booboo.tvsize, renders, booboo.verts, booboo.tverts);
       // addrender(buffarray2, renderinc, cubeobj.drawobject.vao, cubeobj.drawobject.vbo,cubeobj.objectmesh.tvsize, renders, cubeobj.objectmesh.verts, cubeobj.objectmesh.tverts);
       // objaddrender(buffarray2,renderinc,renders,cubeobj);
       // objaddrender(buffarray2, renderinc, renders, sphereobj);
       // objaddrender(buffarray2, renderinc, renders, pyrobj);
        ifs(window);
        matt3 rot;
        
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
        glUniform3f(glGetUniformLocation(defaultShader.program, "lightpos"), lightpos.x, lightpos.y, lightpos.z);

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
        glBindVertexArray(gol.drawobject.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(textShader.program);
    
    
 
      //  
       drawtext(bruh, textShader);


        glUseProgram(frameShader.program);

        pxpos = xpos;
        pypos = ypos;
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        //typingtexts.clear();
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
