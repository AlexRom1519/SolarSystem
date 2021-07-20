#include "Ring.h"

using glm::sin;
using glm::cos;

//=====================================================================================================================CONSTRUCTOR===================
Ring::Ring(const float inner_radius, const float out_radius):
inner_radius_ (inner_radius),
out_radius_ (out_radius)
{
    generateObjectData();
}

//=====================================================================================================================DESTRUCTOR====================
Ring::~Ring()
{}

//=====================================================================================================================GENERATE=OBJECT=DATA==========
void Ring::generateObjectData ()
{
    if (pData_ != NULL)
	{
		delete[] pData_;
		delete[] pIndexes_;
	}

    dataCount_ = 361 * 2;
    indexesCount_ = 360 * 2 * 3;

    pData_ = new VertexData [dataCount_];
    pIndexes_ = new unsigned int [indexesCount_];

    for (unsigned int i = 0; i < 361; i++)
    {
        float angle = i * glm::pi<float>() / 180;
        pData_[i].pos = vec3(glm::cos(angle), 0, glm::sin(angle)) * inner_radius_;
        pData_[i].tex = vec2(glm::cos(angle), glm::sin(angle)) * ((static_cast<float>(123)/256) / 2) + vec2 (0.5, 0.5);
        pData_[i].nor = vec3(0, 1, 0);
    }
    for (unsigned int i = 0; i < 361; i++)
    {
        float angle = i * glm::pi<float>() / 180;
        pData_[361 + i].pos = vec3(glm::cos(angle), 0, glm::sin(angle)) * out_radius_;
        pData_[361 + i].tex = vec2(glm::cos(angle), glm::sin(angle)) / static_cast<float>(2) + vec2(0.5, 0.5);
        pData_[361 + i].nor = vec3(0, 1, 0);
    }

    unsigned int index = 0;
    for (unsigned int i = 0; i < 360; i++)
    {
        pIndexes_[index] = i;
        pIndexes_[index +1] = i + 361 + 1;
        pIndexes_[index +2] = i + 361;

        pIndexes_[index +3] = i;
        pIndexes_[index +4] = i + 1;
        pIndexes_[index +5] = i + 361 + 1;
        index += 6;
    }
}
