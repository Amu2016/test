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
	* ����·��
	* @param startCell
	* @param startPos
	* @param endCell
	* @param endPos
	* @return Point·������
	*/
	vector<Vec2> bulidPath(const Cell* startCell, const Vector2f& startPos, Cell* endCell, const Vector2f& endPos);

	/**
	* ·������������
	* @return
	*/
	vector<Cell*> getCellPath();

	/**
	* ���ݾ����������η���·����(��һ���սǵ㷨)
	* @param start
	* @param end
	* @return Point����
	*/
	vector<Vec2> getPath(const Vec2& start, const Vec2& end);

	/**
	* ��һ���յ�
	* @param wayPoint ��ǰ����·��
	* @param cellPath ����·��
	* @param end �յ�
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