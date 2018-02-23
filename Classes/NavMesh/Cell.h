#ifndef _Cell_H_
#define _Cell_H_

#include"stdafx.h"
#include"graphic\MyTriangle.h"

class Cell : public MyTriangle{
public:
	Cell(){};
	
	Cell(Vector2f p1, Vector2f p2, Vector2f p3)
		:MyTriangle(p1, p2, p3),
		isOpen(false),
		parent(nullptr),
		m_ArrivalWall(0),
		sessionId(-1)
	{
		init();
	};

	~Cell(){};

	//vector<int> getLinks(){ return _links; }
	//void setLinks(const vector<int>& value){ _links = value; }

	void init();

	/**
	* 检查并设置当前三角型与cellB的连接关系（方法会同时设置cellB与该三角型的连接）
	* @param cellB
	*/
	void checkAndLink(Cell* cellB);

	/**
	* 记录路径从上一个节点进入该节点的边（如果从终点开始寻路即为穿出边）
	* @param index	路径上一个节点的索引
	*/
	int setAndGetArrivalWall(const int index);

	/**
	* 计算估价（h）  Compute the A* Heuristic for this cell given a Goal point
	* @param goal
	*/
	void computeHeuristic(const Vector2f& goal);

	/**
	* 测试直线与该cell（三角形）的关系
	* @param motionPath
	* @return ClassifyResult对象
	*/
	//ClassifyResult classifyPathToCell(const Line2D& motionPath);

	/**
	* 绘制网格索引
	* @param sp
	*/
	void drawIndex();

public:
	int sessionId;

	int index;  //在数组中的索引值
	vector<int> links;   // 与该三角型连接的三角型索引， -1表示改边没有连接

	int f; //f(n)=g(n)+h(n)--- g(n)表示从起点到任意节点n的路径花费
	int h; //h(n)表示从节点n到目标节点路径花费的估计值（启发值）
	bool isOpen;
	Cell* parent;
	int m_ArrivalWall; //我们到达的那一边。
	vector<Vector2f> m_WallMidpoint; // 每个边的中点
	vector<float> m_WallDistance; //各墙中点之间的距离（0-1，1-2，2-0）

private:
	/**
	* 获得两个点的相邻三角型
	* @param pA
	* @param pB
	* @param caller
	* @return 如果提供的两个点是caller的一个边, 返回true
	*/
	bool requestLink(const Vector2f& pA, const Vector2f& pB, const int& index);
	/**
	* 设置side指定的边的连接到caller的索引
	* @param side
	* @param caller
	*/
	void setLink(const int& side, const Cell* caller);
	/**
	* 取得指定边的相邻三角型的索引
	* @param side
	* @return
	*/
	int getLink(const int& side) const;

private:
};

#endif _Cell_H_