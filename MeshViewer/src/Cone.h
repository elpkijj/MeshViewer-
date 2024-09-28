#pragma once

#include <glm/glm.hpp>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Cone
{
public:
    Cone(float radius = 1.0f, float height = 2.0f, unsigned int segments = 1000) {
        // 每个细分段有6个顶点，3个用于侧面，3个用于底面
        num = segments * 6;

        // 确保 segments 不超过最大值1000
        if (segments > 1000) {
            segments = 1000;
        }

        // 计算圆锥的顶点、颜色和法线
        for (unsigned int i = 0; i < segments; ++i) {
            // 当前段和下一个段的角度
            float theta = 2.0f * M_PI * i / segments;
            float nextTheta = 2.0f * M_PI * (i + 1) / segments;

            // 底面上的两个相邻顶点
            glm::vec3 p1 = glm::vec3(radius * cos(theta), -height / 2, radius * sin(theta));
            glm::vec3 p2 = glm::vec3(radius * cos(nextTheta), -height / 2, radius * sin(nextTheta));

            // 圆锥的顶点
            glm::vec3 coneTop = glm::vec3(0.0f, height / 2, 0.0f);

            // 法线向量 (侧面法线计算)
            glm::vec3 normal1 = glm::normalize(glm::cross(p2 - p1, coneTop - p1));

            // 顶点1：圆锥顶点
            kPositions[i * 6] = coneTop;
            kColors[i * 6] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // 红色
            kNormals[i * 6] = normal1; // 侧面法线

            // 顶点2：底面第一个点
            kPositions[i * 6 + 1] = p1;
            kColors[i * 6 + 1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
            kNormals[i * 6 + 1] = normal1; // 侧面法线

            // 顶点3：底面第二个点
            kPositions[i * 6 + 2] = p2;
            kColors[i * 6 + 2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // 蓝色
            kNormals[i * 6 + 2] = normal1; // 侧面法线

            // 顶点4：底面中心
            glm::vec3 baseCenter = glm::vec3(0.0f, -height / 2, 0.0f);
            kPositions[i * 6 + 3] = baseCenter;
            kColors[i * 6 + 3] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
            kNormals[i * 6 + 3] = glm::vec3(0.0f, -1.0f, 0.0f); // 底面法线

            // 顶点5：底面第一个点
            kPositions[i * 6 + 4] = p1;
            kColors[i * 6 + 4] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
            kNormals[i * 6 + 4] = glm::vec3(0.0f, -1.0f, 0.0f); // 底面法线

            // 顶点6：底面第二个点
            kPositions[i * 6 + 5] = p2;
            kColors[i * 6 + 5] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
            kNormals[i * 6 + 5] = glm::vec3(0.0f, -1.0f, 0.0f); // 底面法线
        }
    }

    glm::vec3* getPos() {
        return kPositions;
    }

    glm::vec4* getColor() {
        return kColors;
    }

    glm::vec3* getNormals() {
        return kNormals;
    }

    unsigned int getNum() {
        return num;
    }

private:
    glm::vec3 kPositions[6000];  // 支持最大6000个顶点
    glm::vec4 kColors[6000];     // 颜色数组
    glm::vec3 kNormals[6000];    // 法线数组
    unsigned int num;            // 顶点总数
};
