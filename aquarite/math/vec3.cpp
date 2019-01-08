/**
*	Filename: vec3.cpp
*
*	Description: Source file for Vec3 class
*
*	Version: 22/11/2018
*
*	© 2018, Jens Heukers
*/

#include "vec3.h"
#include <cmath>

Vec3::Vec3() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vec3::Vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3* Vec3::Set(Vec3 other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return this;
}

Vec3* Vec3::Multiply(Vec3 other) {
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
	return this;
}

Vec3 Vec3::Multiply(Vec3 a, Vec3 b) {
	Vec3 vec;
	vec.x = a.x *= b.x;
	vec.y = a.y *= b.y;
	vec.z = a.z *= b.z;
	return vec;
}

Vec3* Vec3::Devide(Vec3 other) {
	this->x /= other.x;
	this->y /= other.y;
	this->z /= other.z;
	return this;
}

Vec3 Vec3::Devide(Vec3 a, Vec3 b) {
	Vec3 vec;
	vec.x = a.x /= b.x;
	vec.y = a.y /= b.y;
	vec.z = a.z /= b.z;
	return vec;
}

Vec3* Vec3::Add(Vec3 other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return this;
}

Vec3 Vec3::Add(Vec3 a, Vec3 b) {
	Vec3 vec;
	vec.x = a.x += b.x;
	vec.y = a.y += b.y;
	vec.z = a.z += b.z;
	return vec;
}


Vec3* Vec3::Minus(Vec3 other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return this;
}

Vec3 Vec3::Minus(Vec3 a, Vec3 b) {
	Vec3 vec;
	vec.x = a.x -= b.x;
	vec.y = a.y -= b.y;
	vec.z = a.z -= b.z;
	return vec;
}

Vec3* Vec3::Lerp(Vec3 other, float amount) {
	this->x = (this->x + other.x) * amount;
	this->y = (this->y + other.y) * amount;
	this->z = (this->z + other.z) * amount;
	return this;
}

Vec3 Vec3::Lerp(Vec3 a,Vec3 b, float amount) {
	float x = (a.x + b.x) * amount;
	float y = (a.y + b.y) * amount;
	float z = (a.z + b.z) * amount;
	return Vec3(x,y,z);
}

float Vec3::Angle() {
	return atan2(this->y, this->x);
}

Vec3* Vec3::Normalize() {
	float m = this->Magnitude();
	this->x = this->x / m;
	this->y = this->y / m;
	this->z = this->z / m;
	return this;
}

Vec3* Vec3::Limit(Vec3 other) {
	if (this->x > other.x) {
		this->x = other.x;
	}

	if (this->y > other.y) {
		this->y = other.y;
	}

	if (this->z > other.z) {
		this->z = other.z;
	}
	return this;
}

glm::vec3 Vec3::ToGLM() {
	return glm::vec3(this->x,this->y,this->z);
}

float Vec3::RadToDeg(float r) {
	return (float)(360 - (r / PI) * 180);
}

float Vec3::DegToRad(float d) {
	return (float)(d * PI / 180);
}

bool Vec3::Equals(Vec3 other) {
	if (this->x == other.x && this->y == other.y && this->z == other.z) {
		return true;
	}
	return false;
}

float Vec3::Distance(Vec3 other) {
	return this->Minus(other)->Magnitude();
}

float Vec3::Magnitude() {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}