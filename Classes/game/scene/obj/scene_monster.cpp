#include"scene_monster.h"

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

	initModel();

	return true;
}

void SceneMonster::initModel(){
	_centerPoint = GraphRender::getInstance()->drawPoint(this, Vec2(0, 0), 5, Color4F::BLACK);
	vector<Vector2f> trigPoint{ Vector2f(0, 20), Vector2f(-17, -10), Vector2f(17, -10) };
	_model = GraphRender::getInstance()->fillTriangle(this, trigPoint, Color4F::BLACK);
}