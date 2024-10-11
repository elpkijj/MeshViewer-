#ifndef OBJ_CLASS
#define OBJ_CLASS

#include <vector>
#include <cmath>
#include <glm/glm.hpp>   // ���� GLM ���ͷ�ļ�

struct Point
{
	glm::vec3 pos;      // ʹ�� glm::vec3 �����Զ��� Vector3
	glm::vec3 normal;   // ʹ�� glm::vec3 �����Զ��� Vector3
};

struct Face
{
	int pts[3];         // ������������
	glm::vec3 normal;   // ʹ�� glm::vec3 �����Զ��� Vector3
};

class CObj
{
public:
	CObj(void);
	~CObj(void);

	std::vector<Point> m_pts; //����
	std::vector<Face> m_faces;//��

public:
	bool ReadObjFile(const char* pcszFileName);//����ģ���ļ�

private:
	void UnifyModel(); //��λ��ģ��
	void ComputeFaceNormal(Face& f); //������ķ���
	void NormalizeVertexNormals(); //��һ�����㷨��
};

#endif
