#include <glad/glad.h>
//glad管理函数指针
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform2.hpp>
#include<glm/gtx/euler_angles.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>

//创建图形界面的库
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Geometry.h"
#include "CObj.h"

#include "ShaderSource.h"
#include "ModelShader.h"
#include "Arcball/arcball.h"

using namespace std;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xposIn, double yposIn);


//GLuint是unsigned int的别名，GLint是int
int Width = 700, Height = 700;
GLFWwindow* window;
GLuint vertex_shader, fragment_shader, shader, mshader, lshader;
GLint mvp_location, vpos_location, vcol_location;
GLint m_mvp_location, m_vpos_location, m_vcol_location, m_vnor_location;
GLint light_ambient, light_specular, light_diffuse, light_dir, viewPos, m_model_location;
GLuint kVBO, kEBO;
GLuint kVAO;
glm::vec3* pos;
glm::vec4* color;
glm::vec3 cameraPos = glm::vec3(0, 0, 10);
unsigned int num = 0;
int id = 0;

bool lineMode = false;//初始化为false
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//鼠标交互状态变换
float lastX = 400, lastY = 300, lastZ = 0;
float dx = 0.0f, dy = 0.0f;
float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
bool firstMouse = true;



//坐标系变换 平移 旋转 缩放
glm::mat4 trans = glm::translate(glm::vec3(0, 0, 0)); //不移动顶点坐标;
glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0, 0, 1.0f)); //不旋转;
glm::quat q_rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
float scaleNum = 2.0f;
glm::mat4 scale = glm::scale(glm::vec3(scaleNum, scaleNum, scaleNum)); //缩放;
glm::mat4 model, view, projection;
glm::mat4 mvp;
glm::mat4 rotated_model;

//加载模型
CObj obj;
bool loaded = false;


//初始化OpenGL
void init_opengl()
{
    cout << "init opengl" << endl;

    //设置错误回调
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    //指定opengl版本号，通用3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //创建窗口对象，通用
    window = glfwCreateWindow(Width, Height, "MeshViewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    gladLoadGL();
    //设置垂直同步，避免画面撕裂
    glfwSwapInterval(1);


}

//编译、链接Shader
GLuint compile_shader(const char* vertex, const char* fragment)
{
    //创建顶点Shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //指定Shader源码 &vertex其实是类c源码，1是id,vertex_shader是创建的对象用于存储，后面再编译
    glShaderSource(vertex_shader, 1, &vertex, NULL);
    //编译Shader 顶点着色器
    glCompileShader(vertex_shader);
    //获取编译结果
    GLint compile_status = GL_FALSE;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        GLchar message[256];
        glGetShaderInfoLog(vertex_shader, sizeof(message), 0, message);
        cout << "compile vs error:" << message << endl;
    }

    //创建片段Shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //指定Shader源码
    glShaderSource(fragment_shader, 1, &fragment, NULL);
    //编译Shader
    glCompileShader(fragment_shader);
    //获取编译结果
    compile_status = GL_FALSE;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        GLchar message[256];
        glGetShaderInfoLog(fragment_shader, sizeof(message), 0, message);
        cout << "compile fs error:" << message << endl;
    }


    //创建GPU程序 链接顶点程序
    GLuint program = glCreateProgram();
    //附加Shader 顶点和片段链接程序 链接之后就可以删除顶点和片段着色器了
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    //Link
    glLinkProgram(program);
    //获取编译结果
    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        GLchar message[256];
        glGetProgramInfoLog(program, sizeof(message), 0, message);
        cout << "link error:" << message << endl;
    }
    return program;
}

void Init_imgui()
{
    ImGui::CreateContext();//初始化
    ImGui_ImplGlfw_InitForOpenGL(window, true);//绑定glfw窗口
    ImGui::StyleColorsDark();//设置主题样式为深色
    //ImGui::StyleColorsClassic();//设置主题样式为经典
    //ImGui::StyleColorsLight();//设置主题样式为浅色

    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

}

bool loadObjFile()
{
    OPENFILENAME ofn;       // 文件对话框结构体
    TCHAR szFile[260];       // 存储文件名的缓冲区

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("OBJ Files(*.OBJ)\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::cout << "Selected file: " << szFile << std::endl;
        obj.ReadObjFile(szFile);
        return true;
    }
    else {
        std::cout << "No file selected" << std::endl;
    }

    return false;
}

void drawModel()
{
    //TODO:设置绘制模型所用VAO，VBO
    // 生成和绑定 VAO（顶点数组对象）
    GLuint VAO, VBO, NBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 分配和初始化 VBO（顶点缓冲对象），用于存储顶点位置数据
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 创建一个顶点数组，存储模型的所有顶点
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    // 从 obj 中获取顶点和法线数据
    for (size_t i = 0; i < obj.m_pts.size(); i++) {
        vertices.push_back(glm::vec3(obj.m_pts[i].pos.fX, obj.m_pts[i].pos.fY, obj.m_pts[i].pos.fZ));
        normals.push_back(glm::vec3(obj.m_pts[i].normal.fX, obj.m_pts[i].normal.fY, obj.m_pts[i].normal.fZ));
    }

    // 从 obj 中获取面的索引
    for (size_t i = 0; i < obj.m_faces.size(); i++) {
        indices.push_back(obj.m_faces[i].pts[0] - 1);
        indices.push_back(obj.m_faces[i].pts[1] - 1);
        indices.push_back(obj.m_faces[i].pts[2] - 1);
    }

    // 将顶点数据传送到 GPU
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // 配置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 顶点位置属性

    // 创建法线缓冲对象并将法线数据传输到 GPU
    glGenBuffers(1, &NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

    // 配置法线属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); // 法线属性

    // 创建 EBO（元素缓冲对象），用于存储索引
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 启用深度测试，防止图像出现重叠问题
    glEnable(GL_DEPTH_TEST);

    // 使用已编译的 shader 程序
    glUseProgram(mshader);

    // 上传 MVP 矩阵和其他 uniforms
    glUniformMatrix4fv(m_mvp_location, 1, GL_FALSE, &mvp[0][0]);
    glUniform3fv(viewPos, 1, &cameraPos[0]);
    glUniformMatrix4fv(m_model_location, 1, GL_FALSE, &model[0][0]);


    glm::vec3 lightDir = glm::vec3(0.0f, -1.0f, -1.0f);
    glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

    //平型光照设置
    glUseProgram(mshader);
    glUniformMatrix4fv(m_mvp_location, 1, GL_FALSE, &mvp[0][0]);
    glUniform3fv(viewPos, 1, &cameraPos[0]);
    glUniformMatrix4fv(m_model_location, 1, GL_FALSE, &model[0][0]);
    glUniform3fv(light_dir, 1, &lightDir[0]);
    glUniform3fv(light_ambient, 1, &lightAmbient[0]);
    glUniform3fv(light_specular, 1, &lightSpecular[0]);
    glUniform3fv(light_diffuse, 1, &lightDiffuse[0]);


    //TODO:索引绘制
    // 使用 EBO 绘制模型（索引绘制）
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // 解绑 VAO
    glBindVertexArray(0);

    // 删除缓冲对象
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}


int main(void)
{
    init_opengl();

    //传入着色器源码
    shader = compile_shader(vertex_shader_text, fragment_shader_text);
    mshader = compile_shader(mvertex_shader_text, mfragment_shader_text);

    Init_imgui();

    //获取shader属性ID
    mvp_location = glGetUniformLocation(shader, "u_mvp");
    vpos_location = glGetAttribLocation(shader, "a_pos");
    vcol_location = glGetAttribLocation(shader, "a_color");

    m_mvp_location = glGetUniformLocation(mshader, "u_mvp");
    m_vpos_location = glGetAttribLocation(mshader, "a_pos");
    m_vnor_location = glGetAttribLocation(mshader, "a_normal");
    light_dir = glGetUniformLocation(mshader, "light.direction");
    light_ambient = glGetUniformLocation(mshader, "light.ambient");
    light_diffuse = glGetUniformLocation(mshader, "light.diffuse");
    light_specular = glGetUniformLocation(mshader, "light.specular");


    viewPos = glGetUniformLocation(mshader, "viewPos");
    m_model_location = glGetUniformLocation(mshader, "model");

    //创建几何体对象
    Quad* quad = new Quad();
    Triangle* triangle = new Triangle();
    Cube* cube = new Cube();
    Cone* cone = new Cone();
    Cylinder* cylinder = new Cylinder();


    while (!glfwWindowShouldClose(window))
    {
        cameraPos = glm::vec3(0, 0, 10);

        if (lineMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
        }

        float ratio;
        int width, height;


        //获取画面宽高
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        //视口大小和glfw一样大，前面两个是左下角的位置
        glViewport(0, 0, width, height);

        //实现imgui和opengl,glfw的交互
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        //可以开始引入按钮等
        ImGui::NewFrame();

        //渲染，一直存在，放在while
        //glclear清空屏幕，清空屏幕用的颜色是glClearColor中的颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);//设置背景颜色

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Setup");

            ImGui::Checkbox("Line Mode", &lineMode);      // Edit bools storing our window open/close state

            if (ImGui::Button("Triangle"))
                id = 0;
            if (ImGui::Button("Quad"))
                id = 1;
            if (ImGui::Button("Cube"))
                id = 2;
            if (ImGui::Button("Cone"))
                id = 3;
            if (ImGui::Button("Cylinder"))
                id = 4;
            if (ImGui::Button("Model"))
            {
                id = 5;
                loaded = loadObjFile();
            }


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        switch (id)
        {
        case 0:
            pos = triangle->getPos();
            color = triangle->getColor();
            num = triangle->getNum();
            break;
        case 1:
            pos = quad->getPos();
            color = quad->getColor();
            num = quad->getNum();
            break;
        case 2:
            pos = cube->getPos();
            color = cube->getColor();
            num = cube->getNum();
            break;
        case 3:
            pos = cone->getPos();
            color = cone->getColor();
            num = cone->getNum();
            break;
        case 4:
            pos = cylinder->getPos();
            color = cylinder->getColor();
            num = cylinder->getNum();
            break;
        case 5:
            break;
        default:
            cout << "wrong" << endl;
        }

        //坐标系变换
        if (id == 2)
            rotation = glm::rotate(rotation, glm::radians(1.0f), glm::vec3(0, 1, 0));

        scale = glm::scale(glm::vec3(scaleNum, scaleNum, scaleNum)); //缩放;

        model = trans * scale * rotation;

        view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        projection = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);

        mvp = projection * view * model;

        //绑定顶点数组绘制
        if (id != 5)
        {
            //指定GPU程序(就是指定顶点着色器、片段着色器)
            glUseProgram(shader);
            glEnable(GL_DEPTH_TEST);
            //glEnable(GL_CULL_FACE);//开启背面剔除

            //启用顶点Shader属性(a_pos)，指定与顶点坐标数据进行关联
            glEnableVertexAttribArray(vpos_location);
            glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(glm::vec3), pos);

            //启用顶点Shader属性(a_color)，指定与顶点颜色数据进行关联
            glEnableVertexAttribArray(vcol_location);
            glVertexAttribPointer(vcol_location, 3, GL_FLOAT, false, sizeof(glm::vec4), color);

            //上传mvp矩阵
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
            //上传顶点数据并进行绘制
            glDrawArrays(GL_TRIANGLES, 0, num);
        }
        //绘制模型
        else
        {
            if (loaded) drawModel();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //双缓冲可以防止出现闪烁问题
        glfwSwapBuffers(window);
        //检查是否有事件，如鼠标移动等+
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    //释放所有资源
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//glfwSetCursorPosCallback调用
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //TODO:添加鼠标控制
    // 检查鼠标按键状态
    int leftButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int rightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

    if (firstMouse)
    {
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }

    // 计算鼠标移动的偏移量
    float xoffset = xposIn - lastX;
    float yoffset = lastY - yposIn; // 反转yoffset是为了使旋转符合惯性
    lastX = xposIn;
    lastY = yposIn;

    // 左键按住时平移
    if (leftButtonState == GLFW_PRESS)
    {
        float sensitivity = 0.01f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // 在屏幕平面上进行平移
        trans = glm::translate(trans, glm::vec3(xoffset, yoffset, 0.0f));
    }

    // 右键按住时旋转
    if (rightButtonState == GLFW_PRESS)
    {
        float sensitivity = 0.005f;  // 旋转速度
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // 根据屏幕上的移动来旋转,这里定义了旋转轴
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f); // 屏幕的右方向（X轴）
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);    // 屏幕的上方向（Y轴）

        // 绕屏幕的X轴旋转（上下移动）
        glm::quat pitchQuat = glm::angleAxis(-yoffset, right);
        // 绕屏幕的Y轴旋转（左右移动）
        glm::quat yawQuat = glm::angleAxis(-xoffset, up);

        // 组合旋转
        q_rot = yawQuat * pitchQuat * q_rot;

        // 将四元数转换为旋转矩阵
        rotation = glm::mat4_cast(q_rot);
    }
}


void scroll_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //TODO:添加鼠标控制
    // 缩放控制
    if (yposIn > 0) {
        scaleNum += 0.1f; // 放大
    }
    else if (yposIn < 0) {
        scaleNum -= 0.1f; // 缩小
        if (scaleNum < 0.1f) scaleNum = 0.1f; // 限制最小缩放比例
    }

    scale = glm::scale(glm::vec3(scaleNum, scaleNum, scaleNum)); // 更新缩放矩阵
}
//glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//0代表Location(0) 顶点是vec3 
//用VAO可以简化重复以上两个操作，存储glEnableVertexAttribArray和glDisableVertexAttribArray的调用。