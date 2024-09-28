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
        // ÿ��ϸ�ֶ���6�����㣬3�����ڲ��棬3�����ڵ���
        num = segments * 6;

        // ȷ�� segments ���������ֵ1000
        if (segments > 1000) {
            segments = 1000;
        }

        // ����Բ׶�Ķ��㡢��ɫ�ͷ���
        for (unsigned int i = 0; i < segments; ++i) {
            // ��ǰ�κ���һ���εĽǶ�
            float theta = 2.0f * M_PI * i / segments;
            float nextTheta = 2.0f * M_PI * (i + 1) / segments;

            // �����ϵ��������ڶ���
            glm::vec3 p1 = glm::vec3(radius * cos(theta), -height / 2, radius * sin(theta));
            glm::vec3 p2 = glm::vec3(radius * cos(nextTheta), -height / 2, radius * sin(nextTheta));

            // Բ׶�Ķ���
            glm::vec3 coneTop = glm::vec3(0.0f, height / 2, 0.0f);

            // �������� (���淨�߼���)
            glm::vec3 normal1 = glm::normalize(glm::cross(p2 - p1, coneTop - p1));

            // ����1��Բ׶����
            kPositions[i * 6] = coneTop;
            kColors[i * 6] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // ��ɫ
            kNormals[i * 6] = normal1; // ���淨��

            // ����2�������һ����
            kPositions[i * 6 + 1] = p1;
            kColors[i * 6 + 1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // ��ɫ
            kNormals[i * 6 + 1] = normal1; // ���淨��

            // ����3������ڶ�����
            kPositions[i * 6 + 2] = p2;
            kColors[i * 6 + 2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // ��ɫ
            kNormals[i * 6 + 2] = normal1; // ���淨��

            // ����4����������
            glm::vec3 baseCenter = glm::vec3(0.0f, -height / 2, 0.0f);
            kPositions[i * 6 + 3] = baseCenter;
            kColors[i * 6 + 3] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // ��ɫ
            kNormals[i * 6 + 3] = glm::vec3(0.0f, -1.0f, 0.0f); // ���淨��

            // ����5�������һ����
            kPositions[i * 6 + 4] = p1;
            kColors[i * 6 + 4] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // ��ɫ
            kNormals[i * 6 + 4] = glm::vec3(0.0f, -1.0f, 0.0f); // ���淨��

            // ����6������ڶ�����
            kPositions[i * 6 + 5] = p2;
            kColors[i * 6 + 5] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // ��ɫ
            kNormals[i * 6 + 5] = glm::vec3(0.0f, -1.0f, 0.0f); // ���淨��
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
    glm::vec3 kPositions[6000];  // ֧�����6000������
    glm::vec4 kColors[6000];     // ��ɫ����
    glm::vec3 kNormals[6000];    // ��������
    unsigned int num;            // ��������
};
