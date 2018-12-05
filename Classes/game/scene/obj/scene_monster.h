#ifndef _scene_monster_H_
#define _scene_monster_H_

#include"stdafx.h"
#include"scene_fight_object.h"
#include"graphic\GraphRender.h"
#include"game/ui/base/shadow.h"

class SceneMonster : public SceneFightObject{
public:
	SceneMonster():
		pointArray(new Vec2[100]),
		_visibility_polygon(nullptr)
	{}

	static SceneMonster* create();

	virtual bool init();

	virtual void update(float delta);

	void initModel();

	void updateVisiblitily();

private:
	DrawNode* _centerPoint;
	DrawNode* _model;
	
	ClippingNode* clipper;

	vector<DrawNode*> vec_node;
	
	vector<DrawNode*> vec_visibility_polygon;

	DrawNode* _visibility_polygon;

	Shadow* _shadow;

	Vec2* pointArray;
};


#endif _scene_monster_H_