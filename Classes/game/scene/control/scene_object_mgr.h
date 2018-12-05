#ifndef _scene_object_mgr_h_
#define _scene_object_mgr_h_

#include"graphic\GraphRender.h"
#include"game\scene\obj\scene_monster.h"
#include"scene_layer_mgr.h"
#include"net/SocketTcp.h"

union GameObj{
	Node* gameObj;
	SceneMonster* monster;

	GameObj(){}

	const GameObj operator=(SceneMonster* obj){
		monster = obj;
		gameObj = obj;
	}

	GameObj(SceneMonster* obj){
		monster = obj;
		gameObj = obj;
	}

	Node* getObj(){
		return gameObj;
	}
};

class SceneObjFactoryList{
public:
	typedef function<SceneBaseObject*()> CreateFun;
public:
	SceneObjFactoryList(CreateFun fun, map<int, SceneBaseObject*>* list) :
		createFun(fun),
		refHashList(list)
	{}

	SceneBaseObject* CreateObj(const int gid){
		SceneBaseObject* obj;
		auto iter = sceneObjHashList.find(gid);
		if (iter == sceneObjHashList.end()){
			obj = createFun();
			sceneObjHashList.insert(map<int, SceneBaseObject*>::value_type(gid, obj));
			refHashList->insert(map<int, SceneBaseObject*>::value_type(gid, obj));
		}
		else{
			obj = iter->second;
		}
		return obj;
	}

private:
	map<int, SceneBaseObject*> sceneObjHashList;
	map<int, SceneBaseObject*>* refHashList;
	CreateFun createFun;
};

class SceneObjectMgr{
public:
	static SceneObjectMgr* getInstance(){
		static SceneObjectMgr* sceneObjectMgr = new SceneObjectMgr();
		return sceneObjectMgr;
	}

	void CreateObject(const GameSceneObjType type, const int gid){
		SceneBaseObject* obj;
		switch (type)
		{
		case MONSTER:
			obj = oMonsterHashList->CreateObj(gid);
			obj->setPosition(0, 0);
			//SocketTcp::getInstance()->getSocket()->send("---------------Create   monster----------");
			//CCLOG("---------------Create   monster----------");
			break;
		case PLAYER:
			break;
		case MAINPLAYER:
			break;
		default:
			break;
		}
		SceneLayerMgr::getInstance()->addMiddle(obj);
	}

	void update(float delta){
		for (iter = refHashList->begin(); iter != refHashList->end(); iter++){
			iter->second->update(delta);
		}
	}

	~SceneObjectMgr(){
		delete refHashList;
		delete oMonsterHashList;
	}

public:
	map<int, SceneBaseObject*>* refHashList;
	map<int, SceneBaseObject*>::iterator iter;

	SceneObjFactoryList* oMonsterHashList;

private:
	SceneObjectMgr():
		refHashList(new map<int, SceneBaseObject*>)
	{
		oMonsterHashList = new SceneObjFactoryList([](){
			SceneBaseObject* obj = SceneMonster::create();
			return obj;
		}, refHashList);
	}
	const SceneObjectMgr operator=(const SceneObjectMgr&);
	
};

#endif _scene_object_mgr_h_