
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cfloat>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

#ifdef __APPLE__
#  include "GL/freeglut.h"
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#endif

#ifndef MESH_H
#define MESH_H

#define DEG2RAD_MES 0.017453292519943 /* rad = deg*PI/180 = deg*DEG2RAD */
#define RAD2DEG_MES 57.29577951308232 /* deg = rad*180/PI = rad*RAD2DEG */

using namespace std;
using namespace Eigen;


struct mesh
{
	vector<Vector3f> vertices, normals, textures;
	vector< vector<unsigned int> > faceVertices;

	// properties
	float meshScale; // object scale divisor
	Vector3f center; // object center
	Vector3f translation; // object translation
	Quaternionf quaternion; // object orientation
	float color[4];
    unsigned int bufferIds[3];

	// display list
	unsigned int displayList;

	// vertex array
	float* vertexArray;
    // normal array
    float* normalArray;
    // texture array
    float* TextureArray;
	unsigned int numVertices, numNormals, numTextures;
	unsigned int* triangleIndices;
	unsigned int numTriangles;
	unsigned int vaoId[1];


	// member functions

	mesh()
	{
		translation << 0, 0, 0;
		quaternion = Quaternionf::Identity();
		color[0] = color[1] = color[2] = 1.0;
		color[3] = 1.0;
		displayList = 0;
		vertexArray = NULL;
		numVertices = 0;
		triangleIndices = NULL;
		numTriangles = 0;
	}


	void reset()
	{
		translation << 0, 0, 0;
		quaternion = Quaternionf::Identity();
	}


	void glPosition(Vector3f initialPosition)
	// place mesh in specified position+orientation in openGL
	{
		glTranslatef(translation[0], translation[1], translation[2]);
		glTranslatef(initialPosition[0], initialPosition[1], initialPosition[2]);
		float w = max( (float)-1, min( (float)1, quaternion.w() ) ); // |w|<=1
		float angle = 2*acos(w)*RAD2DEG_MES;
		glRotatef(angle, quaternion.x(), quaternion.y(), quaternion.z());
		glScalef(meshScale, meshScale, meshScale);
		glTranslatef(-center[0], -center[1], -center[2]);
	}


	void glColor()
	{
		glColor4fv(color);
	}


	void xTransl(float trans) // translate along mesh x-axis
	{
		Vector3f axis = quaternion._transformVector(Vector3f::UnitX());
		translation += trans*axis;
	}


	void yTransl(float trans) // translate along mesh y-axis
	{
		Vector3f axis = quaternion._transformVector(Vector3f::UnitY());
		translation += trans*axis;
	}


	void zTransl(float trans) // translate along mesh z-axis
	{
		Vector3f axis = quaternion._transformVector(Vector3f::UnitZ());
		translation += trans*axis;
	}


	void xRotate(float angleDeg) // pitch around mesh x-axis
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_MES;
		Vector3f axis = quaternion._transformVector(Vector3f::UnitX());
		Vector4f newRotation;
		newRotation << axis * -sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}


	void yRotate(float angleDeg) // yaw around mesh y-axis
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_MES;
		Vector3f axis = quaternion._transformVector(Vector3f::UnitY());
		Vector4f newRotation;
		newRotation << axis * -sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}


	void zRotate(float angleDeg) // roll around mesh z-axis
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_MES;
		Vector3f axis = quaternion._transformVector(Vector3f::UnitZ());
		Vector4f newRotation;
		newRotation << axis * -sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}


	// other member functions defined in mesh.cpp

	void readObjFile(char*);
	void writeObjFile(char*);
	void normalize();

		// display list
	void glCreateDisplayList();
	void glCallDisplayList();

		// vertex array
	void createVertexArray();
	void glEnableVertexArray();
	void glDrawVertexArray();

		// VBO
	void glEnableVBO();
	void glDrawVBO();

		// VAO
	void glEnableVAO();
	void glDrawVAO();

};

#endif
