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
	* 是否是简单多边形
	*/
	bool isSimplicity() const;

	/**
	* 将多边形的顶点按逆时针排序
	*/
	void cw();

	/**
	*是否是顺时针
	*返回true -顺时针方向；false -逆时针方向
	*/
	bool isCW();

	/**
	* r=multiply(sp,ep,op),得到(sp-op)*(ep-op)的叉积
	* r>0:ep在矢量opsp的逆时针方向；
	* r=0：opspep三点共线；
	* r<0:ep在矢量opsp的顺时针方向
	*/
	float multiply(const Vector2f& sp, const Vector2f& ep, const Vector2f& op);

	/**
	* 返回矩形包围盒
	*/
	Rect* rectangle();

	/**
	* 合并两个多边形(Weiler-Athenton算法)
	* null--两个多边形不相交，合并前后两个多边形不变
	* Polygon--一个新的多边形
	*/
	vector<MyPolygon>* combination(MyPolygon& polygon);

	/**
	* 生成多边形，顺时针序； 生成的内部孔洞多边形为逆时针序
	* @param cv0
	* @param cv1
	* @return 合并后的结果多边形数组(可能有多个多边形)
	*/
	vector<MyPolygon>* linkToPolygon(vector<MyNode*>& cv0, vector<MyNode*>& cv1);

	/**
	* 生成交点，并按顺时针序插入到顶点表中
	* @param cv0 （in/out）主多边形顶点表，并返回插入交点后的顶点表
	* @param cv1 （in/out）合并多边形顶点表，并返回插入交点后的顶点表
	* @return 交点数
	*/
	int intersectPoint(vector<MyNode*>& cv0, vector<MyNode*>& cv1);

	/**
	* 取得节点的索引(合并多边形用)
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
	vector<Vector2f> vertexV; //顶点列表
	int vertexNum; //顶点数

	vector<DrawNode*> polyLine;
	vector<Text*> polyPointPos;
	DrawNode* polyFill;

private:
	Rect* rectBox; //矩形包围盒
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