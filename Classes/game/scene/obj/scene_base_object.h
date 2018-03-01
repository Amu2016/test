#ifndef _scene_base_object_H_
#define _scene_base_object_H_

#include"stdafx.h"

class SceneBaseObject : public Node{
public:
	SceneBaseObject(){}

	static SceneBaseObject* create();

	virtual bool init();

	virtual void update(float delta);

private:

};


#endif _scene_base_object_H_