#ifndef _Vector2f_H_
#define _Vector2f_H_

#include"stdafx.h"

class Vector2f{
public:
	float x;
	float y;

	Vector2f(){}

	Vector2f(float _x, float _y){
		x = _x;
		y = _y;
	}

	Vector2f(const Vector2f& v){
		x = v.x;
		y = v.y;
	}

	float getAngle(){
		return atan2f(y, x);
	}

	float length() const;

	float lengthSquared() const;

	float distanceSquared(const Vector2f& v) const;

	inline float dot(const Vector2f& v) const;

	inline Vector2f negate();

	void normalize();

	Vector2f getNormalize() const;

	float smallestAngleBetween(const Vector2f& v) const;

	float angleBetween(const Vector2f& v) const;

	inline Vector2f lerp(const Vector2f& v, float alpha) const;

	void rotateAroundOrigin(const float& angle, bool cw);

	bool equals(const Vector2f& v) const;

	Vector2f clone() const;

	inline const Vector2f operator=(const Vector2f& v);

	inline const Vector2f operator+(const Vector2f& v) const;

	inline const Vector2f operator-(const Vector2f& v) const;

	inline const Vector2f operator*(const float& s) const;

	inline const Vector2f operator/(const float& s) const;
	
	inline bool operator==(const Vector2f& v) const;

	inline bool operator!=(const Vector2f& v) const;

private:

};

inline float Vector2f::dot(const Vector2f& v) const{
	return (x*v.x + y*v.y);
}

inline Vector2f Vector2f::negate(){
	x = -x; 
	y = -y;
}

inline Vector2f Vector2f::lerp(const Vector2f& v, float alpha) const{
	return *this * (1.0f - alpha) + v * alpha;
}

inline const Vector2f Vector2f::operator=(const Vector2f& v){
	this->x = v.x;
	this->y = v.y;
	return (*this);
}

inline const Vector2f Vector2f::operator+(const Vector2f& v) const{
	return Vector2f(this->x + v.x, this->y + v.y);
}

inline const Vector2f Vector2f::operator-(const Vector2f& v) const{
	return Vector2f(this->x - v.x, this->y - v.y);
}

inline const Vector2f Vector2f::operator*(const float& s) const{
	return Vector2f(this->x * s, this->y * s);
}

inline const Vector2f Vector2f::operator/(const float& s) const{
	return Vector2f(this->x / s, this->y / s);
}

inline bool Vector2f::operator == (const Vector2f& v) const{
	return this->x == v.x && this->y == v.y;
}

inline bool Vector2f::operator != (const Vector2f& v) const{
	return this->x != v.x || this->y != v.y;
}

#endif _Vector2f_H_