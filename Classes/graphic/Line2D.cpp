#include"Line2D.h"

const int Line2D::ON_LINE = 0;
const int Line2D::LEFT_SIDE = 1;
const int Line2D::RIGHT_SIDE = 2;

const int Line2D::COLLINEAR = 3;
const int Line2D::LINES_INTERSECT = 4;
const int Line2D::SEGMENTS_INTERSECT = 5;
const int Line2D::A_BISECTS_B = 6;
const int Line2D::B_BISECTS_A = 7;
const int Line2D::PARALELL = 8;


Vector2f Line2D::getNormal(){
	if (!m_NormalCalculated)
		computeNormal();
	return m_Normal;
}

float Line2D::signedDistance(const Vector2f& point){
	if (!m_NormalCalculated)
		computeNormal();
	Vector2f v2f = point - pointA;
	return v2f.dot(m_Normal);
}

int Line2D::classifyPoint(const Vector2f& point){
	float dis = signedDistance(point);
	if (dis > 0.0000001f)
		return RIGHT_SIDE;
	else if (dis < -0.0000001f)
		return LEFT_SIDE;
	return ON_LINE;
}

int Line2D::intersection(const Line2D& other, Vector2f& pIntersectPoint){
	float denom = (other.pointB.y - other.pointA.y)*(pointB.x - pointA.x) - (other.pointB.x - other.pointA.x)*(pointB.y - pointA.y);
	float u0 = (other.pointB.x - other.pointA.x)*(pointA.y - other.pointA.y) - (other.pointB.y - other.pointA.y)*(pointA.x - other.pointA.x);
	float u1 = (other.pointA.x - pointA.x)*(pointB.y - pointA.y) - (other.pointA.y - pointA.y)*(pointB.x - pointA.x);

	if (denom == 0.0f){ //如果平行
		if (u0 == 0.0f && u1 == 0.0f)//如果共线
			return COLLINEAR;
		else
			return PARALELL;
	}
	else{ //检查它们是否相交
		u0 = u0 / denom;
		u1 = u1 / denom;
		float x1 = pointA.x + u0*(pointB.x - pointA.x);
		float y1 = pointA.y + u0*(pointB.y - pointA.y);
		
		if (&pIntersectPoint != nullptr){
			pIntersectPoint.x = x1;//(m_PointA.x + (FactorAB * Bx_minus_Ax));
			pIntersectPoint.y = y1;//(m_PointA.y + (FactorAB * By_minus_Ay));
		}

		//确定相交的类型
		if ((u0 >= 0.0f) && (u0 <= 1.0f) && (u1 >= 0.0f) && (u1 <= 1.0f))
			return SEGMENTS_INTERSECT;
		else if ((u1 >= 0.0f) && (u1 <= 1.0f))
			return A_BISECTS_B; 
		else if ((u0 >= 0.0f) && (u0 <= 1.0f))
			return B_BISECTS_A;
		else
			return LINES_INTERSECT;
	}
}

float Line2D::length() const{
	Vector2f pt = pointB - pointA;
	return pt.length();
}

Vector2f Line2D::getDirection() const{
	Vector2f pt = pointB - pointA;
	pt.normalize();
	return pt;
}

void Line2D::computeNormal(){
	Vector2f dir = getDirection();
	m_Normal.y = dir.x;
	m_Normal.x = -dir.y;
	m_NormalCalculated = true;
}

bool Line2D::equals(const Line2D& line) const{
	return (pointA.equals(line.getPointA()) && pointB.equals(line.getPointB())) ||
		(pointB.equals(line.getPointA()) && pointA.equals(line.getPointB()));
}

Line2D Line2D::clone() const{
	return Line2D(pointA, pointB);
}