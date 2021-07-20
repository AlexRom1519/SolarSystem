#pragma once

#include "Object.h"

class MySphere : public Object
{
public:
    MySphere ();
	~MySphere ();

private:
	//This functions generates MySphere pData and pIndexes
	virtual void generateObjectData ();
};

