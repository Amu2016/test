#ifndef _scene_object_mgr_h_
#define _scene_object_mgr_h_

#include"graphic\GraphRender.h"
#include"game\scene\obj\scene_monster.h"
#include"scene_layer_mgr.h"

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
	typedef function<GameObj()> CreateFun;
public:
	SceneObjFactoryList(CreateFun fun, map<int, GameObj>* list):
		createFun(fun),
		refHashList(list)
	{}

	GameObj CreateObj(const int gid){
		GameObj obj;
		auto iter = sceneObjHashList.find(gid);
		if (iter == sceneObjHashList.end()){
			obj = createFun();
			sceneObjHashList.insert(map<int, GameObj>::value_type(gid, obj));
			refHashList->insert(map<int, GameObj>::value_type(gid, obj));
		}
		else{
			obj = iter->second;
		}
		return obj;
	}

private:
	map<int, GameObj> sceneObjHashList;
	map<int, GameObj>* refHashList;
	CreateFun createFun;
};

class SceneObjectMgr{
public:
	static SceneObjectMgr* getInstance(){
		static SceneObjectMgr* sceneObjectMgr = new SceneObjectMgr();
		return sceneObjectMgr;
	}

	void CreateObject(const GameSceneObjType type, const int gid){
		GameObj obj;
		switch (type)
		{
		case MONSTER:
			obj = oMonsterHashList->CreateObj(gid);
			obj.getObj()->setPosition(100, 200);
			CCLOG("---------------Create   monster----------");
			break;
		case PLAYER:
			break;
		case MAINPLAYER:
			break;
		default:
			break;
		}
		SceneLayerMgr::getInstance()->addMiddle(obj.getObj());
	}

	~SceneObjectMgr(){
		delete refHashList;
		delete oMonsterHashList;
	}

public:
	map<int, GameObj>* refHashList;

	SceneObjFactoryList* oMonsterHashList;

private:
	SceneObjectMgr():
		refHashList(new map<int, GameObj>)
	{
		oMonsterHashList = new SceneObjFactoryList([](){
			GameObj obj = SceneMonster::create();
			return obj;
		}, refHashList);
	}
	const SceneObjectMgr operator=(const SceneObjectMgr&);
	
};

#endif _scene_object_mgr_h_