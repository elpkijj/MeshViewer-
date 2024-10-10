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
        // ȷ�� segments ���������ֵ500
        if (segments > 500) {
            segments = 500;
        }

        num = segments * 12; // ÿ��ϸ�ֶ���12�����㣬6�����ڲ��棬6�����ڶ���͵���

        // ����Բ���Ķ������ɫ
        for (unsigned int i = 0; i < segments; ++i) {
            // ��ǰ�κ���һ���εĽǶ�
            float theta = 2.0f * M_PI * i / segments;
            float nextTheta = 2.0f * M_PI * (i + 1) / segments;

            // ����͵����ϵ��������ڶ���
            glm::vec3 p1Top = glm::vec3(radius * cos(theta), height / 2, radius * sin(theta));
            glm::vec3 p2Top = glm::vec3(radius * cos(nextTheta), height / 2, radius * sin(nextTheta));

            glm::vec3 p1Bottom = glm::vec3(radius * cos(theta), -height / 2, radius * sin(theta));
            glm::vec3 p2Bottom = glm::vec3(radius * cos(nextTheta), -height / 2, radius * sin(nextTheta));

            // ����������ĵ����Χ����
            glm::vec3 baseCenterTop = glm::vec3(0.0f, height / 2, 0.0f);
            kPositions[i * 12] = p1Top;
            kColors[i * 12] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 1] = p2Top;
            kColors[i * 12 + 1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 2] = baseCenterTop;
            kColors[i * 12 + 2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // ����������ɫ

            // ����������ĵ����Χ����
            glm::vec3 baseCenterBottom = glm::vec3(0.0f, -height / 2, 0.0f);
            kPositions[i * 12 + 3] = p1Bottom;
            kColors[i * 12 + 3] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 4] = p2Bottom;
            kColors[i * 12 + 4] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 5] = baseCenterBottom;
            kColors[i * 12 + 5] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // ����������ɫ

            // ����
            kPositions[i * 12 + 6] = p1Top;
            kColors[i * 12 + 6] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 7] = p1Bottom;
            kColors[i * 12 + 7] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 8] = p2Bottom;
            kColors[i * 12 + 8] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 9] = p2Bottom;
            kColors[i * 12 + 9] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 10] = p2Top;
            kColors[i * 12 + 10] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // ������ɫ

            kPositions[i * 12 + 11] = p1Top;
            kColors[i * 12 + 11] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // ������ɫ
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
    glm::vec3 kPositions[6000];  // ֧�����6000������
    glm::vec4 kColors[6000];     // ��ɫ����
    unsigned int num;            // ��������
};
