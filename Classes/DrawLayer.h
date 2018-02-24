#ifndef _DrawLayer_H_
#define _DrawLayer_H_

#include"graphic\GraphRender.h"
#include"NavMesh\FindPath.h"
#include"game\scene\obj\scene_monster.h"
//#include"NavMesh\Delaunay.h"

class DrawLayer : public cocos2d::LayerColor
{
public:
	enum TouchType
	{
		DELAUNAY,
		FINDPATH,
	};

	static DrawLayer * create(const Color4B& color, GLfloat width, GLfloat height);

	virtual bool initWithColor(const Color4B& color, GLfloat width, GLfloat height);

	void drawPoint(const Vector2f& point);

	Text* setText(const std::string &textContent, const Vector2f& pos, const Color3B& color);

	void unionAll();//�ϲ�

	void linkCells(const vector<Cell*>& pv);

	void changeTouchType(const TouchType& type);

	void clear();

	void clearCellV();

	CREATE_FUNC(DrawLayer);

private:
	EventListenerTouchOneByOne* listener;

	vector<Vector2f> drawPath;
	vector<MyPolygon> polygonV;  //���ж����
	vector<MyPolygon> unionpolygonV;
	vector<MyTriangle> triangleV;  //���ɵ�Delaunay������

	vector<Cell*> cellV;//�����ɵ�Ѱ·����

	vector<DrawNode*> polyLine;
	vector<Text*> polyPointPos;

	Vector2f previousP;

	function<void(Touch* touch, Event* event)> touchCallBack;


	bool startPtSign;
	Vec2 startPot;
	Vec2 endPot;

	GLfloat glwidth;
	GLfloat glheight;

	DrawLayer()
		:startPtSign(false)
	{}
};

#endif _DrawLayer_H_
