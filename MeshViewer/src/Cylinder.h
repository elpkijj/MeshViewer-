#pragma once

#include <glm/glm.hpp>

class Cylinder
{
public:
    Cylinder() {
        float radius = 1.0f;
        float height = 2.0f;
        unsigned int numSlices = 500;
        num = 0;

        float angleStep = 2.0f * glm::pi<float>() / numSlices;

        glm::vec3 topCenter = glm::vec3(0.0f, height / 2.0f, 0.0f);
        glm::vec3 bottomCenter = glm::vec3(0.0f, -height / 2.0f, 0.0f);

        // 生成顶部和底部的圆盘
        for (unsigned int i = 0; i < numSlices; ++i)
        {
            float theta = i * angleStep;
            float nextTheta = (i + 1) * angleStep;

            // 顶圆的两个边缘顶点
            glm::vec3 topVertex1 = glm::vec3(radius * cos(theta), height / 2.0f, radius * sin(theta));
            glm::vec3 topVertex2 = glm::vec3(radius * cos(nextTheta), height / 2.0f, radius * sin(nextTheta));

            // 底圆的两个边缘顶点
            glm::vec3 bottomVertex1 = glm::vec3(radius * cos(theta), -height / 2.0f, radius * sin(theta));
            glm::vec3 bottomVertex2 = glm::vec3(radius * cos(nextTheta), -height / 2.0f, radius * sin(nextTheta));

            // 颜色：顶部为红色，底部为绿色，侧面为蓝色
            glm::vec4 topColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);      // 红色
            glm::vec4 bottomColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);   // 绿色
            glm::vec4 sideColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);     // 蓝色

            // 添加顶圆的三角形
            kPositions[num] = topCenter;
            kColors[num] = topColor; // 固定为红色
            num++;
            kPositions[num] = topVertex1;
            kColors[num] = topColor; // 顶部固定为红色
            num++;
            kPositions[num] = topVertex2;
            kColors[num] = topColor; // 顶部固定为红色
            num++;

            // 添加底圆的三角形
            kPositions[num] = bottomCenter;
            kColors[num] = bottomColor; // 固定为绿色
            num++;
            kPositions[num] = bottomVertex1;
            kColors[num] = bottomColor; // 底部固定为绿色
            num++;
            kPositions[num] = bottomVertex2;
            kColors[num] = bottomColor; // 底部固定为绿色
            num++;

            // 生成侧面四边形（两个三角形）
            // 第一个三角形
            kPositions[num] = topVertex1;
            kColors[num] = sideColor; // 侧面固定为蓝色
            num++;
            kPositions[num] = bottomVertex1;
            kColors[num] = sideColor; // 侧面固定为蓝色
            num++;
            kPositions[num] = bottomVertex2;
            kColors[num] = sideColor; // 侧面固定为蓝色
            num++;

            // 第二个三角形
            kPositions[num] = topVertex1;
            kColors[num] = sideColor; // 侧面固定为蓝色
            num++;
            kPositions[num] = bottomVertex2;
            kColors[num] = sideColor; // 侧面固定为蓝色
            num++;
            kPositions[num] = topVertex2;
            kColors[num] = sideColor; // 侧面固定为蓝色
            num++;
        }
    };

    glm::vec3* getPos()
    {
        return kPositions;
    }
    glm::vec4* getColor()
    {
        return kColors;
    }
    unsigned int getNum()
    {
        return num;
    }

private:
    glm::vec3 kPositions[6000];
    glm::vec4 kColors[6000];
    unsigned int num;
};
