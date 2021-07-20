#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <gl/wglew.h>

#include <glm/glm.hpp>

#include "Object.h"

#include <iostream>
using std::cout;
using std::endl;

class Ring : public Object
{
private:
    float inner_radius_;
    float out_radius_;

public:
    Ring (const float inner_radius, const float out_radius);
	~Ring ();

private:
	//This functions generates MySphere pData and pIndexes
	virtual void generateObjectData ();
};
