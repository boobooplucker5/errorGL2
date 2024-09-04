const char* fvertex =
"#version 330 core\r\n"
"layout(location = 0) in vec2 aPos;"
"layout(location = 1) in vec2 aTexCoord;"


"out vec2 pos;"

"void main()"
"{"
"gl_Position = vec4(aPos.x,aPos.y,0.0, 1.0);"

"pos = aTexCoord;"
" }";
const char* ffrag =
"#version 330 core\r\n"
"out vec4 FragColor;"


"in vec2 pos;"

"uniform sampler2D ourTexture;"
"uniform float t;"

"void main()"
"{"



"float tx = t * floor(pos.x / t);"
"float ty = t * floor(pos.y / t);"


" FragColor = texture2D(ourTexture, vec2(tx,ty));"

"}";
