#ifndef _scene_active_object_H_
#define _scene_active_object_H_

#include"stdafx.h"
#include"scene_base_object.h"

#include"game\scene\fsm\MoveRule.h"

class SceneActiveObject : public SceneBaseObject{
public:
	SceneActiveObject():
		_mass(1.0f),
		_maxSpeed(getRandomFloat(rng, 90.0f, 120.0f)),
		_minSpeed(10.0f),
		_maxForce(100.0f)
	{
		vVelocity = Vec2(
			getRandomFloat(rng, -10.0f, 10.0f),
			getRandomFloat(rng, -10.0f, 10.0f));
		vHeading = vVelocity.getNormalized();
		vSide = vHeading.getPerp();
		monsterNode = Node::create();
		addChild(monsterNode);
	}

	static SceneActiveObject* create();

	virtual bool init();

	virtual void update(float delta);

	~SceneActiveObject(){
		delete moveRule;
	}

public:
	inline void setVelocity(const Vec2& vel){ vVelocity = vel; }
	inline void setHeading(const Vec2& heading){ vHeading = heading; }
	inline void setSide(const Vec2& side){ vSide = side; }
	inline void setMass(const float& mass){ _mass = mass; }
	inline void setMaxSpeed(const float& maxSpeed){ _maxSpeed = maxSpeed; }
	inline void setMinSpeed(const float& minSpeed){ _minSpeed = minSpeed; }
	inline void setMaxForce(const float& maxForce){ _maxForce = maxForce; }
	inline void setMaxTurnRate(const float& maxTurnRate){ _maxTurnRate = maxTurnRate; }

	inline Vec2 getVelocity()const{ return vVelocity; }
	inline Vec2 getHeading()const{ return vHeading; }
	inline Vec2 getSide()const{ return vSide; }
	inline float getMass()const{ return _mass; }
	inline float getMaxSpeed()const{ return _maxSpeed; }
	inline float getMinSpeed()const{ return _minSpeed; }
	inline float getMaxForce()const{ return _maxForce; }
	inline float getMaxTurnRate()const{ return _maxTurnRate; }

protected:
	Node* monsterNode;

private:
	MoveRule* moveRule;


	RngT rng;

	Vec2 vVelocity;//速率 
	Vec2 vHeading;//标准化向量   实体朝向
	Vec2 vSide;//垂直于实体朝向向量

	float _mass; //质量
	float _maxSpeed;//最大速度
	float _minSpeed;//最小速度
	float _maxForce;//最大驱动力
	float _maxTurnRate;//最大转向速率（弧度每秒）
};


#endif _scene_active_object_H_