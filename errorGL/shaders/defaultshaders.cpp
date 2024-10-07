const char* vertex =
"#version 330 core\r\n"
"layout(location = 0) in vec3 pos;"
"layout(location = 1) in vec2 uv;"
"layout(location = 2) in vec3 norm;"

"uniform mat4 projmat;"
"uniform mat4 viewmat;"
"uniform vec3 lightpos;"

"out vec2 textcord;"
"out vec3 color;"

"out vec3 fragpos;"
"out vec3 normal;"
"out float diffuse;"

"void main()"
"{"

//"diffuse = max(dot(normalize(lightpos-pos),normal),0) / (distance/(5/5));"

"gl_Position = projmat*viewmat*vec4(pos,1);"

"textcord = uv;"
"color = vec3(0,0,0);"
"fragpos = pos;"
"normal = norm;"
//"diffuse = 0;"
"diffuse = max(dot(normalize(lightpos-pos),normal),0);"
" }";
const char* frag =
"#version 330 core\r\n"
"out vec4 fragcolor;"

"uniform vec3 campos;"
"uniform vec3 lightpos;"

"in vec3 color;"
"in vec2 textcord;"

"in vec3 fragpos;"
"in vec3 normal;"
"in float diffuse;"

"uniform sampler2D text;"



"void main()"
"{"
"float lightpower = 50;"
"vec3 lightdirection = normalize(lightpos - fragpos);"
"float distance = length(lightpos - fragpos);"


"vec3 viewdirection = normalize(campos - fragpos);"
"vec3 reflectdirection = reflect(-lightdirection,normal);"
"vec3 lightcolor = vec3(0,0,1);"
"float specular = pow(max(dot(viewdirection,reflectdirection),0),2)*(lightpower/distance/10)*2;"
"vec3 light = vec3( (diffuse + specular)*lightcolor.x,(diffuse + specular)*lightcolor.y,(diffuse + specular)*lightcolor.z);"
"fragcolor = texture2D(text, textcord)*vec4(color.x+0.2,color.y+0.2,color.z+0.2,1)+vec4(light,1);"

"}";