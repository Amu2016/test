#ifndef _Line2D_H_
#define _Line2D_H_

#include"Vector2f.h"

class MyNode{
public:
	MyNode(){}
	MyNode(const Vector2f& point, bool isInters, bool main){
		v = point;
		i = isInters;
		isMain = main;
		other = nullptr;
		next = nullptr;
		p = false;
		o = false;
	}

	void setData(const Vector2f& point, bool isInters, bool main){
		v = point;
		i = isInters;
		isMain = main;
		other = nullptr;
		next = nullptr;
		p = false;
		o = false;
	}

	~MyNode(){}
public:
	/** 坐标点 */
	Vector2f v;
	/** 是否是交点 */
	bool i;
	/** 是否已处理过 */
	bool p;
	/** 进点--false； 出点--true */
	bool o;
	/** 交点的双向引用 */
	MyNode* other;
	/** 点是否在主多边形中*/
	bool isMain;

	/** 多边形的下一个点 */
	MyNode* next;
};

class Line2D{
public:

	Line2D(){
		m_NormalCalculated = false;
	}

	Line2D(const Vector2f& point1, const Vector2f& point2){
		pointA = point1;
		pointB = point2;
		m_NormalCalculated = false;
	}

	Line2D(const Line2D& line){
		pointA = line.pointA;
		pointB = line.pointB;
		m_NormalCalculated = false;
	}

	void setPointA(const Vector2f& point){
		pointA = point;
		m_NormalCalculated = false;
	}
	void setPointB(const Vector2f& point){
		pointB = point;
		m_NormalCalculated = false;
	}
	Vector2f getPointA() const{
		return pointA;
	}
	Vector2f getPointB() const{
		return pointB;
	}
	void setPoints(const Vector2f& point1, const Vector2f& point2){
		pointA = point1;
		pointB = point2;
		m_NormalCalculated = false;
	}

	Vector2f getNormal();

	/**
	* 给定点到直线的带符号距离，从a点朝向b点，右向为正，左向为负
	*/
	float signedDistance(const Vector2f& point);

	/**
	* 判断点与直线的关系，假设你站在a点朝向b点，
	* 则输入点与直线的关系分为：Left, Right or Centered on the line
	*/
	int classifyPoint(const Vector2f& point);

	/**
	* 判断两个直线关系
	* this line A = x0, y0 and B = x1, y1
	* other is A = x2, y2 and B = x3, y3
	*/
	int intersection(const Line2D& other, Vector2f& pIntersectPoint);

	/**
	* 直线长度
	*/
	float length() const;

	/**
	* 直线方向
	*/
	Vector2f getDirection() const;

	/**
	* 计算法线
	*/
	void computeNormal();

	/**
	* 线段是否相等 （忽略方向）
	*/
	bool equals(const Line2D& line) const;

	Line2D clone() const;

	inline const Line2D operator=(const Line2D& v);


public:
	Vector2f m_Normal; //法线

	//点线关系
	/*
	点位于或靠近线
	*/
	static const int ON_LINE;
	/*
	从端点A到B，测试点在左边
	*/
	static const int LEFT_SIDE;
	/*
	从端点A到B，测试点在右边
	*/
	static const int RIGHT_SIDE;

	//线线关系
	/*
	两行平行且互相重叠。
	*/
	static const int COLLINEAR;
	/*
	线相交
	*/
	static const int LINES_INTERSECT;
	/*
	两线段平分彼此
	*/
	static const int SEGMENTS_INTERSECT;
	/*
	A穿过B
	*/
	static const int A_BISECTS_B;
	/*
	B穿过A
	*/
	static const int B_BISECTS_A;
	/*
	线平行
	*/
	static const int PARALELL;

private:
	Vector2f pointA;
	Vector2f pointB;
	bool m_NormalCalculated;
};

inline const Line2D Line2D::operator=(const Line2D& v){
	this->pointA = v.pointA;
	this->pointB = v.pointB;
	this->m_Normal = v.m_Normal;
	this->m_NormalCalculated = v.m_NormalCalculated;
	return (*this);
}

#endif _Line2D_H_