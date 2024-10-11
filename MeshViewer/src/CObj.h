#ifndef OBJ_CLASS
#define OBJ_CLASS

#include <vector>
#include <cmath>
#include <glm/glm.hpp>   // 引入 GLM 库的头文件

struct Point
{
	glm::vec3 pos;      // 使用 glm::vec3 代替自定义 Vector3
	glm::vec3 normal;   // 使用 glm::vec3 代替自定义 Vector3
};

struct Face
{
	int pts[3];         // 三个顶点索引
	glm::vec3 normal;   // 使用 glm::vec3 代替自定义 Vector3
};

class CObj
{
public:
	CObj(void);
	~CObj(void);

	std::vector<Point> m_pts; //顶点
	std::vector<Face> m_faces;//面

public:
	bool ReadObjFile(const char* pcszFileName);//读入模型文件

private:
	void UnifyModel(); //单位化模型
	void ComputeFaceNormal(Face& f); //计算面的法线
	void NormalizeVertexNormals(); //归一化顶点法线
};

#endif
