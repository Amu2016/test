#include"Cell.h"

void Cell::init(){
	links.push_back(-1);
	links.push_back(-1);
	links.push_back(-1);

	calculateData();

	m_WallMidpoint.resize(3);
	m_WallDistance.resize(3);
	m_WallMidpoint[0] = Vector2f((_pointA.x + _pointB.x) / 2.0, (_pointA.y + _pointB.y) / 2.0);
	m_WallMidpoint[1] = Vector2f((_pointC.x + _pointB.x) / 2.0, (_pointC.y + _pointB.y) / 2.0);
	m_WallMidpoint[2] = Vector2f((_pointC.x + _pointA.x) / 2.0, (_pointC.y + _pointA.y) / 2.0);

	Vector2f wallVector;
	wallVector = m_WallMidpoint[0] - m_WallMidpoint[1];
	m_WallDistance[0] = wallVector.length();
	wallVector = m_WallMidpoint[1] - m_WallMidpoint[2];
	m_WallDistance[1] = wallVector.length();
	wallVector = m_WallMidpoint[2] - m_WallMidpoint[0];
	m_WallDistance[2] = wallVector.length();
}

void Cell::checkAndLink(Cell* cellB){
	if (getLink(SIDE_AB) == -1 && cellB->requestLink(_pointA, _pointB, index)) {
		setLink(SIDE_AB, cellB);
	}
	else if (getLink(SIDE_BC) == -1 && cellB->requestLink(_pointB, _pointC, index)) {
		setLink(SIDE_BC, cellB);
	}
	else if (getLink(SIDE_CA) == -1 && cellB->requestLink(_pointC, _pointA, index)) {
		setLink(SIDE_CA, cellB);
	}
}

int Cell::setAndGetArrivalWall(const int index){
	if (index == links[0]) {
		m_ArrivalWall = 0;
		return 0;
	}
	else if (index == links[1]) {
		m_ArrivalWall = 1;
		return 1;
	}
	else if (index == links[2]) {
		m_ArrivalWall = 2;
		return 2;
	}
	return -1;
}

void Cell::computeHeuristic(const Vector2f& goal){
	// our heuristic is the estimated distance (using the longest axis delta) 
	// between our cell center and the goal location

	float XDelta = abs(goal.x - _center.x);
	float YDelta = abs(goal.y - _center.y);

	//			h = Math.max(XDelta, YDelta);
	h = XDelta + YDelta;
}

void Cell::drawIndex(){

}

bool Cell::requestLink(const Vector2f& pA, const Vector2f& pB, const int& index){
	if (_pointA.equals(pA)) {
		if (_pointB.equals(pB)) {
			links[SIDE_AB] = index;
			return true;
		}
		else if (_pointC.equals(pB)) {
			links[SIDE_CA] = index;
			return true;
		}
	}
	else if (_pointB.equals(pA)) {
		if (_pointA.equals(pB)) {
			links[SIDE_AB] = index;
			return true;
		}
		else if (_pointC.equals(pB)) {
			links[SIDE_BC] = index;
			return true;
		}
	}
	else if (_pointC.equals(pA)) {
		if (_pointA.equals(pB)) {
			links[SIDE_CA] = index;
			return true;
		}
		else if (_pointB.equals(pB)) {
			links[SIDE_BC] = index;
			return true;
		}
	}
	return false;
}

void Cell::setLink(const int& side, const Cell* caller){
	links[side] = caller->index;
}

int Cell::getLink(const int& side) const{
	return links[side];
}