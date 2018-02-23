#ifndef _MyCircle_H_
#define _MyCircle_H_

#include"stdafx.h"
#include"Vector2f.h"

class MyCircle{
public:
	Vector2f center; //Ô²ÐÄ
	float radius; //°ë¾¶

	MyCircle(){}
	MyCircle(const Vector2f& cen, const float& r){
		center = cen;
		radius = r;
	}

	inline const MyCircle operator=(const MyCircle& circle);
};

inline const MyCircle MyCircle::operator=(const MyCircle& circle){
	this->center = circle.center;
	this->radius = circle.radius;
	return (*this);
}

#endif _MyCircle_H_