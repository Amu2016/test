#ifndef _scene_active_object_H_
#define _scene_active_object_H_

#include"stdafx.h"
#include"scene_base_object.h"

class MoveRule;

class SceneActiveObject : public SceneBaseObject{
public:
	SceneActiveObject(){}

	static SceneActiveObject* create();

	virtual bool init();



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

private:
	RngT rng;

	Vec2 vVelocity;//���� 
	Vec2 vHeading;//��׼������   ʵ�峯��
	Vec2 vSide;//��ֱ��ʵ�峯������

	float _mass; //����
	float _maxSpeed;//����ٶ�
	float _minSpeed;//��С�ٶ�
	float _maxForce;//���������
	float _maxTurnRate;//���ת�����ʣ�����ÿ�룩
};


#endif _scene_active_object_H_