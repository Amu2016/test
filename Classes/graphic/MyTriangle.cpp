#include"MyTriangle.h"

const int MyTriangle::SIDE_AB = 0;
const int MyTriangle::SIDE_BC = 1;
const int MyTriangle::SIDE_CA = 2;

void MyTriangle::calculateData(){
	_center = (_pointA + _pointB + _pointC) / 3;
	
	_sides = vector<Line2D>();
	_sides.resize(3);
	_sides[SIDE_AB] = Line2D(_pointA, _pointB);
	_sides[SIDE_BC] = Line2D(_pointB, _pointC);
	_sides[SIDE_CA] = Line2D(_pointC, _pointA);

	dataCalculated = true;
}

Vector2f MyTriangle::getVertex(const int& i) const{
	switch (i)
	{
	case 0:
		return _pointA;
	case 1:
		return _pointB;
	case 2:
		return _pointC;
	default:
		return Vector2f();
	}
}

void MyTriangle::setVertex(const int& i, const Vector2f& point){
	switch (i)
	{
	case 0:
		_pointA = point;
		break;
	case 1:
		_pointB = point;
		break;
	case 2:
		_pointC = point;
		break;
	default:
		break;
	}
	dataCalculated = false;
}

Line2D MyTriangle::getSide(const int& i){
	if (!dataCalculated)
		calculateData();
	return _sides[i];
}

bool MyTriangle::isPointIn(const Vector2f& point){
	if (!dataCalculated)
		calculateData();
	// 点在所有边的右面
	int interiorCount = 0;
	for (int i = 0; i < 3; i++){
		if (_sides[i].classifyPoint(point) != Line2D::RIGHT_SIDE)
			interiorCount++;
	}
	return (interiorCount == 3);
}

MyTriangle MyTriangle::clone() const{
	return MyTriangle();
}

void MyTriangle::clear(){
	for (auto& l : line)
		l->removeFromParent();
	for (auto text : centerPos)
		text->removeFromParent();
}