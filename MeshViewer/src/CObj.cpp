#include "CObj.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>  // �������� GLM ����

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
			if (strLine[1] != 'n') // ���Է��� vn��ֻ������ v
			{
				std::istringstream sin(strLine);
				sin >> s1 >> point.pos.x >> point.pos.y >> point.pos.z;
				point.normal = glm::vec3(0.0f);  // ��ʼ�����㷨��Ϊ 0
				m_pts.push_back(point);
			}
		}
		else if (strLine[0] == 'f') // ����������
		{
			std::istringstream sin(strLine);
			sin >> s1 >> face.pts[0] >> face.pts[1] >> face.pts[2];
			ComputeFaceNormal(face);  // ������ķ���
			m_faces.push_back(face);
		}
	}

	fclose(fpFile);

	// ��һ��ģ�ͺͷ���
	UnifyModel();  // ��һ��ģ������
	NormalizeVertexNormals();  // ��һ�����㷨��

	return true;
}

void CObj::UnifyModel()
{
	glm::vec3 vec_max(-1e5f, -1e5f, -1e5f), vec_min(1e5f, 1e5f, 1e5f);

	// ����ģ�͵�������Сֵ
	for (size_t i = 0; i < m_pts.size(); i++)
	{
		vec_max.x = std::max(vec_max.x, m_pts[i].pos.x);
		vec_max.y = std::max(vec_max.y, m_pts[i].pos.y);
		vec_max.z = std::max(vec_max.z, m_pts[i].pos.z);

		vec_min.x = std::min(vec_min.x, m_pts[i].pos.x);
		vec_min.y = std::min(vec_min.y, m_pts[i].pos.y);
		vec_min.z = std::min(vec_min.z, m_pts[i].pos.z);
	}

	// �������ĵ�
	glm::vec3 center = (vec_max + vec_min) * 0.5f;

	// ����ģ�͵����ߴ磬ȡ x��y��z �е�����ֵ��Ϊͳһ��������
	float max_dim = std::max(vec_max.x - vec_min.x, std::max(vec_max.y - vec_min.y, vec_max.z - vec_min.z));

	// ��ÿ��������й�һ��
	for (size_t i = 0; i < m_pts.size(); i++)
	{
		m_pts[i].pos = (m_pts[i].pos - center) / max_dim;
	}
}

void CObj::ComputeFaceNormal(Face& f)
{
	// ��ȡ���������������
	glm::vec3 v1 = m_pts[f.pts[0] - 1].pos;
	glm::vec3 v2 = m_pts[f.pts[1] - 1].pos;
	glm::vec3 v3 = m_pts[f.pts[2] - 1].pos;

	// ������ķ���
	glm::vec3 faceNormal = glm::normalize(glm::cross(v2 - v1, v3 - v2));
	f.normal = faceNormal;  // �����߸�ֵ����

	// ����ķ����ۻ������㷨��
	m_pts[f.pts[0] - 1].normal += faceNormal;
	m_pts[f.pts[1] - 1].normal += faceNormal;
	m_pts[f.pts[2] - 1].normal += faceNormal;
}

void CObj::NormalizeVertexNormals()
{
	for (size_t i = 0; i < m_pts.size(); i++) {
		m_pts[i].normal = glm::normalize(m_pts[i].normal);  // ��һ�����㷨��
	}
}

