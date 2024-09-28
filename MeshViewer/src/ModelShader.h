//
// Created by captain on 2021/3/25.
//

#ifndef MESHVIEWER_MODELSHADER_H
#define MESHVIEWER_MODELSHADER_H

//顶点着色器代码
static const char* mvertex_shader_text =
"#version 330\n"

//将局部坐标转换成世界坐标
"uniform mat4 u_mvp;\n"
"uniform mat4 model;\n"

"in vec3 a_pos;\n"
"in vec3 a_normal;\n"
"out vec3 Normal;\n"
"out vec3 FragPos;\n"

"void main()\n"
"{\n"
"    gl_Position = u_mvp * vec4(a_pos, 1.0);\n"
"    //Normal = a_normal;\n"
"    Normal = mat3(transpose(inverse(model))) * a_normal;\n"
"    FragPos = vec3(model * vec4(a_pos, 1.0));\n"
"}\n";
//片段着色器代码
static const char* mfragment_shader_text =
"#version 330\n"
"struct Light {\n"
"    vec3 direction;\n"
"    vec3 ambient;\n"
"    vec3 diffuse;\n"
"    vec3 specular;\n"
"};\n"

"in vec3 Normal;\n"
"in vec3 FragPos;\n"

"uniform Light light;\n"
"uniform vec3 viewPos;\n"

//物体的颜色是灰色
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   vec3 objectColor = vec3(0.5, 0.5, 0.5);\n"
"	// ambient\n"
"	vec3 ambient = light.ambient * objectColor;\n"
"	// diffuse\n"
"	vec3 norm = normalize(Normal);\n"
"	vec3 lightDir = normalize(-light.direction);\n"
"	float diff = max(dot(norm, lightDir), 0.0);\n"
"	vec3 diffuse = light.diffuse * diff * objectColor;\n"
"	// specular\n"
"	vec3 viewDir = normalize(viewPos - FragPos);\n"
"	vec3 reflectDir = reflect(-lightDir, norm);\n"
"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"	vec3 specular = light.specular * spec * objectColor;\n"

"	vec3 result = ambient + diffuse + specular;\n"
"	FragColor = vec4(result, 1.0);\n"
"}\n";

#endif //UNTITLED_SHADERSOURCE_H
