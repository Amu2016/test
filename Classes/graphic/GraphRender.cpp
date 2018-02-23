#include"GraphRender.h"

//DrawNode* GraphRender::drawPoint(const Vec2& pt, const float pointSize, const Color4F& color){
//	auto point = DrawNode::create();
//	point->drawPoint(pt, pointSize, color);
//	canvas->addChild(point, 10);
//	return point;
//}

DrawNode* GraphRender::drawPoint(Node* parent, const Vec2& pt, const float pointSize, const Color4F& color){
	auto point = DrawNode::create();
	point->drawPoint(pt, pointSize, color);
	parent->addChild(point, 10);
	return point;
}

DrawNode* GraphRender::drawCircle(Node* parent, const Vec2 &center, float radius, const Color4F &color){
	auto circle = DrawNode::create();
	circle->drawCircle(center, radius, (float)M_PI, 0, false, color);
	parent->addChild(circle, 10);
	return circle;
}

DrawNode* GraphRender::drawLine(Node* parent, const Vector2f& sP, const Vector2f& eP, const Color4F& color){
	auto line = DrawNode::create();
	line->drawLine(Vec2(sP.x, sP.y), Vec2(eP.x, eP.y), color);
	parent->addChild(line, 10);
	return line;
}

//DrawNode* GraphRender::drawPolygon(const MyPolygon& polygon, const Color4F& color){
//	
//}

DrawNode* GraphRender::fillPolygon(Node* parent, const MyPolygon& poly, const Color4F& color){

	DrawNode* polygon = DrawNode::create();

	parent->addChild(polygon);

	if (poly.vertexNum > 3){
		Delaunay delaunay;
		vector<MyPolygon> vec;
		vec.push_back(poly);
		reverse(vec[0].vertexV.begin(), vec[0].vertexV.end());
		auto vecTrig = delaunay.createDelaunay(vec);
		for (auto trig : vecTrig){
			fillTriangle(polygon, trig.vertexV, color);
		}
	}
	else
		polygon = fillTriangle(parent, poly.vertexV, color);

	return polygon;
}

void GraphRender::fillTriangle(Node* parent, DrawNode* poly, const vector<Vector2f>& vec, const Color4F &color){
	Vec2* pointArray = new Vec2[vec.size()];
	for (unsigned int i = 0; i < vec.size(); i++){
		pointArray[i] = Vec2(vec[i].x, vec[i].y);
	}
	poly->drawSolidPoly(pointArray, 3, color);
	delete[]pointArray;
}

DrawNode* GraphRender::fillTriangle(Node* parent, const vector<Vector2f>& vec, const Color4F &color){
	Vec2* pointArray = new Vec2[vec.size()];
	for (unsigned int i = 0; i < vec.size(); i++){
		pointArray[i] = Vec2(vec[i].x, vec[i].y);
	}

	auto triangle = DrawNode::create();
	triangle->drawSolidPoly(pointArray, 3, color);
	parent->addChild(triangle);
	delete []pointArray;
	return triangle;
}

