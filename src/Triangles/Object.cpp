#include "Object.h"

//=====================================================================================================================CONSTRUCTOR===================
Object::Object (/*const vec3 background_light_koef, const vec3 diffuse_light_koef, const vec3 reflect_light_koef, const float shine_koef*/):
dataCount_(0),
indexesCount_(0),
pData_(NULL),
pIndexes_(NULL)
{}

//=====================================================================================================================DESTRUCTOR====================
Object::~Object ()
{
	glDeleteBuffers(2, vbo_);
	glDeleteVertexArrays(1, &vao_);
	if (pData_)
	{
		delete[] pData_;
		delete[] pIndexes_;
	}
}

//=====================================================================================================================INIT=GL=BUFFERS===============
void Object::initGLBuffers(GLuint programId, const char* posName,const char* norName,const char* texName)
{
    int err = glGetError();

	glGenVertexArrays (1, &vao_);
	glBindVertexArray (vao_);

	glGenBuffers ( 2, &vbo_[0]);
		
	glBindBuffer ( GL_ARRAY_BUFFER, vbo_[0] );
	//glEnableVertexAttribArray(vbo_[0]);

	glBufferData ( GL_ARRAY_BUFFER, dataCount_*sizeof(VertexData), pData_, GL_STATIC_DRAW );
	
	//glEnable(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vbo_[1] );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indexesCount_*sizeof(unsigned int), pIndexes_, GL_STATIC_DRAW );
	
	int	loc = glGetAttribLocation(programId, posName);
	if (loc>-1)
	{
		glVertexAttribPointer(loc,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLuint *)0);
		glEnableVertexAttribArray (loc);
	}
	int loc2 = glGetAttribLocation(programId, norName);
	if (loc2>-1)
	{
		glVertexAttribPointer(loc2,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLuint *)(0+sizeof(float)*3));
		glEnableVertexAttribArray (loc2);
	}
	int loc3 = glGetAttribLocation(programId, texName);
	if (loc3>-1)
	{
		glVertexAttribPointer(loc3,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(GLuint *)(0+sizeof(float)*6));
		glEnableVertexAttribArray (loc3);
	}

    err = glGetError();
}

//=====================================================================================================================DRAW==========================
void Object::Draw(const GLuint programId, const Serializator& serializator, const vec4 color, const float light_distance, const vec3 light_spectrum,
                  const vec3 background_light_koef, const vec3 diffuse_light_koef, const vec3 reflect_light_koef, const float shine_koef,
                  const bool use_texture, const Texture* texture, const int light_source, vec3 light,
                  const mat4x4 modelMatrix)
{
    int err = glGetError();

    //==================================================binding=======================================

	glBindVertexArray ( vao_ );
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[1]);

    if (use_texture)
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());

    //==================================================position=matrix===============================

    //camera matrix. camera is placed in point "eye" and looks at point "cen".
    mat4x4 viewMatrix = glm::lookAt(serializator.eye_, serializator.cen_, serializator.up_);
    //modelMatrix is connected with current object. move square to scene's center
//    mat4x4 modelMatrix = glm::translate(glm::mat4(),glm::vec3(-0.5f,-0.5f,0));

    //modelViewMatrix consists of viewMatrix and modelMatrix
    mat4x4 modelViewMatrix = viewMatrix * modelMatrix;
    //calculate normal matrix 
    mat4x4 normalMatrix = glm::inverseTranspose(modelViewMatrix);
    //finally calculate modelViewProjectionMatrix
    mat4x4 modelViewProjectionMatrix = serializator.projectionMatrix_ * modelViewMatrix;

    
    //pass variables to the shaders

    int locMV = glGetUniformLocation(programId, "modelViewMatrix");
    if (locMV>-1)
        glUniformMatrix4fv(locMV,1,0,glm::value_ptr(modelViewMatrix));
    int locN = glGetUniformLocation(programId, "normalMatrix");
    if (locN>-1)
        glUniformMatrix4fv(locN,1,0,glm::value_ptr(normalMatrix));
    int locP = glGetUniformLocation(programId, "modelViewProjectionMatrix");
    if (locP>-1)
        glUniformMatrix4fv(locP,1,0,glm::value_ptr(modelViewProjectionMatrix));

    // LightPos нужно подать уже в системе координат камеры
    int locLight = glGetUniformLocation(programId, "light_pos");
    if (locLight>-1)
    {
        vec4 light_pos = viewMatrix * vec4 (light.x, light.y, light.z, 1);
        float light_pos_tmp [3] = {light_pos.x, light_pos.y, light_pos.z};
        glUniform3fv(locLight, 1, light_pos_tmp);
    }

    //==================================================color=and=light==============================

    err = glGetError();
    int colorLoc = glGetUniformLocation(programId, "mycolor");
    
    err = glGetError();
    if (colorLoc>-1)
    {
        float sh_color [4] = {color.x, color.y, color.z, color.w};
        err = glGetError();
        glUniform4fv (colorLoc, 1, sh_color);
        err = glGetError();
    }
    err = glGetError();
    int lightLoc1 = glGetUniformLocation(programId, "background_light_power");
    if (lightLoc1>-1)
    {
        //float background_light_power [3] = {1 * light_spectrum.x / light_distance, 1 * light_spectrum.y / light_distance, 1 * light_spectrum.z / light_distance};
        float background_light_power [3] = {1 * light_spectrum.x, 1 * light_spectrum.y, 1 * light_spectrum.z};
        glUniform3fv (lightLoc1, 1, background_light_power);
    }
    int lightLoc2 = glGetUniformLocation(programId, "diffuse_light_power");
    if (lightLoc2>-1)
    {
        //float diffuse_light_power [3] = {1 * light_spectrum.x / light_distance, 1 * light_spectrum.y / light_distance, 1 * light_spectrum.z / light_distance};
        float diffuse_light_power [3] = {1 * light_spectrum.x, 1 * light_spectrum.y, 1 * light_spectrum.z};
        glUniform3fv (lightLoc2, 1, diffuse_light_power);
    }

    int lightLoc3 = glGetUniformLocation(programId, "reflect_light_power");
    if (lightLoc3>-1)
    {
        //float reflect_light_power [3] = {1 * light_spectrum.x / light_distance, 1 * light_spectrum.y / light_distance, 1 * light_spectrum.z / light_distance};
        float reflect_light_power [3] = {1 * light_spectrum.x, 1 * light_spectrum.y, 1 * light_spectrum.z};
        glUniform3fv (lightLoc3, 1, reflect_light_power);
    }
    int lightLoc4 = glGetUniformLocation(programId, "background_light_koef");
    if (lightLoc4>-1)
    {
        float background_light_koef_tmp [3] = {background_light_koef.x, background_light_koef.y, background_light_koef.z};
        glUniform3fv (lightLoc4, 1, background_light_koef_tmp);
    }
    int lightLoc5 = glGetUniformLocation(programId, "diffuse_light_koef");
    if (lightLoc5>-1)
    {
        float diffuse_light_koef_tmp [3] = {diffuse_light_koef.x, diffuse_light_koef.y, diffuse_light_koef.z};
        glUniform3fv (lightLoc5, 1, diffuse_light_koef_tmp);
    }
    int lightLoc6 = glGetUniformLocation(programId, "reflect_light_koef");
    if (lightLoc6>-1)
    {
        float reflect_light_koef_tmp [3] = {reflect_light_koef.x, reflect_light_koef.y, reflect_light_koef.z};
        glUniform3fv (lightLoc6, 1, reflect_light_koef_tmp);
    }
    int lightLoc7 = glGetUniformLocation(programId, "shine_koef");
    if (lightLoc7>-1)
        glUniform1f (lightLoc7, shine_koef);

    //==================================================texture======================================

    int locFlag = glGetUniformLocation(programId, "useTexture");
    if (locFlag>-1)
        glUniform1i(locFlag, use_texture);
    int locLightSource = glGetUniformLocation(programId, "light_source");
    if (locLightSource>-1)
        glUniform1i(locLightSource, light_source);
    if (use_texture)
    {
        int texLoc = glGetUniformLocation(programId, "mytexture");
        if (texLoc>-1)
            glUniform1ui (texLoc, texture->GetTextureId());
    }
    
    err = glGetError();

    //==================================================at=last======================================
	glDrawElements(GL_TRIANGLES,indexesCount_,GL_UNSIGNED_INT,0);
    
    //glClearColor(0,0,1,1);
    //glClear(GL_COLOR_BUFFER_BIT);

    //==================================================unbind=======================================
    glBindVertexArray (0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    err = glGetError();
}
