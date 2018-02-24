#ifndef _scene_layer_mgr_h_
#define _scene_layer_mgr_h_

#include"DrawLayer.h"

class SceneLayerMgr{
public:
	static SceneLayerMgr* getInstance(){
		static SceneLayerMgr* sceneLayerMgr = new SceneLayerMgr();
		return sceneLayerMgr;
	}

	void Init(Node* rootNode){
		oSceneRoot = Layer::create();
		rootNode->addChild(oSceneRoot);
		
		oLayerMap = Layer::create();
		oLayerBottom = Layer::create();
		oLayerMiddle = Layer::create();
		oLayerTop = Layer::create();

		oSceneRoot->setAnchorPoint(Vec2(0, 0));
		oSceneRoot->addChild(oLayerMap);
		oSceneRoot->addChild(oLayerBottom);
		oSceneRoot->addChild(oLayerMiddle);
		oSceneRoot->addChild(oLayerTop);
	}

	void addMiddle(Node* node){
		oLayerMiddle->addChild(node);
	}

private:
	SceneLayerMgr(){}
	const SceneLayerMgr operator=(const SceneLayerMgr&);

	Layer* oSceneRoot;
	Layer* oLayerMap;
	Layer* oLayerBottom;
	Layer* oLayerMiddle;
	Layer* oLayerTop;
};



#endif _scene_layer_mgr_h_