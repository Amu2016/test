#include"Vector2f.h"

float Vector2f::length() const{
	return sqrtf(x*x + y*y);
}

float Vector2f::lengthSquared() const{
	return (x*x + y*y);
}

float Vector2f::distanceSquared(const Vector2f& v) const{
	return (*this - v).lengthSquared();
}

void Vector2f::normalize(){
	float n = x*x + y*y;
	if (n == 1.0f)
		return;
	n = sqrt(n);
	if (n < MATH_TOLERANCE)
		return;
	n = 1.0f / n;
	x *= n;
	y *= n;
}

Vector2f Vector2f::getNormalize() const{
	Vector2f v(*this);
	v.normalize();
	return v;
}

float Vector2f::smallestAngleBetween(const Vector2f& v) const{
	return 1;
}

float Vector2f::angleBetween(const Vector2f& v) const{
	return 1;
}

void Vector2f::rotateAroundOrigin(const float& angle, bool cw){
	if (cw)
		float angle = -angle;
	double sinAngle = sin(angle);
	double cosAngle = cos(angle);

	float tempX = x*cosAngle - y*sinAngle;
	y = y*cosAngle + x*sinAngle;
	x = tempX;
}

bool Vector2f::equals(const Vector2f& v) const{
	return (fabs(this->x - v.x) < 0.001f) && (fabs(this->y - v.y) < 0.001f);
}

Vector2f Vector2f::clone() const{
	Vector2f v(*this);
	return v;
}

