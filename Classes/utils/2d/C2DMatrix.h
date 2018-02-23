#pragma once
#ifndef _C2DMatrix_H_
#define _C2DMatrix_H_

#include"stdafx.h"

class C2DMatrix {
public:
	C2DMatrix(){
		matrix._11 = 1;  matrix._12 = 0; matrix._13 = 0;

		matrix._21 = 0;  matrix._22 = 1; matrix._23 = 0;

		matrix._31 = 0;  matrix._32 = 0; matrix._33 = 1;
	}

	inline void Rotate(const Vec2& fwd, const Vec2& side);

	inline void Translate(float x, float y);

	inline void TransformVector2Ds(Vec2& point);

	void _11(double val) { matrix._11 = val; }
	void _12(double val) { matrix._12 = val; }
	void _13(double val) { matrix._13 = val; }

	void _21(double val) { matrix._21 = val; }
	void _22(double val) { matrix._22 = val; }
	void _23(double val) { matrix._23 = val; }

	void _31(double val) { matrix._31 = val; }
	void _32(double val) { matrix._32 = val; }
	void _33(double val) { matrix._33 = val; }


private:
	struct Matrix {
		float _11, _12, _13;
		float _21, _22, _23;
		float _31, _32, _33;

		Matrix()
		{
			_11 = 0.0; _12 = 0.0; _13 = 0.0;
			_21 = 0.0; _22 = 0.0; _23 = 0.0;
			_31 = 0.0; _32 = 0.0; _33 = 0.0;
		}
	};

	Matrix matrix;

	inline void  MatrixMultiply(Matrix &mIn);
};

inline void C2DMatrix::Rotate(const Vec2& fwd, const Vec2& side) {
	C2DMatrix::Matrix mat;

	mat._11 = fwd.x;  mat._12 = fwd.y;  mat._13 = 0;

	mat._21 = side.x; mat._22 = side.y; mat._23 = 0;

	mat._31 = 0;	  mat._32 = 0;		mat._33 = 1;

	//and multiply
	MatrixMultiply(mat);
}

inline void C2DMatrix::Translate(float x, float y) {
	Matrix mat;

	mat._11 = 1; mat._12 = 0; mat._13 = 0;

	mat._21 = 0; mat._22 = 1; mat._23 = 0;

	mat._31 = x; mat._32 = y; mat._33 = 1;

	//and multiply
	MatrixMultiply(mat);
}

inline void C2DMatrix::TransformVector2Ds(Vec2& point) {
	double tempX = (matrix._11*point.x) + (matrix._21*point.y) + (matrix._31);

	double tempY = (matrix._12*point.x) + (matrix._22*point.y) + (matrix._32);

	point.x = tempX;

	point.y = tempY;
}

inline void C2DMatrix::MatrixMultiply(Matrix &mIn)
{
	C2DMatrix::Matrix mat_temp;

	//first row
	mat_temp._11 = (matrix._11*mIn._11) + (matrix._12*mIn._21) + (matrix._13*mIn._31);
	mat_temp._12 = (matrix._11*mIn._12) + (matrix._12*mIn._22) + (matrix._13*mIn._32);
	mat_temp._13 = (matrix._11*mIn._13) + (matrix._12*mIn._23) + (matrix._13*mIn._33);

	//second
	mat_temp._21 = (matrix._21*mIn._11) + (matrix._22*mIn._21) + (matrix._23*mIn._31);
	mat_temp._22 = (matrix._21*mIn._12) + (matrix._22*mIn._22) + (matrix._23*mIn._32);
	mat_temp._23 = (matrix._21*mIn._13) + (matrix._22*mIn._23) + (matrix._23*mIn._33);

	//third
	mat_temp._31 = (matrix._31*mIn._11) + (matrix._32*mIn._21) + (matrix._33*mIn._31);
	mat_temp._32 = (matrix._31*mIn._12) + (matrix._32*mIn._22) + (matrix._33*mIn._32);
	mat_temp._33 = (matrix._31*mIn._13) + (matrix._32*mIn._23) + (matrix._33*mIn._33);

	matrix = mat_temp;
}

#endif // !_C2DMatrix_H_
