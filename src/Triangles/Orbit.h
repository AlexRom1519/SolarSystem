#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <gl/wglew.h>

#include <glm/glm.hpp>

#include "serialization.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

#include <iostream>
using std::cout;
using std::endl;

class Orbit
{
private:
	
    /*vec3* pData_;	//pointer to object's internal data
	unsigned int dataCount_;
	
	unsigned int vbo_;    // VertexBufferObject for MeshVertexData
	unsigned int vao_;*/   //one VertexArrayObject

    vec3* pData_;	//pointer to object's internal data
	unsigned int dataCount_;

	unsigned int* pIndexes_;	//pointer to indexes (list of vetrices) 
	unsigned int indexesCount_;
	
	unsigned int vbo_[2];    // VertexBufferObject one for MeshVertexData, another for Indexes
	unsigned int vao_;   //one VertexArrayObject

public:
	Orbit ();
	~Orbit ();
	//function for initialization
	void initGLBuffers (unsigned int programId, const char* posName);
	//function for drawing
	void Draw (const GLuint programId, const Serializator& serializator, const mat4x4 modelMatrix);

    void generateObjectData (Serializator* serializator, const int i_planet);
};
