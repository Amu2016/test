#ifndef _scene_fight_object_H_
#define _scene_fight_object_H_

#include"stdafx.h"
#include"scene_active_object.h"

class SceneFightObject : public SceneActiveObject{
public:
	SceneFightObject(){}

	static SceneFightObject* create();

	virtual bool init();

private:

};


#endif _scene_fight_object_H_