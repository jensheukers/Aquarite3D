/**
*	Filename: vec3.h
*
*	Description: Header file for Vec3 class, Note that this class can also be used as a 2 dimensional vector
*
*	Version: 22/11/2018
*
*	© 2018, Jens Heukers
*/

#ifndef VEC3_H
#define VEC3_H

//Define PI
#define PI 3.14159265358979323846

#include <glm/glm.hpp>

class Vec3 {
public:
	float x, y, z; /// @brief Dimensions of the vector

	/**
	* Default Constructor
	*/
	Vec3();

	/**
	* Constructor with parameters
	*/
	Vec3(float x, float y, float z);

	/**
	* Set the vector to given param
	*/
	Vec3* Set(Vec3 other);

	/**
	* Multiply the vector by other Vec3 values
	*/
	Vec3* Multiply(Vec3 other);

	/**
	* Static Multiply the vector by other Vec3 values
	*/
	static Vec3 Multiply(Vec3 a, Vec3 b);

	/**
	* Devide the vector by other Vec3 values
	*/
	Vec3* Devide(Vec3 other);

	/**
	* Static Devide the vector by other Vec3 values
	*/
	static Vec3 Devide(Vec3 a, Vec3 b);

	/**
	* Add to vector by other Vec3 values
	*/
	Vec3* Add(Vec3 other);

	/**
	* static Add to vector by other Vec3 values
	*/
	static Vec3 Add(Vec3 a, Vec3 b);

	/**
	* Subtracts to vector by other Vec3 values
	*/
	Vec3* Minus(Vec3 other);

	/**
	* static Subtracts to vector by other Vec3 values
	*/
	static Vec3 Minus(Vec3 a, Vec3 b);

	/**
	* Linarly interpolates between this and other Vec3.
	*/
	Vec3* Lerp(Vec3 other, float amount);

	/**
	* Linarly interpolates between two Vec3's
	*/
	static Vec3 Lerp(Vec3 a, Vec3 b, float amount);

	/**
	* Returns true if given parameter equals other Vec3
	*/
	bool Equals(Vec3 other);

	/**
	* Returns the distance between the 2 vectors
	*/
	float Distance(Vec3 other);

	/**
	* Returns the magnitude
	*/
	float Magnitude();

	/**
	* Returns the angle
	*/
	float Angle(Vec3 other);
	
	/**
	* Normalize the vector by magnitude
	*/
	Vec3* Normalize();

	/**
	* Limit the vector to another vector
	*/
	Vec3* Limit(Vec3 other);

	/**
	* Converts Vec3 To a glm::vec3 type
	*/
	glm::vec3 ToGLM();

	/**
	* Converts glm::vec3 To a Vec3 type
	*/
	static Vec3 ToVec3(glm::vec3 glm);

	/**
	* Converts from radians to degrees, returns float
	*/
	static float RadToDeg(float r);

	/**
	* Converts from degrees to radians, returns float
	*/
	static float DegToRad(float d);

	/*** Returns a Vec3 with coordinates: 0.0f. 1.0f, 0.0f*/
	static Vec3 Up() { return Vec3(0.0f, 1.0f, 0.0f); }

	/*** Returns a Vec3 with coordinates: 0.0f. -1.0f, 0.0f*/
	static Vec3 Down() { return Vec3(0.0f, -1.0f, 0.0f); }

	/*** Returns a Vec3 with coordinates: 0.0f. 0.0f, 1.0f*/
	static Vec3 Forward() { return Vec3(0.0f, 0.0f, 1.0f); }

	/*** Returns a Vec3 with coordinates: 0.0f. 0.0f, -1.0f*/
	static Vec3 Back() { return Vec3(0.0f, 0.0f, -1.0f); }

	/*** Returns a Vec3 with coordinates: 1.0f. 0.0f, 0.0f*/
	static Vec3 Right() { return Vec3(1.0f, 0.0f, 0.0f); }

	/*** Returns a Vec3 with coordinates: -1.0f. 0.0f, 0.0f*/
	static Vec3 Left() { return Vec3(-1.0f, 0.0f, 0.0f); }
};

#endif // !VEC3_H

