#ifndef _MoveRule_H_
#define _MoveRule_H_

#include"stdafx.h"
#include"game\scene\fsm\State.h"
#include"utils\2d\Transformations.h"
#include"game\scene\control\scene_map_mgr.h"

class SceneActiveObject;

class MoveRule{
public:
	MoveRule(SceneActiveObject* obj):
		seed(random_device()()),
		_object(obj),
		_state(GlobalState::WANDER),
		wanderJitter(30.0f),
		wanderRadius(110.0f),
		wanderDistance(120.0f),
		rng(RngT(random_device()())),
		_mass(1.0f),
		//_maxSpeed(getRandomFloat(rng, 90.0f, 120.0f)),
		_maxSpeed(120.0f),
		_minSpeed(10.0f),
		_maxForce(100.0f),
		_maxTurnRate(10.0f)
	{
		_vVelocity = Vec2(
			getRandomFloat(rng, -10.0f, 10.0f),
			getRandomFloat(rng, -10.0f, 10.0f));
		_vHeading = _vVelocity.getNormalized();
		_vSide = _vHeading.getPerp();
	}

	Vec2 Calculate();

	void setWarmState(const GlobalState& state) { _state = state; }
	GlobalState getWarmState() const{ return _state; }

	Vec2 Seek();

	Vec2 Seek(const Vec2& vecPos);

	Vec2 Flee();

	Vec2 Arrive(const Vec2& tragetPos);

	Vec2 Pursuit();

	Vec2 Evade();

	Vec2 Wander();

	Vec2 ObstacleAvoidance();

	Vec2 FollowPath();

	void update(float delta);


	~MoveRule(){}

public:
	inline void setVelocity(const Vec2& vel){ _vVelocity = vel; }
	inline void setHeading(const Vec2& heading){ _vHeading = heading; }
	inline void setSide(const Vec2& side){ _vSide = side; }
	inline void setMass(const float& mass){ _mass = mass; }
	inline void setMaxSpeed(const float& maxSpeed){ _maxSpeed = maxSpeed; }
	inline void setMinSpeed(const float& minSpeed){ _minSpeed = minSpeed; }
	inline void setMaxForce(const float& maxForce){ _maxForce = maxForce; }
	inline void setMaxTurnRate(const float& maxTurnRate){ _maxTurnRate = maxTurnRate; }

	inline Vec2 getVelocity()const{ return _vVelocity; }
	inline Vec2 getHeading()const{ return _vHeading; }
	inline Vec2 getSide()const{ return _vSide; }
	inline float getMass()const{ return _mass; }
	inline float getMaxSpeed()const{ return _maxSpeed; }
	inline float getMinSpeed()const{ return _minSpeed; }
	inline float getMaxForce()const{ return _maxForce; }
	inline float getMaxTurnRate()const{ return _maxTurnRate; }

private:
	SceneActiveObject* _object;// ���Ƶ�������

	int seed;
	GlobalState _state;
	RngT rng;

	Vec2 _vVelocity;//���� 
	Vec2 _vHeading;//��׼������   ʵ�峯��
	Vec2 _vSide;//��ֱ��ʵ�峯������

	float _mass; //����
	float _maxSpeed;//����ٶ�
	float _minSpeed;//��С�ٶ�
	float _maxForce;//���������
	float _maxTurnRate;//���ת�����ʣ�����ÿ�룩

	//---------Wander-----------
	Vec2			wanderTarget;//�ǻ���Ŀ���
	float			wanderJitter;//ÿ��ӵ�Ŀ�����������
	float			wanderRadius;//wanderȦ�İ뾶
	float			wanderDistance;//wanderȦ��������ǰ��ľ���(һ����ڰ뾶)
	//--------------------------

	//----ObstacleAvoidance-----
	float			boxLength;
	float			boxHeight;
	//--------------------------
};

#endif _MoveRule_H_