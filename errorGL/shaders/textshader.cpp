const char* textvertex =
"#version 330 core\r\n"
"layout(location = 0) in vec2 pos;"
"layout(location = 1) in vec2 uv;"

"out vec2 textcord;"


"void main()"
"{"

"gl_Position = vec4(pos,0,1);"
"textcord = uv;"

" }";
const char* textfrag =
"#version 330 core\r\n"

"out vec4 fragcolor;"

"in vec2 textcord;"

"uniform sampler2D text;"
"uniform vec3 color;"
"void main()"
"{"
"vec4 textcolor = texture(text,vec2(textcord.x,1-textcord.y));"
"if(textcolor.x < 1)"
"discard;"
"fragcolor = vec4(textcolor.x * color,1);"

"}";