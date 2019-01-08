/**
*	Filename: pointx.h
*
*	Description: Pointx is a template class with multiple types of points, point sizes can be x,y,z,w
*				 Declarations are added in the headers
*
*	Version: 19/12/2018
*
*	© 2018, Jens Heukers
*/
#ifndef POINTX_H
#define POINTX_H

template<class T>
class Point {
public:
	T x; /// @brief x component of Point

	/**
	* Constructor
	*/
	Point<T>() {
		this->x = 0;
	}

	/**
	* Constructor with parameters
	*/
	Point<T>(T x) {
		this->x = x;
	}
};

template<class T>
class Point2 {
public:
	T x, y; /// @brief x, y components of Point2

	/**
	* Constructor
	*/
	Point2<T>() {
		this->x = 0;
		this->y = 0;
	}

	/**
	* Constructor with parameters
	*/
	Point2<T>(T x, T y) {
		this->x = x;
		this->y = y;
	}
};

template<class T>
class Point3 {
public:
	T x, y, z; /// @brief x, y, z components of Point3

	/**
	* Constructor
	*/
	Point3<T>() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	/**
	* Constructor with parameters
	*/
	Point3<T>(T x, T y, T z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

template<class T>
class Point4 {
public:
	T x, y, z, w; /// @brief x, y, z, w components of Point4
	
	/**
	* Constructor
	*/
	Point4<T>() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	/**
	* Constructor with parameters
	*/
	Point4<T>(T x, T y, T z, T w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

//Typedefs
typedef Point<int> Point1i;
typedef Point<float> Point1f;
typedef Point<double> Point1d;

typedef Point2<int> Point2i;
typedef Point2<float> Point2f;
typedef Point2<double> Point2d;

typedef Point3<int> Point3i;
typedef Point3<float> Point3f;
typedef Point3<double> Point3d;

typedef Point4<int> Point4i;
typedef Point4<float> Point4f;
typedef Point4<double> Point4d;

#endif // !POINTX_H