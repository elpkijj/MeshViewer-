#pragma once

#include <glm/glm.hpp>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Cylinder
{
public:
    Cylinder(float radius = 1.0f, float height = 2.0f, unsigned int segments = 500) {
        // 确保 segments 不超过最大值500
        if (segments > 500) {
            segments = 500;
        }

        num = segments * 12; // 每个细分段有12个顶点，6个用于侧面，6个用于顶面和底面

        // 计算圆柱的顶点和颜色
        for (unsigned int i = 0; i < segments; ++i) {
            // 当前段和下一个段的角度
            float theta = 2.0f * M_PI * i / segments;
            float nextTheta = 2.0f * M_PI * (i + 1) / segments;

            // 顶面和底面上的两个相邻顶点
            glm::vec3 p1Top = glm::vec3(radius * cos(theta), height / 2, radius * sin(theta));
            glm::vec3 p2Top = glm::vec3(radius * cos(nextTheta), height / 2, radius * sin(nextTheta));

            glm::vec3 p1Bottom = glm::vec3(radius * cos(theta), -height / 2, radius * sin(theta));
            glm::vec3 p2Bottom = glm::vec3(radius * cos(nextTheta), -height / 2, radius * sin(nextTheta));

            // 顶点1：侧面第一个点 (p1Top)
            kPositions[i * 12] = p1Top;
            kColors[i * 12] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // 红色

            // 顶点2：侧面第二个点 (p2Top)
            kPositions[i * 12 + 1] = p2Top;
            kColors[i * 12 + 1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // 绿色

            // 顶点3：侧面底部第一个点 (p1Bottom)
            kPositions[i * 12 + 2] = p1Bottom;
            kColors[i * 12 + 2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // 蓝色

            // 顶点4：侧面底部第二个点 (p2Bottom)
            kPositions[i * 12 + 3] = p2Bottom;
            kColors[i * 12 + 3] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色

            // 顶点5：顶面中心 (baseCenterTop)
            glm::vec3 baseCenterTop = glm::vec3(0.0f, height / 2, 0.0f);
            kPositions[i * 12 + 4] = baseCenterTop;
            kColors[i * 12 + 4] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 灰色

            // 顶点6：底面中心 (baseCenterBottom)
            glm::vec3 baseCenterBottom = glm::vec3(0.0f, -height / 2, 0.0f);
            kPositions[i * 12 + 5] = baseCenterBottom;
            kColors[i * 12 + 5] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
        }
    }

    glm::vec3* getPos() {
        return kPositions;
    }

    glm::vec4* getColor() {
        return kColors;
    }

    unsigned int getNum() {
        return num;
    }

private:
    glm::vec3 kPositions[6000];  // 支持最大6000个顶点
    glm::vec4 kColors[6000];     // 颜色数组
    unsigned int num;            // 顶点总数
};
