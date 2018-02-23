#pragma once
#ifndef _Transformations_H_
#define _Transformations_H_

#include"C2DMatrix.h"


inline Vec2 PointToWorldSpace(const Vec2 &point,
	const Vec2 &AgentHeading,
	const Vec2 &AgentSide,
	const Vec2 &AgentPosition)
{

	Vec2 TransPoint = point;

	C2DMatrix matTransform;

	matTransform.Rotate(AgentHeading, AgentSide);

	matTransform.Translate(AgentPosition.x, AgentPosition.y);

	matTransform.TransformVector2Ds(TransPoint);

	return TransPoint;
}

inline Vec2 VectorToWorldSpace(const Vec2 &vec,
	const Vec2 &AgentHeading,
	const Vec2 &AgentSide)
{
	//make a copy of the point
	Vec2 TransVec = vec;

	//create a transformation matrix
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	//now transform the vertices
	matTransform.TransformVector2Ds(TransVec);

	return TransVec;
}

inline Vec2 PointToLocalSpace(const Vec2 &point,
	const Vec2 &AgentHeading,
	const Vec2 &AgentSide,
	const Vec2 &AgentPosition)
{

	//make a copy of the point
	Vec2 TransPoint = point;

	//create a transformation matrix
	C2DMatrix matTransform;

	double Tx = -AgentPosition.dot(AgentHeading);
	double Ty = -AgentPosition.dot(AgentSide);

	//create the transformation matrix
	matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	matTransform._31(Tx);           matTransform._32(Ty);

	//now transform the vertices
	matTransform.TransformVector2Ds(TransPoint);

	return TransPoint;
}

#endif // !_Transformations_H_
