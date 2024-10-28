#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef F_PI
#define F_PI		((float)(M_PI))
#define F_2_PI		((float)(2.f*F_PI))
#define F_PI_2		((float)(F_PI/2.f))
#endif


#include "glew.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glut.h"

// grid components
#define XSIDE	5.0f			// length of the x side of the grid
#define X0      (-XSIDE/2.)		// where one side starts
#define NX		300			// how many points in x
#define DX		( XSIDE/(float)NX )	// change in x between the points

#define YGRID	0.f

#define ZSIDE	5.0f			// length of the z side of the grid
#define Z0      (-ZSIDE/2.)		// where one side starts
#define NZ		300			// how many points in z
#define DZ		( ZSIDE/(float)NZ )	// change in z between the points



//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//

// title of these windows:

const char *WINDOWTITLE = "OpenGL / GLUT Sample -- Miles Kesser";
const char *GLUITITLE   = "User Interface Window";

// what the glui package defines as true and false:

const int GLUITRUE  = true;
const int GLUIFALSE = false;

// the escape key:

const int ESCAPE = 0x1b;

// initial window size:

const int INIT_WINDOW_SIZE = 600;

// size of the 3d box to be drawn:

const float BOXSIZE = 2.f;

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = 1.f;
const float SCLFACT = 0.005f;

// minimum allowable scale factor:

const float MINSCALE = 0.05f;

// scroll wheel button values:

const int SCROLL_WHEEL_UP   = 3;
const int SCROLL_WHEEL_DOWN = 4;

// equivalent mouse movement when we click the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;

// active mouse buttons (or them together):

const int LEFT   = 4;
const int MIDDLE = 2;
const int RIGHT  = 1;

// which projection:

enum Projections
{
	ORTHO,
	PERSP
};

// which button:

enum ButtonVals
{
	RESET,
	QUIT
};

// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };

// line width for the axes:

const GLfloat AXES_WIDTH   = 3.;

// the color numbers:
// this order must match the radio button order, which must match the order of the color names,
// 	which must match the order of the color RGB values

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA
};

char * ColorNames[ ] =
{
	(char *)"Red",
	(char*)"Yellow",
	(char*)"Green",
	(char*)"Cyan",
	(char*)"Blue",
	(char*)"Magenta"
};

// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
};

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0f, .0f, .0f, 1.f };
const GLenum  FOGMODE     = GL_LINEAR;
const GLfloat FOGDENSITY  = 0.30f;
const GLfloat FOGSTART    = 1.5f;
const GLfloat FOGEND      = 4.f;

// for lighting:

const float	WHITE[ ] = { 1.,1.,1.,1. };

// for animation:

const int MS_PER_CYCLE = 10000;		// 10000 milliseconds = 10 seconds


// what options should we compile-in?
// in general, you don't need to worry about these
// i compile these in to show class examples of things going wrong
//#define DEMO_Z_FIGHTING
//#define DEMO_DEPTH_BUFFER


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
GLuint	SphereList;				

GLuint 	ObjectList4;
GLuint 	ObjectList5;
GLuint 	LightSourceList;
GLuint 	LightSourceList1;
GLuint 	LightSourceList2;
GLuint 	LightSourceList3;
GLuint	GridDL;
GLuint	TableTop;
GLuint Light;
GLuint Circle;
GLuint Cue;
GLuint cueBall;
GLuint ball1;
GLuint ball2;
GLuint ball3;
GLuint ball4;
GLuint ball5;
GLuint ball6;

GLuint rugTex;
GLuint poolfeltTex;
GLuint metalTex;
GLuint woodTex;
GLuint wood2Tex;


// set lighting as changable variables 
GLfloat ColorR = 1.0f;
GLfloat ColorG = 1.0f;
GLfloat ColorB = 1.0f;

// light source & sphere XZ travel pattern 
GLfloat lightPosX = 2.75f;  
GLfloat lightPosY = 6.75f;
GLfloat lightPosZ = 1.75f;

// point light settings 
GLfloat ambientLight[] = { 0.7f, 0.7f, 0.7f, 1.0f}; 
GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat spotDirection[] = {0.0, -1.0, 0.0};

// dull settings 
GLfloat dullDiffuse[] = { 0.0f, 0.6f, 1.0f, 1.0f };
GLfloat dullSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat dullShininess = 10.0f; 

// shiny settings 
GLfloat shinyDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat shinySpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat shinyShininess = 128.0f; 


// 0 for point light, 1 for spot light 
int Spotlight = 1;              

int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to force the creation of z-fighting
int		MainWindow;				// window id for main graphics window
int		NowColor;				// index into Colors[ ]
int		NowProjection;		// ORTHO or PERSP
float	Scale;					// scaling factor
int		ShadowsOn;				// != 0 means to turn shadows on
float	Time;					// used for animation, this has a value between 0. and 1.
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees

// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void			Axes( float );
void			HsvRgb( float[3], float [3] );
void			Cross(float[3], float[3], float[3]);
float			Dot(float [3], float [3]);
float			Unit(float [3], float [3]);
float			Unit(float [3]);



// utility to create an array from 3 separate values:

float *
Array3( float a, float b, float c )
{
	static float array[4];

	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}

// utility to create an array from a multiplier and an array:

float *
MulArray3( float factor, float array0[ ] )
{
	static float array[4];

	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}


float *
MulArray3(float factor, float a, float b, float c )
{
	static float array[4];

	float* abc = Array3(a, b, c);
	array[0] = factor * abc[0];
	array[1] = factor * abc[1];
	array[2] = factor * abc[2];
	array[3] = 1.;
	return array;
}

// these are here for when you need them -- just uncomment the ones you need:

#include "setmaterial.cpp"
#include "setlight.cpp"
#include "osusphere.cpp"
//#include "osucone.cpp"
//#include "osutorus.cpp"
#include "bmptotexture.cpp"
#include "loadobjfile.cpp"
#include "keytime.cpp"
//#include "glslprogram.cpp"

#define MSEC        10000
Keytimes cue_X, cue_Y, cue_Z;
Keytimes cue_BallX, cue_BallY, cue_BallZ;
Keytimes ball_1X, ball_1Y, ball_1Z;
Keytimes ball_2X, ball_2Y, ball_2Z;
Keytimes ball_3X, ball_3Y, ball_3Z;
Keytimes ball_4X, ball_4Y, ball_4Z;
Keytimes ball_5X, ball_5Y, ball_5Z;
Keytimes ball_6X, ball_6Y, ball_6Z;
Keytimes rotation_AngleY;

// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since glutInit might
	// pull some command line arguments out)


	// keytimes 
	
	cue_X.Init(); 
	cue_Y.Init();
	cue_Z.Init();

	cue_X.AddTimeValue(0.0, -2.0);
	cue_Y.AddTimeValue(0.0, 0.5);
	cue_Z.AddTimeValue(0.0, 0.0);

	cue_X.AddTimeValue(1.0, -2.0);
	cue_Y.AddTimeValue(1.0, 0.5);
	cue_Z.AddTimeValue(1.0, 0.0);

	cue_X.AddTimeValue(2.0, -2.25);
	cue_Y.AddTimeValue(2.0, 0.6);
	cue_Z.AddTimeValue(2.0, 0.0);

	cue_X.AddTimeValue(3.8, -2.25);
	cue_Y.AddTimeValue(3.8, 0.6);
	cue_Z.AddTimeValue(3.8, 0.0);

	cue_X.AddTimeValue(4.0, -1.9);
	cue_Y.AddTimeValue(4.0, 0.5);
	cue_Z.AddTimeValue(4.0, 0.0);

	for (float time = 4.0; time <= 5.0; time += 0.01) {
    cue_X.AddTimeValue(time, -1.9);
	cue_Y.AddTimeValue(time, 0.5);
	cue_Z.AddTimeValue(time, 0.0);
}
	cue_X.AddTimeValue(7.0, 1.4);
	cue_Y.AddTimeValue(7.0, 0.5);
	cue_Z.AddTimeValue(7.0, -0.85);

	for (float time = 7.0; time <= 8.03; time += 0.01) {
    cue_X.AddTimeValue(time, 1.4);
	cue_Y.AddTimeValue(time, 0.5);
	cue_Z.AddTimeValue(time, -0.85);
}

	cue_X.AddTimeValue(8.5, 1.7);
	cue_Y.AddTimeValue(8.5, 0.5);
	cue_Z.AddTimeValue(8.5, -1.05);

	cue_X.AddTimeValue(8.6, 1.34);
	cue_Y.AddTimeValue(8.6, 0.5);
	cue_Z.AddTimeValue(8.6, -0.82);

	rotation_AngleY.Init();

	for (float time = 0.0; time <= 5.0; time += 0.01) {
    rotation_AngleY.AddTimeValue(time, 0);
}
 	rotation_AngleY.AddTimeValue(7.0, -150);


	// cue ball 
	cue_BallX.Init(); 
	cue_BallY.Init();
	cue_BallZ.Init();

for (float time = 0.0; time <= 4.0; time += 0.01) {
    cue_BallX.AddTimeValue(time, -0.875f);
    cue_BallY.AddTimeValue(time, 0.35);
    cue_BallZ.AddTimeValue(time, 0.0);
}
	cue_BallX.AddTimeValue(4.5, 0.65f);
	cue_BallY.AddTimeValue(4.5, 0.35);
	cue_BallZ.AddTimeValue(4.5, 0.0);

	for (float time = 4.5; time <= 4.52; time += 0.01) {
    cue_BallX.AddTimeValue(time, 0.65f);
	cue_BallY.AddTimeValue(time, 0.35);
	cue_BallZ.AddTimeValue(time, 0.0);
}

	cue_BallX.AddTimeValue(7.5, 0.44f);
	cue_BallY.AddTimeValue(7.5, 0.35);
	cue_BallZ.AddTimeValue(7.5, -0.29);

	for (float time = 7.5; time <= 8.6; time += 0.01) {
    cue_BallX.AddTimeValue(time, 0.44f);
	cue_BallY.AddTimeValue(time, 0.35);
	cue_BallZ.AddTimeValue(time, -0.29);
}

	cue_BallX.AddTimeValue(9.1, -0.915f);
	cue_BallY.AddTimeValue(9.1, 0.35);
	cue_BallZ.AddTimeValue(9.1, 0.49);

	for (float time = 9.1; time <= 9.13; time += 0.01) {
	cue_BallX.AddTimeValue(time, -0.915f);
	cue_BallY.AddTimeValue(time, 0.35);
	cue_BallZ.AddTimeValue(time, 0.49);
}

	cue_BallX.AddTimeValue(9.9, -0.7f);
	cue_BallY.AddTimeValue(9.9, 0.35);
	cue_BallZ.AddTimeValue(9.9, 0.55);


	// orange ball 
	ball_4X.Init(); 
	ball_4Y.Init();
	ball_4Z.Init();

	for (float time = 0.0; time <= 4.5; time += 0.01) {
    ball_4X.AddTimeValue(time, 0.95);
    ball_4Y.AddTimeValue(time, 0.35);
    ball_4Z.AddTimeValue(time, -0.1);
}
	ball_4X.AddTimeValue(5.0, 1.37);
    ball_4Y.AddTimeValue(5.0, 0.35);
    ball_4Z.AddTimeValue(5.0, -0.62);

	ball_4X.AddTimeValue(5.3, 1.37);
    ball_4Y.AddTimeValue(5.3, 0.2);
    ball_4Z.AddTimeValue(5.3, -0.62);

	// yellow ball 
	ball_6X.Init(); 
	ball_6Y.Init();
	ball_6Z.Init();

	for (float time = 0.0; time <= 4.5; time += 0.01) {
    ball_6X.AddTimeValue(time, 0.95);
    ball_6Y.AddTimeValue(time, 0.35);
    ball_6Z.AddTimeValue(time, 0.1);
}
	ball_6X.AddTimeValue(4.65, 1.35);
    ball_6Y.AddTimeValue(4.65, 0.35);
    ball_6Z.AddTimeValue(4.65, 0.3);

	for (float time = 4.65; time <= 4.68; time += 0.01) {
    ball_6X.AddTimeValue(time, 1.35);
    ball_6Y.AddTimeValue(time, 0.35);
    ball_6Z.AddTimeValue(time, 0.3);
}
 	ball_6X.AddTimeValue(5.2, 0.8);
    ball_6Y.AddTimeValue(5.2, 0.35);
    ball_6Z.AddTimeValue(5.2, 0.6);

	for (float time = 5.2; time <= 5.23; time += 0.01) {
 	ball_6X.AddTimeValue(time, 0.8);
    ball_6Y.AddTimeValue(time, 0.35);
    ball_6Z.AddTimeValue(time, 0.6);
}
 	ball_6X.AddTimeValue(6.7, 0.02);
    ball_6Y.AddTimeValue(6.7, 0.35);
    ball_6Z.AddTimeValue(6.7, -0.62);

	ball_6X.AddTimeValue(7, 0.02);
    ball_6Y.AddTimeValue(7, 0.2);
    ball_6Z.AddTimeValue(7, -0.62);

	// black ball 
	ball_5X.Init(); 
	ball_5Y.Init();
	ball_5Z.Init();

	for (float time = 0.0; time <= 4.5; time += 0.01) {
    ball_5X.AddTimeValue(time, 0.95);
    ball_5Y.AddTimeValue(time, 0.35);
    ball_5Z.AddTimeValue(time, 0.0);
}
	ball_5X.AddTimeValue(4.8, 1.35);
    ball_5Y.AddTimeValue(4.8, 0.35);
    ball_5Z.AddTimeValue(4.8, 0.17);
	
	for (float time = 4.8; time <= 4.82; time += 0.01) {
    ball_5X.AddTimeValue(time, 1.35);
    ball_5Y.AddTimeValue(time, 0.35);
    ball_5Z.AddTimeValue(time, 0.17);
}
	ball_5X.AddTimeValue(6.3, -0.4);
    ball_5Y.AddTimeValue(6.3, 0.35);
    ball_5Z.AddTimeValue(6.3, 0.48);

	ball_5X.AddTimeValue(8.3, -1.0);
    ball_5Y.AddTimeValue(8.3, 0.35);
    ball_5Z.AddTimeValue(8.3, 0.5);

	for (float time = 8.3; time <= 9.13; time += 0.01) {
   	ball_5X.AddTimeValue(time, -1.0);
    ball_5Y.AddTimeValue(time, 0.35);
    ball_5Z.AddTimeValue(time, 0.5);
}
	ball_5X.AddTimeValue(9.6, -1.35);
    ball_5Y.AddTimeValue(9.6, 0.35);
    ball_5Z.AddTimeValue(9.6, 0.63);

	ball_5X.AddTimeValue(9.8, -1.35);
    ball_5Y.AddTimeValue(9.8, 0.2);
    ball_5Z.AddTimeValue(9.8, 0.63);

	// red ball 
	ball_1X.Init(); 
	ball_1Y.Init();
	ball_1Z.Init();

	for (float time = 0.0; time <= 4.5; time += 0.01) {
    ball_1X.AddTimeValue(time, 0.75);
    ball_1Y.AddTimeValue(time, 0.35);
    ball_1Z.AddTimeValue(time, 0.00);
}
	ball_1X.AddTimeValue(7.5, 1.35);
    ball_1Y.AddTimeValue(7.5, 0.35);
    ball_1Z.AddTimeValue(7.5, 0.6);

	ball_1X.AddTimeValue(7.8, 1.38);
    ball_1Y.AddTimeValue(7.8, 0.2);
    ball_1Z.AddTimeValue(7.8, 0.63);

	// blue ball 
	ball_3X.Init(); 
	ball_3Y.Init();
	ball_3Z.Init();

	for (float time = 0.0; time <= 4.5; time += 0.01) {
    ball_3X.AddTimeValue(time, 0.85);
    ball_3Y.AddTimeValue(time, 0.35);
    ball_3Z.AddTimeValue(time, 0.05);
}

	ball_3X.AddTimeValue(5.5, 1.2);
    ball_3Y.AddTimeValue(5.5, 0.35);
    ball_3Z.AddTimeValue(5.5, 0.59);

	for (float time = 5.5; time <= 5.53; time += 0.01) {
		ball_3X.AddTimeValue(time, 1.2);
		ball_3Y.AddTimeValue(time, 0.35);
		ball_3Z.AddTimeValue(time, 0.59);
}
	ball_3X.AddTimeValue(6.4, 1.31);
    ball_3Y.AddTimeValue(6.4, 0.35);
    ball_3Z.AddTimeValue(6.4, 0.55);

	for (float time = 6.4; time <= 7; time += 0.01) {
		ball_3X.AddTimeValue(time, 1.31);
		ball_3Y.AddTimeValue(time, 0.35);
		ball_3Z.AddTimeValue(time, 0.55);
}

		ball_3X.AddTimeValue(7.3, 1.35);
		ball_3Y.AddTimeValue(7.3, 0.35);
		ball_3Z.AddTimeValue(7.3, 0.6);

	for (float time = 7.3; time <= 7.31; time += 0.01) {
		ball_3X.AddTimeValue(time, 1.37);
		ball_3Y.AddTimeValue(time, 0.35);
		ball_3Z.AddTimeValue(time, 0.62);
}
		ball_3X.AddTimeValue(7.4, 1.37);
		ball_3Y.AddTimeValue(7.4, 0.2);
		ball_3Z.AddTimeValue(7.4, 0.62);
		
	// green ball 
	ball_2X.Init(); 
	ball_2Y.Init();
	ball_2Z.Init();

	for (float time = 0.0; time <= 4.5; time += 0.01) {
    ball_2X.AddTimeValue(time, 0.85);
    ball_2Y.AddTimeValue(time, 0.35);
    ball_2Z.AddTimeValue(time, -0.05);
}
	ball_2X.AddTimeValue(4.75, 1.35);
    ball_2Y.AddTimeValue(4.75, 0.35);
    ball_2Z.AddTimeValue(4.75, -0.2);

	for (float time = 4.75; time <= 4.77; time += 0.01) {
	ball_2X.AddTimeValue(time, 1.35);
    ball_2Y.AddTimeValue(time, 0.35);
    ball_2Z.AddTimeValue(time, -0.2);
}
	ball_2X.AddTimeValue(6.0, -0.1);
    ball_2Y.AddTimeValue(6.0, 0.35);
    ball_2Z.AddTimeValue(6.0, -0.4);

	ball_2X.AddTimeValue(7.8, -1.35);
    ball_2Y.AddTimeValue(7.8, 0.35);
    ball_2Z.AddTimeValue(7.8, -0.6);

	ball_2X.AddTimeValue(8.1, -1.35);
    ball_2Y.AddTimeValue(8.1, 0.2);
    ball_2Z.AddTimeValue(8.1, -0.6);


	glutInit( &argc, argv );

	// setup all the graphics stuff:

	InitGraphics( );

	// create the display lists that **will not change**:

	InitLists( );

	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );

	// setup all the user interface stuff:

	InitMenus( );

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );

	// glutMainLoop( ) never actually returns
	// the following line is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutPostRedisplay( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables for Display( ) to find:

	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;							// makes the value of ms between 0 and MS_PER_CYCLE-1
	Time = (float)ms / (float)MS_PER_CYCLE;		// makes the value of Time between 0. and slightly less than 1.

	// for example, if you wanted to spin an object in Display( ), you might call: glRotatef( 360.f*Time,   0., 1., 0. );

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting Display.\n");

	// set which window we want to do the graphics into:
	glutSetWindow( MainWindow );

	// erase the background:
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );
#ifdef DEMO_DEPTH_BUFFER
	if( DepthBufferOn == 0 )
		glDisable( GL_DEPTH_TEST );
#endif

	// specify shading to be smooth:

	glShadeModel(GL_SMOOTH);

	// set the viewport to be a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	// ortho disabled 

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( NowProjection == ORTHO )
		//glOrtho( -2.f, 2.f,     -2.f, 2.f,     0.1f, 1000.f );
		gluPerspective( 70.f, 1.f,	0.1f, 1000.f );
	else
		gluPerspective( 70.f, 1.f,	0.1f, 1000.f );

	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );



	int msec= glutGet( GLUT_ELAPSED_TIME )  %  MSEC; 
	// turn that into a time in seconds: 
	//float nowSecs= (float)msec/ 1000.f; 

	double nowSecs = (float)msec/ 1000.f;

	// set the eye position, look-at position, and up-vector:

	gluLookAt( 0.f, 0.f, 4.f,     0.f, 0.f, 0.f,     0.f, 1.f, 0.f );

	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0.f, 1.f, 0.f );
	glRotatef( (GLfloat)Xrot, 1.f, 0.f, 0.f );

	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}

	// possibly draw the axes:

	if( AxesOn != 1 )
	{
		glColor3fv( &Colors[NowColor][0] );
		glDisable(GL_LIGHTING);
		glCallList( AxesList );
		
	}


	// since we are using glScalef( ), be sure the normals get unitized:
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	// call light setting variables 
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);


	// set light position and rotation
	glPushMatrix();
		glRotatef(360.f * Time, 0., 1., 0.);
		SetPointLight(GL_LIGHT0, lightPosX, lightPosY, lightPosZ, ColorR, ColorG, ColorB);
	glPopMatrix();
	
	glPushMatrix();
		glRotatef(360.f * Time, 0., 1., 0.);
		glTranslatef(lightPosX, lightPosY, lightPosZ);
		glCallList(LightSourceList);
	glPopMatrix();

	// make spotlight 35 degrees
	if (Spotlight == 1) {
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0); 
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25.0);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25.0);
		GLfloat spotDirection[] = {0.0, -1.0, 0.0};
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);
	} else {
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0); 
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0); 
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 180.0); 
	}

	glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // call grid 
	glPushMatrix();
		glCallList(GridDL);
	glPopMatrix();

	glPushMatrix();
		glCallList(TableTop);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(cue_BallX.GetValue(nowSecs), cue_BallY.GetValue(nowSecs), cue_BallZ.GetValue(nowSecs));
		glCallList(cueBall);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(ball_1X.GetValue(nowSecs), ball_1Y.GetValue(nowSecs), ball_1Z.GetValue(nowSecs));
		glCallList(ball1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(ball_2X.GetValue(nowSecs), ball_2Y.GetValue(nowSecs), ball_2Z.GetValue(nowSecs));
		glCallList(ball2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(ball_3X.GetValue(nowSecs), ball_3Y.GetValue(nowSecs), ball_3Z.GetValue(nowSecs));
		glCallList(ball3);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(ball_4X.GetValue(nowSecs), ball_4Y.GetValue(nowSecs), ball_4Z.GetValue(nowSecs));
		glCallList(ball4);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(ball_5X.GetValue(nowSecs), ball_5Y.GetValue(nowSecs), ball_5Z.GetValue(nowSecs));
		glCallList(ball5);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(ball_6X.GetValue(nowSecs), ball_6Y.GetValue(nowSecs), ball_6Z.GetValue(nowSecs));
		glCallList(ball6);
	glPopMatrix();


	glPushMatrix();
		glTranslatef(cue_X.GetValue(nowSecs), cue_Y.GetValue(nowSecs), cue_Z.GetValue(nowSecs));
		glRotatef(rotation_AngleY.GetValue(nowSecs), 0.0f, 1.0f, 0.0f);
			glCallList(Cue);
	glPopMatrix();

	glPushMatrix();
		glCallList(ObjectList4);
	glPopMatrix();



	glPushMatrix();
		glCallList(ObjectList5);
	glPopMatrix();


	glPushMatrix();
		glCallList(Light);
	glPopMatrix();

	// middle light sphere 
    glPushMatrix();
		glDisable(GL_LIGHTING);
		glCallList(LightSourceList1);
    glPopMatrix();

		// middle light 
	glPushMatrix();
		SetPointLight(GL_LIGHT1, 0.0f, 1.52f, 0.0f, ColorR, ColorG, ColorB);
	glPopMatrix();

	// right light sphere 
    glPushMatrix();
		glDisable(GL_LIGHTING);
		glCallList(LightSourceList2);
    glPopMatrix();

		// right light 
	glPushMatrix();
		SetPointLight(GL_LIGHT2, 0.68f, 1.52f, 0.0f, ColorR, ColorG, ColorB);
	glPopMatrix();

	// left light sphere 
    glPushMatrix();
		glDisable(GL_LIGHTING);
		glCallList(LightSourceList3);
    glPopMatrix();

	// left light 
	glPushMatrix();
		SetPointLight(GL_LIGHT3, -0.68f, 1.52f, 0.0f, ColorR, ColorG, ColorB);
	glPopMatrix();

	// pockets 
	glPushMatrix();
		glCallList( Circle );
	glPopMatrix();

	
#ifdef DEMO_Z_FIGHTING
	if( DepthFightingOn != 0 )
	{
		glPushMatrix( );
			glRotatef( 90.f,   0.f, 1.f, 0.f );
			glCallList( SphereList );
		glPopMatrix( );
	}
#endif


	// draw some gratuitous text that just rotates on top of the scene:
	// i commented out the actual text-drawing calls -- put them back in if you have a use for them
	// a good use for thefirst one might be to have your name on the screen
	// a good use for the second one might be to have vertex numbers on the screen alongside each vertex

	glDisable( GL_DEPTH_TEST );
	glColor3f( 0.f, 1.f, 1.f );
	//DoRasterString( 0.f, 1.f, 0.f, (char *)"Text That Moves" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0.f, 100.f,     0.f, 100.f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glColor3f( 1.f, 1.f, 1.f );
	//DoRasterString( 5.f, 5.f, 0.f, (char *)"Text That Doesn't" );

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	NowColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	NowProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitMenus.\n");

	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(float) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Axis Colors",   colormenu);

#ifdef DEMO_DEPTH_BUFFER
	glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
#endif

#ifdef DEMO_Z_FIGHTING
	glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
#endif

	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup callback functions

void
InitGraphics( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitGraphics.\n");

	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc(MouseMotion);
	//glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );

	// setup glut to call Animate( ) every time it has
	// 	nothing it needs to respond to (which is most of the time)
	// we don't need to do this for this program, and really should set the argument to NULL
	// but, this sets us up nicely for doing animation

	glutIdleFunc( Animate );

	// init the glew package (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	// all other setups go here, such as GLSLProgram and KeyTime setups:

}


void InitLists() {
    if (DebugOn != 0)
        fprintf(stderr, "Starting InitLists.\n");

glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glEnable( GL_TEXTURE_2D );

int rugWidth, rugHeight;
    unsigned char* rugImgData = BmpToTexture("rug.bmp", &rugWidth, &rugHeight);
    glGenTextures(1, &rugTex);
    glBindTexture(GL_TEXTURE_2D, rugTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, rugWidth, rugHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, rugImgData);

// floor grid with rug texture
GridDL = glGenLists(1);
glNewList(GridDL, GL_COMPILE);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, dullDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dullSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, dullShininess);
    glTranslatef(0.0f, -0.5f, 0.0f);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rugTex);
    glColor3f(1.0, 1.0, 1.0);

    for (int i = 0; i < NZ; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < NX; j++) {
            glTexCoord2f((float)j / NX, (float)i / NZ); 
            glVertex3f(X0 + DX * (float)j, YGRID, Z0 + DZ * (float)(i + 0));

            glTexCoord2f((float)j / NX, (float)(i + 1) / NZ); 
            glVertex3f(X0 + DX * (float)j, YGRID, Z0 + DZ * (float)(i + 1));
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
glEndList();



cueBall = glGenLists(1);
glNewList(cueBall, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(0.7, 0.7,0.7);
	OsuSphere(0.05, 50, 50);
glEndList();


ball1 = glGenLists(1);
glNewList(ball1, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(0.6,0.0,0.0);
	OsuSphere(0.05, 50, 50);
glEndList();

ball2 = glGenLists(1);
glNewList(ball2, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(0.0,0.6,0.0);
	OsuSphere(0.05, 50, 50);
glEndList();

ball3 = glGenLists(1);
glNewList(ball3, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(0.0, 0.0,0.6);
	OsuSphere(0.05, 50, 50);
glEndList();

ball4 = glGenLists(1);
glNewList(ball4, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(1.0, 0.4, 0.0);  
	OsuSphere(0.05, 50, 50);
glEndList();

ball5 = glGenLists(1);
glNewList(ball5, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(0.05, 0.05,0.05);
	OsuSphere(0.05, 50, 50);
glEndList();

ball6 = glGenLists(1);
glNewList(ball6, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(0.7, 0.7, 0.0);  
	OsuSphere(0.05, 50, 50);
glEndList();



float L = 3.0f; 
float W = 1.5f; 
int newNX = NX * (L / XSIDE);
int newNZ = NZ * (W / ZSIDE);

int poolfeltWidth, poolfeltHeight;
unsigned char* poolfeltImgData = BmpToTexture("poolfelt.bmp", &poolfeltWidth, &poolfeltHeight);
glGenTextures(1, &poolfeltTex);
glBindTexture(GL_TEXTURE_2D, poolfeltTex);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, 3, poolfeltWidth, poolfeltHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, poolfeltImgData);

// tabltop with felt texture
TableTop = glGenLists(1);
glNewList(TableTop, GL_COMPILE);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, dullDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dullSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, dullShininess);

    glTranslatef(1.009f, 0.3f, 1.75f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, poolfeltTex);
    glColor3f(0.0, 0.4, 0.0); 

    for (int i = 0; i < newNZ; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < newNX; j++) {
            glTexCoord2f((float)j / newNX, (float)i / newNZ); 
            glVertex3f(X0 + DX * (float)j, YGRID, Z0 + DZ * (float)(i + 0));
            glTexCoord2f((float)j / newNX, (float)(i + 1) / newNZ); 
            glVertex3f(X0 + DX * (float)j, YGRID, Z0 + DZ * (float)(i + 1));
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
glEndList();


int woodWidth, woodHeight;
    unsigned char* woodImgData = BmpToTexture("wood.bmp", &woodWidth, &woodHeight);
    glGenTextures(1, &woodTex);
    glBindTexture(GL_TEXTURE_2D, woodTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, woodWidth, woodHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, woodImgData);

ObjectList4 = glGenLists(1);
glNewList(ObjectList4, GL_COMPILE);
    glTranslatef(0.0f, 0.2f, 0.0f); 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, woodTex);
glColor3f(0.32f, 0.22f, 0.12f); 

    // front  face
    glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, 0.1, W / 2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, 0.1, W / 2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, -0.1, W / 2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, -0.1, W / 2);
    glEnd();

	// Back face
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, 0.1, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, 0.1, -W / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, -0.1, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, -0.1, -W / 2);
	glEnd();

	// Left face
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, 0.1, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-L / 2, 0.1, W / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, -0.1, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-L / 2, -0.1, W / 2);
	glEnd();

	// Right face
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2, 0.1, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, 0.1, W / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2, -0.1, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, -0.1, W / 2);
	glEnd();

	// Bottom face
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, -1.0f, 0.0f); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, -0.1, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, -0.1, -W / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, -0.1, W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, -0.1, W / 2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEndList();

float legW = 0.2f; 
float legH = 0.5f; 

int wood2Width, wood2Height;
unsigned char* wood2ImgData = BmpToTexture("wood2.bmp", &wood2Width, &wood2Height);
glGenTextures(1, &wood2Tex);
glBindTexture(GL_TEXTURE_2D, wood2Tex);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, 3, wood2Width, wood2Height, 0, GL_RGB, GL_UNSIGNED_BYTE, wood2ImgData);

// legs 
ObjectList5 = glGenLists(1);
glNewList(ObjectList5 , GL_COMPILE);

//top left leg 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wood2Tex);

	// back side 
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, 0.1f, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+ legW, 0.1f, -W / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, -legH, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+ legW, -legH, -W / 2);
	glEnd();


	// front side 
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, 0.1f, (-W / 2) + legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2) + legW, 0.1f, (-W / 2) + legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, -legH, (-W / 2) + legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2) + legW, -legH, (-W / 2) + legW);

	glEnd();

	// Left side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, 0.1f, (-W / 2));
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2), 0.1f, (-W / 2) + legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, -legH, (-W / 2));
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), -legH, (-W / 2) + legW);

	glEnd();

	// Right side 
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2) + legW, 0.1f, (-W / 2));
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2) + legW, 0.1f, (-W / 2) + legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2) + legW, -legH, (-W / 2));
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2) + legW, -legH, (-W / 2) + legW);

	glEnd();

// bottom left leg 

	// Back side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, 0.1f, -W / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, -legH, -W / 2);
	glEnd();

	// Front side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, (-W / 2) + legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, 0.1f, (-W / 2) + legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, (-W / 2) + legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, -legH, (-W / 2) + legW);
	glEnd();

	// Left side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, (-W / 2));
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, (-W / 2) + legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, (-W / 2));
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, (-W / 2) + legW);
	glEnd();

	// Right side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2, 0.1f, (-W / 2));
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, 0.1f, (-W / 2) + legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2, -legH, (-W / 2));
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, -legH, (-W / 2) + legW);
	glEnd();

	// Back side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, W / 2 - legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, 0.1f, W / 2 - legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, W / 2 - legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, -legH, W / 2 - legW);
	glEnd();

	// Front side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, 0.1f, W / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, -legH, W / 2);
	glEnd();

	// Left side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, W / 2 - legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2 - legW, 0.1f, W / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, W / 2 - legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2 - legW, -legH, W / 2);
	glEnd();

	// Right side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(L / 2, 0.1f, W / 2 - legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(L / 2, 0.1f, W / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(L / 2, -legH, W / 2 - legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(L / 2, -legH, W / 2);
	glEnd();

	// Back side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, 0.1f, W / 2 - legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-L / 2 + legW, 0.1f, W / 2 - legW);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, -legH, W / 2 - legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-L / 2 + legW, -legH, W / 2 - legW);
	glEnd();

	// Front side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, 0.1f, W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-L / 2 + legW, 0.1f, W / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, -legH, W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-L / 2 + legW, -legH, W / 2);
	glEnd();

	// Left side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2, 0.1f, W / 2 - legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-L / 2, 0.1f, W / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2, -legH, W / 2 - legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-L / 2, -legH, W / 2);
	glEnd();

	// Right side
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-L / 2 + legW, 0.1f, W / 2 - legW);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-L / 2 + legW, 0.1f, W / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-L / 2 + legW, -legH, W / 2 - legW);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-L / 2 + legW, -legH, W / 2);
	glEnd();

	/////////////////////////////////////////////////////////////////////////////////////////////

	glColor3f(0.45f, 0.22f, 0.12f);  

	// Front left bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.4f, W / 2 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+(0.3), 0.4f, (W / 2) - 0.1 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, 0.4f, (W / 2)-0.1 );
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+(0.3), 0.4f, (W / 2) - 0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+(0.3), 0.3f, (W / 2) - 0.1 );
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+(0.3), 0.4f, (W / 2) - 0.1 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, 0.4f, (W / 2)-0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+(0.3), 0.3f, (W / 2) - 0.1 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, 0.3f, (W / 2)-0.1 );
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.4f, W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, 0.4f, (W / 2)-0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, 0.3f, (W / 2)-0.1 );
	glEnd();

	// Front right bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.4f, W / 2 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-(0.3), 0.4f, (W / 2) - 0.1 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.4f, (W / 2)-0.1 );
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.4f, W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.4f, (W / 2)-0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.3f, (W / 2)-0.1 );
	glEnd();


	// Back right bumper
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2)-(0.3), 0.4f, (W / 2) - 0.1 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.4f, (W / 2)-0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-(0.3), 0.3f, (W / 2) - 0.1 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.3f, (W / 2)-0.1 );
	glEnd();

	// Right wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f((L / 2)-(0.3), 0.4f, (W / 2) - 0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.3f, W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f((L / 2)-(0.3), 0.3f, (W / 2) - 0.1 );
	glEnd();

	// Front wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2), 0.4f, W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), 0.4f, W / 2 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2), 0.3f, W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2), 0.3f, W / 2 );
	glEnd();

	// Back left bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, -W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.4f, -W / 2 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+(0.3), 0.4f, (-W / 2) + 0.1 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, 0.4f, (-W / 2)+0.1 );
	glEnd();

	// Left wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, -W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+(0.3), 0.4f, (-W / 2) + 0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, -W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+(0.3), 0.3f, (-W / 2) + 0.1 );
	glEnd();

	// Back wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+(0.3), 0.4f, (-W / 2) + 0.1 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, 0.4f, (-W / 2)+0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+(0.3), 0.3f, (-W / 2) + 0.1 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, 0.3f, (-W / 2)+0.1 );
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.4f, -W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, 0.4f, (-W / 2)+0.1 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, -W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, 0.3f, (-W / 2)+0.1 );
	glEnd();
	// Back right bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.4f, -W / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-(0.3), 0.4f, (-W / 2) + 0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.4f, (-W / 2)+0.1);
	glEnd();

	// Side of back right bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.4f, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.4f, (-W / 2)+0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.3f, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.3f, (-W / 2)+0.1);
	glEnd();

	// Bottom of back right bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2)-(0.3), 0.4f, (-W / 2) + 0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.4f, (-W / 2)+0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-(0.3), 0.3f, (-W / 2) + 0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.3f, (-W / 2)+0.1);
	glEnd();

	// Side of back right bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((L / 2)-(0.3), 0.4f, (-W / 2) + 0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.3f, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((L / 2)-(0.3), 0.3f, (-W / 2) + 0.1);
	glEnd();

	// Back wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2), 0.4f, -W / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), 0.4f, -W / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2), 0.3f, -W / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2), 0.3f, -W / 2);
	glEnd();

	// Left side bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (-W / 2)+0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (W / 2)-0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.4f, (-W / 2)+0.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.4f, (W / 2)-0.3);
	glEnd();

	// Left side bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (-W / 2)+0.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (W / 2)-0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (-W / 2)+0.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (W / 2)-0.3);
	glEnd();

	// Left wall, back part
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (-W / 2)+0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (-W / 2)+0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (-W / 2)+0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (-W / 2)+0.3);
	glEnd();

	// Left wall, front part
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (W / 2)-0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (W / 2)-0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (W / 2)-0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (W / 2)-0.3);
	glEnd();

	// left wall// Back wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2), 0.4f, -W / 2 );
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), 0.4f, -W / 2 );
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2), 0.3f, -W / 2 );
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2), 0.3f, -W / 2 );
	glEnd();

	// Left side bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (-W / 2 )+0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (W / 2 )-0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.4f, (-W / 2 )+0.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.4f, (W / 2 )-0.3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (-W / 2 )+0.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (W / 2 )-0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (-W / 2 )+0.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (W / 2 )-0.3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (-W / 2 )+0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (-W / 2 )+0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (-W / 2 )+0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (-W / 2 )+0.3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (W / 2 )-0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2)+0.1, 0.4f, (W / 2 )-0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (W / 2 )-0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2)+0.1, 0.3f, (W / 2 )-0.3);
	glEnd();

	// Left wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (-W / 2 ));
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (W / 2 ));
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (-W / 2 ));
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (W / 2 ));
	glEnd();
	// Left wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (-W / 2 ));
		glTexCoord2f(1.0f, 0.0f); glVertex3f((-L / 2), 0.4f, (W / 2 ));
		glTexCoord2f(0.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (-W / 2 ));
		glTexCoord2f(1.0f, 1.0f); glVertex3f((-L / 2), 0.3f, (W / 2 ));
	glEnd();

	// Right side bumper
	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2), 0.4f, (-W / 2 )+0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((L / 2), 0.4f, (W / 2 )-0.1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.4f, (-W / 2 )+0.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.4f, (W / 2 )-0.3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, (-W / 2 )+0.3);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, (W / 2 )-0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.3f, (-W / 2 )+0.3);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.3f, (W / 2 )-0.3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2), 0.4f, (W / 2 )-0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, (W / 2 )-0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2), 0.3f, (W / 2 )-0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.3f, (W / 2 )-0.3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2), 0.4f, (-W / 2 )+0.1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((L / 2)-0.1, 0.4f, (-W / 2 )+0.3);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2), 0.3f, (-W / 2 )+0.1);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((L / 2)-0.1, 0.3f, (-W / 2 )+0.3);
	glEnd();

	// Right wall
	glBegin(GL_QUAD_STRIP);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((L / 2), 0.4f, (-W / 2 ));
		glTexCoord2f(1.0f, 0.0f); glVertex3f((L / 2), 0.4f, (W / 2 ));
		glTexCoord2f(0.0f, 1.0f); glVertex3f((L / 2), 0.3f, (-W / 2 ));
		glTexCoord2f(1.0f, 1.0f); glVertex3f((L / 2), 0.3f, (W / 2 ));
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEndList();









Circle = glGenLists(1);
glNewList(Circle, GL_COMPILE);

    glColor3f(0.0f, 0.0f, 0.0f);

    float centerX = 0.0f;
    float Y = 0.3001f;
    float centerZ = 0.0f; 
    float radius = 0.1f;
    int numSegments = 200;

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = ((-L/2) + radius) + cos(angle) * radius;
        float z = (W/2) - radius + sin(angle) * radius;
        glVertex3f(x, Y, z);
    }
    glEnd();


 glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = (L/2)- radius + cos(angle) * radius;
        float z = (W/2) -radius + sin(angle) * radius;
        glVertex3f(x, Y, z);
    }
    glEnd();

	 glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = (-L/2)+radius + cos(angle) * radius;
        float z = (-W/2)+radius + sin(angle) * radius;
        glVertex3f(x, Y, z);
    }
    glEnd();

 glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = (L/2)-radius + cos(angle) * radius;
        float z = (-W/2)+radius + sin(angle) * radius;
        glVertex3f(x, Y, z);
    }
    glEnd();


 glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = 0.0 + cos(angle) * radius;
        float z = (-W/2)+radius + sin(angle) * radius;
        glVertex3f(x, Y, z);
    }
    glEnd();


 glBegin(GL_TRIANGLE_FAN);
    //glVertex3f(centerX, centerY, centerZ);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = 0.0 + cos(angle) * radius;
        float z = (W/2)-radius  + sin(angle) * radius;
        glVertex3f(x, Y, z);
    }
    glEnd();
glEndList();

	// light 
	Light = glGenLists( 1 );
	glNewList( Light, GL_COMPILE );

    glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
	glColor3f(0.25f, 0.25f, 0.25f);
		 glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 1.5f, 0.0f);
   		glScalef(0.01f, 0.01f, 0.01f);
		LoadObjFile( (char *)"Light.obj" );
	glEndList( );


	// cue 
	Cue = glGenLists( 1 );
	glNewList( Cue, GL_COMPILE );
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shinyDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shinySpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shinyShininess);
		glColor3f(0.82, 0.70, 0.55);  
			 glScalef(0.3333f, 0.3333f, 0.3333f); 
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(7.0f, 0.0f, 0.0f, 1.0f);
		LoadObjFile( (char *)"cue.obj" );
	glEndList( );

	// light source sphere 
	LightSourceList = glGenLists(1);
		glNewList(LightSourceList, GL_COMPILE);
		glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
	glEndList();

	// light source sphere 
	LightSourceList1 = glGenLists(1);
		glNewList(LightSourceList1, GL_COMPILE);
		glTranslatef(0.0f, 1.52f, 0.0f);
		glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
	glEndList();

		// light source sphere 
	LightSourceList2 = glGenLists(1);
		glNewList(LightSourceList2, GL_COMPILE);
		glTranslatef(0.68f, 1.52f, 0.0f);
		glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
	glEndList();

		// light source sphere 
	LightSourceList3 = glGenLists(1);
		glNewList(LightSourceList3, GL_COMPILE); 
		glTranslatef(-0.68f, 1.52f, 0.0f);
		glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
	glEndList();

    // Create the axes:
    AxesList = glGenLists(1);
		glNewList(AxesList, GL_COMPILE);
		glLineWidth(AXES_WIDTH);
		Axes(1.5);
		glLineWidth(1.0);
	glEndList();
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )

{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			NowProjection = ORTHO;
			break;

		// green light case
    	case 'g':
		case 'G':
			ColorR = 0.0f; 
			ColorG = 1.0f;
			ColorB = 0.0f;
			glDeleteLists(LightSourceList, 1);
			LightSourceList = glGenLists(1);
			glNewList(LightSourceList, GL_COMPILE);
			glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
			glEndList();
			glutPostRedisplay();
			break;

		// white light case
		case 'w':
		case 'W':
			ColorR = 1.0f; 
			ColorG = 1.0f;
			ColorB = 1.0f;
			glDeleteLists(LightSourceList, 1);
			LightSourceList = glGenLists(1);
			glNewList(LightSourceList, GL_COMPILE);
			glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
			glEndList();
			glutPostRedisplay();
			break;

		// red light case
		case 'r':
		case 'R':
			ColorR = 1.0f; 
			ColorG = 0.0f;
			ColorB = 0.0f;
			glDeleteLists(LightSourceList, 1);
			LightSourceList = glGenLists(1);
			glNewList(LightSourceList, GL_COMPILE);
			glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
			glEndList();
			glutPostRedisplay();
			break;

		// blue light case
		case 'b':
		case 'B':
			ColorR = 0.0f; 
			ColorG = 0.0f;
			ColorB = 1.0f;
			glDeleteLists(LightSourceList, 1);
			LightSourceList = glGenLists(1);
			glNewList(LightSourceList, GL_COMPILE);
			glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
			glEndList();
			glutPostRedisplay();
			break;

		// yellow light case
		case 'y':
		case 'Y':
			ColorR = 1.0f; 
			ColorG = 1.0f;
			ColorB = 0.0f;
			glDeleteLists(LightSourceList, 1);
			LightSourceList = glGenLists(1);
			glNewList(LightSourceList, GL_COMPILE);
			glColor3f(ColorR, ColorG, ColorB);  
			OsuSphere(0.08, 50, 50);
			glEndList();
			glutPostRedisplay();
			break;

		// point light 
		case 'p':
		case 'P':
			Spotlight = 0;
			break;

		// spot light 
		case 's':
		case 'S':
			Spotlight = 1;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}

	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}

	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	ShadowsOn = 0;
	NowColor = YELLOW;
	NowProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = { 0.f, 1.f, 0.f, 1.f };

static float xy[ ] = { -.5f, .5f, .5f, -.5f };

static int xorder[ ] = { 1, 2, -3, 4 };

static float yx[ ] = { 0.f, 0.f, -.5f, .5f };

static float yy[ ] = { 0.f, .6f, 1.f, 1.f };

static int yorder[ ] = { 1, 2, 3, -2, 4 };

static float zx[ ] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f };

static float zy[ ] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f };

static int zorder[ ] = { 1, 2, 3, 4, -5, 6 };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = (float)floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r=0., g=0., b=0.;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void
Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}

float
Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}


float
Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}
	return dist;
}


float
Unit( float v[3] )
{
	float dist = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		v[0] /= dist;
		v[1] /= dist;
		v[2] /= dist;
	}
	return dist;
}
