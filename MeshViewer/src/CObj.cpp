#include "CObj.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>  // 包含常用 GLM 函数

CObj::CObj(void)
{
}

CObj::~CObj(void)
{
}

bool CObj::ReadObjFile(const char* pcszFileName)
{
	FILE* fpFile = fopen(pcszFileName, "r");
	if (fpFile == NULL)
	{
		return false;
	}

	m_pts.clear();
	m_faces.clear();

	char strLine[1024];
	Point point;
	Face face;
	std::string s1;

	while (!feof(fpFile))
	{
		fgets(strLine, 1024, fpFile);
		if (strLine[0] == 'v')
		{
			if (strLine[1] != 'n') // 忽略法线 vn，只处理顶点 v
			{
				std::istringstream sin(strLine);
				sin >> s1 >> point.pos.x >> point.pos.y >> point.pos.z;
				point.normal = glm::vec3(0.0f);  // 初始化顶点法线为 0
				m_pts.push_back(point);
			}
		}
		else if (strLine[0] == 'f') // 处理面数据
		{
			std::istringstream sin(strLine);
			sin >> s1 >> face.pts[0] >> face.pts[1] >> face.pts[2];
			ComputeFaceNormal(face);  // 计算面的法线
			m_faces.push_back(face);
		}
	}

	fclose(fpFile);

	// 归一化模型和法线
	UnifyModel();  // 归一化模型坐标
	NormalizeVertexNormals();  // 归一化顶点法线

	return true;
}

void CObj::UnifyModel()
{
	glm::vec3 vec_max(-1e5f, -1e5f, -1e5f), vec_min(1e5f, 1e5f, 1e5f);

	// 计算模型的最大和最小值
	for (size_t i = 0; i < m_pts.size(); i++)
	{
		vec_max.x = std::max(vec_max.x, m_pts[i].pos.x);
		vec_max.y = std::max(vec_max.y, m_pts[i].pos.y);
		vec_max.z = std::max(vec_max.z, m_pts[i].pos.z);

		vec_min.x = std::min(vec_min.x, m_pts[i].pos.x);
		vec_min.y = std::min(vec_min.y, m_pts[i].pos.y);
		vec_min.z = std::min(vec_min.z, m_pts[i].pos.z);
	}

	// 计算中心点
	glm::vec3 center = (vec_max + vec_min) * 0.5f;

	// 计算模型的最大尺寸，取 x、y、z 中的最大差值作为统一缩放因子
	float max_dim = std::max(vec_max.x - vec_min.x, std::max(vec_max.y - vec_min.y, vec_max.z - vec_min.z));

	// 对每个顶点进行归一化
	for (size_t i = 0; i < m_pts.size(); i++)
	{
		m_pts[i].pos = (m_pts[i].pos - center) / max_dim;
	}
}

void CObj::ComputeFaceNormal(Face& f)
{
	// 获取面的三个顶点坐标
	glm::vec3 v1 = m_pts[f.pts[0] - 1].pos;
	glm::vec3 v2 = m_pts[f.pts[1] - 1].pos;
	glm::vec3 v3 = m_pts[f.pts[2] - 1].pos;

	// 计算面的法线
	glm::vec3 faceNormal = glm::normalize(glm::cross(v2 - v1, v3 - v2));
	f.normal = faceNormal;  // 将法线赋值给面

	// 将面的法线累积到顶点法线
	m_pts[f.pts[0] - 1].normal += faceNormal;
	m_pts[f.pts[1] - 1].normal += faceNormal;
	m_pts[f.pts[2] - 1].normal += faceNormal;
}

void CObj::NormalizeVertexNormals()
{
	for (size_t i = 0; i < m_pts.size(); i++) {
		m_pts[i].normal = glm::normalize(m_pts[i].normal);  // 归一化顶点法线
	}
}

