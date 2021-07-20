#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <gl/wglew.h>

#include <glm/glm.hpp>

#include "serialization.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

//helper struct for Vertex
//contains position, normal and texture coordinates
struct VertexData
{
	 vec3 pos;
	 vec3 nor;
	 vec2 tex;
};

#include <iostream>
using std::cout;
using std::endl;

class Object
{
protected:
	
    VertexData* pData_;	//pointer to object's internal data
	unsigned int dataCount_;

	unsigned int* pIndexes_;	//pointer to indexes (list of vetrices) 
	unsigned int indexesCount_;
	
	unsigned int vbo_[2];    // VertexBufferObject one for MeshVertexData, another for Indexes
	unsigned int vao_;   //one VertexArrayObject

public:
	Object ();
	~Object ();
	//function for initialization
	void initGLBuffers (unsigned int programId, const char* posName, const char* norName, const char* texName);
	//function for drawing
	void Draw (const GLuint programId, const Serializator& serializator, const vec4 color, const float light_distance, const vec3 light_spectrum,
               const vec3 background_light_koef, const vec3 diffuse_light_koef, const vec3 reflect_light_koef, const float shine_koef,
               const bool use_texture, const Texture* texture, const int light_source, vec3 light,
               const mat4x4 modelMatrix);

protected:
	//helper function generates two triangles
    virtual void generateObjectData () = 0 {}
};
