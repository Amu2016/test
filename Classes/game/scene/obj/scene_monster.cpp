#include"scene_monster.h"
#include <visibility/visibility.hpp>
#include"game\scene\control\scene_layer_mgr.h"

using vector_type = geometry::vec2;
using segment_type = geometry::line_segment<vector_type>;

SceneMonster* SceneMonster::create(){
	SceneMonster* obj = new SceneMonster();
	if (obj && obj->init()){
		obj->autorelease();
	}
	else
		CC_SAFE_DELETE(obj);
	return obj;
}

bool SceneMonster::init(){
	if (!SceneFightObject::init())
		return false;
	//CCLOG("SceneMonster::init");
	initModel();

	for (int i = 0; i < 50; i++) {
		vec_node.push_back(nullptr);
	}

	for (int i = 0; i < 50; i++) {
		vec_visibility_polygon.push_back(nullptr);
	}
	

	_visibility_polygon = DrawNode::create();
	SceneLayerMgr::getInstance()->addMiddle(_visibility_polygon);

	_shadow = Shadow::create();
	SceneLayerMgr::getInstance()->addMiddle(_shadow);

	//DrawNode* stencil = DrawNode::create();
	//stencil->drawSolidRect(Vec2(0, 0), Vec2(868, 680), Color4F::BLACK);
	//SceneLayerMgr::getInstance()->addMiddle(stencil);

	//DrawNode* node = DrawNode::create();
	//node->drawTriangle(Vec2(0, 0), Vec2(0, 680), Vec2(868, 0), Color4F::BLACK);

	//clipper = ClippingNode::create();
	//clipper->setStencil(node);
	//clipper->setInverted(false);
	//stencil->addChild(clipper);
	////clipper->addChild(node);

	return true;
}

void SceneMonster::update(float delta){
	//CCLOG("SceneMonster::update");
	SceneFightObject::update(delta);
	updateVisiblitily();
}

void SceneMonster::initModel(){
	_centerPoint = GraphRender::getInstance()->drawPoint(monsterNode, Vec2(0, 0), 5, Color4F::RED);
	vector<Vector2f> trigPoint{ Vector2f(0, -20), Vector2f(-14, 10), Vector2f(14, 10) };
	_model = GraphRender::getInstance()->fillTriangle(monsterNode, trigPoint, Color4F::BLACK);
}

void SceneMonster::updateVisiblitily() {
	auto mapPolygon = MapMgr::getInstance()->getPolygon();

	if (mapPolygon == nullptr ||mapPolygon->size() == 0)
		return;

	std::vector<segment_type> test;
	for (auto poly : *mapPolygon) {
		for (auto node = poly.vertexV.begin(); node != poly.vertexV.end(); node++) {
			auto nextNode = node + 1;
			if (nextNode != poly.vertexV.end()) {
				segment_type line{ { node->x, node->y },{ nextNode->x, nextNode->y } };
				test.push_back(line);
			}
			else {
				segment_type line{ { node->x, node->y },{ poly.vertexV.begin()->x, poly.vertexV.begin()->y } };
				test.push_back(line);
			}
		}
	}

	auto points = visibility_polygon(vector_type{ this->getPosition().x, this->getPosition().y }, test.begin(), test.end());

	//for (auto point : points) {
	//	GraphRender::getInstance()->drawLine(SceneLayerMgr::getInstance()->getLayerMap(), 
	//		Vector2f(this->getPosition().x, this->getPosition().y),
	//		Vector2f(point.x, point.y), Color4F::GRAY);
	//}
	//_visibility_polygon->clear();
	_shadow->clear();

	//_visibility_polygon->drawSolidRect(Vec2(0,0), Vec2(868, 680), Color4F::BLACK);

	//_shadow->drawTriangle(
	//	Vec2(0, 0), Vec2(0, 680), Vec2(868, 0),
	//	Color4F::BLACK);
	//_shadow->drawTriangle(
	//	Vec2(868, 680), Vec2(0, 680), Vec2(868, 0),
	//	Color4F::BLACK);

	int i = 0;
	for (; i < points.size(); i++) {

		//if (i < points.size() - 1)
		//	GraphRender::getInstance()->fillTriangle(_visibility_polygon,
		//		{ Vector2f(this->getPosition().x, this->getPosition().y), Vector2f(points[i].x, points[i].y), Vector2f(points[i + 1].x, points[i + 1].y) },
		//		Color4F(1, 1, 1, 1));
		//else
		//	GraphRender::getInstance()->fillTriangle(_visibility_polygon,
		//		{ Vector2f(this->getPosition().x, this->getPosition().y), Vector2f(points[i].x, points[i].y), Vector2f(points[0].x, points[0].y) },
		//		Color4F(1, 1, 1, 1));
		if (i < points.size() - 1)
			_shadow->drawTriangle(
				Vec2(this->getPosition().x, this->getPosition().y), Vec2(points[i].x, points[i].y), Vec2(points[i + 1].x, points[i + 1].y),
				Color4F::WHITE);
		else
			_shadow->drawTriangle(
				Vec2(this->getPosition().x, this->getPosition().y), Vec2(points[i].x, points[i].y), Vec2(points[0].x, points[0].y),
				Color4F::WHITE);
		//path.push_back(Vector2f(points[i].x, points[i].y));
	}
	//i = 0;
	//for (; i < points.size(); i++) {
	//	if (vec_node[i] == nullptr) {
	//		auto line = DrawNode::create();
	//		SceneLayerMgr::getInstance()->addLayerMap(line);
	//		vec_node[i] = line;
	//	}
	//	vec_node[i]->clear();
	//	vec_node[i]->drawLine(Vec2(this->getPosition().x, this->getPosition().y), Vec2(points[i].x, points[i].y), Color4F::YELLOW);
	//	//path.push_back(Vector2f(points[i].x, points[i].y));
	//}
	//for (; i < vec_node.size(); i++) {
	//	if (vec_node[i] == nullptr)
	//		break;
	//	vec_node[i]->clear();
	//}

	//MyPolygon ploy(path.size(), path);

	//if (ploy.vertexNum > 3) {
	//	Delaunay delaunay;
	//	vector<MyPolygon> vec;
	//	vec.push_back(ploy);
	//	reverse(vec[0].vertexV.begin(), vec[0].vertexV.end());
	//	auto vecTrig = delaunay.createDelaunay(vec);
	//	for (auto trig : vecTrig) {
	//		GraphRender::getInstance()->fillTriangle(_visibility_polygon, trig.vertexV, Color4F::YELLOW);
	//	}
	//}
}