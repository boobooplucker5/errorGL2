const char* uivertex =
"#version 330 core\r\n"
"layout(location = 0) in vec3 pos;"
"layout(location = 1) in vec2 uv;"

"out vec2 textcord;"


"void main()"
"{"

"gl_Position = pos;"
"textcord = uv;"

" }";
const char* uifrag =
"#version 330 core\r\n"
"out vec4 fragcolor;"


"in vec2 textcord;"

"uniform sampler2D text;"

"void main()"
"{"

"fragcolor = texture2D(text, textcord);"

"}";