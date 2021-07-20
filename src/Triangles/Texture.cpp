#include "EasyBMP/EasyBMP.h"
#include "Texture.h"

//=====================================================================================================================CONSTRUCTOR===================
Texture::Texture(const string texture_name):
tex_data_ (NULL)
{
    int err = glGetError();

    //generate as many textures as you need
	glGenTextures(1, &tex_id_);
	//make texId current 
	glBindTexture(GL_TEXTURE_2D, tex_id_);
	
	//don't use alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
    BMP in;

    if (!in.ReadFromFile(texture_name.c_str()))
        throw string("Error reading file ") + texture_name;

    tex_width_ = in.TellWidth();
    tex_height_ = in.TellHeight();
    GLubyte* tex_data_ = new GLubyte [tex_height_ * tex_width_ * 3];

    unsigned long int counter = 0;
    for (unsigned int i = 0; i < tex_height_; ++i)
        for (unsigned int j = 0; j < tex_width_; ++j)
        {
            RGBApixel *p = in(j, i);
            tex_data_[counter] = static_cast<GLubyte>(p->Red);
            counter++;
            tex_data_[counter] = static_cast<GLubyte>(p->Green);
            counter++;
            tex_data_[counter] = static_cast<GLubyte>(p->Blue);
            counter++;
        }

	//set Texture Data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width_, tex_height_, 0, GL_RGB, GL_UNSIGNED_BYTE, &tex_data_[0]);

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR /*GL_NEAREST*/);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER /*GL_CLAMP*/);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER /*GL_CLAMP*/);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    err = glGetError();
}

//=====================================================================================================================DESTRUCTOR====================
Texture::~Texture()
{
    if (tex_data_ != NULL)
        delete [] tex_data_;
    glDeleteTextures (1, &tex_id_);
}
