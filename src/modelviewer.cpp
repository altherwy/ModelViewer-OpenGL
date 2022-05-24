/*
 * This file contains the main method and
 * the required methods to setup the program
*/
#ifdef __APPLE__
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#endif

#include "camera.h"
#include "mesh.h"


int main(int, char**);
void setup(char*);
void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void specialKeyInput(int, int, int);


/* global variables */
mesh obj;
camera cam;
Vector3f initialPosition(0.0, 0.0, -2.0);
bool fog = true;
const float fogColor[4] = {1.0, 1.0, 1.0, 0.0};


int main(int argc, char** argv)
{
	/* Draw the window */
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("modelviewer");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glewExperimental = GL_TRUE;
	glewInit();
	setup(argv[1]);
	glutMainLoop();
	return 0;
}

/*
 * setup the program
 * @param filename the .obj file to read
*/
void setup(char* fileName)
{
	obj.readObjFile(fileName); // read the object
	obj.normalize();
	obj.glCreateDisplayList();
	cam.initialize(ortho, -1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
}

/*
 * draw the scene 
*/

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.glVolume(); // camera view volume

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.glPosition(); // camera transformation
	obj.glPosition(initialPosition); // object transformation

	if (fog) { // set fog
		glEnable(GL_FOG);
		glHint(GL_FOG_HINT, GL_NICEST);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogi(GL_FOG_START, 1.0);
		glFogi(GL_FOG_END, 5.0);
	} else
		glDisable(GL_FOG);

	// draw model
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	obj.glColor();
	obj.glCallDisplayList();
	glutSwapBuffers();
}

/*
 * resize the window 
 * @param w the wanted width of the window
 * @param h the wanted height of the window 
*/

void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/*
 * the keys to manipulate the model
 * @param key the keyboard key pressed
 * @param x the mouse relative coordinate along the x axis
 * @param y the mouse relative coordinate along the y axis
*/
void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'q': exit(0);            break; // quit
		case 'w': obj.writeObjFile("output.obj"); break;

		// Manipulate the camera
		case 'd': cam.xTransl(0.1);   break;
		case 'D': cam.xTransl(-0.1);  break;
		case 'c': cam.yTransl(0.1);   break;
		case 'C': cam.yTransl(-0.1);  break;
		case 'i': cam.zTransl(0.1);   break;
		case 'I': cam.zTransl(-0.1);  break;
		case 't': cam.xRotate(10.0);  break;
		case 'T': cam.xRotate(-10.0); break;
		case 'a': cam.yRotate(10.0);  break;
		case 'A': cam.yRotate(-10.0); break;
		case 'l': cam.zRotate(10.0);  break;
		case 'L': cam.zRotate(-10.0); break;
		
		// Manipulate the model
		case 'n': obj.zTransl(-0.1);  break;
		case 'N': obj.zTransl(0.1);   break; 
		case 'p': obj.xRotate(-10.0); break;
		case 'P': obj.xRotate(10.0);  break;
		case 'y': obj.yRotate(-10.0); break;
		case 'Y': obj.yRotate(10.0);  break;
		case 'r': obj.zRotate(-10.0); break;
		case 'R': obj.zRotate(10.0);  break;
		
		// toggle camera type
		case 'v': cam.type = ortho;   break; 
		case 'V': cam.type = persp;   break;
		
		// toggle fog on and off
		case 'f': fog = false;        break; 
		case 'F': fog = true;         break; 
		case 'x': // reset
			obj.reset();
			cam.initialize(ortho, -1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
			fog = false;
			break;
		default: break;
	}
	glutPostRedisplay();
}

/*
 * sets the special keyboard callback for the current window
 * @param key the keyboard key pressed
 * @param x the mouse in window relative coordinate along the x axis when the key was pressed
 * @param y the mouse in window relative coordinate along the y axis when the key was pressed 
*/

void specialKeyInput(int key, int x, int y)
{
	switch(key) 
	{
		case GLUT_KEY_LEFT:  obj.xTransl(-0.1); break;
		case GLUT_KEY_RIGHT: obj.xTransl(0.1);  break;
		case GLUT_KEY_DOWN:  obj.yTransl(-0.1); break;
		case GLUT_KEY_UP:    obj.yTransl(0.1);  break;
		default: break;
	}
	glutPostRedisplay();
}


