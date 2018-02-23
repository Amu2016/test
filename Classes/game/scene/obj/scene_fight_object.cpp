#include"scene_fight_object.h"

SceneFightObject* SceneFightObject::create(){
	SceneFightObject* obj = new SceneFightObject();
	if (obj && obj->init()){
		obj->autorelease();
	}
	else
		CC_SAFE_DELETE(obj);
	return obj;
}

bool SceneFightObject::init(){
	if (!SceneActiveObject::init())
		return false;


	return true;
}