#include"scene_active_object.h"
#include"game\scene\fsm\MoveRule.h"

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


	return true;
}