#ifndef _QuadTree_H_
#define _QuadTree_H_

#include"stdafx.h"
#define MAX_OBJECTS 10
#define MAX_DEPTH 5

class MyRect : public Rect{
public:
	Vec2 centroid;

public:
	void SetCentroid(){
		centroid =  Vec2(origin.x + size.width / 2, origin.y + size.height / 2);
	}
};

class QuadTree{
public:
	QuadTree(Rect rect, int depth) :
		bounds(rect),
		depth(depth)
	{}

	/*
	获取物体对应的象限序号，以屏幕中心为界限，切割屏幕:
	- 右上：象限一
	- 左上：象限二
	- 左下：象限三
	- 右下：象限四
	*/
	int getIndex(Rect rect){
		bool onTop;
	}

	// 划分
	void split(){

	}



private:
	struct TreePoint{
		QuadTree *LT, *RT, *LB, *RB;
		TreePoint() :
			LT(nullptr),
			RT(nullptr),
			LB(nullptr),
			RB(nullptr)
		{}
		~TreePoint(){}
	};

	enum{
		
	};

private:
	Rect bounds;
	int depth;
	Node* object;

};

#endif _QuadTree_H_