#ifndef _GRAPHRENDER_H_
#define _GRAPHRENDER_H_

#include"stdafx.h"
#include"NavMesh\Delaunay.h"

class Delaunay;
class GraphRender{
public:

	static GraphRender* getInstance(){
		static GraphRender* render = new GraphRender();
		return render;
	}

	//void setCanvas(Node* node){
	//	canvas = node;
	//}

	//DrawNode* drawPoint(const Vec2& pt, const float pointSize, const Color4F& color);

	DrawNode* drawPoint(Node* parent, const Vec2& pt, const float pointSize, const Color4F& color);

	DrawNode* drawCircle(Node* parent, const Vec2 &center, float radius, const Color4F &color);

	DrawNode* drawLine(Node* parent, const Vector2f& sP, const Vector2f& eP, const Color4F& color);
	//DrawNode* drawPolygon(const MyPolygon& polygon, const Color4F& color);
	DrawNode* fillPolygon(Node* parent, const MyPolygon& poly, const Color4F& color);

	void fillTriangle(DrawNode* poly, const vector<Vector2f>& vec, const Color4F &color);

	void drawPolygon(DrawNode* poly, const vector<Vector2f>& vec, const Color4F &color);

	DrawNode* fillTriangle(Node* parent, const vector<Vector2f>& vec, const Color4F &color);

private:
	GraphRender():
		pointArray(new Vec2[100])
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
	};

	const GraphRender operator=(const GraphRender&);

	~GraphRender();

private:
	//Node* canvas;
	Vec2 * pointArray;
};

#endif _GRAPHRENDER_H_