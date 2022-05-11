// CMPUT 411/511 Assignment 1 solution
// Usage: ./modelviewerVBO <mesh.obj>

#ifdef __APPLE__
#  include "GL/glew.h"
#  include "GL/freeglut.h"
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#endif

#include "camera.h"
#include "mesh.h"
#include "TGALoader.h"


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
static unsigned int texture[1];

// Load external textures.
void loadExternalTextures()
{
    // Local storage for bmp image data.
   /* BitMapFile *image[2];
    
    // Load the images.
    image[0] = getbmp("../../Textures/grass.bmp");
    image[1] = getbmp("../../Textures/sky.bmp");
    
    // Bind grass image to texture object texture[0].
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    // Bind sky image to texture object texture[1]
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("modelViewer");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glewExperimental = GL_TRUE;
	glewInit();
	//setup(argv[1]);
    setup("/Users/youssef_mac/Documents/CppProjects/SolidViewer/SolidViewer/person_large.obj");
	glutMainLoop();
	return 0;
}


void setup(char* fileName)
{
	obj.readObjFile(fileName);
	obj.normalize();
	obj.createVertexArray();
    cam.initialize(ortho, -0.3, 0.3, -0.3, 0.3, 1.0, 100.0);
    
    /// TGA LOADER
    glGenTextures(1, texture);
    NS_TGALOADER::IMAGE tga;
    tga.LoadTGA("/Users/youssef_mac/Documents/CppProjects/SolidViewerVBO/skin.tga");
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    gluBuild2DMipmaps (GL_TEXTURE_2D, 4, tga.getWidth (), tga.getHeight (), GL_BGRA, GL_UNSIGNED_BYTE, tga.getDataForOpenGL  ());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);
    
    // LIGHTING /////////////////////////////////
    glEnable(GL_LIGHTING);
    
    // Light property vectors.
    float lightAmb[] = { 0.2, 0.2, 0.2, 1.0 };
    float lightDifAndSpec[] = { 0.2, 0.2, 0.2, 1.0 };
    float lightPos[] = {0.0,20.0, 4.0, 1.0};
    float globAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    
    
    
    // Light properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    glEnable(GL_LIGHT0); // Enable particular light source.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    
    // Material property vectors.
    float matAmbAndDif1[] = {0.05, 0.05, 0.05, 0.0};
    float matSpec[] = { 0.1, 0.1, 0.1, 1.0 };
    float matShine[] = { 0.0 };
    
    // Material properties.
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    
    // Enable color material mode.
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    //  END OF LIGHTING //////////////////////////*/

    
    obj.glEnableVBO();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.glVolume(); // camera view volume
    
    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glPushMatrix();
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
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
    obj.glColor();
	obj.glDrawVBO();
    glPopMatrix();
	glutSwapBuffers();
}


void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}


void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'q': exit(0);            break; // quit
		case 'w': obj.writeObjFile("output.obj"); break;
		case 'n': obj.zTransl(-0.1);  break;
		case 'N': obj.zTransl(0.1);   break;
		case 'p': obj.xRotate(-10.0); break;
		case 'P': obj.xRotate(10.0);  break;
		case 'y': obj.yRotate(-10.0); break;
		case 'Y': obj.yRotate(10.0);  break;
		case 'r': obj.zRotate(-10.0); break;
		case 'R': obj.zRotate(10.0);  break;
		case 'v': cam.type = ortho;   break; // toggle camera type
		case 'V': cam.type = persp;   break;
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
		case 'f': fog = false;        break; // toggle fog off
		case 'F': fog = true;         break; // toggle fog on
		case 'x': // reset
			obj.reset();
			cam.initialize(ortho, -1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
			fog = false;
			break;
		default: break;
	}
	glutPostRedisplay();
}


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


