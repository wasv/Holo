#define SHADER_VERT3D \
"#version 330 \n"\
"in  vec3 position;\n"\
"in  vec2 texcoord;\n"\
"in  vec3 normal;\n"\
"out vec2 Texcoord;\n"\
"out vec3 Position_world;\n"\
"out vec3 Normal;\n"\
"out vec3 EyeDir;\n"\
"out vec3 LightDir;\n"\
"uniform mat4 model;\n"\
"uniform mat4 view;\n"\
"uniform mat4 proj;\n"\
"uniform vec3 lighting;\n"\
"void main(void) { "\
"     Texcoord              = texcoord;\n"\
"     Position_world        = (model * vec4(position, 1.0)).xyz;\n"\
"     vec3 Position_camera  = (view * model * vec4(position, 1.0)).xyz;\n"\
"     EyeDir                = vec3(0.0, 0.0, 0.0) - Position_camera;\n"\
"     vec3 LightPos_camera  = (view * vec4(lighting, 1.0)).xyz;\n"\
"     LightDir              = LightPos_camera + EyeDir;\n"\
"     Normal                = (view * model * vec4(normal,0)).xyz;\n"\
"     gl_Position           = proj * view * model * vec4(position, 1.0);\n"\
"}"

#define SHADER_FRAG_SIMPLE \
"#version 330 core\n"\
"in  vec2 Texcoord;\n"\
"in  vec3 Position_world;\n"\
"in  vec3 Normal;\n"\
"in  vec3 EyeDir;\n"\
"in  vec3 LightDir;\n"\
"out vec3 outColor;\n"\
"uniform sampler2D tex;\n"\
"void main()"\
"{"\
"     outColor = texture(tex, Texcoord).xyz;\n"\
"}"

#define SHADER_FRAG_LIGHTING \
"#version 330 core\n"\
"in  vec2 Texcoord;\n"\
"in  vec3 Position_world;\n"\
"in  vec3 Normal;\n"\
"in  vec3 EyeDir;\n"\
"in  vec3 LightDir;\n"\
"out vec3 outColor;\n"\
"uniform sampler2D tex;\n"\
"uniform vec3 lighting;\n"\
"void main()"\
"{"\
"     vec3  LightColor  = vec3(1,1,1);\n"\
"     float LightPower  = 5.0f;\n"\
"     vec3  mtlDiffuse  = texture(tex, Texcoord).xyz;\n"\
"     vec3  mtlAmbient  = vec3(0.3, 0.3, 0.3) * mtlDiffuse;\n"\
"     vec3  mtlSpecular = vec3(0.3, 0.3, 0.3);\n"\
"     float distance    = length(lighting - Position_world);\n"\
"     vec3  n           = normalize(Normal);\n"\
"     vec3  l           = normalize(LightDir);\n"\
"     float cosTheta    = clamp( dot(n,l), 0,1);\n"\
"     vec3  E           = normalize(EyeDir);\n"\
"     vec3  R           = reflect(-l,n);\n"\
"     float cosAlpha    = clamp( dot(E,R), 0,1);\n"\
"     outColor = mtlAmbient +\n"\
"       mtlDiffuse * LightColor * LightPower * cosTheta / (distance*distance) +\n"\
"       mtlSpecular * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);\n"\
"}"
