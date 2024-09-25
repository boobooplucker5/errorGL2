const char* vertex =
"#version 330 core\r\n"
"layout(location = 0) in vec3 pos;"
"layout(location = 1) in vec2 uv;"
"layout(location = 2) in vec3 norm;"

"uniform mat4 projmat;"
"uniform mat4 viewmat;"

"out vec2 textcord;"
"out vec3 color;"

"out vec3 fragpos;"
"out vec3 normal;"

"void main()"
"{"


"gl_Position = projmat*viewmat*vec4(pos,1);"

"textcord = uv;"
"color = vec3(0,0,0);"
"fragpos = pos;"
"normal = norm;"

" }";
const char* frag =
"#version 330 core\r\n"
"out vec4 fragcolor;"

"uniform vec3 campos;"

"in vec3 color;"
"in vec2 textcord;"

"in vec3 fragpos;"
"in vec3 normal;"

"uniform sampler2D text;"



"void main()"
"{"
"float lightpower = 15;"
"vec3 lightdirection = normalize(vec3(3,3,3) - fragpos);"
"float distance = length(vec3(3,3,3) - fragpos);"
"float diffuse = max(dot(lightdirection,normal),0) / (distance/(lightpower/5));"

"vec3 viewdirection = normalize(campos - fragpos);"
"vec3 reflectdirection = reflect(-lightdirection,normal);"
"vec3 lightcolor = vec3(1,0,0);"
"float specular = pow(max(dot(viewdirection,reflectdirection),0),2)*(lightpower/distance/10);"
"vec3 light = vec3( (diffuse + specular)*lightcolor.x,(diffuse + specular)*lightcolor.y,(diffuse + specular)*lightcolor.z);"
"fragcolor = texture2D(text, textcord)*vec4(color.x+0.2,color.y+0.2,color.z+0.2,1)+vec4(light,1);"

"}";