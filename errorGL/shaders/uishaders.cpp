const char* uivertex =
"#version 330 core\r\n"
"layout(location = 0) in vec2 pos;"
"layout(location = 1) in vec3 clr;"
"out vec3 color;"


"void main()"
"{"

"gl_Position = vec4(pos,0,1);"
"color = clr;"

" }";
const char* uifrag =
"#version 330 core\r\n"

"out vec4 fragcolor;"

"in vec3 color;"

//"uniform sampler2D text;"
//"uniform vec3 color;"
"void main()"
"{"

"fragcolor = vec4(color,1);"

"}";