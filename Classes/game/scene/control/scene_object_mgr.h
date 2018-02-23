#ifndef _scene_object_mgr_h_
#define _scene_object_mgr_h_

#include"graphic\GraphRender.h"

class SceneObjectMgr{
public:
	static SceneObjectMgr* getInstance(){
		static SceneObjectMgr* sceneObjectMgr = new SceneObjectMgr();
		return sceneObjectMgr;
	}

	void CreateObject();

	

public:

private:
	SceneObjectMgr(){}
	const SceneObjectMgr operator=(const SceneObjectMgr&);

	
};

#endif _scene_object_mgr_h_