﻿//
// Created by captain on 2021/3/25.
//

#ifndef MESHVIEWER_SHADERSOURCE_H
#define MESHVIEWER_SHADERSOURCE_H

//顶点着色器代码
static const char* vertex_shader_text =
"#version 330\n"

"uniform mat4 u_mvp;\n"

"in vec3 a_pos;\n"
"in vec4 a_color;\n"

"out vec4 v_color;\n"

"void main()\n"
"{\n"
"    gl_Position = u_mvp * vec4(a_pos, 1.0);\n"
"    v_color = a_color;\n"
"}\n";
//片段着色器代码
static const char* fragment_shader_text =
"#version 330\n"
"in vec4 v_color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = v_color;\n"
"}\n";

#endif //UNTITLED_SHADERSOURCE_H
//一旦在顶点着色器中处理过后（图元，形状）就会标准化范围至-1到1
//gl_position是顶点着色器的输出，是vec4,因此在三维坐标加上1.0f
