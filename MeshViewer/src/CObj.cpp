#include "CObj.h"
#include <iostream>
#include <sstream>
#include <algorithm>

CObj::CObj(void)
{
}


CObj::~CObj(void)
{
}

bool CObj::ReadObjFile(const char* pcszFileName)
{//��ȡģ���ļ�

	FILE* fpFile = fopen(pcszFileName, "r"); //��ֻ����ʽ���ļ�
	if (fpFile == NULL)
	{
		return false;
	}

	m_pts.clear();
	m_faces.clear();
	indices.clear();

	//TODO����ģ���ļ��еĵ�������ݷֱ����m_pts��m_faces��
	

	fclose(fpFile);

	UnifyModel(); //��ģ�͹�һ��
	for (int i = 0; i < m_faces.size(); i++)
	{
		ComputeFaceNormal(m_faces[i]);
	}
	ComputePointNormal();
	

	return true;
}

void CObj::UnifyModel()
{//Ϊͳһ��ʾ��ͬ�ߴ��ģ�ͣ���ģ�͹�һ������ģ�ͳߴ����ŵ�-1.0-1.0֮��
//ԭ���ҳ�ģ�͵ı߽�������Сֵ�������ҳ�ģ�͵�����
//��ģ�͵����ĵ�Ϊ��׼��ģ�Ͷ����������
//TODO:���ģ�͹�һ������
	

}

void CObj::ComputeFaceNormal(Face& f)
{//TODO:������f�ķ�������������
	
}

void CObj::ComputePointNormal()
{
	//TODO:���㶥�㷨������������
}