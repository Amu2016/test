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
	CCLOG("SceneMonster::init");
	initModel();

	return true;
}

void SceneMonster::update(float delta){
	//CCLOG("SceneMonster::update");
	SceneFightObject::update(delta);
}

void SceneMonster::initModel(){
	_centerPoint = GraphRender::getInstance()->drawPoint(monsterNode, Vec2(0, 0), 5, Color4F::BLACK);
	vector<Vector2f> trigPoint{ Vector2f(0, -20), Vector2f(-14, 10), Vector2f(14, 10) };
	_model = GraphRender::getInstance()->fillTriangle(monsterNode, trigPoint, Color4F::BLACK);
}