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
	/** ����� */
	Vector2f v;
	/** �Ƿ��ǽ��� */
	bool i;
	/** �Ƿ��Ѵ���� */
	bool p;
	/** ����--false�� ����--true */
	bool o;
	/** �����˫������ */
	MyNode* other;
	/** ���Ƿ������������*/
	bool isMain;

	/** ����ε���һ���� */
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
	* �����㵽ֱ�ߵĴ����ž��룬��a�㳯��b�㣬����Ϊ��������Ϊ��
	*/
	float signedDistance(const Vector2f& point);

	/**
	* �жϵ���ֱ�ߵĹ�ϵ��������վ��a�㳯��b�㣬
	* ���������ֱ�ߵĹ�ϵ��Ϊ��Left, Right or Centered on the line
	*/
	int classifyPoint(const Vector2f& point);

	/**
	* �ж�����ֱ�߹�ϵ
	* this line A = x0, y0 and B = x1, y1
	* other is A = x2, y2 and B = x3, y3
	*/
	int intersection(const Line2D& other, Vector2f& pIntersectPoint);

	/**
	* ֱ�߳���
	*/
	float length() const;

	/**
	* ֱ�߷���
	*/
	Vector2f getDirection() const;

	/**
	* ���㷨��
	*/
	void computeNormal();

	/**
	* �߶��Ƿ���� �����Է���
	*/
	bool equals(const Line2D& line) const;

	Line2D clone() const;

	inline const Line2D operator=(const Line2D& v);


public:
	Vector2f m_Normal; //����

	//���߹�ϵ
	/*
	��λ�ڻ򿿽���
	*/
	static const int ON_LINE;
	/*
	�Ӷ˵�A��B�����Ե������
	*/
	static const int LEFT_SIDE;
	/*
	�Ӷ˵�A��B�����Ե����ұ�
	*/
	static const int RIGHT_SIDE;

	//���߹�ϵ
	/*
	����ƽ���һ����ص���
	*/
	static const int COLLINEAR;
	/*
	���ཻ
	*/
	static const int LINES_INTERSECT;
	/*
	���߶�ƽ�ֱ˴�
	*/
	static const int SEGMENTS_INTERSECT;
	/*
	A����B
	*/
	static const int A_BISECTS_B;
	/*
	B����A
	*/
	static const int B_BISECTS_A;
	/*
	��ƽ��
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