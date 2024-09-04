const char* vertex =
"#version 330 core\r\n"
"layout(location = 0) in vec3 pos;"
"layout(location = 1) in vec2 uv;"
"layout(location = 2) in vec3 color;"

"uniform mat4 projmat;"
"uniform mat4 viewmat;"
"out vec2 TexCoord;"
"out vec3 Color;"

"void main()"
"{"

"vec4 glpos = projmat*viewmat*vec4(pos.x,pos.y,pos.z,1);"
"gl_Position = glpos;"
"Color = color;"
"TexCoord = uv;"

" }";
const char* frag =
"#version 330 core\r\n"
"out vec4 FragColor;"

"in vec3 Color;"
"in vec2 TexCoord;"

"uniform sampler2D ourTexture;"


"void main()"
"{"
" FragColor = (texture2D(ourTexture, TexCoord))*vec4(Color,1);"

"}";
