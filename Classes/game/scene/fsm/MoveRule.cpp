#include"MoveRule.h"
#include"game\scene\obj\scene_active_object.h"

Vec2 MoveRule::Calculate(){
	Vec2 force(0.0f, 0.0f);
	switch (_state)
	{
	case SEEK:
		force = Seek();
		force += ObstacleAvoidance();
		break;
	case FLEE:
		force = Flee();
		force += ObstacleAvoidance();
		break;
	case WANDER:
		force = Wander();
		//force += ObstacleAvoidance();
		break;
	default:
		break;
	}
	return force;
}

Vec2 MoveRule::Seek(){
	return Vec2();
}

Vec2 MoveRule::Seek(const Vec2& vecPos){
	return Vec2();
}

Vec2 MoveRule::Flee(){
	return Vec2();
}

Vec2 MoveRule::Arrive(const Vec2& tragetPos){
	return Vec2();
}

Vec2 MoveRule::Pursuit(){
	return Vec2();
}

Vec2 MoveRule::Evade(){
	return Vec2();
}

Vec2 MoveRule::Wander(){
	wanderTarget += Vec2(randomClamped(rng)*wanderJitter,
						 randomClamped(rng)*wanderJitter);
	wanderTarget.normalize();
	wanderTarget *= wanderRadius;
	Vec2 targetLocal = wanderTarget + Vec2(wanderDistance, 0);
	Vec2 targetParent = PointToWorldSpace(
		targetLocal,
		_object->getHeading(),
		_object->getSide(),
		_object->getPosition()
		);
	return targetParent - _object->getPosition();
}

Vec2 MoveRule::ObstacleAvoidance(){
	Vec2 objPos = _object->getPosition();




	return Vec2();
}

Vec2 MoveRule::FollowPath(){
	return Vec2();
}

void MoveRule::updata(float delta){
	Vec2 accelerartion = Calculate() / _mass;
	_vVelocity += accelerartion*delta;

	if (_vVelocity.length() > _maxSpeed) {
		_vVelocity.normalize();
		_vVelocity *= _maxSpeed;
	}
	Vec2 newposition = _object->getPosition() + _vVelocity * delta;

	if (newposition.x > MapMgr::getInstance()->glwidth)
		newposition -= Vec2(MapMgr::getInstance()->glwidth, 0);
	if (newposition.x < 0)
		newposition += Vec2(MapMgr::getInstance()->glwidth, 0);

	if (newposition.y > MapMgr::getInstance()->glheight)
		newposition -= Vec2(0, MapMgr::getInstance()->glheight);
	if (newposition.y < 0)
		newposition += Vec2(0, MapMgr::getInstance()->glheight);

	_object->setPosition(newposition);

	if (_vVelocity.length() > 0.000001f) {
		_vHeading = _vVelocity.getNormalized();
		_vSide = _vHeading.getPerp();
	}

	_object->setRotation(-(CC_RADIANS_TO_DEGREES(_vHeading.getAngle()) + 90.0f));
}