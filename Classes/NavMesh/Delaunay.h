#ifndef _Delaunay_H_
#define _Delaunay_H_

#include"stdafx.h"
#include"graphic\MyPolygon.h"
#include"graphic\MyTriangle.h"
#include"graphic\MyCircle.h"

/**
* blc
Step1. 	������Ԫ��СΪ E*E �ľ������񣬲�������εĶ���ͱ߷�������.
���� E=sqrt(w*h/n)��w �� h �ֱ�Ϊ��������Χ�еĿ�ȡ��߶ȣ�n Ϊ�������Ķ����� .
Step2.	ȡ����һ����߽�� p1p2 .
Step3. 	���� DT �� p3������Լ�� Delaunay ������ ��p1p2p3 .
Step4.	��������ɵı� p1p3 ����Լ���ߣ����Ѿ��ڶ�ջ�У�
�������ɾ�������򣬽�������ջ�����Ƶأ��ɴ��� p3p2 .
Step5.	����ջ���գ������ȡ��һ���ߣ�תStep3�������㷨ֹͣ .
*/
/**
���ǳ� p3 Ϊ p1p2 �Ŀɼ��㣬�������������
����������
��1�� p3 �ڱ� p1p2 ���Ҳ� (����ζ���˳��Ϊ˳ʱ��)��
��2�� p3 �� p1 �ɼ����� p1p3 �����κ�һ��Լ�����ཻ��
��3�� p3 �� p2 �ɼ����� p1p3 �����κ�һ��Լ�����ཻ��
*/
/**
ȷ�� DT ��Ĺ������£�
Step1. 	���� ��p1p2p3 �����Բ C��p1��p2��p3�����������Χ�� B��C��p1��p2��p3��������ͼ ������ʾ��
Step2.	���η��������Χ���ڵ�ÿ������Ԫ��
��δ����ǰ������ǵ�����Ԫ������������������Ϊ��ǰ����
��ĳ������Ԫ�д��ڿɼ��� p, ���� ��p1pp2 > ��p1p3p2������ p3=p��תStep1��
����תStep3.
Step3. 	����ǰ�����Χ������������Ԫ���ѱ����Ϊ��ǰ������
Ҳ��C��p1��p2��p3�����޿ɼ��㣬�� p3 Ϊ�� p1p2 �� DT ��
*/
class Delaunay{
public:
	Delaunay(){}

	vector<MyTriangle> createDelaunay(const vector<MyPolygon>& polyV);

	/**
	* ��ʼ������
	*/
	void initData(const vector<MyPolygon>& polyV);

	/**
	* ��ȡ��ʼ��߽�
	*/
	Line2D getInitOutEdge();

	/**
	* ��srcV�еĵ����dstV
	*/
	void putVertex(vector<Vector2f>& dstV, const vector<Vector2f>& srcV);

	/**
	* ����srcV�еĵ����ɶ�����߶Σ�������dstV
	*/
	void putEdge(vector<Line2D>& dstV, const vector<Vector2f>& srcV);

	/**
	* �ж��߶��Ƿ���Լ����
	* @param line
	* @return �߶ε����������û���ҵ�������-1
	*/
	int indexOfVector(const Line2D& line, const vector<Line2D>& vec);

	/**
	* ���� DT ��
	*/
	Vector2f* findDT(Line2D& line);

	/**
	* ���ض�����o�㣬��ʼ��Ϊos����ֹ��Ϊoe�ļн�, ����soe (��λ������)
	* �Ƕ�С��pi��������ֵ;   �Ƕȴ���pi�����ظ�ֵ
	*/
	float lineAngle(const Vector2f& s, const Vector2f& o, const Vector2f& e);

	/**
	* ����Բ�İ�Χ��
	*/
	Rect circleBounds(const MyCircle& c);

	/**
	* ���������ε����Բ
	*/
	MyCircle circumCircle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3);

	/**
	* �жϵ�vec�Ƿ�Ϊline�Ŀɼ���
	* @param vec
	* @param line
	* @return true:vec��line�Ŀɼ���
	*/
	bool isVisiblePointOfLine(const Vector2f& vec, Line2D& line) const;

	/**
	* ��pa��pb�Ƿ�ɼ�(pa��pb���ɵ��߶β����κ�Լ�����ཻ������������)
	*/
	bool isVisibleIn2Point(const Vector2f& pa, const Vector2f& pb) const;



private:
	vector<MyPolygon> polygonV; //���ж���Σ���0��Ԫ��Ϊ������߽� (��������)
	vector<Vector2f> vertexV; //���ж����б�, ǰoutEdgeVecNmu��Ϊ��߽綥��
	vector<Line2D> edgeV; //����Լ����

	int outEdgeVecNmu; //������߽綥����

	vector<Line2D> lineV; //�߶ζ�ջ
	
	vector<MyTriangle> triangleV; //���ɵ�Delaunay������
};


#endif _Delaunay_H_