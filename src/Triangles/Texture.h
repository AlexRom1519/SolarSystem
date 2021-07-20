#pragma once

#include <string>

#include <GL/glew.h>
#include <GL/glut.h>
#include <gl/wglew.h>

#include <glm/ext.hpp>

using std::string;

//some object for drawing
class Texture
{
private:
    GLuint tex_id_;

    GLubyte* tex_data_;
    unsigned int tex_width_;
    unsigned int tex_height_;

public:
	Texture (const string texture_name);
	~Texture ();
	
    GLuint GetTextureId () const
    {
        return tex_id_;
    }
};
