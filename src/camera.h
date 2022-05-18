/*
 * The header file for the camera manipulation 
*/
#include <cmath>
#include <vector>
#include <Eigen/Dense>

#ifndef CAMERA_H
#define CAMERA_H

#define DEG2RAD_CAM 0.017453292519943 /* rad = deg*PI/180 = deg*DEG2RAD */
#define RAD2DEG_CAM 57.29577951308232 /* deg = rad*180/PI = rad*RAD2DEG */

using namespace std;
using namespace Eigen;


enum cameraType {ortho, persp};

struct camera
{
	cameraType type;
	Vector2f xView; // view volume left, right
	Vector2f yView; // view volume down, up
	Vector2f zView; // view volume -near, -far
	Vector3f transl; // translation
	Quaternionf quaternion; // orientation


	// member functions

	/*
	 * initializer not constructor, since we also want to reset after creation
	 * @param tp the camera's projection either Perspective and Orthographic 
	 * @param x0, x1 the projection x coordinates
	 * @param y0, y1 the projection y coordinates
	 * @param z0, z1 the projection z coordinates 
	*/ 
	void initialize(cameraType tp,
	       float x0, float x1, float y0, float y1, float z0, float z1)
	{
		type = tp;
		xView << x0, x1;
		yView << y0, y1;
		zView << z0, z1;
		transl = Vector3f::Zero();
		quaternion = Quaternionf::Identity();
	}

	/*
	 * Define view volume in openGL 
	*/

	void glVolume() 
	{
		if (type == ortho)
			glOrtho(xView[0], xView[1], yView[0], yView[1], zView[0], zView[1]);
		else
			glFrustum(xView[0], xView[1], yView[0], yView[1], zView[0], zView[1]);
	}

	/*
	 * Place camera in specified position+orientation in openGL
	*/
	void glPosition()  
	{
		float w = max( (float)-1, min( (float)1, quaternion.w() ) ); // |w|<=1
		float angle = 2*acos(w)*RAD2DEG_CAM;
		glRotatef(angle, quaternion.x(), quaternion.y(), quaternion.z()); // orient
		glTranslatef(transl[0], transl[1], transl[2]); // position
	}
	
	/*
	 * Dolly along camera x-axis
	*/

	void xTransl(float trans) 
	{
		Vector3f axis = quaternion.conjugate()._transformVector(Vector3f::UnitX());
		transl += trans*axis;
	}

	/*
	 * Crane along camera y-axis
	*/

	void yTransl(float trans)  
	{
		Vector3f axis = quaternion.conjugate()._transformVector(Vector3f::UnitY());
		transl += trans*axis;
	}

	/*
	 * Zoom along camera z-axis
	*/

	void zTransl(float trans)  
	{
		Vector3f axis = quaternion.conjugate()._transformVector(Vector3f::UnitZ());
		transl += trans*axis;
	}

	/*
	 * Tilt around camera x-axis
	*/

	void xRotate(float angleDeg) 
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_CAM;
		Vector4f newRotation;
		newRotation << Vector3f::UnitX() * sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}


	/*
	 * Pan around camera y-axis
	*/

	void yRotate(float angleDeg) 
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_CAM;
		Vector4f newRotation;
		newRotation << Vector3f::UnitY() * sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}

	/*
	 * Roll around camera z-axis
	*/

	void zRotate(float angleDeg) // 
	{
		float angleQuat = angleDeg/2.0*DEG2RAD_CAM;
		Vector4f newRotation;
		newRotation << Vector3f::UnitZ() * sin(angleQuat), cos(angleQuat);
		quaternion = (Quaternionf)newRotation * quaternion;
	}

};

#endif
