#include "Orbit.h"

//=====================================================================================================================CONSTRUCTOR===================
Orbit::Orbit ():
dataCount_(0),
indexesCount_(0),
pData_(NULL),
pIndexes_ (NULL)
{}

//=====================================================================================================================DESTRUCTOR====================
Orbit::~Orbit ()
{
	glDeleteBuffers(2, &(vbo_[0]));
	glDeleteVertexArrays(2, &vao_);
	if (pData_)
	{
		delete[] pData_;
        delete[] pIndexes_;
	}
}

//=====================================================================================================================INIT=GL=BUFFERS===============
void Orbit::initGLBuffers(GLuint programId, const char* posName)
{
    int err = glGetError();

	glGenVertexArrays (1, &vao_);
	glBindVertexArray (vao_);

	glGenBuffers ( 2, &vbo_[0]);
		
	glBindBuffer ( GL_ARRAY_BUFFER, vbo_[0] );
	//glEnableVertexAttribArray(vbo_[0]);

	glBufferData ( GL_ARRAY_BUFFER, dataCount_*sizeof(vec3), pData_, GL_STATIC_DRAW );
	
	//glEnable(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vbo_[1] );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indexesCount_*sizeof(unsigned int), pIndexes_, GL_STATIC_DRAW );
	
	int	loc = glGetAttribLocation(programId, posName);
	if (loc>-1)
	{
		glVertexAttribPointer(loc,3,GL_FLOAT,GL_FALSE,sizeof(vec3),(GLuint *)0);
		glEnableVertexAttribArray (loc);
	}

    err = glGetError();
}

//=====================================================================================================================DRAW==========================
void Orbit::Draw(const GLuint programId, const Serializator& serializator, const mat4x4 modelMatrix)
{
    int err = glGetError();

    //==================================================binding=======================================

	glBindVertexArray ( vao_ );
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[1]);

    //==================================================position=matrix===============================

    //camera matrix. camera is placed in point "eye" and looks at point "cen".
    mat4x4 viewMatrix = glm::lookAt(serializator.eye_, serializator.cen_, serializator.up_);

    //modelViewMatrix consists of viewMatrix and modelMatrix
    mat4x4 modelViewMatrix = viewMatrix * modelMatrix;
    //finally calculate modelViewProjectionMatrix
    mat4x4 modelViewProjectionMatrix = serializator.projectionMatrix_ * modelViewMatrix;

    int locP = glGetUniformLocation(programId, "modelViewProjectionMatrix");
    if (locP>-1)
        glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));

    //==================================================at=last======================================
    glDrawElements(GL_LINE_LOOP,indexesCount_,GL_UNSIGNED_INT,0);
    
    //glClearColor(0,0,1,1);
    //glClear(GL_COLOR_BUFFER_BIT);

    //==================================================unbind=======================================
    glBindVertexArray (0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    err = glGetError();
}

//=====================================================================================================================GENERATE=OBJECT=DATA==========
void Orbit::generateObjectData (Serializator* serializator, const int i_planet)
{
	if (pData_ != NULL)
	{
		delete[] pData_;
        delete[] pIndexes_;
	}

    dataCount_ = 360;

    pData_ = new vec3 [dataCount_];
    for (unsigned int i = 0; i < dataCount_; i++)
    {
        float angle = static_cast<float>(i) * glm::pi<float>() / 180;
        pData_[i] = serializator->MovePlanetPosition (i_planet, true, angle);
    }
    indexesCount_ = 360;
    pIndexes_ = new unsigned int [indexesCount_];
    for (unsigned int i = 0; i < indexesCount_; i++)
        pIndexes_[i] = i;
}
