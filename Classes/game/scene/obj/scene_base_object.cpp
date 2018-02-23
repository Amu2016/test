#include"scene_base_object.h"

SceneBaseObject* SceneBaseObject::create(){
	SceneBaseObject* obj = new SceneBaseObject();
	if (obj && obj->init()){
		obj->autorelease();
	}
	else
		CC_SAFE_DELETE(obj);
	return obj;
}

bool SceneBaseObject::init(){
	if (!Node::init())
		return false;


	return true;
}