#ifndef _MyTriangle_H_
#define _MyTriangle_H_

#include "stdafx.h"
#include "Line2D.h"

class MyTriangle{
public:
	MyTriangle(){
		dataCalculated = false;
	};
	MyTriangle(Vector2f p1, Vector2f p2, Vector2f p3){
		_pointA = p1;
		_pointB = p2;
		_pointC = p3;

		vertexV.push_back(_pointA);
		vertexV.push_back(_pointB);
		vertexV.push_back(_pointC);

		dataCalculated = false;
	}

	/**
	* �������ĵ㣨3�������ƽ��ֵ��
	*/
	void calculateData();

	/**
	* ����i���ض���
	*/
	Vector2f getVertex(const int& i) const;

	/**
	* ����iָ�����������������εĶ���
	*/
	void setVertex(const int& i, const Vector2f& point);

	/**
	* ȡ��ָ�������ı�(��0��ʼ��˳ʱ��)
	*/
	Line2D getSide(const int& i);

	/**
	* ���Ը������Ƿ�����������
	*/
	bool isPointIn(const Vector2f& point);

	MyTriangle clone() const;

	void clear();


	inline const MyTriangle operator=(const MyTriangle& v);

	inline bool operator==(const MyTriangle& trig) const;

	inline bool operator!=(const MyTriangle& trig) const;

public:
	Vector2f _pointA;
	Vector2f _pointB;
	Vector2f _pointC;

	vector<Vector2f> vertexV;

	Vector2f _center; //���ĵ�

	std::vector<Line2D> _sides; // �����͵�3����

	vector<DrawNode*> line; // �����͵�3���ߵ�node

	vector<Text*> centerPos;

	static const int SIDE_AB;
	static const int SIDE_BC;
	static const int SIDE_CA;
private:
	bool dataCalculated; //���ĵ��Ƿ��Ѽ���
	
};

inline const MyTriangle MyTriangle::operator=(const MyTriangle& triangle){
	this->_pointA = triangle._pointA;
	this->_pointB = triangle._pointB;
	this->_pointC = triangle._pointC;

	this->vertexV = triangle.vertexV;

	this->_center = triangle._center;

	this->_sides = triangle._sides;

	this->dataCalculated = false;
	return (*this);
}

inline bool MyTriangle::operator == (const MyTriangle& trig) const{
	return (this->_pointA == trig._pointA && this->_pointB == trig._pointB && this->_pointC == trig._pointC);
}

inline bool MyTriangle::operator != (const MyTriangle& trig) const{
	return (this->_pointA != trig._pointA || this->_pointB != trig._pointB || this->_pointC != trig._pointC);
}


#endif _MyTriangle_H_