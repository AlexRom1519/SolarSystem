
//standard libraries
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <conio.h>

//opengl headers
#include <GL/glew.h>
#include <GL/glut.h>
#include <gl/wglew.h>

//opengl mathematics
#include <glm/glm.hpp>
#include <glm/ext.hpp>

//functions for shader compilation and linking
#include "shaderhelper.h"
//object for drawing
#include "MyObject.h"
#include "Sphere.h"
#include "Orbit.h"
#include "Ring.h"
#include "serialization.h"

using std::cout;
using std::endl;
using std::string;
using std::exception;

//===================================================================================================================================================
//===================================================================================================================================================
//===================================================================================================================================================

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

//model for drawing: a square from two triangles
MySphere* mysphere;
Ring* ring;
Orbit orbits [9];

//struct for loading shaders
ShaderProgram shaderProgram;
ShaderProgram shaderProgramOrbit;

Serializator serializator;

//matrices
/*glm::mat4x4 modelViewMatrix;
glm::mat4x4 projectionMatrix;
glm::mat4x4 modelViewProjectionMatrix;
glm::mat4x4 normalMatrix;*/

//names of shader files, program will search for them during execution
//don't forget place it near executable 
char VertexShaderName[] = "Vertex.vert.cpp";
char FragmentShaderName[] = "Fragment.frag.cpp";
char VertexOrbitShaderName[] = "Vertex-orbit.vert.cpp";
char FragmentOrbitShaderName[] = "Fragment-orbit.frag.cpp";

void init ();
void freeResources ();
void display ();

GLvoid ReSizeGLScene (GLsizei width, GLsizei height);
GLvoid KillGLWindow (GLvoid);
BOOL CreateGLWindow (char* title, int width, int height, int bits, bool fullscreenflag);
LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//===================================================================================================================================================
//===================================================================================================================================================
//=====================================================================================================================INIT==========================
void init()
{
    int err = glGetError();

    glewInit();
    wglSwapIntervalEXT(1);
	//initialize shader program
	shaderProgram.init(VertexShaderName,FragmentShaderName);
    shaderProgramOrbit.init(VertexOrbitShaderName,FragmentOrbitShaderName);
	//use this shader program
	glUseProgram(shaderProgram.programObject);

    //enable texturing and zero slot
    glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE0);

    // Optimization
    glCullFace(GL_BACK);    // Discard inner or outer
    glEnable(GL_CULL_FACE);

    // Enabling Depth Buffer
    glEnable(GL_DEPTH_TEST);

    //create new objects
    serializator.InitializeTextures();
    mysphere = new MySphere();
    ring = new Ring(74500 / serializator.radius_[6], 180000 / serializator.radius_[6]);

    //initialize opengl buffers and variables inside of object
    mysphere->initGLBuffers(shaderProgram.programObject, "pos","nor","tex");
    ring->initGLBuffers(shaderProgram.programObject, "pos","nor","tex");

    for (int i = 0; i < 9; i++)
    {
        orbits[i].generateObjectData(&serializator, i+1);
        orbits[i].initGLBuffers(shaderProgramOrbit.programObject, "pos");
    }

    err = glGetError();
}

//=====================================================================================================================FREE=RESOURCES================
void freeResources ()
{
    delete mysphere;
    delete ring;
}

//=====================================================================================================================DISPLAY=======================
void display()
{
    int err = glGetError();

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Draw triangle with shaders (in screen coordinates)
    //need to set uniform in modelViewMatrix

    glUseProgram(shaderProgram.programObject);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);

    //draw objects
    err = glGetError();

    glUseProgram(shaderProgramOrbit.programObject);
    for (int i = 0; i < 9; i++)
    {
        orbits[i].Draw (shaderProgramOrbit.programObject, serializator, serializator.GetPlanetOrbitScaleMatrix(i+1));
    }

    err = glGetError();

    glUseProgram(shaderProgram.programObject);
    glEnable(GL_CULL_FACE);
    for (int i = 0; i < serializator.planet_num_; ++i)
    {
        float light_distance = 0;
        if (i == 10)
        {
            glCullFace(GL_FRONT);
            light_distance = 1;
        }
        else
        {
            glCullFace(GL_BACK);
            light_distance = glm::length(serializator.GetPlanetPosition(i));
        }
        
        mysphere->Draw( shaderProgram.programObject, serializator, serializator.color_[i], light_distance, serializator.light_spectrum_, 
            serializator.background_light_koef_[i], serializator.diffuse_light_koef_[i], serializator.reflect_light_koef_[i], serializator.shine_koef_[i],
            serializator.use_texture_[i], serializator.texture_[i], serializator.light_source_[i], vec3(0, 0, 0),
            serializator.GetPlanetModelMatrix(i) );
        float  len = glm::length(serializator.position_[3]);
        if (serializator.move_planets_)
            serializator.MovePlanets();
    }
    glDisable (GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    float light_distance = glm::length(serializator.GetPlanetPosition(6));
    
    ring->Draw (shaderProgram.programObject, serializator, vec4(1, 1, 1, 0.2), light_distance, serializator.light_spectrum_, 
                vec3(1.0, 1.0, 1.0)/*serializator.background_light_koef_[6]*/, serializator.diffuse_light_koef_[6], serializator.reflect_light_koef_[6], serializator.shine_koef_[6],
                serializator.use_texture_[11], serializator.texture_[11], serializator.light_source_[6], vec3(0, 0, 0),
                serializator.GetPlanetModelMatrix(6));
    
    glDisable (GL_BLEND);

    /*
    void Draw (const GLuint programId, const Serializator& serializator, const vec4 color, const float light_distance, const vec3 light_spectrum,
               const vec3 background_light_koef, const vec3 diffuse_light_koef, const vec3 reflect_light_koef, const float shine_koef,
               const bool use_texture, const Texture* texture, const int light_source, vec3 light,
               const mat4x4 modelMatrix);
    */
    err = glGetError();
}

//=====================================================================================================================MAIN==========================
int main ()
{
    try
    {
	    MSG		msg;									// Windows Message Structure
	    BOOL	done=FALSE;								// Bool Variable To Exit Loop

	    // Create Our OpenGL Window
	    if (!CreateGLWindow("Solar system", serializator.windowWidth_, serializator.windowHeight_, 32, fullscreen))
	    {
		    return 0;									// Quit If Window Was Not Created
	    }

	    while(!done)									// Loop That Runs While done=FALSE
	    {
		    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		    {
			    if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			    {
				    done=TRUE;							// If So done=TRUE
			    }
			    else									// If Not, Deal With Window Messages
			    {
				    TranslateMessage(&msg);				// Translate The Message
				    DispatchMessage(&msg);				// Dispatch The Message
			    }
		    }
		    else										// If There Are No Messages
		    {
			    // Draw The Scene.  Watch For ESC Key And Quit Messages From display()
			    if (active)								// Program Active?
			    {
				    if (keys[VK_ESCAPE])				// Was ESC Pressed?
				    {
					    done=TRUE;						// ESC Signalled A Quit
				    }
				    else								// Not Time To Quit, Update Screen
				    {
					    display();
                        SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				    }
			    }
                if (keys['W'])
                {
                    keys['W'] = false;
                    serializator.SetStep();
                    vec3 move = glm::normalize (serializator.cen_ - serializator.eye_);
                    move = move * serializator.step_;
                    if (glm::length(serializator.eye_ + move) < serializator.GetPlanetRadius(10) /1.5)
                    {
                        serializator.eye_ += move;
                        serializator.cen_ += move;
                    }
                }
                if (keys['S'])
                {
                    keys['S'] = false;
                    serializator.SetStep();
                    vec3 move = glm::normalize (serializator.cen_ - serializator.eye_);
                    move = move * (serializator.step_ *2);
                    if (glm::length(serializator.eye_ - move) < serializator.GetPlanetRadius(10) /1.5)
                    {
                        serializator.eye_ -= move;
                        serializator.cen_ -= move;
                    }
                }
                if (keys['D'])
                {
                    keys['D'] = false;
                    serializator.SetStep();
                    vec3 move = serializator.cen_ - serializator.eye_;
                    move = glm::normalize(glm::cross (serializator.up_, move)) * serializator.step_;
                    if (glm::length(serializator.eye_ - move) < serializator.GetPlanetRadius(10) /1.5)
                    {
                        serializator.eye_ -= move;
                        serializator.cen_ -= move;
                    }
                }
                if (keys['A'])
                {
                    keys['A'] = false;
                    serializator.SetStep();
                    vec3 move = serializator.cen_ - serializator.eye_;
                    move = glm::normalize(glm::cross (serializator.up_, move)) * serializator.step_;
                    if (glm::length(serializator.eye_ + move) < serializator.GetPlanetRadius(10) /1.5)
                    {
                        serializator.eye_ += move;
                        serializator.cen_ += move;
                    }
                }
                if (keys['E'])
                {
                    keys['E'] = false;
                    serializator.SetStep();
                    vec3 move = glm::normalize(serializator.up_) * serializator.step_;
                    if (glm::length(serializator.eye_ + move) < serializator.GetPlanetRadius(10) /1.5)
                    {
                        serializator.eye_ += move;
                        serializator.cen_ += move;
                    }
                }
                if (keys['Q'])
                {
                    keys['Q'] = false;
                    serializator.SetStep();
                    vec3 move = glm::normalize(serializator.up_) * serializator.step_;
                    if (glm::length(serializator.eye_ - move) < serializator.GetPlanetRadius(10) /1.5)
                    {
                        serializator.eye_ -= move;
                        serializator.cen_ -= move;
                    }
                }

                if (keys[VK_SPACE])
                {
                    keys[VK_SPACE] = false;
                    serializator.move_planets_ = !serializator.move_planets_;
                    serializator.last_time_measure_ = glutGet(GLUT_ELAPSED_TIME);
                }
                if (keys[VK_TAB])
                {
                    keys[VK_TAB] = false;
                    serializator.enable_schema_mode_ = !serializator.enable_schema_mode_;
                }

                if (keys['T'])
                {
                    keys['T'] = false;
                    serializator.SetStep();
                    if (serializator.enable_schema_mode_)
                    {
                        if (serializator.schema_orbits_scale_ * serializator.schema_orbits_scale_step_ > 1000)
                            serializator.schema_orbits_scale_ *= serializator.schema_orbits_scale_step_;
                        else
                            cout << "Too big orbit radius." << endl;
                    }
                    else
                    {
                        
                        if (serializator.real_mode_scale_ * serializator.real_mode_scale_step_ > 35806 /*1118*/)
                            serializator.real_mode_scale_ *= serializator.real_mode_scale_step_;
                        else
                            cout << "Too big scale." << endl;
                    }
                }
                if (keys['R'])
                {
                    keys['R'] = false;
                    serializator.SetStep();
                    if (serializator.enable_schema_mode_)
                    {
                        if (serializator.schema_orbits_scale_ / serializator.schema_orbits_scale_step_ < 5E+09)
                        {
                            serializator.schema_orbits_scale_ /= serializator.schema_orbits_scale_step_;
                            if (glm::length(serializator.eye_) > serializator.GetPlanetRadius(10) /2)
                            {
                                serializator.eye_ *= serializator.schema_orbits_scale_step_;
                                serializator.cen_ *= serializator.schema_orbits_scale_step_;
                            }
                        }
                        else
                            cout << "Too small orbit radius." << endl;
                    }
                    else
                    {
                        if (serializator.real_mode_scale_ / serializator.real_mode_scale_step_ < 36665472)
                        {
                            serializator.real_mode_scale_ /= serializator.real_mode_scale_step_;
                            if (glm::length(serializator.eye_) > serializator.GetPlanetRadius(10) /2)
                            {
                                serializator.eye_ *= serializator.real_mode_scale_step_;
                                serializator.cen_ *= serializator.real_mode_scale_step_;
                            }
                        }
                        else
                            cout << "Too small scale." << endl;
                    }
                }
                if (keys['F'])
                {
                    keys['F'] = false;
                    serializator.SetStep();
                    if (serializator.enable_schema_mode_)
                        if (serializator.schema_radius_scale_ > 22765/*640*/)
                            serializator.schema_radius_scale_ *= serializator.schema_radius_scale_step_;
                        else
                            cout << "Too big radius." << endl;
                }
                if (keys['G'])
                {
                    keys['G'] = false;
                    serializator.SetStep();
                    if (serializator.enable_schema_mode_)
                        if (serializator.schema_radius_scale_ < 4820674)
                            serializator.schema_radius_scale_ /= serializator.schema_radius_scale_step_;
                        else
                            cout << "Too small radius." << endl;
                }

                if (keys['K'])
                {
                    keys['K'] = false;
                    serializator.SetStep();
                    serializator.eye_ = vec3(0.0, 0.0, 11);
                    serializator.cen_ = vec3(0.0, 0.0, serializator.eye_.z - 1);
                    serializator.SetSkyMapRadius();
                }

                if (keys['X'])
                {
                    keys['X'] = false;
                    if (serializator.game_speed_ < 10000000000)
                        serializator.game_speed_ *= static_cast<float>(10.0/9);
                    else
                        cout << "Too fast." << endl;
                }
                if (keys['C'])
                {
                    keys['C'] = false;
                    if (serializator.game_speed_ > 5 )
                        serializator.game_speed_ /= static_cast<float>(10.0/9);
                    else
                        cout << "Too slow. Press enter for full stop." << endl;
                }
		    }
	    }

	    // Shutdown
	    KillGLWindow();									// Kill The Window
	    return (msg.wParam);							// Exit The Program
    }
    catch (string& s)
    {
        cout << "Error. " << s << endl;
        cout << "Press any key to continue." << endl;
        _getch ();
        return 1;
    }
    catch (exception& e)
    {
        cout << "Unexpected Error. " << e.what() << endl;
        cout << "Press any key to continue." << endl;
        _getch ();
        return 1;
    }
    catch (...)
    {
        cout << "Fatal error." << endl;
        cout << "Press any key to continue." << endl;
        _getch ();
        return 1;
    }
}

//===================================================================================================================================================
//===================================================================================================================================================
//===================================================================================================================================================

//=====================================================================================================================RESIZE=GL=SCENE===============
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

    if (width < 800)
        width = 800;
    if (height < 600)
        height = 600;

    serializator.windowWidth_ = width;
    serializator.windowHeight_ = height;
    //set viewport to match window size
    glViewport(0, 0, width, height);
  
    float fieldOfView = 45.0f;
    float aspectRatio = float(width)/float(height);
    float zNear = 0.1f;
    float zFar = 1000000.0f;
    //set projection matrix
    serializator.projectionMatrix_ = glm::perspective(fieldOfView,aspectRatio,zNear,zFar);
}

//===SEARCH=AND=DESTROY================================================================================================KILL=GLWINDOWS================
GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
    freeResources ();

	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

//=====================================================================================================================CREATE=GLWINDOW===============
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

    serializator.mouseX_ = serializator.windowWidth_/2;
    serializator.mouseY_ = serializator.windowHeight_/2;
    SetCursorPos (serializator.mouseX_, serializator.mouseY_);
    ShowCursor (false);

    init();

	return TRUE;									// Success
}

//=====================================================================================================================WND=PROC======================
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

        case WM_KEYDOWN:							// Has A Key Been Pressed?
        {
            keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
        }

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}

        case WM_MOUSEMOVE:
        {
            int new_mouseX = GET_X_LPARAM(lParam);
            int new_mouseY = GET_Y_LPARAM(lParam);

            if (new_mouseX != serializator.mouseX_)
            {
                vec3 move = serializator.cen_ - serializator.eye_;
                move = glm::rotate<float> (move, (new_mouseX - serializator.mouseX_) * serializator.vision_step_, serializator.up_);
                serializator.cen_ = serializator.eye_ + move;
                serializator.mouseX_ = new_mouseX;
            }
            if (new_mouseY != serializator.mouseY_)
            {
                vec3 move = serializator.cen_ - serializator.eye_;
                vec3 norm = glm::rotate<float> (serializator.up_, 90.0, move);
                move = glm::rotate<float> (move, (new_mouseY - serializator.mouseY_) * serializator.vision_step_, norm);
                serializator.up_ = glm::rotate<float> (serializator.up_, (new_mouseY - serializator.mouseY_) * serializator.vision_step_, norm);
                
                float len = glm::sqrt (serializator.up_.x*serializator.up_.x + serializator.up_.z*serializator.up_.z);
                vec3 new_up = move * (float)-1 / glm::l2Norm (move) * len;
                serializator.up_.x = new_up.x;
                serializator.up_.z = new_up.z;

                serializator.cen_ = serializator.eye_ + move;
                serializator.mouseY_ = new_mouseY;
            }
            if (new_mouseX < 200 || new_mouseX > serializator.windowWidth_ -200)
            {
                serializator.mouseX_ = new_mouseX = (int)serializator.windowWidth_/2;
                //serializator.mouseY_ = new_mouseY = (int)serializator.windowHeight_/2;
                SetCursorPos (serializator.mouseX_, serializator.mouseY_ +30);
            }
            if (new_mouseY < 200 || new_mouseY > serializator.windowHeight_ -200)
            {
                //serializator.mouseX_ = new_mouseX = (int)serializator.windowWidth_/2;
                serializator.mouseY_ = new_mouseY = (int)serializator.windowHeight_/2;
                SetCursorPos (serializator.mouseX_, serializator.mouseY_ +30);
            }
        }
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
