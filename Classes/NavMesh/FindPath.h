#ifndef _FindPath_H_
#define _FindPath_H_

#include"stdafx.h"
#include"NavMesh\Cell.h"
#include"utils\MyHeap.h"

struct WayPoint{
	WayPoint(Cell* c, const Vector2f& v){
		cell = c;
		position = v;
	}
	Cell* cell;
	Vector2f position;
};

class FindPath{
public:
	FindPath(const vector<Cell*> cellVector){
		m_CellVector = cellVector;
		openList = MyHeap<Cell>(m_CellVector.size(), [](const Cell* a, const Cell* b){
			return b->f - a->f;
		});
		//closeList = new Cell[m_CellVector.size()];
	}
	~FindPath(){}

	Cell* findClosestCell(const Vector2f& pt);

	vector<Vec2> find(const Vec2& startPoint, const Vec2& endPoint);

	/**
	* 构建路径
	* @param startCell
	* @param startPos
	* @param endCell
	* @param endPos
	* @return Point路径数组
	*/
	vector<Vec2> bulidPath(const Cell* startCell, const Vector2f& startPos, Cell* endCell, const Vector2f& endPos);

	/**
	* 路径经过的网格
	* @return
	*/
	vector<Cell*> getCellPath();

	/**
	* 根据经过的三角形返回路径点(下一个拐角点法)
	* @param start
	* @param end
	* @return Point数组
	*/
	vector<Vec2> getPath(const Vec2& start, const Vec2& end);

	/**
	* 下一个拐点
	* @param wayPoint 当前所在路点
	* @param cellPath 网格路径
	* @param end 终点
	* @return
	*/
	WayPoint getFurthestWayPoint(const WayPoint& wayPoint, const vector<Cell*>& cellPath, const Vector2f& end);

	int indexOf(const vector<Cell*>& cellPath, const Cell* cell);

private:
	FindPath(){};
private:
	static int pathSessionId;
	vector<Cell*> m_CellVector;
	MyHeap<Cell> openList;
	vector<Cell*> closeList;

};

#endif _FindPath_H_