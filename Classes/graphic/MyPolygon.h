#ifndef _MyPolygon_H_
#define _MyPolygon_H_

#include "stdafx.h"
#include "Line2D.h"
//#include "GraphRender.h"

class MyPolygon{
public:
	MyPolygon(){};
	MyPolygon(int Num, vector<Vector2f> V){
		vertexNum = Num;
		vertexV = V;
		rectBox = nullptr;
	}

	~MyPolygon(){
		if (rectBox)
			delete rectBox;
	}

	/**
	* �Ƿ��Ǽ򵥶����
	*/
	bool isSimplicity() const;

	/**
	* ������εĶ��㰴��ʱ������
	*/
	void cw();

	/**
	*�Ƿ���˳ʱ��
	*����true -˳ʱ�뷽��false -��ʱ�뷽��
	*/
	bool isCW();

	/**
	* r=multiply(sp,ep,op),�õ�(sp-op)*(ep-op)�Ĳ��
	* r>0:ep��ʸ��opsp����ʱ�뷽��
	* r=0��opspep���㹲�ߣ�
	* r<0:ep��ʸ��opsp��˳ʱ�뷽��
	*/
	float multiply(const Vector2f& sp, const Vector2f& ep, const Vector2f& op);

	/**
	* ���ؾ��ΰ�Χ��
	*/
	Rect* rectangle();

	/**
	* �ϲ����������(Weiler-Athenton�㷨)
	* null--��������β��ཻ���ϲ�ǰ����������β���
	* Polygon--һ���µĶ����
	*/
	vector<MyPolygon>* combination(MyPolygon& polygon);

	/**
	* ���ɶ���Σ�˳ʱ���� ���ɵ��ڲ��׶������Ϊ��ʱ����
	* @param cv0
	* @param cv1
	* @return �ϲ���Ľ�����������(�����ж�������)
	*/
	vector<MyPolygon>* linkToPolygon(vector<MyNode*>& cv0, vector<MyNode*>& cv1);

	/**
	* ���ɽ��㣬����˳ʱ������뵽�������
	* @param cv0 ��in/out��������ζ���������ز��뽻���Ķ����
	* @param cv1 ��in/out���ϲ�����ζ���������ز��뽻���Ķ����
	* @return ������
	*/
	int intersectPoint(vector<MyNode*>& cv0, vector<MyNode*>& cv1);

	/**
	* ȡ�ýڵ������(�ϲ��������)
	* @param cv
	* @param node
	* @return
	*/
	int getNodeIndex(const vector<MyNode*>& cv, const Vector2f& node);


	void fullMyPolygon(Node* parent);

	void clear();

	inline const MyPolygon operator=(const MyPolygon& v);

	inline bool operator==(const MyPolygon& v) const;

	inline bool operator!=(const MyPolygon& v) const;

public:
	vector<Vector2f> vertexV; //�����б�
	int vertexNum; //������

	vector<DrawNode*> polyLine;
	vector<Text*> polyPointPos;
	DrawNode* polyFill;

private:
	Rect* rectBox; //���ΰ�Χ��
};

inline const MyPolygon MyPolygon::operator=(const MyPolygon& poly){
	this->vertexV = poly.vertexV;
	this->vertexNum = poly.vertexNum;
	this->rectBox = nullptr;
	this->polyLine = poly.polyLine;
	this->polyFill = poly.polyFill;
	return (*this);
}

inline bool MyPolygon::operator == (const MyPolygon& poly) const{
	if (vertexNum != poly.vertexNum)
		return false;
	for (auto v1 : vertexV){
		for (auto v2 : poly.vertexV){
			if (v1 != v2)
				return false;
		}
	}
	return true;
}

inline bool MyPolygon::operator != (const MyPolygon& poly) const{
	for (auto v1 : vertexV){
		for (auto v2 : poly.vertexV){
			if (v1 != v2 && vertexNum != poly.vertexNum)
				return true;
		}
	}
	return false;
}

#endif _MyPolygon_H_