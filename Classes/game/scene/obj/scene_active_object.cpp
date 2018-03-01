#include"scene_active_object.h"

SceneActiveObject* SceneActiveObject::create(){
	SceneActiveObject* obj = new SceneActiveObject();
	if (obj && obj->init()){
		obj->autorelease();
	}
	else
		CC_SAFE_DELETE(obj);
	return obj;
}

bool SceneActiveObject::init(){
	if (!SceneBaseObject::init())
		return false;
	CCLOG("SceneActiveObject::init");
	moveRule = new MoveRule(this);

	return true;
}

void SceneActiveObject::update(float delta){
	SceneBaseObject::update(delta);
	moveRule->update(delta);
}