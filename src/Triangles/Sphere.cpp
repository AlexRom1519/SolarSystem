#include "Sphere.h"

using glm::sin;
using glm::cos;

//=====================================================================================================================CONSTRUCTOR===================
MySphere::MySphere()
{
    generateObjectData();
}

//=====================================================================================================================DESTRUCTOR====================
MySphere::~MySphere()
{}

//=====================================================================================================================GENERATE=OBJECT=DATA==========
void MySphere::generateObjectData ()
{
    // i - vertical angle
    // j - horizontal angle
    const int i_step = 1;   
    const int j_step = 1;   
    const int i_begin = -90;
    const int i_end = 90;
    const int j_begin = -180;
    const int j_end = 180;

	if (pData_ != NULL)
	{
		delete[] pData_;
		delete[] pIndexes_;
	}

    dataCount_ = ( (j_end -j_begin) / j_step +1 ) * ((i_end - i_begin) / i_step -1) + 2;
    indexesCount_ = ( ( (j_end -j_begin) / j_step) * ((i_end - i_begin) / i_step -2) * 2 + 2* ((j_end -j_begin) / j_step)) * 3;

    pData_ = new VertexData [dataCount_];
    pIndexes_ = new unsigned int [indexesCount_];

    ////const float magic_number = 2.84444/tex_data_width_; // 1024/380
    //const float magic_number = static_cast<float>(2.84444/1024); // 1024/380
    float change = 180/glm::pi<float>();
    pData_[0].pos = vec3 (0, -1, 0);
    pData_[0].nor = vec3 (0, -1, 0);
    pData_[0].tex = vec2 (0.5, 1);
    unsigned int data_num = 1;
    float i_2dlen = i_end - i_begin;
    float j_2dlen = j_end - j_begin;
    for (int i = i_begin + i_step; i <= i_end - i_step; i += i_step)
        for (int j = j_begin; j <= j_end; j += j_step)
        {
            pData_[data_num].pos = vec3 (cos(i/change) * cos (j/change), sin (i/change), cos (i/change) * sin (j/change));
            pData_[data_num].nor = glm::normalize(pData_[data_num].pos);
            pData_[data_num].tex = vec2 ( 1 - (static_cast<float>(j - j_begin) / j_2dlen /* * (1 - 2*magic_number) +magic_number*/), 1 - static_cast<float>(i - i_begin) / i_2dlen);
            data_num++;
        }
    pData_[data_num].pos = vec3 (0, 1, 0);
    pData_[data_num].nor = vec3 (0, 1, 0);
    pData_[data_num].tex = vec2 (0.5, 0);
    data_num++;
    if (data_num != dataCount_)
        throw string ("data_num != dataCount_");

    unsigned int indexes_num = 0;
    // Last line (sphere down)
    unsigned int k = 1;
    for (k = 1; k <= 1 + (j_end -j_begin) / j_step -1 ; ++k)
    {
        pIndexes_[indexes_num] = 0;
        indexes_num++;
        pIndexes_[indexes_num] = k;
        indexes_num++;
        pIndexes_[indexes_num] = k +1;
        indexes_num++;
    }
    // First line (sphere up)
    for (k = dataCount_-1 -(j_end -j_begin) / j_step; k < dataCount_-1; ++k)
    {
        pIndexes_[indexes_num] = dataCount_-1;
        indexes_num++;
        pIndexes_[indexes_num] = k +1;
        indexes_num++;
        pIndexes_[indexes_num] = k;
        indexes_num++;
    }
    // Other lines
    int new_line = 0;
    for (k = (j_end -j_begin) / j_step +1 +1; k < dataCount_-1; ++k)
    {
        if (new_line < (j_end -j_begin) / j_step)
        {
            pIndexes_[indexes_num] = k;
            indexes_num++;
            pIndexes_[indexes_num] = k -(j_end -j_begin) / j_step;
            indexes_num++;
            pIndexes_[indexes_num] = k -(j_end -j_begin) / j_step -1;
            indexes_num++;

            pIndexes_[indexes_num] = k;
            indexes_num++;
            pIndexes_[indexes_num] = k +1;
            indexes_num++;
            pIndexes_[indexes_num] = k -(j_end -j_begin) / j_step;
            indexes_num++;

            new_line++;
        }
        else
            new_line = 0;
    }
    if (indexes_num != indexesCount_)
        throw string("indexes_num != indexesCount_");
}
