class Cone
{
public:
    Cone(float radius = 1.0f, float height = 2.0f, unsigned int numSlices = 50)
    {
        num = 0; // ��ʼ����������

        float angleStep = 2.0f * glm::pi<float>() / numSlices;

        // �������ĵ�
        glm::vec3 bottomCenter = glm::vec3(0.0f, -height / 2.0f, 0.0f);
        glm::vec4 bottomColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // �ײ���ɫ

        // ���ɵ��涥�������������
        for (unsigned int i = 0; i < numSlices; ++i)
        {
            float theta = i * angleStep;
            float nextTheta = (i + 1) * angleStep;

            // ��ǰ����һ����
            glm::vec3 currentVertex = glm::vec3(radius * cos(theta), -height / 2.0f, radius * sin(theta));
            glm::vec3 nextVertex = glm::vec3(radius * cos(nextTheta), -height / 2.0f, radius * sin(nextTheta));

            // ���������Σ����ĵ㡢��ǰ�㡢��һ����
            kPositions[num] = bottomCenter;
            kColors[num] = bottomColor;
            num++;

            kPositions[num] = currentVertex;
            kColors[num] = bottomColor;
            num++;

            kPositions[num] = nextVertex;
            kColors[num] = bottomColor;
            num++;
        }

        // Բ׶���㣨��ˣ�
        glm::vec3 tip = glm::vec3(0.0f, height / 2.0f, 0.0f);
        glm::vec4 tipColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // ������ɫ

        // ���ɲ��棬���ӵ��������
        for (unsigned int i = 0; i < numSlices; ++i)
        {
            float theta = i * angleStep;
            float nextTheta = (i + 1) * angleStep;

            // ��ǰ����һ�������
            glm::vec3 currentVertex = glm::vec3(radius * cos(theta), -height / 2.0f, radius * sin(theta));
            glm::vec3 nextVertex = glm::vec3(radius * cos(nextTheta), -height / 2.0f, radius * sin(nextTheta));

            // ���������Σ���ˡ���ǰ�㡢��һ����
            kPositions[num] = tip;
            kColors[num] = tipColor;
            num++;

            kPositions[num] = currentVertex;
            kColors[num] = bottomColor;
            num++;

            kPositions[num] = nextVertex;
            kColors[num] = bottomColor;
            num++;
        }
    }

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
