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
{//读取模型文件

	FILE* fpFile = fopen(pcszFileName, "r"); //以只读方式打开文件
	if (fpFile == NULL)
	{
		return false;
	}

	m_pts.clear();
	m_faces.clear();
	indices.clear();

	//TODO：将模型文件中的点和面数据分别存入m_pts和m_faces中
	

	fclose(fpFile);

	UnifyModel(); //将模型归一化
	for (int i = 0; i < m_faces.size(); i++)
	{
		ComputeFaceNormal(m_faces[i]);
	}
	ComputePointNormal();
	

	return true;
}

void CObj::UnifyModel()
{//为统一显示不同尺寸的模型，将模型归一化，将模型尺寸缩放到-1.0-1.0之间
//原理：找出模型的边界最大和最小值，进而找出模型的中心
//以模型的中心点为基准对模型顶点进行缩放
//TODO:添加模型归一化代码
	

}

void CObj::ComputeFaceNormal(Face& f)
{//TODO:计算面f的法向量，并保存
	
}

void CObj::ComputePointNormal()
{
	//TODO:计算顶点法向量，并保存
}