#ifndef _scene_monster_H_
#define _scene_monster_H_

#include"stdafx.h"
#include"scene_fight_object.h"
#include"graphic\GraphRender.h"

class SceneMonster : public SceneFightObject{
public:
	SceneMonster(){}

	static SceneMonster* create();

	virtual bool init();

	void initModel();

private:
	DrawNode* _centerPoint;
	DrawNode* _model;
};


#endif _scene_monster_H_