#ifndef _Delaunay_H_
#define _Delaunay_H_

#include"stdafx.h"
#include"graphic\MyPolygon.h"
#include"graphic\MyTriangle.h"
#include"graphic\MyCircle.h"

/**
* blc
Step1. 	建立单元大小为 E*E 的均匀网格，并将多边形的顶点和边放入其中.
其中 E=sqrt(w*h/n)，w 和 h 分别为多边形域包围盒的宽度、高度，n 为多边形域的顶点数 .
Step2.	取任意一条外边界边 p1p2 .
Step3. 	计算 DT 点 p3，构成约束 Delaunay 三角形 Δp1p2p3 .
Step4.	如果新生成的边 p1p3 不是约束边，若已经在堆栈中，
则将其从中删除；否则，将其放入堆栈；类似地，可处理 p3p2 .
Step5.	若堆栈不空，则从中取出一条边，转Step3；否则，算法停止 .
*/
/**
我们称 p3 为 p1p2 的可见点，其必须满足下面
三个条件：
（1） p3 在边 p1p2 的右侧 (多边形顶点顺序为顺时针)；
（2） p3 与 p1 可见，即 p1p3 不与任何一个约束边相交；
（3） p3 与 p2 可见，即 p1p3 不与任何一个约束边相交；
*/
/**
确定 DT 点的过程如下：
Step1. 	构造 Δp1p2p3 的外接圆 C（p1，p2，p3）及其网格包围盒 B（C（p1，p2，p3））（如图 虚线所示）
Step2.	依次访问网格包围盒内的每个网格单元：
对未作当前趟数标记的网格单元进行搜索，并将其标记为当前趟数
若某个网格单元中存在可见点 p, 并且 ∠p1pp2 > ∠p1p3p2，则令 p3=p，转Step1；
否则，转Step3.
Step3. 	若当前网格包围盒内所有网格单元都已被标记为当前趟数，
也即C（p1，p2，p3）内无可见点，则 p3 为的 p1p2 的 DT 点
*/
class Delaunay{
public:
	Delaunay(){}

	vector<MyTriangle> createDelaunay(const vector<MyPolygon>& polyV);

	/**
	* 初始化数据
	*/
	void initData(const vector<MyPolygon>& polyV);

	/**
	* 获取初始外边界
	*/
	Line2D getInitOutEdge();

	/**
	* 将srcV中的点放入dstV
	*/
	void putVertex(vector<Vector2f>& dstV, const vector<Vector2f>& srcV);

	/**
	* 根据srcV中的点生成多边形线段，并放入dstV
	*/
	void putEdge(vector<Line2D>& dstV, const vector<Vector2f>& srcV);

	/**
	* 判断线段是否是约束边
	* @param line
	* @return 线段的索引，如果没有找到，返回-1
	*/
	int indexOfVector(const Line2D& line, const vector<Line2D>& vec);

	/**
	* 计算 DT 点
	*/
	Vector2f* findDT(Line2D& line);

	/**
	* 返回顶角在o点，起始边为os，终止边为oe的夹角, 即∠soe (单位：弧度)
	* 角度小于pi，返回正值;   角度大于pi，返回负值
	*/
	float lineAngle(const Vector2f& s, const Vector2f& o, const Vector2f& e);

	/**
	* 返回圆的包围盒
	*/
	Rect circleBounds(const MyCircle& c);

	/**
	* 返回三角形的外接圆
	*/
	MyCircle circumCircle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3);

	/**
	* 判断点vec是否为line的可见点
	* @param vec
	* @param line
	* @return true:vec是line的可见点
	*/
	bool isVisiblePointOfLine(const Vector2f& vec, Line2D& line) const;

	/**
	* 点pa和pb是否可见(pa和pb构成的线段不与任何约束边相交，不包括顶点)
	*/
	bool isVisibleIn2Point(const Vector2f& pa, const Vector2f& pb) const;



private:
	vector<MyPolygon> polygonV; //所有多边形，第0个元素为区域外边界 (输入数据)
	vector<Vector2f> vertexV; //所有顶点列表, 前outEdgeVecNmu个为外边界顶点
	vector<Line2D> edgeV; //所有约束边

	int outEdgeVecNmu; //区域外边界顶点数

	vector<Line2D> lineV; //线段堆栈
	
	vector<MyTriangle> triangleV; //生成的Delaunay三角形
};


#endif _Delaunay_H_